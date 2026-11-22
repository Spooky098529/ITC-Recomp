// Lift sweep batch 035 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/c_runtime.h"
#include "schema/race_setup.h"
#include "schema/system_services.h"
#include "schema/xact.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_035, true, "ITC",
              "Run lift sweep batch 035's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82099BA0 game::XACTWaveEntry::GetPlayLength: lifted from generated/ by lift.py.
uint64_t f82099BA0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82099BA0 (lifted)");
  // lwz r3,24(r3)
  r3 = guest::LoadU32(base, schema::XACTWaveEntry(base, uint32_t(r3)).play_length_address());
  // blr
  return r3;
}

// sub_82101D20: lifted from generated/ by lift.py.
void f82101D20(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82101D20 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stfs f0,792(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::RaceDataEntry(base, uint32_t(r3)).mean_address(), temp.u32);
  // stw r11,796(r3)
  guest::StoreU32(base, schema::RaceDataEntry(base, uint32_t(r3)).mean_count_address(), uint32_t(r11));
  // blr
  return;
}

// sub_8210B174: lifted from generated/ by lift.py.
uint64_t f8210B174(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8210B174 (lifted)");
  uint64_t r3 = 0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_8211A660: lifted from generated/ by lift.py.
void f8211A660(uint8_t* base, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8211A660 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmplwi cr6,r5,11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 11, so);
  // bgt cr6,0x8211a684
  if (cr6.gt) goto loc_8211A684;
  // addi r11,r5,62
  r11 = uint64_t(int64_t(int64_t(r5) + 62));
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // cmplwi cr6,r11,147
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 147, so);
  // blt cr6,0x8211a6b4
  if (cr6.lt) goto loc_8211A6B4;
  // li r11,147
  r11 = uint64_t(int64_t(147));
  // b 0x8211a6b4
  goto loc_8211A6B4;
loc_8211A684:
  // cmplwi cr6,r5,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 12, so);
  // blt cr6,0x8211a6a8
  if (cr6.lt) goto loc_8211A6A8;
  // cmplwi cr6,r5,43
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 43, so);
  // bgt cr6,0x8211a6b0
  if (cr6.gt) goto loc_8211A6B0;
  // addi r11,r5,291
  r11 = uint64_t(int64_t(int64_t(r5) + 291));
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // cmplwi cr6,r11,667
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 667, so);
  // blt cr6,0x8211a6b4
  if (cr6.lt) goto loc_8211A6B4;
  // b 0x8211a6b0
  goto loc_8211A6B0;
loc_8211A6A8:
  // cmplwi cr6,r5,43
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 43, so);
  // ble cr6,0x8211a6b4
  if (!cr6.gt) goto loc_8211A6B4;
loc_8211A6B0:
  // li r11,667
  r11 = uint64_t(int64_t(667));
loc_8211A6B4:
  // stw r11,72(r4)
  guest::StoreU32(base, uint32_t(r4) + 72, uint32_t(r11));
  // blr
  return;
}

