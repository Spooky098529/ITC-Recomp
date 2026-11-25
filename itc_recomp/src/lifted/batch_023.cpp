// Lift sweep batch 023 (docs/loops/lift-sweep.md): written by
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

ITC_FLAG_BOOL(native_lifted_023, true, "ITC",
              "Run lift sweep batch 023's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_825C68D8: lifted from generated/ by lift.py.
lifted::Result f825C68D8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C68D8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,108(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 108);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C68F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = guest::LoadU8(uint32_t(r3) + 0);
  // addi r11,r11,-255
  r11 = uint64_t(int64_t(int64_t(r11) + -255));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825C6918: lifted from generated/ by lift.py.
uint64_t f825C6918(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C6918 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,48(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 48);
  // addi r3,r11,9
  r3 = uint64_t(int64_t(int64_t(r11) + 9));
  // blr
  return r3;
}

// sub_825C6988 game::XACTSoundBank::Notify: lifted from generated/ by lift.py.
lifted::Result f825C6988(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_825C6988 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint8_t ca = 0;
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
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,81
  r3 = uint64_t(int64_t(int64_t(r1) + 81));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(LIFTED_ARGS(0x825C69B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 104);
  // lbz r10,108(r1)
  r10 = guest::LoadU8(uint32_t(r1) + 108);
  // rldicr r11,r11,8,63
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFFFF;
  // stb r30,80(r1)
  guest::StoreU8(uint32_t(r1) + 80, uint8_t(r30));
  // stb r31,92(r1)
  guest::StoreU8(uint32_t(r1) + 92, uint8_t(r31));
  // or r11,r10,r11
  r11 = r10 | r11;
  // lwz r3,24(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24);
  // srawi r10,r31,24
  ca = (int32_t(r31) < 0) & ((uint32_t(r31) & 0xFFFFFF) != 0);
  r10 = uint64_t(int64_t(int32_t(r31) >> 24));
  // ld r4,80(r1)
  r4 = guest::LoadU64(uint32_t(r1) + 80);
  // rldicr r7,r11,24,39
  r7 = __builtin_rotateleft64(r11, 24) & 0xFFFFFFFFFF000000;
  // ld r6,96(r1)
  r6 = guest::LoadU64(uint32_t(r1) + 96);
  // stb r10,89(r1)
  guest::StoreU8(uint32_t(r1) + 89, uint8_t(r10));
  // rldicl r10,r31,56,8
  r10 = __builtin_rotateleft64(r31, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,90(r1)
  guest::StoreU16(uint32_t(r1) + 90, uint16_t(r10));
  // ld r5,88(r1)
  r5 = guest::LoadU64(uint32_t(r1) + 88);
  // bl 0x825c1388
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTEngine::SendNotification(LIFTED_ARGS(0x825C69F4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825C6A10: lifted from generated/ by lift.py.
lifted::Result f825C6A10(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_825C6A10 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x825c7608
  { const lifted::Result o = f825C7608(r1, r13, r3, r4, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C6A18 game::XACTCue::DeletingDestructorThunk: lifted from generated/ by lift.py.
lifted::Result f825C6A18(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_825C6A18 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x825c7570
  { const lifted::Result o = game::XACTCue::Dtor(r1, r13, r3, r4, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C6B50 game::XACTSoundBank::Destroy: lifted from generated/ by lift.py.
lifted::Result f825C6B50(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C6B50 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,24(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(LIFTED_ARGS(0x825C6B70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,24(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24);
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = game::XACTEngine::CheckNotInCallback(r1, r13, r3);
  // mr r27,r3
  r27 = r3;
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // blt cr6,0x825c6bf8
  if (cr6.lt) goto loc_825C6BF8;
  // lwz r11,240(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 240);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825c6be0
  if (!cr6.eq) goto loc_825C6BE0;
  // addi r29,r31,28
  r29 = uint64_t(int64_t(int64_t(r31) + 28));
  // addi r30,r31,4
  r30 = uint64_t(int64_t(int64_t(r31) + 4));
loc_825C6BA0:
  // lwz r11,204(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c6bb4
  if (cr6.eq) goto loc_825C6BB4;
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // b 0x825c6bb8
  goto loc_825C6BB8;
loc_825C6BB4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825C6BB8:
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // mr r4,r30
  r4 = r30;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6BCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,240(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 240);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c6ba0
  if (cr6.eq) goto loc_825C6BA0;
loc_825C6BE0:
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6BF8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C6BF8:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825C6C00, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C6C10 game::XACTSoundBank::Prepare: lifted from generated/ by lift.py.
lifted::Result f825C6C10(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C6C10 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r27,r6
  r27 = r6;
  // mr r26,r7
  r26 = r7;
  // lwz r11,24(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24);
  // addi r25,r11,144
  r25 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(LIFTED_ARGS(0x825C6C3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,24(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24);
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = game::XACTEngine::CheckNotInCallback(r1, r13, r3);
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825c6d9c
  if (cr6.lt) goto loc_825C6D9C;
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bge cr6,0x825c6c64
  if (!cr6.lt) goto loc_825C6C64;
  // lis r28,-32761
  r28 = uint64_t(int64_t(-2147024896));
  // ori r28,r28,87
  r28 = r28 | 87;
  // b 0x825c6ca0
  goto loc_825C6CA0;
loc_825C6C64:
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // bne cr6,0x825c6c78
  if (!cr6.eq) goto loc_825C6C78;
  // lis r28,-32761
  r28 = uint64_t(int64_t(-2147024896));
  // ori r28,r28,87
  r28 = r28 | 87;
  // b 0x825c6ca0
  goto loc_825C6CA0;
loc_825C6C78:
  // clrlwi r30,r29,16
  r30 = uint32_t(r29) & 0xFFFF;
  // cmplwi cr6,r30,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 65535, so);
  // beq cr6,0x825c6c98
  if (cr6.eq) goto loc_825C6C98;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c6928
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSoundBank::GetCueCount(LIFTED_ARGS(0x825C6C8C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x825c6ca0
  if (cr6.lt) goto loc_825C6CA0;
loc_825C6C98:
  // lis r28,-30009
  r28 = uint64_t(int64_t(-1966669824));
  // ori r28,r28,12
  r28 = r28 | 12;
loc_825C6CA0:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825c6d9c
  if (cr6.lt) goto loc_825C6D9C;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // lwz r11,72(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 72);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6CC0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // lbz r11,0(r30)
  r11 = guest::LoadU8(uint32_t(r30) + 0);
  // rlwinm r10,r11,0,30,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825c6d0c
  if (cr6.eq) goto loc_825C6D0C;
  // ori r4,r4,32775
  r4 = r4 | 32775;
  // li r3,444
  r3 = uint64_t(int64_t(444));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(LIFTED_ARGS(0x825C6CE4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c6d04
  if (cr6.eq) goto loc_825C6D04;
  // mr r7,r29
  r7 = r29;
  // lwz r4,24(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 24);
  // mr r6,r30
  r6 = r30;
  // mr r5,r31
  r5 = r31;
  // bl 0x825cd270
  // (left out: dead in C++)
  { const lifted::Result o = f825CD270(r1, r13, r3, r4, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // b 0x825c6d68
  goto loc_825C6D68;
loc_825C6D04:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // b 0x825c6d74
  goto loc_825C6D74;
loc_825C6D0C:
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // li r3,352
  r3 = uint64_t(int64_t(352));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c6d44
  if (cr6.eq) goto loc_825C6D44;
  // ori r4,r4,32774
  r4 = r4 | 32774;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(LIFTED_ARGS(0x825C6D24, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c6d04
  if (cr6.eq) goto loc_825C6D04;
  // mr r7,r29
  r7 = r29;
  // lwz r4,24(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 24);
  // mr r6,r30
  r6 = r30;
  // mr r5,r31
  r5 = r31;
  // bl 0x825c6858
  // (left out: dead in C++)
  { const lifted::Result o = f825C6858(r1, r13, r3, r4, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // b 0x825c6d68
  goto loc_825C6D68;
loc_825C6D44:
  // ori r4,r4,32773
  r4 = r4 | 32773;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(LIFTED_ARGS(0x825C6D4C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c6d04
  if (cr6.eq) goto loc_825C6D04;
  // mr r7,r29
  r7 = r29;
  // lwz r4,24(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 24);
  // mr r6,r30
  r6 = r30;
  // mr r5,r31
  r5 = r31;
  // bl 0x825c67e8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::Ctor(LIFTED_ARGS(0x825C6D68, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C6D68:
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x825c6db0
  if (!cr6.eq) goto loc_825C6DB0;
loc_825C6D74:
  // lis r28,-32761
  r28 = uint64_t(int64_t(-2147024896));
  // ori r28,r28,14
  r28 = r28 | 14;
loc_825C6D7C:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c6d9c
  if (cr6.eq) goto loc_825C6D9C;
  // addi r3,r30,12
  r3 = uint64_t(int64_t(int64_t(r30) + 12));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6D9C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C6D9C:
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825C6DA4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C6DB0:
  // lwz r11,0(r30)
  r11 = lifted::VtableWord(uint32_t(r30));
  // mr r4,r27
  r4 = r27;
  // mr r3,r30
  r3 = r30;
  // lwz r11,80(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 80);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6DC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825c6d7c
  if (cr6.lt) goto loc_825C6D7C;
  // mr r4,r30
  r4 = r30;
  // stw r30,0(r26)
  guest::StoreU32(uint32_t(r26) + 0, uint32_t(r30));
  // lwz r3,24(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24);
  // bl 0x825c1518
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTEngine::AddCue(LIFTED_ARGS(0x825C6DE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825C6DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C6DF8: lifted from generated/ by lift.py.
lifted::Result f825C6DF8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C6DF8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r28,r4
  r28 = r4;
  // mr r25,r5
  r25 = r5;
  // mr r26,r6
  r26 = r6;
  // mr r24,r7
  r24 = r7;
  // lwz r11,24(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24);
  // addi r23,r11,144
  r23 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r23
  r3 = r23;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(LIFTED_ARGS(0x825C6E28, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,24(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24);
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = game::XACTEngine::CheckNotInCallback(r1, r13, r3);
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c6e5c
  if (cr6.lt) goto loc_825C6E5C;
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // bge cr6,0x825c6e50
  if (!cr6.lt) goto loc_825C6E50;
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // ori r30,r30,87
  r30 = r30 | 87;
  // b 0x825c6e5c
  goto loc_825C6E5C;
loc_825C6E50:
  // clrlwi r27,r28,16
  r27 = uint32_t(r28) & 0xFFFF;
  // cmplwi cr6,r27,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 65535, so);
  // beq cr6,0x825c6ea4
  if (cr6.eq) goto loc_825C6EA4;
loc_825C6E5C:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6E70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = lifted::VtableWord(uint32_t(r31));
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r10,64(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 64);
  // lhz r29,9(r11)
  r29 = guest::LoadU16(uint32_t(r11) + 9);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6E8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,11(r3)
  r11 = guest::LoadU16(uint32_t(r3) + 11);
  // clrlwi r27,r28,16
  r27 = uint32_t(r28) & 0xFFFF;
  // add r11,r11,r29
  r11 = r11 + r29;
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // blt cr6,0x825c6eac
  if (cr6.lt) goto loc_825C6EAC;
loc_825C6EA4:
  // lis r30,-30009
  r30 = uint64_t(int64_t(-1966669824));
  // ori r30,r30,12
  r30 = r30 | 12;
loc_825C6EAC:
  // cntlzw r11,r24
  r11 = uint32_t(r24) == 0 ? 32 : __builtin_clz(uint32_t(r24));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // rlwinm r29,r11,27,31,31
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // blt cr6,0x825c6f5c
  if (cr6.lt) goto loc_825C6F5C;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq cr6,0x825c6efc
  if (cr6.eq) goto loc_825C6EFC;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // lwz r11,72(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 72);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6EDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = guest::LoadU8(uint32_t(r3) + 0);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c6ef8
  if (cr6.eq) goto loc_825C6EF8;
  // lis r30,-30009
  r30 = uint64_t(int64_t(-1966669824));
  // ori r30,r30,6
  r30 = r30 | 6;
  // b 0x825c6f5c
  goto loc_825C6F5C;
loc_825C6EF8:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825C6EFC:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // lwz r11,72(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 72);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6F14, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = guest::LoadU8(uint32_t(r3) + 0);
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c6f5c
  if (cr6.eq) goto loc_825C6F5C;
  // lbz r11,9(r3)
  r11 = guest::LoadU8(uint32_t(r3) + 9);
  // cmplwi cr6,r11,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 255, so);
  // beq cr6,0x825c6f5c
  if (cr6.eq) goto loc_825C6F5C;
  // lwz r10,248(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 248);
  // rlwinm r9,r27,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r9,r10
  r10 = guest::LoadU32(uint32_t(r9) + uint32_t(r10));
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // blt cr6,0x825c6f5c
  if (cr6.lt) goto loc_825C6F5C;
  // lbz r11,14(r3)
  r11 = guest::LoadU8(uint32_t(r3) + 14);
  // rlwinm r11,r11,0,26,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x38;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c6f5c
  if (!cr6.eq) goto loc_825C6F5C;
  // lis r30,-30009
  r30 = uint64_t(int64_t(-1966669824));
  // ori r30,r30,8
  r30 = r30 | 8;
loc_825C6F5C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // blt cr6,0x825c7000
  if (cr6.lt) goto loc_825C7000;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r6,r26
  r6 = r26;
  // mr r5,r25
  r5 = r25;
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6F90, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c6fe4
  if (cr6.lt) goto loc_825C6FE4;
  // lwz r3,80(r1)
  r3 = guest::LoadU32(uint32_t(r1) + 80);
  // mr r4,r29
  r4 = r29;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6FB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c6fe4
  if (cr6.lt) goto loc_825C6FE4;
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // beq cr6,0x825c7000
  if (cr6.eq) goto loc_825C7000;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 80);
  // mr r3,r23
  r3 = r23;
  // stw r11,0(r24)
  guest::StoreU32(uint32_t(r24) + 0, uint32_t(r11));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825C6FD8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C6FE4:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(uint32_t(r1) + 80);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c7000
  if (cr6.eq) goto loc_825C7000;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7000, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C7000:
  // mr r3,r23
  r3 = r23;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825C7008, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C7018: lifted from generated/ by lift.py.
lifted::Result f825C7018(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C7018 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // lwz r11,24(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24);
  // addi r30,r11,144
  r30 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(LIFTED_ARGS(0x825C7040, r31)); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // clrlwi r6,r28,31
  r6 = uint32_t(r28) & 0x1;
  // lwz r3,24(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24);
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // bl 0x825c15e8
  // (left out: dead in C++)
  { const lifted::Result o = f825C15E8(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825C7064, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C7070 game::XACTSoundBank::GetState: lifted from generated/ by lift.py.
lifted::Result f825C7070(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C7070 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,24(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24);
  // addi r30,r11,144
  r30 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(LIFTED_ARGS(0x825C7094, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,240(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 240);
  // mr r3,r30
  r3 = r30;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // stw r11,0(r29)
  guest::StoreU32(uint32_t(r29) + 0, uint32_t(r11));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825C70B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C70C0: lifted from generated/ by lift.py.
lifted::Result f825C70C0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C70C0 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,24(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 24);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(LIFTED_ARGS(0x825C70E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // ori r27,r10,65535
  r27 = r10 | 65535;
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7100, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,8(r3)
  r11 = guest::LoadU8(uint32_t(r3) + 8);
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c7200
  if (cr6.eq) goto loc_825C7200;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7124, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C713C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r4,r29
  r4 = r29;
  // lwz r3,24(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 24);
  // lhz r5,15(r11)
  r5 = guest::LoadU16(uint32_t(r11) + 15);
  // bl 0x825bfc50
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTHashName(LIFTED_ARGS(0x825C7150, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,1,15,30
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0x1FFFE;
  // lhzx r4,r11,r30
  r4 = guest::LoadU16(uint32_t(r11) + uint32_t(r30));
  // cmplwi cr6,r4,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 65535, so);
  // beq cr6,0x825c7200
  if (cr6.eq) goto loc_825C7200;
loc_825C7160:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,76(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 76);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7174, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // lwz r4,0(r30)
  r4 = guest::LoadU32(uint32_t(r30) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7190, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r3
  r10 = r3;
  // mr r11,r29
  r11 = r29;
loc_825C7198:
  // lbz r9,0(r11)
  r9 = guest::LoadU8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = guest::LoadU8(uint32_t(r10) + 0);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq cr6,0x825c71bc
  if (cr6.eq) goto loc_825C71BC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x825c7198
  if (cr6.eq) goto loc_825C7198;
loc_825C71BC:
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x825c71e4
  if (cr6.eq) goto loc_825C71E4;
  // lhz r4,4(r30)
  r4 = guest::LoadU16(uint32_t(r30) + 4);
  // cmplwi cr6,r4,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 65535, so);
  // bne cr6,0x825c7160
  if (!cr6.eq) goto loc_825C7160;
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825C71D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C71E4:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C71FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
loc_825C7200:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825C7208, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C7510 game::XACTSoundBank::AddCueInstance: lifted from generated/ by lift.py.
lifted::Result f825C7510(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7510 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0;
  // mr r5,r4
  r5 = r4;
  // lwz r10,248(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 248);
  // mr r4,r3
  r4 = r3;
  // rlwinm r11,r5,2,14,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0x3FFFC;
  // lwzx r9,r11,r10
  r9 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // stwx r9,r11,r10
  guest::StoreU32(uint32_t(r11) + uint32_t(r10), uint32_t(r9));
  // lwz r10,248(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 248);
  // lwz r3,24(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 24);
  // lwzx r6,r11,r10
  r6 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // b 0x825c08c0
  { const lifted::Result o = f825C08C0(r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C7540 game::XACTSoundBank::RemoveCueInstance: lifted from generated/ by lift.py.
lifted::Result f825C7540(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7540 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0;
  // mr r5,r4
  r5 = r4;
  // lwz r10,248(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 248);
  // mr r4,r3
  r4 = r3;
  // rlwinm r11,r5,2,14,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0x3FFFC;
  // lwzx r9,r11,r10
  r9 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stwx r9,r11,r10
  guest::StoreU32(uint32_t(r11) + uint32_t(r10), uint32_t(r9));
  // lwz r10,248(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 248);
  // lwz r3,24(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 24);
  // lwzx r6,r11,r10
  r6 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // b 0x825c08c0
  { const lifted::Result o = f825C08C0(r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C7570 game::XACTCue::Dtor: lifted from generated/ by lift.py.
lifted::Result f825C7570(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7570 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,22792
  r11 = uint64_t(int64_t(int64_t(r11) + 22792));
  // addi r10,r10,22636
  r10 = uint64_t(int64_t(int64_t(r10) + 22636));
  // addi r9,r9,24748
  r9 = uint64_t(int64_t(int64_t(r9) + 24748));
  // addi r8,r8,22788
  r8 = uint64_t(int64_t(int64_t(r8) + 22788));
  // addi r7,r7,24732
  r7 = uint64_t(int64_t(int64_t(r7) + 24732));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r9));
  // stw r8,12(r31)
  guest::StoreU32(uint32_t(r31) + 12, uint32_t(r8));
  // stw r7,24(r31)
  guest::StoreU32(uint32_t(r31) + 24, uint32_t(r7));
  // bl 0x825c6148
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCueBase::Dtor(r1, r13, r3, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c75ec
  if (cr6.eq) goto loc_825C75EC;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32773
  r4 = r4 | 32773;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C75E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C75EC:
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

// sub_825C7608: lifted from generated/ by lift.py.
lifted::Result f825C7608(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7608 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
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
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r9));
  // stw r8,12(r31)
  guest::StoreU32(uint32_t(r31) + 12, uint32_t(r8));
  // stw r7,24(r31)
  guest::StoreU32(uint32_t(r31) + 24, uint32_t(r7));
  // bl 0x825c6148
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCueBase::Dtor(r1, r13, r3, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c7684
  if (cr6.eq) goto loc_825C7684;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32774
  r4 = r4 | 32774;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C7680, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C7684:
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

// sub_825C7750: lifted from generated/ by lift.py.
lifted::Result f825C7750(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7750 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x825c7960
  { const lifted::Result o = f825C7960(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C7860 game::XACTSoundBankBase::Dtor: lifted from generated/ by lift.py.
lifted::Result f825C7860(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7860 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lis r8,-32249
  r8 = uint64_t(int64_t(-2113470464));
  // addi r30,r31,12
  r30 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r11,r11,23088
  r11 = uint64_t(int64_t(int64_t(r11) + 23088));
  // addi r10,r10,23080
  r10 = uint64_t(int64_t(int64_t(r10) + 23080));
  // addi r9,r9,23076
  r9 = uint64_t(int64_t(int64_t(r9) + 23076));
  // addi r8,r8,23172
  r8 = uint64_t(int64_t(int64_t(r8) + 23172));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r9));
  // stw r8,0(r30)
  guest::StoreU32(uint32_t(r30) + 0, uint32_t(r8));
  // lwz r11,4(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 4);
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // beq cr6,0x825c78e4
  if (cr6.eq) goto loc_825C78E4;
  // lwz r11,8(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 8);
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // beq cr6,0x825c78e4
  if (cr6.eq) goto loc_825C78E4;
  // lwz r10,4(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 4);
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // stw r11,8(r10)
  guest::StoreU32(uint32_t(r10) + 8, uint32_t(r11));
  // lwz r11,8(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 8);
  // lwz r10,4(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 4);
  // stw r10,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r30,4(r30)
  guest::StoreU32(uint32_t(r30) + 4, uint32_t(r30));
  // stw r30,8(r30)
  guest::StoreU32(uint32_t(r30) + 8, uint32_t(r30));
loc_825C78E4:
  // mr r3,r31
  r3 = r31;
  // bl 0x825c6a20
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSoundBank::ReleaseWaveBanks(LIFTED_ARGS(0x825C78EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c6988
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSoundBank::Notify(r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // lwz r11,204(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c792c
  if (cr6.eq) goto loc_825C792C;
loc_825C7908:
  // lwz r11,204(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 204);
  // lwz r9,0(r3)
  r9 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 4);
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r11,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r11));
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r10,204(r3)
  guest::StoreU32(uint32_t(r3) + 204, uint32_t(r10));
  // bne cr6,0x825c7908
  if (!cr6.eq) goto loc_825C7908;
loc_825C792C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,212(r3)
  guest::StoreU32(uint32_t(r3) + 212, uint32_t(r11));
  // stw r11,208(r3)
  guest::StoreU32(uint32_t(r3) + 208, uint32_t(r11));
  // bl 0x825c1f30
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTNodePool::FreeChunks(LIFTED_ARGS(0x825C793C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // stw r11,0(r30)
  guest::StoreU32(uint32_t(r30) + 0, uint32_t(r11));
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

// sub_825C7960: lifted from generated/ by lift.py.
lifted::Result f825C7960(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7960 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x825c7860
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSoundBankBase::Dtor(r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c79a0
  if (cr6.eq) goto loc_825C79A0;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32861
  r4 = r4 | 32861;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C799C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C79A0:
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

// sub_825C7A30 game::XACTSoundBank::DeletingDestructorThunk: lifted from generated/ by lift.py.
lifted::Result f825C7A30(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7A30 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x825c82b8
  { const lifted::Result o = game::XACTSoundBank::Dtor_825C82B8(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C7A38: lifted from generated/ by lift.py.
lifted::Result f825C7A38(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C7A38 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r5
  r29 = r5;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7A5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // bl 0x825c65b8
  // (left out: dead in C++)
  r3 = f825C65B8(r4);
  // mr r28,r3
  r28 = r3;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c65f8
  // (left out: dead in C++)
  r3 = f825C65F8(r4);
  // clrlwi r11,r29,24
  r11 = uint32_t(r29) & 0xFF;
  // clrlwi r9,r3,16
  r9 = uint32_t(r3) & 0xFFFF;
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // clrlwi r10,r28,16
  r10 = uint32_t(r28) & 0xFFFF;
  // rlwinm r8,r11,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r8
  r11 = r11 + r8;
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // add r3,r11,r30
  r3 = r11 + r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C7BD0: lifted from generated/ by lift.py.
lifted::Result f825C7BD0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C7BD0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7BF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7C10, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,54(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 54);
  // add r3,r11,r30
  r3 = r11 + r30;
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

// sub_825C7C30: lifted from generated/ by lift.py.
lifted::Result f825C7C30(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C7C30 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7C54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r29,r3
  r29 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7C6C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // lwz r10,58(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 58);
  // rlwinm r9,r11,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r9
  r11 = r11 + r9;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r10
  r11 = r11 + r10;
  // add r3,r11,r29
  r3 = r11 + r29;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C7CD0: lifted from generated/ by lift.py.
lifted::Result f825C7CD0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C7CD0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // lwz r11,76(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 76);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7CF4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = lifted::VtableWord(uint32_t(r31));
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r10,48(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 48);
  // lwz r4,0(r11)
  r4 = guest::LoadU32(uint32_t(r11) + 0);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7D10, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825C7D28: lifted from generated/ by lift.py.
uint64_t f825C7D28(uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C7D28 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // subf r11,r4,r11
  r11 = r11 - r4;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // blr
  return r3;
}

// sub_825C7D40: lifted from generated/ by lift.py.
lifted::Result f825C7D40(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C7D40 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // lwz r11,68(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7D64, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r29,r3
  r29 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7D7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r10,15(r3)
  r10 = guest::LoadU16(uint32_t(r3) + 15);
  // lis r11,-21846
  r11 = uint64_t(int64_t(-1431699456));
  // rotlwi r10,r10,1
  r10 = __builtin_rotateleft32(uint32_t(r10), 1);
  // ori r11,r11,43691
  r11 = r11 | 43691;
  // subf r10,r10,r30
  r10 = r30 - r10;
  // subf r10,r29,r10
  r10 = r10 - r29;
  // mulhwu r11,r10,r11
  r11 = (uint64_t(uint32_t(r10)) * uint64_t(uint32_t(r11))) >> 32;
  // rlwinm r11,r11,30,2,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // clrlwi r3,r11,16
  r3 = uint32_t(r11) & 0xFFFF;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C7DA8 game::XACTSoundBank::Dtor_825C7DA8: lifted from generated/ by lift.py.
lifted::Result f825C7DA8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7DA8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lis r8,-32249
  r8 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,23184
  r11 = uint64_t(int64_t(int64_t(r11) + 23184));
  // addi r10,r10,23080
  r10 = uint64_t(int64_t(int64_t(r10) + 23080));
  // addi r9,r9,23076
  r9 = uint64_t(int64_t(int64_t(r9) + 23076));
  // addi r8,r8,23176
  r8 = uint64_t(int64_t(int64_t(r8) + 23176));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r9));
  // stw r8,12(r31)
  guest::StoreU32(uint32_t(r31) + 12, uint32_t(r8));
  // bl 0x825c6a20
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSoundBank::ReleaseWaveBanks(LIFTED_ARGS(0x825C7DF4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,252(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 252);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c7e20
  if (cr6.eq) goto loc_825C7E20;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r30,256(r31)
  r30 = guest::LoadU32(uint32_t(r31) + 256);
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7E18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C7E20, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C7E20:
  // mr r3,r31
  r3 = r31;
  // bl 0x825c7860
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSoundBankBase::Dtor(r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_825C82B8 game::XACTSoundBank::Dtor_825C82B8: lifted from generated/ by lift.py.
lifted::Result f825C82B8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C82B8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x825c7da8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSoundBank::Dtor_825C7DA8(r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c82f8
  if (cr6.eq) goto loc_825C82F8;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32793
  r4 = r4 | 32793;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C82F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C82F8:
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

// sub_825C8380 game::XACTWaveBank::DeletingDestructorThunk: lifted from generated/ by lift.py.
lifted::Result f825C8380(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8380 (lifted)");
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x825c85f8
  { const lifted::Result o = game::XACTWaveBank::Dtor(r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C83B8: lifted from generated/ by lift.py.
uint64_t f825C83B8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C83B8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,284(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 284);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // blr
  return r3;
}

// sub_825C83C8: lifted from generated/ by lift.py.
uint64_t f825C83C8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C83C8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,284(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 284);
  // addi r3,r11,88
  r3 = uint64_t(int64_t(int64_t(r11) + 88));
  // blr
  return r3;
}

// sub_825C83D8 game::XACTWaveBank::Load: lifted from generated/ by lift.py.
lifted::Result f825C83D8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825C83D8 (lifted)");
  uint64_t r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stw r6,260(r3)
  guest::StoreU32(uint32_t(r3) + 260, uint32_t(r6));
  // stw r7,264(r3)
  guest::StoreU32(uint32_t(r3) + 264, uint32_t(r7));
  // stw r5,268(r3)
  guest::StoreU32(uint32_t(r3) + 268, uint32_t(r5));
  // stw r4,280(r3)
  guest::StoreU32(uint32_t(r3) + 280, uint32_t(r4));
  // lwz r11,8(r4)
  r11 = guest::LoadU32(uint32_t(r4) + 8);
  // add r11,r11,r4
  r11 = r11 + r4;
  // stw r11,284(r3)
  guest::StoreU32(uint32_t(r3) + 284, uint32_t(r11));
  // lwz r11,40(r4)
  r11 = guest::LoadU32(uint32_t(r4) + 40);
  // add r11,r11,r4
  r11 = r11 + r4;
  // stw r11,276(r3)
  guest::StoreU32(uint32_t(r3) + 276, uint32_t(r11));
  // lwz r11,24(r4)
  r11 = guest::LoadU32(uint32_t(r4) + 24);
  // add r11,r11,r4
  r11 = r11 + r4;
  // stw r11,288(r3)
  guest::StoreU32(uint32_t(r3) + 288, uint32_t(r11));
  // lwz r11,16(r4)
  r11 = guest::LoadU32(uint32_t(r4) + 16);
  // add r11,r11,r4
  r11 = r11 + r4;
  // stw r11,272(r3)
  guest::StoreU32(uint32_t(r3) + 272, uint32_t(r11));
  // bl 0x825cc610
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWaveBank::SetPrepared(r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825C85C8: lifted from generated/ by lift.py.
uint64_t f825C85C8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C85C8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,280(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 280);
  // lwz r3,4(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 4);
  // blr
  return r3;
}

// sub_825C85D8: lifted from generated/ by lift.py.
uint64_t f825C85D8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C85D8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,280(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 280);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // blr
  return r3;
}

// sub_825C85F8 game::XACTWaveBank::Dtor: lifted from generated/ by lift.py.
lifted::Result f825C85F8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825C85F8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,23272
  r11 = uint64_t(int64_t(int64_t(r11) + 23272));
  // addi r10,r10,24604
  r10 = uint64_t(int64_t(int64_t(r10) + 24604));
  // lwz r8,260(r31)
  r8 = guest::LoadU32(uint32_t(r31) + 260);
  // addi r9,r9,23268
  r9 = uint64_t(int64_t(int64_t(r9) + 23268));
  // mr r30,r4
  r30 = r4;
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r9));
  // beq cr6,0x825c8650
  if (cr6.eq) goto loc_825C8650;
  // lwz r4,264(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 264);
  // lwz r3,280(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 280);
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C8650, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C8650:
  // mr r3,r31
  r3 = r31;
  // bl 0x825cec38
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWaveBankBase::Dtor(r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c8678
  if (cr6.eq) goto loc_825C8678;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32796
  r4 = r4 | 32796;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C8674, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C8678:
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

// sub_825C8698: lifted from generated/ by lift.py.
uint64_t f825C8698(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C8698 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x825cf9b0
  // (left out: dead in C++)
  r3 = f825CF9B0(r1, r13, r3, r4);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r3,r31
  r3 = r31;
  // addi r10,r11,23344
  r10 = uint64_t(int64_t(int64_t(r11) + 23344));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r9,r11,24604
  r9 = uint64_t(int64_t(int64_t(r11) + 24604));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r10,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r10));
  // li r10,4096
  r10 = uint64_t(int64_t(4096));
  // addi r8,r11,23336
  r8 = uint64_t(int64_t(int64_t(r11) + 23336));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r9,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r9));
  // stw r10,336(r31)
  guest::StoreU32(uint32_t(r31) + 336, uint32_t(r10));
  // stw r8,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r8));
  // stw r11,320(r31)
  guest::StoreU32(uint32_t(r31) + 320, uint32_t(r11));
  // stw r11,324(r31)
  guest::StoreU32(uint32_t(r31) + 324, uint32_t(r11));
  // stw r11,328(r31)
  guest::StoreU32(uint32_t(r31) + 328, uint32_t(r11));
  // stw r11,332(r31)
  guest::StoreU32(uint32_t(r31) + 332, uint32_t(r11));
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

// sub_825C8708: lifted from generated/ by lift.py.
lifted::Result f825C8708(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8708 (lifted)");
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x825c8c40
  { const lifted::Result o = f825C8C40(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C8710 game::XACTWaveBank::Notify: lifted from generated/ by lift.py.
lifted::Result f825C8710(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8710 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint8_t ca = 0;
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
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,81
  r3 = uint64_t(int64_t(int64_t(r1) + 81));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(LIFTED_ARGS(0x825C873C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r1)
  r11 = guest::LoadU32(uint32_t(r1) + 104);
  // lbz r10,108(r1)
  r10 = guest::LoadU8(uint32_t(r1) + 108);
  // rldicr r11,r11,8,63
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFFFF;
  // stb r30,80(r1)
  guest::StoreU8(uint32_t(r1) + 80, uint8_t(r30));
  // stb r31,92(r1)
  guest::StoreU8(uint32_t(r1) + 92, uint8_t(r31));
  // or r11,r10,r11
  r11 = r10 | r11;
  // lwz r3,20(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 20);
  // srawi r10,r31,24
  ca = (int32_t(r31) < 0) & ((uint32_t(r31) & 0xFFFFFF) != 0);
  r10 = uint64_t(int64_t(int32_t(r31) >> 24));
  // ld r4,80(r1)
  r4 = guest::LoadU64(uint32_t(r1) + 80);
  // rldicr r7,r11,24,39
  r7 = __builtin_rotateleft64(r11, 24) & 0xFFFFFFFFFF000000;
  // ld r6,96(r1)
  r6 = guest::LoadU64(uint32_t(r1) + 96);
  // stb r10,89(r1)
  guest::StoreU8(uint32_t(r1) + 89, uint8_t(r10));
  // rldicl r10,r31,56,8
  r10 = __builtin_rotateleft64(r31, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,90(r1)
  guest::StoreU16(uint32_t(r1) + 90, uint16_t(r10));
  // ld r5,88(r1)
  r5 = guest::LoadU64(uint32_t(r1) + 88);
  // bl 0x825c1388
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTEngine::SendNotification(LIFTED_ARGS(0x825C877C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825C8798: lifted from generated/ by lift.py.
uint64_t f825C8798(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C8798 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,324(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 324);
  // addi r3,r11,8
  r3 = uint64_t(int64_t(int64_t(r11) + 8));
  // blr
  return r3;
}

// sub_825C87A8: lifted from generated/ by lift.py.
uint64_t f825C87A8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C87A8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,320(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 320);
  // lwz r3,4(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 4);
  // blr
  return r3;
}

// sub_825C87B8: lifted from generated/ by lift.py.
uint64_t f825C87B8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C87B8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,320(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 320);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // blr
  return r3;
}

// sub_825C87C8: lifted from generated/ by lift.py.
uint64_t f825C87C8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C87C8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,324(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 324);
  // lwz r3,4(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 4);
  // blr
  return r3;
}

// sub_825C87D8: lifted from generated/ by lift.py.
uint64_t f825C87D8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C87D8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,324(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 324);
  // addi r3,r11,88
  r3 = uint64_t(int64_t(int64_t(r11) + 88));
  // blr
  return r3;
}

// sub_825C87E8: lifted from generated/ by lift.py.
uint64_t f825C87E8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C87E8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,324(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 324);
  // lwz r3,80(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 80);
  // blr
  return r3;
}

// sub_825C87F8: lifted from generated/ by lift.py.
uint64_t f825C87F8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C87F8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,324(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 324);
  // lwz r3,0(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 0);
  // blr
  return r3;
}

// sub_825C8808: lifted from generated/ by lift.py.
lifted::Result f825C8808(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8808 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r11,r4
  r11 = r4;
  // mr r31,r3
  r31 = r3;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,1
  r4 = r4 | 1;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 0);
  // stw r10,260(r31)
  guest::StoreU32(uint32_t(r31) + 260, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 4);
  // stw r10,264(r31)
  guest::StoreU32(uint32_t(r31) + 264, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 8);
  // stw r10,268(r31)
  guest::StoreU32(uint32_t(r31) + 268, uint32_t(r10));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 12);
  // stw r11,272(r31)
  guest::StoreU32(uint32_t(r31) + 272, uint32_t(r11));
  // lwz r3,336(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 336);
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(LIFTED_ARGS(0x825C8850, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825c8884
  if (cr6.eq) goto loc_825C8884;
  // addi r6,r31,276
  r6 = uint64_t(int64_t(int64_t(r31) + 276));
  // lwz r5,336(r31)
  r5 = guest::LoadU32(uint32_t(r31) + 336);
  // mr r3,r31
  r3 = r31;
  // stw r4,320(r31)
  guest::StoreU32(uint32_t(r31) + 320, uint32_t(r4));
  // bl 0x825cf498
  // (left out: dead in C++)
  { const lifted::Result o = f825CF498(r1, r13, r3, r4, r5, r6, r8, r9); r3 = o.r3; f1 = o.f1; }
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
loc_825C8884:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
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

// sub_825C88A0: lifted from generated/ by lift.py.
lifted::Result f825C88A0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C88A0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // lwz r11,36(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C88C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,17486
  r11 = uint64_t(int64_t(1145962496));
  // lis r10,-30009
  r10 = uint64_t(int64_t(-1966669824));
  // ori r11,r11,16983
  r11 = r11 | 16983;
  // ori r30,r10,7
  r30 = r10 | 7;
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // beq cr6,0x825c88e8
  if (cr6.eq) goto loc_825C88E8;
  // mr r3,r30
  r3 = r30;
  // b 0x825c89b8
  goto loc_825C89B8;
loc_825C88E8:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C88FC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,39
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 39, so);
  // beq cr6,0x825c890c
  if (cr6.eq) goto loc_825C890C;
  // mr r3,r30
  r3 = r30;
  // b 0x825c89b8
  goto loc_825C89B8;
loc_825C890C:
  // lwz r11,320(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 320);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 8);
  // cmplwi cr6,r10,1952
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1952, so);
  // blt cr6,0x825c8924
  if (cr6.lt) goto loc_825C8924;
  // mr r3,r30
  r3 = r30;
  // b 0x825c89b8
  goto loc_825C89B8;
loc_825C8924:
  // lwz r10,8(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 8);
  // mr r3,r31
  r3 = r31;
  // lwz r8,336(r31)
  r8 = guest::LoadU32(uint32_t(r31) + 336);
  // add r10,r11,r10
  r10 = r11 + r10;
  // stw r10,324(r31)
  guest::StoreU32(uint32_t(r31) + 324, uint32_t(r10));
  // lwz r10,28(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 28);
  // lwz r9,24(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 24);
  // add r4,r10,r9
  r4 = r10 + r9;
  // cmplw cr6,r4,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r8), so);
  // ble cr6,0x825c89ec
  if (!cr6.gt) goto loc_825C89EC;
  // bl 0x825cf4d8
  // (left out: dead in C++)
  { const lifted::Result o = f825CF4D8(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // lwz r3,320(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 320);
  // ori r4,r4,1
  r4 = r4 | 1;
  // stw r11,336(r31)
  guest::StoreU32(uint32_t(r31) + 336, uint32_t(r11));
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C8968, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // lwz r3,336(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 336);
  // ori r4,r4,1
  r4 = r4 | 1;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(LIFTED_ARGS(0x825C8978, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r4,r3
  r4 = r3;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // stw r11,324(r31)
  guest::StoreU32(uint32_t(r31) + 324, uint32_t(r11));
  // stw r11,328(r31)
  guest::StoreU32(uint32_t(r31) + 328, uint32_t(r11));
  // stw r11,332(r31)
  guest::StoreU32(uint32_t(r31) + 332, uint32_t(r11));
  // stw r4,320(r31)
  guest::StoreU32(uint32_t(r31) + 320, uint32_t(r4));
  // beq cr6,0x825c89d4
  if (cr6.eq) goto loc_825C89D4;
  // addi r6,r31,276
  r6 = uint64_t(int64_t(int64_t(r31) + 276));
  // stw r11,284(r31)
  guest::StoreU32(uint32_t(r31) + 284, uint32_t(r11));
  // mr r3,r31
  r3 = r31;
  // lwz r5,336(r31)
  r5 = guest::LoadU32(uint32_t(r31) + 336);
  // stw r11,288(r31)
  guest::StoreU32(uint32_t(r31) + 288, uint32_t(r11));
  // bl 0x825cf498
  // (left out: dead in C++)
  { const lifted::Result o = f825CF498(r1, r13, r3, r4, r5, r6, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge cr6,0x825c89d4
  if (!cr6.lt) goto loc_825C89D4;
loc_825C89B8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // cmpw cr6,r3,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r30), so);
  // stw r11,256(r31)
  guest::StoreU32(uint32_t(r31) + 256, uint32_t(r11));
  // bne cr6,0x825c89d4
  if (!cr6.eq) goto loc_825C89D4;
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c8710
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWaveBank::Notify(r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_825C89D4:
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
loc_825C89EC:
  // lwz r10,8(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 8);
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r10,324(r31)
  guest::StoreU32(uint32_t(r31) + 324, uint32_t(r10));
  // lwz r10,16(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 16);
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r10,328(r31)
  guest::StoreU32(uint32_t(r31) + 328, uint32_t(r10));
  // lwz r10,24(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 24);
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r11,332(r31)
  guest::StoreU32(uint32_t(r31) + 332, uint32_t(r11));
  // bl 0x825cc610
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTWaveBank::SetPrepared(r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x825c89d4
  goto loc_825C89D4;
}

// sub_825C8A18: lifted from generated/ by lift.py.
lifted::Result f825C8A18(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C8A18 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r31,r5
  r31 = r5;
  // lwz r11,0(r30)
  r11 = lifted::VtableWord(uint32_t(r30));
  // lwz r11,16(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C8A44, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,320(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 320);
  // addi r6,r31,52
  r6 = uint64_t(int64_t(int64_t(r31) + 52));
  // lwz r5,76(r31)
  r5 = guest::LoadU32(uint32_t(r31) + 76);
  // mr r11,r3
  r11 = r3;
  // lwz r4,84(r31)
  r4 = guest::LoadU32(uint32_t(r31) + 84);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // lwz r8,40(r10)
  r8 = guest::LoadU32(uint32_t(r10) + 40);
  // lwz r9,8(r6)
  r9 = guest::LoadU32(uint32_t(r6) + 8);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 8);
  // add r11,r9,r8
  r11 = r9 + r8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,60(r31)
  guest::StoreU32(uint32_t(r31) + 60, uint32_t(r11));
  // stw r7,64(r31)
  guest::StoreU32(uint32_t(r31) + 64, uint32_t(r7));
  // bl 0x825cf498
  // (left out: dead in C++)
  { const lifted::Result o = f825CF498(r1, r13, r3, r4, r5, r6, r8, r9); r3 = o.r3; f1 = o.f1; }
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

// sub_825C8A98: lifted from generated/ by lift.py.
lifted::Result f825C8A98(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C8A98 (lifted)");
  uint64_t r4 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r11,r3
  r11 = r3;
  // mr r4,r5
  r4 = r5;
  // mr r5,r6
  r5 = r6;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r3,260(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 260);
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // lwz r11,192(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 192);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C8AC0: lifted from generated/ by lift.py.
lifted::Result f825C8AC0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8AC0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,320(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 320);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r31,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r31));
  // beq cr6,0x825c8b0c
  if (cr6.eq) goto loc_825C8B0C;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r3,276
  r5 = uint64_t(int64_t(int64_t(r3) + 276));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C8B00, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beq cr6,0x825c8b10
  if (cr6.eq) goto loc_825C8B10;
loc_825C8B0C:
  // mr r3,r31
  r3 = r31;
loc_825C8B10:
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

// sub_825C8B28: lifted from generated/ by lift.py.
uint64_t f825C8B28(uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C8B28 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  // clrlwi r11,r4,16
  r11 = uint32_t(r4) & 0xFFFF;
  // lwz r10,328(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 328);
  // rlwinm r9,r11,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r9
  r11 = r11 + r9;
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // add r3,r11,r10
  r3 = r11 + r10;
  // blr
  return r3;
}

// sub_825C8B48: lifted from generated/ by lift.py.
uint64_t f825C8B48(uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C8B48 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // rlwinm r9,r4,2,14,29
  r9 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0x3FFFC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r10,332(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 332);
  // lwzx r9,r9,r10
  r9 = guest::LoadU32(uint32_t(r9) + uint32_t(r10));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // lwz r11,324(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 324);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r10
  r11 = r11 + r10;
  // add r3,r11,r9
  r3 = r11 + r9;
  // blr
  return r3;
}

// sub_825C8B80: lifted from generated/ by lift.py.
lifted::Result f825C8B80(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8B80 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,23344
  r11 = uint64_t(int64_t(int64_t(r11) + 23344));
  // addi r10,r10,24604
  r10 = uint64_t(int64_t(int64_t(r10) + 24604));
  // addi r9,r9,23336
  r9 = uint64_t(int64_t(int64_t(r9) + 23336));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r9));
loc_825C8BC0:
  // lwz r11,320(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 320);
  // stw r30,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r30));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c8bf8
  if (cr6.eq) goto loc_825C8BF8;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r31,276
  r5 = uint64_t(int64_t(int64_t(r31) + 276));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C8BF0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c8bc0
  if (cr6.eq) goto loc_825C8BC0;
loc_825C8BF8:
  // lwz r3,320(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 320);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c8c10
  if (cr6.eq) goto loc_825C8C10;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,1
  r4 = r4 | 1;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C8C10, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C8C10:
  // mr r3,r31
  r3 = r31;
  // bl 0x825cff90
  // (left out: dead in C++)
  { const lifted::Result o = f825CFF90(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825c8c10
  if (!cr6.eq) goto loc_825C8C10;
  // mr r3,r31
  r3 = r31;
  // bl 0x825cf308
  // (left out: dead in C++)
  { const lifted::Result o = f825CF308(r1, r13, r3, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_825C8C40: lifted from generated/ by lift.py.
lifted::Result f825C8C40(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8C40 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x825c8b80
  // (left out: dead in C++)
  { const lifted::Result o = f825C8B80(r1, r13, r3, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c8c80
  if (cr6.eq) goto loc_825C8C80;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32797
  r4 = r4 | 32797;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C8C7C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C8C80:
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

// sub_825C8C98: lifted from generated/ by lift.py.
lifted::Result f825C8C98(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8C98 (lifted)");
  uint64_t r0 = 0, r3 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r4
  r31 = r4;
  // lbz r10,0(r31)
  r10 = guest::LoadU8(uint32_t(r31) + 0);
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmplwi cr6,r11,14
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 14, so);
  // bgt cr6,0x825c8d54
  if (cr6.gt) goto loc_825C8D54;
  // lis r12,-32163
  r12 = uint64_t(int64_t(-2107834368));
  // addi r12,r12,-29484
  r12 = uint64_t(int64_t(int64_t(r12) + -29484));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = guest::LoadU32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825C8D10;
  case 1:
  goto loc_825C8D10;
  case 2:
  goto loc_825C8D10;
  case 3:
  goto loc_825C8D10;
  case 4:
  goto loc_825C8D10;
  case 5:
  goto loc_825C8E00;
  case 6:
  goto loc_825C8E00;
  case 7:
  goto loc_825C8D10;
  case 8:
  goto loc_825C8E00;
  case 9:
  goto loc_825C8E00;
  case 10:
  goto loc_825C8E00;
  case 11:
  goto loc_825C8DA0;
  case 12:
  goto loc_825C8DA0;
  case 13:
  goto loc_825C8E00;
  case 14:
  goto loc_825C8E00;
  default:
  std::abort();
  }
loc_825C8D10:
  // lwz r11,6(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 6);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c8d54
  if (!cr6.eq) goto loc_825C8D54;
  // lwz r3,2(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 2);
  // lwz r11,10(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 10);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c8d6c
  if (cr6.eq) goto loc_825C8D6C;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c8d54
  if (!cr6.eq) goto loc_825C8D54;
  // lhz r11,14(r31)
  r11 = guest::LoadU16(uint32_t(r31) + 14);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825c8e00
  if (cr6.eq) goto loc_825C8E00;
  // bl 0x825c6928
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSoundBank::GetCueCount(LIFTED_ARGS(0x825C8D44, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,14(r31)
  r11 = guest::LoadU16(uint32_t(r31) + 14);
  // clrlwi r10,r3,16
  r10 = uint32_t(r3) & 0xFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x825c8e00
  if (cr6.lt) goto loc_825C8E00;
loc_825C8D54:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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
loc_825C8D6C:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c8e00
  if (cr6.eq) goto loc_825C8E00;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x825c8e00
  if (!cr6.eq) goto loc_825C8E00;
  // lwz r3,10(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 10);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,68(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C8D90, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,30
  r11 = uint32_t(r3) & 0x3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c8e00
  if (!cr6.eq) goto loc_825C8E00;
  // b 0x825c8d54
  goto loc_825C8D54;
loc_825C8DA0:
  // lwz r11,6(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 6);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c8dc8
  if (cr6.eq) goto loc_825C8DC8;
  // lwz r11,2(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 2);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c8d54
  if (!cr6.eq) goto loc_825C8D54;
  // lwz r11,10(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 10);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c8e00
  if (cr6.eq) goto loc_825C8E00;
  // b 0x825c8d54
  goto loc_825C8D54;
loc_825C8DC8:
  // lwz r3,2(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 2);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c8e00
  if (cr6.eq) goto loc_825C8E00;
  // lwz r11,10(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 10);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c8d54
  if (!cr6.eq) goto loc_825C8D54;
  // lhz r11,14(r31)
  r11 = guest::LoadU16(uint32_t(r31) + 14);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825c8e00
  if (cr6.eq) goto loc_825C8E00;
  // bl 0x825c6928
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSoundBank::GetCueCount(LIFTED_ARGS(0x825C8DF0, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,14(r31)
  r11 = guest::LoadU16(uint32_t(r31) + 14);
  // clrlwi r10,r3,16
  r10 = uint32_t(r3) & 0xFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x825c8d54
  if (!cr6.lt) goto loc_825C8D54;
loc_825C8E00:
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_825C8E18: lifted from generated/ by lift.py.
lifted::Result f825C8E18(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8E18 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825c8e54
  if (cr6.eq) goto loc_825C8E54;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32799
  r4 = r4 | 32799;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825C8E50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C8E54:
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

// sub_825C8E68: lifted from generated/ by lift.py.
uint64_t f825C8E68(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C8E68 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,23432
  r11 = uint64_t(int64_t(int64_t(r11) + 23432));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // li r5,20
  r5 = uint64_t(int64_t(20));
  // stw r31,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r31));
  // stw r31,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r31));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(LIFTED_ARGS(0x825C8E9C, r31)); r3 = o.r3; f1 = o.f1; }
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
  return r3;
}

// sub_825C93A8: lifted from generated/ by lift.py.
lifted::Result f825C93A8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C93A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,20(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20);
  // lhz r11,0(r11)
  r11 = guest::LoadU16(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x825c93ec
  if (cr6.eq) goto loc_825C93EC;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x825c93ec
  if (cr6.eq) goto loc_825C93EC;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 0);
  // lwz r10,24(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 24);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // lfs f1,20(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r10) + 20);
  f1 = double(temp.f32);
  // b 0x825c9534
  goto loc_825C9534;
loc_825C93EC:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x825c9488
  if (cr6.eq) goto loc_825C9488;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x825c9540
  if (!cr6.eq) goto loc_825C9540;
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r3,36(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c9540
  if (cr6.eq) goto loc_825C9540;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,116(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 116);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C9420, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c9540
  if (cr6.eq) goto loc_825C9540;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(LIFTED_ARGS(0x825C942C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // mr r30,r3
  r30 = r3;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r3,36(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c9458
  if (cr6.eq) goto loc_825C9458;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,116(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 116);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C9454, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825c945c
  goto loc_825C945C;
loc_825C9458:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825C945C:
  // subf r11,r3,r30
  r11 = r30 - r3;
  // lwz r10,0(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // lwz r10,0(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 0);
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f1,f0
  f1 = double(float(f0));
  // mtctr r10
  ctr = r10;
  // b 0x825c953c
  goto loc_825C953C;
loc_825C9488:
  // lbz r11,40(r31)
  r11 = guest::LoadU8(uint32_t(r31) + 40);
  // rlwinm r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  // cmplwi cr6,r11,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 128, so);
  // bne cr6,0x825c9540
  if (!cr6.eq) goto loc_825C9540;
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r3,36(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c9524
  if (cr6.eq) goto loc_825C9524;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,112(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 112);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C94BC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c9524
  if (cr6.eq) goto loc_825C9524;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(LIFTED_ARGS(0x825C94C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // mr r30,r3
  r30 = r3;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r3,36(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c94f4
  if (cr6.eq) goto loc_825C94F4;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,112(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 112);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C94F0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825c94f8
  goto loc_825C94F8;
loc_825C94F4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825C94F8:
  // subf r11,r3,r30
  r11 = r30 - r3;
  // lwz r10,0(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // lwz r10,0(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 0);
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f1,f0
  f1 = double(float(f0));
  // mtctr r10
  ctr = r10;
  // b 0x825c953c
  goto loc_825C953C;
loc_825C9524:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,0(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 0);
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 0);
loc_825C9534:
  // mr r3,r31
  r3 = r31;
  // mtctr r11
  ctr = r11;
loc_825C953C:
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C9540, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C9540:
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

// sub_825C9558: lifted from generated/ by lift.py.
lifted::Result f825C9558(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C9558 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,20(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20);
  // lhz r11,0(r11)
  r11 = guest::LoadU16(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x825c9584
  if (cr6.eq) goto loc_825C9584;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x825c95e4
  if (!cr6.eq) goto loc_825C95E4;
loc_825C9584:
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r3,36(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c95b0
  if (cr6.eq) goto loc_825C95B0;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,112(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 112);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C95A8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825c95dc
  if (!cr6.eq) goto loc_825C95DC;
loc_825C95B0:
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r3,36(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c95e4
  if (cr6.eq) goto loc_825C95E4;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,116(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 116);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C95D4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c95e4
  if (cr6.eq) goto loc_825C95E4;
loc_825C95DC:
  // mr r3,r31
  r3 = r31;
  // bl 0x825c93a8
  // (left out: dead in C++)
  { const lifted::Result o = f825C93A8(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825C95E4:
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

// sub_825C9AF8 game::XACTSpeakerMask: lifted from generated/ by lift.py.
uint64_t f825C9AF8(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C9AF8 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // clrlwi r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x825c9b10
  if (!cr6.eq) goto loc_825C9B10;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // blr
  return r3;
loc_825C9B10:
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x825c9b20
  if (!cr6.eq) goto loc_825C9B20;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // blr
  return r3;
loc_825C9B20:
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x825c9b30
  if (!cr6.eq) goto loc_825C9B30;
  // li r3,11
  r3 = uint64_t(int64_t(11));
  // blr
  return r3;
loc_825C9B30:
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x825c9b40
  if (!cr6.eq) goto loc_825C9B40;
  // li r3,51
  r3 = uint64_t(int64_t(51));
  // blr
  return r3;
loc_825C9B40:
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // bne cr6,0x825c9b50
  if (!cr6.eq) goto loc_825C9B50;
  // li r3,63
  r3 = uint64_t(int64_t(63));
  // blr
  return r3;
loc_825C9B50:
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // li r3,1599
  r3 = uint64_t(int64_t(1599));
  // blr
  return r3;
}

// sub_825C9C30 game::XACTVoice::ExitLoop: lifted from generated/ by lift.py.
lifted::Result f825C9C30(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C9C30 (lifted)");
  uint64_t r4 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,4(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 4);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x8245a270
  { const lifted::Result o = lifted::entry::e8245A270(LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C9D18 game::XACTVoice::SetOutputs: lifted from generated/ by lift.py.
lifted::Result f825C9D18(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825C9D18 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r4,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r4));
  // mr r31,r3
  r31 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stb r11,80(r1)
  guest::StoreU8(uint32_t(r1) + 80, uint8_t(r11));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 8);
  // stw r10,96(r1)
  guest::StoreU32(uint32_t(r1) + 96, uint32_t(r10));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c9d5c
  if (cr6.eq) goto loc_825C9D5C;
  // stw r11,104(r1)
  guest::StoreU32(uint32_t(r1) + 104, uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r10,108(r1)
  guest::StoreU32(uint32_t(r1) + 108, uint32_t(r10));
  // stb r11,80(r1)
  guest::StoreU8(uint32_t(r1) + 80, uint8_t(r11));
loc_825C9D5C:
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 4);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x82459ef0
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioVoice::SetOutputVoices_82459EF0(LIFTED_ARGS(0x825C9D70, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825c9da0
  if (cr6.lt) goto loc_825C9DA0;
  // lwz r11,8(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c9da0
  if (cr6.eq) goto loc_825C9DA0;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r31,16
  r4 = uint64_t(int64_t(int64_t(r31) + 16));
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C9DA0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C9DA0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825C9E90 game::XACTVoice::SetMainMatrix: lifted from generated/ by lift.py.
lifted::Result f825C9E90(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825C9E90 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r3,4(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 4);
  // mr r11,r4
  r11 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stb r10,80(r1)
  guest::StoreU8(uint32_t(r1) + 80, uint8_t(r10));
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,92(r1)
  guest::StoreU32(uint32_t(r1) + 92, uint32_t(r11));
  // stw r10,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stb r10,88(r1)
  guest::StoreU8(uint32_t(r1) + 88, uint8_t(r10));
  // bl 0x82459f48
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioVoice::SetOutputMatrix(LIFTED_ARGS(0x825C9EC8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825C9ED8 game::XACTVoice::SetOutputVoices: lifted from generated/ by lift.py.
lifted::Result f825C9ED8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C9ED8 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,4(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 4);
  // b 0x82459ef0
  { const lifted::Result o = game::XAudioVoice::SetOutputVoices_82459EF0(LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C9EE0 game::XACTVoice::SetOutputMatrices: lifted from generated/ by lift.py.
lifted::Result f825C9EE0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C9EE0 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,4(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 4);
  // b 0x82459f48
  { const lifted::Result o = game::XAudioVoice::SetOutputMatrix(LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CA3B8 game::XACTVoice::SetReverbSend: lifted from generated/ by lift.py.
lifted::Result f825CA3B8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825CA3B8 (lifted)");
  uint64_t r8 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lbz r8,12(r3)
  r8 = guest::LoadU8(uint32_t(r3) + 12);
  // lfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r3) + 16, temp.u32);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x825ca404
  if (cr6.eq) goto loc_825CA404;
  // lfs f0,0(r4)
  temp.u32 = guest::LoadU32(uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // clrlwi r9,r8,24
  r9 = uint32_t(r8) & 0xFF;
  // addi r11,r1,116
  r11 = uint64_t(int64_t(int64_t(r1) + 116));
loc_825CA3E8:
  // mr r7,r10
  r7 = r10;
  // stfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r11) + 0, temp.u32);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // stb r7,-4(r11)
  guest::StoreU8(uint32_t(r11) + -4, uint8_t(r7));
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // blt cr6,0x825ca3e8
  if (cr6.lt) goto loc_825CA3E8;
loc_825CA404:
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r3,4(r3)
  r3 = guest::LoadU32(uint32_t(r3) + 4);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // stb r8,80(r1)
  guest::StoreU8(uint32_t(r1) + 80, uint8_t(r8));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r10,92(r1)
  guest::StoreU32(uint32_t(r1) + 92, uint32_t(r10));
  // stb r11,88(r1)
  guest::StoreU8(uint32_t(r1) + 88, uint8_t(r11));
  // stb r11,96(r1)
  guest::StoreU8(uint32_t(r1) + 96, uint8_t(r11));
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,100(r1)
  guest::StoreU32(uint32_t(r1) + 100, uint32_t(r11));
  // bl 0x82459f48
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioVoice::SetOutputMatrix(LIFTED_ARGS(0x825CA438, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825CA4B8 game::XACTReverb::Create: lifted from generated/ by lift.py.
lifted::Result f825CA4B8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CA4B8 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r4
  r28 = r4;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // mr r30,r3
  r30 = r3;
  // ori r4,r4,32811
  r4 = r4 | 32811;
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(LIFTED_ARGS(0x825CA4DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825ca520
  if (cr6.eq) goto loc_825CA520;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r4,r31,4
  r4 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r11,r11,23508
  r11 = uint64_t(int64_t(int64_t(r11) + 23508));
  // mr r3,r30
  r3 = r30;
  // stw r29,0(r4)
  guest::StoreU32(uint32_t(r4) + 0, uint32_t(r29));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8245a7c8
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioCreateSubmixVoice(LIFTED_ARGS(0x825CA508, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825ca538
  if (cr6.lt) goto loc_825CA538;
  // stw r31,0(r28)
  guest::StoreU32(uint32_t(r28) + 0, uint32_t(r31));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CA520:
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // stw r29,0(r28)
  guest::StoreU32(uint32_t(r28) + 0, uint32_t(r29));
  // ori r30,r30,14
  r30 = r30 | 14;
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CA538:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CA550, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r28)
  guest::StoreU32(uint32_t(r28) + 0, uint32_t(r29));
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CA560 game::XACTReverb::SetParameters: lifted from generated/ by lift.py.
lifted::Result f825CA560(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CA560 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r31,r4
  r31 = r4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r3
  r30 = r3;
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lfs f13,84(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r31) + 84);
  f13 = double(temp.f32);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // lfs f0,8748(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 8748);
  f0 = double(temp.f32);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lwz r3,4(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 4);
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 80, temp.u32);
  // bl 0x82459e80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82459E80, LIFTED_ARGS(0x825CA5A0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825ca678
  if (cr6.lt) goto loc_825CA678;
  // lfs f0,52(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r31) + 52);
  f0 = double(temp.f32);
  // lwz r11,4(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 4);
  // stfs f0,112(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 112, temp.u32);
  // lwz r3,4(r30)
  r3 = guest::LoadU32(uint32_t(r30) + 4);
  // lfs f0,56(r31)
  temp.u32 = guest::LoadU32(uint32_t(r31) + 56);
  f0 = double(temp.f32);
  // lwz r10,8(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 8);
  // stfs f0,116(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 116, temp.u32);
  // lwz r9,12(r31)
  r9 = guest::LoadU32(uint32_t(r31) + 12);
  // lfs f0,60(r31)
  temp.u32 = guest::LoadU32(uint32_t(r31) + 60);
  f0 = double(temp.f32);
  // lwz r8,16(r31)
  r8 = guest::LoadU32(uint32_t(r31) + 16);
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 120, temp.u32);
  // stb r11,100(r1)
  guest::StoreU8(uint32_t(r1) + 100, uint8_t(r11));
  // lfs f0,64(r31)
  temp.u32 = guest::LoadU32(uint32_t(r31) + 64);
  f0 = double(temp.f32);
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // stfs f0,124(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 124, temp.u32);
  // lwz r30,20(r31)
  r30 = guest::LoadU32(uint32_t(r31) + 20);
  // lfs f0,68(r31)
  temp.u32 = guest::LoadU32(uint32_t(r31) + 68);
  f0 = double(temp.f32);
  // lwz r29,24(r31)
  r29 = guest::LoadU32(uint32_t(r31) + 24);
  // stfs f0,128(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 128, temp.u32);
  // lwz r28,28(r31)
  r28 = guest::LoadU32(uint32_t(r31) + 28);
  // lfs f0,72(r31)
  temp.u32 = guest::LoadU32(uint32_t(r31) + 72);
  f0 = double(temp.f32);
  // lwz r27,32(r31)
  r27 = guest::LoadU32(uint32_t(r31) + 32);
  // stfs f0,132(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 132, temp.u32);
  // lwz r26,36(r31)
  r26 = guest::LoadU32(uint32_t(r31) + 36);
  // lfs f0,76(r31)
  temp.u32 = guest::LoadU32(uint32_t(r31) + 76);
  f0 = double(temp.f32);
  // lwz r25,40(r31)
  r25 = guest::LoadU32(uint32_t(r31) + 40);
  // stfs f0,136(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 136, temp.u32);
  // lwz r24,44(r31)
  r24 = guest::LoadU32(uint32_t(r31) + 44);
  // lwz r23,48(r31)
  r23 = guest::LoadU32(uint32_t(r31) + 48);
  // lfs f0,80(r31)
  temp.u32 = guest::LoadU32(uint32_t(r31) + 80);
  f0 = double(temp.f32);
  // stw r11,80(r1)
  guest::StoreU32(uint32_t(r1) + 80, uint32_t(r11));
  // li r11,48
  r11 = uint64_t(int64_t(48));
  // lwz r31,0(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 0);
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,3
  r6 = uint64_t(int64_t(3));
  // stfs f0,140(r1)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r1) + 140, temp.u32);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stb r10,102(r1)
  guest::StoreU8(uint32_t(r1) + 102, uint8_t(r10));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stb r9,103(r1)
  guest::StoreU8(uint32_t(r1) + 103, uint8_t(r9));
  // stb r8,104(r1)
  guest::StoreU8(uint32_t(r1) + 104, uint8_t(r8));
  // stw r31,96(r1)
  guest::StoreU32(uint32_t(r1) + 96, uint32_t(r31));
  // stb r30,105(r1)
  guest::StoreU8(uint32_t(r1) + 105, uint8_t(r30));
  // stb r29,106(r1)
  guest::StoreU8(uint32_t(r1) + 106, uint8_t(r29));
  // stb r28,107(r1)
  guest::StoreU8(uint32_t(r1) + 107, uint8_t(r28));
  // stb r27,108(r1)
  guest::StoreU8(uint32_t(r1) + 108, uint8_t(r27));
  // stb r26,109(r1)
  guest::StoreU8(uint32_t(r1) + 109, uint8_t(r26));
  // stb r25,110(r1)
  guest::StoreU8(uint32_t(r1) + 110, uint8_t(r25));
  // stb r24,111(r1)
  guest::StoreU8(uint32_t(r1) + 111, uint8_t(r24));
  // stb r23,101(r1)
  guest::StoreU8(uint32_t(r1) + 101, uint8_t(r23));
  // stw r11,84(r1)
  guest::StoreU32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x82459e80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82459E80, LIFTED_ARGS(0x825CA678, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CA678:
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CA680 game::XACTReverb::Dtor: lifted from generated/ by lift.py.
lifted::Result f825CA680(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CA680 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r30,r4
  r30 = r4;
  // addi r11,r11,23508
  r11 = uint64_t(int64_t(int64_t(r11) + 23508));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825ca6b8
  if (cr6.eq) goto loc_825CA6B8;
  // bl 0x82459e18
  // (left out: dead in C++)
  { const lifted::Result o = game::XAudioVoice::Release_82459E18(LIFTED_ARGS(0x825CA6B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CA6B8:
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825ca6d8
  if (cr6.eq) goto loc_825CA6D8;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32811
  r4 = r4 | 32811;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825CA6D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825CA6D8:
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

// sub_825CA6F0: lifted from generated/ by lift.py.
lifted::Result f825CA6F0(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825CA6F0 (lifted)");
  uint64_t r3 = 0;
  double f1 = 0.0;
  // mr r3,r4
  r3 = r4;
  // b 0x825d1830
  { const lifted::Result o = game::XACTEvent::Cancel(r1, r13, r3, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CA6F8 game::XACTSound::Pause: lifted from generated/ by lift.py.
lifted::Result f825CA6F8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CA6F8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r28,r4
  r28 = r4;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lwz r11,80(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 80);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bgt cr6,0x825ca7fc
  if (cr6.gt) goto loc_825CA7FC;
  // lbz r11,52(r30)
  r11 = guest::LoadU8(uint32_t(r30) + 52);
  // rlwinm r10,r11,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpw cr6,r28,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r10), so);
  // beq cr6,0x825ca7fc
  if (cr6.eq) goto loc_825CA7FC;
  // cntlzw r10,r28
  r10 = uint32_t(r28) == 0 ? 32 : __builtin_clz(uint32_t(r28));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // xori r10,r10,1
  r10 = r10 ^ 1;
  // rlwimi r11,r10,5,26,26
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0x20) | (r11 & 0xFFFFFFFFFFFFFFDF);
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // rlwinm r10,r11,0,26,26
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r10,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 32, so);
  // stb r11,52(r30)
  guest::StoreU8(uint32_t(r30) + 52, uint8_t(r11));
  // bne cr6,0x825ca75c
  if (!cr6.eq) goto loc_825CA75C;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(LIFTED_ARGS(0x825CA754, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,108(r30)
  guest::StoreU32(uint32_t(r30) + 108, uint32_t(r3));
  // b 0x825ca794
  goto loc_825CA794;
loc_825CA75C:
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(LIFTED_ARGS(0x825CA760, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,60(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 60);
  // lwz r10,108(r30)
  r10 = guest::LoadU32(uint32_t(r30) + 108);
  // addi r9,r30,56
  r9 = uint64_t(int64_t(int64_t(r30) + 56));
  // subf r7,r10,r3
  r7 = r3 - r10;
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // beq cr6,0x825ca794
  if (cr6.eq) goto loc_825CA794;
loc_825CA778:
  // addi r10,r11,20
  r10 = uint64_t(int64_t(int64_t(r11) + 20));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // lwz r8,0(r10)
  r8 = guest::LoadU32(uint32_t(r10) + 0);
  // add r8,r8,r7
  r8 = r8 + r7;
  // stw r8,0(r10)
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r8));
  // bne cr6,0x825ca778
  if (!cr6.eq) goto loc_825CA778;
loc_825CA794:
  // lwz r31,40(r30)
  r31 = guest::LoadU32(uint32_t(r30) + 40);
  // addi r29,r30,36
  r29 = uint64_t(int64_t(int64_t(r30) + 36));
loc_825CA79C:
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // beq cr6,0x825ca7c0
  if (cr6.eq) goto loc_825CA7C0;
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d05a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::Pause(LIFTED_ARGS(0x825CA7B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bge cr6,0x825ca79c
  if (!cr6.lt) goto loc_825CA79C;
loc_825CA7C0:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x825ca7fc
  if (!cr6.eq) goto loc_825CA7FC;
  // lwz r31,32(r30)
  r31 = guest::LoadU32(uint32_t(r30) + 32);
  // addi r30,r31,144
  r30 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(LIFTED_ARGS(0x825CA7D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r3,12(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 12);
  // stw r11,184(r31)
  guest::StoreU32(uint32_t(r31) + 184, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CA7F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825CA7FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CA7FC:
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CA808: lifted from generated/ by lift.py.
lifted::Result f825CA808(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CA808 (lifted)");
  uint64_t r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r31,40(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 40);
  // addi r29,r3,36
  r29 = uint64_t(int64_t(int64_t(r3) + 36));
  // mr r30,r4
  r30 = r4;
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // beq cr6,0x825ca848
  if (cr6.eq) goto loc_825CA848;
loc_825CA828:
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d03b0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::UsesWaveBank(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825ca854
  if (!cr6.eq) goto loc_825CA854;
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // bne cr6,0x825ca828
  if (!cr6.eq) goto loc_825CA828;
loc_825CA848:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825CA854:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CA860 game::XACTSound::UpdateVolume: lifted from generated/ by lift.py.
lifted::Result f825CA860(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CA860 (lifted)");
  uint64_t r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r31,40(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 40);
  // addi r30,r3,36
  r30 = uint64_t(int64_t(int64_t(r3) + 36));
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825ca898
  if (cr6.eq) goto loc_825CA898;
loc_825CA884:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d0428
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::UpdateVolume(LIFTED_ARGS(0x825CA88C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825ca884
  if (!cr6.eq) goto loc_825CA884;
loc_825CA898:
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

// sub_825CA900: lifted from generated/ by lift.py.
lifted::Result f825CA900(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CA900 (lifted)");
  uint64_t r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r31,40(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 40);
  // addi r30,r3,36
  r30 = uint64_t(int64_t(int64_t(r3) + 36));
  // mr r29,r4
  r29 = r4;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825ca938
  if (cr6.eq) goto loc_825CA938;
loc_825CA920:
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d0510
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::SetOutputVoices(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825CA92C); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825ca920
  if (!cr6.eq) goto loc_825CA920;
loc_825CA938:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CA940: lifted from generated/ by lift.py.
lifted::Result f825CA940(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CA940 (lifted)");
  uint64_t r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r31,40(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 40);
  // addi r30,r3,36
  r30 = uint64_t(int64_t(int64_t(r3) + 36));
  // mr r29,r4
  r29 = r4;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825ca978
  if (cr6.eq) goto loc_825CA978;
loc_825CA960:
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d0528
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::SetOutputMatrices(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825CA96C); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825ca960
  if (!cr6.eq) goto loc_825CA960;
loc_825CA978:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CA9C0: lifted from generated/ by lift.py.
lifted::Result f825CA9C0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CA9C0 (lifted)");
  uint64_t r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r31,40(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 40);
  // addi r30,r3,36
  r30 = uint64_t(int64_t(int64_t(r3) + 36));
  // mr r29,r4
  r29 = r4;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825ca9f8
  if (cr6.eq) goto loc_825CA9F8;
loc_825CA9E0:
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d0558
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::SetMainMatrix(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825CA9EC); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825ca9e0
  if (!cr6.eq) goto loc_825CA9E0;
loc_825CA9F8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CAA00: lifted from generated/ by lift.py.
lifted::Result f825CAA00(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CAA00 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r3
  r11 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r31,40(r11)
  r31 = guest::LoadU32(uint32_t(r11) + 40);
  // addi r27,r11,36
  r27 = uint64_t(int64_t(int64_t(r11) + 36));
loc_825CAA28:
  // cmplw cr6,r31,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r27), so);
  // beq cr6,0x825caa50
  if (cr6.eq) goto loc_825CAA50;
  // mr r6,r28
  r6 = r28;
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d0570
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::SetChannelMatrix(r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, 0x825CAA44); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge cr6,0x825caa28
  if (!cr6.lt) goto loc_825CAA28;
loc_825CAA50:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CAA58 game::XACTSound::AnyTrackBusy: lifted from generated/ by lift.py.
lifted::Result f825CAA58(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CAA58 (lifted)");
  uint64_t r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r31,40(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 40);
  // addi r30,r3,36
  r30 = uint64_t(int64_t(int64_t(r3) + 36));
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825caa98
  if (cr6.eq) goto loc_825CAA98;
loc_825CAA7C:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d0590
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::IsBusy(LIFTED_ARGS(0x825CAA84, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825caab4
  if (!cr6.eq) goto loc_825CAAB4;
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825caa7c
  if (!cr6.eq) goto loc_825CAA7C;
loc_825CAA98:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825CAA9C:
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
loc_825CAAB4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825caa9c
  goto loc_825CAA9C;
}

// sub_825CAAC0 game::XACTSound::CheckInstanceLimit: lifted from generated/ by lift.py.
lifted::Result f825CAAC0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CAAC0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,48(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 48);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,124(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 124);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAAE8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825caafc
  if (!cr6.eq) goto loc_825CAAFC;
  // mr r4,r31
  r4 = r31;
  // lwz r3,32(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 32);
  // bl 0x825c0458
  // (left out: dead in C++)
  { const lifted::Result o = f825C0458(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CAAFC:
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

// sub_825CAC58: lifted from generated/ by lift.py.
lifted::Result f825CAC58(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CAC58 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,40(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 40);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825caca8
  if (cr6.eq) goto loc_825CACA8;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r10,36(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825cac98
  if (cr6.eq) goto loc_825CAC98;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAC98, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CAC98:
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // addi r11,r11,-255
  r11 = uint64_t(int64_t(int64_t(r11) + -255));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
loc_825CACA8:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825CACB8: lifted from generated/ by lift.py.
lifted::Result f825CACB8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CACB8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,40(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 40);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cad08
  if (cr6.eq) goto loc_825CAD08;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r10,36(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825cacf8
  if (cr6.eq) goto loc_825CACF8;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CACF8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CACF8:
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // subfic r11,r11,0
  ca = uint32_t(r11) <= 0;
  r11 = static_cast<uint64_t>(0) - r11;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // clrlwi r3,r11,31
  r3 = uint32_t(r11) & 0x1;
loc_825CAD08:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825CAD18: lifted from generated/ by lift.py.
lifted::Result f825CAD18(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CAD18 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,40(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 40);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cada4
  if (cr6.eq) goto loc_825CADA4;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r10,36(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825cad68
  if (cr6.eq) goto loc_825CAD68;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAD68, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CAD68:
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cada4
  if (cr6.eq) goto loc_825CADA4;
  // lwz r11,40(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 40);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r11,36(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cada8
  if (cr6.eq) goto loc_825CADA8;
  // lwz r10,0(r11)
  r10 = lifted::VtableWord(uint32_t(r11));
  // mr r3,r11
  r3 = r11;
  // lwz r11,40(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CADA0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825cada8
  goto loc_825CADA8;
loc_825CADA4:
  // mr r3,r30
  r3 = r30;
loc_825CADA8:
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

// sub_825CADC0: lifted from generated/ by lift.py.
lifted::Result f825CADC0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825CADC0 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,40(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 40);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r3,36(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,36(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CADF0: lifted from generated/ by lift.py.
lifted::Result f825CADF0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825CADF0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r4,r3
  r4 = r3;
  // lwz r3,48(r4)
  r3 = guest::LoadU32(uint32_t(r4) + 48);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,92(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 92);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CAE08: lifted from generated/ by lift.py.
lifted::Result f825CAE08(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CAE08 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // lwz r11,16(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAE30, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // mr r30,r3
  r30 = r3;
  // lbz r11,0(r11)
  r11 = guest::LoadU8(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x825cae6c
  if (!cr6.eq) goto loc_825CAE6C;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAE5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,2(r3)
  r11 = guest::LoadU8(uint32_t(r3) + 2);
  // rotlwi r11,r11,2
  r11 = __builtin_rotateleft32(uint32_t(r11), 2);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x825cae70
  goto loc_825CAE70;
loc_825CAE6C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825CAE70:
  // add r11,r11,r30
  r11 = r11 + r30;
  // lbz r3,2(r11)
  r3 = guest::LoadU8(uint32_t(r11) + 2);
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

// sub_825CAE90 game::XACTSound::Dtor: lifted from generated/ by lift.py.
lifted::Result f825CAE90(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825CAE90 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,24556
  r11 = uint64_t(int64_t(int64_t(r11) + 24556));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r28,r31,4
  r28 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r10,r10,24544
  r10 = uint64_t(int64_t(int64_t(r10) + 24544));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 84);
  // stw r10,0(r28)
  guest::StoreU32(uint32_t(r28) + 0, uint32_t(r10));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825caee0
  if (cr6.eq) goto loc_825CAEE0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,28(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 28);
  // lwz r3,32(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 32);
  // stw r11,84(r31)
  guest::StoreU32(uint32_t(r31) + 84, uint32_t(r11));
  // lhz r4,1(r10)
  r4 = guest::LoadU16(uint32_t(r10) + 1);
  // bl 0x825c05b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C05B0(LIFTED_ARGS(0x825CAEE0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CAEE0:
  // lwz r3,40(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 40);
  // addi r29,r31,36
  r29 = uint64_t(int64_t(int64_t(r31) + 36));
  // cmplw cr6,r3,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r29), so);
  // beq cr6,0x825caf30
  if (cr6.eq) goto loc_825CAF30;
loc_825CAEF0:
  // lwz r11,8(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 8);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r30,4(r3)
  r30 = guest::LoadU32(uint32_t(r3) + 4);
  // stw r11,8(r30)
  guest::StoreU32(uint32_t(r30) + 8, uint32_t(r11));
  // lwz r11,8(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 8);
  // lwz r10,4(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 4);
  // stw r10,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r3,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r3));
  // stw r3,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r3));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAF24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // bne cr6,0x825caef0
  if (!cr6.eq) goto loc_825CAEF0;
loc_825CAF30:
  // lwz r11,96(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 96);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825caf6c
  if (cr6.eq) goto loc_825CAF6C;
  // addi r3,r11,-4
  r3 = uint64_t(int64_t(int64_t(r11) + -4));
  // lwz r10,0(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825caf68
  if (cr6.eq) goto loc_825CAF68;
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAF64, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825caf6c
  goto loc_825CAF6C;
loc_825CAF68:
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(LIFTED_ARGS(0x825CAF6C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CAF6C:
  // lbz r11,52(r31)
  r11 = guest::LoadU8(uint32_t(r31) + 52);
  // rlwinm r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // bne cr6,0x825caf94
  if (!cr6.eq) goto loc_825CAF94;
  // lwz r3,48(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 48);
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,88(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 88);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAF94, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CAF94:
  // lwz r11,48(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 48);
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAFB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r3,r28
  r3 = r28;
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // stw r11,68(r31)
  guest::StoreU32(uint32_t(r31) + 68, uint32_t(r11));
  // stw r11,56(r31)
  guest::StoreU32(uint32_t(r31) + 56, uint32_t(r11));
  // stw r11,0(r29)
  guest::StoreU32(uint32_t(r29) + 0, uint32_t(r11));
  // bl 0x825cc0d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTFader::Dtor(LIFTED_ARGS(0x825CAFCC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CAFD8 game::XACTSound::OnCueGone: lifted from generated/ by lift.py.
lifted::Result f825CAFD8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CAFD8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r3,r4
  r3 = r4;
  // mr r4,r31
  r4 = r31;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB004, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,52(r31)
  r11 = guest::LoadU8(uint32_t(r31) + 52);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // andi. r11,r11,239
  r11 = r11 & 239;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stb r11,52(r31)
  guest::StoreU8(uint32_t(r31) + 52, uint8_t(r11));
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB028, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825CB040 game::XACTSound::Update: lifted from generated/ by lift.py.
lifted::Result f825CB040(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CB040 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,80(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 80);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x825cb08c
  if (!cr6.eq) goto loc_825CB08C;
  // bl 0x825caa58
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::AnyTrackBusy(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825cb08c
  if (!cr6.eq) goto loc_825CB08C;
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(uint32_t(r31));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB088, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825cb0b0
  goto loc_825CB0B0;
loc_825CB08C:
  // addi r30,r31,36
  r30 = uint64_t(int64_t(int64_t(r31) + 36));
  // lwz r31,40(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 40);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825cb0b0
  if (cr6.eq) goto loc_825CB0B0;
loc_825CB09C:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d0e30
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::Update(LIFTED_ARGS(0x825CB0A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825cb09c
  if (!cr6.eq) goto loc_825CB09C;
loc_825CB0B0:
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

// sub_825CB188: lifted from generated/ by lift.py.
void f825CB188(uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825CB188 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r3,68
  r11 = uint64_t(int64_t(int64_t(r3) + 68));
  // stw r11,4(r4)
  guest::StoreU32(uint32_t(r4) + 4, uint32_t(r11));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 8);
  // stw r10,8(r4)
  guest::StoreU32(uint32_t(r4) + 8, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 8);
  // stw r4,4(r10)
  guest::StoreU32(uint32_t(r10) + 4, uint32_t(r4));
  // stw r4,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r4));
  // blr
  return;
}

// sub_825CB1A8 game::XACTSound::SetState: lifted from generated/ by lift.py.
lifted::Result f825CB1A8(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CB1A8 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // addi r11,r30,-3
  r11 = uint64_t(int64_t(int64_t(r30) + -3));
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bgt cr6,0x825cb2bc
  if (cr6.gt) goto loc_825CB2BC;
  // lis r12,-32163
  r12 = uint64_t(int64_t(-2107834368));
  // addi r12,r12,-19992
  r12 = uint64_t(int64_t(int64_t(r12) + -19992));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = guest::LoadU32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825CB1F8;
  case 1:
  goto loc_825CB238;
  case 2:
  goto loc_825CB298;
  case 3:
  goto loc_825CB2D8;
  default:
  std::abort();
  }
loc_825CB1F8:
  // lwz r11,80(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 80);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bge cr6,0x825cb2bc
  if (!cr6.lt) goto loc_825CB2BC;
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // lwz r3,48(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 48);
  // stw r11,80(r31)
  guest::StoreU32(uint32_t(r31) + 80, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,132(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 132);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB220, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825cb2c0
  if (!cr6.eq) goto loc_825CB2C0;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,48(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 48);
  // bl 0x825c4a98
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTCue::Notify(r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x825cb2c0
  goto loc_825CB2C0;
loc_825CB238:
  // lwz r11,80(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 80);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bge cr6,0x825cb2bc
  if (!cr6.lt) goto loc_825CB2BC;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // lwz r9,28(r31)
  r9 = guest::LoadU32(uint32_t(r31) + 28);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r3,32(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 32);
  // stw r11,80(r31)
  guest::StoreU32(uint32_t(r31) + 80, uint32_t(r11));
  // stw r10,84(r31)
  guest::StoreU32(uint32_t(r31) + 84, uint32_t(r10));
  // lhz r4,1(r9)
  r4 = guest::LoadU16(uint32_t(r9) + 1);
  // bl 0x825c0598
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C0598(LIFTED_ARGS(0x825CB264, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,48(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 48);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,132(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 132);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB278, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825cb2c0
  if (!cr6.eq) goto loc_825CB2C0;
  // lwz r3,48(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 48);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,120(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 120);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB294, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825cb2c0
  goto loc_825CB2C0;
loc_825CB298:
  // lwz r11,84(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 84);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cb2bc
  if (cr6.eq) goto loc_825CB2BC;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,28(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 28);
  // lwz r3,32(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 32);
  // stw r11,84(r31)
  guest::StoreU32(uint32_t(r31) + 84, uint32_t(r11));
  // lhz r4,1(r10)
  r4 = guest::LoadU16(uint32_t(r10) + 1);
  // bl 0x825c05b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C05B0(LIFTED_ARGS(0x825CB2BC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CB2BC:
  // stw r30,80(r31)
  guest::StoreU32(uint32_t(r31) + 80, uint32_t(r30));
loc_825CB2C0:
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
loc_825CB2D8:
  // lwz r11,80(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 80);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bge cr6,0x825cb2bc
  if (!cr6.lt) goto loc_825CB2BC;
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,80(r31)
  guest::StoreU32(uint32_t(r31) + 80, uint32_t(r11));
  // lwz r11,84(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 84);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cb310
  if (cr6.eq) goto loc_825CB310;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,28(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 28);
  // lwz r3,32(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 32);
  // stw r11,84(r31)
  guest::StoreU32(uint32_t(r31) + 84, uint32_t(r11));
  // lhz r4,1(r10)
  r4 = guest::LoadU16(uint32_t(r10) + 1);
  // bl 0x825c05b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C05B0(LIFTED_ARGS(0x825CB310, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CB310:
  // lwz r3,48(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 48);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,132(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 132);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB324, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825cb2c0
  if (!cr6.eq) goto loc_825CB2C0;
  // lwz r3,48(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 48);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,116(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 116);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB340, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825cb2c0
  goto loc_825CB2C0;
}

// sub_825CB348: lifted from generated/ by lift.py.
lifted::Result f825CB348(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CB348 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,0(r30)
  r11 = lifted::VtableWord(uint32_t(r30));
  // lwz r11,16(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB36C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 28);
  // addi r31,r3,2
  r31 = uint64_t(int64_t(int64_t(r3) + 2));
  // lbz r11,0(r11)
  r11 = guest::LoadU8(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x825cb398
  if (!cr6.eq) goto loc_825CB398;
  // mr r3,r30
  r3 = r30;
  // bl 0x825cae08
  // (left out: dead in C++)
  { const lifted::Result o = f825CAE08(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,2,22,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0x3FC;
  // add r11,r11,r31
  r11 = r11 + r31;
  // addi r31,r11,1
  r31 = uint64_t(int64_t(int64_t(r11) + 1));
loc_825CB398:
  // clrlwi r9,r29,24
  r9 = uint32_t(r29) & 0xFF;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x825cb3c8
  if (cr6.eq) goto loc_825CB3C8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825CB3A8:
  // lbz r10,0(r31)
  r10 = guest::LoadU8(uint32_t(r31) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // rotlwi r10,r10,2
  r10 = __builtin_rotateleft32(uint32_t(r10), 2);
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // add r10,r10,r31
  r10 = r10 + r31;
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // addi r31,r10,1
  r31 = uint64_t(int64_t(int64_t(r10) + 1));
  // blt cr6,0x825cb3a8
  if (cr6.lt) goto loc_825CB3A8;
loc_825CB3C8:
  // lbz r3,0(r31)
  r3 = guest::LoadU8(uint32_t(r31) + 0);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CB660: lifted from generated/ by lift.py.
uint64_t f825CB660(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CB660 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,40(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 40);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // lwz r11,36(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 36);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 16);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // clrlwi r11,r11,27
  r11 = uint32_t(r11) & 0x1F;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_825CB6A0: lifted from generated/ by lift.py.
uint64_t f825CB6A0(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825CB6A0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,60(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 60);
  // addi r9,r3,56
  r9 = uint64_t(int64_t(int64_t(r3) + 56));
  // lwz r8,40(r3)
  r8 = guest::LoadU32(uint32_t(r3) + 40);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // beqlr cr6
  if (cr6.eq) return r3;
loc_825CB6B8:
  // lwz r10,44(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 44);
  // lwz r10,16(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 16);
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // bne cr6,0x825cb6ec
  if (!cr6.eq) goto loc_825CB6EC;
  // lwz r7,16(r11)
  r7 = guest::LoadU32(uint32_t(r11) + 16);
  // lwz r10,28(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 28);
  // lwz r7,0(r7)
  r7 = guest::LoadU32(uint32_t(r7) + 0);
  // clrlwi r7,r7,27
  r7 = uint32_t(r7) & 0x1F;
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // beq cr6,0x825cb6fc
  if (cr6.eq) goto loc_825CB6FC;
  // cmpw cr6,r10,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r3), so);
  // ble cr6,0x825cb6ec
  if (!cr6.gt) goto loc_825CB6EC;
  // mr r3,r10
  r3 = r10;
loc_825CB6EC:
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x825cb6b8
  if (!cr6.eq) goto loc_825CB6B8;
  // blr
  return r3;
loc_825CB6FC:
  // mr r3,r10
  r3 = r10;
  // blr
  return r3;
}

// sub_825CB708: lifted from generated/ by lift.py.
uint64_t f825CB708(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_825CB708 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1168(r1)
  ea = -1168 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(LIFTED_ARGS(0x825CB728, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // lwz r26,40(r31)
  r26 = guest::LoadU32(uint32_t(r31) + 40);
  // li r5,1024
  r5 = uint64_t(int64_t(1024));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(LIFTED_ARGS(0x825CB740, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,60(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 60);
  // addi r6,r31,56
  r6 = uint64_t(int64_t(int64_t(r31) + 56));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmplw cr6,r11,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r6), so);
  // beq cr6,0x825cb7cc
  if (cr6.eq) goto loc_825CB7CC;
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
loc_825CB758:
  // lwz r10,44(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 44);
  // lwz r10,16(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 16);
  // cmplw cr6,r10,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r26), so);
  // bne cr6,0x825cb7c0
  if (!cr6.eq) goto loc_825CB7C0;
  // lwz r9,16(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 16);
  // lwz r10,0(r9)
  r10 = guest::LoadU32(uint32_t(r9) + 0);
  // clrlwi r10,r10,27
  r10 = uint32_t(r10) & 0x1F;
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x825cb784
  if (cr6.eq) goto loc_825CB784;
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // bne cr6,0x825cb7c0
  if (!cr6.eq) goto loc_825CB7C0;
loc_825CB784:
  // lwz r10,28(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 28);
  // cmpw cr6,r10,r27
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r27), so);
  // ble cr6,0x825cb7c0
  if (!cr6.gt) goto loc_825CB7C0;
  // lwz r10,7(r9)
  r10 = guest::LoadU32(uint32_t(r9) + 7);
  // cmplw cr6,r10,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r29), so);
  // blt cr6,0x825cb7c0
  if (cr6.lt) goto loc_825CB7C0;
  // cmplw cr6,r10,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r28), so);
  // bgt cr6,0x825cb7c0
  if (cr6.gt) goto loc_825CB7C0;
  // stw r11,0(r7)
  guest::StoreU32(uint32_t(r7) + 0, uint32_t(r11));
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // bne cr6,0x825cb7cc
  if (!cr6.eq) goto loc_825CB7CC;
  // cmpwi cr6,r8,256
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 256, so);
  // beq cr6,0x825cb7cc
  if (cr6.eq) goto loc_825CB7CC;
loc_825CB7C0:
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmplw cr6,r11,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r6), so);
  // bne cr6,0x825cb758
  if (!cr6.eq) goto loc_825CB758;
loc_825CB7CC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // ble cr6,0x825cb810
  if (!cr6.gt) goto loc_825CB810;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x825cb7f4
  if (cr6.eq) goto loc_825CB7F4;
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
  // addi r1,r1,1168
  r1 = uint64_t(int64_t(int64_t(r1) + 1168));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
loc_825CB7F4:
  // addi r5,r8,-1
  r5 = uint64_t(int64_t(int64_t(r8) + -1));
  // lwz r3,32(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 32);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825bf6b8
  // (left out: dead in C++)
  r3 = f825BF6B8(r1, r13, r3, r4, r5);
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(uint32_t(r11) + uint32_t(r10));
loc_825CB810:
  // addi r1,r1,1168
  r1 = uint64_t(int64_t(int64_t(r1) + 1168));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_825CB958 game::XACTSound::Ctor: lifted from generated/ by lift.py.
lifted::Result f825CB958(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_825CB958 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32252
  r11 = uint64_t(int64_t(-2113667072));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-8448
  r11 = uint64_t(int64_t(int64_t(r11) + -8448));
  // addi r30,r31,4
  r30 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r29,r4
  r29 = r4;
  // mr r3,r30
  r3 = r30;
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825cc0a0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTFader::Ctor(LIFTED_ARGS(0x825CB98C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r7,-32249
  r7 = uint64_t(int64_t(-2113470464));
  // stw r29,32(r31)
  guest::StoreU32(uint32_t(r31) + 32, uint32_t(r29));
  // addi r10,r31,36
  r10 = uint64_t(int64_t(int64_t(r31) + 36));
  // stw r27,28(r31)
  guest::StoreU32(uint32_t(r31) + 28, uint32_t(r27));
  // addi r6,r7,24544
  r6 = uint64_t(int64_t(int64_t(r7) + 24544));
  // lis r7,-32249
  r7 = uint64_t(int64_t(-2113470464));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r7,r7,21316
  r7 = uint64_t(int64_t(int64_t(r7) + 21316));
  // addi r9,r31,56
  r9 = uint64_t(int64_t(int64_t(r31) + 56));
  // addi r8,r31,68
  r8 = uint64_t(int64_t(int64_t(r31) + 68));
  // stw r6,0(r30)
  guest::StoreU32(uint32_t(r30) + 0, uint32_t(r6));
  // mr r4,r31
  r4 = r31;
  // lfs f13,2612(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f13 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,24556
  r11 = uint64_t(int64_t(int64_t(r11) + 24556));
  // stw r11,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r7,0(r10)
  guest::StoreU32(uint32_t(r10) + 0, uint32_t(r7));
  // stw r10,4(r10)
  guest::StoreU32(uint32_t(r10) + 4, uint32_t(r10));
  // stw r10,8(r10)
  guest::StoreU32(uint32_t(r10) + 8, uint32_t(r10));
  // stw r28,48(r31)
  guest::StoreU32(uint32_t(r31) + 48, uint32_t(r28));
  // stb r11,52(r31)
  guest::StoreU8(uint32_t(r31) + 52, uint8_t(r11));
  // stw r7,0(r9)
  guest::StoreU32(uint32_t(r9) + 0, uint32_t(r7));
  // stw r9,4(r9)
  guest::StoreU32(uint32_t(r9) + 4, uint32_t(r9));
  // stw r9,8(r9)
  guest::StoreU32(uint32_t(r9) + 8, uint32_t(r9));
  // stw r7,0(r8)
  guest::StoreU32(uint32_t(r8) + 0, uint32_t(r7));
  // stw r8,4(r8)
  guest::StoreU32(uint32_t(r8) + 4, uint32_t(r8));
  // stw r8,8(r8)
  guest::StoreU32(uint32_t(r8) + 8, uint32_t(r8));
  // stfs f0,112(r31)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r31) + 112, temp.u32);
  // lwz r10,48(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 48);
  // stfs f0,116(r31)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r31) + 116, temp.u32);
  // stw r11,80(r31)
  guest::StoreU32(uint32_t(r31) + 80, uint32_t(r11));
  // stfs f13,120(r31)
  temp.f32 = float(f13);
  guest::StoreU32(uint32_t(r31) + 120, temp.u32);
  // addi r3,r10,4
  r3 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r11,84(r31)
  guest::StoreU32(uint32_t(r31) + 84, uint32_t(r11));
  // stw r11,96(r31)
  guest::StoreU32(uint32_t(r31) + 96, uint32_t(r11));
  // stw r11,100(r31)
  guest::StoreU32(uint32_t(r31) + 100, uint32_t(r11));
  // stw r11,104(r31)
  guest::StoreU32(uint32_t(r31) + 104, uint32_t(r11));
  // stw r11,108(r31)
  guest::StoreU32(uint32_t(r31) + 108, uint32_t(r11));
  // stw r11,124(r31)
  guest::StoreU32(uint32_t(r31) + 124, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CBA44, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 28);
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r10,r10,23520
  r10 = uint64_t(int64_t(int64_t(r10) + 23520));
  // lbz r9,3(r11)
  r9 = guest::LoadU8(uint32_t(r11) + 3);
  // rotlwi r9,r9,2
  r9 = __builtin_rotateleft32(uint32_t(r9), 2);
  // lfsx f0,r9,r10
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r9) + uint32_t(r10));
  f0 = double(temp.f32);
  // stfs f0,88(r31)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r31) + 88, temp.u32);
  // lhz r11,4(r11)
  r11 = guest::LoadU16(uint32_t(r11) + 4);
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // cmpwi cr6,r11,2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2400, so);
  // blt cr6,0x825cba7c
  if (cr6.lt) goto loc_825CBA7C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8680(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 8680);
  f0 = double(temp.f32);
  // b 0x825cbab0
  goto loc_825CBAB0;
loc_825CBA7C:
  // cmpwi cr6,r11,-2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -2400, so);
  // bgt cr6,0x825cba90
  if (cr6.gt) goto loc_825CBA90;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,10416(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 10416);
  f0 = double(temp.f32);
  // b 0x825cbab0
  goto loc_825CBAB0;
loc_825CBA90:
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r11);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,23436(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 23436);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
loc_825CBAB0:
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stfs f0,92(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r31) + 92, temp.u32);
  // mr r3,r31
  r3 = r31;
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CBAD0: lifted from generated/ by lift.py.
lifted::Result f825CBAD0(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CBAD0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x825cae90
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Dtor(r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cbb10
  if (cr6.eq) goto loc_825CBB10;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32864
  r4 = r4 | 32864;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(LIFTED_ARGS(0x825CBB0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825CBB10:
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

// sub_825CBB28 game::XACTSound::Play: lifted from generated/ by lift.py.
lifted::Result f825CBB28(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CBB28 (lifted)");
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
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 80);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x825cbbf4
  if (!cr6.eq) goto loc_825CBBF4;
  // lwz r3,48(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 48);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,132(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 132);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CBB5C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825cbba4
  if (!cr6.eq) goto loc_825CBBA4;
  // mr r3,r31
  r3 = r31;
  // bl 0x825caac0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::CheckInstanceLimit(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // beq cr6,0x825cbb88
  if (cr6.eq) goto loc_825CBB88;
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // bne cr6,0x825cbba4
  if (!cr6.eq) goto loc_825CBBA4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CBB88:
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,8
  r3 = r3 | 8;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CBBA4:
  // lwz r30,40(r31)
  r30 = guest::LoadU32(uint32_t(r31) + 40);
  // addi r28,r31,36
  r28 = uint64_t(int64_t(int64_t(r31) + 36));
loc_825CBBAC:
  // cmplw cr6,r30,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r28), so);
  // beq cr6,0x825cbbd0
  if (cr6.eq) goto loc_825CBBD0;
  // mr r3,r30
  r3 = r30;
  // bl 0x825d01d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::Play(LIFTED_ARGS(0x825CBBBC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r30,4(r30)
  r30 = guest::LoadU32(uint32_t(r30) + 4);
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge cr6,0x825cbbac
  if (!cr6.lt) goto loc_825CBBAC;
  // b 0x825cbc14
  goto loc_825CBC14;
loc_825CBBD0:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825cbc14
  if (cr6.lt) goto loc_825CBC14;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTGetTimeMs(LIFTED_ARGS(0x825CBBDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // stw r11,100(r31)
  guest::StoreU32(uint32_t(r31) + 100, uint32_t(r11));
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x825cbc14
  goto loc_825CBC14;
loc_825CBBF4:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bge cr6,0x825cbc0c
  if (!cr6.lt) goto loc_825CBC0C;
  // lbz r11,52(r31)
  r11 = guest::LoadU8(uint32_t(r31) + 52);
  // ori r11,r11,64
  r11 = r11 | 64;
  // stb r11,52(r31)
  guest::StoreU8(uint32_t(r31) + 52, uint8_t(r11));
  // b 0x825cbc14
  goto loc_825CBC14;
loc_825CBC0C:
  // lis r29,-30009
  r29 = uint64_t(int64_t(-1966669824));
  // ori r29,r29,6
  r29 = r29 | 6;
loc_825CBC14:
  // lwz r31,32(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 32);
  // addi r30,r31,144
  r30 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlEnterCriticalSection>(LIFTED_ARGS(0x825CBC24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r3,12(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 12);
  // stw r11,184(r31)
  guest::StoreU32(uint32_t(r31) + 184, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CBC40, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::RtlLeaveCriticalSection>(LIFTED_ARGS(0x825CBC48, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CBC58 game::XACTSound::Stop: lifted from generated/ by lift.py.
lifted::Result f825CBC58(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CBC58 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // mr r28,r4
  r28 = r4;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lbz r11,52(r30)
  r11 = guest::LoadU8(uint32_t(r30) + 52);
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // bne cr6,0x825cbc8c
  if (!cr6.eq) goto loc_825CBC8C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825ca6f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Pause(r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r28,1
  r28 = uint64_t(int64_t(1));
loc_825CBC8C:
  // lwz r11,80(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 80);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x825cbd00
  if (cr6.eq) goto loc_825CBD00;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bne cr6,0x825cbcb8
  if (!cr6.eq) goto loc_825CBCB8;
  // lwz r11,124(r30)
  r11 = guest::LoadU32(uint32_t(r30) + 124);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825cbd00
  if (!cr6.eq) goto loc_825CBD00;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x825cbd00
  if (cr6.eq) goto loc_825CBD00;
  // b 0x825cbcc0
  goto loc_825CBCC0;
loc_825CBCB8:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x825cbcc8
  if (cr6.eq) goto loc_825CBCC8;
loc_825CBCC0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,124(r30)
  guest::StoreU32(uint32_t(r30) + 124, uint32_t(r11));
loc_825CBCC8:
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r31,40(r30)
  r31 = guest::LoadU32(uint32_t(r30) + 40);
  // addi r30,r30,36
  r30 = uint64_t(int64_t(int64_t(r30) + 36));
loc_825CBCDC:
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825cbd00
  if (cr6.eq) goto loc_825CBD00;
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d01d8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::Stop(LIFTED_ARGS(0x825CBCF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge cr6,0x825cbcdc
  if (!cr6.lt) goto loc_825CBCDC;
loc_825CBD00:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CBD10 game::XACTSound::AudioUpdate: lifted from generated/ by lift.py.
lifted::Result f825CBD10(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CBD10 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r28,r3
  r28 = r3;
  // lbz r11,52(r28)
  r11 = guest::LoadU8(uint32_t(r28) + 52);
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // beq cr6,0x825cc088
  if (cr6.eq) goto loc_825CC088;
  // lwz r11,80(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 80);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x825cbecc
  if (cr6.eq) goto loc_825CBECC;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x825cbecc
  if (cr6.eq) goto loc_825CBECC;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x825cbdb4
  if (!cr6.eq) goto loc_825CBDB4;
  // lwz r11,40(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 40);
  // addi r9,r28,36
  r9 = uint64_t(int64_t(int64_t(r28) + 36));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // beq cr6,0x825cbd80
  if (cr6.eq) goto loc_825CBD80;
loc_825CBD5C:
  // lwz r10,24(r11)
  r10 = guest::LoadU32(uint32_t(r11) + 24);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825cbdb4
  if (cr6.eq) goto loc_825CBDB4;
  // lwz r10,40(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 40);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // bne cr6,0x825cbdb4
  if (!cr6.eq) goto loc_825CBDB4;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x825cbd5c
  if (!cr6.eq) goto loc_825CBD5C;
loc_825CBD80:
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r28
  r3 = r28;
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lbz r11,52(r28)
  r11 = guest::LoadU8(uint32_t(r28) + 52);
  // rlwinm r10,r11,0,25,25
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  // cmplwi cr6,r10,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 64, so);
  // bne cr6,0x825cbdb4
  if (!cr6.eq) goto loc_825CBDB4;
  // andi. r11,r11,191
  r11 = r11 & 191;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r3,r28
  r3 = r28;
  // stb r11,52(r28)
  guest::StoreU8(uint32_t(r28) + 52, uint8_t(r11));
  // bl 0x825cbb28
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::Play(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825cc088
  if (cr6.lt) goto loc_825CC088;
loc_825CBDB4:
  // lwz r11,28(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 28);
  // lbz r11,0(r11)
  r11 = guest::LoadU8(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x825cbe00
  if (!cr6.eq) goto loc_825CBE00;
  // mr r3,r28
  r3 = r28;
  // bl 0x825cae08
  // (left out: dead in C++)
  { const lifted::Result o = f825CAE08(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // clrlwi r30,r3,24
  r30 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825cbe00
  if (cr6.eq) goto loc_825CBE00;
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_825CBDE0:
  // lwz r10,96(r28)
  r10 = guest::LoadU32(uint32_t(r28) + 96);
  // mulli r11,r31,44
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(44))));
  // add r3,r11,r10
  r3 = r11 + r10;
  // bl 0x825c9558
  // (left out: dead in C++)
  { const lifted::Result o = f825C9558(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // blt cr6,0x825cbde0
  if (cr6.lt) goto loc_825CBDE0;
loc_825CBE00:
  // lwz r31,40(r28)
  r31 = guest::LoadU32(uint32_t(r28) + 40);
  // addi r30,r28,36
  r30 = uint64_t(int64_t(int64_t(r28) + 36));
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825cbe24
  if (cr6.eq) goto loc_825CBE24;
loc_825CBE10:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d0d88
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTrack::AudioUpdate(LIFTED_ARGS(0x825CBE18, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825cbe10
  if (!cr6.eq) goto loc_825CBE10;
loc_825CBE24:
  // lwz r3,60(r28)
  r3 = guest::LoadU32(uint32_t(r28) + 60);
  // addi r30,r28,56
  r30 = uint64_t(int64_t(int64_t(r28) + 56));
  // cmplw cr6,r3,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r30), so);
  // beq cr6,0x825cbe84
  if (cr6.eq) goto loc_825CBE84;
loc_825CBE34:
  // lwz r11,32(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 32);
  // lwz r10,20(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 20);
  // lwz r11,24(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 24);
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // bgt cr6,0x825cbe84
  if (cr6.gt) goto loc_825CBE84;
  // lwz r11,8(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 8);
  // lwz r10,4(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 4);
  // stw r11,8(r10)
  guest::StoreU32(uint32_t(r10) + 8, uint32_t(r11));
  // lwz r11,8(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 8);
  // lwz r10,4(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 4);
  // stw r10,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(uint32_t(r3));
  // stw r3,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r3));
  // stw r3,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CBE78, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,60(r28)
  r3 = guest::LoadU32(uint32_t(r28) + 60);
  // cmplw cr6,r3,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r30), so);
  // bne cr6,0x825cbe34
  if (!cr6.eq) goto loc_825CBE34;
loc_825CBE84:
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lwz r11,-12816(r11)
  r11 = guest::LoadU32(uint32_t(r11) + -12816);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cbf40
  if (cr6.eq) goto loc_825CBF40;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::QueryPerformanceCounter(LIFTED_ARGS(0x825CBE9C, r31)); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = guest::LoadU64(uint32_t(r1) + 80);
  // mulli r9,r11,1000
  r9 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1000))));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // rotldi r10,r9,1
  r10 = __builtin_rotateleft64(r9, 1);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // ld r11,5248(r11)
  r11 = guest::LoadU64(uint32_t(r11) + 5248);
  // divd r9,r9,r11
  r9 = uint64_t(int64_t((int64_t(r11) && !(int64_t(r9) == INT64_MIN && int64_t(r11) == -1)) ? int64_t(r9) / int64_t(r11) : 0));
  // tdllei r11,0
  if (int64_t(r11) == 0ll || r11 < 0ull) lifted::Trap(0);
  // andc r11,r11,r10
  r11 = r11 & ~r10;
  // extsw r3,r9
  r3 = uint64_t(int64_t(int32_t(r9)));
  // tdlgei r11,-1
  if (int64_t(r11) == -1ll || r11 > 18446744073709551615ull) lifted::Trap(0);
  // b 0x825cbf44
  goto loc_825CBF44;
loc_825CBECC:
  // addi r3,r28,4
  r3 = uint64_t(int64_t(int64_t(r28) + 4));
  // bl 0x825cc1d0
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTFader::Step(LIFTED_ARGS(0x825CBED4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825cbee4
  if (cr6.eq) goto loc_825CBEE4;
  // mr r3,r28
  r3 = r28;
  // bl 0x825ca860
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::UpdateVolume(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825CBEE4:
  // lwz r10,40(r28)
  r10 = guest::LoadU32(uint32_t(r28) + 40);
  // addi r8,r28,36
  r8 = uint64_t(int64_t(int64_t(r28) + 36));
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // beq cr6,0x825cbf2c
  if (cr6.eq) goto loc_825CBF2C;
loc_825CBEF4:
  // lwz r11,24(r10)
  r11 = guest::LoadU32(uint32_t(r10) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cbf20
  if (cr6.eq) goto loc_825CBF20;
  // lwz r9,40(r11)
  r9 = guest::LoadU32(uint32_t(r11) + 40);
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // beq cr6,0x825cbdb4
  if (cr6.eq) goto loc_825CBDB4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825cbf20
  if (cr6.eq) goto loc_825CBF20;
  // rotlwi r11,r9,0
  r11 = __builtin_rotateleft32(uint32_t(r9), 0);
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x825cbdb4
  if (cr6.eq) goto loc_825CBDB4;
loc_825CBF20:
  // lwz r10,4(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 4);
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // bne cr6,0x825cbef4
  if (!cr6.eq) goto loc_825CBEF4;
loc_825CBF2C:
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r28
  r3 = r28;
  // bl 0x825cb1a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTSound::SetState(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_825CBF40:
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(LIFTED_ARGS(0x825CBF44, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CBF44:
  // lwz r11,32(r28)
  r11 = guest::LoadU32(uint32_t(r28) + 32);
  // lwz r31,60(r28)
  r31 = guest::LoadU32(uint32_t(r28) + 60);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // add r27,r11,r3
  r27 = r11 + r3;
  // beq cr6,0x825cbfdc
  if (cr6.eq) goto loc_825CBFDC;
loc_825CBF5C:
  // lwz r11,20(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 20);
  // lwz r29,4(r31)
  r29 = guest::LoadU32(uint32_t(r31) + 4);
  // cmpw cr6,r11,r27
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r27), so);
  // bgt cr6,0x825cbfdc
  if (cr6.gt) goto loc_825CBFDC;
  // lwz r11,16(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 16);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 0);
  // clrlwi r11,r11,27
  r11 = uint32_t(r11) & 0x1F;
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // beq cr6,0x825cbf88
  if (cr6.eq) goto loc_825CBF88;
  // cmpwi cr6,r11,11
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 11, so);
  // bne cr6,0x825cbfd0
  if (!cr6.eq) goto loc_825CBFD0;
loc_825CBF88:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d1b40
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTTransitionEvent::IsDue(r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825cbfc8
  if (cr6.eq) goto loc_825CBFC8;
  // lwz r11,44(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 16);
  // stw r31,36(r11)
  guest::StoreU32(uint32_t(r11) + 36, uint32_t(r31));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplw cr6,r3,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r30), so);
  // beq cr6,0x825cbfd0
  if (cr6.eq) goto loc_825CBFD0;
loc_825CBFB0:
  // lwz r31,4(r3)
  r31 = guest::LoadU32(uint32_t(r3) + 4);
  // bl 0x825d1830
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTEvent::Cancel(r1, r13, r3, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825cbfb0
  if (!cr6.eq) goto loc_825CBFB0;
  // b 0x825cbfd0
  goto loc_825CBFD0;
loc_825CBFC8:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d1830
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTEvent::Cancel(r1, r13, r3, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
loc_825CBFD0:
  // mr r31,r29
  r31 = r29;
  // cmplw cr6,r29,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r30), so);
  // bne cr6,0x825cbf5c
  if (!cr6.eq) goto loc_825CBF5C;
loc_825CBFDC:
  // lwz r31,72(r28)
  r31 = guest::LoadU32(uint32_t(r28) + 72);
  // addi r26,r28,68
  r26 = uint64_t(int64_t(int64_t(r28) + 68));
  // cmplw cr6,r31,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r26), so);
  // beq cr6,0x825cc088
  if (cr6.eq) goto loc_825CC088;
loc_825CBFEC:
  // lwz r11,36(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 36);
  // lwz r27,4(r31)
  r27 = guest::LoadU32(uint32_t(r31) + 4);
  // lbz r10,40(r11)
  r10 = guest::LoadU8(uint32_t(r11) + 40);
  // rlwinm r9,r10,0,0,24
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFF80;
  // cmplwi cr6,r9,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 128, so);
  // bne cr6,0x825cc07c
  if (!cr6.eq) goto loc_825CC07C;
  // lwz r9,16(r31)
  r9 = guest::LoadU32(uint32_t(r31) + 16);
  // lwz r9,0(r9)
  r9 = guest::LoadU32(uint32_t(r9) + 0);
  // rlwinm r9,r9,0,1,1
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0x40000000;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x825cc020
  if (cr6.eq) goto loc_825CC020;
  // lwz r30,20(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 20);
  // b 0x825cc038
  goto loc_825CC038;
loc_825CC020:
  // rlwinm r10,r10,0,25,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x40;
  // cmplwi cr6,r10,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 64, so);
  // bne cr6,0x825cc034
  if (!cr6.eq) goto loc_825CC034;
  // lwz r30,24(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 24);
  // b 0x825cc038
  goto loc_825CC038;
loc_825CC034:
  // lwz r30,28(r11)
  r30 = guest::LoadU32(uint32_t(r11) + 28);
loc_825CC038:
  // lwz r10,32(r28)
  r10 = guest::LoadU32(uint32_t(r28) + 32);
  // mr r3,r31
  r3 = r31;
  // lwz r29,24(r10)
  r29 = guest::LoadU32(uint32_t(r10) + 24);
  // bl 0x825d18f0
  // (left out: dead in C++)
  r3 = game::XACTEvent::GetStartTime(r3);
  // subf r11,r30,r29
  r11 = r29 - r30;
  // cmpw cr6,r11,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r3), so);
  // blt cr6,0x825cc07c
  if (cr6.lt) goto loc_825CC07C;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 4);
  // mr r3,r31
  r3 = r31;
  // lwz r10,8(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 8);
  // stw r10,8(r11)
  guest::StoreU32(uint32_t(r11) + 8, uint32_t(r10));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(uint32_t(r31) + 8);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(uint32_t(r31) + 4);
  // stw r10,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r31,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r31));
  // stw r31,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r31));
  // bl 0x825d1918
  // (left out: dead in C++)
  { const lifted::Result o = game::XACTEvent::Schedule(LIFTED_ARGS(0x825CC07C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825CC07C:
  // mr r31,r27
  r31 = r27;
  // cmplw cr6,r27,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r26), so);
  // bne cr6,0x825cbfec
  if (!cr6.eq) goto loc_825CBFEC;
loc_825CC088:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CC090: lifted from generated/ by lift.py.
lifted::Result f825CC090(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CC090 (lifted)");
  uint64_t r4 = 0;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x825cbc58
  { const lifted::Result o = game::XACTSound::Stop(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CC0E0: lifted from generated/ by lift.py.
void f825CC0E0(uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_825CC0E0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,16(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 16);
  // cmpwi cr6,r4,1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 1, so);
  // stw r4,16(r3)
  guest::StoreU32(uint32_t(r3) + 16, uint32_t(r4));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // beq cr6,0x825cc124
  if (cr6.eq) goto loc_825CC124;
  // cmpwi cr6,r4,2
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 2, so);
  // bnelr cr6
  if (!cr6.eq) return;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cc13c
  if (cr6.eq) goto loc_825CC13C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r6,12(r3)
  guest::StoreU32(uint32_t(r3) + 12, uint32_t(r6));
  // sth r5,8(r3)
  guest::StoreU16(uint32_t(r3) + 8, uint16_t(r5));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,4(r3)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r3) + 4, temp.u32);
  // stfs f0,20(r3)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r3) + 20, temp.u32);
  // blr
  return;
loc_825CC124:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825cc13c
  if (cr6.eq) goto loc_825CC13C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,4(r3)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r3) + 4, temp.u32);
  // stfs f0,20(r3)
  temp.f32 = float(f0);
  guest::StoreU32(uint32_t(r3) + 20, temp.u32);
loc_825CC13C:
  // stw r6,12(r3)
  guest::StoreU32(uint32_t(r3) + 12, uint32_t(r6));
  // sth r5,8(r3)
  guest::StoreU16(uint32_t(r3) + 8, uint16_t(r5));
  // blr
  return;
}

lifted::Result e825C68D8(const lifted::Args& a) {
  return f825C68D8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C6918(const lifted::Args& a) {
  return {f825C6918(a.r[0]), a.f[0]};
}

lifted::Result e825C6988(const lifted::Args& a) {
  return f825C6988(a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e825C6A10(const lifted::Args& a) {
  return f825C6A10(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e825C6A18(const lifted::Args& a) {
  return f825C6A18(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e825C6B50(const lifted::Args& a) {
  return f825C6B50(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C6C10(const lifted::Args& a) {
  return f825C6C10(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C6DF8(const lifted::Args& a) {
  return f825C6DF8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C7018(const lifted::Args& a) {
  return f825C7018(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C7070(const lifted::Args& a) {
  return f825C7070(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C70C0(const lifted::Args& a) {
  return f825C70C0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C7510(const lifted::Args& a) {
  return f825C7510(a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825C7540(const lifted::Args& a) {
  return f825C7540(a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825C7570(const lifted::Args& a) {
  return f825C7570(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e825C7608(const lifted::Args& a) {
  return f825C7608(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e825C7750(const lifted::Args& a) {
  return f825C7750(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C7860(const lifted::Args& a) {
  return f825C7860(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825C7960(const lifted::Args& a) {
  return f825C7960(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C7A30(const lifted::Args& a) {
  return f825C7A30(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C7A38(const lifted::Args& a) {
  return f825C7A38(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C7BD0(const lifted::Args& a) {
  return f825C7BD0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C7C30(const lifted::Args& a) {
  return f825C7C30(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C7CD0(const lifted::Args& a) {
  return f825C7CD0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C7D28(const lifted::Args& a) {
  return {f825C7D28(a.r[1]), a.f[0]};
}

lifted::Result e825C7D40(const lifted::Args& a) {
  return f825C7D40(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C7DA8(const lifted::Args& a) {
  return f825C7DA8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825C82B8(const lifted::Args& a) {
  return f825C82B8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C8380(const lifted::Args& a) {
  return f825C8380(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825C83B8(const lifted::Args& a) {
  return {f825C83B8(a.r[0]), a.f[0]};
}

lifted::Result e825C83C8(const lifted::Args& a) {
  return {f825C83C8(a.r[0]), a.f[0]};
}

lifted::Result e825C83D8(const lifted::Args& a) {
  return f825C83D8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825C85C8(const lifted::Args& a) {
  return {f825C85C8(a.r[0]), a.f[0]};
}

lifted::Result e825C85D8(const lifted::Args& a) {
  return {f825C85D8(a.r[0]), a.f[0]};
}

lifted::Result e825C85F8(const lifted::Args& a) {
  return f825C85F8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825C8698(const lifted::Args& a) {
  return {f825C8698(a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825C8708(const lifted::Args& a) {
  return f825C8708(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C8710(const lifted::Args& a) {
  return f825C8710(a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e825C8798(const lifted::Args& a) {
  return {f825C8798(a.r[0]), a.f[0]};
}

lifted::Result e825C87A8(const lifted::Args& a) {
  return {f825C87A8(a.r[0]), a.f[0]};
}

lifted::Result e825C87B8(const lifted::Args& a) {
  return {f825C87B8(a.r[0]), a.f[0]};
}

lifted::Result e825C87C8(const lifted::Args& a) {
  return {f825C87C8(a.r[0]), a.f[0]};
}

lifted::Result e825C87D8(const lifted::Args& a) {
  return {f825C87D8(a.r[0]), a.f[0]};
}

lifted::Result e825C87E8(const lifted::Args& a) {
  return {f825C87E8(a.r[0]), a.f[0]};
}

lifted::Result e825C87F8(const lifted::Args& a) {
  return {f825C87F8(a.r[0]), a.f[0]};
}

lifted::Result e825C8808(const lifted::Args& a) {
  return f825C8808(a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C88A0(const lifted::Args& a) {
  return f825C88A0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C8A18(const lifted::Args& a) {
  return f825C8A18(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C8A98(const lifted::Args& a) {
  return f825C8A98(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825C8AC0(const lifted::Args& a) {
  return f825C8AC0(a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C8B28(const lifted::Args& a) {
  return {f825C8B28(a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825C8B48(const lifted::Args& a) {
  return {f825C8B48(a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825C8B80(const lifted::Args& a) {
  return f825C8B80(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825C8C40(const lifted::Args& a) {
  return f825C8C40(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C8C98(const lifted::Args& a) {
  return f825C8C98(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C8E18(const lifted::Args& a) {
  return f825C8E18(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C8E68(const lifted::Args& a) {
  return {f825C8E68(a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825C93A8(const lifted::Args& a) {
  return f825C93A8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C9558(const lifted::Args& a) {
  return f825C9558(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C9AF8(const lifted::Args& a) {
  return {f825C9AF8(a.r[0]), a.f[0]};
}

lifted::Result e825C9C30(const lifted::Args& a) {
  return f825C9C30(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825C9D18(const lifted::Args& a) {
  return f825C9D18(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825C9E90(const lifted::Args& a) {
  return f825C9E90(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825C9ED8(const lifted::Args& a) {
  return f825C9ED8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825C9EE0(const lifted::Args& a) {
  return f825C9EE0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825CA3B8(const lifted::Args& a) {
  return f825CA3B8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[6]);
}

lifted::Result e825CA4B8(const lifted::Args& a) {
  return f825CA4B8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CA560(const lifted::Args& a) {
  return f825CA560(a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CA680(const lifted::Args& a) {
  return f825CA680(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CA6F0(const lifted::Args& a) {
  return f825CA6F0(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825CA6F8(const lifted::Args& a) {
  return f825CA6F8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CA808(const lifted::Args& a) {
  return f825CA808(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CA860(const lifted::Args& a) {
  return f825CA860(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CA900(const lifted::Args& a) {
  return f825CA900(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CA940(const lifted::Args& a) {
  return f825CA940(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CA9C0(const lifted::Args& a) {
  return f825CA9C0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CAA00(const lifted::Args& a) {
  return f825CAA00(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CAA58(const lifted::Args& a) {
  return f825CAA58(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CAAC0(const lifted::Args& a) {
  return f825CAAC0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CAC58(const lifted::Args& a) {
  return f825CAC58(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CACB8(const lifted::Args& a) {
  return f825CACB8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CAD18(const lifted::Args& a) {
  return f825CAD18(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CADC0(const lifted::Args& a) {
  return f825CADC0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825CADF0(const lifted::Args& a) {
  return f825CADF0(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825CAE08(const lifted::Args& a) {
  return f825CAE08(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CAE90(const lifted::Args& a) {
  return f825CAE90(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825CAFD8(const lifted::Args& a) {
  return f825CAFD8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CB040(const lifted::Args& a) {
  return f825CB040(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CB188(const lifted::Args& a) {
  f825CB188(a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825CB1A8(const lifted::Args& a) {
  return f825CB1A8(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CB348(const lifted::Args& a) {
  return f825CB348(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CB660(const lifted::Args& a) {
  return {f825CB660(a.r[0]), a.f[0]};
}

lifted::Result e825CB6A0(const lifted::Args& a) {
  return {f825CB6A0(a.r[0]), a.f[0]};
}

lifted::Result e825CB708(const lifted::Args& a) {
  return {f825CB708(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e825CB958(const lifted::Args& a) {
  return f825CB958(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e825CBAD0(const lifted::Args& a) {
  return f825CBAD0(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CBB28(const lifted::Args& a) {
  return f825CBB28(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CBC58(const lifted::Args& a) {
  return f825CBC58(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CBD10(const lifted::Args& a) {
  return f825CBD10(a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CC090(const lifted::Args& a) {
  return f825CC090(a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CC0E0(const lifted::Args& a) {
  f825CC0E0(a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

namespace {

const Registration kEntries[] = {
    {0x825C68D8, e825C68D8},
    {0x825C6918, e825C6918},
    {0x825C6988, e825C6988},
    {0x825C6A10, e825C6A10},
    {0x825C6A18, e825C6A18},
    {0x825C6B50, e825C6B50},
    {0x825C6C10, e825C6C10},
    {0x825C6DF8, e825C6DF8},
    {0x825C7018, e825C7018},
    {0x825C7070, e825C7070},
    {0x825C70C0, e825C70C0},
    {0x825C7510, e825C7510},
    {0x825C7540, e825C7540},
    {0x825C7570, e825C7570},
    {0x825C7608, e825C7608},
    {0x825C7750, e825C7750},
    {0x825C7860, e825C7860},
    {0x825C7960, e825C7960},
    {0x825C7A30, e825C7A30},
    {0x825C7A38, e825C7A38},
    {0x825C7BD0, e825C7BD0},
    {0x825C7C30, e825C7C30},
    {0x825C7CD0, e825C7CD0},
    {0x825C7D28, e825C7D28},
    {0x825C7D40, e825C7D40},
    {0x825C7DA8, e825C7DA8},
    {0x825C82B8, e825C82B8},
    {0x825C8380, e825C8380},
    {0x825C83B8, e825C83B8},
    {0x825C83C8, e825C83C8},
    {0x825C83D8, e825C83D8},
    {0x825C85C8, e825C85C8},
    {0x825C85D8, e825C85D8},
    {0x825C85F8, e825C85F8},
    {0x825C8698, e825C8698},
    {0x825C8708, e825C8708},
    {0x825C8710, e825C8710},
    {0x825C8798, e825C8798},
    {0x825C87A8, e825C87A8},
    {0x825C87B8, e825C87B8},
    {0x825C87C8, e825C87C8},
    {0x825C87D8, e825C87D8},
    {0x825C87E8, e825C87E8},
    {0x825C87F8, e825C87F8},
    {0x825C8808, e825C8808},
    {0x825C88A0, e825C88A0},
    {0x825C8A18, e825C8A18},
    {0x825C8A98, e825C8A98},
    {0x825C8AC0, e825C8AC0},
    {0x825C8B28, e825C8B28},
    {0x825C8B48, e825C8B48},
    {0x825C8B80, e825C8B80},
    {0x825C8C40, e825C8C40},
    {0x825C8C98, e825C8C98},
    {0x825C8E18, e825C8E18},
    {0x825C8E68, e825C8E68},
    {0x825C93A8, e825C93A8},
    {0x825C9558, e825C9558},
    {0x825C9AF8, e825C9AF8},
    {0x825C9C30, e825C9C30},
    {0x825C9D18, e825C9D18},
    {0x825C9E90, e825C9E90},
    {0x825C9ED8, e825C9ED8},
    {0x825C9EE0, e825C9EE0},
    {0x825CA3B8, e825CA3B8},
    {0x825CA4B8, e825CA4B8},
    {0x825CA560, e825CA560},
    {0x825CA680, e825CA680},
    {0x825CA6F0, e825CA6F0},
    {0x825CA6F8, e825CA6F8},
    {0x825CA808, e825CA808},
    {0x825CA860, e825CA860},
    {0x825CA900, e825CA900},
    {0x825CA940, e825CA940},
    {0x825CA9C0, e825CA9C0},
    {0x825CAA00, e825CAA00},
    {0x825CAA58, e825CAA58},
    {0x825CAAC0, e825CAAC0},
    {0x825CAC58, e825CAC58},
    {0x825CACB8, e825CACB8},
    {0x825CAD18, e825CAD18},
    {0x825CADC0, e825CADC0},
    {0x825CADF0, e825CADF0},
    {0x825CAE08, e825CAE08},
    {0x825CAE90, e825CAE90},
    {0x825CAFD8, e825CAFD8},
    {0x825CB040, e825CB040},
    {0x825CB188, e825CB188},
    {0x825CB1A8, e825CB1A8},
    {0x825CB348, e825CB348},
    {0x825CB660, e825CB660},
    {0x825CB6A0, e825CB6A0},
    {0x825CB708, e825CB708},
    {0x825CB958, e825CB958},
    {0x825CBAD0, e825CBAD0},
    {0x825CBB28, e825CBB28},
    {0x825CBC58, e825CBC58},
    {0x825CBD10, e825CBD10},
    {0x825CC090, e825CC090},
    {0x825CC0E0, e825CC0E0},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_023)), true);

}  // namespace

}  // namespace lifted
