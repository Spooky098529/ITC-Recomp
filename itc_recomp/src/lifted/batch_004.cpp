// Lift sweep batch 004 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/accessories.h"
#include "schema/app_states.h"
#include "schema/car_draw.h"
#include "schema/car_physics.h"
#include "schema/car_specs.h"
#include "schema/collision.h"
#include "schema/device_shaders.h"
#include "schema/models.h"
#include "schema/movies.h"
#include "schema/race_event.h"
#include "schema/scene.h"
#include "schema/scene_loaders.h"
#include "schema/scenery_lists.h"
#include "schema/vehicle_accessors.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_004, true, "ITC",
              "Run lift sweep batch 004's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_820ACD40: lifted from generated/ by lift.py.
lifted::Result f820ACD40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820ACD40 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r11,r4,4
  r11 = uint64_t(int64_t(int64_t(r4) + 4));
  // mr r4,r3
  r4 = r3;
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,16(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 16);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820ACE80: lifted from generated/ by lift.py.
lifted::Result f820ACE80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820ACE80 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // addi r11,r4,4
  r11 = uint64_t(int64_t(int64_t(r4) + 4));
  // stw r4,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r4));
  // mr r4,r3
  r4 = r3;
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820ACEAC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,124
  r3 = uint64_t(int64_t(int64_t(r1) + 124));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820ACEC8: lifted from generated/ by lift.py.
lifted::Result f820ACEC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820ACEC8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r11,r4
  r11 = r4;
  // mr r4,r3
  r4 = r3;
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r11,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r11));
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820ACEF4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,124
  r3 = uint64_t(int64_t(int64_t(r1) + 124));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820AD188: lifted from generated/ by lift.py.
lifted::Result f820AD188(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AD188 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x820ad350
  { const lifted::Result o = f820AD350(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820AD190: lifted from generated/ by lift.py.
lifted::Result f820AD190(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820AD190 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,10464
  r11 = uint64_t(int64_t(int64_t(r11) + 10464));
  // addi r10,r10,10416
  r10 = uint64_t(int64_t(int64_t(r10) + 10416));
  // addi r30,r31,36
  r30 = uint64_t(int64_t(int64_t(r31) + 36));
  // mr r3,r30
  r3 = r30;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::StateLayer(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, schema::StateLayer(base, uint32_t(r31)).state_vtable_address(), uint32_t(r10));
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x820AD1C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r30) - 36).ops_head_address());
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820AD1CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // addi r29,r31,24
  r29 = uint64_t(int64_t(int64_t(r31) + 24));
  // mr r3,r29
  r3 = r29;
  // stw r28,4(r30)
  guest::StoreU32(base, schema::StateLayer(base, uint32_t(r30) - 36).ops_head_address(), uint32_t(r28));
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x820AD1E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r29)
  r3 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r29) - 24).states_head_address());
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820AD1E8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,12
  r30 = uint64_t(int64_t(int64_t(r31) + 12));
  // stw r28,4(r29)
  guest::StoreU32(base, schema::StateLayer(base, uint32_t(r29) - 24).states_head_address(), uint32_t(r28));
  // mr r3,r30
  r3 = r30;
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x820AD1F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r30) - 12).children_head_address());
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820AD200, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stw r28,4(r30)
  guest::StoreU32(base, schema::StateLayer(base, uint32_t(r30) - 12).children_head_address(), uint32_t(r28));
  // addi r11,r11,10368
  r11 = uint64_t(int64_t(int64_t(r11) + 10368));
  // addi r10,r10,2616
  r10 = uint64_t(int64_t(int64_t(r10) + 2616));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::StateLayer(base, uint32_t(r31)).state_vtable_address(), uint32_t(r11));
  // stw r10,0(r31)
  guest::StoreU32(base, schema::StateLayer(base, uint32_t(r31)).vtable_address(), uint32_t(r10));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820AD228: lifted from generated/ by lift.py.
uint64_t f820AD228(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820AD228 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // addi r31,r3,12
  r31 = uint64_t(int64_t(int64_t(r3) + 12));
  // lwz r29,32(r3)
  r29 = guest::LoadU32(base, uint32_t(r3) + 32);
  // lwz r28,4(r31)
  r28 = guest::LoadU32(base, uint32_t(r31) + 4);
  // mr r11,r28
  r11 = r28;
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
loc_820AD248:
  // cmplw cr6,r31,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r31), so);
  // beq cr6,0x820ad254
  if (cr6.eq) goto loc_820AD254;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820AD254:
  // subf r11,r30,r28
  r11 = r28 - r30;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820ad2a0
  if (cr0.eq) goto loc_820AD2A0;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x820ad27c
  if (!cr6.eq) goto loc_820AD27C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820AD27C:
  // lwz r3,8(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 8);
  // bl 0x820ad228
  // (left out: dead in C++)
  r3 = f820AD228(base, r1, r13, r3);
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // add r29,r3,r29
  r29 = r3 + r29;
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x820ad298
  if (!cr6.eq) goto loc_820AD298;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820AD298:
  // lwz r30,0(r30)
  r30 = guest::LoadU32(base, uint32_t(r30) + 0);
  // b 0x820ad248
  goto loc_820AD248;
loc_820AD2A0:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_820AD2B0: lifted from generated/ by lift.py.
lifted::Result f820AD2B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820AD2B0 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,28(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 28);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beqlr
  if (cr0.eq) return {r3, f1};
  // addi r11,r3,20
  r11 = uint64_t(int64_t(int64_t(r3) + 20));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // rotlwi r9,r10,0
  r9 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x820ad2dc
  if (!cr6.eq) goto loc_820AD2DC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820AD2DC:
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // addi r4,r3,-4
  r4 = uint64_t(int64_t(int64_t(r3) + -4));
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,24(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 24);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820AD300: lifted from generated/ by lift.py.
lifted::Result f820AD300(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820AD300 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,28(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 28);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beqlr
  if (cr0.eq) return {r3, f1};
  // addi r11,r3,20
  r11 = uint64_t(int64_t(int64_t(r3) + 20));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // rotlwi r9,r10,0
  r9 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x820ad32c
  if (!cr6.eq) goto loc_820AD32C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820AD32C:
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // addi r4,r3,-4
  r4 = uint64_t(int64_t(int64_t(r3) + -4));
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,28(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 28);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820AD350: lifted from generated/ by lift.py.
lifted::Result f820AD350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AD350 (lifted)");
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
  // bl 0x820ad190
  // (left out: dead in C++)
  { const lifted::Result o = f820AD190(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820ad380
  if (cr0.eq) goto loc_820AD380;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820AD380, r31)); r3 = o.r3; f1 = o.f1; }
loc_820AD380:
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

// sub_820AD5B0: lifted from generated/ by lift.py.
lifted::Result f820AD5B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820AD5B0 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r31)).states_size_address());
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x820ad608
  if (!cr0.eq) goto loc_820AD608;
  // addi r11,r31,24
  r11 = uint64_t(int64_t(int64_t(r31) + 24));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r11) - 24).states_head_address());
  // rotlwi r9,r10,0
  r9 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::StateListNode(base, uint32_t(r10)).next_address());
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x820ad5f0
  if (!cr6.eq) goto loc_820AD5F0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820AD5F0:
  // lwz r3,8(r11)
  r3 = guest::LoadU32(base, schema::StateListNode(base, uint32_t(r11)).value_address());
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::AppState(base, uint32_t(r3)).vtable_address());
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::AppStateVtable(base, uint32_t(r11)).on_step_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AD608, r31)); r3 = o.r3; f1 = o.f1; }
loc_820AD608:
  // lis r10,-32245
  r10 = uint64_t(int64_t(-2113208320));
  // stw r31,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r31));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r10,r10,-12992
  r10 = uint64_t(int64_t(int64_t(r10) + -12992));
  // addi r11,r31,12
  r11 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r7,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r7));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // ld r10,80(r1)
  r10 = guest::LoadU64(base, uint32_t(r1) + 80);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // std r10,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r10);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r11) - 12).children_head_address());
  // ld r6,0(r9)
  r6 = guest::LoadU64(base, uint32_t(r9) + 0);
  // ld r7,8(r9)
  r7 = guest::LoadU64(base, uint32_t(r9) + 8);
  // mr r9,r10
  r9 = r10;
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::StateListNode(base, uint32_t(r10)).next_address());
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // ld r5,0(r8)
  r5 = guest::LoadU64(base, uint32_t(r8) + 0);
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // ld r4,0(r4)
  r4 = guest::LoadU64(base, uint32_t(r4) + 0);
  // bl 0x820ad3a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AD3A0, LIFTED_ARGS(0x820AD66C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820AD750: lifted from generated/ by lift.py.
lifted::Result f820AD750(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820AD750 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x820ad4e0
  { const lifted::Result o = lifted::Call(base, 0x820AD4E0, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820AD758: lifted from generated/ by lift.py.
lifted::Result f820AD758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820AD758 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x820ad5b0
  { const lifted::Result o = f820AD5B0(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820AD808: lifted from generated/ by lift.py.
lifted::Result f820AD808(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820AD808 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // std r5,168(r1)
  guest::StoreU64(base, uint32_t(r1) + 168, r5);
  // mr r30,r3
  r30 = r3;
  // lwz r11,168(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 168);
  // std r4,160(r1)
  guest::StoreU64(base, uint32_t(r1) + 160, r4);
  // std r6,176(r1)
  guest::StoreU64(base, uint32_t(r1) + 176, r6);
  // std r7,184(r1)
  guest::StoreU64(base, uint32_t(r1) + 184, r7);
  // std r8,192(r1)
  guest::StoreU64(base, uint32_t(r1) + 192, r8);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,164(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 164);
  // std r9,200(r1)
  guest::StoreU64(base, uint32_t(r1) + 200, r9);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // rldicl r10,r11,32,32
  r10 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFF;
  // rotlwi r31,r11,0
  r31 = __builtin_rotateleft32(uint32_t(r11), 0);
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
loc_820AD85C:
  // lwz r11,184(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 184);
  // subf r11,r31,r11
  r11 = r11 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820ad8a0
  if (cr0.eq) goto loc_820AD8A0;
  // lwz r11,196(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 196);
  // lwz r10,200(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 200);
  // add r3,r10,r11
  r3 = r10 + r11;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // lwz r4,8(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lwz r11,192(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 192);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AD898, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = guest::LoadU32(base, uint32_t(r31) + 4);
  // b 0x820ad85c
  goto loc_820AD85C;
loc_820AD8A0:
  // addi r11,r1,192
  r11 = uint64_t(int64_t(int64_t(r1) + 192));
  // mr r3,r30
  r3 = r30;
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r10,0(r30)
  guest::StoreU64(base, schema::AppMemberFunctor(base, uint32_t(r30)).function_object_address(), r10);
  // std r11,8(r30)
  guest::StoreU64(base, schema::AppMemberFunctor(base, uint32_t(r30)).adjust_address(), r11);
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

// sub_820AD8D0: lifted from generated/ by lift.py.
lifted::Result f820AD8D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820AD8D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,32(r30)
  r11 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r30)).states_size_address());
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x820ad954
  if (!cr0.eq) goto loc_820AD954;
  // lis r10,-32245
  r10 = uint64_t(int64_t(-2113208320));
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // addi r11,r30,24
  r11 = uint64_t(int64_t(int64_t(r30) + 24));
  // stw r30,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r30));
  // addi r10,r10,-12600
  r10 = uint64_t(int64_t(int64_t(r10) + -12600));
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // ld r10,80(r1)
  r10 = guest::LoadU64(base, uint32_t(r1) + 80);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // std r10,128(r1)
  guest::StoreU64(base, uint32_t(r1) + 128, r10);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r11) - 24).states_head_address());
  // ld r6,0(r9)
  r6 = guest::LoadU64(base, uint32_t(r9) + 0);
  // ld r7,8(r9)
  r7 = guest::LoadU64(base, uint32_t(r9) + 8);
  // mr r9,r10
  r9 = r10;
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::StateListNode(base, uint32_t(r10)).next_address());
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // ld r5,0(r8)
  r5 = guest::LoadU64(base, uint32_t(r8) + 0);
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // ld r4,0(r4)
  r4 = guest::LoadU64(base, uint32_t(r4) + 0);
  // bl 0x820ad3a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AD3A0, LIFTED_ARGS(0x820AD954, r31)); r3 = o.r3; f1 = o.f1; }
loc_820AD954:
  // lis r11,-32245
  r11 = uint64_t(int64_t(-2113208320));
  // stw r29,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r29));
  // addi r31,r30,12
  r31 = uint64_t(int64_t(int64_t(r30) + 12));
  // stw r30,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r30));
  // addi r11,r11,-12672
  r11 = uint64_t(int64_t(int64_t(r11) + -12672));
  // stw r29,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r29));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r29,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r29));
  // addi r7,r1,112
  r7 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r31,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r31));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // ld r11,96(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 96);
  // std r11,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r11);
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r31) - 12).children_head_address());
  // ld r8,0(r9)
  r8 = guest::LoadU64(base, uint32_t(r9) + 0);
  // mr r6,r11
  r6 = r11;
  // ld r9,8(r9)
  r9 = guest::LoadU64(base, uint32_t(r9) + 8);
  // stw r31,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r31));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::StateListNode(base, uint32_t(r11)).next_address());
  // stw r6,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r6));
  // ld r6,96(r1)
  r6 = guest::LoadU64(base, uint32_t(r1) + 96);
  // rldicl r5,r6,32,32
  r5 = __builtin_rotateleft64(r6, 32) & 0xFFFFFFFF;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // stw r6,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r6));
  // stw r5,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r5));
  // lwz r6,8(r7)
  r6 = guest::LoadU32(base, uint32_t(r7) + 8);
  // ld r4,0(r7)
  r4 = guest::LoadU64(base, uint32_t(r7) + 0);
  // rldicl r7,r11,32,32
  r7 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFF;
  // stw r11,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r11));
  // rldicr r5,r6,32,63
  r5 = __builtin_rotateleft64(r6, 32) & 0xFFFFFFFFFFFFFFFF;
  // stw r7,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r7));
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // ld r6,0(r10)
  r6 = guest::LoadU64(base, uint32_t(r10) + 0);
  // rldicr r7,r11,32,63
  r7 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // bl 0x820ad808
  // (left out: dead in C++)
  { const lifted::Result o = f820AD808(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x820AD9F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820ADA00: lifted from generated/ by lift.py.
lifted::Result f820ADA00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820ADA00 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x820ad8d0
  { const lifted::Result o = f820AD8D0(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820ADA70: lifted from generated/ by lift.py.
lifted::Result f820ADA70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820ADA70 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r31,r3,36
  r31 = uint64_t(int64_t(int64_t(r3) + 36));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r30,4(r31)
  r30 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r31) - 36).ops_head_address());
  // mr r4,r30
  r4 = r30;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lwz r5,4(r30)
  r5 = guest::LoadU32(base, schema::AppOpListNode(base, uint32_t(r30)).previous_address());
  // bl 0x820acff8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACFF8, LIFTED_ARGS(0x820ADAA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x820ad768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AD768, LIFTED_ARGS(0x820ADAB8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,4(r30)
  guest::StoreU32(base, schema::AppOpListNode(base, uint32_t(r30)).previous_address(), uint32_t(r29));
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::AppOpListNode(base, uint32_t(r29)).previous_address());
  // stw r29,0(r11)
  guest::StoreU32(base, schema::AppOpListNode(base, uint32_t(r11)).next_address(), uint32_t(r29));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820ADB28: lifted from generated/ by lift.py.
lifted::Result f820ADB28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820ADB28 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r31,r3,36
  r31 = uint64_t(int64_t(int64_t(r3) + 36));
  // stw r4,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r4));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r30,4(r31)
  r30 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r31) - 36).ops_head_address());
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // mr r4,r30
  r4 = r30;
  // lwz r5,4(r30)
  r5 = guest::LoadU32(base, schema::AppOpListNode(base, uint32_t(r30)).previous_address());
  // bl 0x820acff8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACFF8, LIFTED_ARGS(0x820ADB5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x820ad768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AD768, LIFTED_ARGS(0x820ADB6C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,4(r30)
  guest::StoreU32(base, schema::AppOpListNode(base, uint32_t(r30)).previous_address(), uint32_t(r29));
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::AppOpListNode(base, uint32_t(r29)).previous_address());
  // stw r29,0(r11)
  guest::StoreU32(base, schema::AppOpListNode(base, uint32_t(r11)).next_address(), uint32_t(r29));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820ADB80: lifted from generated/ by lift.py.
lifted::Result f820ADB80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820ADB80 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r31,r3,36
  r31 = uint64_t(int64_t(int64_t(r3) + 36));
  // stw r4,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r4));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r30,4(r31)
  r30 = guest::LoadU32(base, schema::StateLayer(base, uint32_t(r31) - 36).ops_head_address());
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // mr r4,r30
  r4 = r30;
  // lwz r5,4(r30)
  r5 = guest::LoadU32(base, schema::AppOpListNode(base, uint32_t(r30)).previous_address());
  // bl 0x820acff8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACFF8, LIFTED_ARGS(0x820ADBB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x820ad768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AD768, LIFTED_ARGS(0x820ADBC4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,4(r30)
  guest::StoreU32(base, schema::AppOpListNode(base, uint32_t(r30)).previous_address(), uint32_t(r29));
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::AppOpListNode(base, uint32_t(r29)).previous_address());
  // stw r29,0(r11)
  guest::StoreU32(base, schema::AppOpListNode(base, uint32_t(r11)).next_address(), uint32_t(r29));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820AE198: lifted from generated/ by lift.py.
uint64_t f820AE198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820AE198 (lifted)");
  double f1 = 0.0;
  // lwz r3,8(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 8);
  // b 0x820ad228
  r3 = f820AD228(base, r1, r13, r3);
  return r3;
}

// sub_820AE2B0: lifted from generated/ by lift.py.
lifted::Result f820AE2B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AE2B0 (lifted)");
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
  // addi r11,r11,10488
  r11 = uint64_t(int64_t(int64_t(r11) + 10488));
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3400
  r11 = uint64_t(int64_t(int64_t(r11) + 3400));
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

// sub_820AE2F8: lifted from generated/ by lift.py.
lifted::Result f820AE2F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AE2F8 (lifted)");
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
  // bl 0x820ae2b0
  // (left out: dead in C++)
  { const lifted::Result o = f820AE2B0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820ae328
  if (cr0.eq) goto loc_820AE328;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820AE328, r31)); r3 = o.r3; f1 = o.f1; }
loc_820AE328:
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