// sub_82297340: lifted from generated/ by lift.py.
uint64_t f82297340(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82297340 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r22,r3
  r22 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r10,r22
  r10 = r22;
  // cmpwi cr6,r4,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), -1, so);
  // bne cr6,0x82297388
  if (!cr6.eq) goto loc_82297388;
  // mr r11,r22
  r11 = r22;
  // mr r9,r11
  r9 = r11;
loc_82297368:
  // lbz r8,0(r11)
  r8 = guest::LoadU8(base, schema::Utf8Bytes(base, uint32_t(r11)).byte0_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // bne cr6,0x82297368
  if (!cr6.eq) goto loc_82297368;
  // subf r11,r9,r11
  r11 = r11 - r9;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // addi r4,r11,1
  r4 = uint64_t(int64_t(int64_t(r11) + 1));
loc_82297388:
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // ble cr6,0x82297518
  if (!cr6.gt) goto loc_82297518;
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r24,r11,-30768
  r24 = uint64_t(int64_t(int64_t(r11) + -30768));
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // lis r27,-32156
  r27 = uint64_t(int64_t(-2107375616));
  // lis r26,-32156
  r26 = uint64_t(int64_t(-2107375616));
  // lis r28,-32156
  r28 = uint64_t(int64_t(-2107375616));
  // lis r29,-32156
  r29 = uint64_t(int64_t(-2107375616));
  // lis r25,-32156
  r25 = uint64_t(int64_t(-2107375616));
  // lis r21,-32156
  r21 = uint64_t(int64_t(-2107375616));
  // lis r20,-32156
  r20 = uint64_t(int64_t(-2107375616));
  // lis r23,-32156
  r23 = uint64_t(int64_t(-2107375616));
  // addi r30,r11,-30744
  r30 = uint64_t(int64_t(int64_t(r11) + -30744));
loc_822973C4:
  // lbz r8,0(r10)
  r8 = guest::LoadU8(base, schema::Utf8Bytes(base, uint32_t(r10)).byte0_address());
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lbzx r9,r8,r30
  r9 = guest::LoadU8(base, uint32_t(r8) + uint32_t(r30));
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // clrlwi r7,r9,16
  r7 = uint32_t(r9) & 0xFFFF;
  // subf r9,r22,r7
  r9 = r7 - r22;
  // add r9,r9,r10
  r9 = r9 + r10;
  // cmpw cr6,r9,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r4), so);
  // bgt cr6,0x82297518
  if (cr6.gt) goto loc_82297518;
  // cmplwi cr6,r7,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 1, so);
  // blt cr6,0x82297468
  if (cr6.lt) goto loc_82297468;
  // beq cr6,0x82297454
  if (cr6.eq) goto loc_82297454;
  // cmplwi cr6,r7,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 3, so);
  // blt cr6,0x82297440
  if (cr6.lt) goto loc_82297440;
  // beq cr6,0x8229742c
  if (cr6.eq) goto loc_8229742C;
  // cmplwi cr6,r7,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 5, so);
  // blt cr6,0x82297418
  if (cr6.lt) goto loc_82297418;
  // bne cr6,0x82297478
  if (!cr6.eq) goto loc_82297478;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // rlwinm r11,r8,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 6) & 0xFFFFFFC0;
loc_82297418:
  // lbz r9,0(r10)
  r9 = guest::LoadU8(base, schema::Utf8Bytes(base, uint32_t(r10) - 1).byte1_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r11,r9,r11
  r11 = r9 + r11;
  // rlwinm r11,r11,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0xFFFFFFC0;
loc_8229742C:
  // lbz r9,0(r10)
  r9 = guest::LoadU8(base, schema::Utf8Bytes(base, uint32_t(r10) - 2).byte2_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r11,r9,r11
  r11 = r9 + r11;
  // rlwinm r11,r11,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0xFFFFFFC0;
loc_82297440:
  // lbz r9,0(r10)
  r9 = guest::LoadU8(base, schema::Utf8Bytes(base, uint32_t(r10) - 3).byte3_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r11,r9,r11
  r11 = r9 + r11;
  // rlwinm r11,r11,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0xFFFFFFC0;
loc_82297454:
  // lbz r9,0(r10)
  r9 = guest::LoadU8(base, schema::Utf8Bytes(base, uint32_t(r10) - 4).byte4_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r11,r9,r11
  r11 = r9 + r11;
  // rlwinm r11,r11,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0xFFFFFFC0;
loc_82297468:
  // lbz r9,0(r10)
  r9 = guest::LoadU8(base, schema::Utf8Bytes(base, uint32_t(r10) - 5).byte5_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r11,r9,r11
  r11 = r9 + r11;
loc_82297478:
  // rlwinm r9,r7,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // lwzx r9,r9,r24
  r9 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r24));
  // subf r11,r9,r11
  r11 = r11 - r9;
  // lwz r9,-30796(r23)
  r9 = guest::LoadU32(base, uint32_t(r23) + -30796);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bgt cr6,0x822974a0
  if (cr6.gt) goto loc_822974A0;
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
  // cmpw cr6,r3,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r6), so);
  // bgt cr6,0x82297510
  if (cr6.gt) goto loc_82297510;
  // b 0x82297508
  goto loc_82297508;
loc_822974A0:
  // lwz r9,-30792(r20)
  r9 = guest::LoadU32(base, uint32_t(r20) + -30792);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // ble cr6,0x822974c0
  if (!cr6.gt) goto loc_822974C0;
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
  // cmpw cr6,r3,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r6), so);
  // bgt cr6,0x82297510
  if (cr6.gt) goto loc_82297510;
  // lwz r11,-30800(r21)
  r11 = guest::LoadU32(base, uint32_t(r21) + -30800);
  // b 0x82297508
  goto loc_82297508;
loc_822974C0:
  // lwz r9,-30784(r25)
  r9 = guest::LoadU32(base, uint32_t(r25) + -30784);
  // addi r7,r3,1
  r7 = uint64_t(int64_t(int64_t(r3) + 1));
  // subf r11,r9,r11
  r11 = r11 - r9;
  // cmpw cr6,r7,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r6), so);
  // bgt cr6,0x822974ec
  if (cr6.gt) goto loc_822974EC;
  // lwz r9,-30788(r29)
  r9 = guest::LoadU32(base, uint32_t(r29) + -30788);
  // srw r8,r11,r9
  r8 = uint8_t(r9) & 0x20 ? 0 : (uint32_t(r11) >> (uint8_t(r9) & 0x3F));
  // lwz r9,-30776(r28)
  r9 = guest::LoadU32(base, uint32_t(r28) + -30776);
  // add r9,r8,r9
  r9 = r8 + r9;
  // sth r9,0(r5)
  guest::StoreU16(base, schema::Utf16Chars(base, uint32_t(r5)).char0_address(), uint16_t(r9));
  // addi r5,r5,2
  r5 = uint64_t(int64_t(int64_t(r5) + 2));
loc_822974EC:
  // addi r3,r7,1
  r3 = uint64_t(int64_t(int64_t(r7) + 1));
  // cmpw cr6,r3,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r6), so);
  // bgt cr6,0x82297510
  if (cr6.gt) goto loc_82297510;
  // lwz r9,-30780(r26)
  r9 = guest::LoadU32(base, uint32_t(r26) + -30780);
  // and r9,r9,r11
  r9 = r9 & r11;
  // lwz r11,-30772(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + -30772);
  // add r11,r9,r11
  r11 = r9 + r11;
loc_82297508:
  // sth r11,0(r5)
  guest::StoreU16(base, schema::Utf16Chars(base, uint32_t(r5) - 2).char1_address(), uint16_t(r11));
  // addi r5,r5,2
  r5 = uint64_t(int64_t(int64_t(r5) + 2));
loc_82297510:
  // cmpw cr6,r31,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r4), so);
  // blt cr6,0x822973c4
  if (cr6.lt) goto loc_822973C4;
loc_82297518:
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // beq cr6,0x82297534
  if (cr6.eq) goto loc_82297534;
  // cmpw cr6,r6,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r6), int32_t(r3), so);
  // bge cr6,0x82297534
  if (!cr6.lt) goto loc_82297534;
  // li r3,122
  r3 = uint64_t(int64_t(122));
  // bl 0x82296fd0
  // (left out: dead in C++)
  { const lifted::Result o = game::SetLastError_82296FD0(base, LIFTED_ARGS(0x82297530, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82297534:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return r3;
}

// sub_822B07B0 game::SrgbCurve: lifted from generated/ by lift.py.
lifted::Result f822B07B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_822B07B0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beq cr6,0x822b080c
  if (cr6.eq) goto loc_822B080C;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfd f0,17296(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 17296)));
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x822b07e0
  if (cr6.gt) goto loc_822B07E0;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,17292(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17292);
  f0 = double(temp.f32);
  // b 0x822b0824
  goto loc_822B0824;
loc_822B07E0:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,17288(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17288);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fadds f13,f1,f0
  f13 = double(float(f1 + f0));
  // lfd f2,17280(r11)
  f2 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 17280)));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,17272(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17272);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = game::pow(base, LIFTED_ARGS(0x822B0804, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f1,f1
  lifted::ScalarFloatMode();
  f1 = double(float(f1));
  // b 0x822b0850
  goto loc_822B0850;
loc_822B080C:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfd f0,17264(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 17264)));
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x822b082c
  if (cr6.gt) goto loc_822B082C;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,17256(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17256);
  f0 = double(temp.f32);
loc_822B0824:
  // fmuls f1,f1,f0
  lifted::ScalarFloatMode();
  f1 = double(float(f1 * f0));
  // b 0x822b0850
  goto loc_822B0850;
loc_822B082C:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfd f2,17248(r11)
  lifted::ScalarFloatMode();
  f2 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 17248)));
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = game::pow(base, LIFTED_ARGS(0x822B0838, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f12,f1
  lifted::ScalarFloatMode();
  f12 = double(float(f1));
  // lfs f0,17240(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17240);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,17288(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17288);
  f13 = double(temp.f32);
  // fmsubs f1,f12,f0,f13
  f1 = double(float(std::fma(f12, f0, -f13)));
loc_822B0850:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822B0860 game::Bt709Curve: lifted from generated/ by lift.py.
lifted::Result f822B0860(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_822B0860 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beq cr6,0x822b08bc
  if (cr6.eq) goto loc_822B08BC;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfd f0,17352(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 17352)));
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x822b0890
  if (!cr6.lt) goto loc_822B0890;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,17348(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17348);
  f0 = double(temp.f32);
  // b 0x822b08d4
  goto loc_822B08D4;
loc_822B0890:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,17344(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17344);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fadds f13,f1,f0
  f13 = double(float(f1 + f0));
  // lfd f2,17336(r11)
  f2 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 17336)));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,17328(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17328);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = game::pow(base, LIFTED_ARGS(0x822B08B4, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f1,f1
  lifted::ScalarFloatMode();
  f1 = double(float(f1));
  // b 0x822b0900
  goto loc_822B0900;
loc_822B08BC:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfd f0,17320(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 17320)));
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x822b08dc
  if (!cr6.lt) goto loc_822B08DC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,9584(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9584);
  f0 = double(temp.f32);
