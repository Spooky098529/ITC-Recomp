// Lift sweep batch 024 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_024, true, "ITC",
              "Run lift sweep batch 024's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_825CC3F8: lifted from generated/ by lift.py.
lifted::Result f825CC3F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CC3F8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // addi r29,r11,144
  r29 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CC41C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,236(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 236);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
  // lwz r10,252(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 252);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825cc444
  if (cr6.eq) goto loc_825CC444;
  // ori r11,r11,2
  r11 = r11 | 2;
  // b 0x825cc454
  goto loc_825CC454;
loc_825CC444:
  // lwz r10,256(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 256);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825cc458
  if (cr6.eq) goto loc_825CC458;
  // ori r11,r11,4
  r11 = r11 | 4;
loc_825CC454:
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_825CC458:
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CC460, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CC470 game::XACTWaveBank::Destroy: lifted from generated/ by lift.py.
lifted::Result f825CC470(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CC470 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r3
  r30 = r3;
  // lwz r11,20(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 20);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CC490, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,20(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 20);
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = game::XACTEngine::CheckNotInCallback(base, r1, r13, r3);
  // mr r27,r3
  r27 = r3;
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // blt cr6,0x825cc54c
  if (cr6.lt) goto loc_825CC54C;
  // lwz r11,236(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 236);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825cc500
  if (!cr6.eq) goto loc_825CC500;
  // addi r29,r30,24
  r29 = uint64_t(int64_t(int64_t(r30) + 24));
  // addi r31,r30,4
  r31 = uint64_t(int64_t(int64_t(r30) + 4));
loc_825CC4C0:
  // lwz r11,204(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cc4d4
  if (cr6.eq) goto loc_825CC4D4;
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 0);
  // b 0x825cc4d8
  goto loc_825CC4D8;
loc_825CC4D4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825CC4D8:
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CC4EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,236(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 236);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cc4c0
  if (cr6.eq) goto loc_825CC4C0;
loc_825CC500:
  // lwz r3,244(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 244);
  // addi r29,r30,240
  r29 = uint64_t(int64_t(int64_t(r30) + 240));
  // cmplw cr6,r3,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r29), so);
  // beq cr6,0x825cc534
  if (cr6.eq) goto loc_825CC534;
loc_825CC510:
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r31,4(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 4);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CC528, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // bne cr6,0x825cc510
  if (!cr6.eq) goto loc_825CC510;
loc_825CC534:
  // addi r3,r30,8
  r3 = uint64_t(int64_t(int64_t(r30) + 8));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CC54C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CC54C:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CC554, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CC560 game::XACTWaveBank::DoWork: lifted from generated/ by lift.py.