// sub_820AE5D0 game::Movie::Open: lifted from generated/ by lift.py.
lifted::Result f820AE5D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820AE5D0 (lifted)");
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
  // stwu r1,-384(r1)
  ea = -384 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820a6be0
  // (left out: dead in C++)
  { const lifted::Result o = game::GamePath::Ctor(base, LIFTED_ARGS(0x820AE5F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::Movie(base, uint32_t(r31)).framework_interface_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AE604, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AE614, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // bl 0x8258d8c8
  // (left out: dead in C++)
  { const lifted::Result o = game::VideoOutput::LockDevice(base, LIFTED_ARGS(0x820AE61C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // addi r4,r31,48
  r4 = uint64_t(int64_t(int64_t(r31) + 48));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820a6bf0
  // (left out: dead in C++)
  { const lifted::Result o = game::GamePath::SetGame(base, LIFTED_ARGS(0x820AE62C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::Movie(base, uint32_t(r31)).wrapper_address());
  // mr r5,r3
  r5 = r3;
  // lwz r4,80(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 80);
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::MovieWrapper(base, uint32_t(r11)).vtable_address());
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::MovieWrapperVtable(base, uint32_t(r10)).open_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AE64C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // bl 0x8258d908
  // (left out: dead in C++)
  { const lifted::Result o = game::VideoOutput::UnlockDevice(base, LIFTED_ARGS(0x820AE65C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq cr6,0x820ae668
  if (cr6.eq) goto loc_820AE668;
  // bl 0x8209ac78
  // (left out: dead in C++)
  { const lifted::Result o = game::ReportReadError(base, r1, r13, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_820AE668:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820a6be0
  // (left out: dead in C++)
  { const lifted::Result o = game::GamePath::Ctor(base, LIFTED_ARGS(0x820AE670, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,384
  r1 = uint64_t(int64_t(int64_t(r1) + 384));
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

// sub_820AE688 game::Movie::OnTick: lifted from generated/ by lift.py.
lifted::Result f820AE688(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AE688 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r3
  r29 = r3;
  // lwz r11,308(r29)
  r11 = guest::LoadU32(base, schema::Movie(base, uint32_t(r29)).state_address());
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x820ae6ac
  if (cr6.eq) goto loc_820AE6AC;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x820ae778
  if (!cr6.eq) goto loc_820AE778;
loc_820AE6AC:
  // lwz r3,4(r29)
  r3 = guest::LoadU32(base, schema::Movie(base, uint32_t(r29)).framework_interface_address());
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AE6C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AE6D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AE6EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x8258c9d0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::OpenChunk(base, LIFTED_ARGS(0x820AE6F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x820AE704, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x820AE714, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,28
  r10 = uint64_t(int64_t(28));
  // mr r3,r30
  r3 = r30;
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258ca50
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::CloseChunk(base, LIFTED_ARGS(0x820AE72C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // sth r29,0(r31)
  guest::StoreU16(base, uint32_t(r31) + 0, uint16_t(r29));
  // sth r29,2(r31)
  guest::StoreU16(base, uint32_t(r31) + 2, uint16_t(r29));
  // stw r29,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r29));
  // bl 0x8258ca88
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::ChunkSlots(base, LIFTED_ARGS(0x820AE744, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // stw r29,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r29));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // li r5,168
  r5 = uint64_t(int64_t(168));
  // mr r4,r31
  r4 = r31;
  // mr r3,r28
  r3 = r28;
  // lfs f1,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2612);
  f1 = double(temp.f32);
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AE778, r31)); r3 = o.r3; f1 = o.f1; }
loc_820AE778:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820AE780 game::Movie::OnMessage: lifted from generated/ by lift.py.
lifted::Result f820AE780(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AE780 (lifted)");
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
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x820ae7e0
  if (!cr6.eq) goto loc_820AE7E0;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24);
  // stw r30,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r30));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AE7C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // stw r30,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r30));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AE7E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820AE7E0:
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

// sub_820AE808: lifted from generated/ by lift.py.
lifted::Result f820AE808(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820AE808 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r12,0(r3)
  r12 = guest::LoadU32(base, schema::Movie(base, uint32_t(r3)).vtable_address());
  // lwz r11,36(r12)
  r11 = guest::LoadU32(base, schema::MovieVtable(base, uint32_t(r12)).on_tick_address());
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820AE818: lifted from generated/ by lift.py.
lifted::Result f820AE818(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820AE818 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r12,0(r3)
  r12 = guest::LoadU32(base, schema::Movie(base, uint32_t(r3)).vtable_address());
  // lwz r11,40(r12)
  r11 = guest::LoadU32(base, schema::MovieVtable(base, uint32_t(r12)).on_message_address());
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820AE9F8: lifted from generated/ by lift.py.
lifted::Result f820AE9F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AE9F8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // addi r11,r11,10512
  r11 = uint64_t(int64_t(int64_t(r11) + 10512));
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, schema::Movie(base, uint32_t(r31)).wrapper_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::Movie(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x820aea38
  if (cr0.eq) goto loc_820AEA38;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::MovieWrapper(base, uint32_t(r3)).vtable_address());
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, schema::MovieWrapperVtable(base, uint32_t(r11)).deleting_destructor_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820AEA38, r31)); r3 = o.r3; f1 = o.f1; }
loc_820AEA38:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // addi r11,r31,12
  r11 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r10,r10,2748
  r10 = uint64_t(int64_t(int64_t(r10) + 2748));
  // addi r9,r9,2984
  r9 = uint64_t(int64_t(int64_t(r9) + 2984));
  // addi r8,r8,2740
  r8 = uint64_t(int64_t(int64_t(r8) + 2740));
  // addi r7,r7,3768
  r7 = uint64_t(int64_t(int64_t(r7) + 3768));
  // stw r10,24(r11)
  guest::StoreU32(base, schema::Movie(base, uint32_t(r11) - 12).delegate_vtable_address(), uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(base, schema::Movie(base, uint32_t(r11) - 12).command_vtable_4_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::Movie(base, uint32_t(r11) - 12).command_vtable_4_address(), uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::Movie(base, uint32_t(r11) - 12).command_vtable_address(), uint32_t(r7));
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

// sub_820AEBC0: lifted from generated/ by lift.py.
lifted::Result f820AEBC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820AEBC0 (lifted)");
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
  // bl 0x820ae9f8
  // (left out: dead in C++)
  { const lifted::Result o = f820AE9F8(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820aebf0
  if (cr0.eq) goto loc_820AEBF0;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820AEBF0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820AEBF0:
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

// sub_820B0098: lifted from generated/ by lift.py.
void f820B0098(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820B0098 (lifted)");
  uint64_t r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,2284(r3)
  guest::StoreU8(base, schema::Vehicle(base, uint32_t(r3)).shift_up_request_address(), uint8_t(r11));
  // blr
  return;
}

// sub_820B00A8: lifted from generated/ by lift.py.
void f820B00A8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820B00A8 (lifted)");
  uint64_t r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,2285(r3)
  guest::StoreU8(base, schema::Vehicle(base, uint32_t(r3)).shift_down_request_address(), uint8_t(r11));
  // blr
  return;
}

// sub_820B00B8: lifted from generated/ by lift.py.
void f820B00B8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820B00B8 (lifted)");
  uint64_t r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,2286(r3)
  guest::StoreU8(base, schema::Vehicle(base, uint32_t(r3)).unknown_2286_address(), uint8_t(r11));
  // blr
  return;
}

// sub_820B03E0: lifted from generated/ by lift.py.
void f820B03E0(uint8_t* base, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_820B03E0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // clrlwi. r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820b0414
  if (cr0.eq) goto loc_820B0414;
  // lwz r11,272(r3)
  r11 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r3)).car_data_address());
  // lfs f13,292(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r3)).brake_address());
  f13 = double(temp.f32);
  // lfs f0,128(r11)
  temp.u32 = guest::LoadU32(base, schema::CarData(base, uint32_t(r11)).handbrake_ramp_address());
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmadds f0,f0,f1,f13
  f0 = double(float(std::fma(f0, f1, f13)));
  // stfs f0,292(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r3)).brake_address(), temp.u32);
  // lfs f13,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blelr cr6
  if (!cr6.gt) return;
  // stfs f13,292(r3)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r3)).brake_address(), temp.u32);
  // blr
  return;
loc_820B0414:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,292(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r3)).brake_address(), temp.u32);
  // blr
  return;
}

// sub_820B0428: lifted from generated/ by lift.py.
void f820B0428(uint8_t* base, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_820B0428 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // clrlwi. r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820b0468
  if (cr0.eq) goto loc_820B0468;
  // lwz r11,2252(r3)
  r11 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r3)).gear_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt cr6,0x820b0468
  if (cr6.lt) goto loc_820B0468;
  // lwz r11,272(r3)
  r11 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r3)).car_data_address());
  // lfs f13,284(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r3)).throttle_address());
  f13 = double(temp.f32);
  // lfs f0,1264(r11)
  temp.u32 = guest::LoadU32(base, schema::CarData(base, uint32_t(r11)).reverse_ramp_address());
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmadds f0,f0,f1,f13
  f0 = double(float(std::fma(f0, f1, f13)));
  // stfs f0,284(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r3)).throttle_address(), temp.u32);
  // lfs f13,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blelr cr6
  if (!cr6.gt) return;
  // stfs f13,284(r3)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r3)).throttle_address(), temp.u32);
  // blr
  return;
loc_820B0468:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,284(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r3)).throttle_address(), temp.u32);
  // blr
  return;
}

// sub_820B95F8: lifted from generated/ by lift.py.
lifted::Result f820B95F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820B95F8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
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
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r31,48
  r5 = uint64_t(int64_t(int64_t(r31) + 48));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820B9618, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294170
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Dot(base, LIFTED_ARGS(0x820B9624, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,32(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 32);
  f0 = double(temp.f32);
  // fmuls f0,f0,f0
  f0 = double(float(f0 * f0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x820b963c
  if (cr6.lt) goto loc_820B963C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820B963C:
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

// sub_820B9650: lifted from generated/ by lift.py.
uint64_t f820B9650(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820B9650 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,48
  r3 = uint64_t(int64_t(int64_t(r3) + 48));
  // b 0x82294320
  { const lifted::Result o = game::Vec3::AddTo(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820B9680: lifted from generated/ by lift.py.
lifted::Result f820B9680(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_820B9680 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-64(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -64, std::bit_cast<uint64_t>(f29));
  // stfd f30,-56(r1)
  guest::StoreU64(base, uint32_t(r1) + -56, std::bit_cast<uint64_t>(f30));
  // stfd f31,-48(r1)
  guest::StoreU64(base, uint32_t(r1) + -48, std::bit_cast<uint64_t>(f31));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r5
  r30 = r5;
  // mr r28,r4
  r28 = r4;
  // addi r5,r31,48
  r5 = uint64_t(int64_t(int64_t(r31) + 48));
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r29,r6
  r29 = r6;
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820B96B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820B96C8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820B96D4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820B96E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f0,8680(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8680);
  f0 = double(temp.f32);
  // fmuls f31,f1,f0
  f31 = double(float(f1 * f0));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820B96FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,32(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820B9680Shape(base, uint32_t(r31)).unknown_32_address());
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fnmsubs f0,f0,f0,f1
  f0 = double(float(-std::fma(f0, f0, -f1)));
  // fmuls f13,f0,f30
  f13 = double(float(f0 * f30));
  // lfs f0,8696(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8696);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f29,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f29 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // fmsubs f1,f31,f31,f0
  f1 = double(float(std::fma(f31, f31, -f0)));
  // fcmpu cr6,f1,f29
  cr6 = lifted::CompareFloat(f1, f29);
  // bge cr6,0x820b9730
  if (!cr6.lt) goto loc_820B9730;
loc_820B9728:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820b97a8
  goto loc_820B97A8;
loc_820B9730:
  // bl 0x82293590
  // (left out: dead in C++)
  { const lifted::Result o = game::sqrtf(base, LIFTED_ARGS(0x820B9734, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11016(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11016);
  f0 = double(temp.f32);
  // fcmpu cr6,f30,f0
  cr6 = lifted::CompareFloat(f30, f0);
  // bge cr6,0x820b9748
  if (!cr6.lt) goto loc_820B9748;
  // fmr f30,f0
  f30 = f0;
loc_820B9748:
  // fneg f13,f31
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f31) ^ 0x8000000000000000));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fsubs f12,f1,f31
  f12 = double(float(f1 - f31));
  // lfs f0,7140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // fdivs f0,f0,f30
  f0 = double(float(f0 / f30));
  // fsubs f13,f13,f1
  f13 = double(float(f13 - f1));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // fcmpu cr6,f13,f29
  cr6 = lifted::CompareFloat(f13, f29);
  // bge cr6,0x820b9778
  if (!cr6.lt) goto loc_820B9778;
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // blt cr6,0x820b9728
  if (cr6.lt) goto loc_820B9728;
loc_820B9778:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f12 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // ble cr6,0x820b9790
  if (!cr6.gt) goto loc_820B9790;
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bgt cr6,0x820b9728
  if (cr6.gt) goto loc_820B9728;
loc_820B9790:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stfs f13,8(r28)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::Sub820B9680Record(base, uint32_t(r28)).unknown_8_address(), temp.u32);
  // stfs f0,12(r28)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Sub820B9680Record(base, uint32_t(r28)).unknown_12_address(), temp.u32);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  guest::StoreU32(base, schema::Sub820B9680Record(base, uint32_t(r28)).unknown_0_address(), uint32_t(r11));
  // stw r11,4(r28)
  guest::StoreU32(base, schema::Sub820B9680Record(base, uint32_t(r28)).unknown_4_address(), uint32_t(r11));
loc_820B97A8:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f29,-64(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // lfd f30,-56(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // lfd f31,-48(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820B97C0: lifted from generated/ by lift.py.
uint64_t f820B97C0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820B97C0 (lifted)");
  // addi r3,r3,48
  r3 = uint64_t(int64_t(int64_t(r3) + 48));
  // blr
  return r3;
}

// sub_820B97C8: lifted from generated/ by lift.py.
uint64_t f820B97C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820B97C8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,48
  r3 = uint64_t(int64_t(int64_t(r3) + 48));
  // b 0x822940e0
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820B97D0: lifted from generated/ by lift.py.
void f820B97D0(uint8_t* base, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820B97D0 (lifted)");
  lifted::Temp temp{};
  // stfs f1,32(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r3) + 32, temp.u32);
  // blr
  return;
}

// sub_820B97D8: lifted from generated/ by lift.py.
lifted::Result f820B97D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820B97D8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // addi r5,r5,48
  r5 = uint64_t(int64_t(int64_t(r5) + 48));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r31,r4
  r31 = r4;
  // bl 0x822938f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Matrix::TransformPoint(base, LIFTED_ARGS(0x820B9804, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r30,48
  r3 = uint64_t(int64_t(int64_t(r30) + 48));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820B9810, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::VectorLibMatrix(base, uint32_t(r31)).m00_address());
  f0 = double(temp.f32);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // addi r3,r30,64
  r3 = uint64_t(int64_t(int64_t(r30) + 64));
  // lfs f0,4(r31)
  temp.u32 = guest::LoadU32(base, schema::VectorLibMatrix(base, uint32_t(r31)).m01_address());
  f0 = double(temp.f32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // lfs f31,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // lfs f0,8(r31)
  temp.u32 = guest::LoadU32(base, schema::VectorLibMatrix(base, uint32_t(r31)).m02_address());
  f0 = double(temp.f32);
  // stfs f31,92(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820B9840, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,16(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::VectorLibMatrix(base, uint32_t(r31)).m10_address());
  f0 = double(temp.f32);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // addi r3,r30,80
  r3 = uint64_t(int64_t(int64_t(r30) + 80));
  // lfs f0,20(r31)
  temp.u32 = guest::LoadU32(base, schema::VectorLibMatrix(base, uint32_t(r31)).m11_address());
  f0 = double(temp.f32);
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // lfs f0,24(r31)
  temp.u32 = guest::LoadU32(base, schema::VectorLibMatrix(base, uint32_t(r31)).m12_address());
  f0 = double(temp.f32);
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820B9868, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,32(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::VectorLibMatrix(base, uint32_t(r31)).m20_address());
  f0 = double(temp.f32);
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // stfs f0,112(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 112, temp.u32);
  // addi r3,r30,96
  r3 = uint64_t(int64_t(int64_t(r30) + 96));
  // lfs f0,36(r31)
  temp.u32 = guest::LoadU32(base, schema::VectorLibMatrix(base, uint32_t(r31)).m21_address());
  f0 = double(temp.f32);
  // stfs f0,116(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 116, temp.u32);
  // lfs f0,40(r31)
  temp.u32 = guest::LoadU32(base, schema::VectorLibMatrix(base, uint32_t(r31)).m22_address());
  f0 = double(temp.f32);
  // stfs f31,124(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 124, temp.u32);
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 120, temp.u32);
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820B9890, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
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

// sub_820B98B0: lifted from generated/ by lift.py.
lifted::Result f820B98B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820B98B0 (lifted)");
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
  // mr r29,r4
  r29 = r4;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820B98D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // mr r3,r29
  r3 = r29;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820B98E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,76(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 76);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820B98FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr. r28,r3
  r28 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq 0x820b9948
  if (cr0.eq) goto loc_820B9948;
loc_820B9908:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // lwz r11,84(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820B9920, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r5,r3
  r5 = r3;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820B993C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplw cr6,r30,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r28), so);
  // blt cr6,0x820b9908
  if (cr6.lt) goto loc_820B9908;
loc_820B9948:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820B9950: lifted from generated/ by lift.py.
uint64_t f820B9950(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B9950 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r3,r11,14224
  r3 = uint64_t(int64_t(int64_t(r11) + 14224));
  // blr
  return r3;
}

// sub_820B9960: lifted from generated/ by lift.py.
uint64_t f820B9960(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B9960 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,11680
  r3 = uint64_t(int64_t(int64_t(r11) + 11680));
  // blr
  return r3;
}

// sub_820B9970: lifted from generated/ by lift.py.
lifted::Result f820B9970(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820B9970 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
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
  // addi r31,r3,48
  r31 = uint64_t(int64_t(int64_t(r3) + 48));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r5,r31
  r5 = r31;
  // bl 0x82293f68
  // (left out: dead in C++)
  { const lifted::Result o = f82293F68(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820B999C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820B9C30: lifted from generated/ by lift.py.
void f820B9C30(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820B9C30 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,10920
  r11 = uint64_t(int64_t(int64_t(r11) + 10920));
  // stw r11,0(r3)
  guest::StoreU32(base, schema::BoxShape(base, uint32_t(r3)).vtable_address(), uint32_t(r11));
  // blr
  return;
}

// sub_820B9FF8: lifted from generated/ by lift.py.
lifted::Result f820B9FF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_820B9FF8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-88
  r12 = uint64_t(int64_t(int64_t(r1) + -88));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r23,r4
  r23 = r4;
  // addi r22,r30,304
  r22 = uint64_t(int64_t(int64_t(r30) + 304));
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // mr r31,r22
  r31 = r22;
loc_820BA028:
  // addi r27,r31,16
  r27 = uint64_t(int64_t(int64_t(r31) + 16));
  // mr r3,r29
  r3 = r29;
  // mr r4,r27
  r4 = r27;
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA038, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x820ba05c
  if (cr6.lt) goto loc_820BA05C;
  // mr r4,r27
  r4 = r27;
  // mr r3,r28
  r3 = r28;
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA050, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x820ba13c
  if (cr6.gt) goto loc_820BA13C;
loc_820BA05C:
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // addi r31,r31,32
  r31 = uint64_t(int64_t(int64_t(r31) + 32));
  // cmplwi cr6,r26,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 6, so);
  // blt cr6,0x820ba028
  if (cr6.lt) goto loc_820BA028;
  // mr r5,r29
  r5 = r29;
  // mr r4,r28
  r4 = r28;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BA07C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lfs f28,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f28 = double(temp.f32);
  // addi r31,r30,320
  r31 = uint64_t(int64_t(int64_t(r30) + 320));
  // fmr f29,f28
  f29 = f28;
  // fmr f30,f28
  f30 = f28;
loc_820BA0A0:
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA0AC, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // fcmpu cr6,f31,f28
  cr6 = lifted::CompareFloat(f31, f28);
  // beq cr6,0x820ba150
  if (cr6.eq) goto loc_820BA150;
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA0C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,-16(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + -16);
  f0 = double(temp.f32);
  // fsubs f0,f0,f1
  f0 = double(float(f0 - f1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // fdivs f31,f0,f31
  f31 = double(float(f0 / f31));
  // fmr f1,f31
  f1 = f31;
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA0E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BA0F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r28,r22
  r28 = r22;
loc_820BA0F8:
  // cmplw cr6,r27,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r30), so);
  // beq cr6,0x820ba118
  if (cr6.eq) goto loc_820BA118;
  // addi r4,r28,16
  r4 = uint64_t(int64_t(int64_t(r28) + 16));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA10C, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r28)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r28) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x820ba150
  if (cr6.gt) goto loc_820BA150;
loc_820BA118:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r28,r28,32
  r28 = uint64_t(int64_t(int64_t(r28) + 32));
  // cmplwi cr6,r30,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 6, so);
  // blt cr6,0x820ba0f8
  if (cr6.lt) goto loc_820BA0F8;
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // bne cr6,0x820ba144
  if (!cr6.eq) goto loc_820BA144;
  // mr r25,r27
  r25 = r27;
  // fmr f29,f31
  lifted::ScalarFloatMode();
  f29 = f31;
  // b 0x820ba14c
  goto loc_820BA14C;
loc_820BA13C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820ba1c0
  goto loc_820BA1C0;
loc_820BA144:
  // mr r26,r27
  r26 = r27;
  // fmr f30,f31
  lifted::ScalarFloatMode();
  f30 = f31;
loc_820BA14C:
  // addi r24,r24,1
  r24 = uint64_t(int64_t(int64_t(r24) + 1));
loc_820BA150:
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r31,r31,32
  r31 = uint64_t(int64_t(int64_t(r31) + 32));
  // cmplwi cr6,r27,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 6, so);
  // blt cr6,0x820ba0a0
  if (cr6.lt) goto loc_820BA0A0;
  // cmplwi cr6,r24,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 2, so);
  // bne cr6,0x820ba13c
  if (!cr6.eq) goto loc_820BA13C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fcmpu cr6,f29,f0
  cr6 = lifted::CompareFloat(f29, f0);
  // ble cr6,0x820ba180
  if (!cr6.gt) goto loc_820BA180;
  // fcmpu cr6,f30,f0
  cr6 = lifted::CompareFloat(f30, f0);
  // bgt cr6,0x820ba13c
  if (cr6.gt) goto loc_820BA13C;
loc_820BA180:
  // fcmpu cr6,f29,f28
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f29, f28);
  // bge cr6,0x820ba190
  if (!cr6.lt) goto loc_820BA190;
  // fcmpu cr6,f30,f28
  cr6 = lifted::CompareFloat(f30, f28);
  // blt cr6,0x820ba13c
  if (cr6.lt) goto loc_820BA13C;
loc_820BA190:
  // fcmpu cr6,f29,f30
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f29, f30);
  // bge cr6,0x820ba1ac
  if (!cr6.lt) goto loc_820BA1AC;
  // stfs f29,8(r23)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r23) + 8, temp.u32);
  // stw r25,0(r23)
  guest::StoreU32(base, uint32_t(r23) + 0, uint32_t(r25));
  // stfs f30,12(r23)
  temp.f32 = float(f30);
  guest::StoreU32(base, uint32_t(r23) + 12, temp.u32);
  // stw r26,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r26));
  // b 0x820ba1bc
  goto loc_820BA1BC;
loc_820BA1AC:
  // stfs f30,8(r23)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  guest::StoreU32(base, uint32_t(r23) + 8, temp.u32);
  // stw r26,0(r23)
  guest::StoreU32(base, uint32_t(r23) + 0, uint32_t(r26));
  // stfs f29,12(r23)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r23) + 12, temp.u32);
  // stw r25,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r25));
loc_820BA1BC:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_820BA1C0:
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // addi r12,r1,-88
  r12 = uint64_t(int64_t(int64_t(r1) + -88));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BA1D0: lifted from generated/ by lift.py.
lifted::Result f820BA1D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820BA1D0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r31,r3,304
  r31 = uint64_t(int64_t(int64_t(r3) + 304));
loc_820BA1E8:
  // addi r4,r31,16
  r4 = uint64_t(int64_t(int64_t(r31) + 16));
  // mr r3,r29
  r3 = r29;
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA1F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31) - 304).face_px_distance_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x820ba21c
  if (cr6.gt) goto loc_820BA21C;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,32
  r31 = uint64_t(int64_t(int64_t(r31) + 32));
  // cmplwi cr6,r30,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 6, so);
  // blt cr6,0x820ba1e8
  if (cr6.lt) goto loc_820BA1E8;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_820BA214:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_820BA21C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820ba214
  goto loc_820BA214;
}

// sub_820BA228: lifted from generated/ by lift.py.
lifted::Result f820BA228(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820BA228 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r5
  r29 = r5;
  // addi r5,r31,320
  r5 = uint64_t(int64_t(int64_t(r31) + 320));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // lfs f1,304(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_304_address());
  f1 = double(temp.f32);
  // bl 0x82294470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294470, LIFTED_ARGS(0x820BA250, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BA25C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r31,352
  r5 = uint64_t(int64_t(int64_t(r31) + 352));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,336(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_336_address());
  f1 = double(temp.f32);
  // bl 0x82294470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294470, LIFTED_ARGS(0x820BA26C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x82294320
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::AddTo(base, LIFTED_ARGS(0x820BA278, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r31,384
  r5 = uint64_t(int64_t(int64_t(r31) + 384));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,368(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_368_address());
  f1 = double(temp.f32);
  // bl 0x82294470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294470, LIFTED_ARGS(0x820BA288, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x82294320
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::AddTo(base, LIFTED_ARGS(0x820BA294, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r31,416
  r5 = uint64_t(int64_t(int64_t(r31) + 416));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,400(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_400_address());
  f1 = double(temp.f32);
  // bl 0x82294470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294470, LIFTED_ARGS(0x820BA2A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BA2B0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r31,448
  r5 = uint64_t(int64_t(int64_t(r31) + 448));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,432(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_432_address());
  f1 = double(temp.f32);
  // bl 0x82294470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294470, LIFTED_ARGS(0x820BA2C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x82294320
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::AddTo(base, LIFTED_ARGS(0x820BA2CC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r31,480
  r5 = uint64_t(int64_t(int64_t(r31) + 480));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,464(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_464_address());
  f1 = double(temp.f32);
  // bl 0x82294470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294470, LIFTED_ARGS(0x820BA2DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x82294320
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::AddTo(base, LIFTED_ARGS(0x820BA2E8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BA688: lifted from generated/ by lift.py.
lifted::Result f820BA688(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820BA688 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x82294270
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Length(base, LIFTED_ARGS(0x820BA6A4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,128
  r30 = uint64_t(int64_t(int64_t(r31) + 128));
  // mr r4,r29
  r4 = r29;
  // stfs f1,32(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_32_address(), temp.u32);
  // mr r3,r30
  r3 = r30;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BA6B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r30) - 128).unknown_128_address());
  f13 = double(temp.f32);
  // addi r28,r31,384
  r28 = uint64_t(int64_t(int64_t(r31) + 384));
  // lfs f1,136(r31)
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_136_address());
  f1 = double(temp.f32);
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r4,r28
  r4 = r28;
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fdivs f13,f0,f13
  f13 = double(float(f0 / f13));
  // stfs f13,144(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_144_address(), temp.u32);
  // lfs f13,132(r31)
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_132_address());
  f13 = double(temp.f32);
  // fdivs f13,f0,f13
  f13 = double(float(f0 / f13));
  // stfs f13,148(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_148_address(), temp.u32);
  // fdivs f0,f0,f1
  f0 = double(float(f0 / f1));
  // stfs f0,152(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_152_address(), temp.u32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA6F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r31,352
  r27 = uint64_t(int64_t(int64_t(r31) + 352));
  // mr r25,r3
  r25 = r3;
  // lfs f1,132(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_132_address());
  f1 = double(temp.f32);
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r4,r27
  r4 = r27;
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA70C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r26,r31,320
  r26 = uint64_t(int64_t(int64_t(r31) + 320));
  // mr r24,r3
  r24 = r3;
  // lfs f1,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r30) - 128).unknown_128_address());
  f1 = double(temp.f32);
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // mr r4,r26
  r4 = r26;
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA724, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r31,48
  r29 = uint64_t(int64_t(int64_t(r31) + 48));
  // mr r5,r3
  r5 = r3;
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // mr r4,r29
  r4 = r29;
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BA738, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r24
  r5 = r24;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BA748, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r25
  r5 = r25;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BA758, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r28
  r4 = r28;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfs f1,136(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_136_address());
  f1 = double(temp.f32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA768, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // mr r4,r27
  r4 = r27;
  // lfs f1,132(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_132_address());
  f1 = double(temp.f32);
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA77C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // mr r4,r26
  r4 = r26;
  // lfs f1,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BA688Object(base, uint32_t(r30) - 128).unknown_128_address());
  f1 = double(temp.f32);
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA790, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BA7A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r24
  r5 = r24;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BA7B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r25
  r5 = r25;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BA7C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r31,180
  r10 = uint64_t(int64_t(int64_t(r31) + 180));
loc_820BA7C8:
  // clrlwi. r9,r11,31
  r9 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x820ba7d8
  if (cr0.eq) goto loc_820BA7D8;
  // lfs f0,96(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 96);
  f0 = double(temp.f32);
  // b 0x820ba7dc
  goto loc_820BA7DC;
loc_820BA7D8:
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
loc_820BA7DC:
  // stfs f0,-4(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r10) - 180).unknown_176_address(), temp.u32);
  // rlwinm. r9,r11,0,30,30
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x820ba7f0
  if (cr0.eq) goto loc_820BA7F0;
  // lfs f0,100(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 100);
  f0 = double(temp.f32);
  // b 0x820ba7f4
  goto loc_820BA7F4;
loc_820BA7F0:
  // lfs f0,84(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f0 = double(temp.f32);
loc_820BA7F4:
  // stfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r10) - 180).unknown_180_address(), temp.u32);
  // rlwinm. r9,r11,0,29,29
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x820ba808
  if (cr0.eq) goto loc_820BA808;
  // lfs f0,104(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 104);
  f0 = double(temp.f32);
  // b 0x820ba80c
  goto loc_820BA80C;
loc_820BA808:
  // lfs f0,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f0 = double(temp.f32);
loc_820BA80C:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stfs f0,4(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r10) - 180).unknown_184_address(), temp.u32);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // blt cr6,0x820ba7c8
  if (cr6.lt) goto loc_820BA7C8;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r26
  r3 = r26;
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA82C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r27
  r3 = r27;
  // stfs f1,304(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_304_address(), temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA83C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // stfs f1,336(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_336_address(), temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA84C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r31,416
  r3 = uint64_t(int64_t(int64_t(r31) + 416));
  // stfs f1,368(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_368_address(), temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA85C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r31,448
  r3 = uint64_t(int64_t(int64_t(r31) + 448));
  // stfs f1,400(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_400_address(), temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA86C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r31,480
  r3 = uint64_t(int64_t(int64_t(r31) + 480));
  // stfs f1,432(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_432_address(), temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA87C, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f1,464(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, schema::Sub820BA688Object(base, uint32_t(r31)).unknown_464_address(), temp.u32);
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BA888: lifted from generated/ by lift.py.
lifted::Result f820BA888(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820BA888 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r30,r31,48
  r30 = uint64_t(int64_t(int64_t(r31) + 48));
  // mr r3,r30
  r3 = r30;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BA8A4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r31,384
  r29 = uint64_t(int64_t(int64_t(r31) + 384));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lfs f1,136(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 136);
  f1 = double(temp.f32);
  // mr r4,r29
  r4 = r29;
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA8B8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r31,352
  r28 = uint64_t(int64_t(int64_t(r31) + 352));
  // mr r26,r3
  r26 = r3;
  // lfs f1,132(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 132);
  f1 = double(temp.f32);
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r4,r28
  r4 = r28;
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA8D0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r31,320
  r27 = uint64_t(int64_t(int64_t(r31) + 320));
  // mr r25,r3
  r25 = r3;
  // lfs f1,128(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 128);
  f1 = double(temp.f32);
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // mr r4,r27
  r4 = r27;
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA8E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BA8F8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r25
  r5 = r25;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BA908, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r26
  r5 = r26;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BA918, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfs f1,136(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 136);
  f1 = double(temp.f32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA928, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // mr r4,r28
  r4 = r28;
  // lfs f1,132(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 132);
  f1 = double(temp.f32);
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA93C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // mr r4,r27
  r4 = r27;
  // lfs f1,128(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 128);
  f1 = double(temp.f32);
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BA950, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BA960, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r25
  r5 = r25;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BA970, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r26
  r5 = r26;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BA980, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r31,180
  r10 = uint64_t(int64_t(int64_t(r31) + 180));
loc_820BA988:
  // clrlwi. r9,r11,31
  r9 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x820ba998
  if (cr0.eq) goto loc_820BA998;
  // lfs f0,96(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 96);
  f0 = double(temp.f32);
  // b 0x820ba99c
  goto loc_820BA99C;
loc_820BA998:
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
loc_820BA99C:
  // stfs f0,-4(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + -4, temp.u32);
  // rlwinm. r9,r11,0,30,30
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x820ba9b0
  if (cr0.eq) goto loc_820BA9B0;
  // lfs f0,100(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 100);
  f0 = double(temp.f32);
  // b 0x820ba9b4
  goto loc_820BA9B4;
loc_820BA9B0:
  // lfs f0,84(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f0 = double(temp.f32);
loc_820BA9B4:
  // stfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 0, temp.u32);
  // rlwinm. r9,r11,0,29,29
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x820ba9c8
  if (cr0.eq) goto loc_820BA9C8;
  // lfs f0,104(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 104);
  f0 = double(temp.f32);
  // b 0x820ba9cc
  goto loc_820BA9CC;
loc_820BA9C8:
  // lfs f0,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f0 = double(temp.f32);
loc_820BA9CC:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stfs f0,4(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 4, temp.u32);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // blt cr6,0x820ba988
  if (cr6.lt) goto loc_820BA988;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r27
  r3 = r27;
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA9EC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // stfs f1,304(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r31) + 304, temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BA9FC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r29
  r3 = r29;
  // stfs f1,336(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r31) + 336, temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BAA0C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r31,416
  r3 = uint64_t(int64_t(int64_t(r31) + 416));
  // stfs f1,368(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r31) + 368, temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BAA1C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r31,448
  r3 = uint64_t(int64_t(int64_t(r31) + 448));
  // stfs f1,400(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r31) + 400, temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BAA2C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r31,480
  r3 = uint64_t(int64_t(int64_t(r31) + 480));
  // stfs f1,432(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r31) + 432, temp.u32);
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BAA3C, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f1,464(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r31) + 464, temp.u32);
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BAA48: lifted from generated/ by lift.py.
uint64_t f820BAA48(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820BAA48 (lifted)");
  uint64_t r3 = 0;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // blr
  return r3;
}

// sub_820BB270: lifted from generated/ by lift.py.
lifted::Result f820BB270(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_820BB270 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f31,f1
  f31 = f1;
  // mr r31,r3
  r31 = r3;
  // mr r3,r4
  r3 = r4;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // bl 0x820ba228
  // (left out: dead in C++)
  { const lifted::Result o = f820BA228(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BB2C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BB2D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r30
  r5 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x820b99d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B99D0, LIFTED_ARGS(0x820BB2E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
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

// sub_820BB410: lifted from generated/ by lift.py.
lifted::Result f820BB410(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820BB410 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stfs f1,84(r1)
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // stfs f1,88(r1)
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // bl 0x820ba688
  // (left out: dead in C++)
  { const lifted::Result o = f820BA688(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820BB450: lifted from generated/ by lift.py.
lifted::Result f820BB450(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820BB450 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // addi r29,r31,48
  r29 = uint64_t(int64_t(int64_t(r31) + 48));
  // mr r30,r4
  r30 = r4;
  // mr r5,r29
  r5 = r29;
  // bl 0x82293f68
  // (left out: dead in C++)
  { const lifted::Result o = f82293F68(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BB484, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f0,128(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BB450Object(base, uint32_t(r31)).unknown_128_address());
  f0 = double(temp.f32);
  // mr r4,r30
  r4 = r30;
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfs f31,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,84(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // stfs f31,88(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // stfs f31,92(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // bl 0x82293fd0
  // (left out: dead in C++)
  { const lifted::Result o = f82293FD0(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f0,132(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BB450Object(base, uint32_t(r31)).unknown_132_address());
  f0 = double(temp.f32);
  // mr r4,r30
  r4 = r30;
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // stfs f31,96(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // stfs f31,104(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // bl 0x82293fd0
  // (left out: dead in C++)
  { const lifted::Result o = f82293FD0(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // lfs f0,136(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BB450Object(base, uint32_t(r31)).unknown_136_address());
  f0 = double(temp.f32);
  // mr r4,r30
  r4 = r30;
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 120, temp.u32);
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // stfs f31,112(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 112, temp.u32);
  // stfs f31,116(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 116, temp.u32);
  // stfs f31,124(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 124, temp.u32);
  // bl 0x82293fd0
  // (left out: dead in C++)
  { const lifted::Result o = f82293FD0(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // lfs f0,144(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 144);
  f0 = double(temp.f32);
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfs f13,160(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 160);
  f13 = double(temp.f32);
  // fabs f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // lfs f12,148(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 148);
  f12 = double(temp.f32);
  // fabs f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) & ~0x8000000000000000));
  // lfs f11,164(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 164);
  f11 = double(temp.f32);
  // fabs f12,f12
  f12 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) & ~0x8000000000000000));
  // lfs f10,152(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 152);
  f10 = double(temp.f32);
  // fabs f11,f11
  f11 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f11) & ~0x8000000000000000));
  // lfs f9,168(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 168);
  f9 = double(temp.f32);
  // fabs f10,f10
  f10 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f10) & ~0x8000000000000000));
  // fabs f9,f9
  f9 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f9) & ~0x8000000000000000));
  // lfs f8,176(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 176);
  f8 = double(temp.f32);
  // lfs f7,180(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 180);
  f7 = double(temp.f32);
  // fabs f8,f8
  f8 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f8) & ~0x8000000000000000));
  // lfs f6,184(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 184);
  f6 = double(temp.f32);
  // fabs f7,f7
  f7 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f7) & ~0x8000000000000000));
  // fabs f6,f6
  f6 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f6) & ~0x8000000000000000));
  // mr r3,r31
  r3 = r31;
  // stfs f31,140(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 140, temp.u32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // fadds f13,f11,f12
  f13 = double(float(f11 + f12));
  // fadds f12,f9,f10
  f12 = double(float(f9 + f10));
  // fadds f0,f0,f8
  f0 = double(float(f0 + f8));
  // stfs f0,128(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 128, temp.u32);
  // fadds f0,f13,f7
  f0 = double(float(f13 + f7));
  // stfs f0,132(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 132, temp.u32);
  // fadds f0,f12,f6
  f0 = double(float(f12 + f6));
  // stfs f0,136(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 136, temp.u32);
  // bl 0x820ba688
  // (left out: dead in C++)
  { const lifted::Result o = f820BA688(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BC198: lifted from generated/ by lift.py.
uint64_t f820BC198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820BC198 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // addi r4,r11,-32128
  r4 = uint64_t(int64_t(int64_t(r11) + -32128));
  // mr r30,r5
  r30 = r5;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BC1C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1024
  r11 = uint64_t(int64_t(67108864));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,48(r31)
  guest::StoreU32(base, schema::CollisionGroundDefault(base, uint32_t(r31)).surface_address(), uint32_t(r11));
  // lfs f0,4(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionGroundDefaultSource(base, uint32_t(r30)).unknown_4_address());
  f0 = double(temp.f32);
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::CollisionGroundDefault(base, uint32_t(r31)).depth_address(), temp.u32);
  // stw r10,52(r31)
  guest::StoreU32(base, schema::CollisionGroundDefault(base, uint32_t(r31)).position_address(), uint32_t(r10));
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

// sub_820BC4D0: lifted from generated/ by lift.py.
uint64_t f820BC4D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820BC4D0 (lifted)");
  uint64_t r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r6,r5
  r6 = r5;
  // mr r5,r4
  r5 = r4;
  // addi r3,r11,11824
  r3 = uint64_t(int64_t(int64_t(r11) + 11824));
  // mr r4,r31
  r4 = r31;
  // bl 0x820bc198
  // (left out: dead in C++)
  r3 = f820BC198(base, r1, r13, r4, r5);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820bc518
  if (cr0.eq) goto loc_820BC518;
  // lwz r11,48(r31)
  r11 = guest::LoadU32(base, schema::CollisionGroundDefault(base, uint32_t(r31)).surface_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // rlwinm r11,r11,0,8,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFF80FFFFFF;
  // stw r11,48(r31)
  guest::StoreU32(base, schema::CollisionGroundDefault(base, uint32_t(r31)).surface_address(), uint32_t(r11));
  // b 0x820bc51c
  goto loc_820BC51C;
loc_820BC518:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820BC51C:
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

// sub_820BC560: lifted from generated/ by lift.py.
lifted::Result f820BC560(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820BC560 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-64(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -64, std::bit_cast<uint64_t>(f30));
  // stfd f31,-56(r1)
  guest::StoreU64(base, uint32_t(r1) + -56, std::bit_cast<uint64_t>(f31));
  // stwu r1,-336(r1)
  ea = -336 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r4
  r30 = r4;
  // mr r31,r5
  r31 = r5;
  // mr r27,r3
  r27 = r3;
  // mr r3,r31
  r3 = r31;
  // lfs f31,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,48(r30)
  temp.f32 = float(f31);
  guest::StoreU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_48_address(), temp.u32);
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).type_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BC5A0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // beq cr6,0x820bc6e8
  if (cr6.eq) goto loc_820BC6E8;
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // beq cr6,0x820bc634
  if (cr6.eq) goto loc_820BC634;
  // mr r3,r31
  r3 = r31;
  // stfs f31,236(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 236, temp.u32);
  // stfs f31,252(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 252, temp.u32);
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r5,r3
  r5 = r3;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r4,r1,208
  r4 = uint64_t(int64_t(int64_t(r1) + 208));
  // mr r3,r27
  r3 = r27;
  // bl 0x820bc198
  // (left out: dead in C++)
  r3 = f820BC198(base, r1, r13, r4, r5);
  // mr r3,r31
  r3 = r31;
  // bl 0x820bab40
  // (left out: dead in C++)
  { const lifted::Result o = game::Shape::Radius(base, LIFTED_ARGS(0x820BC5DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f13,208(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 208);
  f13 = double(temp.f32);
  // fsubs f30,f1,f13
  f30 = double(float(f1 - f13));
  // lfs f0,48(r30)
  temp.u32 = guest::LoadU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_48_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f30,f0
  cr6 = lifted::CompareFloat(f30, f0);
  // ble cr6,0x820bc7ec
  if (!cr6.gt) goto loc_820BC7EC;
  // addi r4,r1,224
  r4 = uint64_t(int64_t(int64_t(r1) + 224));
  // addi r3,r30,16
  r3 = uint64_t(int64_t(int64_t(r30) + 16));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BC5FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // stfs f30,48(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  guest::StoreU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_48_address(), temp.u32);
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BC614, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820bab40
  // (left out: dead in C++)
  { const lifted::Result o = game::Shape::Radius(base, LIFTED_ARGS(0x820BC61C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,256(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 256);
  // lfs f0,8(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_8_address());
  f0 = double(temp.f32);
  // fsubs f0,f0,f1
  f0 = double(float(f0 - f1));
  // stfs f0,8(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_8_address(), temp.u32);
  // stw r11,56(r30)
  guest::StoreU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_56_address(), uint32_t(r11));
  // b 0x820bc7ec
  goto loc_820BC7EC;
loc_820BC634:
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_820BC638:
  // mr r4,r29
  r4 = r29;
  // stfs f31,172(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 172, temp.u32);
  // mr r3,r31
  r3 = r31;
  // stfs f31,188(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 188, temp.u32);
  // bl 0x820ba368
  // (left out: dead in C++)
  { const lifted::Result o = game::BoxShape::LowerCorner(base, LIFTED_ARGS(0x820BC64C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r28,r3
  r28 = r3;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // lwz r11,84(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).cache_word_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BC668, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // mr r5,r28
  r5 = r28;
  // addi r4,r1,144
  r4 = uint64_t(int64_t(int64_t(r1) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x820bc198
  // (left out: dead in C++)
  r3 = f820BC198(base, r1, r13, r4, r5);
  // lfs f0,144(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 144);
  f0 = double(temp.f32);
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // lfs f13,48(r30)
  temp.u32 = guest::LoadU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_48_address());
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x820bc6bc
  if (!cr6.gt) goto loc_820BC6BC;
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r3,r30,16
  r3 = uint64_t(int64_t(int64_t(r30) + 16));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BC69C, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,144(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 144);
  f0 = double(temp.f32);
  // mr r4,r28
  r4 = r28;
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // mr r3,r30
  r3 = r30;
  // stfs f0,48(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_48_address(), temp.u32);
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BC6B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,192(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 192);
  // stw r11,56(r30)
  guest::StoreU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_56_address(), uint32_t(r11));
loc_820BC6BC:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r29
  r4 = r29;
  // lwz r5,196(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 196);
  // mr r3,r31
  r3 = r31;
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).set_cache_word_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BC6D8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplwi cr6,r29,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 4, so);
  // blt cr6,0x820bc638
  if (cr6.lt) goto loc_820BC638;
  // b 0x820bc7ec
  goto loc_820BC7EC;
loc_820BC6E8:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BC700, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820bc7ec
  if (cr0.eq) goto loc_820BC7EC;
loc_820BC708:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // stfs f31,108(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // stfs f31,124(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 124, temp.u32);
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // lwz r11,84(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).cache_word_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BC728, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r28,r3
  r28 = r3;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BC744, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r6,r28
  r6 = r28;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r27
  r3 = r27;
  // bl 0x820bc198
  // (left out: dead in C++)
  r3 = f820BC198(base, r1, r13, r4, r5);
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // lfs f13,48(r30)
  temp.u32 = guest::LoadU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_48_address());
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x820bc7b0
  if (!cr6.gt) goto loc_820BC7B0;
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r30,16
  r3 = uint64_t(int64_t(int64_t(r30) + 16));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BC778, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,48(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_48_address(), temp.u32);
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BC79C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BC7A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,128(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 128);
  // stw r11,56(r30)
  guest::StoreU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_56_address(), uint32_t(r11));
loc_820BC7B0:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r29
  r4 = r29;
  // lwz r5,132(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 132);
  // mr r3,r31
  r3 = r31;
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).set_cache_word_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BC7CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BC7E4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r29,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r3), so);
  // blt cr6,0x820bc708
  if (cr6.lt) goto loc_820BC708;
loc_820BC7EC:
  // lfs f0,48(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_48_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x820bc810
  if (!cr6.gt) goto loc_820BC810;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lfs f13,8748(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8748);
  f13 = double(temp.f32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfs f0,48(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::CollisionUnreadHitRecord(base, uint32_t(r30)).unknown_48_address(), temp.u32);
  // b 0x820bc814
  goto loc_820BC814;
loc_820BC810:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820BC814:
  // addi r1,r1,336
  r1 = uint64_t(int64_t(int64_t(r1) + 336));
  // lfd f30,-64(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // lfd f31,-56(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BC828: lifted from generated/ by lift.py.
lifted::Result f820BC828(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820BC828 (lifted)");
  uint64_t r6 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0;
  // mr r11,r3
  r11 = r3;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r6,r5
  r6 = r5;
  // mr r5,r4
  r5 = r4;
  // addi r3,r10,11824
  r3 = uint64_t(int64_t(int64_t(r10) + 11824));
  // mr r4,r11
  r4 = r11;
  // b 0x820bc560
  { const lifted::Result o = f820BC560(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820BD740: lifted from generated/ by lift.py.
lifted::Result f820BD740(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820BD740 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-80(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -80, std::bit_cast<uint64_t>(f29));
  // stfd f30,-72(r1)
  guest::StoreU64(base, uint32_t(r1) + -72, std::bit_cast<uint64_t>(f30));
  // stfd f31,-64(r1)
  guest::StoreU64(base, uint32_t(r1) + -64, std::bit_cast<uint64_t>(f31));
  // stwu r1,-288(r1)
  ea = -288 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Sub820BD740Object(base, uint32_t(r31)).unknown_0_address());
  // bl 0x820911f8
  // (left out: dead in C++)
  r3 = f820911F8(base, r3);
  // mr r30,r3
  r30 = r3;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::Sub820BD740Object(base, uint32_t(r31)).unknown_0_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BD770, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::Sub820BD740Object(base, uint32_t(r31)).unknown_4_address());
  // bl 0x820911f8
  // (left out: dead in C++)
  r3 = f820911F8(base, r3);
  // mr r28,r3
  r28 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::Sub820BD740Object(base, uint32_t(r31)).unknown_4_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BD788, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,88(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BD740Object(base, uint32_t(r31)).unknown_88_address());
  f0 = double(temp.f32);
  // mr r27,r3
  r27 = r3;
  // lfs f31,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x820bd8e4
  if (cr6.lt) goto loc_820BD8E4;
  // lfs f13,84(r31)
  temp.u32 = guest::LoadU32(base, schema::Sub820BD740Object(base, uint32_t(r31)).unknown_84_address());
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x820bd8e4
  if (cr6.lt) goto loc_820BD8E4;
  // mr r3,r30
  r3 = r30;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r26,r3
  r26 = r3;
  // mr r3,r28
  r3 = r28;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r4,r3
  r4 = r3;
  // mr r5,r26
  r5 = r26;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BD7D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r26,r3
  r26 = r3;
  // mr r3,r27
  r3 = r27;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r4,r3
  r4 = r3;
  // mr r5,r26
  r5 = r26;
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BD7F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,128
  r5 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r4,r1,144
  r4 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BD804, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r29,r3
  r29 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r5,r3
  r5 = r3;
  // mr r6,r29
  r6 = r29;
  // lfs f1,88(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BD740Object(base, uint32_t(r31)).unknown_88_address());
  f1 = double(temp.f32);
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82294568
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Lerp(base, LIFTED_ARGS(0x820BD82C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r30,r3
  r30 = r3;
  // mr r3,r28
  r3 = r28;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r5,r3
  r5 = r3;
  // mr r6,r30
  r6 = r30;
  // lfs f1,88(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BD740Object(base, uint32_t(r31)).unknown_88_address());
  f1 = double(temp.f32);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82294568
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Lerp(base, LIFTED_ARGS(0x820BD854, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BD864, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294170
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Dot(base, LIFTED_ARGS(0x820BD870, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fcmpu cr6,f1,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f31);
  // lfs f30,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f30 = double(temp.f32);
  // bne cr6,0x820bd88c
  if (!cr6.eq) goto loc_820BD88C;
  // stfs f31,80(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // stfs f30,84(r1)
  temp.f32 = float(f30);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // stfs f31,88(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
loc_820BD88C:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294298
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294298, LIFTED_ARGS(0x820BD894, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x82294270
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Length(base, LIFTED_ARGS(0x820BD89C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // lfs f31,88(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Sub820BD740Object(base, uint32_t(r31)).unknown_88_address());
  f31 = double(temp.f32);
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // fmr f29,f1
  f29 = f1;
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BD8B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // lfs f1,7140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f1 = double(temp.f32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BD8C8, r31)); r3 = o.r3; f1 = o.f1; }
  // fsubs f0,f30,f31
  lifted::ScalarFloatMode();
  f0 = double(float(f30 - f31));
  // mr r4,r3
  r4 = r3;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmr f2,f31
  f2 = f31;
  // addi r3,r31,32
  r3 = uint64_t(int64_t(int64_t(r31) + 32));
  // fmuls f1,f0,f29
  f1 = double(float(f0 * f29));
  // bl 0x820bc908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC908, LIFTED_ARGS(0x820BD8E4, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BD8E4:
  // addi r1,r1,288
  r1 = uint64_t(int64_t(int64_t(r1) + 288));
  // lfd f29,-80(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -80)));
  // lfd f30,-72(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -72)));
  // lfd f31,-64(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BD8F8: lifted from generated/ by lift.py.
lifted::Result f820BD8F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820BD8F8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-64(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -64, std::bit_cast<uint64_t>(f29));
  // stfd f30,-56(r1)
  guest::StoreU64(base, uint32_t(r1) + -56, std::bit_cast<uint64_t>(f30));
  // stfd f31,-48(r1)
  guest::StoreU64(base, uint32_t(r1) + -48, std::bit_cast<uint64_t>(f31));
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r31)).shape_a_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BD91C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r31)).shape_b_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BD928, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r28,r3
  r28 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r4,r3
  r4 = r3;
  // mr r5,r28
  r5 = r28;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BD94C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82294170
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Dot(base, LIFTED_ARGS(0x820BD958, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // bl 0x820bab40
  // (left out: dead in C++)
  { const lifted::Result o = game::Shape::Radius(base, LIFTED_ARGS(0x820BD964, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // bl 0x820bab40
  // (left out: dead in C++)
  { const lifted::Result o = game::Shape::Radius(base, LIFTED_ARGS(0x820BD970, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f29,f1,f30
  lifted::ScalarFloatMode();
  f29 = double(float(f1 + f30));
  // fmuls f0,f29,f29
  f0 = double(float(f29 * f29));
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // bgt cr6,0x820bda30
  if (cr6.gt) goto loc_820BDA30;
  // fmr f1,f31
  f1 = f31;
  // bl 0x82293590
  // (left out: dead in C++)
  { const lifted::Result o = game::sqrtf(base, LIFTED_ARGS(0x820BD988, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // lfs f31,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f31,92(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // lfs f0,10712(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10712);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fcmpu cr6,f30,f0
  cr6 = lifted::CompareFloat(f30, f0);
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // bge cr6,0x820bd9c4
  if (!cr6.lt) goto loc_820BD9C4;
  // stfs f31,80(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // stfs f31,84(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // stfs f31,92(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // b 0x820bd9e0
  goto loc_820BD9E0;
loc_820BD9C4:
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // fdivs f1,f0,f30
  lifted::ScalarFloatMode();
  f1 = double(float(f0 / f30));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BD9D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BD9E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BD9E0:
  // mr r3,r29
  r3 = r29;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r29,r3
  r29 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r4,r3
  r4 = r3;
  // mr r5,r29
  r5 = r29;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BDA04, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfs f1,7140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f1 = double(temp.f32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BDA18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // fmr f2,f31
  lifted::ScalarFloatMode();
  f2 = f31;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // fsubs f1,f29,f30
  f1 = double(float(f29 - f30));
  // addi r3,r31,32
  r3 = uint64_t(int64_t(int64_t(r31) + 32));
  // bl 0x820bc908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC908, LIFTED_ARGS(0x820BDA30, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BDA30:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f29,-64(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // lfd f30,-56(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // lfd f31,-48(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BDA48: lifted from generated/ by lift.py.
lifted::Result f820BDA48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_820BDA48 (lifted)");
  uint64_t r4 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-56
  r12 = uint64_t(int64_t(int64_t(r1) + -56));
  // bl 0x825a7908
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-448(r1)
  ea = -448 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // lwz r3,0(r27)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r27)).shape_a_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BDA68, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r3,4(r27)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r27)).shape_b_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BDA74, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r4,r27,64
  r4 = uint64_t(int64_t(int64_t(r27) + 64));
  // lfs f1,80(r27)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r27)).contact_depth_address());
  f1 = double(temp.f32);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BDA88, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // lfs f29,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f29 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f27,7140(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 7140);
  f27 = double(temp.f32);
  // lfd f25,11160(r9)
  f25 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r9) + 11160)));
  // lfs f24,7136(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 7136);
  f24 = double(temp.f32);
  // stfs f29,220(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 220, temp.u32);
  // lfs f26,11156(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11156);
  f26 = double(temp.f32);
loc_820BDAB8:
  // stfs f29,188(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 188, temp.u32);
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r30)).vtable_address());
  // fmr f30,f26
  f30 = f26;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // fmr f28,f29
  f28 = f29;
  // mr r3,r30
  r3 = r30;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).edge_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDADC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820bdde8
  if (cr0.eq) goto loc_820BDDE8;
loc_820BDAE4:
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r30)).vtable_address());
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).edge_start_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDAFC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r30)).vtable_address());
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDB14, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BDB24, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r30)).vtable_address());
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // lwz r11,72(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).edge_end_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDB3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r30)).vtable_address());
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDB54, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BDB64, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r6,r1,192
  r6 = uint64_t(int64_t(int64_t(r1) + 192));
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r3,r31
  r3 = r31;
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).slot_7_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDB84, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820bdd9c
  if (cr0.eq) goto loc_820BDD9C;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).edge_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDBA4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820bdd9c
  if (cr0.eq) goto loc_820BDD9C;
loc_820BDBAC:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).edge_start_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDBC4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDBDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r9,0(r31)
  r9 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // ld r8,0(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 0);
  // lwz r9,72(r9)
  r9 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r9)).edge_end_address());
  // std r8,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r8);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r11);
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDC0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDC24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,160
  r10 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r9,0(r30)
  r9 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r30)).vtable_address());
  // addi r6,r1,160
  r6 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // ld r8,0(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r9,28(r9)
  r9 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r9)).slot_7_address());
  // std r8,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r8);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r11);
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDC5C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820bdd7c
  if (cr0.eq) goto loc_820BDD7C;
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,192
  r4 = uint64_t(int64_t(int64_t(r1) + 192));
  // addi r3,r1,224
  r3 = uint64_t(int64_t(int64_t(r1) + 224));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BDC74, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BDC84, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,256
  r5 = uint64_t(int64_t(int64_t(r1) + 256));
  // addi r4,r1,224
  r4 = uint64_t(int64_t(int64_t(r1) + 224));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294518
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294518, LIFTED_ARGS(0x820BDC94, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294170
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Dot(base, LIFTED_ARGS(0x820BDCA0, r31)); r3 = o.r3; f1 = o.f1; }
  // fcmpu cr6,f1,f29
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f29);
  // beq cr6,0x820bdd7c
  if (cr6.eq) goto loc_820BDD7C;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294298
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294298, LIFTED_ARGS(0x820BDCB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_direction_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDCC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BDCD4, r31)); r3 = o.r3; f1 = o.f1; }
  // fcmpu cr6,f1,f29
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f29);
  // ble cr6,0x820bdcf8
  if (!cr6.gt) goto loc_820BDCF8;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmr f1,f24
  f1 = f24;
  // addi r3,r1,304
  r3 = uint64_t(int64_t(int64_t(r1) + 304));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BDCEC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BDCF8, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BDCF8:
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r3,r1,272
  r3 = uint64_t(int64_t(int64_t(r1) + 272));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BDD08, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BDD10, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // fcmpu cr6,f31,f25
  cr6 = lifted::CompareFloat(f31, f25);
  // blt cr6,0x820bdd7c
  if (cr6.lt) goto loc_820BDD7C;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r27,96
  r3 = uint64_t(int64_t(int64_t(r27) + 96));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BDD28, r31)); r3 = o.r3; f1 = o.f1; }
  // fcmpu cr6,f1,f29
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f29);
  // ble cr6,0x820bdd7c
  if (!cr6.gt) goto loc_820BDD7C;
  // fdivs f0,f31,f1
  f0 = double(float(f31 / f1));
  // fcmpu cr6,f30,f0
  cr6 = lifted::CompareFloat(f30, f0);
  // ble cr6,0x820bdd7c
  if (!cr6.gt) goto loc_820BDD7C;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmr f30,f0
  f30 = f0;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // fmr f28,f31
  f28 = f31;
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BDD50, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,140(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 140);
  f0 = double(temp.f32);
  // lfs f13,136(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 136);
  f13 = double(temp.f32);
  // addi r6,r1,160
  r6 = uint64_t(int64_t(int64_t(r1) + 160));
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r3,r1,288
  r3 = uint64_t(int64_t(int64_t(r1) + 288));
  // fmuls f1,f0,f27
  f1 = double(float(f0 * f27));
  // bl 0x82294568
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Lerp(base, LIFTED_ARGS(0x820BDD70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BDD7C, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BDD7C:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).edge_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDD94, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r29,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r3), so);
  // blt cr6,0x820bdbac
  if (cr6.lt) goto loc_820BDBAC;