loc_822B08D4:
  // fmuls f1,f1,f0
  lifted::ScalarFloatMode();
  f1 = double(float(f1 * f0));
  // b 0x822b0900
  goto loc_822B0900;
loc_822B08DC:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfd f2,17312(r11)
  lifted::ScalarFloatMode();
  f2 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 17312)));
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = game::pow(base, LIFTED_ARGS(0x822B08E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f12,f1
  lifted::ScalarFloatMode();
  f12 = double(float(f1));
  // lfs f0,17304(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17304);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,17344(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 17344);
  f13 = double(temp.f32);
  // fmsubs f1,f12,f0,f13
  f1 = double(float(std::fma(f12, f0, -f13)));
loc_822B0900:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82379320: lifted from generated/ by lift.py.
uint64_t f82379320(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82379320 (lifted)");
  // lwz r3,124(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 124);
  // blr
  return r3;
}

// sub_824457CC: lifted from generated/ by lift.py.
uint64_t f824457CC(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_824457CC (lifted)");
  uint64_t r3 = 0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_82488508 game::MovieSource::Skip: lifted from generated/ by lift.py.
uint64_t f82488508(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82488508 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // ld r10,56(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 56);
  // ld r8,64(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 64);
  // ld r9,88(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 88);
  // add r8,r8,r10
  r8 = r8 + r10;
  // add r9,r9,r4
  r9 = r9 + r4;
  // cmpld cr6,r9,r8
  cr6 = lifted::Compare<uint64_t>(r9, r8, so);
  // bgt cr6,0x82488534
  if (cr6.gt) goto loc_82488534;
  // cmpld cr6,r9,r10
  cr6 = lifted::Compare<uint64_t>(r9, r10, so);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // bge cr6,0x82488538
  if (!cr6.lt) goto loc_82488538;
loc_82488534:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_82488538:
  // stw r10,48(r11)
  guest::StoreU32(base, uint32_t(r11) + 48, uint32_t(r10));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // std r9,88(r11)
  guest::StoreU64(base, uint32_t(r11) + 88, r9);
  // blr
  return r3;
}

// sub_82488548 game::MovieSource::Seek: lifted from generated/ by lift.py.
uint64_t f82488548(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82488548 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // cmpwi cr6,r4,1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 1, so);
  // beq cr6,0x8248857c
  if (cr6.eq) goto loc_8248857C;
  // cmpwi cr6,r4,3
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 3, so);
  // bne cr6,0x8248857c
  if (!cr6.eq) goto loc_8248857C;
  // ld r10,64(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 64);
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // ld r9,56(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 56);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r10,r10,r9
  r10 = r10 + r9;
  // stw r8,48(r11)
  guest::StoreU32(base, uint32_t(r11) + 48, uint32_t(r8));
  // std r10,88(r11)
  guest::StoreU64(base, uint32_t(r11) + 88, r10);
  // blr
  return r3;
loc_8248857C:
  // ld r10,56(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 56);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r9,48(r11)
  guest::StoreU32(base, uint32_t(r11) + 48, uint32_t(r9));
  // std r10,88(r11)
  guest::StoreU64(base, uint32_t(r11) + 88, r10);
  // blr
  return r3;
}

// sub_8248CA78: lifted from generated/ by lift.py.
uint64_t f8248CA78(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8248CA78 (lifted)");
  // lwz r3,284(r3)
  r3 = guest::LoadU32(base, uint32_t(r3) + 284);
  // blr
  return r3;
}

// sub_824F65C0: lifted from generated/ by lift.py.
uint64_t f824F65C0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_824F65C0 (lifted)");
  uint64_t r3 = 0;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // blr
  return r3;
}

// sub_825A9958 game::wcslen: lifted from generated/ by lift.py.
uint64_t f825A9958(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A9958 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // mr r11,r3
  r11 = r3;
loc_825A995C:
  // lhz r10,0(r11)
  r10 = guest::LoadU16(base, schema::WideString(base, uint32_t(r11)).units_address());
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x825a995c
  if (!cr0.eq) goto loc_825A995C;
  // subf r11,r3,r11
  r11 = r11 - r3;
  // srawi r11,r11,1
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x1) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 1));
  // addi r3,r11,-1
  r3 = uint64_t(int64_t(int64_t(r11) + -1));
  // blr
  return r3;
}