lifted::Result f825CC560(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CC560 (lifted)");
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
  // addi r30,r3,240
  r30 = uint64_t(int64_t(int64_t(r3) + 240));
  // lwz r3,244(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 244);
  // cmplw cr6,r3,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r30), so);
  // beq cr6,0x825cc5a4
  if (cr6.eq) goto loc_825CC5A4;
loc_825CC584:
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r31,4(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 4);
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CC598, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825cc584
  if (!cr6.eq) goto loc_825CC584;
loc_825CC5A4:
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

// sub_825CC5C0 game::XACTWaveBank::AudioUpdate: lifted from generated/ by lift.py.
lifted::Result f825CC5C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825CC5C0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r30,r3,240
  r30 = uint64_t(int64_t(int64_t(r3) + 240));
  // lwz r3,244(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 244);
  // cmplw cr6,r3,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r30), so);
  // beq cr6,0x825cc5f8
  if (cr6.eq) goto loc_825CC5F8;
loc_825CC5E4:
  // lwz r31,4(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 4);
  // bl 0x825d2560
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWave::AudioUpdate(base, LIFTED_ARGS(0x825CC5EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825cc5e4
  if (!cr6.eq) goto loc_825CC5E4;
loc_825CC5F8:
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

// sub_825CC610 game::XACTWaveBank::SetPrepared: lifted from generated/ by lift.py.
lifted::Result f825CC610(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_825CC610 (lifted)");
  uint64_t r4 = 0, r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // stw r11,252(r3)
  guest::StoreU32(base, uint32_t(r3) + 252, uint32_t(r11));
  // b 0x825c8710
  { const lifted::Result o = game::XACTWaveBank::Notify(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CC6C8: lifted from generated/ by lift.py.
lifted::Result f825CC6C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CC6C8 (lifted)");
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x825cc7b8
  { const lifted::Result o = f825CC7B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CC6D0 game::XACTWaveBankRoot::Dtor: lifted from generated/ by lift.py.
lifted::Result f825CC6D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825CC6D0 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // addi r30,r31,8
  r30 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r11,r11,24612
  r11 = uint64_t(int64_t(int64_t(r11) + 24612));
  // addi r10,r10,24604
  r10 = uint64_t(int64_t(int64_t(r10) + 24604));
  // addi r9,r9,24600
  r9 = uint64_t(int64_t(int64_t(r9) + 24600));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r9));
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // beq cr6,0x825cc740
  if (cr6.eq) goto loc_825CC740;
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // beq cr6,0x825cc740
  if (cr6.eq) goto loc_825CC740;
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 4);
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 8);
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 4);
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r30,4(r30)
  guest::StoreU32(base, uint32_t(r30) + 4, uint32_t(r30));
  // stw r30,8(r30)
  guest::StoreU32(base, uint32_t(r30) + 8, uint32_t(r30));
loc_825CC740:
  // mr r4,r31
  r4 = r31;
  // lwz r3,20(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20);
  // bl 0x825c0408
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C0408(base, LIFTED_ARGS(0x825CC74C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c8710
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWaveBank::Notify(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r31,24
  r3 = uint64_t(int64_t(int64_t(r31) + 24));
  // addi r29,r11,21316
  r29 = uint64_t(int64_t(int64_t(r11) + 21316));
  // stw r29,240(r31)
  guest::StoreU32(base, uint32_t(r31) + 240, uint32_t(r29));
  // lwz r11,204(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cc798
  if (cr6.eq) goto loc_825CC798;
loc_825CC774:
  // lwz r11,204(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 204);
  // lwz r9,0(r3)
  r9 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r9,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r9));
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r10,204(r3)
  guest::StoreU32(base, uint32_t(r3) + 204, uint32_t(r10));
  // bne cr6,0x825cc774
  if (!cr6.eq) goto loc_825CC774;
loc_825CC798:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,212(r3)
  guest::StoreU32(base, uint32_t(r3) + 212, uint32_t(r11));
  // stw r11,208(r3)
  guest::StoreU32(base, uint32_t(r3) + 208, uint32_t(r11));
  // bl 0x825c1f30
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTNodePool::FreeChunks(base, LIFTED_ARGS(0x825CC7A8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r29));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CC7B8: lifted from generated/ by lift.py.
lifted::Result f825CC7B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CC7B8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // bl 0x825cc6d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWaveBankRoot::Dtor(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cc7f8
  if (cr6.eq) goto loc_825CC7F8;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32865
  r4 = r4 | 32865;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x825CC7F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825CC7F8:
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

// sub_825CC810 game::XACTComplexSound::Ctor: lifted from generated/ by lift.py.
lifted::Result f825CC810(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_825CC810 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x825cb958
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Ctor(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,24672
  r11 = uint64_t(int64_t(int64_t(r11) + 24672));
  // addi r10,r10,24544
  r10 = uint64_t(int64_t(int64_t(r10) + 24544));
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
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

// sub_825CC8F0: lifted from generated/ by lift.py.
uint64_t f825CC8F0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CC8F0 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,28(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 28);
  // lhz r3,10(r11)
  r3 = guest::LoadU16(base, uint32_t(r11) + 10);
  // blr
  return r3;
}

// sub_825CC900: lifted from generated/ by lift.py.
lifted::Result f825CC900(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CC900 (lifted)");
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
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r31,28(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 28);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CC924, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,0(r3)
  r11 = guest::LoadU16(base, uint32_t(r3) + 0);
  // add r11,r11,r31
  r11 = r11 + r31;
  // lhz r3,10(r11)
  r3 = guest::LoadU16(base, uint32_t(r11) + 10);
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

// sub_825CC948: lifted from generated/ by lift.py.
uint64_t f825CC948(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CC948 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,28(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 28);
  // addi r3,r11,10
  r3 = uint64_t(int64_t(int64_t(r11) + 10));
  // blr
  return r3;
}

// sub_825CC958 game::XACTComplexSound::DeletingDestructor: lifted from generated/ by lift.py.
lifted::Result f825CC958(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CC958 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,24672
  r11 = uint64_t(int64_t(int64_t(r11) + 24672));
  // addi r10,r10,24544
  r10 = uint64_t(int64_t(int64_t(r10) + 24544));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // bl 0x825cae90
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Dtor(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cc9b0
  if (cr6.eq) goto loc_825CC9B0;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32789
  r4 = r4 | 32789;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x825CC9AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825CC9B0:
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

// sub_825CCA88 game::XACTComplexSound::Prepare: lifted from generated/ by lift.py.
lifted::Result f825CCA88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CCA88 (lifted)");
  uint64_t r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r4
  r27 = r4;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r29,r3
  r29 = r3;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r22,1
  r22 = uint64_t(int64_t(1));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-30009
  r11 = uint64_t(int64_t(-1966669824));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // ori r23,r11,25
  r23 = r11 | 25;
loc_825CCAC0:
  // lwz r11,28(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 28);
  // clrlwi r25,r31,24
  r25 = uint32_t(r31) & 0xFF;
  // lbz r11,9(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 9);
  // cmplw cr6,r25,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), uint32_t(r11), so);
  // bge cr6,0x825ccbe8
  if (!cr6.lt) goto loc_825CCBE8;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,64
  r3 = uint64_t(int64_t(64));
  // ori r4,r4,32772
  r4 = r4 | 32772;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CCAE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825ccb74
  if (cr6.eq) goto loc_825CCB74;
  // clrlwi r4,r31,24
  r4 = uint32_t(r31) & 0xFF;
  // mr r3,r29
  r3 = r29;
  // bl 0x825cc9c8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTComplexSound::GetTrackRecord(base, LIFTED_ARGS(0x825CCAFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // mr r7,r31
  r7 = r31;
  // lwz r4,32(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 32);
  // mr r5,r29
  r5 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x825d0ce0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::Ctor(base, LIFTED_ARGS(0x825CCB14, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x825ccb7c
  if (!cr6.eq) goto loc_825CCB7C;
loc_825CCB20:
  // lis r28,-32761
  r28 = uint64_t(int64_t(-2147024896));
  // ori r28,r28,14
  r28 = r28 | 14;
loc_825CCB28:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825ccb48
  if (cr6.eq) goto loc_825CCB48;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CCB48, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CCB48:
  // cmpw cr6,r28,r23
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r23), so);
  // bne cr6,0x825ccb58
  if (!cr6.eq) goto loc_825CCB58;
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // li r28,0
  r28 = uint64_t(int64_t(0));
loc_825CCB58:
  // addi r11,r25,1
  r11 = uint64_t(int64_t(int64_t(r25) + 1));
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // bge cr6,0x825ccac0
  if (!cr6.lt) goto loc_825CCAC0;
loc_825CCB68:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
loc_825CCB74:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x825ccb20
  goto loc_825CCB20;
loc_825CCB7C:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825ccb28
  if (cr6.lt) goto loc_825CCB28;
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 16);
  // mr r5,r27
  r5 = r27;
  // mr r3,r31
  r3 = r31;
  // lwz r4,1(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 1);
  // bl 0x825d0aa0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::Prepare(base, LIFTED_ARGS(0x825CCB98, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825ccb28
  if (cr6.lt) goto loc_825CCB28;
  // stw r27,104(r29)
  guest::StoreU32(base, uint32_t(r29) + 104, uint32_t(r27));
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ccbc4
  if (cr6.eq) goto loc_825CCBC4;
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x825ccbc8
  if (cr6.eq) goto loc_825CCBC8;
loc_825CCBC4:
  // li r22,0
  r22 = uint64_t(int64_t(0));
loc_825CCBC8:
  // addi r11,r29,36
  r11 = uint64_t(int64_t(int64_t(r29) + 36));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // stw r10,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // stw r31,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r31));
  // b 0x825ccb58
  goto loc_825CCB58;
loc_825CCBE8:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825ccb68
  if (cr6.lt) goto loc_825CCB68;
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // bne cr6,0x825ccc0c
  if (!cr6.eq) goto loc_825CCC0C;
  // cmpwi cr6,r24,0
  cr6 = lifted::Compare<int32_t>(int32_t(r24), 0, so);
  // beq cr6,0x825ccc0c
  if (cr6.eq) goto loc_825CCC0C;
  // mr r3,r23
  r3 = r23;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
loc_825CCC0C:
  // mr r3,r29
  r3 = r29;
  // bl 0x825cb470
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetUpRpcs(base, LIFTED_ARGS(0x825CCC14, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825ccc34
  if (cr6.lt) goto loc_825CCC34;
  // cmpwi cr6,r22,0
  cr6 = lifted::Compare<int32_t>(int32_t(r22), 0, so);
  // beq cr6,0x825ccc34
  if (cr6.eq) goto loc_825CCC34;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r29
  r3 = r29;
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CCC34:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CCC40 game::XACTSimpleSound::Ctor: lifted from generated/ by lift.py.
lifted::Result f825CCC40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_825CCC40 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x825cb958
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Ctor(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,24700
  r11 = uint64_t(int64_t(int64_t(r11) + 24700));
  // addi r10,r10,24544
  r10 = uint64_t(int64_t(int64_t(r10) + 24544));
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
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

// sub_825CCC88: lifted from generated/ by lift.py.
uint64_t f825CCC88(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CCC88 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,28(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 28);
  // lhz r3,12(r11)
  r3 = guest::LoadU16(base, uint32_t(r11) + 12);
  // blr
  return r3;
}

// sub_825CCC98: lifted from generated/ by lift.py.
lifted::Result f825CCC98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CCC98 (lifted)");
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
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r31,28(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 28);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CCCBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,0(r3)
  r11 = guest::LoadU16(base, uint32_t(r3) + 0);
  // add r11,r11,r31
  r11 = r11 + r31;
  // lhz r3,12(r11)
  r3 = guest::LoadU16(base, uint32_t(r11) + 12);
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

// sub_825CCCE0: lifted from generated/ by lift.py.
uint64_t f825CCCE0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CCCE0 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,28(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 28);
  // addi r3,r11,12
  r3 = uint64_t(int64_t(int64_t(r11) + 12));
  // blr
  return r3;
}

// sub_825CCCF0 game::XACTSimpleSound::DeletingDestructor: lifted from generated/ by lift.py.
lifted::Result f825CCCF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CCCF0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,24700
  r11 = uint64_t(int64_t(int64_t(r11) + 24700));
  // addi r10,r10,24544
  r10 = uint64_t(int64_t(int64_t(r10) + 24544));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // bl 0x825cae90
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Dtor(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ccd48
  if (cr6.eq) goto loc_825CCD48;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32790
  r4 = r4 | 32790;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x825CCD44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825CCD48:
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

// sub_825CCD60: lifted from generated/ by lift.py.
lifted::Result f825CCD60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_825CCD60 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,40(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 40);
  // lwz r10,100(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 100);
  // lwz r9,104(r3)
  r9 = guest::LoadU32(base, uint32_t(r3) + 104);
  // subf r31,r9,r10
  r31 = r10 - r9;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24);
  // lwz r3,36(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 36);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CCD98, r31)); r3 = o.r3; f1 = o.f1; }
  // add r3,r31,r3
  r3 = r31 + r3;
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

// sub_825CCDB0 game::XACTSimpleSound::Prepare: lifted from generated/ by lift.py.
lifted::Result f825CCDB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CCDB0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r28,r4
  r28 = r4;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r30,r3
  r30 = r3;
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r30,128
  r31 = uint64_t(int64_t(int64_t(r30) + 128));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r10,255
  r10 = uint64_t(int64_t(255));
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // lwz r9,0(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r3,64
  r3 = uint64_t(int64_t(64));
  // ori r4,r4,32772
  r4 = r4 | 32772;
  // rlwimi r9,r11,0,27,31
  r9 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x1F) | (r9 & 0xFFFFFFFFFFFFFFE0);
  // rotlwi r11,r9,0
  r11 = __builtin_rotateleft32(uint32_t(r9), 0);
  // rlwinm r11,r11,0,27,2
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFE000001F;
  // stw r9,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r9));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // oris r11,r11,8192
  r11 = r11 | 536870912;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // sth r29,4(r31)
  guest::StoreU16(base, uint32_t(r31) + 4, uint16_t(r29));
  // stb r10,6(r31)
  guest::StoreU8(base, uint32_t(r31) + 6, uint8_t(r10));
  // lwz r11,28(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 28);
  // lhz r11,9(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 9);
  // sth r11,8(r31)
  guest::StoreU16(base, uint32_t(r31) + 8, uint16_t(r11));
  // lwz r11,28(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 28);
  // lbz r11,11(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 11);
  // stb r11,10(r31)
  guest::StoreU8(base, uint32_t(r31) + 10, uint8_t(r11));
  // stb r29,11(r31)
  guest::StoreU8(base, uint32_t(r31) + 11, uint8_t(r29));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CCE34, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cce5c
  if (cr6.eq) goto loc_825CCE5C;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r4,32(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 32);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r30
  r5 = r30;
  // bl 0x825d0ce0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::Ctor(base, LIFTED_ARGS(0x825CCE50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825cced0
  if (!cr6.eq) goto loc_825CCED0;
loc_825CCE5C:
  // lis r31,-32761
  r31 = uint64_t(int64_t(-2147024896));
  // ori r31,r31,14
  r31 = r31 | 14;
loc_825CCE64:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x825cce84
  if (cr6.eq) goto loc_825CCE84;
  // lwz r11,0(r29)
  r11 = lifted::VtableWord(base, uint32_t(r29));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r29
  r3 = r29;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CCE84, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CCE84:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825ccec4
  if (cr6.lt) goto loc_825CCEC4;
  // mr r3,r30
  r3 = r30;
  // bl 0x825cb470
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetUpRpcs(base, LIFTED_ARGS(0x825CCE94, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825ccec4
  if (cr6.lt) goto loc_825CCEC4;
  // lwz r11,24(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ccec4
  if (cr6.eq) goto loc_825CCEC4;
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x825ccec4
  if (!cr6.eq) goto loc_825CCEC4;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CCEC4:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CCED0:
  // mr r5,r28
  r5 = r28;
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x825d09d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::PrepareSimple(base, LIFTED_ARGS(0x825CCEE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825cce64
  if (cr6.lt) goto loc_825CCE64;
  // addi r11,r30,36
  r11 = uint64_t(int64_t(int64_t(r30) + 36));
  // stw r28,104(r30)
  guest::StoreU32(base, uint32_t(r30) + 104, uint32_t(r28));
  // stw r11,4(r29)
  guest::StoreU32(base, uint32_t(r29) + 4, uint32_t(r11));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // stw r10,8(r29)
  guest::StoreU32(base, uint32_t(r29) + 8, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // stw r29,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r29));
  // stw r29,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r29));
  // b 0x825cce84
  goto loc_825CCE84;
}

// sub_825CCF10 game::XACTCue::Prepare: lifted from generated/ by lift.py.
lifted::Result f825CCF10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CCF10 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r4
  r29 = r4;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CCF34, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,84(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 84);
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CCF50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ccf8c
  if (cr6.lt) goto loc_825CCF8C;
  // lwz r11,48(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 48);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r29
  r6 = r29;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r4,1(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 1);
  // bl 0x825c5c80
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::PrepareSound(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ccf8c
  if (cr6.lt) goto loc_825CCF8C;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // stw r11,52(r31)
  guest::StoreU32(base, uint32_t(r31) + 52, uint32_t(r11));
loc_825CCF8C:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CCF94, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CD020: lifted from generated/ by lift.py.
lifted::Result f825CD020(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CD020 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r4
  r29 = r4;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CD044, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,84(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CD058, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // blt cr6,0x825cd100
  if (cr6.lt) goto loc_825CD100;
  // lwz r11,48(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 48);
  // lbz r10,0(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 0);
  // rlwinm r10,r10,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825cd08c
  if (cr6.eq) goto loc_825CD08C;
  // lwz r11,1(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 1);
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // b 0x825cd100
  goto loc_825CD100;
loc_825CD08C:
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,16
  r3 = uint64_t(int64_t(16));
  // ori r4,r4,32791
  r4 = r4 | 32791;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CD09C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825cd0d8
  if (cr6.eq) goto loc_825CD0D8;
  // lwz r11,48(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 48);
  // lwz r3,296(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 296);
  // lwz r4,1(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 1);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,32(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CD0C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r3,r30
  r3 = r30;
  // lwz r4,56(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 56);
  // bl 0x825d3360
  // (left out: dead in C++)
  game::XACTVariationTable::Ctor(base, r3, r4, r5);
  // b 0x825cd0dc
  goto loc_825CD0DC;
loc_825CD0D8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825CD0DC:
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,300(r31)
  guest::StoreU32(base, uint32_t(r31) + 300, uint32_t(r3));
  // beq cr6,0x825cd0f8
  if (cr6.eq) goto loc_825CD0F8;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825d3618
  // (left out: dead in C++)
  r3 = f825D3618(base, r1, r13, r3, r4);
  // mr r30,r3
  r30 = r3;
  // b 0x825cd100
  goto loc_825CD100;
loc_825CD0F8:
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // ori r30,r30,14
  r30 = r30 | 14;
loc_825CD100:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // blt cr6,0x825cd13c
  if (cr6.lt) goto loc_825CD13C;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r4,80(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 80);
  // mr r6,r29
  r6 = r29;
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5c80
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::PrepareSound(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825cd13c
  if (cr6.lt) goto loc_825CD13C;
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // stw r11,52(r31)
  guest::StoreU32(base, uint32_t(r31) + 52, uint32_t(r11));
loc_825CD13C:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CD144, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CD150: lifted from generated/ by lift.py.
lifted::Result f825CD150(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825CD150 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r11,r3
  r11 = r3;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // lwz r10,0(r3)
  r10 = lifted::VtableWord(base, uint32_t(r3));
  // lbz r9,14(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 14);
  // lhz r5,10(r11)
  r5 = guest::LoadU16(base, uint32_t(r11) + 10);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // clrlwi r6,r9,30
  r6 = uint32_t(r9) & 0x3;
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CD180: lifted from generated/ by lift.py.
uint64_t f825CD180(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CD180 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,360(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 360);
  // lwz r3,356(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 356);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // ori r3,r3,64
  r3 = r3 | 64;
  // blr
  return r3;
}

// sub_825CD198: lifted from generated/ by lift.py.
uint64_t f825CD198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CD198 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,380(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 380);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cd1f8
  if (cr6.eq) goto loc_825CD1F8;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(base, LIFTED_ARGS(0x825CD1BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,388(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 388);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // subf r11,r11,r3
  r11 = r3 - r11;
  // bgt cr6,0x825cd1d0
  if (cr6.gt) goto loc_825CD1D0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825CD1D0:
  // lwz r10,384(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 384);
  // lwz r9,380(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 380);
  // subf r10,r10,r3
  r10 = r3 - r10;
  // subf r11,r11,r10
  r11 = r10 - r11;
  // subf r3,r9,r11
  r3 = r11 - r9;
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
loc_825CD1F8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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

// sub_825CD210: lifted from generated/ by lift.py.
uint64_t f825CD210(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CD210 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,380(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 380);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cd254
  if (cr6.eq) goto loc_825CD254;
  // bl 0x825cd198
  // (left out: dead in C++)
  r3 = f825CD198(base, r1, r13, r3);
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // add r3,r11,r3
  r3 = r11 + r3;
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
loc_825CD254:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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

// sub_825CD270: lifted from generated/ by lift.py.
lifted::Result f825CD270(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825CD270 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // bl 0x825c5e58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5E58, LIFTED_ARGS(0x825CD284, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32252
  r11 = uint64_t(int64_t(-2113667072));
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // addi r9,r11,-8448
  r9 = uint64_t(int64_t(int64_t(r11) + -8448));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // ori r10,r10,65535
  r10 = r10 | 65535;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r9,352(r31)
  guest::StoreU32(base, uint32_t(r31) + 352, uint32_t(r9));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stfs f0,372(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 372, temp.u32);
  // sth r10,424(r31)
  guest::StoreU16(base, uint32_t(r31) + 424, uint16_t(r10));
  // addi r8,r11,24752
  r8 = uint64_t(int64_t(int64_t(r11) + 24752));
  // stw r29,356(r31)
  guest::StoreU32(base, uint32_t(r31) + 356, uint32_t(r29));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // sth r10,426(r31)
  guest::StoreU16(base, uint32_t(r31) + 426, uint16_t(r10));
  // addi r7,r11,22636
  r7 = uint64_t(int64_t(int64_t(r11) + 22636));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r8,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r8));
  // addi r6,r11,24748
  r6 = uint64_t(int64_t(int64_t(r11) + 24748));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r7,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r7));
  // addi r5,r11,24744
  r5 = uint64_t(int64_t(int64_t(r11) + 24744));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r6,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r6));
  // addi r4,r11,24732
  r4 = uint64_t(int64_t(int64_t(r11) + 24732));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r5,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r5));
  // addi r30,r11,24728
  r30 = uint64_t(int64_t(int64_t(r11) + 24728));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r4,24(r31)
  guest::StoreU32(base, uint32_t(r31) + 24, uint32_t(r4));
  // stw r30,352(r31)
  guest::StoreU32(base, uint32_t(r31) + 352, uint32_t(r30));
  // stw r11,360(r31)
  guest::StoreU32(base, uint32_t(r31) + 360, uint32_t(r11));
  // stw r11,364(r31)
  guest::StoreU32(base, uint32_t(r31) + 364, uint32_t(r11));
  // stw r11,368(r31)
  guest::StoreU32(base, uint32_t(r31) + 368, uint32_t(r11));
  // stw r11,376(r31)
  guest::StoreU32(base, uint32_t(r31) + 376, uint32_t(r11));
  // stw r11,380(r31)
  guest::StoreU32(base, uint32_t(r31) + 380, uint32_t(r11));
  // stw r11,384(r31)
  guest::StoreU32(base, uint32_t(r31) + 384, uint32_t(r11));
  // stw r11,388(r31)
  guest::StoreU32(base, uint32_t(r31) + 388, uint32_t(r11));
  // stw r11,392(r31)
  guest::StoreU32(base, uint32_t(r31) + 392, uint32_t(r11));
  // stw r11,396(r31)
  guest::StoreU32(base, uint32_t(r31) + 396, uint32_t(r11));
  // stw r11,400(r31)
  guest::StoreU32(base, uint32_t(r31) + 400, uint32_t(r11));
  // stw r11,404(r31)
  guest::StoreU32(base, uint32_t(r31) + 404, uint32_t(r11));
  // stw r11,408(r31)
  guest::StoreU32(base, uint32_t(r31) + 408, uint32_t(r11));
  // stw r11,412(r31)
  guest::StoreU32(base, uint32_t(r31) + 412, uint32_t(r11));
  // stw r11,416(r31)
  guest::StoreU32(base, uint32_t(r31) + 416, uint32_t(r11));
  // stw r11,420(r31)
  guest::StoreU32(base, uint32_t(r31) + 420, uint32_t(r11));
  // stw r11,428(r31)
  guest::StoreU32(base, uint32_t(r31) + 428, uint32_t(r11));
  // stw r11,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CD350: lifted from generated/ by lift.py.
lifted::Result f825CD350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CD350 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x825ce770
  { const lifted::Result o = f825CE770(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CD378: lifted from generated/ by lift.py.
lifted::Result f825CD378(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825CD378 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CD388: lifted from generated/ by lift.py.
void f825CD388(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CD388 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,408(r3)
  guest::StoreU32(base, uint32_t(r3) + 408, uint32_t(r11));
  // blr
  return;
}

// sub_825CD398: lifted from generated/ by lift.py.
lifted::Result f825CD398(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CD398 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // lwz r11,360(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 360);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825cd4e8
  if (!cr6.eq) goto loc_825CD4E8;
  // lwz r11,420(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 420);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825cd4e8
  if (!cr6.eq) goto loc_825CD4E8;
  // lwz r11,356(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 356);
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // beq cr6,0x825cd3ec
  if (cr6.eq) goto loc_825CD3EC;
  // lwz r10,60(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 60);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x825cd4e8
  if (!cr6.eq) goto loc_825CD4E8;
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // bne cr6,0x825cd4e8
  if (!cr6.eq) goto loc_825CD4E8;
loc_825CD3EC:
  // lwz r11,432(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 432);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x825cd4e8
  if (!cr6.eq) goto loc_825CD4E8;
  // lwz r11,400(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 400);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cd420
  if (cr6.eq) goto loc_825CD420;
  // mr r3,r31
  r3 = r31;
  // bl 0x825cd210
  // (left out: dead in C++)
  r3 = f825CD210(base, r1, r13, r3);
  // lwz r11,396(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 396);
  // cmpw cr6,r11,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r3), so);
  // bgt cr6,0x825cd4b0
  if (cr6.gt) goto loc_825CD4B0;
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x825cd4b4
  goto loc_825CD4B4;
loc_825CD420:
  // lwz r3,52(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cd464
  if (cr6.eq) goto loc_825CD464;
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lhz r11,24(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 24);
  // rlwinm r11,r11,0,24,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xE0;
  // cmplwi cr6,r11,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 128, so);
  // bne cr6,0x825cd464
  if (!cr6.eq) goto loc_825CD464;
  // lwz r11,80(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 80);
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x825cd464
  if (cr6.eq) goto loc_825CD464;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x825cd464
  if (cr6.eq) goto loc_825CD464;
  // bl 0x825cac58
  // (left out: dead in C++)
  { const lifted::Result o = f825CAC58(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825cd464
  if (!cr6.eq) goto loc_825CD464;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825CD464:
  // lwz r11,52(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cd488
  if (cr6.eq) goto loc_825CD488;
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lhz r11,24(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 24);
  // rlwinm r11,r11,0,24,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xE0;
  // cmplwi cr6,r11,96
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 96, so);
  // bne cr6,0x825cd488
  if (!cr6.eq) goto loc_825CD488;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825CD488:
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lhz r11,24(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 24);
  // rlwinm r11,r11,27,29,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x7;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x825cd4a4
  if (cr6.eq) goto loc_825CD4A4;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x825cd4b4
  if (!cr6.eq) goto loc_825CD4B4;
loc_825CD4A4:
  // lwz r11,408(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 408);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cd4b4
  if (cr6.eq) goto loc_825CD4B4;
loc_825CD4B0:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825CD4B4:
  // lwz r11,416(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cd4d0
  if (cr6.eq) goto loc_825CD4D0;
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x825cd4d0
  if (cr6.eq) goto loc_825CD4D0;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825CD4D0:
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cd4f0
  if (cr6.eq) goto loc_825CD4F0;
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x825cd4f0
  if (cr6.eq) goto loc_825CD4F0;
loc_825CD4E8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825cd4f4
  goto loc_825CD4F4;
loc_825CD4F0:
  // mr r3,r30
  r3 = r30;
loc_825CD4F4:
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

// sub_825CD510: lifted from generated/ by lift.py.
uint64_t f825CD510(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825CD510 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,52(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 52);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bne cr6,0x825cd5a0
  if (!cr6.eq) goto loc_825CD5A0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,432(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 432);
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // stw r11,52(r31)
  guest::StoreU32(base, uint32_t(r31) + 52, uint32_t(r11));
  // sth r10,424(r31)
  guest::StoreU16(base, uint32_t(r31) + 424, uint16_t(r10));
  // stw r11,40(r31)
  guest::StoreU32(base, uint32_t(r31) + 40, uint32_t(r11));
  // bne cr6,0x825cd60c
  if (!cr6.eq) goto loc_825CD60C;
  // lwz r11,400(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 400);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825cd60c
  if (!cr6.eq) goto loc_825CD60C;
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lhz r11,24(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 24);
  // rlwinm r11,r11,27,29,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x7;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // beq cr6,0x825cd578
  if (cr6.eq) goto loc_825CD578;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x825cd60c
  if (!cr6.eq) goto loc_825CD60C;
loc_825CD578:
  // mr r3,r31
  r3 = r31;
  // bl 0x825cd198
  // (left out: dead in C++)
  r3 = f825CD198(base, r1, r13, r3);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r3,396(r31)
  guest::StoreU32(base, uint32_t(r31) + 396, uint32_t(r3));
  // stw r11,400(r31)
  guest::StoreU32(base, uint32_t(r31) + 400, uint32_t(r11));
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
loc_825CD5A0:
  // lwz r11,416(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bne cr6,0x825cd5d0
  if (!cr6.eq) goto loc_825CD5D0;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // sth r11,426(r31)
  guest::StoreU16(base, uint32_t(r31) + 426, uint16_t(r11));
  // stw r10,416(r31)
  guest::StoreU32(base, uint32_t(r31) + 416, uint32_t(r10));
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
loc_825CD5D0:
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bne cr6,0x825cd5f8
  if (!cr6.eq) goto loc_825CD5F8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,412(r31)
  guest::StoreU32(base, uint32_t(r31) + 412, uint32_t(r11));
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
loc_825CD5F8:
  // lwz r11,420(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 420);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bne cr6,0x825cd60c
  if (!cr6.eq) goto loc_825CD60C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,420(r31)
  guest::StoreU32(base, uint32_t(r31) + 420, uint32_t(r11));
loc_825CD60C:
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

// sub_825CD620: lifted from generated/ by lift.py.
lifted::Result f825CD620(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CD620 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // mr r3,r4
  r3 = r4;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825cadc0
  // (left out: dead in C++)
  { const lifted::Result o = f825CADC0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825CD640); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825cd198
  // (left out: dead in C++)
  r3 = f825CD198(base, r1, r13, r3);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r3,396(r31)
  guest::StoreU32(base, uint32_t(r31) + 396, uint32_t(r3));
  // stw r11,400(r31)
  guest::StoreU32(base, uint32_t(r31) + 400, uint32_t(r11));
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

// sub_825CD668: lifted from generated/ by lift.py.
uint64_t f825CD668(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CD668 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,408(r31)
  guest::StoreU32(base, uint32_t(r31) + 408, uint32_t(r11));
  // bl 0x825cd198
  // (left out: dead in C++)
  r3 = f825CD198(base, r1, r13, r3);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r3,396(r31)
  guest::StoreU32(base, uint32_t(r31) + 396, uint32_t(r3));
  // stw r11,400(r31)
  guest::StoreU32(base, uint32_t(r31) + 400, uint32_t(r11));
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

// sub_825CD6A8: lifted from generated/ by lift.py.
lifted::Result f825CD6A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CD6A8 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lhz r11,24(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 24);
  // rlwinm r11,r11,27,29,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x7;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bgt cr6,0x825cd840
  if (cr6.gt) goto loc_825CD840;
  // lis r12,-32163
  r12 = uint64_t(int64_t(-2107834368));
  // addi r12,r12,-10516
  r12 = uint64_t(int64_t(int64_t(r12) + -10516));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = guest::LoadU32(base, uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825CD700;
  case 1:
  goto loc_825CD7B8;
  case 2:
  goto loc_825CD7B8;
  case 3:
  goto loc_825CD750;
  case 4:
  goto loc_825CD718;
  default:
  std::abort();
  }
loc_825CD700:
  // mr r3,r31
  r3 = r31;
  // bl 0x825cd210
  // (left out: dead in C++)
  r3 = f825CD210(base, r1, r13, r3);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r3,396(r31)
  guest::StoreU32(base, uint32_t(r31) + 396, uint32_t(r3));
  // stw r11,400(r31)
  guest::StoreU32(base, uint32_t(r31) + 400, uint32_t(r11));
  // b 0x825cd840
  goto loc_825CD840;
loc_825CD718:
  // lwz r3,52(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 52);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CD72C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,32767
  r11 = uint64_t(int64_t(2147418112));
  // ori r11,r11,65535
  r11 = r11 | 65535;
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // beq cr6,0x825cd700
  if (cr6.eq) goto loc_825CD700;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,380(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 380);
  // subf r11,r11,r3
  r11 = r3 - r11;
  // stw r10,400(r31)
  guest::StoreU32(base, uint32_t(r31) + 400, uint32_t(r10));
  // b 0x825cd83c
  goto loc_825CD83C;
loc_825CD750:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r3,52(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 52);
  // stw r11,400(r31)
  guest::StoreU32(base, uint32_t(r31) + 400, uint32_t(r11));
  // bl 0x825cacb8
  // (left out: dead in C++)
  { const lifted::Result o = f825CACB8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825cd798
  if (!cr6.eq) goto loc_825CD798;
  // lwz r3,52(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 52);
  // bl 0x825cb660
  // (left out: dead in C++)
  r3 = f825CB660(base, r3);
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825cd798
  if (!cr6.eq) goto loc_825CD798;
  // lwz r3,52(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 52);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CD78C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,380(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 380);
  // subf r11,r11,r3
  r11 = r3 - r11;
  // b 0x825cd83c
  goto loc_825CD83C;
loc_825CD798:
  // lwz r3,52(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 52);
  // bl 0x825cad18
  // (left out: dead in C++)
  { const lifted::Result o = f825CAD18(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,52(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 52);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r11,396(r31)
  guest::StoreU32(base, uint32_t(r31) + 396, uint32_t(r11));
  // bl 0x825cadc0
  // (left out: dead in C++)
  { const lifted::Result o = f825CADC0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825CD7B4); r3 = o.r3; f1 = o.f1; }
  // b 0x825cd840
  goto loc_825CD840;
loc_825CD7B8:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,400(r31)
  guest::StoreU32(base, uint32_t(r31) + 400, uint32_t(r30));
  // bl 0x825cd210
  // (left out: dead in C++)
  r3 = f825CD210(base, r1, r13, r3);
  // mr r10,r3
  r10 = r3;
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lwz r3,52(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 52);
  // stw r10,396(r31)
  guest::StoreU32(base, uint32_t(r31) + 396, uint32_t(r10));
  // lhz r10,24(r11)
  r10 = guest::LoadU16(base, uint32_t(r11) + 24);
  // lwz r6,8(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + 8);
  // rlwinm r10,r10,0,24,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xE0;
  // lwz r5,4(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r11,r10,-32
  r11 = uint64_t(int64_t(int64_t(r10) + -32));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r4,r11,27,31,31
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x825cb708
  // (left out: dead in C++)
  r3 = f825CB708(base, r1, r13, r3, r4, r5, r6);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,408(r31)
  guest::StoreU32(base, uint32_t(r31) + 408, uint32_t(r3));
  // beq cr6,0x825cd840
  if (cr6.eq) goto loc_825CD840;
  // stw r30,64(r3)
  guest::StoreU32(base, uint32_t(r3) + 64, uint32_t(r30));
  // lwz r11,408(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 408);
  // lhz r9,56(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 56);
  // lwz r10,28(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 28);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x825cd820
  if (cr6.eq) goto loc_825CD820;
  // lwz r10,20(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 20);
loc_825CD820:
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r8,380(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 380);
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // stw r9,400(r31)
  guest::StoreU32(base, uint32_t(r31) + 400, uint32_t(r9));
  // subf r11,r8,r11
  r11 = r11 - r8;
  // add r11,r11,r10
  r11 = r11 + r10;
loc_825CD83C:
  // stw r11,396(r31)
  guest::StoreU32(base, uint32_t(r31) + 396, uint32_t(r11));
loc_825CD840:
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

// sub_825CD858: lifted from generated/ by lift.py.
lifted::Result f825CD858(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CD858 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CD87C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,356(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 356);
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // bne cr6,0x825cd8a4
  if (!cr6.eq) goto loc_825CD8A4;
  // lis r29,-30009
  r29 = uint64_t(int64_t(-1966669824));
  // mr r3,r28
  r3 = r28;
  // ori r29,r29,6
  r29 = r29 | 6;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CD898, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CD8A4:
  // lwz r11,360(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 360);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // beq cr6,0x825cd97c
  if (cr6.eq) goto loc_825CD97C;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c4cf8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::Pause(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825cd97c
  if (cr6.lt) goto loc_825CD97C;
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cd8ec
  if (cr6.eq) goto loc_825CD8EC;
  // cntlzw r11,r30
  r11 = uint32_t(r30) == 0 ? 32 : __builtin_clz(uint32_t(r30));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r4,r11,1
  r4 = r11 ^ 1;
  // bl 0x825ca6f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Pause(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
loc_825CD8EC:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825cd97c
  if (cr6.lt) goto loc_825CD97C;
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cd914
  if (cr6.eq) goto loc_825CD914;
  // cntlzw r11,r30
  r11 = uint32_t(r30) == 0 ? 32 : __builtin_clz(uint32_t(r30));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r4,r11,1
  r4 = r11 ^ 1;
  // bl 0x825ca6f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Pause(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
loc_825CD914:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825cd97c
  if (cr6.lt) goto loc_825CD97C;
  // cntlzw r11,r30
  r11 = uint32_t(r30) == 0 ? 32 : __builtin_clz(uint32_t(r30));
  // lwz r10,356(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 356);
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmplwi cr6,r10,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 8, so);
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // stw r11,360(r31)
  guest::StoreU32(base, uint32_t(r31) + 360, uint32_t(r11));
  // bne cr6,0x825cd97c
  if (!cr6.eq) goto loc_825CD97C;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cd95c
  if (cr6.eq) goto loc_825CD95C;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(base, LIFTED_ARGS(0x825CD944, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,388(r31)
  guest::StoreU32(base, uint32_t(r31) + 388, uint32_t(r3));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CD950, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CD95C:
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(base, LIFTED_ARGS(0x825CD960, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,388(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 388);
  // lwz r10,384(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 384);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // subf r11,r11,r10
  r11 = r10 - r11;
  // add r11,r11,r3
  r11 = r11 + r3;
  // stw r9,388(r31)
  guest::StoreU32(base, uint32_t(r31) + 388, uint32_t(r9));
  // stw r11,384(r31)
  guest::StoreU32(base, uint32_t(r31) + 384, uint32_t(r11));
loc_825CD97C:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CD984, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CD990: lifted from generated/ by lift.py.
lifted::Result f825CD990(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CD990 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r29,r11,144
  r29 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CD9B4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x825cd9d8
  if (!cr6.eq) goto loc_825CD9D8;
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // mr r3,r29
  r3 = r29;
  // ori r30,r30,87
  r30 = r30 | 87;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CD9CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825CD9D8:
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5238
  // (left out: dead in C++)
  { const lifted::Result o = f825C5238(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825cda2c
  if (cr6.lt) goto loc_825CDA2C;
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cda04
  if (cr6.eq) goto loc_825CDA04;
  // addi r4,r31,308
  r4 = uint64_t(int64_t(int64_t(r31) + 308));
  // bl 0x825ca980
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetTrackParameter(base, LIFTED_ARGS(0x825CDA04, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CDA04:
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cda18
  if (cr6.eq) goto loc_825CDA18;
  // addi r4,r31,308
  r4 = uint64_t(int64_t(int64_t(r31) + 308));
  // bl 0x825ca980
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetTrackParameter(base, LIFTED_ARGS(0x825CDA18, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CDA18:
  // lwz r3,420(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 420);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cda2c
  if (cr6.eq) goto loc_825CDA2C;
  // addi r4,r31,308
  r4 = uint64_t(int64_t(int64_t(r31) + 308));
  // bl 0x825ca980
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetTrackParameter(base, LIFTED_ARGS(0x825CDA2C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CDA2C:
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CDA34, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CDA40: lifted from generated/ by lift.py.
lifted::Result f825CDA40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CDA40 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r27,r11,144
  r27 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CDA6C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r28
  r6 = r28;
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c54a0
  // (left out: dead in C++)
  { const lifted::Result o = f825C54A0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825cdafc
  if (cr6.lt) goto loc_825CDAFC;
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cdaac
  if (cr6.eq) goto loc_825CDAAC;
  // mr r5,r29
  r5 = r29;
  // lwz r6,64(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 64);
  // lhz r4,68(r31)
  r4 = guest::LoadU16(base, uint32_t(r31) + 68);
  // bl 0x825caa00
  // (left out: dead in C++)
  { const lifted::Result o = f825CAA00(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825CDAAC:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825cdafc
  if (cr6.lt) goto loc_825CDAFC;
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cdad4
  if (cr6.eq) goto loc_825CDAD4;
  // mr r5,r29
  r5 = r29;
  // lwz r6,64(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 64);
  // lhz r4,68(r31)
  r4 = guest::LoadU16(base, uint32_t(r31) + 68);
  // bl 0x825caa00
  // (left out: dead in C++)
  { const lifted::Result o = f825CAA00(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825CDAD4:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825cdafc
  if (cr6.lt) goto loc_825CDAFC;
  // lwz r3,420(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 420);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cdafc
  if (cr6.eq) goto loc_825CDAFC;
  // mr r5,r29
  r5 = r29;
  // lwz r6,64(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 64);
  // lhz r4,68(r31)
  r4 = guest::LoadU16(base, uint32_t(r31) + 68);
  // bl 0x825caa00
  // (left out: dead in C++)
  { const lifted::Result o = f825CAA00(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825CDAFC:
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CDB04, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CDB10: lifted from generated/ by lift.py.
lifted::Result f825CDB10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CDB10 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CDB30, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // lwz r10,300(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 300);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r3,124(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 124);
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // lhz r30,6(r11)
  r30 = guest::LoadU16(base, uint32_t(r11) + 6);
  // beq cr6,0x825cdb78
  if (cr6.eq) goto loc_825CDB78;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // mr r4,r30
  r4 = r30;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CDB64, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = guest::LoadU8(base, uint32_t(r3) + 0);
  // rlwinm r11,r11,30,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825cdba4
  if (!cr6.eq) goto loc_825CDBA4;
loc_825CDB78:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r31,368
  r5 = uint64_t(int64_t(int64_t(r31) + 368));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5960, LIFTED_ARGS(0x825CDB8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CDB98, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CDBA4:
  // mr r4,r30
  r4 = r30;
  // lwz r3,56(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 56);
  // bl 0x825c0698
  // (left out: dead in C++)
  { const lifted::Result o = f825C0698(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,368(r31)
  guest::StoreU32(base, uint32_t(r31) + 368, uint32_t(r3));
  // bne cr6,0x825cdbc4
  if (!cr6.eq) goto loc_825CDBC4;
  // lis r29,-30009
  r29 = uint64_t(int64_t(-1966669824));
  // ori r29,r29,10
  r29 = r29 | 10;
loc_825CDBC4:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CDBCC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CDBD8: lifted from generated/ by lift.py.
lifted::Result f825CDBD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CDBD8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r29,r11,144
  r29 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CDBFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5060
  // (left out: dead in C++)
  { const lifted::Result o = f825C5060(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 412);
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cdc24
  if (cr6.eq) goto loc_825CDC24;
  // addi r4,r31,340
  r4 = uint64_t(int64_t(int64_t(r31) + 340));
  // mr r3,r11
  r3 = r11;
  // bl 0x825ca900
  // (left out: dead in C++)
  { const lifted::Result o = f825CA900(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CDC24:
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cdc38
  if (cr6.eq) goto loc_825CDC38;
  // addi r4,r31,340
  r4 = uint64_t(int64_t(int64_t(r31) + 340));
  // bl 0x825ca900
  // (left out: dead in C++)
  { const lifted::Result o = f825CA900(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CDC38:
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CDC40, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CDC50: lifted from generated/ by lift.py.
lifted::Result f825CDC50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CDC50 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r29,r11,144
  r29 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CDC74, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5128
  // (left out: dead in C++)
  { const lifted::Result o = f825C5128(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 412);
  // mr r28,r3
  r28 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cdc9c
  if (cr6.eq) goto loc_825CDC9C;
  // mr r4,r30
  r4 = r30;
  // mr r3,r11
  r3 = r11;
  // bl 0x825ca940
  // (left out: dead in C++)
  { const lifted::Result o = f825CA940(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CDC9C:
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cdcb0
  if (cr6.eq) goto loc_825CDCB0;
  // mr r4,r30
  r4 = r30;
  // bl 0x825ca940
  // (left out: dead in C++)
  { const lifted::Result o = f825CA940(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CDCB0:
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CDCB8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CDCC8: lifted from generated/ by lift.py.
lifted::Result f825CDCC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CDCC8 (lifted)");
  uint64_t r5 = 0, r11 = 0;
  // mr r11,r3
  r11 = r3;
  // mr r5,r4
  r5 = r4;
  // lwz r3,300(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 300);
  // lwz r4,296(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 296);
  // b 0x825d3338
  { const lifted::Result o = game::XACTVariationTable::UsesWaveBank(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CDCE0: lifted from generated/ by lift.py.
uint64_t f825CDCE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825CDCE0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // addi r11,r3,352
  r11 = uint64_t(int64_t(int64_t(r3) + 352));
  // bne cr6,0x825cdcf0
  if (!cr6.eq) goto loc_825CDCF0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825CDCF0:
  // lwz r10,368(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 368);
  // lwz r4,228(r10)
  r4 = guest::LoadU32(base, uint32_t(r10) + 228);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beqlr cr6
  if (cr6.eq) return r3;
loc_825CDD00:
  // lwz r9,0(r4)
  r9 = guest::LoadU32(base, uint32_t(r4) + 0);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // beq cr6,0x825cdd1c
  if (cr6.eq) goto loc_825CDD1C;
  // lwz r4,4(r4)
  r4 = guest::LoadU32(base, uint32_t(r4) + 4);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x825cdd00
  if (!cr6.eq) goto loc_825CDD00;
  // blr
  return r3;
loc_825CDD1C:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // addi r3,r10,24
  r3 = uint64_t(int64_t(int64_t(r10) + 24));
  // b 0x825c2c60
  { const lifted::Result o = game::XACTNodePool::Free(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_825CDD30: lifted from generated/ by lift.py.
lifted::Result f825CDD30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_825CDD30 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lis r8,-32249
  r8 = uint64_t(int64_t(-2113470464));
  // lis r7,-32249
  r7 = uint64_t(int64_t(-2113470464));
  // lis r6,-32249
  r6 = uint64_t(int64_t(-2113470464));
  // lwz r3,368(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 368);
  // lis r5,-32249
  r5 = uint64_t(int64_t(-2113470464));
  // addi r11,r31,352
  r11 = uint64_t(int64_t(int64_t(r31) + 352));
  // addi r10,r10,24752
  r10 = uint64_t(int64_t(int64_t(r10) + 24752));
  // addi r9,r9,22636
  r9 = uint64_t(int64_t(int64_t(r9) + 22636));
  // addi r8,r8,24748
  r8 = uint64_t(int64_t(int64_t(r8) + 24748));
  // addi r7,r7,24744
  r7 = uint64_t(int64_t(int64_t(r7) + 24744));
  // addi r6,r6,24732
  r6 = uint64_t(int64_t(int64_t(r6) + 24732));
  // addi r5,r5,24728
  r5 = uint64_t(int64_t(int64_t(r5) + 24728));
  // stw r10,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r10));
  // stw r9,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r9));
  // stw r8,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r8));
  // stw r7,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r7));
  // stw r6,24(r31)
  guest::StoreU32(base, uint32_t(r31) + 24, uint32_t(r6));
  // stw r5,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r5));
  // lwz r4,228(r3)
  r4 = guest::LoadU32(base, uint32_t(r3) + 228);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825cddcc
  if (cr6.eq) goto loc_825CDDCC;
loc_825CDDA0:
  // lwz r10,0(r4)
  r10 = guest::LoadU32(base, uint32_t(r4) + 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // beq cr6,0x825cddbc
  if (cr6.eq) goto loc_825CDDBC;
  // lwz r4,4(r4)
  r4 = guest::LoadU32(base, uint32_t(r4) + 4);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x825cdda0
  if (!cr6.eq) goto loc_825CDDA0;
  // b 0x825cddcc
  goto loc_825CDDCC;
loc_825CDDBC:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825cddcc
  if (cr6.eq) goto loc_825CDDCC;
  // addi r3,r3,24
  r3 = uint64_t(int64_t(int64_t(r3) + 24));
  // bl 0x825c2c60
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTNodePool::Free(base, LIFTED_ARGS(0x825CDDCC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CDDCC:
  // lwz r3,404(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 404);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cddec
  if (cr6.eq) goto loc_825CDDEC;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CDDEC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CDDEC:
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cde0c
  if (cr6.eq) goto loc_825CDE0C;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CDE0C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CDE0C:
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cde2c
  if (cr6.eq) goto loc_825CDE2C;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CDE2C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CDE2C:
  // lwz r3,420(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 420);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cde4c
  if (cr6.eq) goto loc_825CDE4C;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CDE4C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CDE4C:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lis r8,-32249
  r8 = uint64_t(int64_t(-2113470464));
  // lis r7,-32249
  r7 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,22936
  r11 = uint64_t(int64_t(int64_t(r11) + 22936));
  // addi r10,r10,22636
  r10 = uint64_t(int64_t(int64_t(r10) + 22636));
  // addi r9,r9,24748
  r9 = uint64_t(int64_t(int64_t(r9) + 24748));
  // addi r8,r8,22932
  r8 = uint64_t(int64_t(int64_t(r8) + 22932));
  // addi r7,r7,24732
  r7 = uint64_t(int64_t(int64_t(r7) + 24732));
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r9));
  // stw r8,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r8));
  // stw r7,24(r31)
  guest::StoreU32(base, uint32_t(r31) + 24, uint32_t(r7));
  // bl 0x825c6148
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCueBase::Dtor(base, r1, r13, r3, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_825CDEA8: lifted from generated/ by lift.py.
lifted::Result f825CDEA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CDEA8 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,432(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 432);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x825ce014
  if (cr6.eq) goto loc_825CE014;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x825cdf40
  if (cr6.eq) goto loc_825CDF40;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x825ce25c
  if (!cr6.eq) goto loc_825CE25C;
  // lwz r10,52(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 52);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825cdef4
  if (cr6.eq) goto loc_825CDEF4;
  // lwz r11,80(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 80);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x825ce25c
  if (cr6.eq) goto loc_825CE25C;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x825ce25c
  if (cr6.eq) goto loc_825CE25C;
loc_825CDEF4:
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cdf14
  if (cr6.eq) goto loc_825CDF14;
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x825ce25c
  if (cr6.eq) goto loc_825CE25C;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x825ce25c
  if (cr6.eq) goto loc_825CE25C;
loc_825CDF14:
  // stw r10,420(r31)
  guest::StoreU32(base, uint32_t(r31) + 420, uint32_t(r10));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // lhz r10,426(r31)
  r10 = guest::LoadU16(base, uint32_t(r31) + 426);
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // lwz r9,416(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 416);
  // stw r28,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r28));
  // sth r11,426(r31)
  guest::StoreU16(base, uint32_t(r31) + 426, uint16_t(r11));
  // stw r28,416(r31)
  guest::StoreU32(base, uint32_t(r31) + 416, uint32_t(r28));
  // sth r10,424(r31)
  guest::StoreU16(base, uint32_t(r31) + 424, uint16_t(r10));
  // stw r9,52(r31)
  guest::StoreU32(base, uint32_t(r31) + 52, uint32_t(r9));
  // b 0x825ce25c
  goto loc_825CE25C;
loc_825CDF40:
  // lwz r11,440(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 440);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cdfe0
  if (cr6.eq) goto loc_825CDFE0;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(base, LIFTED_ARGS(0x825CDF50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CDF68, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lhz r5,20(r11)
  r5 = guest::LoadU16(base, uint32_t(r11) + 20);
  // subf r10,r5,r3
  r10 = r3 - r5;
  // cmpw cr6,r30,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r10), so);
  // blt cr6,0x825cdfe0
  if (cr6.lt) goto loc_825CDFE0;
  // lwz r10,412(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 412);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lhz r11,24(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 24);
  // addi r3,r10,4
  r3 = uint64_t(int64_t(int64_t(r10) + 4));
  // clrlwi r6,r11,30
  r6 = uint32_t(r11) & 0x3;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CDFA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,416(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 416);
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r3,r10,4
  r3 = uint64_t(int64_t(int64_t(r10) + 4));
  // lhz r10,24(r11)
  r10 = guest::LoadU16(base, uint32_t(r11) + 24);
  // lwz r9,0(r3)
  r9 = lifted::VtableWord(base, uint32_t(r3));
  // lhz r5,20(r11)
  r5 = guest::LoadU16(base, uint32_t(r11) + 20);
  // clrlwi r6,r10,30
  r6 = uint32_t(r10) & 0x3;
  // lwz r11,0(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CDFCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // bl 0x825cbb28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Play(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r11));
  // b 0x825ce25c
  goto loc_825CE25C;
loc_825CDFE0:
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ce000
  if (cr6.eq) goto loc_825CE000;
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x825ce25c
  if (cr6.eq) goto loc_825CE25C;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x825ce25c
  if (cr6.eq) goto loc_825CE25C;
loc_825CE000:
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // bl 0x825cbb28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Play(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r11));
  // b 0x825ce25c
  goto loc_825CE25C;
loc_825CE014:
  // mr r3,r31
  r3 = r31;
  // bl 0x825cd398
  // (left out: dead in C++)
  { const lifted::Result o = f825CD398(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825ce25c
  if (cr6.eq) goto loc_825CE25C;
  // lwz r9,428(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 428);
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r27,3
  r27 = uint64_t(int64_t(3));
  // lhz r11,24(r9)
  r11 = guest::LoadU16(base, uint32_t(r9) + 24);
  // rlwinm r11,r11,30,29,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x7;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bgt cr6,0x825ce0e4
  if (cr6.gt) goto loc_825CE0E4;
  // lis r12,-32163
  r12 = uint64_t(int64_t(-2107834368));
  // addi r12,r12,-8100
  r12 = uint64_t(int64_t(int64_t(r12) + -8100));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = guest::LoadU32(base, uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825CE07C;
  case 1:
  goto loc_825CE0D4;
  case 2:
  goto loc_825CE088;
  case 3:
  goto loc_825CE098;
  case 4:
  goto loc_825CE0AC;
  case 5:
  goto loc_825CE0C0;
  case 6:
  goto loc_825CE07C;
  case 7:
  goto loc_825CE0D4;
  default:
  std::abort();
  }
loc_825CE07C:
  // stw r28,436(r31)
  guest::StoreU32(base, uint32_t(r31) + 436, uint32_t(r28));
  // stw r28,440(r31)
  guest::StoreU32(base, uint32_t(r31) + 440, uint32_t(r28));
  // b 0x825ce0e0
  goto loc_825CE0E0;
loc_825CE088:
  // stw r10,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r10));
  // stw r28,436(r31)
  guest::StoreU32(base, uint32_t(r31) + 436, uint32_t(r28));
  // stw r28,440(r31)
  guest::StoreU32(base, uint32_t(r31) + 440, uint32_t(r28));
  // b 0x825ce0e4
  goto loc_825CE0E4;
loc_825CE098:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r10,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r10));
  // stw r28,440(r31)
  guest::StoreU32(base, uint32_t(r31) + 440, uint32_t(r28));
  // stw r11,436(r31)
  guest::StoreU32(base, uint32_t(r31) + 436, uint32_t(r11));
  // b 0x825ce0e4
  goto loc_825CE0E4;
loc_825CE0AC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r10,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r10));
  // stw r28,436(r31)
  guest::StoreU32(base, uint32_t(r31) + 436, uint32_t(r28));
  // stw r11,440(r31)
  guest::StoreU32(base, uint32_t(r31) + 440, uint32_t(r11));
  // b 0x825ce0e4
  goto loc_825CE0E4;
loc_825CE0C0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r10,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r10));
  // stw r11,436(r31)
  guest::StoreU32(base, uint32_t(r31) + 436, uint32_t(r11));
  // stw r11,440(r31)
  guest::StoreU32(base, uint32_t(r31) + 440, uint32_t(r11));
  // b 0x825ce0e4
  goto loc_825CE0E4;
loc_825CE0D4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,440(r31)
  guest::StoreU32(base, uint32_t(r31) + 440, uint32_t(r11));
  // stw r11,436(r31)
  guest::StoreU32(base, uint32_t(r31) + 436, uint32_t(r11));
loc_825CE0E0:
  // stw r27,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r27));
loc_825CE0E4:
  // mr r29,r10
  r29 = r10;
  // lwz r10,52(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 52);
  // li r30,60
  r30 = uint64_t(int64_t(60));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825ce168
  if (cr6.eq) goto loc_825CE168;
  // lwz r11,80(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 80);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x825ce10c
  if (cr6.eq) goto loc_825CE10C;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bne cr6,0x825ce168
  if (!cr6.eq) goto loc_825CE168;
loc_825CE10C:
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ce128
  if (!cr6.eq) goto loc_825CE128;
  // lwz r11,416(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // mr r11,r28
  r11 = r28;
  // beq cr6,0x825ce12c
  if (cr6.eq) goto loc_825CE12C;
loc_825CE128:
  // li r11,60
  r11 = uint64_t(int64_t(60));
loc_825CE12C:
  // lwz r8,436(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 436);
  // clrlwi r30,r11,16
  r30 = uint32_t(r11) & 0xFFFF;
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x825ce148
  if (cr6.eq) goto loc_825CE148;
  // lhz r11,24(r9)
  r11 = guest::LoadU16(base, uint32_t(r9) + 24);
  // lhz r30,22(r9)
  r30 = guest::LoadU16(base, uint32_t(r9) + 22);
  // clrlwi r29,r11,30
  r29 = uint32_t(r11) & 0x3;
loc_825CE148:
  // addi r3,r10,4
  r3 = uint64_t(int64_t(int64_t(r10) + 4));
  // mr r6,r29
  r6 = r29;
  // mr r5,r30
  r5 = r30;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CE168, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CE168:
  // lwz r10,412(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825ce1d0
  if (cr6.eq) goto loc_825CE1D0;
  // lwz r11,52(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // bne cr6,0x825ce188
  if (!cr6.eq) goto loc_825CE188;
  // mr r11,r28
  r11 = r28;
loc_825CE188:
  // lwz r9,436(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 436);
  // clrlwi r30,r11,16
  r30 = uint32_t(r11) & 0xFFFF;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x825ce1a8
  if (cr6.eq) goto loc_825CE1A8;
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lhz r9,24(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 24);
  // lhz r30,22(r11)
  r30 = guest::LoadU16(base, uint32_t(r11) + 22);
  // clrlwi r29,r9,30
  r29 = uint32_t(r9) & 0x3;
loc_825CE1A8:
  // addi r3,r10,4
  r3 = uint64_t(int64_t(int64_t(r10) + 4));
  // mr r6,r29
  r6 = r29;
  // mr r5,r30
  r5 = r30;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CE1C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // bl 0x825cbb28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Play(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_825CE1D0:
  // lwz r10,416(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825ce24c
  if (cr6.eq) goto loc_825CE24C;
  // lwz r11,432(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 432);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x825ce24c
  if (!cr6.eq) goto loc_825CE24C;
  // lwz r11,52(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ce204
  if (!cr6.eq) goto loc_825CE204;
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // mr r11,r28
  r11 = r28;
  // beq cr6,0x825ce208
  if (cr6.eq) goto loc_825CE208;
loc_825CE204:
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
loc_825CE208:
  // lwz r9,440(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 440);
  // clrlwi r5,r11,16
  r5 = uint32_t(r11) & 0xFFFF;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x825ce228
  if (cr6.eq) goto loc_825CE228;
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lhz r9,24(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 24);
  // lhz r5,20(r11)
  r5 = guest::LoadU16(base, uint32_t(r11) + 20);
  // clrlwi r29,r9,30
  r29 = uint32_t(r9) & 0x3;
loc_825CE228:
  // addi r3,r10,4
  r3 = uint64_t(int64_t(int64_t(r10) + 4));
  // mr r6,r29
  r6 = r29;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CE244, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // bl 0x825cbb28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Play(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_825CE24C:
  // lwz r11,416(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ce25c
  if (!cr6.eq) goto loc_825CE25C;
  // stw r27,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r27));
loc_825CE25C:
  // mr r3,r31
  r3 = r31;
  // bl 0x825c4e88
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::AudioUpdate(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ce274
  if (cr6.eq) goto loc_825CE274;
  // bl 0x825cbd10
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::AudioUpdate(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CE274:
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ce284
  if (cr6.eq) goto loc_825CE284;
  // bl 0x825cbd10
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::AudioUpdate(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CE284:
  // lwz r3,420(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 420);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ce294
  if (cr6.eq) goto loc_825CE294;
  // bl 0x825cbd10
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::AudioUpdate(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CE294:
  // lwz r11,356(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 356);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // bne cr6,0x825ce2ec
  if (!cr6.eq) goto loc_825CE2EC;
  // lwz r11,52(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ce2ec
  if (!cr6.eq) goto loc_825CE2EC;
  // lwz r11,416(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ce2ec
  if (!cr6.eq) goto loc_825CE2EC;
  // lwz r11,412(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ce2ec
  if (!cr6.eq) goto loc_825CE2EC;
  // lwz r11,420(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 420);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ce2ec
  if (!cr6.eq) goto loc_825CE2EC;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,32
  r10 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // lwz r11,116(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 116);
  // stw r10,356(r31)
  guest::StoreU32(base, uint32_t(r31) + 356, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CE2EC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CE2EC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CE2F8: lifted from generated/ by lift.py.
lifted::Result f825CE2F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CE2F8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r27,r11,144
  r27 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CE31C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // lwz r30,296(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 296);
  // stw r29,392(r31)
  guest::StoreU32(base, uint32_t(r31) + 392, uint32_t(r29));
  // lwz r10,48(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 48);
  // mr r3,r30
  r3 = r30;
  // stw r11,356(r31)
  guest::StoreU32(base, uint32_t(r31) + 356, uint32_t(r11));
  // lwz r11,0(r30)
  r11 = lifted::VtableWord(base, uint32_t(r30));
  // lwz r4,1(r10)
  r4 = guest::LoadU32(base, uint32_t(r10) + 1);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CE348, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,48(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 48);
  // lwz r10,0(r30)
  r10 = lifted::VtableWord(base, uint32_t(r30));
  // mr r29,r3
  r29 = r3;
  // mr r3,r30
  r3 = r30;
  // lwz r4,5(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 5);
  // lwz r11,36(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 36);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CE368, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // mr r28,r3
  r28 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,84(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CE380, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ce43c
  if (cr6.lt) goto loc_825CE43C;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,16
  r3 = uint64_t(int64_t(16));
  // ori r4,r4,32791
  r4 = r4 | 32791;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CE39C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ce3bc
  if (cr6.eq) goto loc_825CE3BC;
  // mr r5,r29
  r5 = r29;
  // lwz r4,56(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 56);
  // bl 0x825d3360
  // (left out: dead in C++)
  game::XACTVariationTable::Ctor(base, r3, r4, r5);
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ce3d8
  if (!cr6.eq) goto loc_825CE3D8;
loc_825CE3BC:
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // mr r3,r27
  r3 = r27;
  // ori r30,r30,14
  r30 = r30 | 14;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CE3CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825CE3D8:
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // stw r11,300(r31)
  guest::StoreU32(base, uint32_t(r31) + 300, uint32_t(r11));
  // li r3,12
  r3 = uint64_t(int64_t(12));
  // ori r4,r4,32813
  r4 = r4 | 32813;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CE3EC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ce3bc
  if (cr6.eq) goto loc_825CE3BC;
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // mr r4,r28
  r4 = r28;
  // clrlwi r5,r11,16
  r5 = uint32_t(r11) & 0xFFFF;
  // bl 0x825d3688
  // (left out: dead in C++)
  game::XACTTransitionTable::Ctor(base, r3, r4, r5);
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ce3bc
  if (cr6.eq) goto loc_825CE3BC;
  // mr r3,r31
  r3 = r31;
  // stw r11,404(r31)
  guest::StoreU32(base, uint32_t(r31) + 404, uint32_t(r11));
  // bl 0x825cdb10
  // (left out: dead in C++)
  { const lifted::Result o = f825CDB10(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ce43c
  if (cr6.lt) goto loc_825CE43C;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r11,356(r31)
  guest::StoreU32(base, uint32_t(r31) + 356, uint32_t(r11));
  // bl 0x825c4a98
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::Notify(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_825CE43C:
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CE444, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CE450: lifted from generated/ by lift.py.
lifted::Result f825CE450(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CE450 (lifted)");
  uint64_t r0 = 0, r6 = 0, r7 = 0, r8 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // lwz r11,428(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 428);
  // lhz r10,24(r11)
  r10 = guest::LoadU16(base, uint32_t(r11) + 24);
  // rlwinm r8,r10,24,29,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0x7;
  // cmplwi cr6,r8,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 5, so);
  // bgt cr6,0x825ce5b8
  if (cr6.gt) goto loc_825CE5B8;
  // lis r12,-32163
  r12 = uint64_t(int64_t(-2107834368));
  // addi r12,r12,-7012
  r12 = uint64_t(int64_t(int64_t(r12) + -7012));
  // rlwinm r0,r8,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = guest::LoadU32(base, uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r8)) {
  case 0:
  goto loc_825CE4B4;
  case 1:
  goto loc_825CE4D0;
  case 2:
  goto loc_825CE534;
  case 3:
  goto loc_825CE534;
  case 4:
  goto loc_825CE590;
  case 5:
  goto loc_825CE590;
  default:
  std::abort();
  }
loc_825CE4B4:
  // lwz r6,392(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 392);
  // mr r3,r31
  r3 = r31;
  // cntlzw r11,r6
  r11 = uint32_t(r6) == 0 ? 32 : __builtin_clz(uint32_t(r6));
  // rlwinm r7,r11,27,31,31
  r7 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x825c5c80
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::PrepareSound(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // stw r30,392(r31)
  guest::StoreU32(base, uint32_t(r31) + 392, uint32_t(r30));
  // b 0x825ce5b8
  goto loc_825CE5B8;
loc_825CE4D0:
  // lwz r11,52(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 52);
  // mr r6,r30
  r6 = r30;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ce51c
  if (cr6.eq) goto loc_825CE51C;
  // lwz r10,80(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 80);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x825ce4f4
  if (cr6.eq) goto loc_825CE4F4;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bne cr6,0x825ce51c
  if (!cr6.eq) goto loc_825CE51C;
loc_825CE4F4:
  // lwz r8,56(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 56);
  // lwz r7,104(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 104);
  // lwz r11,100(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 100);
  // lwz r9,396(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 396);
  // lwz r10,380(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 380);
  // lwz r8,40(r8)
  r8 = guest::LoadU32(base, uint32_t(r8) + 40);
  // subf r8,r8,r7
  r8 = r7 - r8;
  // subf r11,r11,r8
  r11 = r8 - r11;
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r6,r11,r10
  r6 = r11 + r10;
loc_825CE51C:
  // lwz r11,392(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 392);
  // mr r3,r31
  r3 = r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r7,r11,27,31,31
  r7 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x825c5c80
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::PrepareSound(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x825ce5b8
  goto loc_825CE5B8;
loc_825CE534:
  // lwz r9,408(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 408);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x825ce57c
  if (cr6.eq) goto loc_825CE57C;
  // rlwinm r11,r10,0,21,23
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x700;
  // addi r11,r11,-768
  r11 = uint64_t(int64_t(int64_t(r11) + -768));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r10,r11,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // mr r11,r30
  r11 = r30;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x825ce560
  if (!cr6.eq) goto loc_825CE560;
  // lhz r11,48(r9)
  r11 = guest::LoadU16(base, uint32_t(r9) + 48);
loc_825CE560:
  // lwz r8,16(r9)
  r8 = guest::LoadU32(base, uint32_t(r9) + 16);
  // mr r9,r5
  r9 = r5;
  // clrlwi r6,r11,16
  r6 = uint32_t(r11) & 0xFFFF;
  // mr r5,r10
  r5 = r10;
  // lwz r8,7(r8)
  r8 = guest::LoadU32(base, uint32_t(r8) + 7);
  // mr r7,r8
  r7 = r8;
  // b 0x825ce5b0
  goto loc_825CE5B0;
loc_825CE57C:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5c80
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::PrepareSound(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x825ce5b8
  goto loc_825CE5B8;
loc_825CE590:
  // lwz r8,16(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mr r9,r5
  r9 = r5;
  // lwz r7,12(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 12);
  // rlwinm r11,r10,0,21,23
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x700;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r11,r11,-1024
  r11 = uint64_t(int64_t(int64_t(r11) + -1024));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r5,r11,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
loc_825CE5B0:
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5da0
  // (left out: dead in C++)
  { const lifted::Result o = f825C5DA0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_825CE5B8:
  // stw r30,392(r31)
  guest::StoreU32(base, uint32_t(r31) + 392, uint32_t(r30));
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

// sub_825CE5D8: lifted from generated/ by lift.py.
lifted::Result f825CE5D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CE5D8 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r27,r11,144
  r27 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CE5FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,356(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 356);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // mr r29,r28
  r29 = r28;
  // beq cr6,0x825ce700
  if (cr6.eq) goto loc_825CE700;
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // bne cr6,0x825ce630
  if (!cr6.eq) goto loc_825CE630;
  // lwz r11,60(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 60);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x825ce700
  if (cr6.eq) goto loc_825CE700;
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ce700
  if (cr6.eq) goto loc_825CE700;
loc_825CE630:
  // lwz r11,360(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 360);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825ce640
  if (cr6.eq) goto loc_825CE640;
  // li r30,1
  r30 = uint64_t(int64_t(1));
loc_825CE640:
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c4a78
  // (left out: dead in C++)
  f825C4A78(base, r3, r4);
  // lwz r11,60(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 60);
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x825ce6c8
  if (cr6.eq) goto loc_825CE6C8;
  // lwz r3,52(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ce684
  if (cr6.eq) goto loc_825CE684;
  // mr r4,r30
  r4 = r30;
  // bl 0x825cbc58
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Stop(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825ce700
  if (cr6.lt) goto loc_825CE700;
loc_825CE684:
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ce69c
  if (cr6.eq) goto loc_825CE69C;
  // mr r4,r30
  r4 = r30;
  // bl 0x825cbc58
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Stop(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
loc_825CE69C:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825ce700
  if (cr6.lt) goto loc_825CE700;
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ce6bc
  if (cr6.eq) goto loc_825CE6BC;
  // mr r4,r30
  r4 = r30;
  // bl 0x825cbc58
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Stop(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
loc_825CE6BC:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825ce700
  if (cr6.lt) goto loc_825CE700;
  // stw r28,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r28));
loc_825CE6C8:
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,356(r31)
  guest::StoreU32(base, uint32_t(r31) + 356, uint32_t(r11));
  // lwz r11,408(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 408);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ce6e4
  if (cr6.eq) goto loc_825CE6E4;
  // stw r28,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r28));
  // stw r28,408(r31)
  guest::StoreU32(base, uint32_t(r31) + 408, uint32_t(r28));
loc_825CE6E4:
  // mr r3,r31
  r3 = r31;
  // stw r28,360(r31)
  guest::StoreU32(base, uint32_t(r31) + 360, uint32_t(r28));
  // stw r28,380(r31)
  guest::StoreU32(base, uint32_t(r31) + 380, uint32_t(r28));
  // stw r28,384(r31)
  guest::StoreU32(base, uint32_t(r31) + 384, uint32_t(r28));
  // stw r28,388(r31)
  guest::StoreU32(base, uint32_t(r31) + 388, uint32_t(r28));
  // stw r28,376(r31)
  guest::StoreU32(base, uint32_t(r31) + 376, uint32_t(r28));
  // bl 0x825cdce0
  // (left out: dead in C++)
  r3 = f825CDCE0(base, r1, r13, r3, 0x825CE700);
loc_825CE700:
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CE708, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CE718: lifted from generated/ by lift.py.
lifted::Result f825CE718(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CE718 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // addi r4,r31,352
  r4 = uint64_t(int64_t(int64_t(r31) + 352));
  // bne cr6,0x825ce73c
  if (!cr6.eq) goto loc_825CE73C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_825CE73C:
  // lwz r11,368(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 368);
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x825c60c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C60C0, LIFTED_ARGS(0x825CE748, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,368(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 368);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lfs f0,20(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 20);
  f0 = double(temp.f32);
  // stfs f0,372(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 372, temp.u32);
  // stw r10,376(r31)
  guest::StoreU32(base, uint32_t(r31) + 376, uint32_t(r10));
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

// sub_825CE770: lifted from generated/ by lift.py.
lifted::Result f825CE770(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CE770 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // bl 0x825cdd30
  // (left out: dead in C++)
  { const lifted::Result o = f825CDD30(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ce7b0
  if (cr6.eq) goto loc_825CE7B0;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32775
  r4 = r4 | 32775;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x825CE7AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825CE7B0:
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

// sub_825CE7C8: lifted from generated/ by lift.py.
lifted::Result f825CE7C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CE7C8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-64(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -64, std::bit_cast<uint64_t>(f31));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f31,f1
  f31 = f1;
  // mr r28,r5
  r28 = r5;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r27,r11,144
  r27 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CE7F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // sth r11,80(r1)
  guest::StoreU16(base, uint32_t(r1) + 80, uint16_t(r11));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r26,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r26));
  // lhz r4,424(r31)
  r4 = guest::LoadU16(base, uint32_t(r31) + 424);
  // lwz r29,300(r31)
  r29 = guest::LoadU32(base, uint32_t(r31) + 300);
  // cmplwi cr6,r4,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 65535, so);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // beq cr6,0x825ce82c
  if (cr6.eq) goto loc_825CE82C;
  // mr r3,r29
  r3 = r29;
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x825d2ea0
  // (left out: dead in C++)
  r3 = f825D2EA0(base, r1, r3, r4, f1);
loc_825CE82C:
  // lwz r11,60(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 60);
  // mr r30,r26
  r30 = r26;
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825ce874
  if (!cr6.eq) goto loc_825CE874;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825ce850
  if (!cr6.eq) goto loc_825CE850;
  // lwz r11,356(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 356);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // beq cr6,0x825ce888
  if (cr6.eq) goto loc_825CE888;
loc_825CE850:
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r29
  r3 = r29;
  // bl 0x825d2d68
  // (left out: dead in C++)
  { const lifted::Result o = f825D2D68(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ce94c
  if (cr6.lt) goto loc_825CE94C;
  // b 0x825ce888
  goto loc_825CE888;
loc_825CE874:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825ce94c
  if (!cr6.eq) goto loc_825CE94C;
  // lwz r11,356(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 356);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // bne cr6,0x825ce94c
  if (!cr6.eq) goto loc_825CE94C;
loc_825CE888:
  // lhz r4,424(r31)
  r4 = guest::LoadU16(base, uint32_t(r31) + 424);
  // lhz r5,80(r1)
  r5 = guest::LoadU16(base, uint32_t(r1) + 80);
  // mr r10,r4
  r10 = r4;
  // mr r11,r5
  r11 = r5;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // beq cr6,0x825ce94c
  if (cr6.eq) goto loc_825CE94C;
  // addi r29,r31,428
  r29 = uint64_t(int64_t(int64_t(r31) + 428));
  // lwz r3,404(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 404);
  // mr r6,r29
  r6 = r29;
  // bl 0x825d3778
  // (left out: dead in C++)
  r3 = f825D3778(base, r1, r13, r3, r4, r5, r6);
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ce8c4
  if (cr6.lt) goto loc_825CE8C4;
  // mr r3,r31
  r3 = r31;
  // bl 0x825cd6a8
  // (left out: dead in C++)
  { const lifted::Result o = f825CD6A8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CE8C4:
  // stw r26,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r26));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ce94c
  if (cr6.lt) goto loc_825CE94C;
  // lhz r11,80(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 80);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825ce8f0
  if (cr6.eq) goto loc_825CE8F0;
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // mr r3,r31
  r3 = r31;
  // bl 0x825ce450
  // (left out: dead in C++)
  { const lifted::Result o = f825CE450(base, r1, r13, r3, r4, r5, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825CE8F0:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ce94c
  if (cr6.lt) goto loc_825CE94C;
  // lhz r10,80(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + 80);
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // stw r26,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r26));
  // sth r10,426(r31)
  guest::StoreU16(base, uint32_t(r31) + 426, uint16_t(r10));
  // lwz r10,88(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 88);
  // stw r10,416(r31)
  guest::StoreU32(base, uint32_t(r31) + 416, uint32_t(r10));
  // lwz r4,0(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpwi cr6,r4,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), -1, so);
  // beq cr6,0x825ce934
  if (cr6.eq) goto loc_825CE934;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5c80
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::PrepareSound(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825CE934:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ce94c
  if (cr6.lt) goto loc_825CE94C;
  // lwz r10,92(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 92);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r10,412(r31)
  guest::StoreU32(base, uint32_t(r31) + 412, uint32_t(r10));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
loc_825CE94C:
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CE954, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CE968: lifted from generated/ by lift.py.
lifted::Result f825CE968(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CE968 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,56(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 56);
  // addi r29,r11,144
  r29 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(base, LIFTED_ARGS(0x825CE988, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,356(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 356);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x825ce9a4
  if (!cr6.eq) goto loc_825CE9A4;
  // lwz r11,360(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 360);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825ce9ac
  if (cr6.eq) goto loc_825CE9AC;
loc_825CE9A4:
  // lis r30,-30009
  r30 = uint64_t(int64_t(-1966669824));
  // ori r30,r30,6
  r30 = r30 | 6;
loc_825CE9AC:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825cea08
  if (cr6.lt) goto loc_825CEA08;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,124(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 124);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CE9C8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // beq cr6,0x825cea1c
  if (cr6.eq) goto loc_825CEA1C;
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // beq cr6,0x825cea08
  if (cr6.eq) goto loc_825CEA08;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(base, LIFTED_ARGS(0x825CE9DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // stw r11,380(r31)
  guest::StoreU32(base, uint32_t(r31) + 380, uint32_t(r11));
  // bl 0x825ce718
  // (left out: dead in C++)
  { const lifted::Result o = f825CE718(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // lwz r11,120(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 120);
  // stw r10,356(r31)
  guest::StoreU32(base, uint32_t(r31) + 356, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CEA08, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CEA08:
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CEA10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825CEA1C:
  // lis r30,-30009
  r30 = uint64_t(int64_t(-1966669824));
  // mr r3,r29
  r3 = r29;
  // ori r30,r30,8
  r30 = r30 | 8;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(base, LIFTED_ARGS(0x825CEA2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CEA38: lifted from generated/ by lift.py.
lifted::Result f825CEA38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CEA38 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,360(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 360);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825cea74
  if (!cr6.eq) goto loc_825CEA74;
  // lwz r11,376(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 376);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cea74
  if (cr6.eq) goto loc_825CEA74;
  // lwz r11,432(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 432);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cea98
  if (cr6.eq) goto loc_825CEA98;
loc_825CEA74:
  // lwz r11,60(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 60);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825ceb90
  if (!cr6.eq) goto loc_825CEB90;
  // lwz r11,356(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 356);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // bne cr6,0x825ceb90
  if (!cr6.eq) goto loc_825CEB90;
  // lwz r11,364(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 364);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825ceb90
  if (!cr6.eq) goto loc_825CEB90;
loc_825CEA98:
  // lwz r11,408(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 408);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ceab0
  if (cr6.eq) goto loc_825CEAB0;
  // stw r30,64(r11)
  guest::StoreU32(base, uint32_t(r11) + 64, uint32_t(r30));
  // stw r30,408(r31)
  guest::StoreU32(base, uint32_t(r31) + 408, uint32_t(r30));
loc_825CEAB0:
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ceae4
  if (cr6.eq) goto loc_825CEAE4;
  // lwz r11,80(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 80);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x825ceae4
  if (cr6.eq) goto loc_825CEAE4;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x825ceae4
  if (cr6.eq) goto loc_825CEAE4;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CEAE4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CEAE4:
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ceb18
  if (cr6.eq) goto loc_825CEB18;
  // lwz r11,80(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 80);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x825ceb18
  if (cr6.eq) goto loc_825CEB18;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x825ceb18
  if (cr6.eq) goto loc_825CEB18;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CEB18, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CEB18:
  // lwz r11,60(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 60);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825ceb34
  if (!cr6.eq) goto loc_825CEB34;
  // lwz r11,356(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 356);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // mr r11,r30
  r11 = r30;
  // beq cr6,0x825ceb38
  if (cr6.eq) goto loc_825CEB38;
loc_825CEB34:
  // lwz r11,432(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 432);
loc_825CEB38:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,372(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 372);
  f1 = double(temp.f32);
  // mr r3,r31
  r3 = r31;
  // stw r30,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r30));
  // stw r11,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r11));
  // bl 0x825ce7c8
  // (left out: dead in C++)
  { const lifted::Result o = f825CE7C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825ceb8c
  if (cr6.lt) goto loc_825CEB8C;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825ceb8c
  if (cr6.eq) goto loc_825CEB8C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r10,60(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 60);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,432(r31)
  guest::StoreU32(base, uint32_t(r31) + 432, uint32_t(r11));
  // bne cr6,0x825ceb84
  if (!cr6.eq) goto loc_825CEB84;
  // lwz r10,356(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 356);
  // cmplwi cr6,r10,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 16, so);
  // beq cr6,0x825ceb88
  if (cr6.eq) goto loc_825CEB88;
loc_825CEB84:
  // mr r11,r30
  r11 = r30;
loc_825CEB88:
  // stw r11,364(r31)
  guest::StoreU32(base, uint32_t(r31) + 364, uint32_t(r11));
loc_825CEB8C:
  // stw r30,376(r31)
  guest::StoreU32(base, uint32_t(r31) + 376, uint32_t(r30));
loc_825CEB90:
  // lwz r3,412(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 412);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ceba0
  if (cr6.eq) goto loc_825CEBA0;
  // bl 0x825cb040
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Update(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CEBA0:
  // lwz r3,416(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 416);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cebb0
  if (cr6.eq) goto loc_825CEBB0;
  // bl 0x825cb040
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Update(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CEBB0:
  // lwz r3,420(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 420);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cebc0
  if (cr6.eq) goto loc_825CEBC0;
  // bl 0x825cb040
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Update(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CEBC0:
  // mr r3,r31
  r3 = r31;
  // bl 0x825c4f00
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::Update(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_825CEC38 game::XACTWaveBankBase::Dtor: lifted from generated/ by lift.py.
lifted::Result f825CEC38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825CEC38 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,24896
  r11 = uint64_t(int64_t(int64_t(r11) + 24896));
  // addi r10,r10,24604
  r10 = uint64_t(int64_t(int64_t(r10) + 24604));
  // addi r9,r9,24892
  r9 = uint64_t(int64_t(int64_t(r9) + 24892));
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // stw r10,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r10));
  // stw r9,8(r3)
  guest::StoreU32(base, uint32_t(r3) + 8, uint32_t(r9));
  // b 0x825cc6d0
  { const lifted::Result o = game::XACTWaveBankRoot::Dtor(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CEC60: lifted from generated/ by lift.py.
lifted::Result f825CEC60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825CEC60 (lifted)");
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x825cec68
  { const lifted::Result o = f825CEC68(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CEC68: lifted from generated/ by lift.py.
lifted::Result f825CEC68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825CEC68 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,24896
  r11 = uint64_t(int64_t(int64_t(r11) + 24896));
  // addi r10,r10,24604
  r10 = uint64_t(int64_t(int64_t(r10) + 24604));
  // addi r9,r9,24892
  r9 = uint64_t(int64_t(int64_t(r9) + 24892));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r9));
  // bl 0x825cc6d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWaveBankRoot::Dtor(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ceccc
  if (cr6.eq) goto loc_825CECCC;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32863
  r4 = r4 | 32863;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x825CECC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825CECCC:
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

// sub_825CECE8 game::XACTWaveBank::CreateWave: lifted from generated/ by lift.py.
lifted::Result f825CECE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CECE8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CED14, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // blt cr6,0x825ced30
  if (cr6.lt) goto loc_825CED30;
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,13
  r3 = r3 | 13;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CED30:
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,96
  r3 = uint64_t(int64_t(96));
  // ori r4,r4,32794
  r4 = r4 | 32794;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CED40, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825ced68
  if (cr6.eq) goto loc_825CED68;
  // mr r7,r29
  r7 = r29;
  // lwz r4,20(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 20);
  // mr r6,r30
  r6 = r30;
  // mr r5,r31
  r5 = r31;
  // bl 0x825d37a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTInMemoryWave::Ctor(base, LIFTED_ARGS(0x825CED5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ced78
  if (!cr6.eq) goto loc_825CED78;
loc_825CED68:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CED78:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CEE50: lifted from generated/ by lift.py.
uint64_t f825CEE50(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CEE50 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,16(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 16);
  // rlwinm r3,r11,30,29,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x7;
  // blr
  return r3;
}

// sub_825CEE60: lifted from generated/ by lift.py.
uint64_t f825CEE60(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CEE60 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,16(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 16);
  // rlwinm r3,r11,27,14,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x3FFFF;
  // blr
  return r3;
}

// sub_825CEEE8: lifted from generated/ by lift.py.
uint64_t f825CEEE8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CEEE8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r9,16(r3)
  r9 = guest::LoadU32(base, uint32_t(r3) + 16);
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // clrlwi r11,r9,30
  r11 = uint32_t(r9) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825cef0c
  if (cr6.lt) goto loc_825CEF0C;
  // bne cr6,0x825cef10
  if (!cr6.eq) goto loc_825CEF10;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // b 0x825cef10
  goto loc_825CEF10;
loc_825CEF0C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_825CEF10:
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x825cef24
  if (cr6.lt) goto loc_825CEF24;
  // bnelr cr6
  if (!cr6.eq) return r3;
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // blr
  return r3;
loc_825CEF24:
  // rlwinm r11,r9,0,0,0
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0x80000000;
  // lis r10,-32768
  r10 = uint64_t(int64_t(-2147483648));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // beq cr6,0x825cef3c
  if (cr6.eq) goto loc_825CEF3C;
  // li r11,8
  r11 = uint64_t(int64_t(8));
loc_825CEF3C:
  // rlwinm r10,r9,30,29,31
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 30) & 0x7;
  // rlwinm r11,r11,29,19,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x1FFF;
  // mullw r3,r11,r10
  r3 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r10))));
  // blr
  return r3;
}

// sub_825CF150: lifted from generated/ by lift.py.
lifted::Result f825CF150(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825CF150 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r11,r3
  r11 = r3;
  // mr r31,r4
  r31 = r4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 16);
  // clrlwi r10,r10,30
  r10 = uint32_t(r10) & 0x3;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x825cf18c
  if (cr6.lt) goto loc_825CF18C;
  // bne cr6,0x825cf190
  if (!cr6.eq) goto loc_825CF190;
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // b 0x825cf190
  goto loc_825CF190;
loc_825CF18C:
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_825CF190:
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // blt cr6,0x825cf1e8
  if (cr6.lt) goto loc_825CF1E8;
  // bne cr6,0x825cf1f8
  if (!cr6.eq) goto loc_825CF1F8;
  // lwz r3,8(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lhz r4,4(r11)
  r4 = guest::LoadU16(base, uint32_t(r11) + 4);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF1B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 0);
  // rlwinm r11,r31,16,16,31
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 16) & 0xFFFF;
  // addi r9,r3,4
  r9 = uint64_t(int64_t(int64_t(r3) + 4));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x825cf1cc
  if (cr6.lt) goto loc_825CF1CC;
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
loc_825CF1CC:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r9
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
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
loc_825CF1E8:
  // mr r3,r11
  r3 = r11;
  // bl 0x825ceee8
  // (left out: dead in C++)
  r3 = f825CEEE8(base, r3);
  // twllei r3,0
  if (int32_t(r3) == 0 || uint32_t(r3) < 0u) lifted::Trap(0);
  // divwu r3,r31,r3
  r3 = uint32_t(uint32_t(r3) ? uint32_t(r31) / uint32_t(r3) : 0);
loc_825CF1F8:
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

// sub_825CF220: lifted from generated/ by lift.py.
lifted::Result f825CF220(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825CF220 (lifted)");
  uint64_t r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r5,r4
  r5 = r4;
  // lwz r11,32(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 32);
  // lwz r10,28(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 28);
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x825cf080
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF080, LIFTED_ARGS(0x825CF248, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,16(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 16);
  // clrlwi r11,r11,30
  r11 = uint32_t(r11) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825cf270
  if (cr6.lt) goto loc_825CF270;
  // bne cr6,0x825cf270
  if (!cr6.eq) goto loc_825CF270;
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24);
  // addis r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 65536));
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // ble cr6,0x825cf270
  if (!cr6.gt) goto loc_825CF270;
  // mr r3,r11
  r3 = r11;
loc_825CF270:
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

// sub_825CF308: lifted from generated/ by lift.py.
lifted::Result f825CF308(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825CF308 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r10,r11,24968
  r10 = uint64_t(int64_t(int64_t(r11) + 24968));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r9,r11,24604
  r9 = uint64_t(int64_t(int64_t(r11) + 24604));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r10,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r10));
  // addi r8,r11,24960
  r8 = uint64_t(int64_t(int64_t(r11) + 24960));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r9,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r9));
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // stw r8,8(r3)
  guest::StoreU32(base, uint32_t(r3) + 8, uint32_t(r8));
  // stw r11,308(r3)
  guest::StoreU32(base, uint32_t(r3) + 308, uint32_t(r11));
  // stw r11,296(r3)
  guest::StoreU32(base, uint32_t(r3) + 296, uint32_t(r11));
  // b 0x825cc6d0
  { const lifted::Result o = game::XACTWaveBankRoot::Dtor(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CF340: lifted from generated/ by lift.py.
lifted::Result f825CF340(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825CF340 (lifted)");
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x825cfa50
  { const lifted::Result o = f825CFA50(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CF348: lifted from generated/ by lift.py.
lifted::Result f825CF348(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CF348 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lwz r11,252(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 252);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825cf384
  if (!cr6.eq) goto loc_825CF384;
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,22
  r3 = r3 | 22;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825CF384:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF398, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // blt cr6,0x825cf3b4
  if (cr6.lt) goto loc_825CF3B4;
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,13
  r3 = r3 | 13;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825CF3B4:
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,180
  r3 = uint64_t(int64_t(180));
  // ori r4,r4,32795
  r4 = r4 | 32795;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CF3C4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cf3ec
  if (cr6.eq) goto loc_825CF3EC;
  // mr r7,r29
  r7 = r29;
  // lwz r4,20(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 20);
  // mr r6,r30
  r6 = r30;
  // mr r5,r31
  r5 = r31;
  // bl 0x825d3f28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTStreamingWave::Ctor(base, r1, r13, r3, r4, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825cf3fc
  if (!cr6.eq) goto loc_825CF3FC;
loc_825CF3EC:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825CF3FC:
  // mr r3,r27
  r3 = r27;
  // stw r11,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CF410: lifted from generated/ by lift.py.
lifted::Result f825CF410(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CF410 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r11,r3
  r11 = r3;
  // mr r3,r4
  r3 = r4;
  // mr r4,r5
  r4 = r5;
  // mr r5,r6
  r5 = r6;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r11,188(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 188);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF448, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825cf480
  if (!cr6.eq) goto loc_825CF480;
  // bl 0x82295f18
  // (left out: dead in C++)
  { const lifted::Result o = game::GetLastError(base, LIFTED_ARGS(0x825CF454, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,997
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 997, so);
  // beq cr6,0x825cf480
  if (cr6.eq) goto loc_825CF480;
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble cr6,0x825cf484
  if (!cr6.gt) goto loc_825CF484;
  // clrlwi r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  // oris r3,r11,32775
  r3 = r11 | 2147942400;
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
loc_825CF480:
  // mr r3,r31
  r3 = r31;
loc_825CF484:
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

// sub_825CF498: lifted from generated/ by lift.py.
lifted::Result f825CF498(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825CF498 (lifted)");
  uint64_t r7 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0;
  // mr r7,r6
  r7 = r6;
  // lwz r11,264(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 264);
  // mr r6,r5
  r6 = r5;
  // mr r5,r4
  r5 = r4;
  // lwz r10,8(r7)
  r10 = guest::LoadU32(base, uint32_t(r7) + 8);
  // add r11,r11,r10
  r11 = r11 + r10;
  // mr r10,r11
  r10 = r11;
  // rldicl r11,r11,32,32
  r11 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFF;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // stw r10,8(r7)
  guest::StoreU32(base, uint32_t(r7) + 8, uint32_t(r10));
  // lwz r10,12(r7)
  r10 = guest::LoadU32(base, uint32_t(r7) + 12);
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,12(r7)
  guest::StoreU32(base, uint32_t(r7) + 12, uint32_t(r11));
  // lwz r4,260(r3)
  r4 = guest::LoadU32(base, uint32_t(r3) + 260);
  // b 0x825cf410
  { const lifted::Result o = f825CF410(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CF4D8: lifted from generated/ by lift.py.
lifted::Result f825CF4D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CF4D8 (lifted)");
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
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // mr r31,r4
  r31 = r4;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF4FC, r31)); r3 = o.r3; f1 = o.f1; }
  // add r11,r3,r31
  r11 = r3 + r31;
  // twllei r3,0
  if (int32_t(r3) == 0 || uint32_t(r3) < 0u) lifted::Trap(0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // divwu r11,r11,r3
  r11 = uint32_t(uint32_t(r3) ? uint32_t(r11) / uint32_t(r3) : 0);
  // mullw r3,r11,r3
  r3 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
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

// sub_825CF528: lifted from generated/ by lift.py.
lifted::Result f825CF528(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CF528 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // mr r31,r4
  r31 = r4;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF54C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r31,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r3), so);
  // ble cr6,0x825cf57c
  if (!cr6.gt) goto loc_825CF57C;
  // divwu r11,r31,r3
  r11 = uint32_t(uint32_t(r3) ? uint32_t(r31) / uint32_t(r3) : 0);
  // twllei r3,0
  if (int32_t(r3) == 0 || uint32_t(r3) < 0u) lifted::Trap(0);
  // mullw r11,r11,r3
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // subf r3,r11,r31
  r3 = r31 - r11;
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
loc_825CF57C:
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

// sub_825CF598: lifted from generated/ by lift.py.
lifted::Result f825CF598(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CF598 (lifted)");
  uint64_t r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r24,r4
  r24 = r4;
  // mr r22,r5
  r22 = r5;
  // mr r21,r6
  r21 = r6;
  // mr r23,r7
  r23 = r7;
  // mr r28,r8
  r28 = r8;
  // bl 0x825ced88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED88, LIFTED_ARGS(0x825CF5C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // mr r4,r24
  r4 = r24;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825ced98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED98, LIFTED_ARGS(0x825CF5D4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82099ba0
  // (left out: dead in C++)
  r3 = game::XACTWaveEntry::GetPlayLength(base, r3);
  // mr r29,r3
  r29 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS(0x825CF5E8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x825cf68c
  if (!cr6.eq) goto loc_825CF68C;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cee50
  // (left out: dead in C++)
  r3 = f825CEE50(base, r3);
  // cmplwi cr6,r3,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 6, so);
  // bne cr6,0x825cf68c
  if (!cr6.eq) goto loc_825CF68C;
  // lis r11,10922
  r11 = uint64_t(int64_t(715784192));
  // ori r31,r11,43691
  r31 = r11 | 43691;
  // lhz r11,272(r30)
  r11 = guest::LoadU16(base, uint32_t(r30) + 272);
  // mulhw r10,r11,r31
  r10 = uint64_t(int64_t((int64_t(int32_t(r11)) * int64_t(int32_t(r31))) >> 32));
  // srawi r10,r10,1
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x1) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 1));
  // rlwinm r9,r10,1,31,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0x1;
  // add r10,r10,r9
  r10 = r10 + r9;
  // rlwinm r9,r10,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r9
  r10 = r10 + r9;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // subf. r10,r10,r11
  r10 = r11 - r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825cf68c
  if (cr0.eq) goto loc_825CF68C;
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
  // lwz r11,0(r30)
  r11 = lifted::VtableWord(base, uint32_t(r30));
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bge cr6,0x825cf65c
  if (!cr6.lt) goto loc_825CF65C;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF64C, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0xFFFFFFFE;
  // add r11,r3,r11
  r11 = r3 + r11;
  // rlwinm r26,r11,2,0,29
  r26 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // b 0x825cf6a8
  goto loc_825CF6A8;
loc_825CF65C:
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF660, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,272(r30)
  r11 = guest::LoadU16(base, uint32_t(r30) + 272);
  // mulhw r11,r11,r31
  r11 = uint64_t(int64_t((int64_t(int32_t(r11)) * int64_t(int32_t(r31))) >> 32));
  // srawi r11,r11,1
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x1) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 1));
  // rlwinm r10,r11,1,31,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0x1;
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // mullw r11,r11,r3
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
  // rlwinm r10,r11,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r26,r11,2,0,29
  r26 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // b 0x825cf6a8
  goto loc_825CF6A8;
loc_825CF68C:
  // lwz r11,0(r30)
  r11 = lifted::VtableWord(base, uint32_t(r30));
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF6A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,272(r30)
  r11 = guest::LoadU16(base, uint32_t(r30) + 272);
  // mullw r26,r11,r3
  r26 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
loc_825CF6A8:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS(0x825CF6B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // ori r31,r11,65535
  r31 = r11 | 65535;
  // bne cr6,0x825cf6c8
  if (!cr6.eq) goto loc_825CF6C8;
  // add r11,r26,r31
  r11 = r26 + r31;
  // rlwinm r26,r11,0,0,15
  r26 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFF0000;
loc_825CF6C8:
  // rlwinm r11,r26,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 1) & 0xFFFFFFFE;
  // li r25,2
  r25 = uint64_t(int64_t(2));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // ble cr6,0x825cf708
  if (!cr6.gt) goto loc_825CF708;
  // clrlwi r11,r23,24
  r11 = uint32_t(r23) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cf750
  if (cr6.eq) goto loc_825CF750;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cf210
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF210, LIFTED_ARGS(0x825CF6F0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r28,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r3), so);
  // bgt cr6,0x825cf750
  if (cr6.gt) goto loc_825CF750;
  // rlwinm r11,r26,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 1) & 0xFFFFFFFE;
  // add r11,r26,r11
  r11 = r26 + r11;
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bgt cr6,0x825cf750
  if (cr6.gt) goto loc_825CF750;
loc_825CF708:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS(0x825CF714, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x825cf728
  if (!cr6.eq) goto loc_825CF728;
  // add r11,r29,r31
  r11 = r29 + r31;
  // rlwinm r26,r11,0,0,15
  r26 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFF0000;
  // b 0x825cf750
  goto loc_825CF750;
loc_825CF728:
  // lwz r11,0(r30)
  r11 = lifted::VtableWord(base, uint32_t(r30));
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF73C, r31)); r3 = o.r3; f1 = o.f1; }
  // add r11,r3,r29
  r11 = r3 + r29;
  // twllei r3,0
  if (int32_t(r3) == 0 || uint32_t(r3) < 0u) lifted::Trap(0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // divwu r11,r11,r3
  r11 = uint32_t(uint32_t(r3) ? uint32_t(r11) / uint32_t(r3) : 0);
  // mullw r26,r11,r3
  r26 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
loc_825CF750:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r11,r1,84
  r11 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r29,r31
  r29 = r31;
  // mr r28,r31
  r28 = r31;
  // addi r27,r1,80
  r27 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r31,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r31));
  // stw r31,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r31));
loc_825CF76C:
  // cmpw cr6,r28,r25
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r25), so);
  // bge cr6,0x825cf818
  if (!cr6.lt) goto loc_825CF818;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,116
  r3 = uint64_t(int64_t(116));
  // ori r4,r4,32804
  r4 = r4 | 32804;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CF784, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cf79c
  if (cr6.eq) goto loc_825CF79C;
  // mr r5,r24
  r5 = r24;
  // mr r4,r30
  r4 = r30;
  // bl 0x825d52a8
  // (left out: dead in C++)
  r3 = game::XACTStreamEntry::Ctor(base, r1, r13, r3, r4, r5);
  // b 0x825cf7a0
  goto loc_825CF7A0;
loc_825CF79C:
  // mr r3,r31
  r3 = r31;
loc_825CF7A0:
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,0(r27)
  guest::StoreU32(base, uint32_t(r27) + 0, uint32_t(r3));
  // beq cr6,0x825cf7b4
  if (cr6.eq) goto loc_825CF7B4;
  // mr r29,r31
  r29 = r31;
  // b 0x825cf7bc
  goto loc_825CF7BC;
loc_825CF7B4:
  // lis r29,-32761
  r29 = uint64_t(int64_t(-2147024896));
  // ori r29,r29,14
  r29 = r29 | 14;
loc_825CF7BC:
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // addi r27,r27,4
  r27 = uint64_t(int64_t(int64_t(r27) + 4));
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge cr6,0x825cf76c
  if (!cr6.lt) goto loc_825CF76C;
loc_825CF7CC:
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // ble cr6,0x825cf80c
  if (!cr6.gt) goto loc_825CF80C;
  // addi r31,r1,80
  r31 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r25
  r30 = r25;
loc_825CF7DC:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cf7fc
  if (cr6.eq) goto loc_825CF7FC;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF7FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CF7FC:
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x825cf7dc
  if (!cr6.eq) goto loc_825CF7DC;
loc_825CF80C:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
loc_825CF818:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825cf7cc
  if (cr6.lt) goto loc_825CF7CC;
  // clrlwi r11,r23,24
  r11 = uint32_t(r23) & 0xFF;
  // lwz r28,80(r1)
  r28 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cf870
  if (cr6.eq) goto loc_825CF870;
  // cmpwi cr6,r25,1
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 1, so);
  // bne cr6,0x825cf870
  if (!cr6.eq) goto loc_825CF870;
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r5,r23
  r5 = r23;
  // mr r4,r26
  r4 = r26;
  // mr r3,r28
  r3 = r28;
  // bl 0x825d50f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTStreamEntry::SetUp(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
loc_825CF850:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825cf7cc
  if (cr6.lt) goto loc_825CF7CC;
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // mr r3,r29
  r3 = r29;
  // stw r28,0(r22)
  guest::StoreU32(base, uint32_t(r22) + 0, uint32_t(r28));
  // stw r11,0(r21)
  guest::StoreU32(base, uint32_t(r21) + 0, uint32_t(r11));
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
loc_825CF870:
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
loc_825CF874:
  // cmpw cr6,r31,r25
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r25), so);
  // bge cr6,0x825cf850
  if (!cr6.lt) goto loc_825CF850;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r26
  r4 = r26;
  // bl 0x825d50f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTStreamEntry::SetUp(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge cr6,0x825cf874
  if (!cr6.lt) goto loc_825CF874;
  // b 0x825cf7cc
  goto loc_825CF7CC;
}

// sub_825CF8A8: lifted from generated/ by lift.py.
lifted::Result f825CF8A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CF8A8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 12);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cf8e4
  if (cr6.eq) goto loc_825CF8E4;
  // bl 0x825d5088
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTStreamEntry::IsReading(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825cf8e4
  if (cr6.eq) goto loc_825CF8E4;
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 12);
  // bl 0x825d50e8
  // (left out: dead in C++)
  r3 = game::XACTStreamEntry::Release(base, r1, r13, r3);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r11));
loc_825CF8E4:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF8FC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825CF910: lifted from generated/ by lift.py.
lifted::Result f825CF910(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CF910 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x825d0020
  { const lifted::Result o = f825D0020(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CF918: lifted from generated/ by lift.py.
lifted::Result f825CF918(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825CF918 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,25072
  r11 = uint64_t(int64_t(int64_t(r11) + 25072));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r10,r10,25052
  r10 = uint64_t(int64_t(int64_t(r10) + 25052));
  // lwz r9,4(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // stw r10,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r10));
  // stw r11,8(r9)
  guest::StoreU32(base, uint32_t(r9) + 8, uint32_t(r11));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // lwz r3,24(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 24);
  // stw r31,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r31));
  // stw r31,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r31));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cf984
  if (cr6.eq) goto loc_825CF984;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CF984, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CF984:
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,-22856
  r11 = uint64_t(int64_t(int64_t(r11) + -22856));
  // addi r10,r10,21316
  r10 = uint64_t(int64_t(int64_t(r10) + 21316));
  // stw r11,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r11));
  // stw r10,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r10));
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

// sub_825CF9B0: lifted from generated/ by lift.py.
uint64_t f825CF9B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825CF9B0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x825cc620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CC620, LIFTED_ARGS(0x825CF9C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // addi r11,r31,296
  r11 = uint64_t(int64_t(int64_t(r31) + 296));
  // addi r7,r9,24968
  r7 = uint64_t(int64_t(int64_t(r9) + 24968));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // addi r10,r31,308
  r10 = uint64_t(int64_t(int64_t(r31) + 308));
  // addi r6,r9,24604
  r6 = uint64_t(int64_t(int64_t(r9) + 24604));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // stw r7,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r7));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r5,r9,24960
  r5 = uint64_t(int64_t(int64_t(r9) + 24960));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // stw r6,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r6));
  // addi r3,r31,276
  r3 = uint64_t(int64_t(int64_t(r31) + 276));
  // addi r8,r9,21316
  r8 = uint64_t(int64_t(int64_t(r9) + 21316));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r5,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r5));
  // li r5,20
  r5 = uint64_t(int64_t(20));
  // stw r11,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r11));
  // stw r11,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r11));
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // stw r8,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r8));
  // stw r10,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r10));
  // stw r10,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r10));
  // stw r9,260(r31)
  guest::StoreU32(base, uint32_t(r31) + 260, uint32_t(r9));
  // stw r9,264(r31)
  guest::StoreU32(base, uint32_t(r31) + 264, uint32_t(r9));
  // stw r9,268(r31)
  guest::StoreU32(base, uint32_t(r31) + 268, uint32_t(r9));
  // stw r9,272(r31)
  guest::StoreU32(base, uint32_t(r31) + 272, uint32_t(r9));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x825CFA38, r31)); r3 = o.r3; f1 = o.f1; }
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
  return r3;
}

// sub_825CFA50: lifted from generated/ by lift.py.
lifted::Result f825CFA50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825CFA50 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r10,r11,24968
  r10 = uint64_t(int64_t(int64_t(r11) + 24968));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r30,r4
  r30 = r4;
  // addi r9,r11,24604
  r9 = uint64_t(int64_t(int64_t(r11) + 24604));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r10,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r10));
  // addi r8,r11,24960
  r8 = uint64_t(int64_t(int64_t(r11) + 24960));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r9,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r9));
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // stw r8,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r8));
  // stw r11,308(r31)
  guest::StoreU32(base, uint32_t(r31) + 308, uint32_t(r11));
  // stw r11,296(r31)
  guest::StoreU32(base, uint32_t(r31) + 296, uint32_t(r11));
  // bl 0x825cc6d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWaveBankRoot::Dtor(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cfac4
  if (cr6.eq) goto loc_825CFAC4;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32859
  r4 = r4 | 32859;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x825CFAC0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825CFAC4:
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

// sub_825CFAE0: lifted from generated/ by lift.py.
lifted::Result f825CFAE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825CFAE0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,300(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 300);
  // addi r10,r3,296
  r10 = uint64_t(int64_t(int64_t(r3) + 296));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x825cfb0c
  if (cr6.eq) goto loc_825CFB0C;
  // clrlwi r9,r4,16
  r9 = uint32_t(r4) & 0xFFFF;
loc_825CFAF4:
  // lhz r8,20(r11)
  r8 = guest::LoadU16(base, uint32_t(r11) + 20);
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // beq cr6,0x825cfb10
  if (cr6.eq) goto loc_825CFB10;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825cfaf4
  if (!cr6.eq) goto loc_825CFAF4;
loc_825CFB0C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825CFB10:
  // addi r3,r11,12
  r3 = uint64_t(int64_t(int64_t(r11) + 12));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CFB28: lifted from generated/ by lift.py.
lifted::Result f825CFB28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CFB28 (lifted)");
  uint64_t r11 = 0, r12 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e64
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // li r21,0
  r21 = uint64_t(int64_t(0));
  // addi r20,r29,296
  r20 = uint64_t(int64_t(int64_t(r29) + 296));
  // mr r23,r4
  r23 = r4;
  // mr r19,r5
  r19 = r5;
  // lwz r11,300(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 300);
  // mr r28,r21
  r28 = r21;
  // cmplw cr6,r11,r20
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r20), so);
  // beq cr6,0x825cfb74
  if (cr6.eq) goto loc_825CFB74;
  // clrlwi r10,r23,16
  r10 = uint32_t(r23) & 0xFFFF;
loc_825CFB5C:
  // lhz r9,20(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 20);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // beq cr6,0x825cfc58
  if (cr6.eq) goto loc_825CFC58;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmplw cr6,r11,r20
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r20), so);
  // bne cr6,0x825cfb5c
  if (!cr6.eq) goto loc_825CFB5C;
loc_825CFB74:
  // mr r30,r21
  r30 = r21;
loc_825CFB78:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r22,r30
  r22 = r30;
  // bl 0x825ced88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED88, LIFTED_ARGS(0x825CFB84, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // mr r4,r23
  r4 = r23;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825ced98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED98, LIFTED_ARGS(0x825CFB94, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r21,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r21));
  // bl 0x825cf210
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF210, LIFTED_ARGS(0x825CFBA4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r21,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r21));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cf220
  // (left out: dead in C++)
  { const lifted::Result o = f825CF220(base, r1, r13, r3, r4, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = lifted::VtableWord(base, uint32_t(r29));
  // mr r3,r29
  r3 = r29;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CFBCC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r25,r31
  r25 = r31;
  // cmplw cr6,r31,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r3), so);
  // ble cr6,0x825cfbec
  if (!cr6.gt) goto loc_825CFBEC;
  // divwu r11,r31,r3
  r11 = uint32_t(uint32_t(r3) ? uint32_t(r31) / uint32_t(r3) : 0);
  // twllei r3,0
  if (int32_t(r3) == 0 || uint32_t(r3) < 0u) lifted::Trap(0);
  // mullw r11,r11,r3
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
  // subf r11,r11,r31
  r11 = r31 - r11;
  // subf r25,r11,r31
  r25 = r31 - r11;
loc_825CFBEC:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // subf r24,r25,r31
  r24 = r31 - r25;
  // bl 0x825cf288
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF288, LIFTED_ARGS(0x825CFBF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = lifted::VtableWord(base, uint32_t(r29));
  // mr r27,r3
  r27 = r3;
  // mr r3,r29
  r3 = r29;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CFC10, r31)); r3 = o.r3; f1 = o.f1; }
  // add r11,r3,r27
  r11 = r3 + r27;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // add r11,r11,r31
  r11 = r11 + r31;
  // twllei r3,0
  if (int32_t(r3) == 0 || uint32_t(r3) < 0u) lifted::Trap(0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // divwu r11,r11,r3
  r11 = uint32_t(uint32_t(r3) ? uint32_t(r11) / uint32_t(r3) : 0);
  // mullw r26,r11,r3
  r26 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
  // beq cr6,0x825cfc60
  if (cr6.eq) goto loc_825CFC60;
  // addi r3,r30,12
  r3 = uint64_t(int64_t(int64_t(r30) + 12));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CFC44, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,24(r30)
  r30 = guest::LoadU32(base, uint32_t(r30) + 24);
  // mr r3,r28
  r3 = r28;
  // stw r30,0(r19)
  guest::StoreU32(base, uint32_t(r19) + 0, uint32_t(r30));
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
loc_825CFC58:
  // mr r30,r11
  r30 = r11;
  // b 0x825cfb78
  goto loc_825CFB78;
loc_825CFC60:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS(0x825CFC68, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x825cfd0c
  if (!cr6.eq) goto loc_825CFD0C;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cee50
  // (left out: dead in C++)
  r3 = f825CEE50(base, r3);
  // cmplwi cr6,r3,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 6, so);
  // bne cr6,0x825cfd0c
  if (!cr6.eq) goto loc_825CFD0C;
  // lis r11,10922
  r11 = uint64_t(int64_t(715784192));
  // ori r31,r11,43691
  r31 = r11 | 43691;
  // lhz r11,272(r29)
  r11 = guest::LoadU16(base, uint32_t(r29) + 272);
  // mulhw r10,r11,r31
  r10 = uint64_t(int64_t((int64_t(int32_t(r11)) * int64_t(int32_t(r31))) >> 32));
  // srawi r10,r10,1
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x1) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 1));
  // rlwinm r9,r10,1,31,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0x1;
  // add r10,r10,r9
  r10 = r10 + r9;
  // rlwinm r9,r10,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r9
  r10 = r10 + r9;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // subf. r10,r10,r11
  r10 = r11 - r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825cfd0c
  if (cr0.eq) goto loc_825CFD0C;
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
  // lwz r11,0(r29)
  r11 = lifted::VtableWord(base, uint32_t(r29));
  // mr r3,r29
  r3 = r29;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bge cr6,0x825cfcdc
  if (!cr6.lt) goto loc_825CFCDC;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CFCCC, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0xFFFFFFFE;
  // add r11,r3,r11
  r11 = r3 + r11;
  // rlwinm r28,r11,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // b 0x825cfd28
  goto loc_825CFD28;
loc_825CFCDC:
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CFCE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,272(r29)
  r11 = guest::LoadU16(base, uint32_t(r29) + 272);
  // mulhw r11,r11,r31
  r11 = uint64_t(int64_t((int64_t(int32_t(r11)) * int64_t(int32_t(r31))) >> 32));
  // srawi r11,r11,1
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x1) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 1));
  // rlwinm r10,r11,1,31,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0x1;
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // mullw r11,r11,r3
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
  // rlwinm r10,r11,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r28,r11,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // b 0x825cfd28
  goto loc_825CFD28;
loc_825CFD0C:
  // lwz r11,0(r29)
  r11 = lifted::VtableWord(base, uint32_t(r29));
  // mr r3,r29
  r3 = r29;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CFD20, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,272(r29)
  r11 = guest::LoadU16(base, uint32_t(r29) + 272);
  // mullw r28,r11,r3
  r28 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
loc_825CFD28:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS(0x825CFD30, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x825cfd70
  if (!cr6.eq) goto loc_825CFD70;
  // add r11,r28,r24
  r11 = r28 + r24;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addis r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 65536));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // rlwinm r28,r11,0,0,15
  r28 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFF0000;
  // mr r4,r28
  r4 = r28;
  // bl 0x825cf150
  // (left out: dead in C++)
  { const lifted::Result o = f825CF150(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820a8958
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A8958, LIFTED_ARGS(0x825CFD60, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r31,31,1,31
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 31) & 0x7FFFFFFF;
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // bge cr6,0x825cfd70
  if (!cr6.lt) goto loc_825CFD70;
  // addis r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 65536));
loc_825CFD70:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825cfd90
  if (cr6.eq) goto loc_825CFD90;
  // subf r11,r24,r28
  r11 = r28 - r24;
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // bge cr6,0x825cfd90
  if (!cr6.lt) goto loc_825CFD90;
  // mr r31,r27
  r31 = r27;
  // subf r28,r25,r26
  r28 = r26 - r25;
  // b 0x825cfd94
  goto loc_825CFD94;
loc_825CFD90:
  // subf r31,r24,r28
  r31 = r28 - r24;
loc_825CFD94:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS(0x825CFD9C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x825cfdb8
  if (!cr6.eq) goto loc_825CFDB8;
  // mr r11,r31
  r11 = r31;
  // cmplwi cr6,r31,2048
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 2048, so);
  // ble cr6,0x825cfde0
  if (!cr6.gt) goto loc_825CFDE0;
  // clrlwi r11,r31,21
  r11 = uint32_t(r31) & 0x7FF;
  // b 0x825cfddc
  goto loc_825CFDDC;
loc_825CFDB8:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825ceee8
  // (left out: dead in C++)
  r3 = f825CEEE8(base, r3);
  // mr r11,r31
  r11 = r31;
  // cmplw cr6,r31,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r3), so);
  // ble cr6,0x825cfde0
  if (!cr6.gt) goto loc_825CFDE0;
  // divwu r11,r31,r3
  r11 = uint32_t(uint32_t(r3) ? uint32_t(r31) / uint32_t(r3) : 0);
  // twllei r3,0
  if (int32_t(r3) == 0 || uint32_t(r3) < 0u) lifted::Trap(0);
  // mullw r11,r11,r3
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
  // subf r11,r11,r31
  r11 = r31 - r11;
loc_825CFDDC:
  // subf r11,r11,r31
  r11 = r31 - r11;
loc_825CFDE0:
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,116
  r3 = uint64_t(int64_t(116));
  // ori r4,r4,32804
  r4 = r4 | 32804;
  // mr r27,r11
  r27 = r11;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CFDF4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825cfe54
  if (cr6.eq) goto loc_825CFE54;
  // mr r5,r23
  r5 = r23;
  // mr r4,r29
  r4 = r29;
  // bl 0x825d52a8
  // (left out: dead in C++)
  r3 = game::XACTStreamEntry::Ctor(base, r1, r13, r3, r4, r5);
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x825cfe5c
  if (!cr6.eq) goto loc_825CFE5C;
loc_825CFE14:
  // lis r28,-32761
  r28 = uint64_t(int64_t(-2147024896));
  // ori r28,r28,14
  r28 = r28 | 14;
loc_825CFE1C:
  // cmplwi cr6,r22,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r22), 0, so);
  // beq cr6,0x825cfe3c
  if (cr6.eq) goto loc_825CFE3C;
  // lwz r11,0(r22)
  r11 = lifted::VtableWord(base, uint32_t(r22));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r22
  r3 = r22;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CFE3C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CFE3C:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825cfe48
  if (cr6.lt) goto loc_825CFE48;
  // stw r30,0(r19)
  guest::StoreU32(base, uint32_t(r19) + 0, uint32_t(r30));
loc_825CFE48:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
loc_825CFE54:
  // mr r30,r21
  r30 = r21;
  // b 0x825cfe14
  goto loc_825CFE14;
loc_825CFE5C:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // bl 0x825d50f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTStreamEntry::SetUp(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825cfe1c
  if (cr6.lt) goto loc_825CFE1C;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,28
  r3 = uint64_t(int64_t(28));
  // ori r4,r4,32805
  r4 = r4 | 32805;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x825CFE8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825cff24
  if (cr6.eq) goto loc_825CFF24;
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // stw r31,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r31));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // stw r31,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r31));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,-22856
  r11 = uint64_t(int64_t(int64_t(r11) + -22856));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // addi r10,r10,25072
  r10 = uint64_t(int64_t(int64_t(r10) + 25072));
  // addi r9,r9,25052
  r9 = uint64_t(int64_t(int64_t(r9) + 25052));
  // mr r22,r31
  r22 = r31;
  // stw r11,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r11));
  // mr r5,r30
  r5 = r30;
  // stw r8,16(r31)
  guest::StoreU32(base, uint32_t(r31) + 16, uint32_t(r8));
  // mr r4,r23
  r4 = r23;
  // stw r10,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r10));
  // mr r3,r29
  r3 = r29;
  // stw r9,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r9));
  // sth r23,20(r31)
  guest::StoreU16(base, uint32_t(r31) + 20, uint16_t(r23));
  // stw r30,24(r31)
  guest::StoreU32(base, uint32_t(r31) + 24, uint32_t(r30));
  // stw r25,60(r30)
  guest::StoreU32(base, uint32_t(r30) + 60, uint32_t(r25));
  // stw r21,64(r30)
  guest::StoreU32(base, uint32_t(r30) + 64, uint32_t(r21));
  // stw r24,88(r30)
  guest::StoreU32(base, uint32_t(r30) + 88, uint32_t(r24));
  // stw r27,80(r30)
  guest::StoreU32(base, uint32_t(r30) + 80, uint32_t(r27));
  // stw r8,72(r30)
  guest::StoreU32(base, uint32_t(r30) + 72, uint32_t(r8));
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // stw r11,40(r30)
  guest::StoreU32(base, uint32_t(r30) + 40, uint32_t(r11));
  // lwz r11,0(r29)
  r11 = lifted::VtableWord(base, uint32_t(r29));
  // lwz r11,60(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CFF10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bge cr6,0x825cff50
  if (!cr6.lt) goto loc_825CFF50;
  // stw r21,72(r30)
  guest::StoreU32(base, uint32_t(r30) + 72, uint32_t(r21));
  // b 0x825cfe1c
  goto loc_825CFE1C;
loc_825CFF24:
  // lwz r11,0(r30)
  r11 = lifted::VtableWord(base, uint32_t(r30));
  // lis r31,-32761
  r31 = uint64_t(int64_t(-2147024896));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // ori r31,r31,14
  r31 = r31 | 14;
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CFF44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
loc_825CFF50:
  // stw r20,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r20));
  // lwz r11,8(r20)
  r11 = guest::LoadU32(base, uint32_t(r20) + 8);
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // lwz r11,8(r20)
  r11 = guest::LoadU32(base, uint32_t(r20) + 8);
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r20)
  guest::StoreU32(base, uint32_t(r20) + 8, uint32_t(r31));
  // b 0x825cfe3c
  goto loc_825CFE3C;
}

// sub_825CFF90: lifted from generated/ by lift.py.
lifted::Result f825CFF90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CFF90 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r31,312(r3)
  r31 = guest::LoadU32(base, uint32_t(r3) + 312);
  // addi r29,r3,308
  r29 = uint64_t(int64_t(int64_t(r3) + 308));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // beq cr6,0x825d0010
  if (cr6.eq) goto loc_825D0010;
loc_825CFFB0:
  // mr r3,r31
  r3 = r31;
  // lwz r30,4(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x825d5088
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTStreamEntry::IsReading(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825cffcc
  if (cr6.eq) goto loc_825CFFCC;
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // b 0x825d0004
  goto loc_825D0004;
loc_825CFFCC:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // mr r3,r31
  r3 = r31;
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r31,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r31));
  // stw r31,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r31));
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D0004, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D0004:
  // mr r31,r30
  r31 = r30;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // bne cr6,0x825cffb0
  if (!cr6.eq) goto loc_825CFFB0;
loc_825D0010:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D0020: lifted from generated/ by lift.py.
lifted::Result f825D0020(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D0020 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // bl 0x825cf918
  // (left out: dead in C++)
  { const lifted::Result o = f825CF918(base, r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d0060
  if (cr6.eq) goto loc_825D0060;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32805
  r4 = r4 | 32805;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x825D005C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D0060:
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

// sub_825D0078: lifted from generated/ by lift.py.
lifted::Result f825D0078(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D0078 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,252(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 252);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825d00e8
  if (!cr6.eq) goto loc_825D00E8;
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r3,260(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 260);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r31,276
  r4 = uint64_t(int64_t(int64_t(r31) + 276));
  // lwz r11,192(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 192);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D00B8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825d00f8
  if (cr6.eq) goto loc_825D00F8;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 80);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D00D4, r31)); r3 = o.r3; f1 = o.f1; }
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
loc_825D00E8:
  // mr r3,r31
  r3 = r31;
  // bl 0x825cff90
  // (left out: dead in C++)
  { const lifted::Result o = f825CFF90(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825cc560
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWaveBank::DoWork(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825D00F8:
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

// sub_825D03B0 game::XACTTrack::UsesWaveBank: lifted from generated/ by lift.py.
lifted::Result f825D03B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D03B0 (lifted)");
  uint64_t r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r3,24(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 24);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d03ec
  if (cr6.eq) goto loc_825D03EC;
  // bl 0x825d5620
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrackInstance::UsesWaveBank(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825d03ec
  if (cr6.eq) goto loc_825D03EC;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825d0410
  goto loc_825D0410;
loc_825D03EC:
  // lwz r3,28(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 28);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d040c
  if (cr6.eq) goto loc_825D040C;
  // mr r4,r31
  r4 = r31;
  // bl 0x825d5620
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrackInstance::UsesWaveBank(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bne cr6,0x825d0410
  if (!cr6.eq) goto loc_825D0410;
loc_825D040C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825D0410:
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

// sub_825D0510 game::XACTTrack::SetOutputVoices: lifted from generated/ by lift.py.
lifted::Result f825D0510(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D0510 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,24(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 24);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825d56d8
  { const lifted::Result o = game::XACTTrackInstance::SetOutputVoices(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D0528 game::XACTTrack::SetOutputMatrices: lifted from generated/ by lift.py.
lifted::Result f825D0528(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D0528 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,24(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 24);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825d56f8
  { const lifted::Result o = game::XACTTrackInstance::SetOutputMatrices(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D0558 game::XACTTrack::SetMainMatrix: lifted from generated/ by lift.py.
lifted::Result f825D0558(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D0558 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,24(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 24);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825d5738
  { const lifted::Result o = game::XACTTrackInstance::SetMainMatrix(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D0570 game::XACTTrack::SetChannelMatrix: lifted from generated/ by lift.py.
lifted::Result f825D0570(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D0570 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,24(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 24);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // mr r3,r11
  r3 = r11;
  // b 0x825d5758
  { const lifted::Result o = game::XACTTrackInstance::SetChannelMatrix(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D05C0 game::XACTTrack::HasReleaseRpc: lifted from generated/ by lift.py.
lifted::Result f825D05C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D05C0 (lifted)");
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
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r3,20(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20);
  // lwz r11,28(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 28);
  // lbz r11,0(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 0);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x825d063c
  if (!cr6.eq) goto loc_825D063C;
  // lbz r4,48(r31)
  r4 = guest::LoadU8(base, uint32_t(r31) + 48);
  // bl 0x825cb348
  // (left out: dead in C++)
  { const lifted::Result o = f825CB348(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // clrlwi r8,r3,24
  r8 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x825d063c
  if (cr6.eq) goto loc_825D063C;
  // lwz r9,52(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 52);
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825D0608:
  // mulli r10,r11,44
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(44))));
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r10,20(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 20);
  // lhz r7,0(r10)
  r7 = guest::LoadU16(base, uint32_t(r10) + 0);
  // cmplwi cr6,r7,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 2, so);
  // bne cr6,0x825d062c
  if (!cr6.eq) goto loc_825D062C;
  // lhz r10,3(r10)
  r10 = guest::LoadU16(base, uint32_t(r10) + 3);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825d0654
  if (cr6.eq) goto loc_825D0654;
loc_825D062C:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x825d0608
  if (cr6.lt) goto loc_825D0608;
loc_825D063C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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
loc_825D0654:
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
  return {r3, f1};
}

// sub_825D0B08 game::XACTTrack::CreateNextInstance: lifted from generated/ by lift.py.
lifted::Result f825D0B08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D0B08 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,52(r4)
  r11 = guest::LoadU32(base, uint32_t(r4) + 52);
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825d0b34
  if (cr6.eq) goto loc_825D0B34;
  // lwz r11,16(r4)
  r11 = guest::LoadU32(base, uint32_t(r4) + 16);
  // lwz r4,41(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 41);
  // b 0x825d0b38
  goto loc_825D0B38;
loc_825D0B34:
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
loc_825D0B38:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // bl 0x825d02f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D02F8, LIFTED_ARGS(0x825D0B48, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825d0b58
  if (cr6.lt) goto loc_825D0B58;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // stw r11,28(r31)
  guest::StoreU32(base, uint32_t(r31) + 28, uint32_t(r11));
loc_825D0B58:
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

// sub_825D0B70 game::XACTTrack::SwitchInstance: lifted from generated/ by lift.py.
lifted::Result f825D0B70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825D0B70 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,28(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d0bd8
  if (cr6.eq) goto loc_825D0BD8;
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x825d0bd8
  if (!cr6.eq) goto loc_825D0BD8;
  // lwz r3,12(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 12);
  // lwz r4,24(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 24);
  // bl 0x825c1548
  // (left out: dead in C++)
  f825C1548(base, r3, r4);
  // lwz r11,28(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 28);
  // rotlwi r3,r11,0
  r3 = __builtin_rotateleft32(uint32_t(r11), 0);
  // stw r11,24(r31)
  guest::StoreU32(base, uint32_t(r31) + 24, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,28(r31)
  guest::StoreU32(base, uint32_t(r31) + 28, uint32_t(r11));
  // bl 0x825d5980
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D5980, LIFTED_ARGS(0x825D0BC4, r31)); r3 = o.r3; f1 = o.f1; }
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
loc_825D0BD8:
  // lbz r11,32(r31)
  r11 = guest::LoadU8(base, uint32_t(r31) + 32);
  // ori r11,r11,128
  r11 = r11 | 128;
  // stb r11,32(r31)
  guest::StoreU8(base, uint32_t(r31) + 32, uint8_t(r11));
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

// sub_825D0C78 game::XACTTrack::SetPitch: lifted from generated/ by lift.py.
lifted::Result f825D0C78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D0C78 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // stfd f30,-32(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -32, std::bit_cast<uint64_t>(f30));
  // stfd f31,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f30,f1
  f30 = f1;
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 24);
  // stfs f30,44(r31)
  temp.f32 = float(f30);
  guest::StoreU32(base, uint32_t(r31) + 44, temp.u32);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d0cc4
  if (cr6.eq) goto loc_825D0CC4;
  // lfs f31,60(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 60);
  f31 = double(temp.f32);
  // lwz r3,20(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20);
  // bl 0x825cac48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CAC48, LIFTED_ARGS(0x825D0CB4, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f0,f1,f31
  lifted::ScalarFloatMode();
  f0 = double(float(f1 + f31));
  // lwz r3,24(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 24);
  // fadds f1,f0,f30
  f1 = double(float(f0 + f30));
  // bl 0x825d56b8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrackInstance::SetPitch(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825D0CC4); r3 = o.r3; f1 = o.f1; }
loc_825D0CC4:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f30,-32(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -32)));
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D17B0 game::XACTEvent::SetQueued: lifted from generated/ by lift.py.
void f825D17B0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825D17B0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // cntlzw r11,r4
  r11 = uint32_t(r4) == 0 ? 32 : __builtin_clz(uint32_t(r4));
  // lbz r10,40(r3)
  r10 = guest::LoadU8(base, uint32_t(r3) + 40);
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // rlwimi r10,r11,5,26,26
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0x20) | (r10 & 0xFFFFFFFFFFFFFFDF);
  // stb r10,40(r3)
  guest::StoreU8(base, uint32_t(r3) + 40, uint8_t(r10));
  // blr
  return;
}

// sub_825D17D0 game::XACTEvent::Prepare: lifted from generated/ by lift.py.
uint64_t f825D17D0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825D17D0 (lifted)");
  uint64_t r11 = 0;
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r4,32(r11)
  guest::StoreU32(base, uint32_t(r11) + 32, uint32_t(r4));
  // blr
  return r3;
}

// sub_825D17E0 game::XACTEvent::Dtor: lifted from generated/ by lift.py.
lifted::Result f825D17E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D17E0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d181c
  if (cr6.eq) goto loc_825D181C;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32862
  r4 = r4 | 32862;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x825D1818, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D181C:
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

// sub_825D1830 game::XACTEvent::Cancel: lifted from generated/ by lift.py.
lifted::Result f825D1830(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D1830 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // stw r31,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r31));
  // stw r31,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r31));
  // lwz r11,16(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D1878, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,40(r31)
  r11 = guest::LoadU8(base, uint32_t(r31) + 40);
  // rlwinm r10,r11,0,28,28
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // cmplwi cr6,r10,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 8, so);
  // bne cr6,0x825d18a8
  if (!cr6.eq) goto loc_825D18A8;
  // mr r4,r31
  r4 = r31;
  // lwz r3,44(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 44);
  // bl 0x825d5e08
  // (left out: dead in C++)
  game::XACTTrackInstance::RemoveHelper(base, r3, r4);
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
loc_825D18A8:
  // rlwinm r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  // cmplwi cr6,r11,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 128, so);
  // bne cr6,0x825d18c4
  if (!cr6.eq) goto loc_825D18C4;
  // lwz r11,44(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 44);
  // lbz r10,45(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 45);
  // addi r10,r10,255
  r10 = uint64_t(int64_t(int64_t(r10) + 255));
  // stb r10,45(r11)
  guest::StoreU8(base, uint32_t(r11) + 45, uint8_t(r10));
loc_825D18C4:
  // lwz r11,44(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 44);
  // lbz r10,44(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 44);
  // addi r10,r10,255
  r10 = uint64_t(int64_t(int64_t(r10) + 255));
  // stb r10,44(r11)
  guest::StoreU8(base, uint32_t(r11) + 44, uint8_t(r10));
  // lwz r3,44(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 44);
  // bl 0x825d55e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D55E0, LIFTED_ARGS(0x825D18DC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825D18F0 game::XACTEvent::GetStartTime: lifted from generated/ by lift.py.
uint64_t f825D18F0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D18F0 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,16(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 16);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r3,r11,27,8,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0xFFFFFF;
  // rlwinm r11,r3,0,7,7
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x1000000;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // clrlwi r11,r3,8
  r11 = uint32_t(r3) & 0xFFFFFF;
  // neg r3,r11
  r3 = uint64_t(int64_t(static_cast<int64_t>(-r11)));
  // blr
  return r3;
}

// sub_825D1B40 game::XACTTransitionEvent::IsDue: lifted from generated/ by lift.py.
lifted::Result f825D1B40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D1B40 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0;
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
  // lwz r11,48(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 48);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825d1b74
  if (!cr6.eq) goto loc_825D1B74;
  // lwz r11,16(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 16);
  // lwz r3,12(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 12);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x825bf7a8
  // (left out: dead in C++)
  { const lifted::Result o = f825BF7A8(base, r1, r13, r3, r4, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // beq cr6,0x825d1b78
  if (cr6.eq) goto loc_825D1B78;
loc_825D1B74:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_825D1B78:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D1C10: lifted from generated/ by lift.py.
void f825D1C10(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D1C10 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r11,24(r3)
  r11 = guest::LoadU8(base, uint32_t(r3) + 24);
  // cmplwi cr6,r11,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 255, so);
  // beqlr cr6
  if (cr6.eq) return;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // addi r11,r11,255
  r11 = uint64_t(int64_t(int64_t(r11) + 255));
  // stb r11,24(r3)
  guest::StoreU8(base, uint32_t(r3) + 24, uint8_t(r11));
  // blr
  return;
}

// sub_825D1CE8 game::XACTWave::SetReverbSend: lifted from generated/ by lift.py.
lifted::Result f825D1CE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D1CE8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r3,44(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 44);
  // stfs f1,124(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r1) + 124, temp.u32);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d1d1c
  if (cr6.eq) goto loc_825D1D1C;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r1,124
  r4 = uint64_t(int64_t(int64_t(r1) + 124));
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D1D1C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D1D1C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D1D30 game::XACTWave::SetPitch: lifted from generated/ by lift.py.
lifted::Result f825D1D30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D1D30 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,44(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 44);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D1D50 game::XACTWave::SetOutputVoices: lifted from generated/ by lift.py.
lifted::Result f825D1D50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D1D50 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,44(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 44);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,28(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

lifted::Result e825CC3F8(uint8_t* base, const lifted::Args& a) {
  return f825CC3F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CC470(uint8_t* base, const lifted::Args& a) {
  return f825CC470(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CC560(uint8_t* base, const lifted::Args& a) {
  return f825CC560(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CC5C0(uint8_t* base, const lifted::Args& a) {
  return f825CC5C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825CC610(uint8_t* base, const lifted::Args& a) {
  return f825CC610(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e825CC6C8(uint8_t* base, const lifted::Args& a) {
  return f825CC6C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CC6D0(uint8_t* base, const lifted::Args& a) {
  return f825CC6D0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825CC7B8(uint8_t* base, const lifted::Args& a) {
  return f825CC7B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CC810(uint8_t* base, const lifted::Args& a) {
  return f825CC810(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e825CC8F0(uint8_t* base, const lifted::Args& a) {
  return {f825CC8F0(base, a.r[0]), a.f[0]};
}

lifted::Result e825CC900(uint8_t* base, const lifted::Args& a) {
  return f825CC900(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CC948(uint8_t* base, const lifted::Args& a) {
  return {f825CC948(base, a.r[0]), a.f[0]};
}

lifted::Result e825CC958(uint8_t* base, const lifted::Args& a) {
  return f825CC958(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CCA88(uint8_t* base, const lifted::Args& a) {
  return f825CCA88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CCC40(uint8_t* base, const lifted::Args& a) {
  return f825CCC40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e825CCC88(uint8_t* base, const lifted::Args& a) {
  return {f825CCC88(base, a.r[0]), a.f[0]};
}

lifted::Result e825CCC98(uint8_t* base, const lifted::Args& a) {
  return f825CCC98(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CCCE0(uint8_t* base, const lifted::Args& a) {
  return {f825CCCE0(base, a.r[0]), a.f[0]};
}

lifted::Result e825CCCF0(uint8_t* base, const lifted::Args& a) {
  return f825CCCF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CCD60(uint8_t* base, const lifted::Args& a) {
  return f825CCD60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e825CCDB0(uint8_t* base, const lifted::Args& a) {
  return f825CCDB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CCF10(uint8_t* base, const lifted::Args& a) {
  return f825CCF10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CD020(uint8_t* base, const lifted::Args& a) {
  return f825CD020(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CD150(uint8_t* base, const lifted::Args& a) {
  return f825CD150(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.lr);
}

lifted::Result e825CD180(uint8_t* base, const lifted::Args& a) {
  return {f825CD180(base, a.r[0]), a.f[0]};
}

lifted::Result e825CD198(uint8_t* base, const lifted::Args& a) {
  return {f825CD198(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825CD210(uint8_t* base, const lifted::Args& a) {
  return {f825CD210(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825CD270(uint8_t* base, const lifted::Args& a) {
  return f825CD270(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
}

lifted::Result e825CD350(uint8_t* base, const lifted::Args& a) {
  return f825CD350(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CD378(uint8_t* base, const lifted::Args& a) {
  return f825CD378(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825CD388(uint8_t* base, const lifted::Args& a) {
  f825CD388(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825CD398(uint8_t* base, const lifted::Args& a) {
  return f825CD398(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CD510(uint8_t* base, const lifted::Args& a) {
  return {f825CD510(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825CD620(uint8_t* base, const lifted::Args& a) {
  return f825CD620(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CD668(uint8_t* base, const lifted::Args& a) {
  return {f825CD668(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825CD6A8(uint8_t* base, const lifted::Args& a) {
  return f825CD6A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CD858(uint8_t* base, const lifted::Args& a) {
  return f825CD858(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CD990(uint8_t* base, const lifted::Args& a) {
  return f825CD990(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CDA40(uint8_t* base, const lifted::Args& a) {
  return f825CDA40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CDB10(uint8_t* base, const lifted::Args& a) {
  return f825CDB10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CDBD8(uint8_t* base, const lifted::Args& a) {
  return f825CDBD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CDC50(uint8_t* base, const lifted::Args& a) {
  return f825CDC50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CDCC8(uint8_t* base, const lifted::Args& a) {
  return f825CDCC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CDCE0(uint8_t* base, const lifted::Args& a) {
  return {f825CDCE0(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e825CDD30(uint8_t* base, const lifted::Args& a) {
  return f825CDD30(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e825CDEA8(uint8_t* base, const lifted::Args& a) {
  return f825CDEA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CE2F8(uint8_t* base, const lifted::Args& a) {
  return f825CE2F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CE450(uint8_t* base, const lifted::Args& a) {
  return f825CE450(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[6], a.f[0]);
}

lifted::Result e825CE5D8(uint8_t* base, const lifted::Args& a) {
  return f825CE5D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CE718(uint8_t* base, const lifted::Args& a) {
  return f825CE718(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CE770(uint8_t* base, const lifted::Args& a) {
  return f825CE770(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CE7C8(uint8_t* base, const lifted::Args& a) {
  return f825CE7C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CE968(uint8_t* base, const lifted::Args& a) {
  return f825CE968(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CEA38(uint8_t* base, const lifted::Args& a) {
  return f825CEA38(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CEC38(uint8_t* base, const lifted::Args& a) {
  return f825CEC38(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825CEC60(uint8_t* base, const lifted::Args& a) {
  return f825CEC60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825CEC68(uint8_t* base, const lifted::Args& a) {
  return f825CEC68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825CECE8(uint8_t* base, const lifted::Args& a) {
  return f825CECE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CEE50(uint8_t* base, const lifted::Args& a) {
  return {f825CEE50(base, a.r[0]), a.f[0]};
}

lifted::Result e825CEE60(uint8_t* base, const lifted::Args& a) {
  return {f825CEE60(base, a.r[0]), a.f[0]};
}

lifted::Result e825CEEE8(uint8_t* base, const lifted::Args& a) {
  return {f825CEEE8(base, a.r[0]), a.f[0]};
}

lifted::Result e825CF150(uint8_t* base, const lifted::Args& a) {
  return f825CF150(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825CF220(uint8_t* base, const lifted::Args& a) {
  return f825CF220(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825CF308(uint8_t* base, const lifted::Args& a) {
  return f825CF308(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825CF340(uint8_t* base, const lifted::Args& a) {
  return f825CF340(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825CF348(uint8_t* base, const lifted::Args& a) {
  return f825CF348(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CF410(uint8_t* base, const lifted::Args& a) {
  return f825CF410(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CF498(uint8_t* base, const lifted::Args& a) {
  return f825CF498(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[5], a.r[6]);
}

lifted::Result e825CF4D8(uint8_t* base, const lifted::Args& a) {
  return f825CF4D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CF528(uint8_t* base, const lifted::Args& a) {
  return f825CF528(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CF598(uint8_t* base, const lifted::Args& a) {
  return f825CF598(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CF8A8(uint8_t* base, const lifted::Args& a) {
  return f825CF8A8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CF910(uint8_t* base, const lifted::Args& a) {
  return f825CF910(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CF918(uint8_t* base, const lifted::Args& a) {
  return f825CF918(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825CF9B0(uint8_t* base, const lifted::Args& a) {
  return {f825CF9B0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825CFA50(uint8_t* base, const lifted::Args& a) {
  return f825CFA50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825CFAE0(uint8_t* base, const lifted::Args& a) {
  return f825CFAE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.lr);
}

lifted::Result e825CFB28(uint8_t* base, const lifted::Args& a) {
  return f825CFB28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CFF90(uint8_t* base, const lifted::Args& a) {
  return f825CFF90(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D0020(uint8_t* base, const lifted::Args& a) {
  return f825D0020(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D0078(uint8_t* base, const lifted::Args& a) {
  return f825D0078(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D03B0(uint8_t* base, const lifted::Args& a) {
  return f825D03B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D0510(uint8_t* base, const lifted::Args& a) {
  return f825D0510(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825D0528(uint8_t* base, const lifted::Args& a) {
  return f825D0528(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825D0558(uint8_t* base, const lifted::Args& a) {
  return f825D0558(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825D0570(uint8_t* base, const lifted::Args& a) {
  return f825D0570(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e825D05C0(uint8_t* base, const lifted::Args& a) {
  return f825D05C0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D0B08(uint8_t* base, const lifted::Args& a) {
  return f825D0B08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D0B70(uint8_t* base, const lifted::Args& a) {
  return f825D0B70(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825D0C78(uint8_t* base, const lifted::Args& a) {
  return f825D0C78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D17B0(uint8_t* base, const lifted::Args& a) {
  f825D17B0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D17D0(uint8_t* base, const lifted::Args& a) {
  return {f825D17D0(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825D17E0(uint8_t* base, const lifted::Args& a) {
  return f825D17E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D1830(uint8_t* base, const lifted::Args& a) {
  return f825D1830(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D18F0(uint8_t* base, const lifted::Args& a) {
  return {f825D18F0(base, a.r[0]), a.f[0]};
}

lifted::Result e825D1B40(uint8_t* base, const lifted::Args& a) {
  return f825D1B40(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D1C10(uint8_t* base, const lifted::Args& a) {
  f825D1C10(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D1CE8(uint8_t* base, const lifted::Args& a) {
  return f825D1CE8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D1D30(uint8_t* base, const lifted::Args& a) {
  return f825D1D30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825D1D50(uint8_t* base, const lifted::Args& a) {
  return f825D1D50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

namespace {

const Registration kEntries[] = {
    {0x825CC3F8, e825CC3F8},
    {0x825CC470, e825CC470},
    {0x825CC560, e825CC560},
    {0x825CC5C0, e825CC5C0},
    {0x825CC610, e825CC610},
    {0x825CC6C8, e825CC6C8},
    {0x825CC6D0, e825CC6D0},
    {0x825CC7B8, e825CC7B8},
    {0x825CC810, e825CC810},
    {0x825CC8F0, e825CC8F0},
    {0x825CC900, e825CC900},
    {0x825CC948, e825CC948},
    {0x825CC958, e825CC958},
    {0x825CCA88, e825CCA88},
    {0x825CCC40, e825CCC40},
    {0x825CCC88, e825CCC88},
    {0x825CCC98, e825CCC98},
    {0x825CCCE0, e825CCCE0},
    {0x825CCCF0, e825CCCF0},
    {0x825CCD60, e825CCD60},
    {0x825CCDB0, e825CCDB0},
    {0x825CCF10, e825CCF10},
    {0x825CD020, e825CD020},
    {0x825CD150, e825CD150},
    {0x825CD180, e825CD180},
    {0x825CD198, e825CD198},
    {0x825CD210, e825CD210},
    {0x825CD270, e825CD270},
    {0x825CD350, e825CD350},
    {0x825CD378, e825CD378},
    {0x825CD388, e825CD388},
    {0x825CD398, e825CD398},
    {0x825CD510, e825CD510},
    {0x825CD620, e825CD620},
    {0x825CD668, e825CD668},
    {0x825CD6A8, e825CD6A8},
    {0x825CD858, e825CD858},
    {0x825CD990, e825CD990},
    {0x825CDA40, e825CDA40},
    {0x825CDB10, e825CDB10},
    {0x825CDBD8, e825CDBD8},
    {0x825CDC50, e825CDC50},
    {0x825CDCC8, e825CDCC8},
    {0x825CDCE0, e825CDCE0},
    {0x825CDD30, e825CDD30},
    {0x825CDEA8, e825CDEA8},
    {0x825CE2F8, e825CE2F8},
    {0x825CE450, e825CE450},
    {0x825CE5D8, e825CE5D8},
    {0x825CE718, e825CE718},
    {0x825CE770, e825CE770},
    {0x825CE7C8, e825CE7C8},
    {0x825CE968, e825CE968},
    {0x825CEA38, e825CEA38},
    {0x825CEC38, e825CEC38},
    {0x825CEC60, e825CEC60},
    {0x825CEC68, e825CEC68},
    {0x825CECE8, e825CECE8},
    {0x825CEE50, e825CEE50},
    {0x825CEE60, e825CEE60},
    {0x825CEEE8, e825CEEE8},
    {0x825CF150, e825CF150},
    {0x825CF220, e825CF220},
    {0x825CF308, e825CF308},
    {0x825CF340, e825CF340},
    {0x825CF348, e825CF348},
    {0x825CF410, e825CF410},
    {0x825CF498, e825CF498},
    {0x825CF4D8, e825CF4D8},
    {0x825CF528, e825CF528},
    {0x825CF598, e825CF598},
    {0x825CF8A8, e825CF8A8},
    {0x825CF910, e825CF910},
    {0x825CF918, e825CF918},
    {0x825CF9B0, e825CF9B0},
    {0x825CFA50, e825CFA50},
    {0x825CFAE0, e825CFAE0},
    {0x825CFB28, e825CFB28},
    {0x825CFF90, e825CFF90},
    {0x825D0020, e825D0020},
    {0x825D0078, e825D0078},
    {0x825D03B0, e825D03B0},
    {0x825D0510, e825D0510},
    {0x825D0528, e825D0528},
    {0x825D0558, e825D0558},
    {0x825D0570, e825D0570},
    {0x825D05C0, e825D05C0},
    {0x825D0B08, e825D0B08},
    {0x825D0B70, e825D0B70},
    {0x825D0C78, e825D0C78},
    {0x825D17B0, e825D17B0},
    {0x825D17D0, e825D17D0},
    {0x825D17E0, e825D17E0},
    {0x825D1830, e825D1830},
    {0x825D18F0, e825D18F0},
    {0x825D1B40, e825D1B40},
    {0x825D1C10, e825D1C10},
    {0x825D1CE8, e825D1CE8},
    {0x825D1D30, e825D1D30},
    {0x825D1D50, e825D1D50},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_024)), true);

}  // namespace

}  // namespace lifted