loc_820BDD9C:
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r30)).vtable_address());
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // mr r3,r30
  r3 = r30;
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).edge_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDDB4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r28,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r3), so);
  // blt cr6,0x820bdae4
  if (cr6.lt) goto loc_820BDAE4;
  // fcmpu cr6,f28,f29
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f28, f29);
  // ble cr6,0x820bdde8
  if (!cr6.gt) goto loc_820BDDE8;
  // addi r4,r1,176
  r4 = uint64_t(int64_t(int64_t(r1) + 176));
  // fmr f1,f28
  f1 = f28;
  // addi r3,r1,240
  r3 = uint64_t(int64_t(int64_t(r1) + 240));
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BDDD8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82294320
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::AddTo(base, LIFTED_ARGS(0x820BDDE4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820bdab8
  goto loc_820BDAB8;
loc_820BDDE8:
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x820bde2c
  if (cr6.eq) goto loc_820BDE2C;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82294270
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Length(base, LIFTED_ARGS(0x820BDDF8, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,240
  r3 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fdivs f1,f0,f31
  f1 = double(float(f0 / f31));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BDE14, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // fmr f2,f29
  lifted::ScalarFloatMode();
  f2 = f29;
  // addi r4,r1,208
  r4 = uint64_t(int64_t(int64_t(r1) + 208));
  // fmr f1,f31
  f1 = f31;
  // addi r3,r27,32
  r3 = uint64_t(int64_t(int64_t(r27) + 32));
  // bl 0x820bc908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC908, LIFTED_ARGS(0x820BDE2C, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BDE2C:
  // addi r1,r1,448
  r1 = uint64_t(int64_t(int64_t(r1) + 448));
  // addi r12,r1,-56
  r12 = uint64_t(int64_t(int64_t(r1) + -56));
  // bl 0x825a7954
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BDE40: lifted from generated/ by lift.py.
lifted::Result f820BDE40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_820BDE40 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-96
  r12 = uint64_t(int64_t(int64_t(r1) + -96));
  // bl 0x825a7910
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r24,r3
  r24 = r3;
  // lwz r3,0(r24)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r24)).shape_a_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BDE60, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // lwz r3,4(r24)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r24)).shape_b_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BDE6C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r4,r24,64
  r4 = uint64_t(int64_t(int64_t(r24) + 64));
  // lfs f1,80(r24)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r24)).contact_depth_address());
  f1 = double(temp.f32);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BDE80, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // li r21,-1
  r21 = uint64_t(int64_t(-1));
  // lfs f26,8748(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8748);
  f26 = double(temp.f32);
  // lfs f28,2612(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2612);
  f28 = double(temp.f32);
  // lfs f27,11156(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11156);
  f27 = double(temp.f32);
loc_820BDEA0:
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r27)).vtable_address());
  // li r23,0
  r23 = uint64_t(int64_t(0));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDEBC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820be068
  if (cr0.eq) goto loc_820BE068;
loc_820BDEC4:
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r27)).vtable_address());
  // mr r4,r26
  r4 = r26;
  // mr r3,r27
  r3 = r27;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDEDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BDEEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r31
  r3 = r31;
  // lwz r11,32(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).slot_8_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDF04, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820be040
  if (cr0.eq) goto loc_820BE040;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // li r23,1
  r23 = uint64_t(int64_t(1));
  // fmr f31,f27
  lifted::ScalarFloatMode();
  f31 = f27;
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // fmr f29,f28
  f29 = f28;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDF34, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820be040
  if (cr0.eq) goto loc_820BE040;
  // addi r25,r24,96
  r25 = uint64_t(int64_t(int64_t(r24) + 96));