// sub_82445798: lifted from generated/ by lift.py.
uint64_t f82445798(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82445798 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, ctr = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // addi r11,r3,-2
  r11 = uint64_t(int64_t(int64_t(r3) + -2));
  // cmplwi cr6,r11,36
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 36, so);
  // bgt cr6,0x824457f4
  if (cr6.gt) goto loc_824457F4;
  // lis r12,-32252
  r12 = uint64_t(int64_t(-2113667072));
  // addi r12,r12,12584
  r12 = uint64_t(int64_t(int64_t(r12) + 12584));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // lis r12,-32188
  r12 = uint64_t(int64_t(-2109472768));
  // addi r12,r12,22476
  r12 = uint64_t(int64_t(int64_t(r12) + 22476));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_824457CC;
  return r3;
  case 1:
  goto loc_824457D4;
  case 2:
  goto loc_824457D4;
  case 3:
  goto loc_824457D4;
  case 4:
  goto loc_824457DC;
  case 5:
  goto loc_824457DC;
  case 6:
  goto loc_frag_824457CC;
  return r3;
  case 7:
  goto loc_frag_824457CC;
  return r3;
  case 8:
  goto loc_824457D4;
  case 9:
  goto loc_824457F4;
  case 10:
  goto loc_824457F4;
  case 11:
  goto loc_824457F4;
  case 12:
  goto loc_824457DC;
  case 13:
  goto loc_824457D4;
  case 14:
  goto loc_824457DC;
  case 15:
  goto loc_824457DC;
  case 16:
  goto loc_824457F4;
  case 17:
  goto loc_824457F4;
  case 18:
  goto loc_824457F4;
  case 19:
  goto loc_824457F4;
  case 20:
  goto loc_824457F4;
  case 21:
  goto loc_824457F4;
  case 22:
  goto loc_824457D4;
  case 23:
  goto loc_824457DC;
  case 24:
  goto loc_824457E4;
  case 25:
  goto loc_824457F4;
  case 26:
  goto loc_824457F4;
  case 27:
  goto loc_824457F4;
  case 28:
  goto loc_824457D4;
  case 29:
  goto loc_824457DC;
  case 30:
  goto loc_824457E4;
  case 31:
  goto loc_824457F4;
  case 32:
  goto loc_824457F4;
  case 33:
  goto loc_824457F4;
  case 34:
  goto loc_824457DC;
  case 35:
  goto loc_824457E4;
  case 36:
  goto loc_824457EC;
  default:
  std::abort();
  }
