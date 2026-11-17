// Lift sweep batch 006 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/heap.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_006, true, "ITC",
              "Run lift sweep batch 006's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82118638: lifted from generated/ by lift.py.
lifted::Result f82118638(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82118638 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-384(r1)
  ea = -384 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r11,r31,1556
  r11 = uint64_t(int64_t(int64_t(r31) + 1556));
  // lwz r10,24(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24);
  // cmplwi cr6,r10,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 16, so);
  // blt cr6,0x82118664
  if (cr6.lt) goto loc_82118664;
  // lwz r5,4(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 4);
  // b 0x82118668
  goto loc_82118668;
loc_82118664:
  // addi r5,r11,4
  r5 = uint64_t(int64_t(int64_t(r11) + 4));
loc_82118668:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r11,-31920
  r4 = uint64_t(int64_t(int64_t(r11) + -31920));
  // bl 0x825a93d0
  // (left out: dead in C++)
  { const lifted::Result o = game::sprintf(base, LIFTED_ARGS(0x82118678, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820a6a00
  // (left out: dead in C++)
  { const lifted::Result o = game::GameFile::Ctor(base, LIFTED_ARGS(0x82118680, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stb r11,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r11));
  // bl 0x820a6ab8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820A6AB8(base, LIFTED_ARGS(0x82118698, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82118704
  if (cr0.eq) goto loc_82118704;
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // bl 0x8259d1f0
  // (left out: dead in C++)
  { const lifted::Result o = f8259D1F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x8258c718
  // (left out: dead in C++)
  { const lifted::Result o = game::File::Close(base, LIFTED_ARGS(0x821186B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,-31928
  r4 = uint64_t(int64_t(int64_t(r11) + -31928));
  // lwz r11,116(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 116);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821186D0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r3));
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82118704
  if (cr6.eq) goto loc_82118704;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lwz r11,120(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 120);
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82118704, r31)); r3 = o.r3; f1 = o.f1; }
loc_82118704:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820a6a40
  // (left out: dead in C++)
  { const lifted::Result o = game::GameFile::Dtor(base, LIFTED_ARGS(0x8211870C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,384
  r1 = uint64_t(int64_t(int64_t(r1) + 384));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82118790 game::HandleList::Remove: lifted from generated/ by lift.py.
lifted::Result f82118790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_82118790 (lifted)");
  uint64_t r5 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r9,r31
  r9 = r31;
  // lwz r30,4(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
loc_821187B4:
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_821187B8:
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x821187c8
  if (cr6.eq) goto loc_821187C8;
  // cmplw cr6,r9,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r31), so);
  // beq cr6,0x821187cc
  if (cr6.eq) goto loc_821187CC;
loc_821187C8:
  // twi 31,r0,22
  lifted::Trap(22);
loc_821187CC:
  // subf r10,r11,r30
  r10 = r30 - r11;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm. r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82118848
  if (cr0.eq) goto loc_82118848;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x821187f0
  if (!cr6.eq) goto loc_821187F0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821187F0:
  // lwz r10,4(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82118800
  if (!cr6.eq) goto loc_82118800;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118800:
  // lwz r8,8(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lwz r7,0(r29)
  r7 = guest::LoadU32(base, uint32_t(r29) + 0);
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // bne cr6,0x82118834
  if (!cr6.eq) goto loc_82118834;
  // mr r4,r31
  r4 = r31;
  // ld r5,80(r1)
  r5 = guest::LoadU64(base, uint32_t(r1) + 80);
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x82098e80
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Erase(base, LIFTED_ARGS(0x82118820, r31)); r3 = o.r3; f1 = o.f1; }
  // ld r11,0(r3)
  r11 = guest::LoadU64(base, uint32_t(r3) + 0);
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // lwz r9,80(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 80);
  // b 0x821187b8
  goto loc_821187B8;
loc_82118834:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82118840
  if (!cr6.eq) goto loc_82118840;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118840:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // b 0x821187b4
  goto loc_821187B4;
loc_82118848:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82118850: lifted from generated/ by lift.py.
lifted::Result f82118850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82118850 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r3
  r30 = r3;
  // std r4,176(r1)
  guest::StoreU64(base, uint32_t(r1) + 176, r4);
  // std r5,184(r1)
  guest::StoreU64(base, uint32_t(r1) + 184, r5);
  // mr r29,r30
  r29 = r30;
  // lwz r28,4(r30)
  r28 = guest::LoadU32(base, uint32_t(r30) + 4);
  // stw r29,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r29));
  // lwz r31,0(r28)
  r31 = guest::LoadU32(base, uint32_t(r28) + 0);
loc_82118878:
  // stw r31,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r31));
loc_8211887C:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8211888c
  if (cr6.eq) goto loc_8211888C;
  // cmplw cr6,r29,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r30), so);
  // beq cr6,0x82118890
  if (cr6.eq) goto loc_82118890;
loc_8211888C:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118890:
  // subf r11,r31,r28
  r11 = r28 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82118924
  if (cr0.eq) goto loc_82118924;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x821188b4
  if (!cr6.eq) goto loc_821188B4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821188B4:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x821188c4
  if (!cr6.eq) goto loc_821188C4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821188C4:
  // lwz r11,184(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 184);
  // lwz r10,180(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 8);
  // add r3,r10,r11
  r3 = r10 + r11;
  // lwz r11,176(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 176);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821188E0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8211890c
  if (cr0.eq) goto loc_8211890C;
  // mr r4,r30
  r4 = r30;
  // ld r5,80(r1)
  r5 = guest::LoadU64(base, uint32_t(r1) + 80);
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x82098e80
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Erase(base, LIFTED_ARGS(0x821188F8, r31)); r3 = o.r3; f1 = o.f1; }
  // ld r11,0(r3)
  r11 = guest::LoadU64(base, uint32_t(r3) + 0);
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lwz r31,84(r1)
  r31 = guest::LoadU32(base, uint32_t(r1) + 84);
  // lwz r29,80(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 80);
  // b 0x8211887c
  goto loc_8211887C;
loc_8211890C:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x8211891c
  if (!cr6.eq) goto loc_8211891C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8211891C:
  // lwz r31,0(r31)
  r31 = guest::LoadU32(base, uint32_t(r31) + 0);
  // b 0x82118878
  goto loc_82118878;
loc_82118924:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82118930: lifted from generated/ by lift.py.
lifted::Result f82118930(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82118930 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // addi r30,r3,1512
  r30 = uint64_t(int64_t(int64_t(r3) + 1512));
  // lwz r31,4(r30)
  r31 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // ble cr6,0x82118954
  if (!cr6.gt) goto loc_82118954;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118954:
  // lwz r29,8(r30)
  r29 = guest::LoadU32(base, uint32_t(r30) + 8);
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r10,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r29), so);
  // ble cr6,0x82118968
  if (!cr6.gt) goto loc_82118968;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118968:
  // cmplw cr6,r30,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r30), so);
  // beq cr6,0x82118974
  if (cr6.eq) goto loc_82118974;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118974:
  // subf r10,r31,r29
  r10 = r29 - r31;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm. r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821189e8
  if (cr0.eq) goto loc_821189E8;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x82118998
  if (cr6.lt) goto loc_82118998;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118998:
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x821189d0
  if (cr6.eq) goto loc_821189D0;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x821189b0
  if (cr6.lt) goto loc_821189B0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821189B0:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x821176e8
  // (left out: dead in C++)
  { const lifted::Result o = game::BankPair::CanFree(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x821189c8
  if (cr6.lt) goto loc_821189C8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821189C8:
  // mr r3,r31
  r3 = r31;
  // bl 0x82118438
  // (left out: dead in C++)
  { const lifted::Result o = f82118438(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821189D0:
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x821189e0
  if (cr6.lt) goto loc_821189E0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821189E0:
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // b 0x82118968
  goto loc_82118968;
loc_821189E8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821189F0 game::AudioManager::StopBank: lifted from generated/ by lift.py.
lifted::Result f821189F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821189F0 (lifted)");
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
  // addi r30,r3,1528
  r30 = uint64_t(int64_t(int64_t(r3) + 1528));
  // mr r29,r4
  r29 = r4;
  // lwz r28,4(r30)
  r28 = guest::LoadU32(base, uint32_t(r30) + 4);
  // mr r11,r28
  r11 = r28;
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 0);
loc_82118A10:
  // cmplw cr6,r30,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r30), so);
  // beq cr6,0x82118a1c
  if (cr6.eq) goto loc_82118A1C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118A1C:
  // subf r11,r31,r28
  r11 = r28 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82118a88
  if (cr0.eq) goto loc_82118A88;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x82118a44
  if (!cr6.eq) goto loc_82118A44;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118A44:
  // mr r4,r29
  r4 = r29;
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x821175b8
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::IsBank(base, LIFTED_ARGS(0x82118A50, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82118a70
  if (cr0.eq) goto loc_82118A70;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x82118a68
  if (!cr6.eq) goto loc_82118A68;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118A68:
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x82117400
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Stop(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1, 0x82118A70); r3 = o.r3; f1 = o.f1; }
loc_82118A70:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x82118a80
  if (!cr6.eq) goto loc_82118A80;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118A80:
  // lwz r31,0(r31)
  r31 = guest::LoadU32(base, uint32_t(r31) + 0);
  // b 0x82118a10
  goto loc_82118A10;
loc_82118A88:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82118A90 game::AudioManager::StopCue: lifted from generated/ by lift.py.
lifted::Result f82118A90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82118A90 (lifted)");
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r30,r3,1528
  r30 = uint64_t(int64_t(int64_t(r3) + 1528));
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // lwz r27,4(r30)
  r27 = guest::LoadU32(base, uint32_t(r30) + 4);
  // mr r11,r27
  r11 = r27;
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 0);
loc_82118AB4:
  // cmplw cr6,r30,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r30), so);
  // beq cr6,0x82118ac0
  if (cr6.eq) goto loc_82118AC0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118AC0:
  // subf r11,r31,r27
  r11 = r27 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82118b50
  if (cr0.eq) goto loc_82118B50;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x82118ae8
  if (!cr6.eq) goto loc_82118AE8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118AE8:
  // mr r4,r29
  r4 = r29;
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x821175b8
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::IsBank(base, LIFTED_ARGS(0x82118AF4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82118b38
  if (cr0.eq) goto loc_82118B38;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x82118b0c
  if (!cr6.eq) goto loc_82118B0C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118B0C:
  // mr r4,r28
  r4 = r28;
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x82117578
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::IsCue(base, LIFTED_ARGS(0x82118B18, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82118b38
  if (cr0.eq) goto loc_82118B38;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x82118b30
  if (!cr6.eq) goto loc_82118B30;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118B30:
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x82117400
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Stop(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1, 0x82118B38); r3 = o.r3; f1 = o.f1; }
loc_82118B38:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x82118b48
  if (!cr6.eq) goto loc_82118B48;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118B48:
  // lwz r31,0(r31)
  r31 = guest::LoadU32(base, uint32_t(r31) + 0);
  // b 0x82118ab4
  goto loc_82118AB4;
loc_82118B50:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82118C70 game::AudioManager::ClearHandles: lifted from generated/ by lift.py.
lifted::Result f82118C70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82118C70 (lifted)");
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
  // addi r31,r3,1540
  r31 = uint64_t(int64_t(int64_t(r3) + 1540));
  // lwz r29,4(r31)
  r29 = guest::LoadU32(base, uint32_t(r31) + 4);
  // mr r11,r29
  r11 = r29;
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
loc_82118C8C:
  // cmplw cr6,r31,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r31), so);
  // beq cr6,0x82118c98
  if (cr6.eq) goto loc_82118C98;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118C98:
  // subf r11,r30,r29
  r11 = r29 - r30;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82118ce0
  if (cr0.eq) goto loc_82118CE0;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x82118cc0
  if (!cr6.eq) goto loc_82118CC0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118CC0:
  // addi r3,r30,8
  r3 = uint64_t(int64_t(int64_t(r30) + 8));
  // bl 0x82118490
  // (left out: dead in C++)
  { const lifted::Result o = f82118490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x82118cd8
  if (!cr6.eq) goto loc_82118CD8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118CD8:
  // lwz r30,0(r30)
  r30 = guest::LoadU32(base, uint32_t(r30) + 0);
  // b 0x82118c8c
  goto loc_82118C8C;
loc_82118CE0:
  // mr r3,r31
  r3 = r31;
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x82118CE8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82118CF0 game::AudioManager::FindHandle: lifted from generated/ by lift.py.
uint64_t f82118CF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82118CF0 (lifted)");
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
  // addi r31,r3,1540
  r31 = uint64_t(int64_t(int64_t(r3) + 1540));
  // mr r29,r4
  r29 = r4;
  // lwz r28,4(r31)
  r28 = guest::LoadU32(base, uint32_t(r31) + 4);
  // mr r11,r28
  r11 = r28;
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
loc_82118D10:
  // cmplw cr6,r31,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r31), so);
  // beq cr6,0x82118d1c
  if (cr6.eq) goto loc_82118D1C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118D1C:
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
  // beq 0x82118d80
  if (cr0.eq) goto loc_82118D80;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x82118d44
  if (!cr6.eq) goto loc_82118D44;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118D44:
  // mr r4,r29
  r4 = r29;
  // lwz r3,8(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 8);
  // bl 0x82117598
  // (left out: dead in C++)
  r3 = game::Sound::HasHandle(base, r3, r4);
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne 0x82118d70
  if (!cr0.eq) goto loc_82118D70;
  // bne cr6,0x82118d68
  if (!cr6.eq) goto loc_82118D68;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118D68:
  // lwz r30,0(r30)
  r30 = guest::LoadU32(base, uint32_t(r30) + 0);
  // b 0x82118d10
  goto loc_82118D10;
loc_82118D70:
  // bne cr6,0x82118d78
  if (!cr6.eq) goto loc_82118D78;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118D78:
  // lwz r3,8(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 8);
  // b 0x82118d84
  goto loc_82118D84;
loc_82118D80:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82118D84:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82118D90 game::AudioManager::SetHandlePosition: lifted from generated/ by lift.py.
uint64_t f82118D90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82118D90 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r5
  r31 = r5;
  // bl 0x82118cf0
  // (left out: dead in C++)
  r3 = game::AudioManager::FindHandle(base, r1, r13, r3, r4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82118db8
  if (cr0.eq) goto loc_82118DB8;
  // mr r4,r31
  r4 = r31;
  // bl 0x82117560
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::SetPosition(base, LIFTED_ARGS(0x82118DB8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82118DB8:
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

// sub_82118DD0 game::AudioManager::SetHandleVelocity: lifted from generated/ by lift.py.
uint64_t f82118DD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82118DD0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r5
  r31 = r5;
  // bl 0x82118cf0
  // (left out: dead in C++)
  r3 = game::AudioManager::FindHandle(base, r1, r13, r3, r4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82118df8
  if (cr0.eq) goto loc_82118DF8;
  // mr r4,r31
  r4 = r31;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82118DF8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82118DF8:
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

// sub_82118E10 game::AudioManager::SetHandle3D: lifted from generated/ by lift.py.
uint64_t f82118E10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82118E10 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r5
  r31 = r5;
  // bl 0x82118cf0
  // (left out: dead in C++)
  r3 = game::AudioManager::FindHandle(base, r1, r13, r3, r4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82118e38
  if (cr0.eq) goto loc_82118E38;
  // mr r4,r31
  r4 = r31;
  // bl 0x82117568
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Set3D(base, LIFTED_ARGS(0x82118E38, r31)); r3 = o.r3; f1 = o.f1; }
loc_82118E38:
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

// sub_82118E50 game::AudioManager::SetHandle3DParams: lifted from generated/ by lift.py.
uint64_t f82118E50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_82118E50 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r5
  r30 = r5;
  // mr r29,r6
  r29 = r6;
  // mr r28,r7
  r28 = r7;
  // bl 0x82118cf0
  // (left out: dead in C++)
  r3 = game::AudioManager::FindHandle(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x82118e98
  if (cr0.eq) goto loc_82118E98;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82117560
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::SetPosition(base, LIFTED_ARGS(0x82118E80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82118E8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x82117568
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Set3D(base, LIFTED_ARGS(0x82118E98, r31)); r3 = o.r3; f1 = o.f1; }
loc_82118E98:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82118EA0 game::AudioManager::StartHandle: lifted from generated/ by lift.py.
uint64_t f82118EA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82118EA0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // bl 0x82118cf0
  // (left out: dead in C++)
  r3 = game::AudioManager::FindHandle(base, r1, r13, r3, r4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82118ebc
  if (cr0.eq) goto loc_82118EBC;
  // bl 0x821173f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Start(base, LIFTED_ARGS(0x82118EBC, r31)); r3 = o.r3; f1 = o.f1; }
loc_82118EBC:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82118ED0 game::AudioManager::StopHandle: lifted from generated/ by lift.py.
lifted::Result f82118ED0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82118ED0 (lifted)");
  uint64_t r12 = 0;
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
  // bl 0x82118cf0
  // (left out: dead in C++)
  r3 = game::AudioManager::FindHandle(base, r1, r13, r3, r4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82118eec
  if (cr0.eq) goto loc_82118EEC;
  // bl 0x82117400
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Stop(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1, 0x82118EEC); r3 = o.r3; f1 = o.f1; }
loc_82118EEC:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82118F00 game::AudioManager::RemoveHandle: lifted from generated/ by lift.py.
lifted::Result f82118F00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82118F00 (lifted)");
  uint64_t r12 = 0, r31 = 0;
  lifted::Cr cr0;
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
  // bl 0x82118cf0
  // (left out: dead in C++)
  r3 = game::AudioManager::FindHandle(base, r1, r13, r3, r4);
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82118f38
  if (cr0.eq) goto loc_82118F38;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r31,1540
  r3 = uint64_t(int64_t(int64_t(r31) + 1540));
  // bl 0x82118790
  // (left out: dead in C++)
  { const lifted::Result o = game::HandleList::Remove(base, r1, r13, r3, r4, r6, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82118490
  // (left out: dead in C++)
  { const lifted::Result o = f82118490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82118F38:
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

// sub_82118F50: lifted from generated/ by lift.py.
lifted::Result f82118F50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82118F50 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // std r4,136(r1)
  guest::StoreU64(base, uint32_t(r1) + 136, r4);
  // mr r29,r3
  r29 = r3;
  // std r5,144(r1)
  guest::StoreU64(base, uint32_t(r1) + 144, r5);
  // mr r30,r6
  r30 = r6;
  // lwz r31,140(r1)
  r31 = guest::LoadU32(base, uint32_t(r1) + 140);
loc_82118F70:
  // lwz r11,148(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 148);
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
  // beq 0x82118fa0
  if (cr0.eq) goto loc_82118FA0;
  // lwz r3,8(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 8);
  // mtctr r30
  ctr = r30;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82118F98, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r31)
  r31 = guest::LoadU32(base, uint32_t(r31) + 0);
  // b 0x82118f70
  goto loc_82118F70;
loc_82118FA0:
  // mr r3,r29
  r3 = r29;
  // stw r30,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r30));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82119018: lifted from generated/ by lift.py.
lifted::Result f82119018(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82119018 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r10,-32238
  r10 = uint64_t(int64_t(-2112749568));
  // mr r11,r3
  r11 = r3;
  // addi r10,r10,-31216
  r10 = uint64_t(int64_t(int64_t(r10) + -31216));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r31,r11,1528
  r31 = uint64_t(int64_t(int64_t(r11) + 1528));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r7,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r7));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // ld r10,80(r1)
  r10 = guest::LoadU64(base, uint32_t(r1) + 80);
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r31,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r31));
  // std r10,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r10);
  // mr r10,r11
  r10 = r11;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // ld r6,0(r9)
  r6 = guest::LoadU64(base, uint32_t(r9) + 0);
  // ld r7,8(r9)
  r7 = guest::LoadU64(base, uint32_t(r9) + 8);
  // stw r31,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r31));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // ld r5,0(r8)
  r5 = guest::LoadU64(base, uint32_t(r8) + 0);
  // ld r4,0(r4)
  r4 = guest::LoadU64(base, uint32_t(r4) + 0);
  // bl 0x820ad3a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AD3A0, LIFTED_ARGS(0x82119090, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x82119098, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821190B0 game::AudioManager::UpdateHandles: lifted from generated/ by lift.py.
lifted::Result f821190B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_821190B0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32239
  r10 = uint64_t(int64_t(-2112815104));
  // addi r11,r3,1540
  r11 = uint64_t(int64_t(int64_t(r3) + 1540));
  // addi r10,r10,29752
  r10 = uint64_t(int64_t(int64_t(r10) + 29752));
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
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82119118 game::AudioManager::Update: lifted from generated/ by lift.py.
lifted::Result f82119118(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82119118 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // bl 0x8259d1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::AudioEngine::DoWork(base, LIFTED_ARGS(0x82119134, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,1596(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 1596);
  // addi r11,r11,-3
  r11 = uint64_t(int64_t(int64_t(r11) + -3));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82119150
  if (cr0.eq) goto loc_82119150;
  // addi r3,r31,1584
  r3 = uint64_t(int64_t(int64_t(r31) + 1584));
  // bl 0x82094b10
  // (left out: dead in C++)
  { const lifted::Result o = game::Job::Wait(base, LIFTED_ARGS(0x82119150, r31)); r3 = o.r3; f1 = o.f1; }
loc_82119150:
  // mr r3,r31
  r3 = r31;
  // bl 0x821184e8
  // (left out: dead in C++)
  { const lifted::Result o = game::AudioManager::FreeReleasedBanks(base, LIFTED_ARGS(0x82119158, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x821184e8
  // (left out: dead in C++)
  { const lifted::Result o = game::AudioManager::FreeReleasedBanks(base, LIFTED_ARGS(0x82119160, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82118fb0
  // (left out: dead in C++)
  { const lifted::Result o = game::AudioManager::UpdateSounds(base, LIFTED_ARGS(0x82119168, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82118c18
  // (left out: dead in C++)
  { const lifted::Result o = game::AudioManager::RemoveStoppedSounds(base, LIFTED_ARGS(0x82119170, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82119188: lifted from generated/ by lift.py.
lifted::Result f82119188(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82119188 (lifted)");
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x8211923c
  if (cr6.eq) goto loc_8211923C;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x82119260
  if (!cr6.eq) goto loc_82119260;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,32(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 32);
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821191D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821191F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r3,r31
  r3 = r31;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24);
  // stw r10,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82119218, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // addi r4,r1,92
  r4 = uint64_t(int64_t(int64_t(r1) + 92));
  // mr r3,r31
  r3 = r31;
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20);
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82119238, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82119260
  goto loc_82119260;
loc_8211923C:
  // mr r3,r30
  r3 = r30;
  // bl 0x82119018
  // (left out: dead in C++)
  { const lifted::Result o = f82119018(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,92(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 92);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82119258, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82118930
  // (left out: dead in C++)
  { const lifted::Result o = f82118930(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_82119260:
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

// sub_82119278 game::AudioManager::Destroy: lifted from generated/ by lift.py.
lifted::Result f82119278(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82119278 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-31864
  r11 = uint64_t(int64_t(int64_t(r11) + -31864));
  // addi r10,r10,-31884
  r10 = uint64_t(int64_t(int64_t(r10) + -31884));
  // addi r9,r9,-31900
  r9 = uint64_t(int64_t(int64_t(r9) + -31900));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r9));
  // bl 0x82119018
  // (left out: dead in C++)
  { const lifted::Result o = f82119018(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82118930
  // (left out: dead in C++)
  { const lifted::Result o = f82118930(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r27,r31,16
  r27 = uint64_t(int64_t(int64_t(r31) + 16));
  // mr r3,r27
  r3 = r27;
  // bl 0x8259d370
  // (left out: dead in C++)
  { const lifted::Result o = f8259D370(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // addi r11,r31,1584
  r11 = uint64_t(int64_t(int64_t(r31) + 1584));
  // addi r10,r10,2748
  r10 = uint64_t(int64_t(int64_t(r10) + 2748));
  // addi r9,r9,2984
  r9 = uint64_t(int64_t(int64_t(r9) + 2984));
  // addi r8,r8,2740
  r8 = uint64_t(int64_t(int64_t(r8) + 2740));
  // addi r7,r7,3768
  r7 = uint64_t(int64_t(int64_t(r7) + 3768));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // addi r3,r31,1556
  r3 = uint64_t(int64_t(int64_t(r31) + 1556));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r7));
  // bl 0x82091328
  // (left out: dead in C++)
  { const lifted::Result o = game::Std::string::_Tidy(base, LIFTED_ARGS(0x82119308, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,1540
  r30 = uint64_t(int64_t(int64_t(r31) + 1540));
  // mr r3,r30
  r3 = r30;
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x82119314, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 4);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8211931C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // addi r29,r31,1528
  r29 = uint64_t(int64_t(int64_t(r31) + 1528));
  // mr r3,r29
  r3 = r29;
  // stw r28,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r28));
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS(0x82119330, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 4);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82119338, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,1512
  r30 = uint64_t(int64_t(int64_t(r31) + 1512));
  // stw r28,4(r29)
  guest::StoreU32(base, uint32_t(r29) + 4, uint32_t(r28));
  // lwz r3,4(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82119350
  if (cr0.eq) goto loc_82119350;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82119350, r31)); r3 = o.r3; f1 = o.f1; }
loc_82119350:
  // mr r3,r27
  r3 = r27;
  // stw r28,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r28));
  // stw r28,8(r30)
  guest::StoreU32(base, uint32_t(r30) + 8, uint32_t(r28));
  // stw r28,12(r30)
  guest::StoreU32(base, uint32_t(r30) + 12, uint32_t(r28));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82119364, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,3400
  r11 = uint64_t(int64_t(int64_t(r11) + 3400));
  // addi r10,r10,3400
  r10 = uint64_t(int64_t(int64_t(r10) + 3400));
  // addi r9,r9,-32072
  r9 = uint64_t(int64_t(int64_t(r9) + -32072));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r9));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82119390: lifted from generated/ by lift.py.
lifted::Result f82119390(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82119390 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,8(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 8);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_821193A8: lifted from generated/ by lift.py.
uint64_t f821193A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_821193A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82117ea8
  { const lifted::Result o = game::AudioManager::GetEngine(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_821193B0: lifted from generated/ by lift.py.
lifted::Result f821193B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821193B0 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82119450
  { const lifted::Result o = game::AudioManager::Dtor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_821193B8: lifted from generated/ by lift.py.
lifted::Result f821193B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821193B8 (lifted)");
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x82119450
  { const lifted::Result o = game::AudioManager::Dtor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82119450 game::AudioManager::Dtor: lifted from generated/ by lift.py.
lifted::Result f82119450(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82119450 (lifted)");
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
  // bl 0x82119278
  // (left out: dead in C++)
  { const lifted::Result o = game::AudioManager::Destroy(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82119480
  if (cr0.eq) goto loc_82119480;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82119480, r31)); r3 = o.r3; f1 = o.f1; }
loc_82119480:
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

// sub_82119500 game::AudioManager::PlayCue: lifted from generated/ by lift.py.
lifted::Result f82119500(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82119500 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r31,r4
  r31 = r4;
  // mr r30,r3
  r30 = r3;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // mr r27,r7
  r27 = r7;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r26,r8
  r26 = r8;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x82119600
  if (cr0.lt) goto loc_82119600;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, uint32_t(r29) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x82119600
  if (cr6.lt) goto loc_82119600;
  // lwz r10,1516(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 1516);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x8211955c
  if (cr0.eq) goto loc_8211955C;
  // lwz r9,1520(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 1520);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x82119560
  if (cr6.lt) goto loc_82119560;
loc_8211955C:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82119560:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82119600
  if (cr6.eq) goto loc_82119600;
  // li r3,24
  r3 = uint64_t(int64_t(24));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82119578, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821195a4
  if (cr0.eq) goto loc_821195A4;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // addi r6,r30,8
  r6 = uint64_t(int64_t(int64_t(r30) + 8));
  // bne cr6,0x82119590
  if (!cr6.eq) goto loc_82119590;
  // li r6,0
  r6 = uint64_t(int64_t(0));
loc_82119590:
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // bl 0x82117348
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Ctor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // b 0x821195a8
  goto loc_821195A8;
loc_821195A4:
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_821195A8:
  // mr r4,r26
  r4 = r26;
  // stw r31,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // bl 0x82117568
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Set3D(base, LIFTED_ARGS(0x821195B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x82117560
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::SetPosition(base, LIFTED_ARGS(0x821195C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r27
  r4 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x821195D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82117438
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Update(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x821173f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Start(base, LIFTED_ARGS(0x821195E0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r30,1528
  r3 = uint64_t(int64_t(int64_t(r30) + 1528));
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 4);
  // stw r3,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r3));
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // ld r4,0(r11)
  r4 = guest::LoadU64(base, uint32_t(r11) + 0);
  // bl 0x82095958
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Insert(base, LIFTED_ARGS(0x82119600, r31)); r3 = o.r3; f1 = o.f1; }
loc_82119600:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82119608 game::AudioManager::PlayCue3D: lifted from generated/ by lift.py.
lifted::Result f82119608(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82119608 (lifted)");
  uint64_t r8 = 0, r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // stb r11,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r11));
  // bl 0x82119500
  // (left out: dead in C++)
  { const lifted::Result o = game::AudioManager::PlayCue(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82119638 game::AudioManager::PlayCue2D: lifted from generated/ by lift.py.
lifted::Result f82119638(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82119638 (lifted)");
  uint64_t r7 = 0, r8 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r7,r6
  r7 = r6;
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // stb r11,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r11));
  // bl 0x82119500
  // (left out: dead in C++)
  { const lifted::Result o = game::AudioManager::PlayCue(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82119690 game::AudioManager::PlayHandle: lifted from generated/ by lift.py.
lifted::Result f82119690(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82119690 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r29,r3
  r29 = r3;
  // mr r30,r5
  r30 = r5;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x821197cc
  if (cr0.lt) goto loc_821197CC;
  // lwz r10,0(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x821197cc
  if (cr6.lt) goto loc_821197CC;
  // lwz r10,1516(r29)
  r10 = guest::LoadU32(base, uint32_t(r29) + 1516);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x821196e4
  if (cr0.eq) goto loc_821196E4;
  // lwz r9,1520(r29)
  r9 = guest::LoadU32(base, uint32_t(r29) + 1520);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x821196e8
  if (cr6.lt) goto loc_821196E8;
loc_821196E4:
  // twi 31,r0,22
  lifted::Trap(22);
loc_821196E8:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x821197cc
  if (cr6.eq) goto loc_821197CC;
  // li r3,24
  r3 = uint64_t(int64_t(24));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82119700, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8211972c
  if (cr0.eq) goto loc_8211972C;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // addi r6,r29,8
  r6 = uint64_t(int64_t(int64_t(r29) + 8));
  // bne cr6,0x82119718
  if (!cr6.eq) goto loc_82119718;
  // li r6,0
  r6 = uint64_t(int64_t(0));
loc_82119718:
  // mr r5,r30
  r5 = r30;
  // mr r4,r31
  r4 = r31;
  // bl 0x82117348
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Ctor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // b 0x82119730
  goto loc_82119730;
loc_8211972C:
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_82119730:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r31,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r31));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stb r11,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r11));
  // bl 0x82117568
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Set3D(base, LIFTED_ARGS(0x82119748, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 104, temp.u32);
  // lfs f31,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // bl 0x82117560
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::SetPosition(base, LIFTED_ARGS(0x82119774, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // stfs f31,88(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // mr r3,r31
  r3 = r31;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x82119784, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r29,1552
  r30 = uint64_t(int64_t(int64_t(r29) + 1552));
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // bl 0x82117550
  // (left out: dead in C++)
  game::Sound::SetHandle(base, r3, r4);
  // mr r3,r31
  r3 = r31;
  // bl 0x82117438
  // (left out: dead in C++)
  { const lifted::Result o = game::Sound::Update(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r29,1540
  r3 = uint64_t(int64_t(int64_t(r29) + 1540));
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 4);
  // stw r3,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r3));
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // ld r4,0(r11)
  r4 = guest::LoadU64(base, uint32_t(r11) + 0);
  // bl 0x82095958
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Insert(base, LIFTED_ARGS(0x821197BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r11,r3,1
  r11 = uint64_t(int64_t(int64_t(r3) + 1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
  // b 0x821197d0
  goto loc_821197D0;
loc_821197CC:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_821197D0:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8211C2C8 game::UiMovie::SetCharText: lifted from generated/ by lift.py.
lifted::Result f8211C2C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8211C2C8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r5
  r30 = r5;
  // mr r28,r6
  r28 = r6;
  // cmpwi cr6,r4,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), -1, so);
  // lwz r3,16(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 16);
  // beq cr6,0x8211c2f0
  if (cr6.eq) goto loc_8211C2F0;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211C2F0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8211C2F0:
  // mr r4,r30
  r4 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211C2F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,21592
  r10 = uint64_t(int64_t(1415053312));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // ori r10,r10,21504
  r10 = r10 | 21504;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8211c310
  if (cr6.eq) goto loc_8211C310;
loc_8211C30C:
  // b 0x8211c30c
  goto loc_8211C30C;
loc_8211C310:
  // addi r30,r3,108
  r30 = uint64_t(int64_t(int64_t(r3) + 108));
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // bl 0x8211b660
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::GetText(base, LIFTED_ARGS(0x8211C320, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // stw r29,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8211C338, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r4,0(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 0);
  // mr r5,r28
  r5 = r28;
  // lwz r3,23552(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 23552);
  // bl 0x82108a70
  // (left out: dead in C++)
  r3 = f82108A70(base, r1, r13, r4, r5);
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addis r3,r31,1
  r3 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r3,r3,-25652
  r3 = uint64_t(int64_t(int64_t(r3) + -25652));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x8211b370
  // (left out: dead in C++)
  { const lifted::Result o = game::TextMap::Set(base, LIFTED_ARGS(0x8211C368, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8211E8A8 game::LightSprite::Init: lifted from generated/ by lift.py.
lifted::Result f8211E8A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8211E8A8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,8(r4)
  r11 = guest::LoadU32(base, uint32_t(r4) + 8);
  // lwz r10,0(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 0);
  // stw r11,140(r3)
  guest::StoreU32(base, uint32_t(r3) + 140, uint32_t(r11));
  // lwz r11,12(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 12);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8211F0C0 game::LightSprite::Dtor: lifted from generated/ by lift.py.
lifted::Result f8211F0C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8211F0C0 (lifted)");
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
  // addi r11,r11,-31676
  r11 = uint64_t(int64_t(int64_t(r11) + -31676));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82126498
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::Ctor(base, LIFTED_ARGS(0x8211F0EC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8211f0fc
  if (cr0.eq) goto loc_8211F0FC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8211F0FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8211F0FC:
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

// sub_8211F178 game::ModelSprite::Init: lifted from generated/ by lift.py.
lifted::Result f8211F178(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_8211F178 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // extsw r11,r5
  r11 = uint64_t(int64_t(int32_t(r5)));
  // lwz r10,0(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 0);
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lwz r11,12(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 12);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,104(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r3) + 104, temp.u32);
  // lwz r10,8(r4)
  r10 = guest::LoadU32(base, uint32_t(r4) + 8);
  // stw r10,208(r3)
  guest::StoreU32(base, uint32_t(r3) + 208, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8211F1B4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8211F1C8: lifted from generated/ by lift.py.
uint64_t f8211F1C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8211F1C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r31,r3,224
  r31 = uint64_t(int64_t(int64_t(r3) + 224));
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 96);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8211f25c
  if (cr6.eq) goto loc_8211F25C;
  // lwz r11,132(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 132);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addic. r11,r11,1
  ca = uint32_t(r11) > 4294967294;
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,132(r31)
  guest::StoreU32(base, uint32_t(r31) + 132, uint32_t(r11));
  // bge 0x8211f204
  if (!cr0.lt) goto loc_8211F204;
  // stw r30,132(r31)
  guest::StoreU32(base, uint32_t(r31) + 132, uint32_t(r30));
loc_8211F204:
  // bl 0x82293568
  // (left out: dead in C++)
  { const lifted::Result o = game::rand_82293568(base, LIFTED_ARGS(0x8211F208, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // srawi r10,r3,2
  ca = (int32_t(r3) < 0) & ((uint32_t(r3) & 0x3) != 0);
  r10 = uint64_t(int64_t(int32_t(r3) >> 2));
  // stw r11,128(r31)
  guest::StoreU32(base, uint32_t(r31) + 128, uint32_t(r11));
  // addze r11,r10
  temp.s64 = int64_t(r10) + ca;
  ca = temp.u32 < uint32_t(r10);
  r11 = uint64_t(int64_t(temp.s64));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r11,r11,r3
  r11 = r3 - r11;
  // stw r11,136(r31)
  guest::StoreU32(base, uint32_t(r31) + 136, uint32_t(r11));
  // bl 0x82293568
  // (left out: dead in C++)
  { const lifted::Result o = game::rand_82293568(base, LIFTED_ARGS(0x8211F228, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // divw r11,r3,r11
  r11 = uint32_t((int32_t(r11) && !(int32_t(r3) == INT32_MIN && int32_t(r11) == -1)) ? int32_t(r3) / int32_t(r11) : 0);
  // mulli r11,r11,7
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(7))));
  // subf. r11,r11,r3
  r11 = r3 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8211f240
  if (cr0.eq) goto loc_8211F240;
  // stw r30,128(r31)
  guest::StoreU32(base, uint32_t(r31) + 128, uint32_t(r30));
loc_8211F240:
  // bl 0x82293568
  // (left out: dead in C++)
  { const lifted::Result o = game::rand_82293568(base, LIFTED_ARGS(0x8211F244, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,22
  r11 = uint64_t(int64_t(22));
  // divw r11,r3,r11
  r11 = uint32_t((int32_t(r11) && !(int32_t(r3) == INT32_MIN && int32_t(r11) == -1)) ? int32_t(r3) / int32_t(r11) : 0);
  // mulli r11,r11,22
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(22))));
  // subf r11,r11,r3
  r11 = r3 - r11;
  // addi r11,r11,3
  r11 = uint64_t(int64_t(int64_t(r11) + 3));
  // stw r11,140(r31)
  guest::StoreU32(base, uint32_t(r31) + 140, uint32_t(r11));
loc_8211F25C:
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

// sub_82120868: lifted from generated/ by lift.py.
lifted::Result f82120868(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82120868 (lifted)");
  uint64_t r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a7904
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-448(r1)
  ea = -448 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // addi r31,r30,224
  r31 = uint64_t(int64_t(int64_t(r30) + 224));
  // lwz r11,128(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 128);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82120ba8
  if (cr6.eq) goto loc_82120BA8;
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 96);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82120ba8
  if (cr6.eq) goto loc_82120BA8;
  // lwz r10,400(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 400);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // mulli r10,r10,592
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(592))));
  // add r29,r10,r11
  r29 = r10 + r11;
  // bl 0x8211eb60
  // (left out: dead in C++)
  { const lifted::Result o = game::Sprite::Reset(base, LIFTED_ARGS(0x821208B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,140(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 140);
  // lwz r11,112(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 112);
  // addi r10,r10,27
  r10 = uint64_t(int64_t(int64_t(r10) + 27));
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r10,r30
  r10 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r30));
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 0);
  // stw r10,320(r1)
  guest::StoreU32(base, uint32_t(r1) + 320, uint32_t(r10));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r10));
  // beq cr6,0x82120910
  if (cr6.eq) goto loc_82120910;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r10,r11,28208
  r10 = uint64_t(int64_t(int64_t(r11) + 28208));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r9,0(r10)
  r9 = guest::LoadU64(base, uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r9,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r9);
  // std r10,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r10);
  // beq cr6,0x8212090c
  if (cr6.eq) goto loc_8212090C;
  // b 0x8212092c
  goto loc_8212092C;
loc_8212090C:
  // b 0x8212092c
  goto loc_8212092C;
loc_82120910:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r10,r11,28224
  r10 = uint64_t(int64_t(int64_t(r11) + 28224));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r9,0(r10)
  r9 = guest::LoadU64(base, uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r9,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r9);
  // std r10,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r10);
loc_8212092C:
  // lwz r11,136(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 136);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821209b0
  if (cr6.lt) goto loc_821209B0;
  // beq cr6,0x82120990
  if (cr6.eq) goto loc_82120990;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82120968
  if (cr6.lt) goto loc_82120968;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,304(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 304, temp.u32);
  // addi r11,r11,2608
  r11 = uint64_t(int64_t(int64_t(r11) + 2608));
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // stfs f12,312(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 312, temp.u32);
  // bne cr6,0x821209cc
  if (!cr6.eq) goto loc_821209CC;
  // b 0x82120984
  goto loc_82120984;
loc_82120968:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2608
  r11 = uint64_t(int64_t(int64_t(r11) + 2608));
  // lfs f12,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,304(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 304, temp.u32);
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,312(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 312, temp.u32);
loc_82120984:
  // stfs f12,308(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 308, temp.u32);
  // stfs f0,316(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 316, temp.u32);
  // b 0x821209d4
  goto loc_821209D4;
loc_82120990:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2608
  r11 = uint64_t(int64_t(int64_t(r11) + 2608));
  // lfs f12,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,304(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 304, temp.u32);
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,312(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 312, temp.u32);
  // b 0x821209cc
  goto loc_821209CC;
loc_821209B0:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,304(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 304, temp.u32);
  // addi r11,r11,2608
  r11 = uint64_t(int64_t(int64_t(r11) + 2608));
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // stfs f12,312(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 312, temp.u32);
loc_821209CC:
  // stfs f0,308(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 308, temp.u32);
  // stfs f12,316(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 316, temp.u32);
loc_821209D4:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // lfs f10,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f10 = double(temp.f32);
  // lwz r4,400(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 400);
  // lfs f9,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f9 = double(temp.f32);
  // mr r3,r30
  r3 = r30;
  // lfs f5,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f5 = double(temp.f32);
  // lfs f4,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f4 = double(temp.f32);
  // lfs f0,7140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f11,f13,f0
  f11 = double(float(f13 * f0));
  // lfs f3,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f3 = double(temp.f32);
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // lfs f2,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f2 = double(temp.f32);
  // fmuls f9,f9,f0
  f9 = double(float(f9 * f0));
  // lfs f1,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f1 = double(temp.f32);
  // fmuls f5,f5,f0
  f5 = double(float(f5 * f0));
  // lfs f31,48(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 48);
  f31 = double(temp.f32);
  // lfs f13,10708(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10708);
  f13 = double(temp.f32);
  // fmuls f4,f4,f0
  f4 = double(float(f4 * f0));
  // fmuls f3,f3,f0
  f3 = double(float(f3 * f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f2,f2,f0
  f2 = double(float(f2 * f0));
  // lfs f30,52(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 52);
  f30 = double(temp.f32);
  // fmuls f1,f1,f0
  f1 = double(float(f1 * f0));
  // lfs f28,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f28 = double(temp.f32);
  // lfs f27,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f27 = double(temp.f32);
  // fmr f25,f28
  f25 = f28;
  // lfs f26,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f26 = double(temp.f32);
  // fmr f24,f27
  f24 = f27;
  // fmuls f8,f11,f13
  f8 = double(float(f11 * f13));
  // fmuls f7,f10,f13
  f7 = double(float(f10 * f13));
  // fmuls f6,f9,f13
  f6 = double(float(f9 * f13));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f10,f10,f13
  f10 = double(float(f10 * f13));
  // fmuls f13,f9,f13
  f13 = double(float(f9 * f13));
  // lfs f9,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f9 = double(temp.f32);
  // fmuls f9,f9,f0
  f9 = double(float(f9 * f0));
  // lfs f0,52(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 52);
  f0 = double(temp.f32);
  // fmr f23,f26
  f23 = f26;
  // fsubs f8,f31,f8
  f8 = double(float(f31 - f8));
  // lfs f31,56(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 56);
  f31 = double(temp.f32);
  // fsubs f7,f0,f7
  f7 = double(float(f0 - f7));
  // lfs f0,48(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 48);
  f0 = double(temp.f32);
  // fsubs f6,f31,f6
  f6 = double(float(f31 - f6));
  // fadds f31,f0,f11
  f31 = double(float(f0 + f11));
  // lfs f0,56(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 56);
  f0 = double(temp.f32);
  // fadds f30,f30,f10
  f30 = double(float(f30 + f10));
  // fadds f29,f0,f13
  f29 = double(float(f0 + f13));
  // lfs f0,4208(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4208);
  f0 = double(temp.f32);
  // fmuls f11,f5,f0
  f11 = double(float(f5 * f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f9,f0
  f13 = double(float(f9 * f0));
  // fmuls f5,f2,f0
  f5 = double(float(f2 * f0));
  // fmuls f10,f4,f0
  f10 = double(float(f4 * f0));
  // fmuls f9,f3,f0
  f9 = double(float(f3 * f0));
  // fmuls f0,f1,f0
  f0 = double(float(f1 * f0));
  // fsubs f11,f7,f11
  f11 = double(float(f7 - f11));
  // stfs f11,180(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 180, temp.u32);
  // fsubs f13,f8,f13
  f13 = double(float(f8 - f13));
  // stfs f13,176(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 176, temp.u32);
  // fsubs f8,f30,f5
  f8 = double(float(f30 - f5));
  // fsubs f10,f6,f10
  f10 = double(float(f6 - f10));
  // stfs f10,184(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + 184, temp.u32);
  // fsubs f9,f31,f9
  f9 = double(float(f31 - f9));
  // fsubs f7,f29,f0
  f7 = double(float(f29 - f0));
  // lfs f0,10796(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10796);
  f0 = double(temp.f32);
  // fmuls f6,f28,f0
  f6 = double(float(f28 * f0));
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // fmuls f5,f27,f0
  f5 = double(float(f27 * f0));
  // fmuls f4,f26,f0
  f4 = double(float(f26 * f0));
  // fmuls f3,f25,f0
  f3 = double(float(f25 * f0));
  // fmuls f2,f24,f0
  f2 = double(float(f24 * f0));
  // fmuls f0,f23,f0
  f0 = double(float(f23 * f0));
  // stw r11,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r11));
  // fadds f6,f6,f13
  f6 = double(float(f6 + f13));
  // fadds f5,f5,f11
  f5 = double(float(f5 + f11));
  // fadds f4,f4,f10
  f4 = double(float(f4 + f10));
  // fadds f3,f3,f9
  f3 = double(float(f3 + f9));
  // fadds f2,f2,f8
  f2 = double(float(f2 + f8));
  // fadds f0,f0,f7
  f0 = double(float(f0 + f7));
  // lwz r10,84(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 84);
  // stfs f12,188(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 188, temp.u32);
  // lwz r9,88(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 88);
  // stfs f9,192(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + 192, temp.u32);
  // lwz r8,92(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 92);
  // stfs f8,196(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + 196, temp.u32);
  // stfs f7,200(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + 200, temp.u32);
  // stw r11,256(r1)
  guest::StoreU32(base, uint32_t(r1) + 256, uint32_t(r11));
  // stfs f12,204(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 204, temp.u32);
  // stw r11,272(r1)
  guest::StoreU32(base, uint32_t(r1) + 272, uint32_t(r11));
  // stfs f6,208(r1)
  temp.f32 = float(f6);
  guest::StoreU32(base, uint32_t(r1) + 208, temp.u32);
  // stw r10,244(r1)
  guest::StoreU32(base, uint32_t(r1) + 244, uint32_t(r10));
  // stfs f5,212(r1)
  temp.f32 = float(f5);
  guest::StoreU32(base, uint32_t(r1) + 212, temp.u32);
  // stw r9,248(r1)
  guest::StoreU32(base, uint32_t(r1) + 248, uint32_t(r9));
  // stfs f4,216(r1)
  temp.f32 = float(f4);
  guest::StoreU32(base, uint32_t(r1) + 216, temp.u32);
  // stw r8,252(r1)
  guest::StoreU32(base, uint32_t(r1) + 252, uint32_t(r8));
  // stfs f12,220(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 220, temp.u32);
  // stw r10,260(r1)
  guest::StoreU32(base, uint32_t(r1) + 260, uint32_t(r10));
  // stfs f3,224(r1)
  temp.f32 = float(f3);
  guest::StoreU32(base, uint32_t(r1) + 224, temp.u32);
  // stw r9,264(r1)
  guest::StoreU32(base, uint32_t(r1) + 264, uint32_t(r9));
  // stfs f2,228(r1)
  temp.f32 = float(f2);
  guest::StoreU32(base, uint32_t(r1) + 228, temp.u32);
  // stw r8,268(r1)
  guest::StoreU32(base, uint32_t(r1) + 268, uint32_t(r8));
  // stfs f0,232(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 232, temp.u32);
  // stw r10,276(r1)
  guest::StoreU32(base, uint32_t(r1) + 276, uint32_t(r10));
  // stfs f12,236(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 236, temp.u32);
  // stw r9,280(r1)
  guest::StoreU32(base, uint32_t(r1) + 280, uint32_t(r9));
  // stw r8,284(r1)
  guest::StoreU32(base, uint32_t(r1) + 284, uint32_t(r8));
  // stw r11,288(r1)
  guest::StoreU32(base, uint32_t(r1) + 288, uint32_t(r11));
  // stw r10,292(r1)
  guest::StoreU32(base, uint32_t(r1) + 292, uint32_t(r10));
  // stw r9,296(r1)
  guest::StoreU32(base, uint32_t(r1) + 296, uint32_t(r9));
  // stw r8,300(r1)
  guest::StoreU32(base, uint32_t(r1) + 300, uint32_t(r8));
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::LayerFor(base, LIFTED_ARGS(0x82120B98, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82129298
  // (left out: dead in C++)
  { const lifted::Result o = game::Sprite::Draw(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, 0x82120BA8); r3 = o.r3; f1 = o.f1; }
loc_82120BA8:
  // addi r1,r1,448
  r1 = uint64_t(int64_t(int64_t(r1) + 448));
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a7950
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82120BB8: lifted from generated/ by lift.py.
lifted::Result f82120BB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82120BB8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f14 = 0.0, f15 = 0.0, f16 = 0.0, f17 = 0.0, f18 = 0.0, f19 = 0.0, f20 = 0.0, f21 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
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
  // bl 0x825a78e0
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-544(r1)
  ea = -544 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // addi r31,r30,224
  r31 = uint64_t(int64_t(int64_t(r30) + 224));
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 96);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82121270
  if (cr6.eq) goto loc_82121270;
  // lwz r10,400(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 400);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // mulli r10,r10,592
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(592))));
  // add r27,r10,r11
  r27 = r10 + r11;
  // bl 0x8211eb60
  // (left out: dead in C++)
  { const lifted::Result o = game::Sprite::Reset(base, LIFTED_ARGS(0x82120BFC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,124(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 124);
  // lwz r11,120(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 120);
  // addi r10,r10,27
  r10 = uint64_t(int64_t(int64_t(r10) + 27));
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r10,r30
  r10 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r30));
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 0);
  // stw r10,320(r1)
  guest::StoreU32(base, uint32_t(r1) + 320, uint32_t(r10));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r10));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stfs f0,304(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 304, temp.u32);
  // stfs f0,308(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 308, temp.u32);
  // lfs f29,2608(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2608);
  f29 = double(temp.f32);
  // stfs f29,312(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 312, temp.u32);
  // stfs f29,316(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 316, temp.u32);
  // beq cr6,0x82120e0c
  if (cr6.eq) goto loc_82120E0C;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x82120e0c
  if (cr6.eq) goto loc_82120E0C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // lfs f11,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f11 = double(temp.f32);
  // lwz r4,400(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 400);
  // lfs f6,48(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 48);
  f6 = double(temp.f32);
  // mr r3,r30
  r3 = r30;
  // lfs f5,52(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 52);
  f5 = double(temp.f32);
  // lfs f4,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f4 = double(temp.f32);
  // lfs f0,7140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f12,f13,f0
  f12 = double(float(f13 * f0));
  // lfs f13,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f13 = double(temp.f32);
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // lfs f3,48(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 48);
  f3 = double(temp.f32);
  // fmuls f10,f13,f0
  f10 = double(float(f13 * f0));
  // lfs f2,52(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 52);
  f2 = double(temp.f32);
  // lfs f1,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f1 = double(temp.f32);
  // lfs f13,10152(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10152);
  f13 = double(temp.f32);
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // lfs f31,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f31 = double(temp.f32);
  // lfs f30,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f30 = double(temp.f32);
  // stfs f29,188(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 188, temp.u32);
  // stfs f29,204(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 204, temp.u32);
  // stw r11,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r11));
  // stfs f29,220(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 220, temp.u32);
  // fmuls f9,f12,f13
  f9 = double(float(f12 * f13));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // fmuls f8,f11,f13
  f8 = double(float(f11 * f13));
  // stfs f29,236(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 236, temp.u32);
  // fmuls f7,f10,f13
  f7 = double(float(f10 * f13));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f13,f10,f13
  f13 = double(float(f10 * f13));
  // lfs f10,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f10 = double(temp.f32);
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stw r11,244(r1)
  guest::StoreU32(base, uint32_t(r1) + 244, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // fsubs f9,f6,f9
  f9 = double(float(f6 - f9));
  // lfs f6,56(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 56);
  f6 = double(temp.f32);
  // fsubs f8,f5,f8
  f8 = double(float(f5 - f8));
  // lfs f5,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f5 = double(temp.f32);
  // fsubs f7,f6,f7
  f7 = double(float(f6 - f7));
  // stw r11,248(r1)
  guest::StoreU32(base, uint32_t(r1) + 248, uint32_t(r11));
  // fmuls f6,f5,f0
  f6 = double(float(f5 * f0));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // fmuls f5,f4,f0
  f5 = double(float(f4 * f0));
  // fadds f4,f12,f3
  f4 = double(float(f12 + f3));
  // lfs f12,56(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 56);
  f12 = double(temp.f32);
  // fadds f3,f2,f11
  f3 = double(float(f2 + f11));
  // fadds f2,f12,f13
  f2 = double(float(f12 + f13));
  // stw r11,252(r1)
  guest::StoreU32(base, uint32_t(r1) + 252, uint32_t(r11));
  // fsubs f13,f9,f10
  f13 = double(float(f9 - f10));
  // stfs f13,176(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 176, temp.u32);
  // fmuls f10,f1,f0
  f10 = double(float(f1 * f0));
  // fmuls f9,f31,f0
  f9 = double(float(f31 * f0));
  // fsubs f12,f8,f6
  f12 = double(float(f8 - f6));
  // stfs f12,180(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 180, temp.u32);
  // fmuls f8,f30,f0
  f8 = double(float(f30 * f0));
  // fsubs f11,f7,f5
  f11 = double(float(f7 - f5));
  // stfs f11,184(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 184, temp.u32);
  // fmr f6,f1
  f6 = f1;
  // fmr f5,f30
  f5 = f30;
  // fmr f7,f31
  f7 = f31;
  // fsubs f0,f4,f10
  f0 = double(float(f4 - f10));
  // stfs f0,192(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 192, temp.u32);
  // fsubs f10,f3,f9
  f10 = double(float(f3 - f9));
  // stfs f10,196(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + 196, temp.u32);
  // fsubs f9,f2,f8
  f9 = double(float(f2 - f8));
  // stfs f9,200(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + 200, temp.u32);
  // fadds f8,f13,f6
  f8 = double(float(f13 + f6));
  // stfs f8,208(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + 208, temp.u32);
  // fadds f6,f5,f11
  f6 = double(float(f5 + f11));
  // stfs f6,216(r1)
  temp.f32 = float(f6);
  guest::StoreU32(base, uint32_t(r1) + 216, temp.u32);
  // fadds f7,f7,f12
  f7 = double(float(f7 + f12));
  // stfs f7,212(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + 212, temp.u32);
  // fadds f5,f1,f0
  f5 = double(float(f1 + f0));
  // stfs f5,224(r1)
  temp.f32 = float(f5);
  guest::StoreU32(base, uint32_t(r1) + 224, temp.u32);
  // fadds f4,f31,f10
  f4 = double(float(f31 + f10));
  // stfs f4,228(r1)
  temp.f32 = float(f4);
  guest::StoreU32(base, uint32_t(r1) + 228, temp.u32);
  // fadds f3,f30,f9
  f3 = double(float(f30 + f9));
  // stfs f3,232(r1)
  temp.f32 = float(f3);
  guest::StoreU32(base, uint32_t(r1) + 232, temp.u32);
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,256(r1)
  guest::StoreU32(base, uint32_t(r1) + 256, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,260(r1)
  guest::StoreU32(base, uint32_t(r1) + 260, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,264(r1)
  guest::StoreU32(base, uint32_t(r1) + 264, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,268(r1)
  guest::StoreU32(base, uint32_t(r1) + 268, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,272(r1)
  guest::StoreU32(base, uint32_t(r1) + 272, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,276(r1)
  guest::StoreU32(base, uint32_t(r1) + 276, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,280(r1)
  guest::StoreU32(base, uint32_t(r1) + 280, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,284(r1)
  guest::StoreU32(base, uint32_t(r1) + 284, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,288(r1)
  guest::StoreU32(base, uint32_t(r1) + 288, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,292(r1)
  guest::StoreU32(base, uint32_t(r1) + 292, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,296(r1)
  guest::StoreU32(base, uint32_t(r1) + 296, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,300(r1)
  guest::StoreU32(base, uint32_t(r1) + 300, uint32_t(r11));
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::LayerFor(base, LIFTED_ARGS(0x82120DF8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82129298
  // (left out: dead in C++)
  { const lifted::Result o = game::Sprite::Draw(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, 0x82120E08); r3 = o.r3; f1 = o.f1; }
  // b 0x82121270
  goto loc_82121270;
loc_82120E0C:
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r29,r30,384
  r29 = uint64_t(int64_t(int64_t(r30) + 384));
  // addi r28,r31,48
  r28 = uint64_t(int64_t(int64_t(r31) + 48));
  // lfs f28,10584(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 10584);
  f28 = double(temp.f32);
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // lfs f31,10152(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 10152);
  f31 = double(temp.f32);
  // lfs f30,7140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f30 = double(temp.f32);
loc_82120E30:
  // extsw r11,r26
  r11 = uint64_t(int64_t(int32_t(r26)));
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // fmuls f13,f0,f30
  f13 = double(float(f0 * f30));
  // lfs f0,0(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 0);
  f0 = double(temp.f32);
  // fmuls f10,f0,f28
  f10 = double(float(f0 * f28));
  // lfs f0,8(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 8);
  f0 = double(temp.f32);
  // fmuls f8,f0,f28
  f8 = double(float(f0 * f28));
  // lfs f0,0(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 0);
  f0 = double(temp.f32);
  // fmuls f7,f0,f28
  f7 = double(float(f0 * f28));
  // lfs f0,8(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 8);
  f0 = double(temp.f32);
  // std r11,336(r1)
  guest::StoreU64(base, uint32_t(r1) + 336, r11);
  // fmuls f5,f0,f28
  f5 = double(float(f0 * f28));
  // lfs f16,8(r28)
  temp.u32 = guest::LoadU32(base, uint32_t(r28) + 8);
  f16 = double(temp.f32);
  // lwz r4,400(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 400);
  // fmr f4,f16
  f4 = f16;
  // lfs f25,0(r28)
  temp.u32 = guest::LoadU32(base, uint32_t(r28) + 0);
  f25 = double(temp.f32);
  // lfs f11,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f11 = double(temp.f32);
  // fmr f15,f25
  f15 = f25;
  // stfs f4,80(r1)
  temp.f32 = float(f4);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // fmuls f11,f11,f30
  f11 = double(float(f11 * f30));
  // lfs f12,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // mr r3,r30
  r3 = r30;
  // fmuls f4,f13,f31
  f4 = double(float(f13 * f31));
  // lfs f6,4(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 4);
  f6 = double(temp.f32);
  // fmuls f24,f13,f31
  f24 = double(float(f13 * f31));
  // lfs f17,4(r28)
  temp.u32 = guest::LoadU32(base, uint32_t(r28) + 4);
  f17 = double(temp.f32);
  // fmuls f12,f12,f30
  f12 = double(float(f12 * f30));
  // lfs f9,4(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 4);
  f9 = double(temp.f32);
  // fmuls f6,f6,f28
  f6 = double(float(f6 * f28));
  // lfs f1,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f1 = double(temp.f32);
  // fmr f14,f17
  f14 = f17;
  // lfs f27,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f27 = double(temp.f32);
  // fmuls f9,f9,f28
  f9 = double(float(f9 * f28));
  // lfs f26,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f26 = double(temp.f32);
  // fmr f19,f1
  f19 = f1;
  // fmr f18,f27
  f18 = f27;
  // fmuls f2,f11,f31
  f2 = double(float(f11 * f31));
  // fmuls f22,f11,f31
  f22 = double(float(f11 * f31));
  // fmuls f1,f1,f30
  f1 = double(float(f1 * f30));
  // fmuls f27,f27,f30
  f27 = double(float(f27 * f30));
  // fmuls f3,f12,f31
  f3 = double(float(f12 * f31));
  // fmuls f23,f12,f31
  f23 = double(float(f12 * f31));
  // fmuls f26,f26,f30
  f26 = double(float(f26 * f30));
  // fmuls f19,f19,f30
  f19 = double(float(f19 * f30));
  // fmuls f18,f18,f30
  f18 = double(float(f18 * f30));
  // fmuls f1,f1,f31
  f1 = double(float(f1 * f31));
  // fmuls f21,f27,f31
  f21 = double(float(f27 * f31));
  // fmuls f20,f26,f31
  f20 = double(float(f26 * f31));
  // lfd f0,336(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 336)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f13,f7,f0
  f13 = double(float(f7 * f0));
  // fmuls f11,f5,f0
  f11 = double(float(f5 * f0));
  // fmuls f12,f6,f0
  f12 = double(float(f6 * f0));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f9,f9,f0
  f9 = double(float(f9 * f0));
  // fmuls f0,f8,f0
  f0 = double(float(f8 * f0));
  // fsubs f13,f15,f13
  f13 = double(float(f15 - f13));
  // fmr f15,f16
  f15 = f16;
  // fsubs f12,f14,f12
  f12 = double(float(f14 - f12));
  // fadds f27,f25,f10
  f27 = double(float(f25 + f10));
  // fadds f26,f9,f17
  f26 = double(float(f9 + f17));
  // fadds f25,f0,f16
  f25 = double(float(f0 + f16));
  // fsubs f8,f13,f4
  f8 = double(float(f13 - f4));
  // fsubs f11,f15,f11
  f11 = double(float(f15 - f11));
  // fsubs f7,f12,f3
  f7 = double(float(f12 - f3));
  // fadds f10,f24,f13
  f10 = double(float(f24 + f13));
  // fadds f9,f23,f12
  f9 = double(float(f23 + f12));
  // fsubs f0,f8,f1
  f0 = double(float(f8 - f1));
  // fsubs f6,f11,f2
  f6 = double(float(f11 - f2));
  // fadds f5,f22,f11
  f5 = double(float(f22 + f11));
  // lfs f11,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f11 = double(temp.f32);
  // fmuls f11,f11,f30
  f11 = double(float(f11 * f30));
  // fsubs f13,f7,f21
  f13 = double(float(f7 - f21));
  // fmuls f8,f19,f31
  f8 = double(float(f19 * f31));
  // fmuls f7,f18,f31
  f7 = double(float(f18 * f31));
  // fsubs f12,f6,f20
  f12 = double(float(f6 - f20));
  // fmuls f6,f11,f31
  f6 = double(float(f11 * f31));
  // fsubs f11,f10,f8
  f11 = double(float(f10 - f8));
  // fsubs f10,f9,f7
  f10 = double(float(f9 - f7));
  // fsubs f9,f5,f6
  f9 = double(float(f5 - f6));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // lfs f8,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f8 = double(temp.f32);
  // lfs f7,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f7 = double(temp.f32);
  // fmuls f8,f8,f31
  f8 = double(float(f8 * f31));
  // lfs f6,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f6 = double(temp.f32);
  // fmuls f7,f7,f31
  f7 = double(float(f7 * f31));
  // lfs f5,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f5 = double(temp.f32);
  // fmuls f6,f6,f31
  f6 = double(float(f6 * f31));
  // lfs f4,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f4 = double(temp.f32);
  // fmuls f5,f5,f31
  f5 = double(float(f5 * f31));
  // stw r11,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r11));
  // lfs f3,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f3 = double(temp.f32);
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // fmuls f4,f4,f31
  f4 = double(float(f4 * f31));
  // fmuls f3,f3,f31
  f3 = double(float(f3 * f31));
  // stfs f0,176(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 176, temp.u32);
  // stfs f13,180(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 180, temp.u32);
  // stfs f12,184(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 184, temp.u32);
  // stfs f11,192(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 192, temp.u32);
  // stw r11,244(r1)
  guest::StoreU32(base, uint32_t(r1) + 244, uint32_t(r11));
  // stfs f10,196(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + 196, temp.u32);
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stfs f9,200(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + 200, temp.u32);
  // fadds f0,f8,f0
  f0 = double(float(f8 + f0));
  // stfs f29,188(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 188, temp.u32);
  // fadds f13,f7,f13
  f13 = double(float(f7 + f13));
  // stfs f29,204(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 204, temp.u32);
  // fadds f12,f6,f12
  f12 = double(float(f6 + f12));
  // stfs f29,220(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 220, temp.u32);
  // fadds f11,f5,f11
  f11 = double(float(f5 + f11));
  // stfs f29,236(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 236, temp.u32);
  // stw r11,248(r1)
  guest::StoreU32(base, uint32_t(r1) + 248, uint32_t(r11));
  // fadds f10,f4,f10
  f10 = double(float(f4 + f10));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // fadds f9,f3,f9
  f9 = double(float(f3 + f9));
  // stfs f0,208(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 208, temp.u32);
  // stfs f13,212(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 212, temp.u32);
  // stfs f12,216(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 216, temp.u32);
  // stfs f11,224(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 224, temp.u32);
  // stw r11,252(r1)
  guest::StoreU32(base, uint32_t(r1) + 252, uint32_t(r11));
  // stfs f10,228(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + 228, temp.u32);
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stfs f9,232(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + 232, temp.u32);
  // stw r11,256(r1)
  guest::StoreU32(base, uint32_t(r1) + 256, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,260(r1)
  guest::StoreU32(base, uint32_t(r1) + 260, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,264(r1)
  guest::StoreU32(base, uint32_t(r1) + 264, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,268(r1)
  guest::StoreU32(base, uint32_t(r1) + 268, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,272(r1)
  guest::StoreU32(base, uint32_t(r1) + 272, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,276(r1)
  guest::StoreU32(base, uint32_t(r1) + 276, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,280(r1)
  guest::StoreU32(base, uint32_t(r1) + 280, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,284(r1)
  guest::StoreU32(base, uint32_t(r1) + 284, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,288(r1)
  guest::StoreU32(base, uint32_t(r1) + 288, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,292(r1)
  guest::StoreU32(base, uint32_t(r1) + 292, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,296(r1)
  guest::StoreU32(base, uint32_t(r1) + 296, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,300(r1)
  guest::StoreU32(base, uint32_t(r1) + 300, uint32_t(r11));
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::LayerFor(base, LIFTED_ARGS(0x82121080, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82129298
  // (left out: dead in C++)
  { const lifted::Result o = game::Sprite::Draw(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, 0x82121090); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // beq cr6,0x82121264
  if (cr6.eq) goto loc_82121264;
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // lfs f13,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // fmuls f13,f13,f30
  f13 = double(float(f13 * f30));
  // lfs f12,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f12 = double(temp.f32);
  // lfs f8,20(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 20);
  f8 = double(temp.f32);
  // fmuls f12,f12,f30
  f12 = double(float(f12 * f30));
  // lfs f11,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f11 = double(temp.f32);
  // fmr f23,f8
  f23 = f8;
  // lfs f6,24(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 24);
  f6 = double(temp.f32);
  // fmr f4,f8
  f4 = f8;
  // fmuls f11,f11,f30
  f11 = double(float(f11 * f30));
  // lfs f5,16(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 16);
  f5 = double(temp.f32);
  // fmuls f8,f8,f30
  f8 = double(float(f8 * f30));
  // stw r11,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r11));
  // fmr f3,f6
  f3 = f6;
  // lwz r4,400(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 400);
  // fmr f22,f6
  f22 = f6;
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // fmr f19,f6
  f19 = f6;
  // stfs f29,188(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 188, temp.u32);
  // fmuls f10,f0,f31
  f10 = double(float(f0 * f31));
  // stfs f29,204(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 204, temp.u32);
  // fmuls f9,f13,f31
  f9 = double(float(f13 * f31));
  // stfs f29,220(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 220, temp.u32);
  // fmuls f7,f12,f31
  f7 = double(float(f12 * f31));
  // stfs f29,236(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 236, temp.u32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // mr r3,r30
  r3 = r30;
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // fmr f24,f5
  f24 = f5;
  // fmr f21,f5
  f21 = f5;
  // fmuls f6,f6,f30
  f6 = double(float(f6 * f30));
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // fsubs f10,f27,f10
  f10 = double(float(f27 - f10));
  // fsubs f9,f26,f9
  f9 = double(float(f26 - f9));
  // fmuls f8,f8,f31
  f8 = double(float(f8 * f31));
  // fmuls f5,f5,f30
  f5 = double(float(f5 * f30));
  // fmuls f4,f4,f30
  f4 = double(float(f4 * f30));
  // fmuls f3,f3,f30
  f3 = double(float(f3 * f30));
  // fadds f2,f0,f27
  f2 = double(float(f0 + f27));
  // fadds f27,f12,f25
  f27 = double(float(f12 + f25));
  // fadds f1,f13,f26
  f1 = double(float(f13 + f26));
  // fsubs f7,f25,f7
  f7 = double(float(f25 - f7));
  // fsubs f0,f10,f11
  f0 = double(float(f10 - f11));
  // stfs f0,176(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 176, temp.u32);
  // fmuls f12,f6,f31
  f12 = double(float(f6 * f31));
  // fsubs f13,f9,f8
  f13 = double(float(f9 - f8));
  // stfs f13,180(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 180, temp.u32);
  // fmuls f11,f5,f31
  f11 = double(float(f5 * f31));
  // fmuls f10,f4,f31
  f10 = double(float(f4 * f31));
  // fmuls f9,f3,f31
  f9 = double(float(f3 * f31));
  // fmr f20,f23
  f20 = f23;
  // fmuls f8,f24,f31
  f8 = double(float(f24 * f31));
  // fmuls f6,f23,f31
  f6 = double(float(f23 * f31));
  // fmuls f5,f21,f31
  f5 = double(float(f21 * f31));
  // fmuls f3,f19,f31
  f3 = double(float(f19 * f31));
  // fsubs f12,f7,f12
  f12 = double(float(f7 - f12));
  // stfs f12,184(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 184, temp.u32);
  // fmuls f7,f22,f31
  f7 = double(float(f22 * f31));
  // fsubs f11,f2,f11
  f11 = double(float(f2 - f11));
  // stfs f11,192(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 192, temp.u32);
  // fsubs f10,f1,f10
  f10 = double(float(f1 - f10));
  // stfs f10,196(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + 196, temp.u32);
  // fsubs f9,f27,f9
  f9 = double(float(f27 - f9));
  // stfs f9,200(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + 200, temp.u32);
  // fmuls f4,f20,f31
  f4 = double(float(f20 * f31));
  // fadds f8,f8,f0
  f8 = double(float(f8 + f0));
  // stfs f8,208(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + 208, temp.u32);
  // fadds f6,f6,f13
  f6 = double(float(f6 + f13));
  // stfs f6,212(r1)
  temp.f32 = float(f6);
  guest::StoreU32(base, uint32_t(r1) + 212, temp.u32);
  // fadds f7,f7,f12
  f7 = double(float(f7 + f12));
  // stfs f7,216(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + 216, temp.u32);
  // fadds f5,f5,f11
  f5 = double(float(f5 + f11));
  // stfs f5,224(r1)
  temp.f32 = float(f5);
  guest::StoreU32(base, uint32_t(r1) + 224, temp.u32);
  // fadds f3,f3,f9
  f3 = double(float(f3 + f9));
  // stfs f3,232(r1)
  temp.f32 = float(f3);
  guest::StoreU32(base, uint32_t(r1) + 232, temp.u32);
  // fadds f4,f4,f10
  f4 = double(float(f4 + f10));
  // stfs f4,228(r1)
  temp.f32 = float(f4);
  guest::StoreU32(base, uint32_t(r1) + 228, temp.u32);
  // stw r11,244(r1)
  guest::StoreU32(base, uint32_t(r1) + 244, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,248(r1)
  guest::StoreU32(base, uint32_t(r1) + 248, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,252(r1)
  guest::StoreU32(base, uint32_t(r1) + 252, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,256(r1)
  guest::StoreU32(base, uint32_t(r1) + 256, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,260(r1)
  guest::StoreU32(base, uint32_t(r1) + 260, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,264(r1)
  guest::StoreU32(base, uint32_t(r1) + 264, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,268(r1)
  guest::StoreU32(base, uint32_t(r1) + 268, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,272(r1)
  guest::StoreU32(base, uint32_t(r1) + 272, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,276(r1)
  guest::StoreU32(base, uint32_t(r1) + 276, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,280(r1)
  guest::StoreU32(base, uint32_t(r1) + 280, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,284(r1)
  guest::StoreU32(base, uint32_t(r1) + 284, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,288(r1)
  guest::StoreU32(base, uint32_t(r1) + 288, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,292(r1)
  guest::StoreU32(base, uint32_t(r1) + 292, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,296(r1)
  guest::StoreU32(base, uint32_t(r1) + 296, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,300(r1)
  guest::StoreU32(base, uint32_t(r1) + 300, uint32_t(r11));
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::LayerFor(base, LIFTED_ARGS(0x82121254, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82129298
  // (left out: dead in C++)
  { const lifted::Result o = game::Sprite::Draw(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, 0x82121264); r3 = o.r3; f1 = o.f1; }
loc_82121264:
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // cmpwi cr6,r26,3
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 3, so);
  // blt cr6,0x82120e30
  if (cr6.lt) goto loc_82120E30;
loc_82121270:
  // addi r1,r1,544
  r1 = uint64_t(int64_t(int64_t(r1) + 544));
  // addi r12,r1,-56
  r12 = uint64_t(int64_t(int64_t(r1) + -56));
  // bl 0x825a792c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82121280: lifted from generated/ by lift.py.
lifted::Result f82121280(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82121280 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-88(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -88, std::bit_cast<uint64_t>(f29));
  // stfd f30,-80(r1)
  guest::StoreU64(base, uint32_t(r1) + -80, std::bit_cast<uint64_t>(f30));
  // stfd f31,-72(r1)
  guest::StoreU64(base, uint32_t(r1) + -72, std::bit_cast<uint64_t>(f31));
  // stwu r1,-416(r1)
  ea = -416 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // addi r31,r29,224
  r31 = uint64_t(int64_t(int64_t(r29) + 224));
  // lwz r11,96(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 96);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8212154c
  if (cr6.eq) goto loc_8212154C;
  // lwz r10,400(r29)
  r10 = guest::LoadU32(base, uint32_t(r29) + 400);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // mulli r10,r10,592
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(592))));
  // add r26,r10,r11
  r26 = r10 + r11;
  // bl 0x8211eb60
  // (left out: dead in C++)
  { const lifted::Result o = game::Sprite::Reset(base, LIFTED_ARGS(0x821212C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // li r25,2
  r25 = uint64_t(int64_t(2));
  // lfs f11,48(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 48);
  f11 = double(temp.f32);
  // mr r3,r29
  r3 = r29;
  // fmr f8,f11
  f8 = f11;
  // lfs f13,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f13 = double(temp.f32);
  // lfs f10,56(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 56);
  f10 = double(temp.f32);
  // lfs f9,52(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 52);
  f9 = double(temp.f32);
  // fmr f7,f10
  f7 = f10;
  // lfs f30,7140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f30 = double(temp.f32);
  // lwz r11,124(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 124);
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // lfs f12,100(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 100);
  f12 = double(temp.f32);
  // addi r11,r11,27
  r11 = uint64_t(int64_t(int64_t(r11) + 27));
  // fmuls f13,f13,f30
  f13 = double(float(f13 * f30));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r29
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r29));
  // fsubs f11,f11,f0
  f11 = double(float(f11 - f0));
  // fadds f0,f0,f8
  f0 = double(float(f0 + f8));
  // fmr f8,f9
  f8 = f9;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fadds f13,f7,f13
  f13 = double(float(f7 + f13));
  // stfs f11,160(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 160, temp.u32);
  // fsubs f7,f9,f12
  f7 = double(float(f9 - f12));
  // stfs f9,164(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + 164, temp.u32);
  // stfs f10,168(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + 168, temp.u32);
  // stw r25,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r25));
  // stfs f0,176(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 176, temp.u32);
  // stw r11,304(r1)
  guest::StoreU32(base, uint32_t(r1) + 304, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f8,180(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + 180, temp.u32);
  // stfs f13,184(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 184, temp.u32);
  // stfs f11,192(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 192, temp.u32);
  // fsubs f12,f8,f12
  f12 = double(float(f8 - f12));
  // stfs f7,196(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + 196, temp.u32);
  // lfs f29,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f29 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f29,288(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 288, temp.u32);
  // stfs f29,292(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 292, temp.u32);
  // stfs f10,200(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + 200, temp.u32);
  // stfs f0,208(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 208, temp.u32);
  // lfs f31,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stfs f31,296(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 296, temp.u32);
  // stfs f31,300(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 300, temp.u32);
  // stfs f31,172(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 172, temp.u32);
  // stfs f31,188(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 188, temp.u32);
  // stw r11,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r11));
  // stfs f31,204(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 204, temp.u32);
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stfs f31,220(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 220, temp.u32);
  // stfs f12,212(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 212, temp.u32);
  // stfs f13,216(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 216, temp.u32);
  // stw r11,228(r1)
  guest::StoreU32(base, uint32_t(r1) + 228, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,232(r1)
  guest::StoreU32(base, uint32_t(r1) + 232, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,236(r1)
  guest::StoreU32(base, uint32_t(r1) + 236, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,244(r1)
  guest::StoreU32(base, uint32_t(r1) + 244, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,248(r1)
  guest::StoreU32(base, uint32_t(r1) + 248, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,252(r1)
  guest::StoreU32(base, uint32_t(r1) + 252, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,256(r1)
  guest::StoreU32(base, uint32_t(r1) + 256, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,260(r1)
  guest::StoreU32(base, uint32_t(r1) + 260, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,264(r1)
  guest::StoreU32(base, uint32_t(r1) + 264, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // lwz r4,400(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 400);
  // stw r11,268(r1)
  guest::StoreU32(base, uint32_t(r1) + 268, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,272(r1)
  guest::StoreU32(base, uint32_t(r1) + 272, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // stw r11,276(r1)
  guest::StoreU32(base, uint32_t(r1) + 276, uint32_t(r11));
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stw r11,280(r1)
  guest::StoreU32(base, uint32_t(r1) + 280, uint32_t(r11));
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // stw r11,284(r1)
  guest::StoreU32(base, uint32_t(r1) + 284, uint32_t(r11));
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::LayerFor(base, LIFTED_ARGS(0x82121420, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r26
  r4 = r26;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82129298
  // (left out: dead in C++)
  { const lifted::Result o = game::Sprite::Draw(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, 0x82121430); r3 = o.r3; f1 = o.f1; }
  // li r30,255
  r30 = uint64_t(int64_t(255));
  // addi r27,r29,424
  r27 = uint64_t(int64_t(int64_t(r29) + 424));
  // mr r28,r30
  r28 = r30;
loc_8212143C:
  // lwz r11,124(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 124);
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // lfs f12,8(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 8);
  f12 = double(temp.f32);
  // fmuls f13,f0,f30
  f13 = double(float(f0 * f30));
  // addi r11,r11,27
  r11 = uint64_t(int64_t(int64_t(r11) + 27));
  // fmuls f12,f12,f30
  f12 = double(float(f12 * f30));
  // lfs f11,-8(r27)
  temp.u32 = guest::LoadU32(base, uint32_t(r27) + -8);
  f11 = double(temp.f32);
  // mr r3,r29
  r3 = r29;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lfs f10,0(r27)
  temp.u32 = guest::LoadU32(base, uint32_t(r27) + 0);
  f10 = double(temp.f32);
  // lfs f0,-4(r27)
  temp.u32 = guest::LoadU32(base, uint32_t(r27) + -4);
  f0 = double(temp.f32);
  // lwz r4,400(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 400);
  // lfs f9,100(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 100);
  f9 = double(temp.f32);
  // lwzx r11,r11,r29
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r29));
  // fsubs f8,f11,f13
  f8 = double(float(f11 - f13));
  // fsubs f7,f10,f12
  f7 = double(float(f10 - f12));
  // fadds f13,f11,f13
  f13 = double(float(f11 + f13));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // fadds f12,f10,f12
  f12 = double(float(f10 + f12));
  // fsubs f11,f0,f9
  f11 = double(float(f0 - f9));
  // stfs f29,288(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 288, temp.u32);
  // fsubs f10,f0,f9
  f10 = double(float(f0 - f9));
  // stfs f31,296(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 296, temp.u32);
  // stfs f29,292(r1)
  temp.f32 = float(f29);
  guest::StoreU32(base, uint32_t(r1) + 292, temp.u32);
  // stw r25,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r25));
  // stfs f31,300(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 300, temp.u32);
  // stw r30,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r30));
  // stfs f31,172(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 172, temp.u32);
  // stw r11,304(r1)
  guest::StoreU32(base, uint32_t(r1) + 304, uint32_t(r11));
  // stfs f31,188(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 188, temp.u32);
  // stw r30,228(r1)
  guest::StoreU32(base, uint32_t(r1) + 228, uint32_t(r30));
  // stfs f31,204(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 204, temp.u32);
  // stw r30,232(r1)
  guest::StoreU32(base, uint32_t(r1) + 232, uint32_t(r30));
  // stfs f31,220(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 220, temp.u32);
  // stw r28,236(r1)
  guest::StoreU32(base, uint32_t(r1) + 236, uint32_t(r28));
  // stfs f8,160(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + 160, temp.u32);
  // stw r30,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r30));
  // stfs f0,164(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 164, temp.u32);
  // stw r30,244(r1)
  guest::StoreU32(base, uint32_t(r1) + 244, uint32_t(r30));
  // stfs f7,168(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + 168, temp.u32);
  // stw r30,248(r1)
  guest::StoreU32(base, uint32_t(r1) + 248, uint32_t(r30));
  // stfs f13,176(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 176, temp.u32);
  // stw r28,252(r1)
  guest::StoreU32(base, uint32_t(r1) + 252, uint32_t(r28));
  // stfs f0,180(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 180, temp.u32);
  // stw r30,256(r1)
  guest::StoreU32(base, uint32_t(r1) + 256, uint32_t(r30));
  // stfs f12,184(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 184, temp.u32);
  // stw r30,260(r1)
  guest::StoreU32(base, uint32_t(r1) + 260, uint32_t(r30));
  // stfs f8,192(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + 192, temp.u32);
  // stw r30,264(r1)
  guest::StoreU32(base, uint32_t(r1) + 264, uint32_t(r30));
  // stfs f11,196(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + 196, temp.u32);
  // stw r28,268(r1)
  guest::StoreU32(base, uint32_t(r1) + 268, uint32_t(r28));
  // stfs f7,200(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + 200, temp.u32);
  // stw r30,272(r1)
  guest::StoreU32(base, uint32_t(r1) + 272, uint32_t(r30));
  // stfs f13,208(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 208, temp.u32);
  // stw r30,276(r1)
  guest::StoreU32(base, uint32_t(r1) + 276, uint32_t(r30));
  // stfs f10,212(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + 212, temp.u32);
  // stw r30,280(r1)
  guest::StoreU32(base, uint32_t(r1) + 280, uint32_t(r30));
  // stfs f12,216(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 216, temp.u32);
  // stw r28,284(r1)
  guest::StoreU32(base, uint32_t(r1) + 284, uint32_t(r28));
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::LayerFor(base, LIFTED_ARGS(0x8212152C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r26
  r4 = r26;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82129298
  // (left out: dead in C++)
  { const lifted::Result o = game::Sprite::Draw(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, 0x8212153C); r3 = o.r3; f1 = o.f1; }
  // addi r28,r28,-30
  r28 = uint64_t(int64_t(int64_t(r28) + -30));
  // addi r27,r27,16
  r27 = uint64_t(int64_t(int64_t(r27) + 16));
  // cmpwi cr6,r28,-15
  cr6 = lifted::Compare<int32_t>(int32_t(r28), -15, so);
  // bgt cr6,0x8212143c
  if (cr6.gt) goto loc_8212143C;
loc_8212154C:
  // addi r1,r1,416
  r1 = uint64_t(int64_t(int64_t(r1) + 416));
  // lfd f29,-88(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -88)));
  // lfd f30,-80(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -80)));
  // lfd f31,-72(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -72)));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82121590: lifted from generated/ by lift.py.
void f82121590(uint8_t* base, uint64_t r3, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82121590 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmpwi cr6,r5,394
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 394, so);
  // bgt cr6,0x82121690
  if (cr6.gt) goto loc_82121690;
  // beq cr6,0x82121674
  if (cr6.eq) goto loc_82121674;
  // cmpwi cr6,r5,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r5), -1, so);
  // beq cr6,0x82121658
  if (cr6.eq) goto loc_82121658;
  // cmpwi cr6,r5,357
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 357, so);
  // beq cr6,0x8212163c
  if (cr6.eq) goto loc_8212163C;
  // cmpwi cr6,r5,382
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 382, so);
  // beq cr6,0x82121620
  if (cr6.eq) goto loc_82121620;
  // cmpwi cr6,r5,391
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 391, so);
  // beq cr6,0x82121604
  if (cr6.eq) goto loc_82121604;
  // cmpwi cr6,r5,392
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 392, so);
  // beq cr6,0x821215e8
  if (cr6.eq) goto loc_821215E8;
  // cmpwi cr6,r5,393
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 393, so);
  // bne cr6,0x821216b8
  if (!cr6.eq) goto loc_821216B8;
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28144
  r11 = uint64_t(int64_t(int64_t(r11) + 28144));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_821215E8:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28192
  r11 = uint64_t(int64_t(int64_t(r11) + 28192));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_82121604:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28096
  r11 = uint64_t(int64_t(int64_t(r11) + 28096));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_82121620:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28176
  r11 = uint64_t(int64_t(int64_t(r11) + 28176));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_8212163C:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28176
  r11 = uint64_t(int64_t(int64_t(r11) + 28176));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_82121658:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,23840
  r11 = uint64_t(int64_t(int64_t(r11) + 23840));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_82121674:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28080
  r11 = uint64_t(int64_t(int64_t(r11) + 28080));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_82121690:
  // cmpwi cr6,r5,395
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 395, so);
  // beq cr6,0x82121744
  if (cr6.eq) goto loc_82121744;
  // cmpwi cr6,r5,396
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 396, so);
  // beq cr6,0x82121728
  if (cr6.eq) goto loc_82121728;
  // cmpwi cr6,r5,397
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 397, so);
  // beq cr6,0x8212170c
  if (cr6.eq) goto loc_8212170C;
  // cmpwi cr6,r5,398
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 398, so);
  // beq cr6,0x821216f0
  if (cr6.eq) goto loc_821216F0;
  // cmpwi cr6,r5,399
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 399, so);
  // beq cr6,0x821216d4
  if (cr6.eq) goto loc_821216D4;
loc_821216B8:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,23840
  r11 = uint64_t(int64_t(int64_t(r11) + 23840));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_821216D4:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28144
  r11 = uint64_t(int64_t(int64_t(r11) + 28144));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_821216F0:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28128
  r11 = uint64_t(int64_t(int64_t(r11) + 28128));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_8212170C:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28144
  r11 = uint64_t(int64_t(int64_t(r11) + 28144));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_82121728:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28096
  r11 = uint64_t(int64_t(int64_t(r11) + 28096));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
loc_82121744:
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28160
  r11 = uint64_t(int64_t(int64_t(r11) + 28160));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // std r10,0(r3)
  guest::StoreU64(base, uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r11,8(r3)
  guest::StoreU64(base, uint32_t(r3) + 8, r11);
  // blr
  return;
}

// sub_82121CC0 game::ModelSprite::Update: lifted from generated/ by lift.py.
lifted::Result f82121CC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82121CC0 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,336(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 336);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bgt cr6,0x82121d64
  if (cr6.gt) goto loc_82121D64;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-31552
  r12 = uint64_t(int64_t(int64_t(r12) + -31552));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // lis r12,-32238
  r12 = uint64_t(int64_t(-2112749568));
  // addi r12,r12,7436
  r12 = uint64_t(int64_t(int64_t(r12) + 7436));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_82121D0C;
  return {r3, f1};
  case 1:
  goto loc_82121D18;
  case 2:
  goto loc_82121D2C;
  case 3:
  goto loc_82121D24;
  case 4:
  goto loc_82121D38;
  case 5:
  goto loc_82121D44;
  case 6:
  goto loc_82121D50;
  case 7:
  goto loc_82121D5C;
  default:
  std::abort();
  }
loc_82121D18:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211f6f8
  // (left out: dead in C++)
  { const lifted::Result o = f8211F6F8(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82121d64
  goto loc_82121D64;
loc_82121D24:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211f530
  // (left out: dead in C++)
  { const lifted::Result o = f8211F530(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
loc_82121D2C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211f1c8
  // (left out: dead in C++)
  r3 = f8211F1C8(base, r1, r13, r3);
  // b 0x82121d64
  goto loc_82121D64;
loc_82121D38:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211f278
  // (left out: dead in C++)
  f8211F278(base, r3);
  // b 0x82121d2c
  goto loc_82121D2C;
loc_82121D44:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211f278
  // (left out: dead in C++)
  f8211F278(base, r3);
  // b 0x82121d64
  goto loc_82121D64;
loc_82121D50:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211f8c0
  // (left out: dead in C++)
  f1 = f8211F8C0(base, r1, r13, r3, f1);
  // b 0x82121d64
  goto loc_82121D64;
loc_82121D5C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ff20
  // (left out: dead in C++)
  f8211FF20(base, r1, r3);
loc_82121D64:
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
loc_frag_82121D0C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211f530
  // (left out: dead in C++)
  { const lifted::Result o = f8211F530(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82121d64
  goto loc_82121D64_82121D0C;
loc_82121D64_82121D0C:
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

// sub_82122900 game::ModelSprite::Dtor: lifted from generated/ by lift.py.
lifted::Result f82122900(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82122900 (lifted)");
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
  // addi r11,r11,-31512
  r11 = uint64_t(int64_t(int64_t(r11) + -31512));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82126498
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::Ctor(base, LIFTED_ARGS(0x8212292C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8212293c
  if (cr0.eq) goto loc_8212293C;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8212293C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8212293C:
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

// sub_82122958 game::CarOverlays::Record: lifted from generated/ by lift.py.
lifted::Result f82122958(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82122958 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r3
  r29 = r3;
  // lwz r4,128(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 128);
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x82122970, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82122db0
  if (cr6.eq) goto loc_82122DB0;
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82122db0
  if (cr6.eq) goto loc_82122DB0;
  // addi r31,r29,164
  r31 = uint64_t(int64_t(int64_t(r29) + 164));
  // li r30,4
  r30 = uint64_t(int64_t(4));
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
loc_82122994:
  // lwz r11,11496(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 11496);
  // lwz r3,-16(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -16);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // beq cr6,0x821229c4
  if (cr6.eq) goto loc_821229C4;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821229BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x821229e8
  goto loc_821229E8;
loc_821229C4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821229D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,784(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 784);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821229e4
  if (cr6.eq) goto loc_821229E4;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821229fc
  if (!cr6.eq) goto loc_821229FC;
loc_821229E4:
  // li r4,3
  r4 = uint64_t(int64_t(3));
loc_821229E8:
  // lwz r3,-16(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -16);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821229FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_821229FC:
  // lwz r11,11496(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 11496);
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // beq cr6,0x82122a2c
  if (cr6.eq) goto loc_82122A2C;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122A24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x82122a50
  goto loc_82122A50;
loc_82122A2C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122A38, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,784(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 784);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122a4c
  if (cr6.eq) goto loc_82122A4C;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82122a64
  if (!cr6.eq) goto loc_82122A64;
loc_82122A4C:
  // li r4,3
  r4 = uint64_t(int64_t(3));
loc_82122A50:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122A64, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122A64:
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82122994
  if (!cr0.eq) goto loc_82122994;
  // addi r31,r29,360
  r31 = uint64_t(int64_t(int64_t(r29) + 360));
  // li r30,4
  r30 = uint64_t(int64_t(4));
loc_82122A78:
  // lwz r11,11496(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 11496);
  // lwz r3,-160(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -160);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // beq cr6,0x82122aa8
  if (cr6.eq) goto loc_82122AA8;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122AA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x82122acc
  goto loc_82122ACC;
loc_82122AA8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122AB4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,784(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 784);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122ac8
  if (cr6.eq) goto loc_82122AC8;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82122ae0
  if (!cr6.eq) goto loc_82122AE0;
loc_82122AC8:
  // li r4,3
  r4 = uint64_t(int64_t(3));
loc_82122ACC:
  // lwz r3,-160(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -160);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122AE0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122AE0:
  // lwz r11,11496(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 11496);
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // beq cr6,0x82122b10
  if (cr6.eq) goto loc_82122B10;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122B08, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x82122b34
  goto loc_82122B34;
loc_82122B10:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122B1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,784(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 784);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122b30
  if (cr6.eq) goto loc_82122B30;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82122b48
  if (!cr6.eq) goto loc_82122B48;
loc_82122B30:
  // li r4,3
  r4 = uint64_t(int64_t(3));
loc_82122B34:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122B48, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122B48:
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82122a78
  if (!cr0.eq) goto loc_82122A78;
  // addi r31,r29,328
  r31 = uint64_t(int64_t(int64_t(r29) + 328));
  // li r30,8
  r30 = uint64_t(int64_t(8));
loc_82122B5C:
  // lwz r11,11496(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 11496);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82122ba4
  if (cr6.eq) goto loc_82122BA4;
  // lwz r11,128(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 128);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82122b90
  if (cr6.eq) goto loc_82122B90;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122B90, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122B90:
  // lwz r11,128(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 128);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122c04
  if (cr6.eq) goto loc_82122C04;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x82122bf0
  goto loc_82122BF0;
loc_82122BA4:
  // lwz r11,812(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 812);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82122bd8
  if (cr6.eq) goto loc_82122BD8;
  // lwz r11,788(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 788);
  // lwz r10,784(r29)
  r10 = guest::LoadU32(base, uint32_t(r29) + 784);
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // bne cr6,0x82122bd8
  if (!cr6.eq) goto loc_82122BD8;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122BD8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122BD8:
  // lwz r11,784(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 784);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122bec
  if (cr6.eq) goto loc_82122BEC;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82122c04
  if (!cr6.eq) goto loc_82122C04;
loc_82122BEC:
  // li r4,3
  r4 = uint64_t(int64_t(3));
loc_82122BF0:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122C04, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122C04:
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82122b5c
  if (!cr0.eq) goto loc_82122B5C;
  // addi r31,r29,396
  r31 = uint64_t(int64_t(int64_t(r29) + 396));
  // li r30,9
  r30 = uint64_t(int64_t(9));
loc_82122C18:
  // lwz r11,11496(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 11496);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,128(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 128);
  // beq cr6,0x82122c60
  if (cr6.eq) goto loc_82122C60;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82122c4c
  if (cr6.eq) goto loc_82122C4C;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122C4C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122C4C:
  // lwz r11,128(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 128);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122cc8
  if (cr6.eq) goto loc_82122CC8;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x82122cb4
  goto loc_82122CB4;
loc_82122C60:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82122c9c
  if (cr6.eq) goto loc_82122C9C;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122C80, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,784(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 784);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122c94
  if (cr6.eq) goto loc_82122C94;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82122cc8
  if (!cr6.eq) goto loc_82122CC8;
loc_82122C94:
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // b 0x82122cb4
  goto loc_82122CB4;
loc_82122C9C:
  // lwz r11,784(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 784);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122cc8
  if (cr6.eq) goto loc_82122CC8;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82122cc8
  if (cr6.eq) goto loc_82122CC8;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_82122CB4:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122CC8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122CC8:
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82122c18
  if (!cr0.eq) goto loc_82122C18;
  // lwz r11,816(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 816);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82122d6c
  if (cr6.eq) goto loc_82122D6C;
  // lwz r11,11496(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 11496);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82122d28
  if (cr6.eq) goto loc_82122D28;
  // lwz r11,128(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 128);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82122d14
  if (cr6.eq) goto loc_82122D14;
  // lwz r3,376(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 376);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122D14, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122D14:
  // lwz r11,128(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 128);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122d6c
  if (cr6.eq) goto loc_82122D6C;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x82122d58
  goto loc_82122D58;
loc_82122D28:
  // lwz r3,376(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 376);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122D40, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,784(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 784);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82122d54
  if (cr6.eq) goto loc_82122D54;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82122d6c
  if (!cr6.eq) goto loc_82122D6C;
loc_82122D54:
  // li r4,3
  r4 = uint64_t(int64_t(3));
loc_82122D58:
  // lwz r3,376(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 376);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122D6C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122D6C:
  // addi r31,r29,380
  r31 = uint64_t(int64_t(int64_t(r29) + 380));
  // li r30,4
  r30 = uint64_t(int64_t(4));
loc_82122D74:
  // lwz r11,11496(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 11496);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82122d9c
  if (!cr6.eq) goto loc_82122D9C;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82122D9C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82122D9C:
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82122d74
  if (!cr0.eq) goto loc_82122D74;
  // lwz r11,784(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 784);
  // stw r11,788(r29)
  guest::StoreU32(base, uint32_t(r29) + 788, uint32_t(r11));
loc_82122DB0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82122DB8: lifted from generated/ by lift.py.
uint64_t f82122DB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82122DB8 (lifted)");
  uint64_t r4 = 0, r10 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r4,128(r3)
  r4 = guest::LoadU32(base, uint32_t(r3) + 128);
  // mr r30,r5
  r30 = r5;
  // mr r29,r6
  r29 = r6;
  // mr r28,r7
  r28 = r7;
  // mr r27,r8
  r27 = r8;
  // mr r26,r9
  r26 = r9;
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x82122DE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r31,r3
  r31 = r3;
  // bl 0x82127198
  // (left out: dead in C++)
  r3 = f82127198(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82122dfc
  if (cr0.eq) goto loc_82122DFC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82122ea0
  goto loc_82122EA0;
loc_82122DFC:
  // cmpwi cr6,r27,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r27), -1, so);
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x82122e20
  if (!cr6.eq) goto loc_82122E20;
  // bl 0x82126bb0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveFlag199FA(base, LIFTED_ARGS(0x82122E10, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82122e9c
  if (cr0.eq) goto loc_82122E9C;
  // mr r4,r28
  r4 = r28;
  // b 0x82122e88
  goto loc_82122E88;
loc_82122E20:
  // cmpwi cr6,r26,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r26), -1, so);
  // bne cr6,0x82122e50
  if (!cr6.eq) goto loc_82122E50;
  // bl 0x82126bb0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveFlag199FA(base, LIFTED_ARGS(0x82122E2C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82122e9c
  if (cr0.eq) goto loc_82122E9C;
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x82126bb0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveFlag199FA(base, LIFTED_ARGS(0x82122E40, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82122e9c
  if (cr0.eq) goto loc_82122E9C;
  // mr r4,r27
  r4 = r27;
  // b 0x82122e88
  goto loc_82122E88;
loc_82122E50:
  // bl 0x82126bb0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveFlag199FA(base, LIFTED_ARGS(0x82122E54, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82122e9c
  if (cr0.eq) goto loc_82122E9C;
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x82126bb0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveFlag199FA(base, LIFTED_ARGS(0x82122E68, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82122e9c
  if (cr0.eq) goto loc_82122E9C;
  // mr r4,r27
  r4 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x82126bb0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveFlag199FA(base, LIFTED_ARGS(0x82122E7C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82122e9c
  if (cr0.eq) goto loc_82122E9C;
  // mr r4,r26
  r4 = r26;
loc_82122E88:
  // mr r3,r31
  r3 = r31;
  // bl 0x82126bb0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveFlag199FA(base, LIFTED_ARGS(0x82122E90, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bne 0x82122ea0
  if (!cr0.eq) goto loc_82122EA0;
loc_82122E9C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_82122EA0:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_82123350 game::CarOverlays::Ctor: lifted from generated/ by lift.py.
lifted::Result f82123350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82123350 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x82126498
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::Ctor(base, LIFTED_ARGS(0x82123370, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r11,r11,-31452
  r11 = uint64_t(int64_t(int64_t(r11) + -31452));
  // mr r3,r31
  r3 = r31;
  // lfs f31,2608(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2608);
  f31 = double(temp.f32);
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // stw r30,144(r31)
  guest::StoreU32(base, uint32_t(r31) + 144, uint32_t(r30));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stfs f31,748(r31)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r31) + 748, temp.u32);
  // stfs f31,764(r31)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r31) + 764, temp.u32);
  // stfs f31,780(r31)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r31) + 780, temp.u32);
  // stw r10,788(r31)
  guest::StoreU32(base, uint32_t(r31) + 788, uint32_t(r10));
  // bl 0x82126530
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::ClearViews(base, LIFTED_ARGS(0x821233A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f31,780(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r31) + 780, temp.u32);
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,768(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 768, temp.u32);
  // stfs f0,772(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 772, temp.u32);
  // stfs f0,776(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 776, temp.u32);
  // stw r30,108(r31)
  guest::StoreU32(base, uint32_t(r31) + 108, uint32_t(r30));
  // stw r30,104(r31)
  guest::StoreU32(base, uint32_t(r31) + 104, uint32_t(r30));
  // stw r30,112(r31)
  guest::StoreU32(base, uint32_t(r31) + 112, uint32_t(r30));
  // stw r30,720(r31)
  guest::StoreU32(base, uint32_t(r31) + 720, uint32_t(r30));
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821233E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821233F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-21600(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + -21600);
  // mr r30,r3
  r30 = r3;
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,32(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82123414, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82123424, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x82128190
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlays::SetRenderTargets(base, LIFTED_ARGS(0x8212342C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
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

// sub_82123450 game::CarOverlays::CreateChildren: lifted from generated/ by lift.py.
lifted::Result f82123450(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82123450 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r27,r4
  r27 = r4;
  // mr r31,r3
  r31 = r3;
  // li r29,4
  r29 = uint64_t(int64_t(4));
  // addi r30,r31,148
  r30 = uint64_t(int64_t(int64_t(r31) + 148));
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r11,r11,28260
  r11 = uint64_t(int64_t(int64_t(r11) + 28260));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 4);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lis r11,128
  r11 = uint64_t(int64_t(8388608));
  // ori r25,r11,4096
  r25 = r11 | 4096;
loc_82123490:
  // lis r4,0
  r4 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // mr r5,r25
  r5 = r25;
  // ori r4,r4,64160
  r4 = r4 | 64160;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x821234A8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821234b8
  if (cr0.eq) goto loc_821234B8;
  // bl 0x8212a140
  // (left out: dead in C++)
  { const lifted::Result o = game::Trail::Ctor(base, LIFTED_ARGS(0x821234B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_821234B8:
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r5,500
  r5 = uint64_t(int64_t(500));
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821234D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 104);
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r4,r11,36
  r4 = uint64_t(int64_t(int64_t(r11) + 36));
  // bl 0x8212d6d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::SetTextures108(base, LIFTED_ARGS(0x821234E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x82123490
  if (!cr0.eq) goto loc_82123490;
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // addi r11,r11,28280
  r11 = uint64_t(int64_t(int64_t(r11) + 28280));
  // addi r30,r31,164
  r30 = uint64_t(int64_t(int64_t(r31) + 164));
  // li r29,4
  r29 = uint64_t(int64_t(4));
  // stw r26,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r26));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_8212350C:
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // mr r5,r25
  r5 = r25;
  // li r4,32640
  r4 = uint64_t(int64_t(32640));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x82123520, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82123530
  if (cr0.eq) goto loc_82123530;
  // bl 0x8212b2e8
  // (left out: dead in C++)
  { const lifted::Result o = game::Smoke::Ctor(base, LIFTED_ARGS(0x82123530, r31)); r3 = o.r3; f1 = o.f1; }
loc_82123530:
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r5,45
  r5 = uint64_t(int64_t(45));
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212354C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 104);
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r4,r11,40
  r4 = uint64_t(int64_t(int64_t(r11) + 40));
  // bl 0x8212d6d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::SetTextures108(base, LIFTED_ARGS(0x8212355C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x8212350c
  if (!cr0.eq) goto loc_8212350C;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r29,r31,200
  r29 = uint64_t(int64_t(int64_t(r31) + 200));
  // addi r11,r11,28296
  r11 = uint64_t(int64_t(int64_t(r11) + 28296));
  // li r28,4
  r28 = uint64_t(int64_t(4));
  // stw r30,180(r31)
  guest::StoreU32(base, uint32_t(r31) + 180, uint32_t(r30));
  // stw r30,184(r31)
  guest::StoreU32(base, uint32_t(r31) + 184, uint32_t(r30));
  // stw r30,188(r31)
  guest::StoreU32(base, uint32_t(r31) + 188, uint32_t(r30));
  // stw r30,192(r31)
  guest::StoreU32(base, uint32_t(r31) + 192, uint32_t(r30));
  // stw r30,196(r31)
  guest::StoreU32(base, uint32_t(r31) + 196, uint32_t(r30));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 4);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_821235A4:
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // mr r5,r25
  r5 = r25;
  // li r4,288
  r4 = uint64_t(int64_t(288));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x821235B8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821235c8
  if (cr0.eq) goto loc_821235C8;
  // bl 0x8211f060
  // (left out: dead in C++)
  { const lifted::Result o = game::LightSprite::Ctor(base, LIFTED_ARGS(0x821235C8, r31)); r3 = o.r3; f1 = o.f1; }
loc_821235C8:
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // li r5,1
  r5 = uint64_t(int64_t(1));
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821235E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,104(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 104);
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // bl 0x8211e9c8
  // (left out: dead in C++)
  { const lifted::Result o = game::LightSprite::SetTextures(base, LIFTED_ARGS(0x821235F0, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne 0x821235a4
  if (!cr0.eq) goto loc_821235A4;
  // addi r11,r31,272
  r11 = uint64_t(int64_t(int64_t(r31) + 272));
  // li r10,14
  r10 = uint64_t(int64_t(14));
loc_82123604:
  // stw r30,-56(r11)
  guest::StoreU32(base, uint32_t(r11) + -56, uint32_t(r30));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x82123604
  if (!cr0.eq) goto loc_82123604;
  // lwz r10,0(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r30,r31,328
  r30 = uint64_t(int64_t(int64_t(r31) + 328));
  // addi r11,r11,28312
  r11 = uint64_t(int64_t(int64_t(r11) + 28312));
  // li r29,8
  r29 = uint64_t(int64_t(8));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lwz r10,4(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 4);
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
loc_8212363C:
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // mr r5,r25
  r5 = r25;
  // li r4,29824
  r4 = uint64_t(int64_t(29824));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x82123650, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82123660
  if (cr0.eq) goto loc_82123660;
  // bl 0x8212e9a0
  // (left out: dead in C++)
  { const lifted::Result o = game::Sparks::Ctor(base, LIFTED_ARGS(0x82123660, r31)); r3 = o.r3; f1 = o.f1; }
loc_82123660:
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r5,200
  r5 = uint64_t(int64_t(200));
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212367C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 104);
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r4,r11,44
  r4 = uint64_t(int64_t(int64_t(r11) + 44));
  // bl 0x8212d6d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::SetTextures108(base, LIFTED_ARGS(0x8212368C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x8212363c
  if (!cr0.eq) goto loc_8212363C;
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // stw r26,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r26));
  // addi r30,r31,360
  r30 = uint64_t(int64_t(int64_t(r31) + 360));
  // addi r11,r11,28328
  r11 = uint64_t(int64_t(int64_t(r11) + 28328));
  // li r29,4
  r29 = uint64_t(int64_t(4));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_821236B0:
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // mr r5,r25
  r5 = r25;
  // li r4,2816
  r4 = uint64_t(int64_t(2816));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x821236C4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821236d4
  if (cr0.eq) goto loc_821236D4;
  // bl 0x8212bfe8
  // (left out: dead in C++)
  { const lifted::Result o = game::ParticlesB::Ctor(base, LIFTED_ARGS(0x821236D4, r31)); r3 = o.r3; f1 = o.f1; }
loc_821236D4:
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r5,15
  r5 = uint64_t(int64_t(15));
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821236F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 104);
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r4,r11,40
  r4 = uint64_t(int64_t(int64_t(r11) + 40));
  // bl 0x8212d6d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::SetTextures108(base, LIFTED_ARGS(0x82123700, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x821236b0
  if (!cr0.eq) goto loc_821236B0;
  // lwz r10,0(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r30,r31,396
  r30 = uint64_t(int64_t(int64_t(r31) + 396));
  // addi r11,r11,28392
  r11 = uint64_t(int64_t(int64_t(r11) + 28392));
  // li r29,9
  r29 = uint64_t(int64_t(9));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lwz r10,4(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 4);
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
loc_82123730:
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // mr r5,r25
  r5 = r25;
  // li r4,576
  r4 = uint64_t(int64_t(576));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x82123744, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82123754
  if (cr0.eq) goto loc_82123754;
  // bl 0x82122880
  // (left out: dead in C++)
  { const lifted::Result o = game::ModelSprite::Ctor(base, LIFTED_ARGS(0x82123754, r31)); r3 = o.r3; f1 = o.f1; }
loc_82123754:
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r5,1
  r5 = uint64_t(int64_t(1));
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82123770, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 104);
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r4,r11,48
  r4 = uint64_t(int64_t(int64_t(r11) + 48));
  // bl 0x8211f408
  // (left out: dead in C++)
  { const lifted::Result o = game::ModelSprite::SetTextures(base, LIFTED_ARGS(0x82123780, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x82123730
  if (!cr0.eq) goto loc_82123730;
  // lwz r10,0(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // addi r11,r11,28360
  r11 = uint64_t(int64_t(int64_t(r11) + 28360));
  // mr r5,r25
  r5 = r25;
  // li r4,304
  r4 = uint64_t(int64_t(304));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r10,4(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 4);
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x821237BC, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,376(r31)
  guest::StoreU32(base, uint32_t(r31) + 376, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821237cc
  if (cr0.eq) goto loc_821237CC;
  // bl 0x8212d620
  // (left out: dead in C++)
  { const lifted::Result o = game::Marker::Ctor(base, LIFTED_ARGS(0x821237CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_821237CC:
  // lwz r3,376(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 376);
  // li r5,1
  r5 = uint64_t(int64_t(1));
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821237E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 104);
  // lwz r3,376(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 376);
  // addi r4,r11,148
  r4 = uint64_t(int64_t(int64_t(r11) + 148));
  // bl 0x8212cc10
  // (left out: dead in C++)
  { const lifted::Result o = game::Marker::SetTextures(base, LIFTED_ARGS(0x821237F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // addi r30,r31,380
  r30 = uint64_t(int64_t(int64_t(r31) + 380));
  // addi r11,r11,28376
  r11 = uint64_t(int64_t(int64_t(r11) + 28376));
  // li r29,4
  r29 = uint64_t(int64_t(4));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lwz r10,4(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 4);
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
loc_8212381C:
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // mr r5,r25
  r5 = r25;
  // li r4,2720
  r4 = uint64_t(int64_t(2720));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x82123830, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82123840
  if (cr0.eq) goto loc_82123840;
  // bl 0x8212caf8
  // (left out: dead in C++)
  { const lifted::Result o = game::ParticlesC::Ctor(base, LIFTED_ARGS(0x82123840, r31)); r3 = o.r3; f1 = o.f1; }
loc_82123840:
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r5,20
  r5 = uint64_t(int64_t(20));
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212385C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 104);
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r4,r11,164
  r4 = uint64_t(int64_t(int64_t(r11) + 164));
  // bl 0x8212c0e0
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::SetTextures104(base, LIFTED_ARGS(0x8212386C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x8212381c
  if (!cr0.eq) goto loc_8212381C;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82123880 game::CarOverlays::Reset: lifted from generated/ by lift.py.
lifted::Result f82123880(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82123880 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // li r29,4
  r29 = uint64_t(int64_t(4));
  // addi r30,r31,164
  r30 = uint64_t(int64_t(int64_t(r31) + 164));
loc_82123898:
  // lwz r3,-16(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + -16);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821238AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821238C0, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x82123898
  if (!cr0.eq) goto loc_82123898;
  // addi r30,r31,360
  r30 = uint64_t(int64_t(int64_t(r31) + 360));
  // li r29,4
  r29 = uint64_t(int64_t(4));
loc_821238D4:
  // lwz r3,-160(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + -160);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821238E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821238FC, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x821238d4
  if (!cr0.eq) goto loc_821238D4;
  // addi r29,r31,328
  r29 = uint64_t(int64_t(int64_t(r31) + 328));
  // li r30,8
  r30 = uint64_t(int64_t(8));
loc_82123910:
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82123924, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne 0x82123910
  if (!cr0.eq) goto loc_82123910;
  // addi r29,r31,396
  r29 = uint64_t(int64_t(int64_t(r31) + 396));
  // li r30,9
  r30 = uint64_t(int64_t(9));
loc_82123938:
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212394C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne 0x82123938
  if (!cr0.eq) goto loc_82123938;
  // lwz r3,376(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 376);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212396C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r31,380
  r29 = uint64_t(int64_t(int64_t(r31) + 380));
  // li r30,4
  r30 = uint64_t(int64_t(4));
loc_82123974:
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82123988, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne 0x82123974
  if (!cr0.eq) goto loc_82123974;
  // addi r11,r31,436
  r11 = uint64_t(int64_t(int64_t(r31) + 436));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_821239A0:
  // stw r9,-4(r11)
  guest::StoreU32(base, uint32_t(r11) + -4, uint32_t(r9));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // bne 0x821239a0
  if (!cr0.eq) goto loc_821239A0;
  // addi r11,r31,480
  r11 = uint64_t(int64_t(int64_t(r31) + 480));
  // li r10,60
  r10 = uint64_t(int64_t(60));
  // mtctr r10
  ctr = r10;
loc_821239C4:
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x821239c4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_821239C4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r10,788(r31)
  guest::StoreU32(base, uint32_t(r31) + 788, uint32_t(r10));
  // stfs f0,768(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 768, temp.u32);
  // stfs f0,772(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 772, temp.u32);
  // stfs f0,776(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 776, temp.u32);
  // lfs f13,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // stfs f13,780(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 780, temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821253F0 game::CarOverlays::FindModel: lifted from generated/ by lift.py.
lifted::Result f821253F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_821253F0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-416(r1)
  ea = -416 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // lwz r4,128(r27)
  r4 = guest::LoadU32(base, uint32_t(r27) + 128);
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x82125408, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r11,50
  r11 = uint64_t(int64_t(50));
  // mtctr r11
  ctr = r11;
loc_82125420:
  // stw r9,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r9));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x82125420
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82125420;
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r3,11844(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x82125444, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8212548c
  if (cr0.eq) goto loc_8212548C;
  // addi r28,r1,80
  r28 = uint64_t(int64_t(int64_t(r1) + 80));
loc_82125450:
  // mr r5,r30
  r5 = r30;
  // lwz r3,11844(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x82125460, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,11844(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r3,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r3));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r11
  r3 = r11;
  // mr r5,r30
  r5 = r30;
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x82125484, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82125450
  if (!cr0.eq) goto loc_82125450;
loc_8212548C:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,11844(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x821254A0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821254f0
  if (cr0.eq) goto loc_821254F0;
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r28,r11,r10
  r28 = r11 + r10;
loc_821254B4:
  // mr r5,r30
  r5 = r30;
  // lwz r3,11844(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x821254C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,11844(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r3,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r11
  r3 = r11;
  // mr r5,r30
  r5 = r30;
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x821254E8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x821254b4
  if (!cr0.eq) goto loc_821254B4;
loc_821254F0:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,11844(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x82125504, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82125550
  if (cr0.eq) goto loc_82125550;
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r29,r11,r10
  r29 = r11 + r10;
loc_82125518:
  // mr r5,r30
  r5 = r30;
  // lwz r3,11844(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x82125528, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,11844(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r3,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r3));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r11
  r3 = r11;
  // mr r5,r30
  r5 = r30;
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x82125548, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82125518
  if (!cr0.eq) goto loc_82125518;
loc_82125550:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x821255cc
  if (cr6.eq) goto loc_821255CC;
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
loc_82125564:
  // mr r4,r26
  r4 = r26;
  // addi r3,r1,288
  r3 = uint64_t(int64_t(int64_t(r1) + 288));
  // bl 0x82126ff0
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::BodyMatrixCopy(base, LIFTED_ARGS(0x82125570, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 0);
  // mr r11,r3
  r11 = r3;
  // addi r10,r10,4304
  r10 = uint64_t(int64_t(int64_t(r10) + 4304));
  // addi r9,r11,64
  r9 = uint64_t(int64_t(int64_t(r11) + 64));
loc_82125580:
  // lbz r8,0(r11)
  r8 = guest::LoadU8(base, uint32_t(r11) + 0);
  // lbz r7,0(r10)
  r7 = guest::LoadU8(base, uint32_t(r10) + 0);
  // subf. r8,r7,r8
  r8 = r8 - r7;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x821255a0
  if (!cr0.eq) goto loc_821255A0;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // bne cr6,0x82125580
  if (!cr6.eq) goto loc_82125580;
loc_821255A0:
  // addi r11,r8,0
  r11 = uint64_t(int64_t(int64_t(r8) + 0));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821255e4
  if (!cr0.eq) goto loc_821255E4;
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // add r30,r11,r10
  r30 = r11 + r10;
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82125564
  if (!cr6.eq) goto loc_82125564;
loc_821255CC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,11844(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 11844);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x821255DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x821255ec
  goto loc_821255EC;
loc_821255E4:
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
loc_821255EC:
  // stw r11,8(r26)
  guest::StoreU32(base, uint32_t(r26) + 8, uint32_t(r11));
  // mr r3,r27
  r3 = r27;
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82125604, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,416
  r1 = uint64_t(int64_t(int64_t(r1) + 416));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82125610 game::CarOverlays::OnTexturesLoaded: lifted from generated/ by lift.py.
lifted::Result f82125610(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82125610 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r3,24
  r3 = uint64_t(int64_t(24));
  // mr r28,r4
  r28 = r4;
  // mr r31,r6
  r31 = r6;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8212562C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8212563c
  if (cr0.eq) goto loc_8212563C;
  // bl 0x82596e68
  // (left out: dead in C++)
  { const lifted::Result o = game::XPR::Ctor(base, LIFTED_ARGS(0x82125638, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82125640
  goto loc_82125640;
loc_8212563C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82125640:
  // mr r4,r28
  r4 = r28;
  // stw r3,108(r31)
  guest::StoreU32(base, uint32_t(r31) + 108, uint32_t(r3));
  // bl 0x82596f68
  // (left out: dead in C++)
  { const lifted::Result o = game::XPR::Create(base, LIFTED_ARGS(0x8212564C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r5,128
  r5 = uint64_t(int64_t(8388608));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x82125664, r31)); r3 = o.r3; f1 = o.f1; }
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // stw r3,104(r31)
  guest::StoreU32(base, uint32_t(r31) + 104, uint32_t(r3));
loc_82125670:
  // mr r4,r29
  r4 = r29;
  // lwz r3,108(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 108);
  // bl 0x82597118
  // (left out: dead in C++)
  { const lifted::Result o = game::XPR::GetTexture(base, LIFTED_ARGS(0x8212567C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 104);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // stwx r3,r11,r30
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r30), uint32_t(r3));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r30,168
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 168, so);
  // blt cr6,0x82125670
  if (cr6.lt) goto loc_82125670;
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // mr r3,r28
  r3 = r28;
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x821256A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,112(r31)
  guest::StoreU32(base, uint32_t(r31) + 112, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821256B8 game::CarOverlays::Init: lifted from generated/ by lift.py.
lifted::Result f821256B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_821256B8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,28496
  r11 = uint64_t(int64_t(int64_t(r11) + 28496));
  // mr r30,r4
  r30 = r4;
  // addi r4,r31,796
  r4 = uint64_t(int64_t(int64_t(r31) + 796));
  // stw r11,144(r31)
  guest::StoreU32(base, uint32_t(r31) + 144, uint32_t(r11));
  // lwz r11,12(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 12);
  // stw r11,128(r31)
  guest::StoreU32(base, uint32_t(r31) + 128, uint32_t(r11));
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 16);
  // stw r11,132(r31)
  guest::StoreU32(base, uint32_t(r31) + 132, uint32_t(r11));
  // lwz r11,20(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 20);
  // stw r11,136(r31)
  guest::StoreU32(base, uint32_t(r31) + 136, uint32_t(r11));
  // lwz r11,24(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 24);
  // stw r11,140(r31)
  guest::StoreU32(base, uint32_t(r31) + 140, uint32_t(r11));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // rotlwi r5,r11,0
  r5 = __builtin_rotateleft32(uint32_t(r11), 0);
  // stw r11,116(r31)
  guest::StoreU32(base, uint32_t(r31) + 116, uint32_t(r11));
  // lwz r6,4(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 4);
  // stw r6,120(r31)
  guest::StoreU32(base, uint32_t(r31) + 120, uint32_t(r6));
  // bl 0x821264c0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::BindViewport(base, LIFTED_ARGS(0x8212571C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82123450
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::CreateChildren(base, r1, r13, r3, r4, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212573C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // lwz r4,128(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 128);
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x82125748, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,4(r3)
  r9 = guest::LoadU32(base, uint32_t(r3) + 4);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,832(r31)
  guest::StoreU32(base, uint32_t(r31) + 832, uint32_t(r9));
  // stw r11,820(r31)
  guest::StoreU32(base, uint32_t(r31) + 820, uint32_t(r11));
  // stw r10,836(r31)
  guest::StoreU32(base, uint32_t(r31) + 836, uint32_t(r10));
  // stw r11,824(r31)
  guest::StoreU32(base, uint32_t(r31) + 824, uint32_t(r11));
  // stw r11,828(r31)
  guest::StoreU32(base, uint32_t(r31) + 828, uint32_t(r11));
  // lwz r11,8(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8212577c
  if (!cr6.eq) goto loc_8212577C;
  // mr r3,r31
  r3 = r31;
  // bl 0x821253f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::FindModel(base, r1, r13, r3, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
loc_8212577C:
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

// sub_82125888 game::CarOverlays::LoadTextures: lifted from generated/ by lift.py.
lifted::Result f82125888(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82125888 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = game::Archive::AssetSize(base, LIFTED_ARGS(0x821258AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,4096
  r5 = uint64_t(int64_t(4096));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x821258C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32238
  r11 = uint64_t(int64_t(-2112749568));
  // mr r4,r3
  r4 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r30
  r6 = r30;
  // addi r5,r11,22032
  r5 = uint64_t(int64_t(int64_t(r11) + 22032));
  // mr r3,r31
  r3 = r31;
  // bl 0x8209de48
  // (left out: dead in C++)
  { const lifted::Result o = game::Archive::Load(base, LIFTED_ARGS(0x821258E0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821258F8 game::CarOverlays::ReleaseTextures: lifted from generated/ by lift.py.
lifted::Result f821258F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821258F8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r30,r31,108
  r30 = uint64_t(int64_t(int64_t(r31) + 108));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82125944
  if (cr6.eq) goto loc_82125944;
  // lwz r3,104(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 104);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8212593c
  if (cr0.eq) goto loc_8212593C;
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x8212593C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8212593C:
  // mr r3,r30
  r3 = r30;
  // bl 0x82108860
  // (left out: dead in C++)
  { const lifted::Result o = game::XPR::Release_82108860(base, LIFTED_ARGS(0x82125944, r31)); r3 = o.r3; f1 = o.f1; }
loc_82125944:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,104(r31)
  guest::StoreU32(base, uint32_t(r31) + 104, uint32_t(r11));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
  // stw r11,112(r31)
  guest::StoreU32(base, uint32_t(r31) + 112, uint32_t(r11));
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

// sub_82125970 game::CarOverlays::DeleteChildren: lifted from generated/ by lift.py.
lifted::Result f82125970(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82125970 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r27,r3
  r27 = r3;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // addi r31,r27,164
  r31 = uint64_t(int64_t(int64_t(r27) + 164));
  // li r28,4
  r28 = uint64_t(int64_t(4));
  // ori r29,r11,32768
  r29 = r11 | 32768;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_82125994:
  // lwz r3,-16(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -16);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821259AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,-16(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -16);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x821259BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // stw r30,-16(r31)
  guest::StoreU32(base, uint32_t(r31) + -16, uint32_t(r30));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821259D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x821259E8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r30));
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82125994
  if (!cr0.eq) goto loc_82125994;
  // addi r31,r27,360
  r31 = uint64_t(int64_t(int64_t(r27) + 360));
  // li r28,4
  r28 = uint64_t(int64_t(4));
loc_82125A00:
  // lwz r3,-160(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -160);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82125A18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,-160(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -160);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x82125A28, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // stw r30,-160(r31)
  guest::StoreU32(base, uint32_t(r31) + -160, uint32_t(r30));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82125A44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x82125A54, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r30));
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82125a00
  if (!cr0.eq) goto loc_82125A00;
  // addi r31,r27,328
  r31 = uint64_t(int64_t(int64_t(r27) + 328));
  // li r28,8
  r28 = uint64_t(int64_t(8));
loc_82125A6C:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82125A84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x82125A94, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r30));
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82125a6c
  if (!cr0.eq) goto loc_82125A6C;
  // addi r31,r27,396
  r31 = uint64_t(int64_t(int64_t(r27) + 396));
  // li r28,9
  r28 = uint64_t(int64_t(9));
loc_82125AAC:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82125AC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x82125AD4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r30));
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82125aac
  if (!cr0.eq) goto loc_82125AAC;
  // lwz r3,376(r27)
  r3 = guest::LoadU32(base, uint32_t(r27) + 376);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82125AFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,376(r27)
  r3 = guest::LoadU32(base, uint32_t(r27) + 376);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x82125B0C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r27,380
  r31 = uint64_t(int64_t(int64_t(r27) + 380));
  // li r28,4
  r28 = uint64_t(int64_t(4));
  // stw r30,376(r27)
  guest::StoreU32(base, uint32_t(r27) + 376, uint32_t(r30));
loc_82125B18:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82125B30, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x82125B40, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r30));
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82125b18
  if (!cr0.eq) goto loc_82125B18;
  // mr r3,r27
  r3 = r27;
  // bl 0x821258f8
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::ReleaseTextures(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82126158 game::CarOverlays::UpdateEffects: lifted from generated/ by lift.py.
lifted::Result f82126158(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82126158 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x82124ee0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateTrails(base, LIFTED_ARGS(0x82126170, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82125b60
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateSmoke(base, LIFTED_ARGS(0x82126178, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x821251c0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateLightSprites(base, LIFTED_ARGS(0x82126180, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x821252d0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateParticlesB(base, LIFTED_ARGS(0x82126188, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82124728
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateSparks(base, r1, r13, r3, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82123e48
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateModelSprites(base, r1, r13, r3, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82123a80
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateMarker(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82125798
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateParticlesC(base, LIFTED_ARGS(0x821261A8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821261C0 game::CarOverlays::Dtor: lifted from generated/ by lift.py.
lifted::Result f821261C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_821261C0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r11,r11,-31452
  r11 = uint64_t(int64_t(int64_t(r11) + -31452));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82125970
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::DeleteChildren(base, r1, r13, r3, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82126498
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::Ctor(base, LIFTED_ARGS(0x821261F4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82126204
  if (cr0.eq) goto loc_82126204;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82126204, r31)); r3 = o.r3; f1 = o.f1; }
loc_82126204:
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

// sub_82126220 game::CarOverlays::Update: lifted from generated/ by lift.py.
lifted::Result f82126220(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82126220 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // lwz r4,128(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 128);
  // stw r28,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r28));
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x82126240, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r4,116(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 116);
  // mr r3,r31
  r3 = r31;
  // bl 0x821264a8
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::ViewportEntry(base, LIFTED_ARGS(0x82126250, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 4);
  // rlwinm r9,r11,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lwz r11,11496(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 11496);
  // lwzx r27,r9,r11
  r27 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r11));
  // bne cr6,0x82126294
  if (!cr6.eq) goto loc_82126294;
  // lwz r11,832(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 832);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82126294
  if (cr6.eq) goto loc_82126294;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // stw r28,832(r31)
  guest::StoreU32(base, uint32_t(r31) + 832, uint32_t(r28));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82126294, r31)); r3 = o.r3; f1 = o.f1; }
loc_82126294:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82126490
  if (cr6.eq) goto loc_82126490;
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82126490
  if (cr6.eq) goto loc_82126490;
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82126ff0
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::BodyMatrixCopy(base, LIFTED_ARGS(0x821262B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x821270e8
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::ModelMatrix(base, LIFTED_ARGS(0x821262C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r10,r29
  r10 = r29;
  // addi r9,r11,64
  r9 = uint64_t(int64_t(int64_t(r11) + 64));
loc_821262D4:
  // lbz r8,0(r11)
  r8 = guest::LoadU8(base, uint32_t(r11) + 0);
  // lbz r7,0(r10)
  r7 = guest::LoadU8(base, uint32_t(r10) + 0);
  // subf. r8,r7,r8
  r8 = r8 - r7;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x821262f4
  if (!cr0.eq) goto loc_821262F4;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // bne cr6,0x821262d4
  if (!cr6.eq) goto loc_821262D4;
loc_821262F4:
  // subf r11,r28,r8
  r11 = r8 - r28;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82126334
  if (!cr0.eq) goto loc_82126334;
  // mr r3,r30
  r3 = r30;
  // bl 0x82126ae0
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::RacerId(base, LIFTED_ARGS(0x82126314, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,836(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 836);
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // bne cr6,0x82126334
  if (!cr6.eq) goto loc_82126334;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r10,832(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 832);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // mr r11,r28
  r11 = r28;
  // beq cr6,0x82126338
  if (cr6.eq) goto loc_82126338;
loc_82126334:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_82126338:
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82126350
  if (cr0.eq) goto loc_82126350;
  // mr r3,r31
  r3 = r31;
  // bl 0x821253f0
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::FindModel(base, r1, r13, r3, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // stw r11,832(r31)
  guest::StoreU32(base, uint32_t(r31) + 832, uint32_t(r11));
loc_82126350:
  // mr r3,r30
  r3 = r30;
  // bl 0x82126ae0
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::RacerId(base, LIFTED_ARGS(0x82126358, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,836(r31)
  guest::StoreU32(base, uint32_t(r31) + 836, uint32_t(r3));
  // lwz r10,1860(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 1860);
  // lwz r11,136(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 136);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r10,784(r31)
  guest::StoreU32(base, uint32_t(r31) + 784, uint32_t(r10));
  // beq 0x82126384
  if (cr0.eq) goto loc_82126384;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x8212638c
  if (!cr6.eq) goto loc_8212638C;
  // mr r3,r31
  r3 = r31;
  // bl 0x82126158
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateEffects(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8212638c
  goto loc_8212638C;
loc_82126384:
  // mr r3,r31
  r3 = r31;
  // bl 0x82126158
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::UpdateEffects(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_8212638C:
  // addi r30,r31,164
  r30 = uint64_t(int64_t(int64_t(r31) + 164));
  // li r29,4
  r29 = uint64_t(int64_t(4));
loc_82126394:
  // lwz r3,-16(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + -16);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821263A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821263BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x82126394
  if (!cr0.eq) goto loc_82126394;
  // addi r30,r31,360
  r30 = uint64_t(int64_t(int64_t(r31) + 360));
  // li r29,4
  r29 = uint64_t(int64_t(4));
loc_821263D0:
  // lwz r3,-160(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + -160);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821263E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821263F8, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x821263d0
  if (!cr0.eq) goto loc_821263D0;
  // addi r29,r31,328
  r29 = uint64_t(int64_t(int64_t(r31) + 328));
  // li r30,8
  r30 = uint64_t(int64_t(8));
loc_8212640C:
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82126420, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne 0x8212640c
  if (!cr0.eq) goto loc_8212640C;
  // addi r29,r31,396
  r29 = uint64_t(int64_t(int64_t(r31) + 396));
  // li r30,9
  r30 = uint64_t(int64_t(9));
loc_82126434:
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82126448, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne 0x82126434
  if (!cr0.eq) goto loc_82126434;
  // lwz r3,376(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 376);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82126468, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,380
  r30 = uint64_t(int64_t(int64_t(r31) + 380));
  // li r31,4
  r31 = uint64_t(int64_t(4));
loc_82126470:
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82126484, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x82126470
  if (!cr0.eq) goto loc_82126470;
loc_82126490:
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82126570 game::CarOverlays::FreeView: lifted from generated/ by lift.py.
lifted::Result f82126570(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82126570 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r4,25
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 25, so);
  // bne cr6,0x821265dc
  if (!cr6.eq) goto loc_821265DC;
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // li r30,25
  r30 = uint64_t(int64_t(25));
loc_82126598:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821265B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x821265C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82126598
  if (!cr0.eq) goto loc_82126598;
  // b 0x82126618
  goto loc_82126618;
loc_821265DC:
  // addi r11,r4,1
  r11 = uint64_t(int64_t(int64_t(r4) + 1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // rlwinm r30,r11,2,0,29
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r30,r31
  r3 = guest::LoadU32(base, uint32_t(r30) + uint32_t(r31));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821265FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwzx r3,r30,r31
  r3 = guest::LoadU32(base, uint32_t(r30) + uint32_t(r31));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualFree(base, LIFTED_ARGS(0x82126610, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stwx r11,r30,r31
  guest::StoreU32(base, uint32_t(r30) + uint32_t(r31), uint32_t(r11));
loc_82126618:
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

// sub_82126660 game::CarOverlays::NewView: lifted from generated/ by lift.py.
lifted::Result f82126660(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82126660 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r5,128
  r5 = uint64_t(int64_t(8388608));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = game::VirtualAlloc(base, LIFTED_ARGS(0x82126694, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r3,r11,r31
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r31), uint32_t(r3));
  // beq 0x821266ac
  if (cr0.eq) goto loc_821266AC;
  // bl 0x82126870
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::Ctor(base, LIFTED_ARGS(0x821266AC, r31)); r3 = o.r3; f1 = o.f1; }
loc_821266AC:
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

// sub_82126818: lifted from generated/ by lift.py.
lifted::Result f82126818(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82126818 (lifted)");
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
  // addi r11,r11,-31272
  r11 = uint64_t(int64_t(int64_t(r11) + -31272));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::VtableObject(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x82126844
  if (cr0.eq) goto loc_82126844;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82126844, r31)); r3 = o.r3; f1 = o.f1; }
loc_82126844:
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

// sub_82126A00: lifted from generated/ by lift.py.
uint64_t f82126A00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82126A00 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r3,4(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 4);
  // bl 0x820c0170
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0170, LIFTED_ARGS(0x82126A14, r31)); r3 = o.r3; f1 = o.f1; }
  // lha r11,80(r3)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r3) + 80))));
  // addi r11,r11,-8
  r11 = uint64_t(int64_t(int64_t(r11) + -8));
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
  return r3;
}

// sub_82126A38: lifted from generated/ by lift.py.
uint64_t f82126A38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82126A38 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint8_t ca = 0;
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
  // lwz r3,4(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 4);
  // bl 0x820c0170
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0170, LIFTED_ARGS(0x82126A4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lha r11,80(r3)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r3) + 80))));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // subfic r11,r11,1
  ca = uint32_t(r11) <= 1;
  r11 = static_cast<uint64_t>(1) - r11;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // addi r3,r11,1
  r3 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82126A90: lifted from generated/ by lift.py.
double f82126A90(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82126A90 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  lifted::Temp temp{};
  // lwz r11,4(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 4);
  // lfs f1,2508(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2508);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82126D30: lifted from generated/ by lift.py.
uint64_t f82126D30(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82126D30 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, ctr = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // addi r11,r3,-1
  r11 = uint64_t(int64_t(int64_t(r3) + -1));
  // cmplwi cr6,r11,70
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 70, so);
  // bgt cr6,0x82126ec4
  if (cr6.gt) goto loc_82126EC4;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-31240
  r12 = uint64_t(int64_t(int64_t(r12) + -31240));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32238
  r12 = uint64_t(int64_t(-2112749568));
  // addi r12,r12,28004
  r12 = uint64_t(int64_t(int64_t(r12) + 28004));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_82126D64;
  return r3;
  case 1:
  goto loc_82126D6C;
  case 2:
  goto loc_82126D74;
  case 3:
  goto loc_82126D7C;
  case 4:
  goto loc_82126D84;
  case 5:
  goto loc_82126D8C;
  case 6:
  goto loc_82126D94;
  case 7:
  goto loc_82126D9C;
  case 8:
  goto loc_82126DA4;
  case 9:
  goto loc_82126DAC;
  case 10:
  goto loc_82126DB4;
  case 11:
  goto loc_82126DBC;
  case 12:
  goto loc_82126DC4;
  case 13:
  goto loc_82126DCC;
  case 14:
  goto loc_82126DD4;
  case 15:
  goto loc_82126DDC;
  case 16:
  goto loc_82126DE4;
  case 17:
  goto loc_82126DEC;
  case 18:
  goto loc_82126DF4;
  case 19:
  goto loc_82126DFC;
  case 20:
  goto loc_82126E04;
  case 21:
  goto loc_82126E0C;
  case 22:
  goto loc_82126E14;
  case 23:
  goto loc_82126E1C;
  case 24:
  goto loc_82126E24;
  case 25:
  goto loc_82126E2C;
  case 26:
  goto loc_82126E34;
  case 27:
  goto loc_82126E3C;
  case 28:
  goto loc_82126E44;
  case 29:
  goto loc_82126E4C;
  case 30:
  goto loc_82126E54;
  case 31:
  goto loc_82126E5C;
  case 32:
  goto loc_82126E64;
  case 33:
  goto loc_82126E6C;
  case 34:
  goto loc_82126E74;
  case 35:
  goto loc_82126E74;
  case 36:
  goto loc_82126E74;
  case 37:
  goto loc_82126E74;
  case 38:
  goto loc_82126E74;
  case 39:
  goto loc_82126E74;
  case 40:
  goto loc_82126E74;
  case 41:
  goto loc_82126E74;
  case 42:
  goto loc_82126E74;
  case 43:
  goto loc_82126E74;
  case 44:
  goto loc_82126E74;
  case 45:
  goto loc_82126E74;
  case 46:
  goto loc_82126E7C;
  case 47:
  goto loc_82126E84;
  case 48:
  goto loc_82126E8C;
  case 49:
  goto loc_82126E94;
  case 50:
  goto loc_82126E9C;
  case 51:
  goto loc_82126EA4;
  case 52:
  goto loc_82126EAC;
  case 53:
  goto loc_82126EB4;
  case 54:
  goto loc_82126DD4;
  case 55:
  goto loc_82126DCC;
  case 56:
  goto loc_82126DF4;
  case 57:
  goto loc_82126DDC;
  case 58:
  goto loc_82126D94;
  case 59:
  goto loc_82126DAC;
  case 60:
  goto loc_82126D84;
  case 61:
  goto loc_82126DC4;
  case 62:
  goto loc_82126D9C;
  case 63:
  goto loc_frag_82126D64;
  return r3;
  case 64:
  goto loc_82126D7C;
  case 65:
  goto loc_82126D8C;
  case 66:
  goto loc_82126EC4;
  case 67:
  goto loc_82126DE4;
  case 68:
  goto loc_82126DB4;
  case 69:
  goto loc_82126DBC;
  case 70:
  goto loc_82126EBC;
  default:
  std::abort();
  }
loc_82126D6C:
  // li r3,18
  r3 = uint64_t(int64_t(18));
  // blr
  return r3;
loc_82126D74:
  // li r3,19
  r3 = uint64_t(int64_t(19));
  // blr
  return r3;
loc_82126D7C:
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // blr
  return r3;
loc_82126D84:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // blr
  return r3;
loc_82126D8C:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // blr
  return r3;
loc_82126D94:
  // li r3,10
  r3 = uint64_t(int64_t(10));
  // blr
  return r3;
loc_82126D9C:
  // li r3,9
  r3 = uint64_t(int64_t(9));
  // blr
  return r3;
loc_82126DA4:
  // li r3,21
  r3 = uint64_t(int64_t(21));
  // blr
  return r3;
loc_82126DAC:
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // blr
  return r3;
loc_82126DB4:
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // blr
  return r3;
loc_82126DBC:
  // li r3,7
  r3 = uint64_t(int64_t(7));
  // blr
  return r3;
loc_82126DC4:
  // li r3,6
  r3 = uint64_t(int64_t(6));
  // blr
  return r3;
loc_82126DCC:
  // li r3,12
  r3 = uint64_t(int64_t(12));
  // blr
  return r3;
loc_82126DD4:
  // li r3,11
  r3 = uint64_t(int64_t(11));
  // blr
  return r3;
loc_82126DDC:
  // li r3,13
  r3 = uint64_t(int64_t(13));
  // blr
  return r3;
loc_82126DE4:
  // li r3,14
  r3 = uint64_t(int64_t(14));
  // blr
  return r3;
loc_82126DEC:
  // li r3,20
  r3 = uint64_t(int64_t(20));
  // blr
  return r3;
loc_82126DF4:
  // li r3,15
  r3 = uint64_t(int64_t(15));
  // blr
  return r3;
loc_82126DFC:
  // li r3,22
  r3 = uint64_t(int64_t(22));
  // blr
  return r3;
loc_82126E04:
  // li r3,17
  r3 = uint64_t(int64_t(17));
  // blr
  return r3;
loc_82126E0C:
  // li r3,16
  r3 = uint64_t(int64_t(16));
  // blr
  return r3;
loc_82126E14:
  // li r3,23
  r3 = uint64_t(int64_t(23));
  // blr
  return r3;
loc_82126E1C:
  // li r3,24
  r3 = uint64_t(int64_t(24));
  // blr
  return r3;
loc_82126E24:
  // li r3,25
  r3 = uint64_t(int64_t(25));
  // blr
  return r3;
loc_82126E2C:
  // li r3,26
  r3 = uint64_t(int64_t(26));
  // blr
  return r3;
loc_82126E34:
  // li r3,27
  r3 = uint64_t(int64_t(27));
  // blr
  return r3;
loc_82126E3C:
  // li r3,28
  r3 = uint64_t(int64_t(28));
  // blr
  return r3;
loc_82126E44:
  // li r3,29
  r3 = uint64_t(int64_t(29));
  // blr
  return r3;
loc_82126E4C:
  // li r3,30
  r3 = uint64_t(int64_t(30));
  // blr
  return r3;
loc_82126E54:
  // li r3,31
  r3 = uint64_t(int64_t(31));
  // blr
  return r3;
loc_82126E5C:
  // li r3,32
  r3 = uint64_t(int64_t(32));
  // blr
  return r3;
loc_82126E64:
  // li r3,33
  r3 = uint64_t(int64_t(33));
  // blr
  return r3;
loc_82126E6C:
  // li r3,34
  r3 = uint64_t(int64_t(34));
  // blr
  return r3;
loc_82126E74:
  // li r3,35
  r3 = uint64_t(int64_t(35));
  // blr
  return r3;
loc_82126E7C:
  // li r3,41
  r3 = uint64_t(int64_t(41));
  // blr
  return r3;
loc_82126E84:
  // li r3,42
  r3 = uint64_t(int64_t(42));
  // blr
  return r3;
loc_82126E8C:
  // li r3,43
  r3 = uint64_t(int64_t(43));
  // blr
  return r3;
loc_82126E94:
  // li r3,44
  r3 = uint64_t(int64_t(44));
  // blr
  return r3;
loc_82126E9C:
  // li r3,36
  r3 = uint64_t(int64_t(36));
  // blr
  return r3;
loc_82126EA4:
  // li r3,37
  r3 = uint64_t(int64_t(37));
  // blr
  return r3;
loc_82126EAC:
  // li r3,38
  r3 = uint64_t(int64_t(38));
  // blr
  return r3;
loc_82126EB4:
  // li r3,39
  r3 = uint64_t(int64_t(39));
  // blr
  return r3;
loc_82126EBC:
  // li r3,40
  r3 = uint64_t(int64_t(40));
  // blr
  return r3;
loc_82126EC4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_frag_82126D64:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_82126ED0 game::OverlayView::ModelPoint: lifted from generated/ by lift.py.
lifted::Result f82126ED0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82126ED0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // mr r31,r3
  r31 = r3;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r10,344(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 344);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82126f50
  if (cr6.eq) goto loc_82126F50;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mr r4,r5
  r4 = r5;
  // lwz r3,344(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 344);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82126F14, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f0,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f0 = double(temp.f32);
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 8);
  // std r9,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r9);
  // std r11,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r11);
  // lfs f13,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f13 = double(temp.f32);
  // stfs f13,0(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // lfs f13,88(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f13 = double(temp.f32);
  // stfs f0,12(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 12, temp.u32);
  // lfs f0,84(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f0 = double(temp.f32);
  // stfs f13,8(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 8, temp.u32);
  // b 0x82126f64
  goto loc_82126F64;
loc_82126F50:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // stfs f0,8(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 8, temp.u32);
  // stfs f0,12(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 12, temp.u32);
loc_82126F64:
  // stfs f0,4(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 4, temp.u32);
  // mr r3,r31
  r3 = r31;
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

// sub_82127188: lifted from generated/ by lift.py.
uint64_t f82127188(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82127188 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  // lwz r11,4(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 4);
  // lwz r11,5712(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 5712);
  // lwz r3,12(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 12);
  // b 0x82126d30
  r3 = f82126D30(base, r3);
  return r3;
}

// sub_82127198: lifted from generated/ by lift.py.
uint64_t f82127198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82127198 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r31,399
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 399, so);
  // bgt cr6,0x821271e0
  if (cr6.gt) goto loc_821271E0;
  // bl 0x82126bb0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveFlag199FA(base, LIFTED_ARGS(0x821271C0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821271e0
  if (!cr0.eq) goto loc_821271E0;
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x82126cb0
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveFlag199F8Or199FC(base, LIFTED_ARGS(0x821271D4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beq 0x821271e4
  if (cr0.eq) goto loc_821271E4;
loc_821271E0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_821271E4:
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

// sub_82129298 game::Sprite::Draw: lifted from generated/ by lift.py.
lifted::Result f82129298(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82129298 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,64(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 64);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x821281a8
  { const lifted::Result o = game::Sprite::Record(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_821292B0 game::SpriteList::Draw: lifted from generated/ by lift.py.
lifted::Result f821292B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_821292B0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,64(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 64);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bnelr cr6
  if (!cr6.eq) return {r3, f1};
  // b 0x82128750
  { const lifted::Result o = game::SpriteList::Record(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_821292C8 game::Trail::Init: lifted from generated/ by lift.py.
lifted::Result f821292C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_821292C8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // stw r5,104(r3)
  guest::StoreU32(base, uint32_t(r3) + 104, uint32_t(r5));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r10,8(r4)
  r10 = guest::LoadU32(base, uint32_t(r4) + 8);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // stw r10,112(r3)
  guest::StoreU32(base, uint32_t(r3) + 112, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8212A1C0 game::Trail::Dtor: lifted from generated/ by lift.py.
lifted::Result f8212A1C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8212A1C0 (lifted)");
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
  // addi r11,r11,-31040
  r11 = uint64_t(int64_t(int64_t(r11) + -31040));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82126498
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::Ctor(base, LIFTED_ARGS(0x8212A1EC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8212a1fc
  if (cr0.eq) goto loc_8212A1FC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8212A1FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8212A1FC:
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

// sub_8212B3A8 game::Smoke::Init: lifted from generated/ by lift.py.
lifted::Result f8212B3A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8212B3A8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // stw r5,104(r3)
  guest::StoreU32(base, uint32_t(r3) + 104, uint32_t(r5));
  // lwz r10,0(r4)
  r10 = guest::LoadU32(base, uint32_t(r4) + 0);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // stw r10,116(r3)
  guest::StoreU32(base, uint32_t(r3) + 116, uint32_t(r10));
  // lwz r10,4(r4)
  r10 = guest::LoadU32(base, uint32_t(r4) + 4);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // stw r10,112(r3)
  guest::StoreU32(base, uint32_t(r3) + 112, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8212C0C0 game::ParticlesC::Init: lifted from generated/ by lift.py.
lifted::Result f8212C0C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8212C0C0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // stw r5,2700(r3)
  guest::StoreU32(base, uint32_t(r3) + 2700, uint32_t(r5));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r10,8(r4)
  r10 = guest::LoadU32(base, uint32_t(r4) + 8);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // stw r10,108(r3)
  guest::StoreU32(base, uint32_t(r3) + 108, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8212CB70 game::ParticlesC::Dtor: lifted from generated/ by lift.py.
lifted::Result f8212CB70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8212CB70 (lifted)");
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
  // addi r11,r11,-30896
  r11 = uint64_t(int64_t(int64_t(r11) + -30896));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82126498
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::Ctor(base, LIFTED_ARGS(0x8212CB9C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8212cbac
  if (cr0.eq) goto loc_8212CBAC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8212CBAC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8212CBAC:
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

// sub_8212CBC8 game::Marker::Init: lifted from generated/ by lift.py.
lifted::Result f8212CBC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8212CBC8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,8(r4)
  r11 = guest::LoadU32(base, uint32_t(r4) + 8);
  // lwz r10,0(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 0);
  // stw r11,120(r3)
  guest::StoreU32(base, uint32_t(r3) + 120, uint32_t(r11));
  // lwz r11,12(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 12);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8212D680 game::Marker::Dtor: lifted from generated/ by lift.py.
lifted::Result f8212D680(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8212D680 (lifted)");
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
  // addi r11,r11,-30844
  r11 = uint64_t(int64_t(int64_t(r11) + -30844));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82126498
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::Ctor(base, LIFTED_ARGS(0x8212D6AC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8212d6bc
  if (cr0.eq) goto loc_8212D6BC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8212D6BC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8212D6BC:
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

// sub_8212EA40 game::Sparks::Dtor: lifted from generated/ by lift.py.
lifted::Result f8212EA40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8212EA40 (lifted)");
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
  // addi r11,r11,-30788
  r11 = uint64_t(int64_t(int64_t(r11) + -30788));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82126498
  // (left out: dead in C++)
  { const lifted::Result o = game::Overlay::Ctor(base, LIFTED_ARGS(0x8212EA6C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8212ea7c
  if (cr0.eq) goto loc_8212EA7C;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8212EA7C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8212EA7C:
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

// sub_8212EDF8 game::ScreenObject::Start: lifted from generated/ by lift.py.
lifted::Result f8212EDF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8212EDF8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r31,r3
  r31 = r3;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // mulli r10,r10,592
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(592))));
  // add. r11,r10,r11
  r11 = r10 + r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8212eebc
  if (cr0.eq) goto loc_8212EEBC;
  // lwz r11,588(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8212eebc
  if (cr6.eq) goto loc_8212EEBC;
  // lbz r11,24(r31)
  r11 = guest::LoadU8(base, uint32_t(r31) + 24);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8212eebc
  if (!cr0.eq) goto loc_8212EEBC;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r3,88(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 88);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stb r11,24(r31)
  guest::StoreU8(base, uint32_t(r31) + 24, uint8_t(r11));
  // beq 0x8212ee94
  if (cr0.eq) goto loc_8212EE94;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212EE60, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,88(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stfs f1,28(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r31) + 28, temp.u32);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,84(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212EE78, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,88(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 88);
  // stfs f1,32(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r31) + 32, temp.u32);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,88(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 88);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212EE90, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f1,36(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r31) + 36, temp.u32);
loc_8212EE94:
  // lwz r3,92(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 92);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8212eebc
  if (cr0.eq) goto loc_8212EEBC;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // addi r6,r31,60
  r6 = uint64_t(int64_t(int64_t(r31) + 60));
  // addi r5,r31,56
  r5 = uint64_t(int64_t(int64_t(r31) + 56));
  // addi r4,r31,52
  r4 = uint64_t(int64_t(int64_t(r31) + 52));
  // lwz r11,36(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212EEBC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8212EEBC:
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

// sub_8212EED0 game::ScreenObject::Stop: lifted from generated/ by lift.py.
void f8212EED0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8212EED0 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // lbz r11,26(r3)
  r11 = guest::LoadU8(base, uint32_t(r3) + 26);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bnelr
  if (!cr0.eq) return;
  // lbz r11,24(r3)
  r11 = guest::LoadU8(base, uint32_t(r3) + 24);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr
  if (cr0.eq) return;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,26(r3)
  guest::StoreU8(base, uint32_t(r3) + 26, uint8_t(r11));
  // blr
  return;
}

// sub_8212EEF8: lifted from generated/ by lift.py.
void f8212EEF8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8212EEF8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8212ef08
  if (!cr6.eq) goto loc_8212EF08;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8212EF08:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 4);
  // lbz r11,33(r10)
  r11 = guest::LoadU8(base, uint32_t(r10) + 33);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8212ef30
  if (cr6.eq) goto loc_8212EF30;
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // lbz r11,33(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 33);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // b 0x8212ef9c
  goto loc_8212EF9C;
loc_8212EF30:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // lbz r9,33(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 33);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8212ef4c
  if (cr6.eq) goto loc_8212EF4C;
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 4);
  // b 0x8212ef80
  goto loc_8212EF80;
loc_8212EF48:
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
loc_8212EF4C:
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lbz r10,33(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 33);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8212ef48
  if (cr6.eq) goto loc_8212EF48;
loc_8212EF5C:
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // blr
  return;
loc_8212EF64:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 4);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8212ef8c
  if (!cr6.eq) goto loc_8212EF8C;
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
loc_8212EF80:
  // lbz r10,33(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 33);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8212ef64
  if (cr6.eq) goto loc_8212EF64;
loc_8212EF8C:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 4);
  // lbz r10,33(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 33);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8212ef5c
  if (cr6.eq) goto loc_8212EF5C;
loc_8212EF9C:
  // twi 31,r0,22
  lifted::Trap(22);
  // blr
  return;
}

// sub_8212F538: lifted from generated/ by lift.py.
lifted::Result f8212F538(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8212F538 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // fmr f28,f1
  lifted::ScalarFloatMode();
  f28 = f1;
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addis r31,r3,1
  r31 = uint64_t(int64_t(int64_t(r3) + 65536));
  // addi r30,r3,112
  r30 = uint64_t(int64_t(int64_t(r3) + 112));
  // lfs f31,7140(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 7140);
  f31 = double(temp.f32);
  // addi r31,r31,-5436
  r31 = uint64_t(int64_t(int64_t(r31) + -5436));
  // lfs f29,-30720(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -30720);
  f29 = double(temp.f32);
  // li r29,720
  r29 = uint64_t(int64_t(720));
  // lfs f30,-30724(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -30724);
  f30 = double(temp.f32);
loc_8212F578:
  // bl 0x825a9bb8
  // (left out: dead in C++)
  { const lifted::Result o = game::rand_825A9BB8(base, LIFTED_ARGS(0x8212F57C, r31)); r3 = o.r3; f1 = o.f1; }
  // extsw r8,r3
  r8 = uint64_t(int64_t(int32_t(r3)));
  // addi r11,r30,20
  r11 = uint64_t(int64_t(int64_t(r30) + 20));
  // mr r9,r31
  r9 = r31;
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // std r8,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r8);
  // addi r30,r11,40
  r30 = uint64_t(int64_t(int64_t(r11) + 40));
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmsubs f0,f0,f30,f29
  f0 = double(float(std::fma(f0, f30, -f29)));
  // fmuls f0,f0,f28
  f0 = double(float(f0 * f28));
loc_8212F5AC:
  // ld r8,0(r9)
  r8 = guest::LoadU64(base, uint32_t(r9) + 0);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r9,r9,8
  r9 = uint64_t(int64_t(int64_t(r9) + 8));
  // std r8,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r8);
  // lfs f13,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f13 = double(temp.f32);
  // lfs f12,84(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f12 = double(temp.f32);
  // fmadds f13,f13,f0,f13
  f13 = double(float(std::fma(f13, f0, f13)));
  // fmadds f12,f12,f0,f12
  f12 = double(float(std::fma(f12, f0, f12)));
  // fadds f13,f13,f31
  f13 = double(float(f13 + f31));
  // stfs f13,-4(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r11) + -4, temp.u32);
  // fadds f13,f12,f31
  f13 = double(float(f12 + f31));
  // stfs f13,0(r11)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,20
  r11 = uint64_t(int64_t(int64_t(r11) + 20));
  // bne 0x8212f5ac
  if (!cr0.eq) goto loc_8212F5AC;
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne 0x8212f578
  if (!cr0.eq) goto loc_8212F578;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8212F600: lifted from generated/ by lift.py.
lifted::Result f8212F600(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8212F600 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // stfd f29,-40(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -40, std::bit_cast<uint64_t>(f29));
  // stfd f30,-32(r1)
  guest::StoreU64(base, uint32_t(r1) + -32, std::bit_cast<uint64_t>(f30));
  // stfd f31,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8212f754
  if (cr6.eq) goto loc_8212F754;
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r11,r11,62460
  r11 = r11 | 62460;
  // lfsx f0,r31,r11
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r11));
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,8712(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8712);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // lfs f13,10772(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10772);
  f13 = double(temp.f32);
  // ble cr6,0x8212f658
  if (!cr6.gt) goto loc_8212F658;
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x8212f660
  if (!cr6.gt) goto loc_8212F660;
loc_8212F658:
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x8212f754
  if (!cr6.gt) goto loc_8212F754;
loc_8212F660:
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x8212f674
  if (!cr6.gt) goto loc_8212F674;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // b 0x8212f684
  goto loc_8212F684;
loc_8212F674:
  // fsubs f13,f0,f12
  lifted::ScalarFloatMode();
  f13 = double(float(f0 - f12));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-30716(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -30716);
  f0 = double(temp.f32);
  // fmuls f31,f13,f0
  f31 = double(float(f13 * f0));
loc_8212F684:
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x825a9d80
  // (left out: dead in C++)
  { const lifted::Result o = game::log10(base, LIFTED_ARGS(0x8212F68C, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,32(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 32);
  f0 = double(temp.f32);
  // lwz r3,88(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 88);
  // lfs f13,36(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 36);
  f13 = double(temp.f32);
  // frsp f11,f1
  f11 = double(float(f1));
  // lfs f12,40(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 40);
  f12 = double(temp.f32);
  // lfs f10,44(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 44);
  f10 = double(temp.f32);
  // lfs f9,48(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 48);
  f9 = double(temp.f32);
  // fsubs f10,f0,f10
  f10 = double(float(f0 - f10));
  // lfs f8,28(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 28);
  f8 = double(temp.f32);
  // fsubs f9,f13,f9
  f9 = double(float(f13 - f9));
  // fsubs f8,f8,f12
  f8 = double(float(f8 - f12));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // fnmsubs f30,f10,f31,f0
  f30 = double(float(-std::fma(f10, f31, -f0)));
  // fnmsubs f29,f9,f31,f13
  f29 = double(float(-std::fma(f9, f31, -f13)));
  // fnmsubs f1,f8,f11,f12
  f1 = double(float(-std::fma(f8, f11, -f12)));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212F6D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,88(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 88);
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212F6EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,88(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 88);
  // fmr f1,f29
  lifted::ScalarFloatMode();
  f1 = f29;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212F704, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8212f754
  if (cr6.eq) goto loc_8212F754;
  // lfs f0,60(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 60);
  f0 = double(temp.f32);
  // rotlwi r3,r11,0
  r3 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lfs f13,56(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 56);
  f13 = double(temp.f32);
  // lfs f12,52(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 52);
  f12 = double(temp.f32);
  // lfs f11,72(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 72);
  f11 = double(temp.f32);
  // lfs f10,68(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 68);
  f10 = double(temp.f32);
  // fsubs f11,f11,f0
  f11 = double(float(f11 - f0));
  // lfs f9,64(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 64);
  f9 = double(temp.f32);
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fsubs f9,f9,f12
  f9 = double(float(f9 - f12));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // fmadds f3,f11,f31,f0
  f3 = double(float(std::fma(f11, f31, f0)));
  // fmadds f2,f10,f31,f13
  f2 = double(float(std::fma(f10, f31, f13)));
  // fmadds f1,f9,f31,f12
  f1 = double(float(std::fma(f9, f31, f12)));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212F754, r31)); r3 = o.r3; f1 = o.f1; }
loc_8212F754:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f29,-40(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // lfd f30,-32(r1)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -32)));
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8212F778: lifted from generated/ by lift.py.
lifted::Result f8212F778(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8212F778 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // addi r12,r1,-16
  r12 = uint64_t(int64_t(int64_t(r1) + -16));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // lwz r11,88(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 88);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8212f8a8
  if (cr6.eq) goto loc_8212F8A8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f28,8124(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8124);
  f28 = double(temp.f32);
  // fcmpu cr6,f31,f28
  cr6 = lifted::CompareFloat(f31, f28);
  // blt cr6,0x8212f7f0
  if (cr6.lt) goto loc_8212F7F0;
  // bl 0x825a9d80
  // (left out: dead in C++)
  { const lifted::Result o = game::log10(base, LIFTED_ARGS(0x8212F7B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,40(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 40);
  f0 = double(temp.f32);
  // lfs f13,32(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 32);
  f13 = double(temp.f32);
  // frsp f8,f1
  f8 = double(float(f1));
  // lfs f12,36(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 36);
  f12 = double(temp.f32);
  // lfs f11,28(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 28);
  f11 = double(temp.f32);
  // lfs f10,44(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 44);
  f10 = double(temp.f32);
  // fsubs f11,f11,f0
  f11 = double(float(f11 - f0));
  // lfs f9,48(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 48);
  f9 = double(temp.f32);
  // fsubs f10,f13,f10
  f10 = double(float(f13 - f10));
  // fsubs f9,f12,f9
  f9 = double(float(f12 - f9));
  // fnmsubs f1,f11,f8,f0
  f1 = double(float(-std::fma(f11, f8, -f0)));
  // fnmsubs f30,f10,f31,f13
  f30 = double(float(-std::fma(f10, f31, -f13)));
  // fnmsubs f29,f9,f31,f12
  f29 = double(float(-std::fma(f9, f31, -f12)));
  // b 0x8212f7fc
  goto loc_8212F7FC;
loc_8212F7F0:
  // lfs f1,28(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 28);
  f1 = double(temp.f32);
  // lfs f30,32(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 32);
  f30 = double(temp.f32);
  // lfs f29,36(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 36);
  f29 = double(temp.f32);
loc_8212F7FC:
  // lwz r3,88(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 88);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212F810, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,88(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 88);
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212F828, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,88(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 88);
  // fmr f1,f29
  lifted::ScalarFloatMode();
  f1 = f29;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212F840, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,92(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 92);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8212f8a8
  if (cr6.eq) goto loc_8212F8A8;
  // rotlwi r3,r11,0
  r3 = __builtin_rotateleft32(uint32_t(r11), 0);
  // fcmpu cr6,f31,f28
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f31, f28);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // blt cr6,0x8212f894
  if (cr6.lt) goto loc_8212F894;
  // lfs f0,60(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 60);
  f0 = double(temp.f32);
  // lfs f13,56(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 56);
  f13 = double(temp.f32);
  // lfs f12,52(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 52);
  f12 = double(temp.f32);
  // lfs f11,72(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 72);
  f11 = double(temp.f32);
  // lfs f10,68(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 68);
  f10 = double(temp.f32);
  // fsubs f11,f11,f0
  f11 = double(float(f11 - f0));
  // lfs f9,64(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 64);
  f9 = double(temp.f32);
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fsubs f9,f9,f12
  f9 = double(float(f9 - f12));
  // fmadds f3,f11,f31,f0
  f3 = double(float(std::fma(f11, f31, f0)));
  // fmadds f2,f10,f31,f13
  f2 = double(float(std::fma(f10, f31, f13)));
  // fmadds f1,f9,f31,f12
  f1 = double(float(std::fma(f9, f31, f12)));
  // b 0x8212f8a0
  goto loc_8212F8A0;
loc_8212F894:
  // lfs f3,60(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 60);
  f3 = double(temp.f32);
  // lfs f2,56(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 56);
  f2 = double(temp.f32);
  // lfs f1,52(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 52);
  f1 = double(temp.f32);
loc_8212F8A0:
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8212F8A8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8212F8A8:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r12,r1,-16
  r12 = uint64_t(int64_t(int64_t(r1) + -16));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82130330: lifted from generated/ by lift.py.
lifted::Result f82130330(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, double f2, double f3, double f4) {
  ITC_NATIVE_RUNNING("sub_82130330 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a7914
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmr f29,f2
  lifted::ScalarFloatMode();
  f29 = f2;
  // mr r31,r3
  r31 = r3;
  // fmr f28,f3
  f28 = f3;
  // fmr f27,f4
  f27 = f4;
  // lfd f31,-30712(r11)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -30712)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f30,8888(r11)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 8888)));
  // fmadd f1,f1,f31,f30
  f1 = std::fma(f1, f31, f30);
  // bl 0x825a8ca0
  // (left out: dead in C++)
  { const lifted::Result o = game::floor(base, LIFTED_ARGS(0x8213036C, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmadd f1,f29,f31,f30
  f1 = std::fma(f29, f31, f30);
  // lfd f31,11160(r11)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 11160)));
  // fmul f0,f0,f31
  f0 = f0 * f31;
  // stfd f0,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, std::bit_cast<uint64_t>(f0));
  // bl 0x825a8ca0
  // (left out: dead in C++)
  { const lifted::Result o = game::floor(base, LIFTED_ARGS(0x82130388, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // fmul f0,f1,f31
  lifted::ScalarFloatMode();
  f0 = f1 * f31;
  // addi r31,r31,10588
  r31 = uint64_t(int64_t(int64_t(r31) + 10588));
  // stfd f0,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, std::bit_cast<uint64_t>(f0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // bl 0x82130290
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82130290, LIFTED_ARGS(0x821303A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,80(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x821303c0
  if (cr6.eq) goto loc_821303C0;
  // cmplw cr6,r30,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r31), so);
  // beq cr6,0x821303c4
  if (cr6.eq) goto loc_821303C4;
loc_821303C0:
  // twi 31,r0,22
  lifted::Trap(22);
loc_821303C4:
  // lwz r29,84(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 84);
  // subf r11,r29,r11
  r11 = r11 - r29;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8213047c
  if (cr0.eq) goto loc_8213047C;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x821303ec
  if (!cr6.eq) goto loc_821303EC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821303EC:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x821303fc
  if (!cr6.eq) goto loc_821303FC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821303FC:
  // addi r31,r29,24
  r31 = uint64_t(int64_t(int64_t(r29) + 24));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // bl 0x821301f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821301F0, LIFTED_ARGS(0x82130410, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x82130420
  if (!cr6.eq) goto loc_82130420;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82130420:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r9,4(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82130438
  if (cr6.eq) goto loc_82130438;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // beq cr6,0x8213043c
  if (cr6.eq) goto loc_8213043C;
loc_82130438:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8213043C:
  // lwz r10,84(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 84);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm. r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x8213047c
  if (cr0.eq) goto loc_8213047C;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82130464
  if (!cr6.eq) goto loc_82130464;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82130464:
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x82130474
  if (!cr6.eq) goto loc_82130474;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82130474:
  // stfs f28,24(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f28);
  guest::StoreU32(base, uint32_t(r10) + 24, temp.u32);
  // stfs f27,28(r10)
  temp.f32 = float(f27);
  guest::StoreU32(base, uint32_t(r10) + 28, temp.u32);
loc_8213047C:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a7960
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82130490: lifted from generated/ by lift.py.
lifted::Result f82130490(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, double f1, double f2) {
  ITC_NATIVE_RUNNING("sub_82130490 (lifted)");
  uint64_t r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmr f29,f2
  f29 = f2;
  // mr r31,r3
  r31 = r3;
  // mr r28,r6
  r28 = r6;
  // mr r27,r7
  r27 = r7;
  // lfd f31,-30712(r11)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + -30712)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f30,8888(r11)
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 8888)));
  // fmadd f1,f1,f31,f30
  f1 = std::fma(f1, f31, f30);
  // bl 0x825a8ca0
  // (left out: dead in C++)
  { const lifted::Result o = game::floor(base, LIFTED_ARGS(0x821304D0, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmadd f1,f29,f31,f30
  f1 = std::fma(f29, f31, f30);
  // lfd f31,11160(r11)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 11160)));
  // fmul f0,f0,f31
  f0 = f0 * f31;
  // stfd f0,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, std::bit_cast<uint64_t>(f0));
  // bl 0x825a8ca0
  // (left out: dead in C++)
  { const lifted::Result o = game::floor(base, LIFTED_ARGS(0x821304EC, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // fmul f0,f1,f31
  lifted::ScalarFloatMode();
  f0 = f1 * f31;
  // addi r31,r31,10588
  r31 = uint64_t(int64_t(int64_t(r31) + 10588));
  // stfd f0,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, std::bit_cast<uint64_t>(f0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // bl 0x82130290
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82130290, LIFTED_ARGS(0x8213050C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,80(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82130524
  if (cr6.eq) goto loc_82130524;
  // cmplw cr6,r30,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r31), so);
  // beq cr6,0x82130528
  if (cr6.eq) goto loc_82130528;
loc_82130524:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82130528:
  // lwz r29,84(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 84);
  // subf r11,r29,r11
  r11 = r11 - r29;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82130600
  if (cr0.eq) goto loc_82130600;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x82130550
  if (!cr6.eq) goto loc_82130550;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82130550:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x82130560
  if (!cr6.eq) goto loc_82130560;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82130560:
  // addi r31,r29,24
  r31 = uint64_t(int64_t(int64_t(r29) + 24));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // bl 0x821301f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821301F0, LIFTED_ARGS(0x82130574, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x82130584
  if (!cr6.eq) goto loc_82130584;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82130584:
  // lwz r10,80(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r9,4(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8213059c
  if (cr6.eq) goto loc_8213059C;
  // cmplw cr6,r10,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r31), so);
  // beq cr6,0x821305a0
  if (cr6.eq) goto loc_821305A0;
loc_8213059C:
  // twi 31,r0,22
  lifted::Trap(22);
loc_821305A0:
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
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
  // beq 0x82130600
  if (cr0.eq) goto loc_82130600;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x821305c8
  if (!cr6.eq) goto loc_821305C8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821305C8:
  // lwz r9,4(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 4);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x821305d8
  if (!cr6.eq) goto loc_821305D8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821305D8:
  // lfs f0,24(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 24);
  f0 = double(temp.f32);
  // stfs f0,0(r28)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r28) + 0, temp.u32);
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x821305f0
  if (!cr6.eq) goto loc_821305F0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821305F0:
  // lfs f0,28(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 28);
  f0 = double(temp.f32);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stfs f0,0(r27)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r27) + 0, temp.u32);
  // b 0x82130604
  goto loc_82130604;
loc_82130600:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82130604:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
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

// sub_82130618: lifted from generated/ by lift.py.
lifted::Result f82130618(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82130618 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f18 = 0.0, f19 = 0.0, f20 = 0.0, f21 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-72
  r12 = uint64_t(int64_t(int64_t(r1) + -72));
  // bl 0x825a78f0
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-336(r1)
  ea = -336 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // mr r26,r3
  r26 = r3;
  // ori r11,r11,62456
  r11 = r11 | 62456;
  // addis r24,r26,2
  r24 = uint64_t(int64_t(int64_t(r26) + 131072));
  // addi r24,r24,-3092
  r24 = uint64_t(int64_t(int64_t(r24) + -3092));
  // lfsx f13,r26,r11
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r26) + uint32_t(r11));
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-30756(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -30756);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f19,f13,f0
  f19 = double(float(f13 * f0));
  // lfs f13,0(r24)
  temp.u32 = guest::LoadU32(base, uint32_t(r24) + 0);
  f13 = double(temp.f32);
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,0(r24)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r24) + 0, temp.u32);
  // lfs f18,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f18 = double(temp.f32);
  // fcmpu cr6,f0,f19
  cr6 = lifted::CompareFloat(f0, f19);
  // ble cr6,0x82130678
  if (!cr6.gt) goto loc_82130678;
  // stfs f18,0(r24)
  temp.f32 = float(f18);
  guest::StoreU32(base, uint32_t(r24) + 0, temp.u32);
loc_82130678:
  // addis r9,r26,2
  r9 = uint64_t(int64_t(int64_t(r26) + 131072));
  // addi r9,r9,-3084
  r9 = uint64_t(int64_t(int64_t(r9) + -3084));
  // lbz r11,0(r9)
  r11 = guest::LoadU8(base, uint32_t(r9) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // addis r11,r26,2
  r11 = uint64_t(int64_t(int64_t(r26) + 131072));
  // addi r11,r11,-3088
  r11 = uint64_t(int64_t(int64_t(r11) + -3088));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // beq 0x821306a0
  if (cr0.eq) goto loc_821306A0;
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // b 0x821306a4
  goto loc_821306A4;
loc_821306A0:
  // addi r10,r10,-2
  r10 = uint64_t(int64_t(int64_t(r10) + -2));
loc_821306A4:
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // li r25,5
  r25 = uint64_t(int64_t(5));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // ble cr6,0x821306c4
  if (!cr6.gt) goto loc_821306C4;
  // stw r25,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r25));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // b 0x821306d8
  goto loc_821306D8;
loc_821306C4:
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // bge cr6,0x821306dc
  if (!cr6.lt) goto loc_821306DC;
  // li r10,-5
  r10 = uint64_t(int64_t(-5));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821306D8:
  // stb r10,0(r9)
  guest::StoreU8(base, uint32_t(r9) + 0, uint8_t(r10));
loc_821306DC:
  // lwz r30,0(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lbz r28,0(r9)
  r28 = guest::LoadU8(base, uint32_t(r9) + 0);
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lfs f30,7140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f30 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f20,10712(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 10712);
  f20 = double(temp.f32);
  // fmuls f23,f19,f30
  f23 = double(float(f19 * f30));
  // lfs f21,9576(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 9576);
  f21 = double(temp.f32);
  // lfs f28,10568(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 10568);
  f28 = double(temp.f32);
  // lfs f24,-30704(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -30704);
  f24 = double(temp.f32);
loc_82130714:
  // addi r27,r29,1
  r27 = uint64_t(int64_t(int64_t(r29) + 1));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // clrldi r11,r27,32
  r11 = r27 & 0xFFFFFFFF;
  // std r11,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r11);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f25,f0
  f25 = double(float(f0));
  // fmuls f22,f25,f24
  f22 = double(float(f25 * f24));
loc_82130734:
  // clrldi r11,r31,32
  r11 = r31 & 0xFFFFFFFF;
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f31,f0,f28
  f31 = double(float(f0 * f28));
  // fmr f1,f31
  f1 = f31;
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = game::cos(base, LIFTED_ARGS(0x82130754, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 8);
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // fmr f1,f31
  f1 = f31;
  // std r11,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r11);
  // frsp f0,f0
  f0 = double(float(f0));
  // fmadds f0,f0,f22,f30
  f0 = double(float(std::fma(f0, f22, f30)));
  // lfd f13,96(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f27,f0,f13
  f27 = double(float(f0 * f13));
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = game::sin(base, LIFTED_ARGS(0x82130780, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,12(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 12);
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lfs f0,0(r24)
  temp.u32 = guest::LoadU32(base, uint32_t(r24) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f23
  cr6 = lifted::CompareFloat(f0, f23);
  // std r11,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r11);
  // fmadds f13,f13,f22,f30
  f13 = double(float(std::fma(f13, f22, f30)));
  // lfd f12,104(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f26,f13,f12
  f26 = double(float(f13 * f12));
  // bge cr6,0x821307d0
  if (!cr6.lt) goto loc_821307D0;
  // clrldi r11,r29,32
  r11 = r29 & 0xFFFFFFFF;
  // fdivs f13,f0,f23
  f13 = double(float(f0 / f23));
  // std r11,112(r1)
  guest::StoreU64(base, uint32_t(r1) + 112, r11);
  // fmuls f13,f13,f21
  f13 = double(float(f13 * f21));
  // lfd f12,112(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 112)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // ble cr6,0x82130800
  if (!cr6.gt) goto loc_82130800;
loc_821307D0:
  // fcmpu cr6,f0,f23
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f23);
  // blt cr6,0x82130830
  if (cr6.lt) goto loc_82130830;
  // clrldi r11,r29,32
  r11 = r29 & 0xFFFFFFFF;
  // fsubs f0,f0,f23
  f0 = double(float(f0 - f23));
  // std r11,120(r1)
  guest::StoreU64(base, uint32_t(r1) + 120, r11);
  // fdivs f0,f0,f23
  f0 = double(float(f0 / f23));
  // fmuls f0,f0,f21
  f0 = double(float(f0 * f21));
  // lfd f13,120(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 120)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82130830
  if (cr6.lt) goto loc_82130830;
loc_82130800:
  // divw r10,r30,r25
  r10 = uint32_t((int32_t(r25) && !(int32_t(r30) == INT32_MIN && int32_t(r25) == -1)) ? int32_t(r30) / int32_t(r25) : 0);
  // add r11,r31,r30
  r11 = r31 + r30;
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r10,136(r1)
  guest::StoreU64(base, uint32_t(r1) + 136, r10);
  // std r11,128(r1)
  guest::StoreU64(base, uint32_t(r1) + 128, r11);
  // lfd f13,136(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 136)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f0,128(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 128)));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f29,f13,f20
  f29 = double(float(f13 * f20));
  // b 0x82130840
  goto loc_82130840;
loc_82130830:
  // extsw r11,r31
  r11 = uint64_t(int64_t(int32_t(r31)));
  // fmr f29,f18
  lifted::ScalarFloatMode();
  f29 = f18;
  // std r11,144(r1)
  guest::StoreU64(base, uint32_t(r1) + 144, r11);
  // lfd f0,144(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 144)));
loc_82130840:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f31,f0,f28
  f31 = double(float(f0 * f28));
  // fmr f1,f31
  f1 = f31;
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = game::cos(base, LIFTED_ARGS(0x82130854, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // fmr f1,f31
  f1 = f31;
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = game::sin(base, LIFTED_ARGS(0x82130864, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f0,f29,f24
  lifted::ScalarFloatMode();
  f0 = double(float(f29 + f24));
  // mr r3,r26
  r3 = r26;
  // fmr f13,f1
  f13 = f1;
  // fmr f2,f26
  f2 = f26;
  // fmr f1,f27
  f1 = f27;
  // fmuls f12,f0,f25
  f12 = double(float(f0 * f25));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f0,f0,f25
  f0 = double(float(f0 * f25));
  // fmadds f4,f12,f13,f30
  f4 = double(float(std::fma(f12, f13, f30)));
  // fmadds f3,f0,f31,f30
  f3 = double(float(std::fma(f0, f31, f30)));
  // bl 0x82130330
  // (left out: dead in C++)
  { const lifted::Result o = f82130330(base, r1, r13, r3, f1, f2, f3, f4); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,10
  r31 = uint64_t(int64_t(int64_t(r31) + 10));
  // cmplwi cr6,r31,360
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 360, so);
  // blt cr6,0x82130734
  if (cr6.lt) goto loc_82130734;
  // clrlwi. r11,r28,24
  r11 = uint32_t(r28) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821308ac
  if (cr0.eq) goto loc_821308AC;
  // addi r30,r30,2
  r30 = uint64_t(int64_t(int64_t(r30) + 2));
  // b 0x821308b0
  goto loc_821308B0;
loc_821308AC:
  // addi r30,r30,-2
  r30 = uint64_t(int64_t(int64_t(r30) + -2));
loc_821308B0:
  // cmpwi cr6,r30,5
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 5, so);
  // ble cr6,0x821308c4
  if (!cr6.gt) goto loc_821308C4;
  // mr r30,r25
  r30 = r25;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // b 0x821308d4
  goto loc_821308D4;
loc_821308C4:
  // cmpwi cr6,r30,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r30), -5, so);
  // bge cr6,0x821308d4
  if (!cr6.lt) goto loc_821308D4;
  // li r30,-5
  r30 = uint64_t(int64_t(-5));
  // li r28,1
  r28 = uint64_t(int64_t(1));
loc_821308D4:
  // mr r29,r27
  r29 = r27;
  // cmplwi cr6,r29,20
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 20, so);
  // blt cr6,0x82130714
  if (cr6.lt) goto loc_82130714;
  // addis r29,r26,2
  r29 = uint64_t(int64_t(int64_t(r26) + 131072));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r29,r29,-3096
  r29 = uint64_t(int64_t(int64_t(r29) + -3096));
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x82130944
  if (!cr6.gt) goto loc_82130944;
  // addis r31,r26,1
  r31 = uint64_t(int64_t(int64_t(r26) + 65536));
  // addi r31,r31,10604
  r31 = uint64_t(int64_t(int64_t(r31) + 10604));
loc_82130900:
  // addi r7,r1,88
  r7 = uint64_t(int64_t(int64_t(r1) + 88));
  // lfs f1,-4(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + -4);
  f1 = double(temp.f32);
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f2,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f2 = double(temp.f32);
  // mr r3,r26
  r3 = r26;
  // bl 0x82130490
  // (left out: dead in C++)
  { const lifted::Result o = f82130490(base, r1, r13, r3, r6, r7, f1, f2); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82130930
  if (cr0.eq) goto loc_82130930;
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // lfs f13,88(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f13 = double(temp.f32);
  // stfs f0,8(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 8, temp.u32);
  // stfs f13,12(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 12, temp.u32);
loc_82130930:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,20
  r31 = uint64_t(int64_t(int64_t(r31) + 20));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x82130900
  if (cr6.lt) goto loc_82130900;
loc_82130944:
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // lfs f0,0(r24)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r24) + 0);
  f0 = double(temp.f32);
  // fdivs f13,f0,f19
  f13 = double(float(f0 / f19));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // ori r11,r11,62460
  r11 = r11 | 62460;
  // fcmpu cr6,f0,f18
  cr6 = lifted::CompareFloat(f0, f18);
  // stfsx f13,r26,r11
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r26) + uint32_t(r11), temp.u32);
  // bne cr6,0x82130968
  if (!cr6.eq) goto loc_82130968;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82130968:
  // addi r1,r1,336
  r1 = uint64_t(int64_t(int64_t(r1) + 336));
  // addi r12,r1,-72
  r12 = uint64_t(int64_t(int64_t(r1) + -72));
  // bl 0x825a793c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82132CE0 game::ScreenObject::Init: lifted from generated/ by lift.py.
lifted::Result f82132CE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82132CE0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // lis r29,1
  r29 = uint64_t(int64_t(65536));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // ori r29,r29,10568
  r29 = r29 | 10568;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r8,80(r31)
  guest::StoreU32(base, uint32_t(r31) + 80, uint32_t(r8));
  // lis r8,1
  r8 = uint64_t(int64_t(65536));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r6,16(r31)
  guest::StoreU32(base, uint32_t(r31) + 16, uint32_t(r6));
  // ori r8,r8,10580
  r8 = r8 | 10580;
  // stw r7,20(r31)
  guest::StoreU32(base, uint32_t(r31) + 20, uint32_t(r7));
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // stfsx f0,r31,r29
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r29), temp.u32);
  // lfs f13,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r9,84(r31)
  guest::StoreU32(base, uint32_t(r31) + 84, uint32_t(r9));
  // ori r3,r3,10564
  r3 = r3 | 10564;
  // stb r10,27(r31)
  guest::StoreU8(base, uint32_t(r31) + 27, uint8_t(r10));
  // lis r28,1
  r28 = uint64_t(int64_t(65536));
  // stfsx f13,r31,r8
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r8), temp.u32);
  // lis r6,1
  r6 = uint64_t(int64_t(65536));
  // lis r7,1
  r7 = uint64_t(int64_t(65536));
  // stb r30,24(r31)
  guest::StoreU8(base, uint32_t(r31) + 24, uint8_t(r30));
  // lfs f11,7140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f11 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r9,1
  r9 = uint64_t(int64_t(65536));
  // stwx r30,r31,r3
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r3), uint32_t(r30));
  // lis r29,1
  r29 = uint64_t(int64_t(65536));
  // stb r30,25(r31)
  guest::StoreU8(base, uint32_t(r31) + 25, uint8_t(r30));
  // lis r27,1
  r27 = uint64_t(int64_t(65536));
  // stb r30,26(r31)
  guest::StoreU8(base, uint32_t(r31) + 26, uint8_t(r30));
  // lis r8,1
  r8 = uint64_t(int64_t(65536));
  // stw r30,88(r31)
  guest::StoreU32(base, uint32_t(r31) + 88, uint32_t(r30));
  // lfs f10,10600(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10600);
  f10 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // stfs f10,40(r31)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r31) + 40, temp.u32);
  // ori r6,r6,10576
  r6 = r6 | 10576;
  // stw r4,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r4));
  // ori r7,r7,10584
  r7 = r7 | 10584;
  // stw r5,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r5));
  // ori r9,r9,62440
  r9 = r9 | 62440;
  // lfs f9,-30704(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -30704);
  f9 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // ori r8,r8,62452
  r8 = r8 | 62452;
  // stfs f9,44(r31)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r31) + 44, temp.u32);
  // ori r10,r10,62460
  r10 = r10 | 62460;
  // stwx r30,r31,r6
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r6), uint32_t(r30));
  // ori r28,r28,10572
  r28 = r28 | 10572;
  // stfsx f13,r31,r7
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r7), temp.u32);
  // ori r29,r29,62444
  r29 = r29 | 62444;
  // stwx r30,r31,r9
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r9), uint32_t(r30));
  // lfs f8,10772(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10772);
  f8 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // ori r27,r27,62448
  r27 = r27 | 62448;
  // stfs f8,48(r31)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r31) + 48, temp.u32);
  // mr r3,r31
  r3 = r31;
  // stfsx f0,r31,r10
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r10), temp.u32);
  // stbx r30,r31,r28
  guest::StoreU8(base, uint32_t(r31) + uint32_t(r28), uint8_t(r30));
  // stfsx f0,r31,r29
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r29), temp.u32);
  // stbx r30,r31,r8
  guest::StoreU8(base, uint32_t(r31) + uint32_t(r8), uint8_t(r30));
  // lfs f12,10804(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10804);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,64(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r31) + 64, temp.u32);
  // stwx r30,r31,r27
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r27), uint32_t(r30));
  // stfs f12,68(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r31) + 68, temp.u32);
  // lfs f7,10716(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10716);
  f7 = double(temp.f32);
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // stfs f7,72(r31)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r31) + 72, temp.u32);
  // ori r11,r11,60096
  r11 = r11 | 60096;
  // stwx r30,r31,r11
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r11), uint32_t(r30));
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r11,r11,62456
  r11 = r11 | 62456;
  // stfsx f11,r31,r11
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r11), temp.u32);
  // bl 0x8212f148
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenObject::BuildFan(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x821329b8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenObject::BuildRings(base, LIFTED_ARGS(0x82132E28, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32153
  r29 = uint64_t(int64_t(-2107179008));
  // lwz r3,-12152(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + -12152);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82132E40, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82132E50, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8258d8c8
  // (left out: dead in C++)
  { const lifted::Result o = game::VideoOutput::LockDevice(base, LIFTED_ARGS(0x82132E54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // lis r8,6184
  r8 = uint64_t(int64_t(405274624));
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // lwz r4,12(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 12);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // ori r8,r8,390
  r8 = r8 | 390;
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r11
  r3 = r11;
  // bl 0x822a5d08
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::CreateTexture(base, LIFTED_ARGS(0x82132E84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r3
  r10 = r3;
  // lis r11,44
  r11 = uint64_t(int64_t(2883584));
  // sth r30,96(r1)
  guest::StoreU16(base, uint32_t(r1) + 96, uint16_t(r30));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // sth r30,98(r1)
  guest::StoreU16(base, uint32_t(r1) + 98, uint16_t(r30));
  // ori r11,r11,9125
  r11 = r11 | 9125;
  // stb r30,104(r1)
  guest::StoreU8(base, uint32_t(r1) + 104, uint8_t(r30));
  // stb r30,105(r1)
  guest::StoreU8(base, uint32_t(r1) + 105, uint8_t(r30));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // stb r30,106(r1)
  guest::StoreU8(base, uint32_t(r1) + 106, uint8_t(r30));
  // sth r30,108(r1)
  guest::StoreU16(base, uint32_t(r1) + 108, uint16_t(r30));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // stw r11,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r11));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // sth r10,110(r1)
  guest::StoreU16(base, uint32_t(r1) + 110, uint16_t(r10));
  // lis r10,24
  r10 = uint64_t(int64_t(1572864));
  // stb r30,116(r1)
  guest::StoreU8(base, uint32_t(r1) + 116, uint8_t(r30));
  // ori r10,r10,10374
  r10 = r10 | 10374;
  // stb r30,118(r1)
  guest::StoreU8(base, uint32_t(r1) + 118, uint8_t(r30));
  // sth r30,120(r1)
  guest::StoreU16(base, uint32_t(r1) + 120, uint16_t(r30));
  // stb r11,129(r1)
  guest::StoreU8(base, uint32_t(r1) + 129, uint8_t(r11));
  // li r11,255
  r11 = uint64_t(int64_t(255));
  // stb r30,128(r1)
  guest::StoreU8(base, uint32_t(r1) + 128, uint8_t(r30));
  // stb r30,130(r1)
  guest::StoreU8(base, uint32_t(r1) + 130, uint8_t(r30));
  // stw r10,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r10));
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // sth r30,134(r1)
  guest::StoreU16(base, uint32_t(r1) + 134, uint16_t(r30));
  // sth r11,132(r1)
  guest::StoreU16(base, uint32_t(r1) + 132, uint16_t(r11));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stb r30,140(r1)
  guest::StoreU8(base, uint32_t(r1) + 140, uint8_t(r30));
  // stb r30,141(r1)
  guest::StoreU8(base, uint32_t(r1) + 141, uint8_t(r30));
  // stb r10,117(r1)
  guest::StoreU8(base, uint32_t(r1) + 117, uint8_t(r10));
  // li r10,12
  r10 = uint64_t(int64_t(12));
  // stb r30,142(r1)
  guest::StoreU8(base, uint32_t(r1) + 142, uint8_t(r30));
  // stw r11,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r11));
  // sth r10,122(r1)
  guest::StoreU16(base, uint32_t(r1) + 122, uint16_t(r10));
  // bl 0x822a8980
  // (left out: dead in C++)
  { const lifted::Result o = game::CreateVertexDeclaration(base, LIFTED_ARGS(0x82132F1C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r3));
  // lwz r3,-12152(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + -12152);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82132F34, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82132F44, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258d908
  // (left out: dead in C++)
  { const lifted::Result o = game::VideoOutput::UnlockDevice(base, LIFTED_ARGS(0x82132F4C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82133000 game::ScreenObject::Ctor: lifted from generated/ by lift.py.
lifted::Result f82133000(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82133000 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addis r3,r31,1
  r3 = uint64_t(int64_t(int64_t(r31) + 65536));
  // mr r30,r4
  r30 = r4;
  // addi r3,r3,10588
  r3 = uint64_t(int64_t(int64_t(r3) + 10588));
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // stw r11,88(r31)
  guest::StoreU32(base, uint32_t(r31) + 88, uint32_t(r11));
  // mr r27,r7
  r27 = r7;
  // stw r11,92(r31)
  guest::StoreU32(base, uint32_t(r31) + 92, uint32_t(r11));
  // mr r26,r8
  r26 = r8;
  // mr r25,r9
  r25 = r9;
  // bl 0x82130198
  // (left out: dead in C++)
  { const lifted::Result o = game::UvMap::Ctor(base, LIFTED_ARGS(0x82133040, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r9,r25
  r9 = r25;
  // mr r8,r26
  r8 = r26;
  // mr r7,r27
  r7 = r27;
  // mr r6,r28
  r6 = r28;
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82132ce0
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenObject::Init(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82133B10: lifted from generated/ by lift.py.
lifted::Result f82133B10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82133B10 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-30664
  r11 = uint64_t(int64_t(int64_t(r11) + -30664));
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // li r5,36
  r5 = uint64_t(int64_t(36));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82133B3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r10,r11,-30648
  r10 = uint64_t(int64_t(int64_t(r11) + -30648));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r4,6688
  r4 = uint64_t(int64_t(438304768));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r9,40(r31)
  guest::StoreU32(base, uint32_t(r31) + 40, uint32_t(r9));
  // ori r4,r4,346
  r4 = r4 | 346;
  // stw r10,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r10));
  // stw r11,44(r31)
  guest::StoreU32(base, uint32_t(r31) + 44, uint32_t(r11));
  // stw r11,48(r31)
  guest::StoreU32(base, uint32_t(r31) + 48, uint32_t(r11));
  // stw r11,52(r31)
  guest::StoreU32(base, uint32_t(r31) + 52, uint32_t(r11));
  // stw r11,8056(r31)
  guest::StoreU32(base, uint32_t(r31) + 8056, uint32_t(r11));
  // stw r11,8060(r31)
  guest::StoreU32(base, uint32_t(r31) + 8060, uint32_t(r11));
  // stw r11,8064(r31)
  guest::StoreU32(base, uint32_t(r31) + 8064, uint32_t(r11));
  // lwz r6,20(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 20);
  // lwz r5,16(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 16);
  // lwz r8,28(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 28);
  // lwz r7,24(r31)
  r7 = guest::LoadU32(base, uint32_t(r31) + 24);
  // bl 0x82133190
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133190, LIFTED_ARGS(0x82133B8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // stw r11,8064(r31)
  guest::StoreU32(base, uint32_t(r31) + 8064, uint32_t(r11));
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

// sub_82133BB0: lifted from generated/ by lift.py.
lifted::Result f82133BB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82133BB0 (lifted)");
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-30648
  r11 = uint64_t(int64_t(int64_t(r11) + -30648));
  // addi r3,r31,8064
  r3 = uint64_t(int64_t(int64_t(r31) + 8064));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x821332f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821332F0, LIFTED_ARGS(0x82133BE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,-30664
  r11 = uint64_t(int64_t(int64_t(r11) + -30664));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // clrlwi. r10,r30,31
  r10 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82133bfc
  if (cr0.eq) goto loc_82133BFC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82133BFC, r31)); r3 = o.r3; f1 = o.f1; }
loc_82133BFC:
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

// sub_82133C18: lifted from generated/ by lift.py.
lifted::Result f82133C18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82133C18 (lifted)");
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
  // addi r11,r11,-30664
  r11 = uint64_t(int64_t(int64_t(r11) + -30664));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, schema::VtableObject(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // beq 0x82133c44
  if (cr0.eq) goto loc_82133C44;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82133C44, r31)); r3 = o.r3; f1 = o.f1; }
loc_82133C44:
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

// sub_82134410: lifted from generated/ by lift.py.
lifted::Result f82134410(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82134410 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-30664
  r11 = uint64_t(int64_t(int64_t(r11) + -30664));
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // li r5,36
  r5 = uint64_t(int64_t(36));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82134438, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lfs f0,8112(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8112);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stw r30,44(r31)
  guest::StoreU32(base, uint32_t(r31) + 44, uint32_t(r30));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r10,40(r31)
  guest::StoreU32(base, uint32_t(r31) + 40, uint32_t(r10));
  // stw r30,48(r31)
  guest::StoreU32(base, uint32_t(r31) + 48, uint32_t(r30));
  // stw r30,52(r31)
  guest::StoreU32(base, uint32_t(r31) + 52, uint32_t(r30));
  // lfs f13,-32716(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32716);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r30,8056(r31)
  guest::StoreU32(base, uint32_t(r31) + 8056, uint32_t(r30));
  // stw r30,8060(r31)
  guest::StoreU32(base, uint32_t(r31) + 8060, uint32_t(r30));
  // lfs f12,12200(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 12200);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f11 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,-30640
  r11 = uint64_t(int64_t(int64_t(r11) + -30640));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // lis r11,6688
  r11 = uint64_t(int64_t(438304768));
  // stw r30,8064(r31)
  guest::StoreU32(base, uint32_t(r31) + 8064, uint32_t(r30));
  // stfs f0,8096(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 8096, temp.u32);
  // stw r30,8068(r31)
  guest::StoreU32(base, uint32_t(r31) + 8068, uint32_t(r30));
  // ori r29,r11,346
  r29 = r11 | 346;
  // stw r30,8072(r31)
  guest::StoreU32(base, uint32_t(r31) + 8072, uint32_t(r30));
  // stfs f13,8100(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 8100, temp.u32);
  // stw r30,8076(r31)
  guest::StoreU32(base, uint32_t(r31) + 8076, uint32_t(r30));
  // mr r4,r29
  r4 = r29;
  // stw r30,8080(r31)
  guest::StoreU32(base, uint32_t(r31) + 8080, uint32_t(r30));
  // stfs f12,8104(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r31) + 8104, temp.u32);
  // stw r30,8084(r31)
  guest::StoreU32(base, uint32_t(r31) + 8084, uint32_t(r30));
  // stfs f11,8108(r31)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r31) + 8108, temp.u32);
  // stw r30,8088(r31)
  guest::StoreU32(base, uint32_t(r31) + 8088, uint32_t(r30));
  // stw r30,8092(r31)
  guest::StoreU32(base, uint32_t(r31) + 8092, uint32_t(r30));
  // lwz r6,20(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 20);
  // lwz r5,16(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 16);
  // bl 0x82133190
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133190, LIFTED_ARGS(0x821344D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r6,20(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 20);
  // lis r4,6690
  r4 = uint64_t(int64_t(438435840));
  // lwz r5,16(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 16);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // ori r4,r4,407
  r4 = r4 | 407;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // stw r11,8064(r31)
  guest::StoreU32(base, uint32_t(r31) + 8064, uint32_t(r11));
  // bl 0x82133190
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133190, LIFTED_ARGS(0x82134500, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // lwz r10,16(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 16);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r3,8068(r31)
  guest::StoreU32(base, uint32_t(r31) + 8068, uint32_t(r3));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r4,r29
  r4 = r29;
  // rlwinm r6,r11,31,1,31
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 31) & 0x7FFFFFFF;
  // rlwinm r5,r10,31,1,31
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 31) & 0x7FFFFFFF;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x82133190
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133190, LIFTED_ARGS(0x82134528, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // lwz r10,16(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 16);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r3,8072(r31)
  guest::StoreU32(base, uint32_t(r31) + 8072, uint32_t(r3));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // rlwinm r6,r11,30,2,31
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // rlwinm r5,r10,30,2,31
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // mr r4,r29
  r4 = r29;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x82133190
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133190, LIFTED_ARGS(0x82134550, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // lwz r10,16(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 16);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r3,8076(r31)
  guest::StoreU32(base, uint32_t(r31) + 8076, uint32_t(r3));
  // rlwinm r6,r11,29,3,31
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x1FFFFFFF;
  // rlwinm r5,r10,29,3,31
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 29) & 0x1FFFFFFF;
  // mr r4,r29
  r4 = r29;
  // li r3,6
  r3 = uint64_t(int64_t(6));
  // bl 0x82133190
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133190, LIFTED_ARGS(0x82134578, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lis r8,6690
  r8 = uint64_t(int64_t(438435840));
  // stw r3,8080(r31)
  guest::StoreU32(base, uint32_t(r31) + 8080, uint32_t(r3));
  // lwz r10,20(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 20);
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r9,16(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 16);
  // ori r29,r8,447
  r29 = r8 | 447;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // std r30,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r30);
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r5,r29
  r5 = r29;
  // rlwinm r4,r10,31,1,31
  r4 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 31) & 0x7FFFFFFF;
  // rlwinm r3,r9,31,1,31
  r3 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 31) & 0x7FFFFFFF;
  // lwz r11,24000(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24000);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x822a5e28
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::CreateSurface(base, LIFTED_ARGS(0x821345BC, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,8084(r31)
  guest::StoreU32(base, uint32_t(r31) + 8084, uint32_t(r3));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r10,16(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 16);
  // mr r5,r29
  r5 = r29;
  // rlwinm r4,r11,30,2,31
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // rlwinm r3,r10,30,2,31
  r3 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // bl 0x822a5e28
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::CreateSurface(base, LIFTED_ARGS(0x821345E0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,8088(r31)
  guest::StoreU32(base, uint32_t(r31) + 8088, uint32_t(r3));
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r10,16(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 16);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // rlwinm r4,r11,29,3,31
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x1FFFFFFF;
  // rlwinm r3,r10,29,3,31
  r3 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 29) & 0x1FFFFFFF;
  // bl 0x822a5e28
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::CreateSurface(base, LIFTED_ARGS(0x82134604, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // stw r11,8092(r31)
  guest::StoreU32(base, uint32_t(r31) + 8092, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82134618: lifted from generated/ by lift.py.
lifted::Result f82134618(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82134618 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-30640
  r11 = uint64_t(int64_t(int64_t(r11) + -30640));
  // addi r3,r31,8092
  r3 = uint64_t(int64_t(int64_t(r31) + 8092));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82133c60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133C60, LIFTED_ARGS(0x82134640, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,8088
  r3 = uint64_t(int64_t(int64_t(r31) + 8088));
  // bl 0x82133c60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133C60, LIFTED_ARGS(0x82134648, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,8084
  r3 = uint64_t(int64_t(int64_t(r31) + 8084));
  // bl 0x82133c60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133C60, LIFTED_ARGS(0x82134650, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,8080
  r3 = uint64_t(int64_t(int64_t(r31) + 8080));
  // bl 0x821332f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821332F0, LIFTED_ARGS(0x82134658, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,8076
  r3 = uint64_t(int64_t(int64_t(r31) + 8076));
  // bl 0x821332f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821332F0, LIFTED_ARGS(0x82134660, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,8072
  r3 = uint64_t(int64_t(int64_t(r31) + 8072));
  // bl 0x821332f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821332F0, LIFTED_ARGS(0x82134668, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,8068
  r3 = uint64_t(int64_t(int64_t(r31) + 8068));
  // bl 0x821332f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821332F0, LIFTED_ARGS(0x82134670, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,8064
  r3 = uint64_t(int64_t(int64_t(r31) + 8064));
  // bl 0x821332f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821332F0, LIFTED_ARGS(0x82134678, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,-30664
  r11 = uint64_t(int64_t(int64_t(r11) + -30664));
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

// sub_821350E8: lifted from generated/ by lift.py.
void f821350E8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821350E8 (lifted)");
  // stw r4,8068(r3)
  guest::StoreU32(base, uint32_t(r3) + 8068, uint32_t(r4));
  // blr
  return;
}

// sub_821350F0: lifted from generated/ by lift.py.
void f821350F0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821350F0 (lifted)");
  // stw r4,8072(r3)
  guest::StoreU32(base, uint32_t(r3) + 8072, uint32_t(r4));
  // blr
  return;
}

// sub_82135118: lifted from generated/ by lift.py.
void f82135118(uint8_t* base, uint64_t r3, double f1, double f2, double f3) {
  ITC_NATIVE_RUNNING("sub_82135118 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f1,8096(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r3) + 8096, temp.u32);
  // stfs f2,8100(r3)
  temp.f32 = float(f2);
  guest::StoreU32(base, uint32_t(r3) + 8100, temp.u32);
  // stfs f3,8104(r3)
  temp.f32 = float(f3);
  guest::StoreU32(base, uint32_t(r3) + 8104, temp.u32);
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,8108(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r3) + 8108, temp.u32);
  // blr
  return;
}

// sub_82135150: lifted from generated/ by lift.py.
void f82135150(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82135150 (lifted)");
  double f0 = 0.0;
  lifted::Temp temp{};
  // lfs f0,8080(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8080);
  f0 = double(temp.f32);
  // stfs f0,0(r4)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r4) + 0, temp.u32);
  // lfs f0,8084(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8084);
  f0 = double(temp.f32);
  // stfs f0,0(r5)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r5) + 0, temp.u32);
  // lfs f0,8088(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8088);
  f0 = double(temp.f32);
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 0, temp.u32);
  // blr
  return;
}

// sub_82135170: lifted from generated/ by lift.py.
void f82135170(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82135170 (lifted)");
  double f0 = 0.0;
  lifted::Temp temp{};
  // lfs f0,8096(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8096);
  f0 = double(temp.f32);
  // stfs f0,0(r4)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r4) + 0, temp.u32);
  // lfs f0,8100(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8100);
  f0 = double(temp.f32);
  // stfs f0,0(r5)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r5) + 0, temp.u32);
  // lfs f0,8104(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8104);
  f0 = double(temp.f32);
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 0, temp.u32);
  // blr
  return;
}

// sub_82135190: lifted from generated/ by lift.py.
double f82135190(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82135190 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8112(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8112);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82135198: lifted from generated/ by lift.py.
double f82135198(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82135198 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8116(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8116);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_821351A0: lifted from generated/ by lift.py.
double f821351A0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821351A0 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8120(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8120);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_821356B0: lifted from generated/ by lift.py.
lifted::Result f821356B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_821356B0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
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
  // lis r25,-32148
  r25 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // lwz r3,-21600(r25)
  r3 = guest::LoadU32(base, uint32_t(r25) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821356D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821356E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // bl 0x8258c9d0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::OpenChunk(base, LIFTED_ARGS(0x821356F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x821356FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // sth r11,2(r27)
  guest::StoreU16(base, uint32_t(r27) + 2, uint16_t(r11));
  // mr r4,r31
  r4 = r31;
  // stw r30,4(r27)
  guest::StoreU32(base, uint32_t(r27) + 4, uint32_t(r30));
  // mr r3,r28
  r3 = r28;
  // lwz r11,8064(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 8064);
  // lwz r5,24(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 24);
  // bl 0x82133f58
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffect::RecordResolve(base, LIFTED_ARGS(0x82135730, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x8213573C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,29
  r10 = uint64_t(int64_t(29));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135760, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,143
  r10 = uint64_t(int64_t(143));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135778, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135798, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // mr r4,r31
  r4 = r31;
  // mr r3,r28
  r3 = r28;
  // lwz r10,23996(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 23996);
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x82133340
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffect::RecordBeginStates(base, LIFTED_ARGS(0x821357B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x821357C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x821357E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // lwz r10,8064(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 8064);
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24);
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135800, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r26,2
  r26 = uint64_t(int64_t(2));
  // li r24,3
  r24 = uint64_t(int64_t(3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // stw r26,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r26));
  // stw r24,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r24));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135824, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r29,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r29));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135844, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r29,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r29));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135864, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,24
  r10 = uint64_t(int64_t(24));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r26,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r26));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135884, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r29));
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x821358A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,60
  r10 = uint64_t(int64_t(60));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x821358BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r24,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r24));
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x821358D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // stw r30,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r30));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x821358F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r23,8
  r23 = uint64_t(int64_t(8));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // stw r23,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r23));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135914, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // li r3,7
  r3 = uint64_t(int64_t(7));
  // bl 0x825896d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825896D8(base, LIFTED_ARGS(0x82135920, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r11));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135934, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135954, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // li r3,7
  r3 = uint64_t(int64_t(7));
  // bl 0x82589700
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82589700(base, LIFTED_ARGS(0x82135960, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r26)
  guest::StoreU32(base, uint32_t(r26) + 0, uint32_t(r3));
  // lis r26,-32153
  r26 = uint64_t(int64_t(-2107179008));
  // lwz r3,-12152(r26)
  r3 = guest::LoadU32(base, uint32_t(r26) + -12152);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8213597C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8213598C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,-12152(r26)
  r3 = guest::LoadU32(base, uint32_t(r26) + -12152);
  // lwz r26,300(r11)
  r26 = guest::LoadU32(base, uint32_t(r11) + 300);
  // lwz r10,0(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r10,28(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 28);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821359A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821359B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r5,r26
  r5 = r26;
  // mr r3,r28
  r3 = r28;
  // lwz r4,296(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 296);
  // lfs f1,-30676(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -30676);
  f1 = double(temp.f32);
  // bl 0x82133e30
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffect::AddQuad(base, LIFTED_ARGS(0x821359D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x821359E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,19
  r10 = uint64_t(int64_t(19));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r29));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135A08, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,40
  r10 = uint64_t(int64_t(40));
  // mr r4,r31
  r4 = r31;
  // mr r3,r28
  r3 = r28;
  // stw r23,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r23));
  // sth r24,4(r11)
  guest::StoreU16(base, uint32_t(r11) + 4, uint16_t(r24));
  // sth r10,6(r11)
  guest::StoreU16(base, uint32_t(r11) + 6, uint16_t(r10));
  // stw r26,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r26));
  // bl 0x82133608
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffect::RecordEndStates(base, LIFTED_ARGS(0x82135A2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::AllocateSlots(base, LIFTED_ARGS(0x82135A38, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,36
  r10 = uint64_t(int64_t(36));
  // mr r3,r31
  r3 = r31;
  // stw r30,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258ca50
  // (left out: dead in C++)
  { const lifted::Result o = game::CommandList::CloseChunk(base, LIFTED_ARGS(0x82135A54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-21600(r25)
  r3 = guest::LoadU32(base, uint32_t(r25) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82135A68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82135A78, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r5,8(r28)
  r5 = guest::LoadU32(base, uint32_t(r28) + 8);
  // mr r4,r27
  r4 = r27;
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82135A98, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,40(r28)
  guest::StoreU32(base, uint32_t(r28) + 40, uint32_t(r30));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

lifted::Result e82118638(uint8_t* base, const lifted::Args& a) {
  return f82118638(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82118790(uint8_t* base, const lifted::Args& a) {
  return f82118790(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.f[0]);
}

lifted::Result e82118850(uint8_t* base, const lifted::Args& a) {
  return f82118850(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82118930(uint8_t* base, const lifted::Args& a) {
  return f82118930(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821189F0(uint8_t* base, const lifted::Args& a) {
  return f821189F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82118A90(uint8_t* base, const lifted::Args& a) {
  return f82118A90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82118C70(uint8_t* base, const lifted::Args& a) {
  return f82118C70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82118CF0(uint8_t* base, const lifted::Args& a) {
  return {f82118CF0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82118D90(uint8_t* base, const lifted::Args& a) {
  return {f82118D90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82118DD0(uint8_t* base, const lifted::Args& a) {
  return {f82118DD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82118E10(uint8_t* base, const lifted::Args& a) {
  return {f82118E10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82118E50(uint8_t* base, const lifted::Args& a) {
  return {f82118E50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e82118EA0(uint8_t* base, const lifted::Args& a) {
  return {f82118EA0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82118ED0(uint8_t* base, const lifted::Args& a) {
  return f82118ED0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82118F00(uint8_t* base, const lifted::Args& a) {
  return f82118F00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82118F50(uint8_t* base, const lifted::Args& a) {
  return f82118F50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82119018(uint8_t* base, const lifted::Args& a) {
  return f82119018(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e821190B0(uint8_t* base, const lifted::Args& a) {
  return f821190B0(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82119118(uint8_t* base, const lifted::Args& a) {
  return f82119118(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82119188(uint8_t* base, const lifted::Args& a) {
  return f82119188(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82119278(uint8_t* base, const lifted::Args& a) {
  return f82119278(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82119390(uint8_t* base, const lifted::Args& a) {
  return f82119390(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e821193A8(uint8_t* base, const lifted::Args& a) {
  return {f821193A8(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e821193B0(uint8_t* base, const lifted::Args& a) {
  return f821193B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821193B8(uint8_t* base, const lifted::Args& a) {
  return f821193B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82119450(uint8_t* base, const lifted::Args& a) {
  return f82119450(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82119500(uint8_t* base, const lifted::Args& a) {
  return f82119500(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82119608(uint8_t* base, const lifted::Args& a) {
  return f82119608(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[6], a.f[0]);
}

lifted::Result e82119638(uint8_t* base, const lifted::Args& a) {
  return f82119638(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[6], a.f[0]);
}

lifted::Result e82119690(uint8_t* base, const lifted::Args& a) {
  return f82119690(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8211C2C8(uint8_t* base, const lifted::Args& a) {
  return f8211C2C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8211E8A8(uint8_t* base, const lifted::Args& a) {
  return f8211E8A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.lr);
}

lifted::Result e8211F0C0(uint8_t* base, const lifted::Args& a) {
  return f8211F0C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8211F178(uint8_t* base, const lifted::Args& a) {
  return f8211F178(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e8211F1C8(uint8_t* base, const lifted::Args& a) {
  return {f8211F1C8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82120868(uint8_t* base, const lifted::Args& a) {
  return f82120868(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82120BB8(uint8_t* base, const lifted::Args& a) {
  return f82120BB8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82121280(uint8_t* base, const lifted::Args& a) {
  return f82121280(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82121590(uint8_t* base, const lifted::Args& a) {
  f82121590(base, a.r[0], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82121CC0(uint8_t* base, const lifted::Args& a) {
  return f82121CC0(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82122900(uint8_t* base, const lifted::Args& a) {
  return f82122900(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82122958(uint8_t* base, const lifted::Args& a) {
  return f82122958(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82122DB8(uint8_t* base, const lifted::Args& a) {
  return {f82122DB8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]), a.f[0]};
}

lifted::Result e82123350(uint8_t* base, const lifted::Args& a) {
  return f82123350(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e82123450(uint8_t* base, const lifted::Args& a) {
  return f82123450(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82123880(uint8_t* base, const lifted::Args& a) {
  return f82123880(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e821253F0(uint8_t* base, const lifted::Args& a) {
  return f821253F0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e82125610(uint8_t* base, const lifted::Args& a) {
  return f82125610(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821256B8(uint8_t* base, const lifted::Args& a) {
  return f821256B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82125888(uint8_t* base, const lifted::Args& a) {
  return f82125888(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e821258F8(uint8_t* base, const lifted::Args& a) {
  return f821258F8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82125970(uint8_t* base, const lifted::Args& a) {
  return f82125970(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82126158(uint8_t* base, const lifted::Args& a) {
  return f82126158(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821261C0(uint8_t* base, const lifted::Args& a) {
  return f821261C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82126220(uint8_t* base, const lifted::Args& a) {
  return f82126220(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82126570(uint8_t* base, const lifted::Args& a) {
  return f82126570(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82126660(uint8_t* base, const lifted::Args& a) {
  return f82126660(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82126818(uint8_t* base, const lifted::Args& a) {
  return f82126818(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82126A00(uint8_t* base, const lifted::Args& a) {
  return {f82126A00(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82126A38(uint8_t* base, const lifted::Args& a) {
  return {f82126A38(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82126A90(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82126A90(base, a.r[0])};
}

lifted::Result e82126D30(uint8_t* base, const lifted::Args& a) {
  return {f82126D30(base, a.r[0]), a.f[0]};
}

lifted::Result e82126ED0(uint8_t* base, const lifted::Args& a) {
  return f82126ED0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82127188(uint8_t* base, const lifted::Args& a) {
  return {f82127188(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82127198(uint8_t* base, const lifted::Args& a) {
  return {f82127198(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82129298(uint8_t* base, const lifted::Args& a) {
  return f82129298(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e821292B0(uint8_t* base, const lifted::Args& a) {
  return f821292B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e821292C8(uint8_t* base, const lifted::Args& a) {
  return f821292C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.lr);
}

lifted::Result e8212A1C0(uint8_t* base, const lifted::Args& a) {
  return f8212A1C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8212B3A8(uint8_t* base, const lifted::Args& a) {
  return f8212B3A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.lr);
}

lifted::Result e8212C0C0(uint8_t* base, const lifted::Args& a) {
  return f8212C0C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.lr);
}

lifted::Result e8212CB70(uint8_t* base, const lifted::Args& a) {
  return f8212CB70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8212CBC8(uint8_t* base, const lifted::Args& a) {
  return f8212CBC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.lr);
}

lifted::Result e8212D680(uint8_t* base, const lifted::Args& a) {
  return f8212D680(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8212EA40(uint8_t* base, const lifted::Args& a) {
  return f8212EA40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8212EDF8(uint8_t* base, const lifted::Args& a) {
  return f8212EDF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8212EED0(uint8_t* base, const lifted::Args& a) {
  f8212EED0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8212EEF8(uint8_t* base, const lifted::Args& a) {
  f8212EEF8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8212F538(uint8_t* base, const lifted::Args& a) {
  return f8212F538(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8212F600(uint8_t* base, const lifted::Args& a) {
  return f8212F600(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8212F778(uint8_t* base, const lifted::Args& a) {
  return f8212F778(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82130330(uint8_t* base, const lifted::Args& a) {
  return f82130330(base, a.r1, a.r13, a.r[0], a.f[0], a.f[1], a.f[2], a.f[3]);
}

lifted::Result e82130490(uint8_t* base, const lifted::Args& a) {
  return f82130490(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.f[0], a.f[1]);
}

lifted::Result e82130618(uint8_t* base, const lifted::Args& a) {
  return f82130618(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e82132CE0(uint8_t* base, const lifted::Args& a) {
  return f82132CE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82133000(uint8_t* base, const lifted::Args& a) {
  return f82133000(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82133B10(uint8_t* base, const lifted::Args& a) {
  return f82133B10(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e82133BB0(uint8_t* base, const lifted::Args& a) {
  return f82133BB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82133C18(uint8_t* base, const lifted::Args& a) {
  return f82133C18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82134410(uint8_t* base, const lifted::Args& a) {
  return f82134410(base, a.r1, a.r13, a.r[0], a.r[1], a.r[6], a.f[0]);
}

lifted::Result e82134618(uint8_t* base, const lifted::Args& a) {
  return f82134618(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e821350E8(uint8_t* base, const lifted::Args& a) {
  f821350E8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821350F0(uint8_t* base, const lifted::Args& a) {
  f821350F0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82135118(uint8_t* base, const lifted::Args& a) {
  f82135118(base, a.r[0], a.f[0], a.f[1], a.f[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82135150(uint8_t* base, const lifted::Args& a) {
  f82135150(base, a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82135170(uint8_t* base, const lifted::Args& a) {
  f82135170(base, a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82135190(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82135190(base, a.r[0])};
}

lifted::Result e82135198(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82135198(base, a.r[0])};
}

lifted::Result e821351A0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f821351A0(base, a.r[0])};
}

lifted::Result e821356B0(uint8_t* base, const lifted::Args& a) {
  return f821356B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

namespace {

const Registration kEntries[] = {
    {0x82118638, e82118638},
    {0x82118790, e82118790},
    {0x82118850, e82118850},
    {0x82118930, e82118930},
    {0x821189F0, e821189F0},
    {0x82118A90, e82118A90},
    {0x82118C70, e82118C70},
    {0x82118CF0, e82118CF0},
    {0x82118D90, e82118D90},
    {0x82118DD0, e82118DD0},
    {0x82118E10, e82118E10},
    {0x82118E50, e82118E50},
    {0x82118EA0, e82118EA0},
    {0x82118ED0, e82118ED0},
    {0x82118F00, e82118F00},
    {0x82118F50, e82118F50},
    {0x82119018, e82119018},
    {0x821190B0, e821190B0},
    {0x82119118, e82119118},
    {0x82119188, e82119188},
    {0x82119278, e82119278},
    {0x82119390, e82119390},
    {0x821193A8, e821193A8},
    {0x821193B0, e821193B0},
    {0x821193B8, e821193B8},
    {0x82119450, e82119450},
    {0x82119500, e82119500},
    {0x82119608, e82119608},
    {0x82119638, e82119638},
    {0x82119690, e82119690},
    {0x8211C2C8, e8211C2C8},
    {0x8211E8A8, e8211E8A8},
    {0x8211F0C0, e8211F0C0},
    {0x8211F178, e8211F178},
    {0x8211F1C8, e8211F1C8},
    {0x82120868, e82120868},
    {0x82120BB8, e82120BB8},
    {0x82121280, e82121280},
    {0x82121590, e82121590},
    {0x82121CC0, e82121CC0},
    {0x82122900, e82122900},
    {0x82122958, e82122958},
    {0x82122DB8, e82122DB8},
    {0x82123350, e82123350},
    {0x82123450, e82123450},
    {0x82123880, e82123880},
    {0x821253F0, e821253F0},
    {0x82125610, e82125610},
    {0x821256B8, e821256B8},
    {0x82125888, e82125888},
    {0x821258F8, e821258F8},
    {0x82125970, e82125970},
    {0x82126158, e82126158},
    {0x821261C0, e821261C0},
    {0x82126220, e82126220},
    {0x82126570, e82126570},
    {0x82126660, e82126660},
    {0x82126818, e82126818},
    {0x82126A00, e82126A00},
    {0x82126A38, e82126A38},
    {0x82126A90, e82126A90},
    {0x82126D30, e82126D30},
    {0x82126ED0, e82126ED0},
    {0x82127188, e82127188},
    {0x82127198, e82127198},
    {0x82129298, e82129298},
    {0x821292B0, e821292B0},
    {0x821292C8, e821292C8},
    {0x8212A1C0, e8212A1C0},
    {0x8212B3A8, e8212B3A8},
    {0x8212C0C0, e8212C0C0},
    {0x8212CB70, e8212CB70},
    {0x8212CBC8, e8212CBC8},
    {0x8212D680, e8212D680},
    {0x8212EA40, e8212EA40},
    {0x8212EDF8, e8212EDF8},
    {0x8212EED0, e8212EED0},
    {0x8212EEF8, e8212EEF8},
    {0x8212F538, e8212F538},
    {0x8212F600, e8212F600},
    {0x8212F778, e8212F778},
    {0x82130330, e82130330},
    {0x82130490, e82130490},
    {0x82130618, e82130618},
    {0x82132CE0, e82132CE0},
    {0x82133000, e82133000},
    {0x82133B10, e82133B10},
    {0x82133BB0, e82133BB0},
    {0x82133C18, e82133C18},
    {0x82134410, e82134410},
    {0x82134618, e82134618},
    {0x821350E8, e821350E8},
    {0x821350F0, e821350F0},
    {0x82135118, e82135118},
    {0x82135150, e82135150},
    {0x82135170, e82135170},
    {0x82135190, e82135190},
    {0x82135198, e82135198},
    {0x821351A0, e821351A0},
    {0x821356B0, e821356B0},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_006)), true);

}  // namespace

}  // namespace lifted