loc_820BDF40:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDF58, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r25
  r3 = r25;
  // addi r4,r11,16
  r4 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BDF68, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // fcmpu cr6,f30,f28
  cr6 = lifted::CompareFloat(f30, f28);
  // ble cr6,0x820bdfdc
  if (!cr6.gt) goto loc_820BDFDC;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDF8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r29,r3
  r29 = r3;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDFA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r11,16
  r4 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BDFB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f0,f1
  f0 = double(float(f0 - f1));
  // fdivs f13,f0,f30
  f13 = double(float(f0 / f30));
  // fcmpu cr6,f31,f13
  cr6 = lifted::CompareFloat(f31, f13);
  // ble cr6,0x820bdfdc
  if (!cr6.gt) goto loc_820BDFDC;
  // fmr f31,f13
  f31 = f13;
  // mr r28,r30
  r28 = r30;
  // fmr f29,f0
  f29 = f0;
  // mr r21,r26
  r21 = r26;
loc_820BDFDC:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BDFF4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r30,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r3), so);
  // blt cr6,0x820bdf40
  if (cr6.lt) goto loc_820BDF40;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x820be040
  if (cr6.lt) goto loc_820BE040;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE01C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // fadds f1,f29,f26
  lifted::ScalarFloatMode();
  f1 = double(float(f29 + f26));
  // addi r4,r11,16
  r4 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BE030, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294320
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::AddTo(base, LIFTED_ARGS(0x820BE03C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r22,1
  r22 = uint64_t(int64_t(1));
loc_820BE040:
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r27)).vtable_address());
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // mr r3,r27
  r3 = r27;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE058, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r26,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r3), so);
  // blt cr6,0x820bdec4
  if (cr6.lt) goto loc_820BDEC4;
  // cmplwi cr6,r23,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 1, so);
  // beq cr6,0x820bdea0
  if (cr6.eq) goto loc_820BDEA0;
loc_820BE068:
  // cmplwi cr6,r22,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r22), 0, so);
  // beq cr6,0x820be0c8
  if (cr6.eq) goto loc_820BE0C8;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294270
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Length(base, LIFTED_ARGS(0x820BE078, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fdivs f1,f0,f31
  f1 = double(float(f0 / f31));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BE094, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r30,r3
  r30 = r3;
  // mr r4,r21
  r4 = r21;
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE0B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r30
  r5 = r30;
  // fmr f2,f28
  lifted::ScalarFloatMode();
  f2 = f28;
  // addi r3,r24,32
  r3 = uint64_t(int64_t(int64_t(r24) + 32));
  // fmr f1,f31
  f1 = f31;
  // bl 0x820bc908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC908, LIFTED_ARGS(0x820BE0C8, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BE0C8:
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // addi r12,r1,-96
  r12 = uint64_t(int64_t(int64_t(r1) + -96));
  // bl 0x825a795c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BE0D8: lifted from generated/ by lift.py.
void f820BE0D8(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1, double f2) {
  ITC_NATIVE_RUNNING("sub_820BE0D8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x820be114
  if (!cr6.gt) goto loc_820BE114;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r3)
  temp.u32 = guest::LoadU32(base, schema::BoxExitBest(base, uint32_t(r3)).fraction_address());
  f13 = double(temp.f32);
  // fmuls f13,f13,f1
  f13 = double(float(f13 * f1));
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fsubs f0,f0,f2
  f0 = double(float(f0 - f2));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blelr cr6
  if (!cr6.gt) return;
  // fdivs f0,f0,f1
  f0 = double(float(f0 / f1));
  // stfs f0,0(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::BoxExitBest(base, uint32_t(r3)).fraction_address(), temp.u32);
  // stw r7,0(r4)
  guest::StoreU32(base, schema::BoxExitFace(base, uint32_t(r4)).face_address(), uint32_t(r7));
  // blr
  return;
loc_820BE114:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // bgelr cr6
  if (!cr6.lt) return;
  // lfs f13,0(r3)
  temp.u32 = guest::LoadU32(base, schema::BoxExitBest(base, uint32_t(r3)).fraction_address());
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f13,f1
  f13 = double(float(f13 * f1));
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fadds f0,f2,f0
  f0 = double(float(f2 + f0));
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blelr cr6
  if (!cr6.gt) return;
  // fdivs f0,f0,f1
  f0 = double(float(f0 / f1));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,0(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::BoxExitBest(base, uint32_t(r3)).fraction_address(), temp.u32);
  // stw r8,0(r4)
  guest::StoreU32(base, schema::BoxExitFace(base, uint32_t(r4)).face_address(), uint32_t(r8));
  // blr
  return;
}

// sub_820BE4F0: lifted from generated/ by lift.py.
lifted::Result f820BE4F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820BE4F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-88
  r12 = uint64_t(int64_t(int64_t(r1) + -88));
  // bl 0x825a790c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1072(r1)
  ea = -1072 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r23,r3
  r23 = r3;
  // lwz r3,0(r23)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r23)).shape_a_address());
  // bl 0x820911f8
  // (left out: dead in C++)
  r3 = f820911F8(base, r3);
  // mr r28,r3
  r28 = r3;
  // lwz r3,0(r23)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r23)).shape_a_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BE51C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r22,r3
  r22 = r3;
  // lwz r3,4(r23)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r23)).shape_b_address());
  // bl 0x820911f8
  // (left out: dead in C++)
  r3 = f820911F8(base, r3);
  // mr r31,r3
  r31 = r3;
  // lwz r3,4(r23)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r23)).shape_b_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BE534, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r26,r3
  r26 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE54C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r28)).vtable_address());
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE564, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820be84c
  if (cr0.eq) goto loc_820BE84C;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f25,7136(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 7136);
  f25 = double(temp.f32);
  // lfs f26,2608(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f26 = double(temp.f32);
  // lfs f27,10820(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 10820);
  f27 = double(temp.f32);
  // lfs f28,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f28 = double(temp.f32);
loc_820BE58C:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r28)).vtable_address());
  // mr r4,r25
  r4 = r25;
  // mr r3,r28
  r3 = r28;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE5A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r9,0(r22)
  r9 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r22)).vtable_address());
  // mr r4,r25
  r4 = r25;
  // mr r3,r22
  r3 = r22;
  // ld r8,0(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 0);
  // lwz r9,48(r9)
  r9 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r9)).corner_address());
  // std r8,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r8);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r11);
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE5D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r9,0(r31)
  r9 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // ld r8,0(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 0);
  // lwz r9,52(r9)
  r9 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r9)).face_count_address());
  // std r8,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r8);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r11);
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE604, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820be6ec
  if (cr0.eq) goto loc_820BE6EC;
  // addi r29,r1,128
  r29 = uint64_t(int64_t(int64_t(r1) + 128));
loc_820BE610:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE628, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,16
  r4 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BE638, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE654, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r10,0(r26)
  r10 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r26)).vtable_address());
  // mr r4,r30
  r4 = r30;
  // mr r3,r26
  r3 = r26;
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f31,f0
  f0 = double(float(f31 - f0));
  // lwz r10,56(r10)
  r10 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r10)).face_address());
  // stfs f0,0(r29)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r29) + 0, temp.u32);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE67C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r11,16
  r4 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BE68C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r26)).vtable_address());
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // mr r4,r30
  r4 = r30;
  // mr r3,r26
  r3 = r26;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE6A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f28
  cr6 = lifted::CompareFloat(f0, f28);
  // lfs f0,0(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f31,f0
  f0 = double(float(f31 - f0));
  // stfs f0,4(r29)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r29) + 4, temp.u32);
  // ble cr6,0x820be6c8
  if (!cr6.gt) goto loc_820BE6C8;
  // fcmpu cr6,f0,f28
  cr6 = lifted::CompareFloat(f0, f28);
  // bgt cr6,0x820be82c
  if (cr6.gt) goto loc_820BE82C;
loc_820BE6C8:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,8
  r29 = uint64_t(int64_t(int64_t(r29) + 8));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE6E4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r30,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r3), so);
  // blt cr6,0x820be610
  if (cr6.lt) goto loc_820BE610;
loc_820BE6EC:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE704, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820be82c
  if (cr0.eq) goto loc_820BE82C;
  // addi r24,r1,132
  r24 = uint64_t(int64_t(int64_t(r1) + 132));
loc_820BE710:
  // lfs f29,0(r24)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r24) + 0);
  f29 = double(temp.f32);
  // fcmpu cr6,f29,f28
  cr6 = lifted::CompareFloat(f29, f28);
  // bgt cr6,0x820be808
  if (cr6.gt) goto loc_820BE808;
  // lfs f31,-4(r24)
  temp.u32 = guest::LoadU32(base, uint32_t(r24) + -4);
  f31 = double(temp.f32);
  // fsubs f0,f31,f29
  f0 = double(float(f31 - f29));
  // fabs f13,f0
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // fcmpu cr6,f13,f27
  cr6 = lifted::CompareFloat(f13, f27);
  // blt cr6,0x820be808
  if (cr6.lt) goto loc_820BE808;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // fdivs f30,f31,f0
  f30 = double(float(f31 / f0));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE750, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820be7a4
  if (cr0.eq) goto loc_820BE7A4;
loc_820BE758:
  // cmpw cr6,r29,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r30), so);
  // beq cr6,0x820be77c
  if (cr6.eq) goto loc_820BE77C;
  // fmr f3,f29
  lifted::ScalarFloatMode();
  f3 = f29;
  // fmr f2,f31
  f2 = f31;
  // fmr f1,f30
  f1 = f30;
  // bl 0x822945f8
  // (left out: dead in C++)
  f1 = f822945F8(base, f1, f2, f3);
  // fcmpu cr6,f1,f28
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f28);
  // ble cr6,0x820be77c
  if (!cr6.gt) goto loc_820BE77C;
  // li r27,0
  r27 = uint64_t(int64_t(0));
loc_820BE77C:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE794, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r30,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r3), so);
  // blt cr6,0x820be758
  if (cr6.lt) goto loc_820BE758;
  // cmplwi cr6,r27,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 1, so);
  // bne cr6,0x820be808
  if (!cr6.eq) goto loc_820BE808;
loc_820BE7A4:
  // fcmpu cr6,f30,f26
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f30, f26);
  // bge cr6,0x820be808
  if (!cr6.lt) goto loc_820BE808;
  // lfs f0,84(r23)
  temp.u32 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r23)).contact_time_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f30,f0
  cr6 = lifted::CompareFloat(f30, f0);
  // bge cr6,0x820be808
  if (!cr6.lt) goto loc_820BE808;
  // fcmpu cr6,f30,f25
  cr6 = lifted::CompareFloat(f30, f25);
  // ble cr6,0x820be808
  if (!cr6.gt) goto loc_820BE808;
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r26)).vtable_address());
  // mr r4,r29
  r4 = r29;
  // mr r3,r26
  r3 = r26;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE7D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82294568
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Lerp(base, LIFTED_ARGS(0x820BE7F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r30,16
  r5 = uint64_t(int64_t(int64_t(r30) + 16));
  // fmr f2,f30
  lifted::ScalarFloatMode();
  f2 = f30;
  // addi r3,r23,32
  r3 = uint64_t(int64_t(int64_t(r23) + 32));
  // fneg f1,f29
  f1 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f29) ^ 0x8000000000000000));
  // bl 0x820bc908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC908, LIFTED_ARGS(0x820BE808, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BE808:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r24,r24,8
  r24 = uint64_t(int64_t(int64_t(r24) + 8));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE824, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r29,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r3), so);
  // blt cr6,0x820be710
  if (cr6.lt) goto loc_820BE710;
loc_820BE82C:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r28)).vtable_address());
  // addi r25,r25,1
  r25 = uint64_t(int64_t(int64_t(r25) + 1));
  // mr r3,r28
  r3 = r28;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).corner_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE844, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r25,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), uint32_t(r3), so);
  // blt cr6,0x820be58c
  if (cr6.lt) goto loc_820BE58C;
loc_820BE84C:
  // addi r1,r1,1072
  r1 = uint64_t(int64_t(int64_t(r1) + 1072));
  // addi r12,r1,-88
  r12 = uint64_t(int64_t(int64_t(r1) + -88));
  // bl 0x825a7958
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BE860: lifted from generated/ by lift.py.
lifted::Result f820BE860(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820BE860 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // bl 0x820be4f0
  // (left out: dead in C++)
  { const lifted::Result o = f820BE4F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820be490
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BE490, LIFTED_ARGS(0x820BE880, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820be4f0
  // (left out: dead in C++)
  { const lifted::Result o = f820BE4F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820be490
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BE490, LIFTED_ARGS(0x820BE890, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820BE8A8: lifted from generated/ by lift.py.
lifted::Result f820BE8A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820BE8A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-80
  r12 = uint64_t(int64_t(int64_t(r1) + -80));
  // bl 0x825a790c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1040(r1)
  ea = -1040 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r3
  r26 = r3;
  // lwz r3,0(r26)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r26)).shape_a_address());
  // bl 0x820911f8
  // (left out: dead in C++)
  r3 = f820911F8(base, r3);
  // mr r25,r3
  r25 = r3;
  // lwz r3,0(r26)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r26)).shape_a_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BE8D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // lwz r3,4(r26)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r26)).shape_b_address());
  // bl 0x820911f8
  // (left out: dead in C++)
  r3 = f820911F8(base, r3);
  // mr r31,r3
  r31 = r3;
  // lwz r3,4(r26)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r26)).shape_b_address());
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BE8EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r28,r3
  r28 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE904, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE91C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // lfs f28,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f28 = double(temp.f32);
  // beq 0x820bea30
  if (cr0.eq) goto loc_820BEA30;
  // addi r29,r1,96
  r29 = uint64_t(int64_t(int64_t(r1) + 96));
loc_820BE930:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE948, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // mr r3,r25
  r3 = r25;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // addi r4,r27,16
  r4 = uint64_t(int64_t(int64_t(r27) + 16));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BE95C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE978, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // mr r3,r25
  r3 = r25;
  // bl 0x820bab40
  // (left out: dead in C++)
  { const lifted::Result o = game::Shape::Radius(base, LIFTED_ARGS(0x820BE984, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r27)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r27) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f31,f0
  f0 = double(float(f31 - f0));
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r28)).vtable_address());
  // mr r4,r30
  r4 = r30;
  // mr r3,r28
  r3 = r28;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // fsubs f0,f0,f1
  f0 = double(float(f0 - f1));
  // stfs f0,0(r29)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r29) + 0, temp.u32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE9AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // mr r3,r24
  r3 = r24;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // addi r4,r27,16
  r4 = uint64_t(int64_t(int64_t(r27) + 16));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BE9C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r28)).vtable_address());
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // mr r4,r30
  r4 = r30;
  // mr r3,r28
  r3 = r28;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BE9DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // mr r3,r24
  r3 = r24;
  // bl 0x820bab40
  // (left out: dead in C++)
  { const lifted::Result o = game::Shape::Radius(base, LIFTED_ARGS(0x820BE9E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r27)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r27) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f31,f0
  f0 = double(float(f31 - f0));
  // lfs f13,0(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f28
  cr6 = lifted::CompareFloat(f13, f28);
  // fsubs f0,f0,f1
  f0 = double(float(f0 - f1));
  // stfs f0,4(r29)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r29) + 4, temp.u32);
  // ble cr6,0x820bea0c
  if (!cr6.gt) goto loc_820BEA0C;
  // fcmpu cr6,f0,f28
  cr6 = lifted::CompareFloat(f0, f28);
  // bgt cr6,0x820beb9c
  if (cr6.gt) goto loc_820BEB9C;
loc_820BEA0C:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,8
  r29 = uint64_t(int64_t(int64_t(r29) + 8));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BEA28, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r30,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r3), so);
  // blt cr6,0x820be930
  if (cr6.lt) goto loc_820BE930;
loc_820BEA30:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BEA48, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820beb9c
  if (cr0.eq) goto loc_820BEB9C;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r23,r1,100
  r23 = uint64_t(int64_t(int64_t(r1) + 100));
  // lfs f25,7136(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 7136);
  f25 = double(temp.f32);
  // lfs f26,2608(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2608);
  f26 = double(temp.f32);
  // lfs f27,10820(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10820);
  f27 = double(temp.f32);
loc_820BEA6C:
  // lfs f29,0(r23)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r23) + 0);
  f29 = double(temp.f32);
  // fcmpu cr6,f29,f28
  cr6 = lifted::CompareFloat(f29, f28);
  // bgt cr6,0x820beb78
  if (cr6.gt) goto loc_820BEB78;
  // lfs f31,-4(r23)
  temp.u32 = guest::LoadU32(base, uint32_t(r23) + -4);
  f31 = double(temp.f32);
  // fsubs f0,f31,f29
  f0 = double(float(f31 - f29));
  // fabs f13,f0
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // fcmpu cr6,f13,f27
  cr6 = lifted::CompareFloat(f13, f27);
  // blt cr6,0x820beb78
  if (cr6.lt) goto loc_820BEB78;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // fdivs f30,f31,f0
  f30 = double(float(f31 / f0));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BEAAC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820beb00
  if (cr0.eq) goto loc_820BEB00;
loc_820BEAB4:
  // cmpw cr6,r27,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r30), so);
  // beq cr6,0x820bead8
  if (cr6.eq) goto loc_820BEAD8;
  // fmr f3,f29
  lifted::ScalarFloatMode();
  f3 = f29;
  // fmr f2,f31
  f2 = f31;
  // fmr f1,f30
  f1 = f30;
  // bl 0x822945f8
  // (left out: dead in C++)
  f1 = f822945F8(base, f1, f2, f3);
  // fcmpu cr6,f1,f28
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f28);
  // ble cr6,0x820bead8
  if (!cr6.gt) goto loc_820BEAD8;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_820BEAD8:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BEAF0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r30,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r3), so);
  // blt cr6,0x820beab4
  if (cr6.lt) goto loc_820BEAB4;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x820beb78
  if (cr6.eq) goto loc_820BEB78;