loc_824457D4:
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // blr
  return r3;
loc_824457DC:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // blr
  return r3;
loc_824457E4:
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // blr
  return r3;
loc_824457EC:
  // li r3,16
  r3 = uint64_t(int64_t(16));
  // blr
  return r3;
loc_824457F4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_frag_824457CC:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

lifted::Result e82099BA0(uint8_t* base, const lifted::Args& a) {
  return {f82099BA0(base, a.r[0]), a.f[0]};
}

lifted::Result e82101D20(uint8_t* base, const lifted::Args& a) {
  f82101D20(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8210B174(uint8_t* base, const lifted::Args& a) {
  return {f8210B174(base), a.f[0]};
}

lifted::Result e8211A660(uint8_t* base, const lifted::Args& a) {
  f8211A660(base, a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82297340(uint8_t* base, const lifted::Args& a) {
  return {f82297340(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e822B07B0(uint8_t* base, const lifted::Args& a) {
  return f822B07B0(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e822B0860(uint8_t* base, const lifted::Args& a) {
  return f822B0860(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e82379320(uint8_t* base, const lifted::Args& a) {
  return {f82379320(base, a.r[0]), a.f[0]};
}

lifted::Result e824457CC(uint8_t* base, const lifted::Args& a) {
  return {f824457CC(base), a.f[0]};
}

lifted::Result e82488508(uint8_t* base, const lifted::Args& a) {
  return {f82488508(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82488548(uint8_t* base, const lifted::Args& a) {
  return {f82488548(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8248CA78(uint8_t* base, const lifted::Args& a) {
  return {f8248CA78(base, a.r[0]), a.f[0]};
}

lifted::Result e824F65C0(uint8_t* base, const lifted::Args& a) {
  return {f824F65C0(base), a.f[0]};
}

lifted::Result e825A9958(uint8_t* base, const lifted::Args& a) {
  return {f825A9958(base, a.r[0]), a.f[0]};
}

lifted::Result e82445798(uint8_t* base, const lifted::Args& a) {
  return {f82445798(base, a.r[0]), a.f[0]};
}

namespace {

const Registration kEntries[] = {
    {0x82099BA0, e82099BA0},
    {0x82101D20, e82101D20},
    {0x8210B174, e8210B174},
    {0x8211A660, e8211A660},
    {0x82297340, e82297340},
    {0x822B07B0, e822B07B0},
    {0x822B0860, e822B0860},
    {0x82379320, e82379320},
    {0x824457CC, e824457CC},
    {0x82488508, e82488508},
    {0x82488548, e82488548},
    {0x8248CA78, e8248CA78},
    {0x824F65C0, e824F65C0},
    {0x825A9958, e825A9958},
    {0x82445798, e82445798},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_035)), true);

}  // namespace

}  // namespace lifted