loc_820BEB00:
  // fcmpu cr6,f30,f26
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f30, f26);
  // bge cr6,0x820beb78
  if (!cr6.lt) goto loc_820BEB78;
  // lfs f0,84(r26)
  temp.u32 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r26)).contact_time_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f30,f0
  cr6 = lifted::CompareFloat(f30, f0);
  // bge cr6,0x820beb78
  if (!cr6.lt) goto loc_820BEB78;
  // fcmpu cr6,f30,f25
  cr6 = lifted::CompareFloat(f30, f25);
  // ble cr6,0x820beb78
  if (!cr6.gt) goto loc_820BEB78;
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r28)).vtable_address());
  // mr r4,r27
  r4 = r27;
  // mr r3,r28
  r3 = r28;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BEB34, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r3,r24
  r3 = r24;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r29,r3
  r29 = r3;
  // mr r3,r25
  r3 = r25;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r5,r3
  r5 = r3;
  // mr r6,r29
  r6 = r29;
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82294568
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Lerp(base, LIFTED_ARGS(0x820BEB60, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r5,r30,16
  r5 = uint64_t(int64_t(int64_t(r30) + 16));
  // fmr f2,f30
  lifted::ScalarFloatMode();
  f2 = f30;
  // addi r3,r26,32
  r3 = uint64_t(int64_t(int64_t(r26) + 32));
  // fneg f1,f29
  f1 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f29) ^ 0x8000000000000000));
  // bl 0x820bc908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC908, LIFTED_ARGS(0x820BEB78, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BEB78:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::BoxShape(base, uint32_t(r31)).vtable_address());
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r23,r23,8
  r23 = uint64_t(int64_t(int64_t(r23) + 8));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, schema::BoxShapeVtable(base, uint32_t(r11)).face_count_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820BEB94, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r27,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r3), so);
  // blt cr6,0x820bea6c
  if (cr6.lt) goto loc_820BEA6C;
loc_820BEB9C:
  // addi r1,r1,1040
  r1 = uint64_t(int64_t(int64_t(r1) + 1040));
  // addi r12,r1,-80
  r12 = uint64_t(int64_t(int64_t(r1) + -80));
  // bl 0x825a7958
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BEBB0: lifted from generated/ by lift.py.
lifted::Result f820BEBB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820BEBB0 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // bl 0x820be490
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BE490, LIFTED_ARGS(0x820BEBC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820be8a8
  // (left out: dead in C++)
  { const lifted::Result o = f820BE8A8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820be490
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BE490, LIFTED_ARGS(0x820BEBD8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_820BEC50: lifted from generated/ by lift.py.
lifted::Result f820BEC50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_820BEC50 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
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
  // bl 0x820bde40
  // (left out: dead in C++)
  { const lifted::Result o = f820BDE40(base, r1, r13, r3, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820bebf0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BEBF0, LIFTED_ARGS(0x820BEC70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820bde40
  // (left out: dead in C++)
  { const lifted::Result o = f820BDE40(base, r1, r13, r3, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820bebf0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BEBF0, LIFTED_ARGS(0x820BEC80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820bda48
  // (left out: dead in C++)
  { const lifted::Result o = f820BDA48(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
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

// sub_820BF690: lifted from generated/ by lift.py.
lifted::Result f820BF690(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820BF690 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // stwu r1,-864(r1)
  ea = -864 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r4
  r26 = r4;
  // mr r27,r3
  r27 = r3;
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x820bf6c0
  if (cr6.eq) goto loc_820BF6C0;
  // lwz r3,4(r27)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r27)).shape_b_address());
  // lwz r28,0(r27)
  r28 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r27)).shape_a_address());
  // b 0x820bf6c8
  goto loc_820BF6C8;
loc_820BF6C0:
  // lwz r3,0(r27)
  r3 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r27)).shape_a_address());
  // lwz r28,4(r27)
  r28 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r27)).shape_b_address());
loc_820BF6C8:
  // bl 0x822be2c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822BE2C8(base, LIFTED_ARGS(0x820BF6CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r3,r28
  r3 = r28;
  // bl 0x820bae38
  // (left out: dead in C++)
  { const lifted::Result o = game::CollisionShape::Current(base, LIFTED_ARGS(0x820BF6D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r31,r3
  r31 = r3;
  // mr r3,r29
  r3 = r29;
  // bl 0x820b97c0
  // (left out: dead in C++)
  r3 = f820B97C0(base, r3);
  // mr r5,r3
  r5 = r3;
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x822942e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Difference(base, LIFTED_ARGS(0x820BF700, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x82294170
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Dot(base, LIFTED_ARGS(0x820BF70C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // bl 0x820bab40
  // (left out: dead in C++)
  { const lifted::Result o = game::Shape::Radius(base, LIFTED_ARGS(0x820BF718, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // bl 0x820bab40
  // (left out: dead in C++)
  { const lifted::Result o = game::Shape::Radius(base, LIFTED_ARGS(0x820BF724, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f0,f1,f30
  lifted::ScalarFloatMode();
  f0 = double(float(f1 + f30));
  // fmuls f0,f0,f0
  f0 = double(float(f0 * f0));
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // bgt cr6,0x820bfb14
  if (cr6.gt) goto loc_820BFB14;
  // fmr f1,f31
  f1 = f31;
  // bl 0x82293590
  // (left out: dead in C++)
  { const lifted::Result o = game::sqrtf(base, LIFTED_ARGS(0x820BF73C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f28,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f28 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f28,140(r1)
  temp.f32 = float(f28);
  guest::StoreU32(base, uint32_t(r1) + 140, temp.u32);
  // lfs f0,10712(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10712);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // addi r11,r11,2608
  r11 = uint64_t(int64_t(int64_t(r11) + 2608));
  // lfs f31,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f31 = double(temp.f32);
  // bge cr6,0x820bf778
  if (!cr6.lt) goto loc_820BF778;
  // stfs f28,128(r1)
  temp.f32 = float(f28);
  guest::StoreU32(base, uint32_t(r1) + 128, temp.u32);
  // stfs f31,132(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 132, temp.u32);
  // stfs f28,136(r1)
  temp.f32 = float(f28);
  guest::StoreU32(base, uint32_t(r1) + 136, temp.u32);
  // stfs f28,140(r1)
  temp.f32 = float(f28);
  guest::StoreU32(base, uint32_t(r1) + 140, temp.u32);
  // b 0x820bf794
  goto loc_820BF794;
loc_820BF778:
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // fdivs f1,f31,f1
  lifted::ScalarFloatMode();
  f1 = double(float(f31 / f1));
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BF788, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x822940e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Copy(base, LIFTED_ARGS(0x820BF794, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BF794:
  // mr r3,r29
  r3 = r29;
  // bl 0x82379328
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82379328, LIFTED_ARGS(0x820BF79C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r3,r28
  r3 = r28;
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, schema::CollisionVector(base, uint32_t(r11)).xy_address());
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, schema::CollisionVector(base, uint32_t(r11)).zw_address());
  // std r9,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r9);
  // std r11,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r11);
  // lfs f0,112(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 112);
  f0 = double(temp.f32);
  // lfs f13,116(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 116);
  f13 = double(temp.f32);
  // fdivs f11,f30,f0
  f11 = double(float(f30 / f0));
  // lfs f12,120(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 120);
  f12 = double(temp.f32);
  // fdivs f10,f30,f13
  f10 = double(float(f30 / f13));
  // fdivs f9,f30,f12
  f9 = double(float(f30 / f12));
  // stfs f28,156(r1)
  temp.f32 = float(f28);
  guest::StoreU32(base, uint32_t(r1) + 156, temp.u32);
  // fadds f11,f11,f31
  f11 = double(float(f11 + f31));
  // fadds f10,f10,f31
  f10 = double(float(f10 + f31));
  // fadds f9,f9,f31
  f9 = double(float(f9 + f31));
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // stfs f0,144(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 144, temp.u32);
  // fmuls f0,f10,f13
  f0 = double(float(f10 * f13));
  // stfs f0,148(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 148, temp.u32);
  // fmuls f0,f9,f12
  f0 = double(float(f9 * f12));
  // stfs f0,152(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 152, temp.u32);
  // bl 0x820bae38
  // (left out: dead in C++)
  { const lifted::Result o = game::CollisionShape::Current(base, LIFTED_ARGS(0x820BF7FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x820bb688
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BB688, LIFTED_ARGS(0x820BF808, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,144
  r4 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x820ba688
  // (left out: dead in C++)
  { const lifted::Result o = f820BA688(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x820ba1d0
  // (left out: dead in C++)
  { const lifted::Result o = f820BA1D0(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // beq 0x820bfb10
  if (cr0.eq) goto loc_820BFB10;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f28,108(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f28);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lfs f0,11156(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11156);
  f0 = double(temp.f32);
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x820b9ee8
  // (left out: dead in C++)
  { const lifted::Result o = game::BoxShape::Face(base, LIFTED_ARGS(0x820BF84C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r27,96
  r30 = uint64_t(int64_t(int64_t(r27) + 96));
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // mr r4,r30
  r4 = r30;
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BF85C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // bl 0x820b6890
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820B6890(base, LIFTED_ARGS(0x820BF868, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionVector(base, uint32_t(r11)).x_address());
  f0 = double(temp.f32);
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // bl 0x820b9ee8
  // (left out: dead in C++)
  { const lifted::Result o = game::BoxShape::Face(base, LIFTED_ARGS(0x820BF884, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BF890, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // bl 0x820b6890
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820B6890(base, LIFTED_ARGS(0x820BF89C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // lfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionVector(base, uint32_t(r11)).y_address());
  f0 = double(temp.f32);
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // bl 0x820b9ee8
  // (left out: dead in C++)
  { const lifted::Result o = game::BoxShape::Face(base, LIFTED_ARGS(0x820BF8B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // bl 0x82294608
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::DotOf(base, LIFTED_ARGS(0x820BF8C4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // bl 0x820b6890
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820B6890(base, LIFTED_ARGS(0x820BF8D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r5,r31
  r5 = r31;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // lfs f0,8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionVector(base, uint32_t(r11)).z_address());
  f0 = double(temp.f32);
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // bl 0x820ba3b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BA3B0, LIFTED_ARGS(0x820BF8F0, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f29,f1
  lifted::ScalarFloatMode();
  f29 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r5,r31
  r5 = r31;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // lfs f30,8680(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8680);
  f30 = double(temp.f32);
  // fmuls f27,f29,f30
  f27 = double(float(f29 * f30));
  // bl 0x820ba3b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BA3B0, LIFTED_ARGS(0x820BF910, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f0,f1,f29
  lifted::ScalarFloatMode();
  f0 = double(float(f1 + f29));
  // mr r5,r31
  r5 = r31;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // fdivs f0,f27,f0
  f0 = double(float(f27 / f0));
  // fsubs f27,f31,f0
  f27 = double(float(f31 - f0));
  // bl 0x820ba3b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BA3B0, LIFTED_ARGS(0x820BF92C, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f29,f1
  lifted::ScalarFloatMode();
  f29 = f1;
  // mr r5,r31
  r5 = r31;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // fmuls f26,f29,f30
  f26 = double(float(f29 * f30));
  // bl 0x820ba3b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BA3B0, LIFTED_ARGS(0x820BF944, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f0,f1,f29
  lifted::ScalarFloatMode();
  f0 = double(float(f1 + f29));
  // mr r5,r31
  r5 = r31;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // fdivs f0,f26,f0
  f0 = double(float(f26 / f0));
  // fsubs f26,f31,f0
  f26 = double(float(f31 - f0));
  // bl 0x820ba3b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BA3B0, LIFTED_ARGS(0x820BF960, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f29,f1
  lifted::ScalarFloatMode();
  f29 = f1;
  // mr r5,r31
  r5 = r31;
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // fmuls f30,f29,f30
  f30 = double(float(f29 * f30));
  // bl 0x820ba3b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BA3B0, LIFTED_ARGS(0x820BF978, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f0,f1,f29
  lifted::ScalarFloatMode();
  f0 = double(float(f1 + f29));
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // fdivs f0,f30,f0
  f0 = double(float(f30 / f0));
  // fsubs f31,f31,f0
  f31 = double(float(f31 - f0));
  // beq cr6,0x820bf99c
  if (cr6.eq) goto loc_820BF99C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f1,7136(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7136);
  f1 = double(temp.f32);
  // bl 0x82294340
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294340, LIFTED_ARGS(0x820BF99C, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BF99C:
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // lfs f1,96(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 96);
  f1 = double(temp.f32);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // fmr f2,f27
  f2 = f27;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,84
  r3 = uint64_t(int64_t(int64_t(r1) + 84));
  // bl 0x820be0d8
  // (left out: dead in C++)
  f820BE0D8(base, r3, r4, r7, r8, f1, f2);
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // lfs f1,100(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 100);
  f1 = double(temp.f32);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmr f2,f26
  f2 = f26;
  // addi r3,r1,84
  r3 = uint64_t(int64_t(int64_t(r1) + 84));
  // bl 0x820be0d8
  // (left out: dead in C++)
  f820BE0D8(base, r3, r4, r7, r8, f1, f2);
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // lfs f1,104(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 104);
  f1 = double(temp.f32);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmr f2,f31
  f2 = f31;
  // addi r3,r1,84
  r3 = uint64_t(int64_t(int64_t(r1) + 84));
  // bl 0x820be0d8
  // (left out: dead in C++)
  f820BE0D8(base, r3, r4, r7, r8, f1, f2);
  // lfs f1,84(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f1 = double(temp.f32);
  // fcmpu cr6,f1,f28
  cr6 = lifted::CompareFloat(f1, f28);
  // ble cr6,0x820bfb0c
  if (!cr6.gt) goto loc_820BFB0C;
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BFA08, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x82379328
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82379328, LIFTED_ARGS(0x820BFA10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x820b9ee8
  // (left out: dead in C++)
  { const lifted::Result o = game::BoxShape::Face(base, LIFTED_ARGS(0x820BFA20, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lfs f0,8(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionVector(base, uint32_t(r30)).z_address());
  f0 = double(temp.f32);
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // lfs f13,120(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 120);
  f13 = double(temp.f32);
  // addi r4,r11,16
  r4 = uint64_t(int64_t(int64_t(r11) + 16));
  // fmuls f1,f0,f13
  f1 = double(float(f0 * f13));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BFA3C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x82379328
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82379328, LIFTED_ARGS(0x820BFA48, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x820b9ee8
  // (left out: dead in C++)
  { const lifted::Result o = game::BoxShape::Face(base, LIFTED_ARGS(0x820BFA58, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lfs f0,4(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionVector(base, uint32_t(r29)).y_address());
  f0 = double(temp.f32);
  // addi r3,r1,240
  r3 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfs f13,116(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 116);
  f13 = double(temp.f32);
  // addi r4,r11,16
  r4 = uint64_t(int64_t(int64_t(r11) + 16));
  // fmuls f1,f0,f13
  f1 = double(float(f0 * f13));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BFA74, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x82379328
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82379328, LIFTED_ARGS(0x820BFA80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x820b9ee8
  // (left out: dead in C++)
  { const lifted::Result o = game::BoxShape::Face(base, LIFTED_ARGS(0x820BFA90, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lfs f0,0(r28)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CollisionVector(base, uint32_t(r28)).x_address());
  f0 = double(temp.f32);
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfs f13,112(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 112);
  f13 = double(temp.f32);
  // addi r4,r11,16
  r4 = uint64_t(int64_t(int64_t(r11) + 16));
  // fmuls f1,f0,f13
  f1 = double(float(f0 * f13));
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec4::Scaled(base, LIFTED_ARGS(0x820BFAAC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r29
  r5 = r29;
  // addi r3,r1,224
  r3 = uint64_t(int64_t(int64_t(r1) + 224));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BFABC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r30
  r5 = r30;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Sum(base, LIFTED_ARGS(0x820BFACC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x82294270
  // (left out: dead in C++)
  { const lifted::Result o = game::Vec3::Length(base, LIFTED_ARGS(0x820BFAD4, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // lfs f0,80(r27)
  temp.u32 = guest::LoadU32(base, schema::CollisionQuery(base, uint32_t(r27)).contact_depth_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // ble cr6,0x820bfb0c
  if (!cr6.gt) goto loc_820BFB0C;
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // lwz r4,80(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x820b9ee8
  // (left out: dead in C++)
  { const lifted::Result o = game::BoxShape::Face(base, LIFTED_ARGS(0x820BFAF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r4,r31
  r4 = r31;
  // fmr f2,f28
  lifted::ScalarFloatMode();
  f2 = f28;
  // addi r3,r27,32
  r3 = uint64_t(int64_t(int64_t(r27) + 32));
  // fmr f1,f31
  f1 = f31;
  // addi r5,r11,16
  r5 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x820bc908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BC908, LIFTED_ARGS(0x820BFB0C, r31)); r3 = o.r3; f1 = o.f1; }
loc_820BFB0C:
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
loc_820BFB10:
  // bl 0x820b9c30
  // (left out: dead in C++)
  f820B9C30(base, r3);
loc_820BFB14:
  // addi r1,r1,864
  r1 = uint64_t(int64_t(int64_t(r1) + 864));
  // addi r12,r1,-56
  r12 = uint64_t(int64_t(int64_t(r1) + -56));
  // bl 0x825a795c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820BFBA8: lifted from generated/ by lift.py.
lifted::Result f820BFBA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820BFBA8 (lifted)");
  uint64_t r4 = 0;
  double f1 = 0.0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x820bf690
  { const lifted::Result o = f820BF690(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820BFBB0: lifted from generated/ by lift.py.
lifted::Result f820BFBB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820BFBB0 (lifted)");
  uint64_t r4 = 0;
  double f1 = 0.0;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // b 0x820bf690
  { const lifted::Result o = f820BF690(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820C4E68 game::CarScene::GetTga: lifted from generated/ by lift.py.
uint64_t f820C4E68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820C4E68 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r3,r4
  r3 = r4;
  // mr r30,r5
  r30 = r5;
  // addi r4,r31,992
  r4 = uint64_t(int64_t(int64_t(r31) + 992));
  // li r5,20
  r5 = uint64_t(int64_t(20));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x820C4E94, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,1012(r31)
  r11 = guest::LoadU32(base, schema::CarScene(base, uint32_t(r31)).tga_pixels_address());
  // stw r11,0(r30)
  guest::StoreU32(base, schema::ScenePagesWord(base, uint32_t(r30)).pages_address(), uint32_t(r11));
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

// sub_820C9D90 game::Params::Free: lifted from generated/ by lift.py.
lifted::Result f820C9D90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820C9D90 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,11848(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 11848);
  // lwz r3,4(r11)
  r3 = guest::LoadU32(base, schema::CarParams(base, uint32_t(r11)).block_address());
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x820C9DB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,11848(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 11848);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820C9DB8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,11848(r31)
  guest::StoreU32(base, uint32_t(r31) + 11848, uint32_t(r11));
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

// sub_820CE7F0: lifted from generated/ by lift.py.
void f820CE7F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820CE7F0 (lifted)");
  double f0 = 0.0, f1 = 0.0;
  lifted::Temp temp{};
  // lfs f0,20(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CarSpecCopiesAt992(base, uint32_t(r4)).unknown_20_address());
  f0 = double(temp.f32);
  // stfs f0,1044(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::StagePowerCurve(base, uint32_t(r3)).highest_rpm_address(), temp.u32);
  // b 0x820ce600
  f820CE600(base, r1, r13, r3, r4);
  return;
}

// sub_820CEC20: lifted from generated/ by lift.py.
lifted::Result f820CEC20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820CEC20 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r10,r11,13196
  r10 = uint64_t(int64_t(int64_t(r11) + 13196));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r30,r4
  r30 = r4;
  // stw r10,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r10));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r11,444(r31)
  guest::StoreU32(base, uint32_t(r31) + 444, uint32_t(r11));
  // stw r11,448(r31)
  guest::StoreU32(base, uint32_t(r31) + 448, uint32_t(r11));
  // stw r11,452(r31)
  guest::StoreU32(base, uint32_t(r31) + 452, uint32_t(r11));
  // stw r11,456(r31)
  guest::StoreU32(base, uint32_t(r31) + 456, uint32_t(r11));
  // stw r11,460(r31)
  guest::StoreU32(base, uint32_t(r31) + 460, uint32_t(r11));
  // stw r11,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r11));
  // stw r10,436(r31)
  guest::StoreU32(base, uint32_t(r31) + 436, uint32_t(r10));
  // stw r11,440(r31)
  guest::StoreU32(base, uint32_t(r31) + 440, uint32_t(r11));
  // bl 0x82594f58
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::Dtor(base, LIFTED_ARGS(0x820CEC74, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820cec84
  if (cr0.eq) goto loc_820CEC84;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820CEC84, r31)); r3 = o.r3; f1 = o.f1; }
loc_820CEC84:
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

// sub_820D39D8: lifted from generated/ by lift.py.
void f820D39D8(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820D39D8 (lifted)");
  uint64_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, ctr = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // addi r9,r3,-18
  r9 = uint64_t(int64_t(int64_t(r3) + -18));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // cmplwi cr6,r9,26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 26, so);
  // bgt cr6,0x820d3b0c
  if (cr6.gt) goto loc_820D3B0C;
  // lis r12,-32256
  r12 = uint64_t(int64_t(-2113929216));
  // addi r12,r12,20272
  r12 = uint64_t(int64_t(int64_t(r12) + 20272));
  // lbzx r0,r12,r9
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r9));
  // lis r12,-32243
  r12 = uint64_t(int64_t(-2113077248));
  // addi r12,r12,14868
  r12 = uint64_t(int64_t(int64_t(r12) + 14868));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r9)) {
  case 0:
  goto loc_frag_820D3A14;
  return;
  case 1:
  goto loc_820D3A20;
  case 2:
  goto loc_820D3A20;
  case 3:
  goto loc_820D3A20;
  case 4:
  goto loc_820D3A20;
  case 5:
  goto loc_820D3A2C;
  case 6:
  goto loc_820D3A38;
  case 7:
  goto loc_820D3A44;
  case 8:
  goto loc_820D3A50;
  case 9:
  goto loc_820D3A5C;
  case 10:
  goto loc_820D3A68;
  case 11:
  goto loc_820D3A74;
  case 12:
  goto loc_820D3A80;
  case 13:
  goto loc_820D3A38;
  case 14:
  goto loc_820D3A8C;
  case 15:
  goto loc_820D3A98;
  case 16:
  goto loc_820D3AA4;
  case 17:
  goto loc_frag_820D3A14;
  return;
  case 18:
  goto loc_820D3AB0;
  case 19:
  goto loc_820D3ABC;
  case 20:
  goto loc_820D3AC8;
  case 21:
  goto loc_820D3AD4;
  case 22:
  goto loc_820D3AE0;
  case 23:
  goto loc_820D3AEC;
  case 24:
  goto loc_820D3AA4;
  case 25:
  goto loc_820D3AF8;
  case 26:
  goto loc_820D3B04;
  default:
  std::abort();
  }
loc_820D3A20:
  // li r11,695
  r11 = uint64_t(int64_t(695));
  // li r10,695
  r10 = uint64_t(int64_t(695));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A2C:
  // li r11,749
  r11 = uint64_t(int64_t(749));
  // li r10,749
  r10 = uint64_t(int64_t(749));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A38:
  // li r11,743
  r11 = uint64_t(int64_t(743));
  // li r10,743
  r10 = uint64_t(int64_t(743));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A44:
  // li r11,734
  r11 = uint64_t(int64_t(734));
  // li r10,734
  r10 = uint64_t(int64_t(734));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A50:
  // li r11,821
  r11 = uint64_t(int64_t(821));
  // li r10,821
  r10 = uint64_t(int64_t(821));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A5C:
  // li r11,770
  r11 = uint64_t(int64_t(770));
  // li r10,770
  r10 = uint64_t(int64_t(770));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A68:
  // li r11,878
  r11 = uint64_t(int64_t(878));
  // li r10,878
  r10 = uint64_t(int64_t(878));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A74:
  // li r11,755
  r11 = uint64_t(int64_t(755));
  // li r10,755
  r10 = uint64_t(int64_t(755));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A80:
  // li r11,767
  r11 = uint64_t(int64_t(767));
  // li r10,767
  r10 = uint64_t(int64_t(767));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A8C:
  // li r11,815
  r11 = uint64_t(int64_t(815));
  // li r10,815
  r10 = uint64_t(int64_t(815));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3A98:
  // li r11,812
  r11 = uint64_t(int64_t(812));
  // li r10,812
  r10 = uint64_t(int64_t(812));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3AA4:
  // li r11,707
  r11 = uint64_t(int64_t(707));
  // li r10,707
  r10 = uint64_t(int64_t(707));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3AB0:
  // li r11,740
  r11 = uint64_t(int64_t(740));
  // li r10,740
  r10 = uint64_t(int64_t(740));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3ABC:
  // li r11,863
  r11 = uint64_t(int64_t(863));
  // li r10,863
  r10 = uint64_t(int64_t(863));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3AC8:
  // li r10,779
  r10 = uint64_t(int64_t(779));
  // li r11,728
  r11 = uint64_t(int64_t(728));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3AD4:
  // li r11,833
  r11 = uint64_t(int64_t(833));
  // li r10,833
  r10 = uint64_t(int64_t(833));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3AE0:
  // li r11,884
  r11 = uint64_t(int64_t(884));
  // li r10,884
  r10 = uint64_t(int64_t(884));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3AEC:
  // li r11,719
  r11 = uint64_t(int64_t(719));
  // li r10,719
  r10 = uint64_t(int64_t(719));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3AF8:
  // li r11,851
  r11 = uint64_t(int64_t(851));
  // li r10,851
  r10 = uint64_t(int64_t(851));
  // b 0x820d3b0c
  goto loc_820D3B0C;
loc_820D3B04:
  // li r11,854
  r11 = uint64_t(int64_t(854));
  // li r10,854
  r10 = uint64_t(int64_t(854));
loc_820D3B0C:
  // addi r10,r10,-587
  r10 = uint64_t(int64_t(int64_t(r10) + -587));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // addi r11,r11,-587
  r11 = uint64_t(int64_t(int64_t(r11) + -587));
  // divw r10,r10,r9
  r10 = uint32_t((int32_t(r9) && !(int32_t(r10) == INT32_MIN && int32_t(r9) == -1)) ? int32_t(r10) / int32_t(r9) : 0);
  // divw r11,r11,r9
  r11 = uint32_t((int32_t(r9) && !(int32_t(r11) == INT32_MIN && int32_t(r9) == -1)) ? int32_t(r11) / int32_t(r9) : 0);
  // addi r10,r10,-36
  r10 = uint64_t(int64_t(int64_t(r10) + -36));
  // addi r11,r11,-36
  r11 = uint64_t(int64_t(int64_t(r11) + -36));
  // stw r10,0(r4)
  guest::StoreU32(base, uint32_t(r4) + 0, uint32_t(r10));
  // stw r11,0(r5)
  guest::StoreU32(base, uint32_t(r5) + 0, uint32_t(r11));
  // blr
  return;
loc_frag_820D3A14:
  // li r11,641
  r11 = uint64_t(int64_t(641));
  // li r10,641
  r10 = uint64_t(int64_t(641));
  // b 0x820d3b0c
  goto loc_820D3B0C_820D3A14;
loc_820D3B0C_820D3A14:
  // addi r10,r10,-587
  r10 = uint64_t(int64_t(int64_t(r10) + -587));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // addi r11,r11,-587
  r11 = uint64_t(int64_t(int64_t(r11) + -587));
  // divw r10,r10,r9
  r10 = uint32_t((int32_t(r9) && !(int32_t(r10) == INT32_MIN && int32_t(r9) == -1)) ? int32_t(r10) / int32_t(r9) : 0);
  // divw r11,r11,r9
  r11 = uint32_t((int32_t(r9) && !(int32_t(r11) == INT32_MIN && int32_t(r9) == -1)) ? int32_t(r11) / int32_t(r9) : 0);
  // addi r10,r10,-36
  r10 = uint64_t(int64_t(int64_t(r10) + -36));
  // addi r11,r11,-36
  r11 = uint64_t(int64_t(int64_t(r11) + -36));
  // stw r10,0(r4)
  guest::StoreU32(base, uint32_t(r4) + 0, uint32_t(r10));
  // stw r11,0(r5)
  guest::StoreU32(base, uint32_t(r5) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_820D69B8: lifted from generated/ by lift.py.
lifted::Result f820D69B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820D69B8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // addi r31,r6,80
  r31 = uint64_t(int64_t(int64_t(r6) + 80));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::Pool4Object(base, uint32_t(r31) - 80).slot_address() + 4);
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // bne cr6,0x820d69f4
  if (!cr6.eq) goto loc_820D69F4;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r31,32
  r3 = uint64_t(int64_t(int64_t(r31) + 32));
  // bl 0x82596920
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::Load(base, LIFTED_ARGS(0x820D69E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820d0758
  // (left out: dead in C++)
  { const lifted::Result o = game::LoadSlot::ReleaseBuffer(base, LIFTED_ARGS(0x820D69EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,20(r31)
  guest::StoreU32(base, schema::Pool4Object(base, uint32_t(r31) - 80).slot_state_address(), uint32_t(r11));
loc_820D69F4:
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

// sub_820D6C10 game::CarParts::OnPartLoadedA: lifted from generated/ by lift.py.
lifted::Result f820D6C10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820D6C10 (lifted)");
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
  // mr r30,r6
  r30 = r6;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r11,r30,100
  r11 = uint64_t(int64_t(int64_t(r30) + 100));
loc_820D6C28:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::CarParts(base, uint32_t(r11) - 100).loads_address() + 4);
  // cmplw cr6,r10,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r3), so);
  // beq cr6,0x820d6c48
  if (cr6.eq) goto loc_820D6C48;
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // cmpwi cr6,r31,216
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 216, so);
  // blt cr6,0x820d6c28
  if (cr6.lt) goto loc_820D6C28;
  // b 0x820d6c80
  goto loc_820D6C80;
loc_820D6C48:
  // mulli r11,r31,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(24))));
  // add r29,r11,r30
  r29 = r11 + r30;
  // mulli r11,r31,464
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(464))));
  // lwz r4,112(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 112);
  // add r11,r11,r30
  r11 = r11 + r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r11,5280
  r3 = uint64_t(int64_t(int64_t(r11) + 5280));
  // bl 0x82596920
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::Load(base, LIFTED_ARGS(0x820D6C68, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,4
  r11 = uint64_t(int64_t(int64_t(r31) + 4));
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r3,r11,r30
  r3 = r11 + r30;
  // bl 0x820d0758
  // (left out: dead in C++)
  { const lifted::Result o = game::LoadSlot::ReleaseBuffer(base, LIFTED_ARGS(0x820D6C78, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,116(r29)
  guest::StoreU32(base, uint32_t(r29) + 116, uint32_t(r11));
loc_820D6C80:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820D6C88 game::CarParts::OnPartLoadedB: lifted from generated/ by lift.py.
lifted::Result f820D6C88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820D6C88 (lifted)");
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
  // mr r30,r6
  r30 = r6;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r11,r30,100
  r11 = uint64_t(int64_t(int64_t(r30) + 100));
loc_820D6CA0:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::CarParts(base, uint32_t(r11) - 100).loads_address() + 4);
  // cmplw cr6,r10,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r3), so);
  // beq cr6,0x820d6cc0
  if (cr6.eq) goto loc_820D6CC0;
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // cmpwi cr6,r31,17
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 17, so);
  // blt cr6,0x820d6ca0
  if (cr6.lt) goto loc_820D6CA0;
  // b 0x820d6cf8
  goto loc_820D6CF8;
loc_820D6CC0:
  // mulli r11,r31,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(24))));
  // add r29,r11,r30
  r29 = r11 + r30;
  // mulli r11,r31,464
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(464))));
  // lwz r4,112(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 112);
  // add r11,r11,r30
  r11 = r11 + r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r11,5280
  r3 = uint64_t(int64_t(int64_t(r11) + 5280));
  // bl 0x82596920
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::Load(base, LIFTED_ARGS(0x820D6CE0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,4
  r11 = uint64_t(int64_t(int64_t(r31) + 4));
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r3,r11,r30
  r3 = r11 + r30;
  // bl 0x820d0758
  // (left out: dead in C++)
  { const lifted::Result o = game::LoadSlot::ReleaseBuffer(base, LIFTED_ARGS(0x820D6CF0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,116(r29)
  guest::StoreU32(base, uint32_t(r29) + 116, uint32_t(r11));
loc_820D6CF8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820D8D20: lifted from generated/ by lift.py.
lifted::Result f820D8D20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820D8D20 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // addi r30,r6,192
  r30 = uint64_t(int64_t(int64_t(r6) + 192));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r11,r30,980
  r11 = uint64_t(int64_t(int64_t(r30) + 980));
loc_820D8D38:
  // lwz r10,-976(r11)
  r10 = guest::LoadU32(base, schema::PartModelPicker(base, uint32_t(r11) - 1172).unknown_196_address());
  // cmplw cr6,r10,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r3), so);
  // beq cr6,0x820d8d7c
  if (cr6.eq) goto loc_820D8D7C;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::PartModelPicker(base, uint32_t(r11) - 1172).unknown_1172_address());
  // cmplw cr6,r10,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r3), so);
  // beq cr6,0x820d8dac
  if (cr6.eq) goto loc_820D8DAC;
  // lwz r10,976(r11)
  r10 = guest::LoadU32(base, schema::PartModelPicker(base, uint32_t(r11) - 1172).unknown_2148_address());
  // cmplw cr6,r10,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r3), so);
  // beq cr6,0x820d8ddc
  if (cr6.eq) goto loc_820D8DDC;
  // lwz r10,1112(r11)
  r10 = guest::LoadU32(base, schema::PartModelPicker(base, uint32_t(r11) - 1172).unknown_2284_address());
  // cmplw cr6,r10,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r3), so);
  // beq cr6,0x820d8e28
  if (cr6.eq) goto loc_820D8E28;
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x820d8d38
  if (cr6.lt) goto loc_820D8D38;
  // b 0x820d8e5c
  goto loc_820D8E5C;
loc_820D8D7C:
  // mulli r11,r31,464
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(464))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r11,48
  r3 = uint64_t(int64_t(int64_t(r11) + 48));
  // bl 0x82596920
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::Load(base, LIFTED_ARGS(0x820D8D90, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r31,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(24))));
  // add r31,r11,r30
  r31 = r11 + r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x820d0758
  // (left out: dead in C++)
  { const lifted::Result o = game::LoadSlot::ReleaseBuffer(base, LIFTED_ARGS(0x820D8DA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,20(r31)
  guest::StoreU32(base, uint32_t(r31) + 20, uint32_t(r11));
  // b 0x820d8e5c
  goto loc_820D8E5C;
loc_820D8DAC:
  // mulli r11,r31,464
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(464))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r11,1024
  r3 = uint64_t(int64_t(int64_t(r11) + 1024));
  // bl 0x82596920
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::Load(base, LIFTED_ARGS(0x820D8DC0, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r31,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(24))));
  // add r31,r11,r30
  r31 = r11 + r30;
  // addi r3,r31,976
  r3 = uint64_t(int64_t(int64_t(r31) + 976));
  // bl 0x820d0758
  // (left out: dead in C++)
  { const lifted::Result o = game::LoadSlot::ReleaseBuffer(base, LIFTED_ARGS(0x820D8DD0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,996(r31)
  guest::StoreU32(base, uint32_t(r31) + 996, uint32_t(r11));
  // b 0x820d8e5c
  goto loc_820D8E5C;
loc_820D8DDC:
  // mulli r11,r31,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(24))));
  // add r29,r11,r30
  r29 = r11 + r30;
  // addi r28,r29,2000
  r28 = uint64_t(int64_t(int64_t(r29) + 2000));
  // mr r3,r28
  r3 = r28;
  // bl 0x82596f68
  // (left out: dead in C++)
  { const lifted::Result o = game::XPR::Create(base, LIFTED_ARGS(0x820D8DF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r31,20
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(20))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // mr r5,r28
  r5 = r28;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r11,2048
  r3 = uint64_t(int64_t(int64_t(r11) + 2048));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820D8E14, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r29,1952
  r3 = uint64_t(int64_t(int64_t(r29) + 1952));
  // bl 0x820d0758
  // (left out: dead in C++)
  { const lifted::Result o = game::LoadSlot::ReleaseBuffer(base, LIFTED_ARGS(0x820D8E1C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,1972(r29)
  guest::StoreU32(base, uint32_t(r29) + 1972, uint32_t(r11));
  // b 0x820d8e5c
  goto loc_820D8E5C;
loc_820D8E28:
  // mulli r11,r31,464
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(464))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r11,2144
  r3 = uint64_t(int64_t(int64_t(r11) + 2144));
  // bl 0x82596920
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::Load(base, LIFTED_ARGS(0x820D8E3C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,87
  r11 = uint64_t(int64_t(int64_t(r31) + 87));
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // add r3,r11,r30
  r3 = r11 + r30;
  // bl 0x820d0758
  // (left out: dead in C++)
  { const lifted::Result o = game::LoadSlot::ReleaseBuffer(base, LIFTED_ARGS(0x820D8E4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mulli r11,r31,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(24))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r10,2108(r11)
  guest::StoreU32(base, uint32_t(r11) + 2108, uint32_t(r10));
loc_820D8E5C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820DCF98 game::LightPoints::DrawFrontSpots: lifted from generated/ by lift.py.
lifted::Result f820DCF98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820DCF98 (lifted)");
  uint64_t r4 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // mr r28,r5
  r28 = r5;
  // mr r30,r6
  r30 = r6;
  // addi r29,r31,2900
  r29 = uint64_t(int64_t(int64_t(r31) + 2900));
  // lfs f31,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // li r27,6
  r27 = uint64_t(int64_t(6));
loc_820DCFC4:
  // lwz r4,224(r29)
  r4 = guest::LoadU32(base, schema::LightPoints(base, uint32_t(r29) - 2900).spot_shape_address());
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, schema::LightPoints(base, uint32_t(r29) - 2900).spot_model_address());
  // bl 0x820d1c00
  // (left out: dead in C++)
  { const lifted::Result o = game::PartModel::SetAlpha(base, LIFTED_ARGS(0x820DCFD4, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r27,r27,-1
  ca = uint32_t(r27) > 0;
  r27 = uint64_t(int64_t(int64_t(r27) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // addi r29,r29,240
  r29 = uint64_t(int64_t(int64_t(r29) + 240));
  // bne 0x820dcfc4
  if (!cr0.eq) goto loc_820DCFC4;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::LightPoints(base, uint32_t(r31)).record_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::LightKitRecord(base, uint32_t(r11)).front_mode_address());
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // beq cr6,0x820dd074
  if (cr6.eq) goto loc_820DD074;
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x820dd00c
  if (cr6.eq) goto loc_820DD00C;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // bne cr6,0x820dd0a4
  if (!cr6.eq) goto loc_820DD0A4;
  // addi r29,r28,16
  r29 = uint64_t(int64_t(int64_t(r28) + 16));
  // addi r3,r31,2896
  r3 = uint64_t(int64_t(int64_t(r31) + 2896));
  // b 0x820dd044
  goto loc_820DD044;
loc_820DD00C:
  // addi r29,r28,16
  r29 = uint64_t(int64_t(int64_t(r28) + 16));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,2896
  r3 = uint64_t(int64_t(int64_t(r31) + 2896));
  // mr r5,r29
  r5 = r29;
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD028, r31)); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,3136
  r3 = uint64_t(int64_t(int64_t(r31) + 3136));
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD040, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,3856
  r3 = uint64_t(int64_t(int64_t(r31) + 3856));
loc_820DD044:
  // mr r4,r30
  r4 = r30;
  // mr r5,r29
  r5 = r29;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD058, r31)); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,4096
  r3 = uint64_t(int64_t(int64_t(r31) + 4096));
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD070, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820dd0a4
  goto loc_820DD0A4;
loc_820DD074:
  // addi r29,r31,2896
  r29 = uint64_t(int64_t(int64_t(r31) + 2896));
  // addi r28,r28,16
  r28 = uint64_t(int64_t(int64_t(r28) + 16));
  // li r31,6
  r31 = uint64_t(int64_t(6));
loc_820DD080:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r28
  r5 = r28;
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD098, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r29,r29,240
  r29 = uint64_t(int64_t(int64_t(r29) + 240));
  // bne 0x820dd080
  if (!cr0.eq) goto loc_820DD080;
loc_820DD0A4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-56(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820DD438 game::LightPoints::DrawSidesSpots: lifted from generated/ by lift.py.
lifted::Result f820DD438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820DD438 (lifted)");
  uint64_t r4 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // mr r29,r5
  r29 = r5;
  // mr r30,r6
  r30 = r6;
  // addi r28,r31,2900
  r28 = uint64_t(int64_t(int64_t(r31) + 2900));
  // lfs f31,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // li r27,6
  r27 = uint64_t(int64_t(6));
loc_820DD464:
  // lwz r4,224(r28)
  r4 = guest::LoadU32(base, schema::LightPoints(base, uint32_t(r28) - 2900).spot_shape_address());
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // lwz r3,0(r28)
  r3 = guest::LoadU32(base, schema::LightPoints(base, uint32_t(r28) - 2900).spot_model_address());
  // bl 0x820d1c00
  // (left out: dead in C++)
  { const lifted::Result o = game::PartModel::SetAlpha(base, LIFTED_ARGS(0x820DD474, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r27,r27,-1
  ca = uint32_t(r27) > 0;
  r27 = uint64_t(int64_t(int64_t(r27) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // addi r28,r28,240
  r28 = uint64_t(int64_t(int64_t(r28) + 240));
  // bne 0x820dd464
  if (!cr0.eq) goto loc_820DD464;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::LightPoints(base, uint32_t(r31)).record_address());
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, schema::LightKitRecord(base, uint32_t(r11)).sides_mode_address());
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // beq cr6,0x820dd584
  if (cr6.eq) goto loc_820DD584;
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x820dd51c
  if (cr6.eq) goto loc_820DD51C;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // beq cr6,0x820dd510
  if (cr6.eq) goto loc_820DD510;
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // beq cr6,0x820dd4ec
  if (cr6.eq) goto loc_820DD4EC;
  // cmpwi cr6,r11,11
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 11, so);
  // bne cr6,0x820dd5b4
  if (!cr6.eq) goto loc_820DD5B4;
  // addi r29,r29,16
  r29 = uint64_t(int64_t(int64_t(r29) + 16));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,2896
  r3 = uint64_t(int64_t(int64_t(r31) + 2896));
  // mr r5,r29
  r5 = r29;
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD4CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,3136
  r3 = uint64_t(int64_t(int64_t(r31) + 3136));
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD4E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,3376
  r3 = uint64_t(int64_t(int64_t(r31) + 3376));
  // b 0x820dd56c
  goto loc_820DD56C;
loc_820DD4EC:
  // addi r29,r29,16
  r29 = uint64_t(int64_t(int64_t(r29) + 16));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,2896
  r3 = uint64_t(int64_t(int64_t(r31) + 2896));
  // mr r5,r29
  r5 = r29;
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD508, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,3136
  r3 = uint64_t(int64_t(int64_t(r31) + 3136));
  // b 0x820dd56c
  goto loc_820DD56C;
loc_820DD510:
  // addi r29,r29,16
  r29 = uint64_t(int64_t(int64_t(r29) + 16));
  // addi r3,r31,2896
  r3 = uint64_t(int64_t(int64_t(r31) + 2896));
  // b 0x820dd554
  goto loc_820DD554;
loc_820DD51C:
  // addi r29,r29,16
  r29 = uint64_t(int64_t(int64_t(r29) + 16));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,2896
  r3 = uint64_t(int64_t(int64_t(r31) + 2896));
  // mr r5,r29
  r5 = r29;
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD538, r31)); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,3136
  r3 = uint64_t(int64_t(int64_t(r31) + 3136));
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD550, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,3856
  r3 = uint64_t(int64_t(int64_t(r31) + 3856));
loc_820DD554:
  // mr r4,r30
  r4 = r30;
  // mr r5,r29
  r5 = r29;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD568, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,4096
  r3 = uint64_t(int64_t(int64_t(r31) + 4096));
loc_820DD56C:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD580, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820dd5b4
  goto loc_820DD5B4;
loc_820DD584:
  // addi r28,r29,16
  r28 = uint64_t(int64_t(int64_t(r29) + 16));
  // addi r29,r31,2896
  r29 = uint64_t(int64_t(int64_t(r31) + 2896));
  // li r31,6
  r31 = uint64_t(int64_t(6));
loc_820DD590:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r28
  r5 = r28;
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD5A8, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r29,r29,240
  r29 = uint64_t(int64_t(int64_t(r29) + 240));
  // bne 0x820dd590
  if (!cr0.eq) goto loc_820DD590;
loc_820DD5B4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-56(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820DD948 game::LightPoints::DrawRearSpots: lifted from generated/ by lift.py.
lifted::Result f820DD948(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820DD948 (lifted)");
  uint64_t r4 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // mr r28,r5
  r28 = r5;
  // mr r30,r6
  r30 = r6;
  // addi r29,r31,2900
  r29 = uint64_t(int64_t(int64_t(r31) + 2900));
  // lfs f31,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // li r27,6
  r27 = uint64_t(int64_t(6));
loc_820DD974:
  // lwz r4,224(r29)
  r4 = guest::LoadU32(base, schema::LightPoints(base, uint32_t(r29) - 2900).spot_shape_address());
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, schema::LightPoints(base, uint32_t(r29) - 2900).spot_model_address());
  // bl 0x820d1c00
  // (left out: dead in C++)
  { const lifted::Result o = game::PartModel::SetAlpha(base, LIFTED_ARGS(0x820DD984, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r27,r27,-1
  ca = uint32_t(r27) > 0;
  r27 = uint64_t(int64_t(int64_t(r27) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // addi r29,r29,240
  r29 = uint64_t(int64_t(int64_t(r29) + 240));
  // bne 0x820dd974
  if (!cr0.eq) goto loc_820DD974;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::LightPoints(base, uint32_t(r31)).record_address());
  // lwz r11,36(r11)
  r11 = guest::LoadU32(base, schema::LightKitRecord(base, uint32_t(r11)).rear_mode_address());
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // beq cr6,0x820dda24
  if (cr6.eq) goto loc_820DDA24;
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x820dd9bc
  if (cr6.eq) goto loc_820DD9BC;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // bne cr6,0x820dda54
  if (!cr6.eq) goto loc_820DDA54;
  // addi r29,r28,16
  r29 = uint64_t(int64_t(int64_t(r28) + 16));
  // addi r3,r31,2896
  r3 = uint64_t(int64_t(int64_t(r31) + 2896));
  // b 0x820dd9f4
  goto loc_820DD9F4;
loc_820DD9BC:
  // addi r29,r28,16
  r29 = uint64_t(int64_t(int64_t(r28) + 16));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,2896
  r3 = uint64_t(int64_t(int64_t(r31) + 2896));
  // mr r5,r29
  r5 = r29;
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD9D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,3136
  r3 = uint64_t(int64_t(int64_t(r31) + 3136));
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DD9F0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,3856
  r3 = uint64_t(int64_t(int64_t(r31) + 3856));
loc_820DD9F4:
  // mr r4,r30
  r4 = r30;
  // mr r5,r29
  r5 = r29;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DDA08, r31)); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,4096
  r3 = uint64_t(int64_t(int64_t(r31) + 4096));
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DDA20, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820dda54
  goto loc_820DDA54;
loc_820DDA24:
  // addi r29,r31,2896
  r29 = uint64_t(int64_t(int64_t(r31) + 2896));
  // addi r28,r28,16
  r28 = uint64_t(int64_t(int64_t(r28) + 16));
  // li r31,6
  r31 = uint64_t(int64_t(6));
loc_820DDA30:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r28
  r5 = r28;
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x820d06f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarPart::Draw(base, LIFTED_ARGS(0x820DDA48, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r29,r29,240
  r29 = uint64_t(int64_t(int64_t(r29) + 240));
  // bne 0x820dda30
  if (!cr0.eq) goto loc_820DDA30;
loc_820DDA54:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-56(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820EF250 game::Accessory::Step: lifted from generated/ by lift.py.
lifted::Result f820EF250(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_820EF250 (lifted)");
  // addi r3,r3,544
  r3 = uint64_t(int64_t(int64_t(r3) + 544));
  // b 0x820f0718
  { const lifted::Result o = game::AccessorySkeleton::Step(base, r1, r13, r3, r4, r6, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820EF418 game::Accessory::SetAnchor: lifted from generated/ by lift.py.
uint64_t f820EF418(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820EF418 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // addi r3,r31,544
  r3 = uint64_t(int64_t(int64_t(r31) + 544));
  // bl 0x820efcf8
  // (left out: dead in C++)
  r3 = game::AccessorySkeleton::Move(base, r1, r13, r3, r4);
  // addi r11,r30,48
  r11 = uint64_t(int64_t(int64_t(r30) + 48));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, schema::AccessoryMatrix(base, uint32_t(r11) - 48).translation_xy_address());
  // std r10,992(r31)
  guest::StoreU64(base, schema::Accessory(base, uint32_t(r31)).anchor_xy_address(), r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, schema::AccessoryMatrix(base, uint32_t(r11) - 48).translation_zw_address());
  // std r11,1000(r31)
  guest::StoreU64(base, schema::Accessory(base, uint32_t(r31)).anchor_zw_address(), r11);
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

// sub_820EF468 game::Accessory::BindSkin: lifted from generated/ by lift.py.
lifted::Result f820EF468(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820EF468 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r29,r6
  r29 = r6;
  // mr r3,r4
  r3 = r4;
  // mr r4,r5
  r4 = r5;
  // mr r5,r29
  r5 = r29;
  // bl 0x8258ab18
  // (left out: dead in C++)
  { const lifted::Result o = f8258AB18(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, schema::Accessory(base, uint32_t(r31)).shape_count_address());
  // mr r27,r3
  r27 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820ef4e0
  if (!cr0.gt) goto loc_820EF4E0;
  // li r28,0
  r28 = uint64_t(int64_t(0));
loc_820EF4A4:
  // lwz r11,40(r31)
  r11 = guest::LoadU32(base, schema::Accessory(base, uint32_t(r31)).shapes_address());
  // add r11,r28,r11
  r11 = r28 + r11;
  // lwz r11,428(r11)
  r11 = guest::LoadU32(base, schema::AccessoryShape(base, uint32_t(r11)).skinned_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x820ef4cc
  if (!cr6.eq) goto loc_820EF4CC;
  // mr r6,r30
  r6 = r30;
  // mr r5,r29
  r5 = r29;
  // mr r4,r27
  r4 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x82594fd0
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::SkinShape(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_820EF4CC:
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, schema::Accessory(base, uint32_t(r31)).shape_count_address());
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r28,r28,544
  r28 = uint64_t(int64_t(int64_t(r28) + 544));
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x820ef4a4
  if (cr6.lt) goto loc_820EF4A4;
loc_820EF4E0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820EF4E8 game::Accessory::RecordEntry: lifted from generated/ by lift.py.
lifted::Result f820EF4E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820EF4E8 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r31,-32105
  r31 = uint64_t(int64_t(-2104033280));
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // lwz r3,-18152(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -18152);
  // bl 0x8258c9d0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::OpenChunk(base, LIFTED_ARGS(0x820EF50C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,-18152(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -18152);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x820EF518, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r26,3
  r26 = uint64_t(int64_t(3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r29,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r29));
  // sth r26,2(r30)
  guest::StoreU16(base, uint32_t(r30) + 2, uint16_t(r26));
  // lwz r3,-18152(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -18152);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x820EF534, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // sth r26,2(r3)
  guest::StoreU16(base, uint32_t(r3) + 2, uint16_t(r26));
  // stw r28,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r28));
  // lwz r3,-18152(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -18152);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x820EF548, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r27,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r27));
  // sth r11,2(r3)
  guest::StoreU16(base, uint32_t(r3) + 2, uint16_t(r11));
  // lwz r3,-18152(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -18152);
  // bl 0x8258ca50
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::CloseChunk(base, LIFTED_ARGS(0x820EF55C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820EF740: lifted from generated/ by lift.py.
lifted::Result f820EF740(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820EF740 (lifted)");
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
  // bl 0x820ef310
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820EF310, LIFTED_ARGS(0x820EF760, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820ef770
  if (cr0.eq) goto loc_820EF770;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820EF770, r31)); r3 = o.r3; f1 = o.f1; }
loc_820EF770:
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

// sub_820EF790 game::Accessory::DrawShape: lifted from generated/ by lift.py.
lifted::Result f820EF790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820EF790 (lifted)");
  uint64_t r8 = 0, r11 = 0, r12 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e64
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-128(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -128, std::bit_cast<uint64_t>(f30));
  // stfd f31,-120(r1)
  guest::StoreU64(base, uint32_t(r1) + -120, std::bit_cast<uint64_t>(f31));
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f30,f1
  f30 = f1;
  // mr r30,r4
  r30 = r4;
  // mr r22,r10
  r22 = r10;
  // mulli r11,r30,544
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(544))));
  // lwz r10,40(r31)
  r10 = guest::LoadU32(base, schema::Accessory(base, uint32_t(r31)).shapes_address());
  // add r11,r11,r10
  r11 = r11 + r10;
  // lis r25,-32105
  r25 = uint64_t(int64_t(-2104033280));
  // lis r26,-32105
  r26 = uint64_t(int64_t(-2104033280));
  // mr r24,r5
  r24 = r5;
  // mr r28,r6
  r28 = r6;
  // mr r21,r7
  r21 = r7;
  // lwz r10,436(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 436);
  // mr r23,r9
  r23 = r9;
  // lwz r20,-18164(r25)
  r20 = guest::LoadU32(base, uint32_t(r25) + -18164);
  // lwz r19,-18176(r26)
  r19 = guest::LoadU32(base, uint32_t(r26) + -18176);
  // add r11,r10,r20
  r11 = r10 + r20;
  // cmplwi cr6,r11,682
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 682, so);
  // ble cr6,0x820ef7f4
  if (!cr6.gt) goto loc_820EF7F4;
  // li r11,682
  r11 = uint64_t(int64_t(682));
loc_820EF7F4:
  // add r10,r10,r19
  r10 = r10 + r19;
  // cmplwi cr6,r10,682
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 682, so);
  // ble cr6,0x820ef804
  if (!cr6.gt) goto loc_820EF804;
  // li r10,682
  r10 = uint64_t(int64_t(682));
loc_820EF804:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r11,-18164(r25)
  guest::StoreU32(base, uint32_t(r25) + -18164, uint32_t(r11));
  // mr r4,r30
  r4 = r30;
  // stw r10,-18176(r26)
  guest::StoreU32(base, uint32_t(r26) + -18176, uint32_t(r10));
  // mr r3,r31
  r3 = r31;
  // bl 0x82594ad8
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::MaterialCount(base, LIFTED_ARGS(0x820EF81C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r29,-32105
  r29 = uint64_t(int64_t(-2104033280));
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // ble 0x820ef8dc
  if (!cr0.gt) goto loc_820EF8DC;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r27,-18164(r25)
  r27 = guest::LoadU32(base, uint32_t(r25) + -18164);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82595550
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::RecordShape(base, LIFTED_ARGS(0x820EF850, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // mr r5,r28
  r5 = r28;
  // mr r4,r24
  r4 = r24;
  // mr r3,r31
  r3 = r31;
  // bl 0x820ef4e8
  // (left out: dead in C++)
  { const lifted::Result o = game::Accessory::RecordEntry(base, r1, r13, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lwz r3,-18148(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + -18148);
  // fneg f1,f30
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f30) ^ 0x8000000000000000));
  // mr r5,r27
  r5 = r27;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::RenderQueue(base, uint32_t(r3)).vtable_address());
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::RenderQueueVtable(base, uint32_t(r11)).add_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820EF884, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x820ef8dc
  if (cr6.eq) goto loc_820EF8DC;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825960d0
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::RecordShapeDepth(base, LIFTED_ARGS(0x820EF8A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // mr r5,r28
  r5 = r28;
  // mr r4,r21
  r4 = r21;
  // mr r3,r31
  r3 = r31;
  // bl 0x820ef4e8
  // (left out: dead in C++)
  { const lifted::Result o = game::Accessory::RecordEntry(base, r1, r13, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lwz r3,-18148(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + -18148);
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // mr r5,r22
  r5 = r22;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::RenderQueue(base, uint32_t(r3)).vtable_address());
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::RenderQueueVtable(base, uint32_t(r11)).add_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820EF8DC, r31)); r3 = o.r3; f1 = o.f1; }
loc_820EF8DC:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82594ad8
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::MaterialCount(base, LIFTED_ARGS(0x820EF8EC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x820ef9a0
  if (!cr0.gt) goto loc_820EF9A0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r27,-18176(r26)
  r27 = guest::LoadU32(base, uint32_t(r26) + -18176);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82595550
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::RecordShape(base, LIFTED_ARGS(0x820EF914, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // mr r5,r28
  r5 = r28;
  // mr r4,r24
  r4 = r24;
  // mr r3,r31
  r3 = r31;
  // bl 0x820ef4e8
  // (left out: dead in C++)
  { const lifted::Result o = game::Accessory::RecordEntry(base, r1, r13, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lwz r3,-18148(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + -18148);
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // mr r5,r27
  r5 = r27;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::RenderQueue(base, uint32_t(r3)).vtable_address());
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::RenderQueueVtable(base, uint32_t(r11)).add_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820EF948, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x820ef9a0
  if (cr6.eq) goto loc_820EF9A0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82595550
  // (left out: dead in C++)
  { const lifted::Result o = game::Model::RecordShape(base, LIFTED_ARGS(0x820EF96C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // mr r5,r28
  r5 = r28;
  // mr r4,r21
  r4 = r21;
  // mr r3,r31
  r3 = r31;
  // bl 0x820ef4e8
  // (left out: dead in C++)
  { const lifted::Result o = game::Accessory::RecordEntry(base, r1, r13, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lwz r3,-18148(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + -18148);
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // lwz r5,292(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 292);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::RenderQueue(base, uint32_t(r3)).vtable_address());
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, schema::RenderQueueVtable(base, uint32_t(r11)).add_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820EF9A0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820EF9A0:
  // stw r20,-18164(r25)
  guest::StoreU32(base, uint32_t(r25) + -18164, uint32_t(r20));
  // stw r19,-18176(r26)
  guest::StoreU32(base, uint32_t(r26) + -18176, uint32_t(r19));
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f30,-128(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -128)));
  // lfd f31,-120(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -120)));
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820EF9B8 game::Accessory::DrawShapes: lifted from generated/ by lift.py.
lifted::Result f820EF9B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820EF9B8 (lifted)");
  uint64_t r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // mr r25,r7
  r25 = r7;
  // mr r24,r8
  r24 = r8;
  // mr r23,r9
  r23 = r9;
  // mr r30,r10
  r30 = r10;
  // bl 0x825953d0
  // (left out: dead in C++)
  { const lifted::Result o = game::Recorder::RecordObject(base, LIFTED_ARGS(0x820EF9E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x82595490
  // (left out: dead in C++)
  { const lifted::Result o = game::Recorder::RecordSceneLights(base, LIFTED_ARGS(0x820EF9F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r22,r3
  r22 = r3;
  // addi r21,r27,16
  r21 = uint64_t(int64_t(int64_t(r27) + 16));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x820efa1c
  if (cr6.eq) goto loc_820EFA1C;
  // mr r5,r25
  r5 = r25;
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x825953d0
  // (left out: dead in C++)
  { const lifted::Result o = game::Recorder::RecordObject(base, LIFTED_ARGS(0x820EFA18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
loc_820EFA1C:
  // cmpwi cr6,r30,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r30), -1, so);
  // bne cr6,0x820efa30
  if (!cr6.eq) goto loc_820EFA30;
  // lwz r29,412(r31)
  r29 = guest::LoadU32(base, schema::Accessory(base, uint32_t(r31)).shape_count_address());
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // b 0x820efa64
  goto loc_820EFA64;
loc_820EFA30:
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x820efa64
  goto loc_820EFA64;
loc_820EFA38:
  // mr r10,r24
  r10 = r24;
  // lfs f1,56(r21)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::RecordedObjectConstants(base, uint32_t(r21) - 16).depth_address());
  f1 = double(temp.f32);
  // mr r9,r28
  r9 = r28;
  // stw r23,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r23));
  // mr r7,r26
  r7 = r26;
  // mr r6,r22
  r6 = r22;
  // mr r5,r27
  r5 = r27;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x820ef790
  // (left out: dead in C++)
  { const lifted::Result o = game::Accessory::DrawShape(base, r1, r13, r3, r4, r5, r6, r7, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
loc_820EFA64:
  // cmpw cr6,r30,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r29), so);
  // blt cr6,0x820efa38
  if (cr6.lt) goto loc_820EFA38;
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820EFA78 game::Accessory::Draw: lifted from generated/ by lift.py.
lifted::Result f820EFA78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820EFA78 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // addi r3,r31,544
  r3 = uint64_t(int64_t(int64_t(r31) + 544));
  // bl 0x820efb28
  // (left out: dead in C++)
  { const lifted::Result o = game::AccessorySkeleton::WaitForStep(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1, 0x820EFA9C); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r6,r31,1024
  r6 = uint64_t(int64_t(int64_t(r31) + 1024));
  // addi r5,r31,992
  r5 = uint64_t(int64_t(int64_t(r31) + 992));
  // addi r4,r31,1008
  r4 = uint64_t(int64_t(int64_t(r31) + 1008));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // stfs f0,124(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 124, temp.u32);
  // stfs f0,140(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 140, temp.u32);
  // bl 0x820ef660
  // (left out: dead in C++)
  { const lifted::Result o = game::Matrix::FromScaleRotationTranslation(base, LIFTED_ARGS(0x820EFAC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,1044(r31)
  r11 = guest::LoadU32(base, schema::Accessory(base, uint32_t(r31)).visible_address());
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820efb10
  if (cr0.eq) goto loc_820EFB10;
  // lwz r11,624(r31)
  r11 = guest::LoadU32(base, schema::Accessory(base, uint32_t(r31)).skeleton_node_file_address());
  // mr r3,r31
  r3 = r31;
  // lwz r5,652(r31)
  r5 = guest::LoadU32(base, schema::Accessory(base, uint32_t(r31)).skeleton_unknown_108_address());
  // lwz r4,692(r31)
  r4 = guest::LoadU32(base, schema::Accessory(base, uint32_t(r31)).skeleton_node_matrices_address());
  // lwz r6,4(r11)
  r6 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).node_count_address());
  // bl 0x820ef468
  // (left out: dead in C++)
  { const lifted::Result o = game::Accessory::BindSkin(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r30,144
  r4 = uint64_t(int64_t(int64_t(r30) + 144));
  // mr r3,r31
  r3 = r31;
  // bl 0x820ef9b8
  // (left out: dead in C++)
  { const lifted::Result o = game::Accessory::DrawShapes(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_820EFB10:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
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

// sub_820EFB28 game::AccessorySkeleton::WaitForStep: lifted from generated/ by lift.py.
lifted::Result f820EFB28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820EFB28 (lifted)");
  uint64_t r4 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,216
  r3 = uint64_t(int64_t(int64_t(r3) + 216));
  // b 0x82094b10
  { const lifted::Result o = game::Job::Wait(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820EFCF8 game::AccessorySkeleton::Move: lifted from generated/ by lift.py.
uint64_t f820EFCF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820EFCF8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // addi r3,r31,288
  r3 = uint64_t(int64_t(int64_t(r31) + 288));
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = game::Matrix::Copy(base, LIFTED_ARGS(0x820EFD1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,152(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).stepped_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x820efd54
  if (!cr6.eq) goto loc_820EFD54;
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,352
  r3 = uint64_t(int64_t(int64_t(r31) + 352));
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = game::Matrix::Copy(base, LIFTED_ARGS(0x820EFD34, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,424(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).movement_z_address(), temp.u32);
  // stfs f0,420(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).movement_y_address(), temp.u32);
  // stfs f0,416(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).movement_x_address(), temp.u32);
  // lfs f13,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // stfs f13,428(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).movement_w_address(), temp.u32);
loc_820EFD54:
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

// sub_820EFD70 game::AccessorySkeleton::SwingNode: lifted from generated/ by lift.py.
uint64_t f820EFD70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820EFD70 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r29,r3
  r29 = r3;
  // mr r27,r5
  r27 = r5;
  // lwz r11,80(r29)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r29)).node_file_address());
  // lwz r10,84(r29)
  r10 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r29)).nodes_address());
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).swing_first_address());
  // add r11,r11,r4
  r11 = r11 + r4;
  // mulli r11,r11,160
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(160))));
  // add r28,r11,r10
  r28 = r11 + r10;
  // lwz r11,132(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 132);
  // rlwinm. r11,r11,0,23,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x100;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820effcc
  if (cr0.eq) goto loc_820EFFCC;
  // lwz r10,128(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 128);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x820effcc
  if (cr6.eq) goto loc_820EFFCC;
  // lwz r7,80(r29)
  r7 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r29)).node_file_address());
  // rlwinm r8,r4,6,0,25
  r8 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 6) & 0xFFFFFFC0;
  // lwz r9,144(r29)
  r9 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r29)).swing_entries_address());
  // rlwinm r10,r10,6,0,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 6) & 0xFFFFFFC0;
  // lwz r11,148(r29)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r29)).node_matrices_address());
  // add r31,r9,r8
  r31 = r9 + r8;
  // lwz r9,12(r7)
  r9 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r7)).swing_first_address());
  // add r9,r9,r4
  r9 = r9 + r4;
  // add r4,r10,r11
  r4 = r10 + r11;
  // rlwinm r9,r9,6,0,25
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 6) & 0xFFFFFFC0;
  // add r30,r9,r11
  r30 = r9 + r11;
  // mr r3,r30
  r3 = r30;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = game::Matrix::Copy(base, LIFTED_ARGS(0x820EFDE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,152(r29)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r29)).stepped_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x820eff80
  if (cr6.eq) goto loc_820EFF80;
  // lwz r11,264(r29)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r29)).swinging_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x820eff80
  if (cr6.eq) goto loc_820EFF80;
  // lwz r11,52(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 52);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x820efe54
  if (!cr6.eq) goto loc_820EFE54;
  // lfs f0,0(r27)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AccessoryFloat3(base, uint32_t(r27)).x_address());
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // lfs f0,4(r27)
  temp.u32 = guest::LoadU32(base, schema::AccessoryFloat3(base, uint32_t(r27)).y_address());
  f0 = double(temp.f32);
  // lfs f13,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,4(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 4, temp.u32);
  // lfs f0,8(r27)
  temp.u32 = guest::LoadU32(base, schema::AccessoryFloat3(base, uint32_t(r27)).z_address());
  f0 = double(temp.f32);
  // lfs f13,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // lfs f12,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // lfs f0,8712(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8712);
  f0 = double(temp.f32);
  // fsubs f0,f12,f0
  f0 = double(float(f12 - f0));
  // stfs f13,8(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 8, temp.u32);
  // stfs f0,4(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 4, temp.u32);
  // b 0x820efe5c
  goto loc_820EFE5C;
loc_820EFE54:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,52(r31)
  guest::StoreU32(base, uint32_t(r31) + 52, uint32_t(r11));
loc_820EFE5C:
  // addi r11,r31,32
  r11 = uint64_t(int64_t(int64_t(r31) + 32));
  // lfs f13,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lfs f12,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // lfs f9,36(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 36);
  f9 = double(temp.f32);
  // lfs f7,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f7 = double(temp.f32);
  // lfs f8,40(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 40);
  f8 = double(temp.f32);
  // lfs f10,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f10 = double(temp.f32);
  // lfs f0,-30720(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -30720);
  f0 = double(temp.f32);
  // fmadds f11,f10,f0,f13
  f11 = double(float(std::fma(f10, f0, f13)));
  // stfs f11,0(r31)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // fmadds f13,f9,f0,f12
  f13 = double(float(std::fma(f9, f0, f12)));
  // stfs f13,4(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 4, temp.u32);
  // fmadds f0,f8,f0,f7
  f0 = double(float(std::fma(f8, f0, f7)));
  // stfs f0,8(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 8, temp.u32);
  // lfs f12,52(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 52);
  f12 = double(temp.f32);
  // lfs f7,56(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 56);
  f7 = double(temp.f32);
  // fsubs f13,f12,f13
  f13 = double(float(f12 - f13));
  // fsubs f12,f7,f0
  f12 = double(float(f7 - f0));
  // lfs f0,48(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 48);
  f0 = double(temp.f32);
  // fsubs f0,f0,f11
  f0 = double(float(f0 - f11));
  // lfs f7,140(r28)
  temp.u32 = guest::LoadU32(base, uint32_t(r28) + 140);
  f7 = double(temp.f32);
  // fmuls f6,f13,f13
  f6 = double(float(f13 * f13));
  // fmadds f11,f12,f12,f6
  f11 = double(float(std::fma(f12, f12, f6)));
  // fmadds f11,f0,f0,f11
  f11 = double(float(std::fma(f0, f0, f11)));
  // fsqrts f11,f11
  f11 = double(float(sqrt(f11)));
  // fcmpu cr6,f7,f11
  cr6 = lifted::CompareFloat(f7, f11);
  // bgt cr6,0x820eff00
  if (cr6.gt) goto loc_820EFF00;
  // fdivs f11,f7,f11
  f11 = double(float(f7 / f11));
  // lfs f7,48(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 48);
  f7 = double(temp.f32);
  // fmuls f0,f0,f11
  f0 = double(float(f0 * f11));
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // fmuls f12,f12,f11
  f12 = double(float(f12 * f11));
  // fsubs f7,f7,f0
  f7 = double(float(f7 - f0));
  // stfs f7,0(r31)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // lfs f7,52(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 52);
  f7 = double(temp.f32);
  // fsubs f7,f7,f13
  f7 = double(float(f7 - f13));
  // stfs f7,4(r31)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r31) + 4, temp.u32);
  // lfs f7,56(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 56);
  f7 = double(temp.f32);
  // fsubs f11,f7,f12
  f11 = double(float(f7 - f12));
  // stfs f11,8(r31)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r31) + 8, temp.u32);
loc_820EFF00:
  // lfs f11,140(r28)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r28) + 140);
  f11 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // fsubs f11,f13,f11
  f11 = double(float(f13 - f11));
  // lfs f13,8676(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 8676);
  f13 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f13,f12,f13
  f13 = double(float(f12 * f13));
  // fadds f0,f10,f0
  f0 = double(float(f10 + f0));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 0, temp.u32);
  // fadds f0,f9,f11
  f0 = double(float(f9 + f11));
  // stfs f0,36(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 36, temp.u32);
  // fadds f0,f8,f13
  f0 = double(float(f8 + f13));
  // stfs f0,40(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 40, temp.u32);
  // lfs f13,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // lfs f0,10796(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 10796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 0, temp.u32);
  // lfs f13,4(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f13 = double(temp.f32);
  // lfs f0,10796(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 10796);
  f0 = double(temp.f32);
  // lfs f12,8(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8);
  f12 = double(temp.f32);
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // stfs f13,4(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r11) + 4, temp.u32);
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 8, temp.u32);
  // lfs f0,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // stfs f0,48(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 48, temp.u32);
  // lfs f0,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f0 = double(temp.f32);
  // stfs f0,52(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 52, temp.u32);
  // lfs f0,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f0 = double(temp.f32);
  // stfs f0,56(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 56, temp.u32);
  // b 0x820effcc
  goto loc_820EFFCC;
loc_820EFF80:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,140(r28)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r28) + 140);
  f11 = double(temp.f32);
  // lfs f9,52(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 52);
  f9 = double(temp.f32);
  // lfs f12,56(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 56);
  f12 = double(temp.f32);
  // fsubs f11,f9,f11
  f11 = double(float(f9 - f11));
  // lfs f10,48(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 48);
  f10 = double(temp.f32);
  // stfs f11,4(r31)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r31) + 4, temp.u32);
  // lfs f13,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f10,0(r31)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // stfs f13,12(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 12, temp.u32);
  // stfs f12,8(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r31) + 8, temp.u32);
  // stfs f13,44(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 44, temp.u32);
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,40(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 40, temp.u32);
  // stfs f0,36(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 36, temp.u32);
  // stfs f0,32(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 32, temp.u32);
  // lfs f0,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f0 = double(temp.f32);
  // stfs f0,52(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 52, temp.u32);
loc_820EFFCC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_820F0068: lifted from generated/ by lift.py.
lifted::Result f820F0068(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820F0068 (lifted)");
  // addi r3,r3,-176
  r3 = uint64_t(int64_t(int64_t(r3) + -176));
  // b 0x820f05d8
  { const lifted::Result o = f820F05D8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820F0120 game::AccessorySkeleton::Swing: lifted from generated/ by lift.py.
lifted::Result f820F0120(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820F0120 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
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
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a7918
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
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f13,336(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).matrix_row3_x_address());
  f13 = double(temp.f32);
  // lfs f0,400(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).last_row3_x_address());
  f0 = double(temp.f32);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // lfs f12,340(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).matrix_row3_y_address());
  f12 = double(temp.f32);
  // lfs f13,404(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).last_row3_y_address());
  f13 = double(temp.f32);
  // fsubs f13,f12,f13
  f13 = double(float(f12 - f13));
  // lfs f11,344(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).matrix_row3_z_address());
  f11 = double(temp.f32);
  // lfs f12,408(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).last_row3_z_address());
  f12 = double(temp.f32);
  // lfs f31,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // fsubs f12,f11,f12
  f12 = double(float(f11 - f12));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f10,416(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).movement_x_address());
  f10 = double(temp.f32);
  // lfs f11,420(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).movement_y_address());
  f11 = double(temp.f32);
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // stfs f31,92(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // lfd f30,26400(r11)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 26400)));
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // fsubs f28,f0,f10
  f28 = double(float(f0 - f10));
  // lfs f0,424(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).movement_z_address());
  f0 = double(temp.f32);
  // fsubs f29,f13,f11
  f29 = double(float(f13 - f11));
  // stfs f28,96(r1)
  temp.f32 = float(f28);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // stfs f29,100(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // fmr f1,f30
  f1 = f30;
  // fsubs f0,f12,f0
  f0 = double(float(f12 - f0));
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r9,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r9);
  // std r11,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r11);
  // fmr f2,f29
  f2 = f29;
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = game::pow(base, LIFTED_ARGS(0x820F01C4, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x820f01e4
  if (!cr6.gt) goto loc_820F01E4;
  // fmr f2,f29
  f2 = f29;
  // fmr f1,f30
  f1 = f30;
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = game::pow(base, LIFTED_ARGS(0x820F01DC, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // b 0x820f01e8
  goto loc_820F01E8;
loc_820F01E4:
  // fmr f13,f31
  lifted::ScalarFloatMode();
  f13 = f31;
loc_820F01E8:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,84(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f12 = double(temp.f32);
  // lfs f11,88(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f11 = double(temp.f32);
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lfs f10,436(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).swing_y_address());
  f10 = double(temp.f32);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lfs f0,10736(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10736);
  f0 = double(temp.f32);
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // fmuls f13,f28,f0
  f13 = double(float(f28 * f0));
  // stfs f13,80(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,84(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // lfs f11,432(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).swing_x_address());
  f11 = double(temp.f32);
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // fadds f13,f11,f13
  f13 = double(float(f11 + f13));
  // lfs f11,440(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).swing_z_address());
  f11 = double(temp.f32);
  // fadds f12,f10,f12
  f12 = double(float(f10 + f12));
  // fadds f11,f11,f0
  f11 = double(float(f11 + f0));
  // lfs f0,7140(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 7140);
  f0 = double(temp.f32);
  // fmuls f29,f13,f0
  f29 = double(float(f13 * f0));
  // stfs f29,96(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // fmuls f30,f12,f0
  f30 = double(float(f12 * f0));
  // stfs f30,100(r1)
  temp.f32 = float(f30);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // fmuls f28,f11,f0
  f28 = double(float(f11 * f0));
  // stfs f28,104(r1)
  temp.f32 = float(f28);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // ld r8,0(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // ld r7,0(r10)
  r7 = guest::LoadU64(base, uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r8,432(r31)
  guest::StoreU64(base, schema::AccessorySkeleton(base, uint32_t(r31)).swing_xy_address(), r8);
  // std r11,440(r31)
  guest::StoreU64(base, schema::AccessorySkeleton(base, uint32_t(r31)).swing_zw_address(), r11);
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).node_file_address());
  // std r7,0(r9)
  guest::StoreU64(base, uint32_t(r9) + 0, r7);
  // std r10,8(r9)
  guest::StoreU64(base, uint32_t(r9) + 8, r10);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).swing_count_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820f02b4
  if (!cr0.gt) goto loc_820F02B4;
loc_820F0290:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x820efd70
  // (left out: dead in C++)
  r3 = game::AccessorySkeleton::SwingNode(base, r1, r13, r3, r4, r5);
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).node_file_address());
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).swing_count_address());
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x820f0290
  if (cr6.lt) goto loc_820F0290;
loc_820F02B4:
  // lwz r11,152(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).stepped_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x820f03b8
  if (cr6.eq) goto loc_820F03B8;
  // lwz r11,264(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).swinging_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x820f03b8
  if (cr6.eq) goto loc_820F03B8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // fcmpu cr6,f29,f0
  cr6 = lifted::CompareFloat(f29, f0);
  // blt cr6,0x820f02e4
  if (cr6.lt) goto loc_820F02E4;
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_820F02E4:
  // fmuls f13,f30,f30
  lifted::ScalarFloatMode();
  f13 = double(float(f30 * f30));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // fcmpu cr6,f30,f0
  cr6 = lifted::CompareFloat(f30, f0);
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmadds f0,f28,f28,f13
  f0 = double(float(std::fma(f28, f28, f13)));
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // fmadds f0,f29,f29,f0
  f0 = double(float(std::fma(f29, f29, f0)));
  // frsp f13,f13
  f13 = double(float(f13));
  // fsqrts f0,f0
  f0 = double(float(sqrt(f0)));
  // fmuls f13,f0,f13
  f13 = double(float(f0 * f13));
  // lfs f0,4208(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4208);
  f0 = double(temp.f32);
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // bge cr6,0x820f032c
  if (!cr6.lt) goto loc_820F032C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,10708(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10708);
  f11 = double(temp.f32);
  // b 0x820f0334
  goto loc_820F0334;
loc_820F032C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,10772(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10772);
  f11 = double(temp.f32);
loc_820F0334:
  // lfs f0,268(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).angle_address());
  f0 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // fmsubs f13,f11,f13,f0
  f13 = double(float(std::fma(f11, f13, -f0)));
  // lfs f10,272(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).angle_speed_address());
  f10 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // addi r4,r31,288
  r4 = uint64_t(int64_t(int64_t(r31) + 288));
  // addi r3,r31,352
  r3 = uint64_t(int64_t(int64_t(r31) + 352));
  // lfs f11,26392(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 26392);
  f11 = double(temp.f32);
  // lfs f12,10796(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10796);
  f12 = double(temp.f32);
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // fadds f13,f13,f10
  f13 = double(float(f13 + f10));
  // stfs f13,272(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).angle_speed_address(), temp.u32);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // fmadds f0,f0,f12,f11
  f0 = double(float(std::fma(f0, f12, f11)));
  // stfs f0,268(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).angle_address(), temp.u32);
  // lfs f13,400(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).last_row3_x_address());
  f13 = double(temp.f32);
  // lfs f0,336(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).matrix_row3_x_address());
  f0 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // lfs f12,340(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).matrix_row3_y_address());
  f12 = double(temp.f32);
  // lfs f0,404(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).last_row3_y_address());
  f0 = double(temp.f32);
  // fsubs f0,f12,f0
  f0 = double(float(f12 - f0));
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // lfs f13,344(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).matrix_row3_z_address());
  f13 = double(temp.f32);
  // lfs f0,408(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).last_row3_z_address());
  f0 = double(temp.f32);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r10,416(r31)
  guest::StoreU64(base, schema::AccessorySkeleton(base, uint32_t(r31)).movement_xy_address(), r10);
  // std r11,424(r31)
  guest::StoreU64(base, schema::AccessorySkeleton(base, uint32_t(r31)).movement_zw_address(), r11);
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = game::Matrix::Copy(base, LIFTED_ARGS(0x820F03B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_820F03B8:
  // mr r3,r31
  r3 = r31;
  // bl 0x8259a400
  // (left out: dead in C++)
  f8259A400(base, r1, r3);
  // mr r3,r31
  r3 = r31;
  // bl 0x8259b660
  // (left out: dead in C++)
  { const lifted::Result o = f8259B660(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,152(r31)
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).stepped_address(), uint32_t(r11));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
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

// sub_820F05D8: lifted from generated/ by lift.py.
lifted::Result f820F05D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820F05D8 (lifted)");
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
  // bl 0x820effd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820EFFD8, LIFTED_ARGS(0x820F05F8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820f0608
  if (cr0.eq) goto loc_820F0608;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820F0608, r31)); r3 = o.r3; f1 = o.f1; }
loc_820F0608:
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

// sub_820F0628 game::AccessorySkeleton::Update: lifted from generated/ by lift.py.
lifted::Result f820F0628(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820F0628 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  guest::StoreU64(base, uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).node_file_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).node_count_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x820f0650
  if (!cr6.gt) goto loc_820F0650;
  // bl 0x8259ace0
  // (left out: dead in C++)
  r3 = f8259ACE0(base, r1, r13, r3);
loc_820F0650:
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).node_file_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).swing_count_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820f06dc
  if (!cr0.gt) goto loc_820F06DC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lfs f31,8116(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8116);
  f31 = double(temp.f32);
loc_820F066C:
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).node_file_address());
  // lwz r9,84(r31)
  r9 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).nodes_address());
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).swing_first_address());
  // add r11,r11,r29
  r11 = r11 + r29;
  // mulli r10,r11,160
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(160))));
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r10,132(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 132);
  // rlwinm. r10,r10,0,24,24
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x80;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x820f06c0
  if (cr0.eq) goto loc_820F06C0;
  // lwz r10,136(r31)
  r10 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).locals_address());
  // mulli r11,r11,144
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(144))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r30,r11,16
  r30 = uint64_t(int64_t(int64_t(r11) + 16));
  // stfs f31,4(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r30) + 4, temp.u32);
  // lfs f2,328(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).matrix_row2_z_address());
  f2 = double(temp.f32);
  // lfs f1,320(r31)
  temp.u32 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).matrix_row2_x_address());
  f1 = double(temp.f32);
  // bl 0x825a8770
  // (left out: dead in C++)
  { const lifted::Result o = game::atan2(base, LIFTED_ARGS(0x820F06B0, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lfs f0,4(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 4);
  f0 = double(temp.f32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,4(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 4, temp.u32);
loc_820F06C0:
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).node_file_address());
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).swing_count_address());
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // blt cr6,0x820f066c
  if (cr6.lt) goto loc_820F066C;
  // mr r3,r31
  r3 = r31;
  // bl 0x8259af20
  // (left out: dead in C++)
  r3 = f8259AF20(base, r1, r13, r3);
loc_820F06DC:
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).node_file_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).node_count_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x820f06f4
  if (!cr6.gt) goto loc_820F06F4;
  // mr r3,r31
  r3 = r31;
  // bl 0x8259c3a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259C3A8, LIFTED_ARGS(0x820F06F4, r31)); r3 = o.r3; f1 = o.f1; }
loc_820F06F4:
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, schema::AccessorySkeleton(base, uint32_t(r31)).node_file_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::AccessoryNodeFile(base, uint32_t(r11)).swing_count_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820f070c
  if (!cr0.gt) goto loc_820F070C;
  // mr r3,r31
  r3 = r31;
  // bl 0x820f0120
  // (left out: dead in C++)
  { const lifted::Result o = game::AccessorySkeleton::Swing(base, r1, r13, r3); r3 = o.r3; f1 = o.f1; }
loc_820F070C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820F0718 game::AccessorySkeleton::Step: lifted from generated/ by lift.py.
lifted::Result f820F0718(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_820F0718 (lifted)");
  uint64_t r5 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r9,-32241
  r9 = uint64_t(int64_t(-2112946176));
  // lwz r8,116(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 116);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r9,r9,1576
  r9 = uint64_t(int64_t(int64_t(r9) + 1576));
  // mr r10,r4
  r10 = r4;
  // addi r4,r3,216
  r4 = uint64_t(int64_t(int64_t(r3) + 216));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // ld r7,80(r1)
  r7 = guest::LoadU64(base, uint32_t(r1) + 80);
  // stw r11,8(r4)
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r4) - 216).unknown_224_address(), uint32_t(r11));
  // stw r11,12(r4)
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r4) - 216).unknown_228_address(), uint32_t(r11));
  // stb r11,16(r4)
  guest::StoreU8(base, schema::AccessorySkeleton(base, uint32_t(r4) - 216).unknown_232_address(), uint8_t(r11));
  // stb r9,17(r4)
  guest::StoreU8(base, schema::AccessorySkeleton(base, uint32_t(r4) - 216).unknown_233_address(), uint8_t(r9));
  // stb r11,18(r4)
  guest::StoreU8(base, schema::AccessorySkeleton(base, uint32_t(r4) - 216).unknown_234_address(), uint8_t(r11));
  // stw r8,20(r4)
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r4) - 216).unknown_236_address(), uint32_t(r8));
  // stw r3,28(r4)
  guest::StoreU32(base, schema::AccessorySkeleton(base, uint32_t(r4) - 216).job_target_address(), uint32_t(r3));
  // std r7,32(r4)
  guest::StoreU64(base, schema::AccessorySkeleton(base, uint32_t(r4) - 216).job_method_pair_address(), r7);
  // beq cr6,0x820f0790
  if (cr6.eq) goto loc_820F0790;
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::AccessoryWorkerPool(base, uint32_t(r10)).vtable_address());
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r10
  r3 = r10;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::AccessoryWorkerPoolVtable(base, uint32_t(r11)).submit_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820F078C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820f07a4
  goto loc_820F07A4;
loc_820F0790:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x820f079c
  if (cr6.eq) goto loc_820F079C;
  // addi r11,r3,220
  r11 = uint64_t(int64_t(int64_t(r3) + 220));
loc_820F079C:
  // mr r4,r11
  r4 = r11;
  // bl 0x820f0628
  // (left out: dead in C++)
  { const lifted::Result o = game::AccessorySkeleton::Update(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
loc_820F07A4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820F0BA0: lifted from generated/ by lift.py.
lifted::Result f820F0BA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820F0BA0 (lifted)");
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
  // bl 0x820f0b58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F0B58, LIFTED_ARGS(0x820F0BC0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820f0bd0
  if (cr0.eq) goto loc_820F0BD0;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x820F0BD0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820F0BD0:
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

lifted::Result e820ACD40(uint8_t* base, const lifted::Args& a) {
  return f820ACD40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.lr);
}

lifted::Result e820ACE80(uint8_t* base, const lifted::Args& a) {
  return f820ACE80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e820ACEC8(uint8_t* base, const lifted::Args& a) {
  return f820ACEC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e820AD188(uint8_t* base, const lifted::Args& a) {
  return f820AD188(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820AD190(uint8_t* base, const lifted::Args& a) {
  return f820AD190(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820AD228(uint8_t* base, const lifted::Args& a) {
  return {f820AD228(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e820AD2B0(uint8_t* base, const lifted::Args& a) {
  return f820AD2B0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.lr);
}

lifted::Result e820AD300(uint8_t* base, const lifted::Args& a) {
  return f820AD300(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.lr);
}

lifted::Result e820AD350(uint8_t* base, const lifted::Args& a) {
  return f820AD350(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820AD5B0(uint8_t* base, const lifted::Args& a) {
  return f820AD5B0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820AD750(uint8_t* base, const lifted::Args& a) {
  return f820AD750(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.lr);
}

lifted::Result e820AD758(uint8_t* base, const lifted::Args& a) {
  return f820AD758(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820AD808(uint8_t* base, const lifted::Args& a) {
  return f820AD808(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820AD8D0(uint8_t* base, const lifted::Args& a) {
  return f820AD8D0(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e820ADA00(uint8_t* base, const lifted::Args& a) {
  return f820ADA00(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e820ADA70(uint8_t* base, const lifted::Args& a) {
  return f820ADA70(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820ADB28(uint8_t* base, const lifted::Args& a) {
  return f820ADB28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820ADB80(uint8_t* base, const lifted::Args& a) {
  return f820ADB80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820AE198(uint8_t* base, const lifted::Args& a) {
  return {f820AE198(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e820AE2B0(uint8_t* base, const lifted::Args& a) {
  return f820AE2B0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820AE2F8(uint8_t* base, const lifted::Args& a) {
  return f820AE2F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820AE5D0(uint8_t* base, const lifted::Args& a) {
  return f820AE5D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820AE688(uint8_t* base, const lifted::Args& a) {
  return f820AE688(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820AE780(uint8_t* base, const lifted::Args& a) {
  return f820AE780(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820AE808(uint8_t* base, const lifted::Args& a) {
  return f820AE808(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e820AE818(uint8_t* base, const lifted::Args& a) {
  return f820AE818(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e820AE9F8(uint8_t* base, const lifted::Args& a) {
  return f820AE9F8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820AEBC0(uint8_t* base, const lifted::Args& a) {
  return f820AEBC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820B0098(uint8_t* base, const lifted::Args& a) {
  f820B0098(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820B00A8(uint8_t* base, const lifted::Args& a) {
  f820B00A8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820B00B8(uint8_t* base, const lifted::Args& a) {
  f820B00B8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820B03E0(uint8_t* base, const lifted::Args& a) {
  f820B03E0(base, a.r[0], a.r[1], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820B0428(uint8_t* base, const lifted::Args& a) {
  f820B0428(base, a.r[0], a.r[1], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820B95F8(uint8_t* base, const lifted::Args& a) {
  return f820B95F8(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e820B9650(uint8_t* base, const lifted::Args& a) {
  return {f820B9650(base, a.r1, a.r13, a.r[0], a.r[1], a.lr), a.f[0]};
}

lifted::Result e820B9680(uint8_t* base, const lifted::Args& a) {
  return f820B9680(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e820B97C0(uint8_t* base, const lifted::Args& a) {
  return {f820B97C0(base, a.r[0]), a.f[0]};
}

lifted::Result e820B97C8(uint8_t* base, const lifted::Args& a) {
  return {f820B97C8(base, a.r1, a.r13, a.r[0], a.r[1], a.lr), a.f[0]};
}

lifted::Result e820B97D0(uint8_t* base, const lifted::Args& a) {
  f820B97D0(base, a.r[0], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820B97D8(uint8_t* base, const lifted::Args& a) {
  return f820B97D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e820B98B0(uint8_t* base, const lifted::Args& a) {
  return f820B98B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820B9950(uint8_t* base, const lifted::Args& a) {
  return {f820B9950(base), a.f[0]};
}

lifted::Result e820B9960(uint8_t* base, const lifted::Args& a) {
  return {f820B9960(base), a.f[0]};
}

lifted::Result e820B9970(uint8_t* base, const lifted::Args& a) {
  return f820B9970(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e820B9C30(uint8_t* base, const lifted::Args& a) {
  f820B9C30(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820B9FF8(uint8_t* base, const lifted::Args& a) {
  return f820B9FF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e820BA1D0(uint8_t* base, const lifted::Args& a) {
  return f820BA1D0(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e820BA228(uint8_t* base, const lifted::Args& a) {
  return f820BA228(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e820BA688(uint8_t* base, const lifted::Args& a) {
  return f820BA688(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e820BA888(uint8_t* base, const lifted::Args& a) {
  return f820BA888(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e820BAA48(uint8_t* base, const lifted::Args& a) {
  return {f820BAA48(base), a.f[0]};
}

lifted::Result e820BB270(uint8_t* base, const lifted::Args& a) {
  return f820BB270(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e820BB410(uint8_t* base, const lifted::Args& a) {
  return f820BB410(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e820BB450(uint8_t* base, const lifted::Args& a) {
  return f820BB450(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e820BC198(uint8_t* base, const lifted::Args& a) {
  return {f820BC198(base, a.r1, a.r13, a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820BC4D0(uint8_t* base, const lifted::Args& a) {
  return {f820BC4D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820BC560(uint8_t* base, const lifted::Args& a) {
  return f820BC560(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820BC828(uint8_t* base, const lifted::Args& a) {
  return f820BC828(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e820BD740(uint8_t* base, const lifted::Args& a) {
  return f820BD740(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e820BD8F8(uint8_t* base, const lifted::Args& a) {
  return f820BD8F8(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e820BDA48(uint8_t* base, const lifted::Args& a) {
  return f820BDA48(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4]);
}

lifted::Result e820BDE40(uint8_t* base, const lifted::Args& a) {
  return f820BDE40(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e820BE0D8(uint8_t* base, const lifted::Args& a) {
  f820BE0D8(base, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0], a.f[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820BE4F0(uint8_t* base, const lifted::Args& a) {
  return f820BE4F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820BE860(uint8_t* base, const lifted::Args& a) {
  return f820BE860(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820BE8A8(uint8_t* base, const lifted::Args& a) {
  return f820BE8A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820BEBB0(uint8_t* base, const lifted::Args& a) {
  return f820BEBB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820BEC50(uint8_t* base, const lifted::Args& a) {
  return f820BEC50(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e820BF690(uint8_t* base, const lifted::Args& a) {
  return f820BF690(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820BFBA8(uint8_t* base, const lifted::Args& a) {
  return f820BFBA8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820BFBB0(uint8_t* base, const lifted::Args& a) {
  return f820BFBB0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820C4E68(uint8_t* base, const lifted::Args& a) {
  return {f820C4E68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820C9D90(uint8_t* base, const lifted::Args& a) {
  return f820C9D90(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820CE7F0(uint8_t* base, const lifted::Args& a) {
  f820CE7F0(base, a.r1, a.r13, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820CEC20(uint8_t* base, const lifted::Args& a) {
  return f820CEC20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820D39D8(uint8_t* base, const lifted::Args& a) {
  f820D39D8(base, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820D69B8(uint8_t* base, const lifted::Args& a) {
  return f820D69B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820D6C10(uint8_t* base, const lifted::Args& a) {
  return f820D6C10(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820D6C88(uint8_t* base, const lifted::Args& a) {
  return f820D6C88(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820D8D20(uint8_t* base, const lifted::Args& a) {
  return f820D8D20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820DCF98(uint8_t* base, const lifted::Args& a) {
  return f820DCF98(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[5], a.r[6]);
}

lifted::Result e820DD438(uint8_t* base, const lifted::Args& a) {
  return f820DD438(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[5], a.r[6]);
}

lifted::Result e820DD948(uint8_t* base, const lifted::Args& a) {
  return f820DD948(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[5], a.r[6]);
}

lifted::Result e820EF250(uint8_t* base, const lifted::Args& a) {
  return f820EF250(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.f[0]);
}

lifted::Result e820EF418(uint8_t* base, const lifted::Args& a) {
  return {f820EF418(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820EF468(uint8_t* base, const lifted::Args& a) {
  return f820EF468(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820EF4E8(uint8_t* base, const lifted::Args& a) {
  return f820EF4E8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820EF740(uint8_t* base, const lifted::Args& a) {
  return f820EF740(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820EF790(uint8_t* base, const lifted::Args& a) {
  return f820EF790(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820EF9B8(uint8_t* base, const lifted::Args& a) {
  return f820EF9B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820EFA78(uint8_t* base, const lifted::Args& a) {
  return f820EFA78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820EFB28(uint8_t* base, const lifted::Args& a) {
  return f820EFB28(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e820EFCF8(uint8_t* base, const lifted::Args& a) {
  return {f820EFCF8(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820EFD70(uint8_t* base, const lifted::Args& a) {
  return {f820EFD70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820F0068(uint8_t* base, const lifted::Args& a) {
  return f820F0068(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820F0120(uint8_t* base, const lifted::Args& a) {
  return f820F0120(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e820F05D8(uint8_t* base, const lifted::Args& a) {
  return f820F05D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820F0628(uint8_t* base, const lifted::Args& a) {
  return f820F0628(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e820F0718(uint8_t* base, const lifted::Args& a) {
  return f820F0718(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.f[0]);
}

lifted::Result e820F0BA0(uint8_t* base, const lifted::Args& a) {
  return f820F0BA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x820ACD40, e820ACD40},
    {0x820ACE80, e820ACE80},
    {0x820ACEC8, e820ACEC8},
    {0x820AD188, e820AD188},
    {0x820AD190, e820AD190},
    {0x820AD228, e820AD228},
    {0x820AD2B0, e820AD2B0},
    {0x820AD300, e820AD300},
    {0x820AD350, e820AD350},
    {0x820AD5B0, e820AD5B0},
    {0x820AD750, e820AD750},
    {0x820AD758, e820AD758},
    {0x820AD808, e820AD808},
    {0x820AD8D0, e820AD8D0},
    {0x820ADA00, e820ADA00},
    {0x820ADA70, e820ADA70},
    {0x820ADB28, e820ADB28},
    {0x820ADB80, e820ADB80},
    {0x820AE198, e820AE198},
    {0x820AE2B0, e820AE2B0},
    {0x820AE2F8, e820AE2F8},
    {0x820AE5D0, e820AE5D0},
    {0x820AE688, e820AE688},
    {0x820AE780, e820AE780},
    {0x820AE808, e820AE808},
    {0x820AE818, e820AE818},
    {0x820AE9F8, e820AE9F8},
    {0x820AEBC0, e820AEBC0},
    {0x820B0098, e820B0098},
    {0x820B00A8, e820B00A8},
    {0x820B00B8, e820B00B8},
    {0x820B03E0, e820B03E0},
    {0x820B0428, e820B0428},
    {0x820B95F8, e820B95F8},
    {0x820B9650, e820B9650},
    {0x820B9680, e820B9680},
    {0x820B97C0, e820B97C0},
    {0x820B97C8, e820B97C8},
    {0x820B97D0, e820B97D0},
    {0x820B97D8, e820B97D8},
    {0x820B98B0, e820B98B0},
    {0x820B9950, e820B9950},
    {0x820B9960, e820B9960},
    {0x820B9970, e820B9970},
    {0x820B9C30, e820B9C30},
    {0x820B9FF8, e820B9FF8},
    {0x820BA1D0, e820BA1D0},
    {0x820BA228, e820BA228},
    {0x820BA688, e820BA688},
    {0x820BA888, e820BA888},
    {0x820BAA48, e820BAA48},
    {0x820BB270, e820BB270},
    {0x820BB410, e820BB410},
    {0x820BB450, e820BB450},
    {0x820BC198, e820BC198},
    {0x820BC4D0, e820BC4D0},
    {0x820BC560, e820BC560},
    {0x820BC828, e820BC828},
    {0x820BD740, e820BD740},
    {0x820BD8F8, e820BD8F8},
    {0x820BDA48, e820BDA48},
    {0x820BDE40, e820BDE40},
    {0x820BE0D8, e820BE0D8},
    {0x820BE4F0, e820BE4F0},
    {0x820BE860, e820BE860},
    {0x820BE8A8, e820BE8A8},
    {0x820BEBB0, e820BEBB0},
    {0x820BEC50, e820BEC50},
    {0x820BF690, e820BF690},
    {0x820BFBA8, e820BFBA8},
    {0x820BFBB0, e820BFBB0},
    {0x820C4E68, e820C4E68},
    {0x820C9D90, e820C9D90},
    {0x820CE7F0, e820CE7F0},
    {0x820CEC20, e820CEC20},
    {0x820D39D8, e820D39D8},
    {0x820D69B8, e820D69B8},
    {0x820D6C10, e820D6C10},
    {0x820D6C88, e820D6C88},
    {0x820D8D20, e820D8D20},
    {0x820DCF98, e820DCF98},
    {0x820DD438, e820DD438},
    {0x820DD948, e820DD948},
    {0x820EF250, e820EF250},
    {0x820EF418, e820EF418},
    {0x820EF468, e820EF468},
    {0x820EF4E8, e820EF4E8},
    {0x820EF740, e820EF740},
    {0x820EF790, e820EF790},
    {0x820EF9B8, e820EF9B8},
    {0x820EFA78, e820EFA78},
    {0x820EFB28, e820EFB28},
    {0x820EFCF8, e820EFCF8},
    {0x820EFD70, e820EFD70},
    {0x820F0068, e820F0068},
    {0x820F0120, e820F0120},
    {0x820F05D8, e820F05D8},
    {0x820F0628, e820F0628},
    {0x820F0718, e820F0718},
    {0x820F0BA0, e820F0BA0},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_004)), true);

}  // namespace

}  // namespace lifted
