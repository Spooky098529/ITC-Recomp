// Lift sweep batch 036 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/boot_mode.h"
#include "schema/car_physics.h"
#include "schema/career.h"
#include "schema/data_tables.h"
#include "schema/frame_driver.h"
#include "schema/mode_tick.h"
#include "schema/race_event.h"
#include "schema/race_hud_machine.h"
#include "schema/race_states.h"
#include "schema/std_string.h"
#include "schema/strings_and_font.h"
#include "schema/system_services.h"
#include "schema/tuning_pages.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies_game.h"
#include "schema/vehicle_accessors.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_036, true, "ITC",
              "Run lift sweep batch 036's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82091398: lifted from generated/ by lift.py.
lifted::Result f82091398(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82091398 (lifted)");
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r30,r5
  r30 = r5;
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).length_address());
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bge cr6,0x820913c0
  if (!cr6.lt) goto loc_820913C0;
  // bl 0x825a6b18
  // (left out: dead in C++)
  { const lifted::Result o = f825A6B18(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_820913C0:
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).length_address());
  // subf r9,r29,r11
  r9 = r11 - r29;
  // cmplw cr6,r9,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r30), so);
  // bge cr6,0x820913d4
  if (!cr6.lt) goto loc_820913D4;
  // mr r30,r9
  r30 = r9;
loc_820913D4:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82091448
  if (cr6.eq) goto loc_82091448;
  // lwz r8,24(r31)
  r8 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).capacity_address());
  // addi r28,r31,4
  r28 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplwi cr6,r8,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 16, so);
  // blt cr6,0x820913f4
  if (cr6.lt) goto loc_820913F4;
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r28) - 4).block_address());
  // b 0x820913f8
  goto loc_820913F8;
loc_820913F4:
  // mr r11,r28
  r11 = r28;
loc_820913F8:
  // cmplwi cr6,r8,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 16, so);
  // blt cr6,0x82091408
  if (cr6.lt) goto loc_82091408;
  // lwz r10,0(r28)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r28) - 4).block_address());
  // b 0x8209140c
  goto loc_8209140C;
loc_82091408:
  // mr r10,r28
  r10 = r28;
loc_8209140C:
  // add r11,r11,r29
  r11 = r11 + r29;
  // subf r6,r30,r9
  r6 = r9 - r30;
  // add r5,r11,r30
  r5 = r11 + r30;
  // subf r4,r29,r8
  r4 = r8 - r29;
  // add r3,r10,r29
  r3 = r10 + r29;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove_s(base, LIFTED_ARGS(0x82091424, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).length_address());
  // lwz r10,24(r31)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).capacity_address());
  // subf r11,r30,r11
  r11 = r11 - r30;
  // cmplwi cr6,r10,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 16, so);
  // blt cr6,0x8209143c
  if (cr6.lt) goto loc_8209143C;
  // lwz r28,0(r28)
  r28 = guest::LoadU32(base, schema::StdString(base, uint32_t(r28) - 4).block_address());
loc_8209143C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r11,20(r31)
  guest::StoreU32(base, schema::StdString(base, uint32_t(r31)).length_address(), uint32_t(r11));
  // stbx r10,r28,r11
  guest::StoreU8(base, uint32_t(r28) + uint32_t(r11), uint8_t(r10));
loc_82091448:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82091BF8: lifted from generated/ by lift.py.
uint64_t f82091BF8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82091BF8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::FrameDriverListenerMap(base, uint32_t(r3)).head_address());
  // lwz r11,4(r10)
  r11 = schema::FrameDriverMapNode(base, uint32_t(r10)).parent_bits();
  // lbz r9,21(r11)
  r9 = schema::FrameDriverMapNode(base, uint32_t(r11)).is_nil_bits();
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x82091c40
  if (!cr6.eq) goto loc_82091C40;
  // lwz r9,0(r4)
  r9 = guest::LoadU32(base, schema::FrameDriverListenerKey(base, uint32_t(r4)).key_address());
loc_82091C10:
  // lwz r8,12(r11)
  r8 = schema::FrameDriverMapNode(base, uint32_t(r11)).key_bits();
  // subfc r8,r9,r8
  ca = uint32_t(r8) >= uint32_t(r9);
  r8 = r8 - r9;
  // subfe r8,r8,r8
  temp.u8 = (~uint32_t(r8) + uint32_t(r8) < ~uint32_t(r8)) | (~uint32_t(r8) + uint32_t(r8) + ca < ca);
  r8 = ~r8 + r8 + ca;
  ca = temp.u8;
  // clrlwi. r8,r8,31
  r8 = uint32_t(r8) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x82091c2c
  if (cr0.eq) goto loc_82091C2C;
  // lwz r11,8(r11)
  r11 = schema::FrameDriverMapNode(base, uint32_t(r11)).right_bits();
  // b 0x82091c34
  goto loc_82091C34;
loc_82091C2C:
  // mr r10,r11
  r10 = r11;
  // lwz r11,0(r11)
  r11 = schema::FrameDriverMapNode(base, uint32_t(r11)).left_bits();
loc_82091C34:
  // lbz r8,21(r11)
  r8 = schema::FrameDriverMapNode(base, uint32_t(r11)).is_nil_bits();
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x82091c10
  if (cr6.eq) goto loc_82091C10;
loc_82091C40:
  // mr r3,r10
  r3 = r10;
  // blr
  return r3;
}

// sub_82091F58: lifted from generated/ by lift.py.
uint64_t f82091F58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82091F58 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r29,r3
  r29 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r4
  r31 = r4;
  // mr r30,r5
  r30 = r5;
  // bl 0x82099f50
  // (left out: dead in C++)
  r3 = f82099F50(base, r1, r13, r3, r4, r5);
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::FrameDriverListenerMap(base, uint32_t(r31)).head_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82091f90
  if (cr6.eq) goto loc_82091F90;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // beq cr6,0x82091f94
  if (cr6.eq) goto loc_82091F94;
loc_82091F90:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82091F94:
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // subf r9,r11,r10
  r9 = r10 - r11;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm. r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x82091fc8
  if (!cr0.eq) goto loc_82091FC8;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 12);
  // lwz r9,0(r30)
  r9 = guest::LoadU32(base, schema::FrameDriverListenerKey(base, uint32_t(r30)).key_address());
  // subfc r11,r11,r9
  ca = uint32_t(r9) >= uint32_t(r11);
  r11 = r9 - r11;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82091fc8
  if (!cr0.eq) goto loc_82091FC8;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // b 0x82091fd4
  goto loc_82091FD4;
loc_82091FC8:
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r31,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r31));
loc_82091FD4:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mr r3,r29
  r3 = r29;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r10,0(r29)
  guest::StoreU32(base, schema::FrameDriverMapIterator(base, uint32_t(r29)).map_address(), uint32_t(r10));
  // stw r11,4(r29)
  guest::StoreU32(base, schema::FrameDriverMapIterator(base, uint32_t(r29)).node_address(), uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82099F50: lifted from generated/ by lift.py.
uint64_t f82099F50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82099F50 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r4,r5
  r4 = r5;
  // mr r3,r30
  r3 = r30;
  // bl 0x82091bf8
  // (left out: dead in C++)
  r3 = f82091BF8(base, r3, r4);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // stw r3,4(r31)
  guest::StoreU32(base, schema::FrameDriverMapIterator(base, uint32_t(r31)).node_address(), uint32_t(r3));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::FrameDriverMapIterator(base, uint32_t(r31)).map_address(), uint32_t(r11));
  // bne cr6,0x82099f90
  if (!cr6.eq) goto loc_82099F90;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82099F90:
  // mr r3,r31
  r3 = r31;
  // stw r30,0(r31)
  guest::StoreU32(base, schema::FrameDriverMapIterator(base, uint32_t(r31)).map_address(), uint32_t(r30));
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

// sub_8209E4F0: lifted from generated/ by lift.py.
void f8209E4F0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8209E4F0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,-21668(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -21668);
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x8209e50c
  if (cr6.lt) goto loc_8209E50C;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bgelr cr6
  if (!cr6.lt) return;
  // b 0x8209e52c
  goto loc_8209E52C;
loc_8209E50C:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,31072(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 31072);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bnelr cr6
  if (!cr6.eq) return;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,31076(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 31076);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bnelr cr6
  if (!cr6.eq) return;
loc_8209E52C:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
  // blr
  return;
}

// sub_8209E540: lifted from generated/ by lift.py.
lifted::Result f8209E540(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8209E540 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r3,r4
  r3 = r4;
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r4,-21668(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + -21668);
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::SignInModeInterfaceVtable(base, uint32_t(r11)).interrupt_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209E56C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8209e5fc
  if (!cr6.eq) goto loc_8209E5FC;
  // lwz r11,-21668(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -21668);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8209e5c4
  if (cr6.lt) goto loc_8209E5C4;
  // beq cr6,0x8209e5bc
  if (cr6.eq) goto loc_8209E5BC;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x8209e5b4
  if (cr6.lt) goto loc_8209E5B4;
  // beq cr6,0x8209e5ac
  if (cr6.eq) goto loc_8209E5AC;
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // blt cr6,0x8209e5a4
  if (cr6.lt) goto loc_8209E5A4;
  // bne cr6,0x8209e5cc
  if (!cr6.eq) goto loc_8209E5CC;
  // li r3,7
  r3 = uint64_t(int64_t(7));
  // b 0x8209e5c8
  goto loc_8209E5C8;
loc_8209E5A4:
  // li r3,6
  r3 = uint64_t(int64_t(6));
  // b 0x8209e5c8
  goto loc_8209E5C8;
loc_8209E5AC:
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // b 0x8209e5c8
  goto loc_8209E5C8;
loc_8209E5B4:
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // b 0x8209e5c8
  goto loc_8209E5C8;
loc_8209E5BC:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x8209e5c8
  goto loc_8209E5C8;
loc_8209E5C4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8209E5C8:
  // bl 0x82291060
  // (left out: dead in C++)
  { const lifted::Result o = f82291060(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_8209E5CC:
  // lwz r11,-21668(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -21668);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x8209e5e8
  if (cr6.eq) goto loc_8209E5E8;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // bne cr6,0x8209e5ec
  if (!cr6.eq) goto loc_8209E5EC;
loc_8209E5E8:
  // li r11,3
  r11 = uint64_t(int64_t(3));
loc_8209E5EC:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209E5FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209E5FC:
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

// sub_8209E610: lifted from generated/ by lift.py.
lifted::Result f8209E610(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8209E610 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r4
  r31 = r4;
  // bl 0x82290ab8
  // (left out: dead in C++)
  r3 = f82290AB8(base);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8209e680
  if (!cr0.eq) goto loc_8209E680;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209E640, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,0(r31)
  r10 = lifted::VtableWord(base, uint32_t(r31));
  // mr r3,r31
  r3 = r31;
  // lwz r4,-21668(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + -21668);
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, schema::SignInModeInterfaceVtable(base, uint32_t(r10)).resume_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209E65C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // stw r11,24860(r10)
  guest::StoreU32(base, uint32_t(r10) + 24860, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,29444(r10)
  guest::StoreU32(base, uint32_t(r10) + 29444, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,-21644(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -21644);
  // stw r11,28180(r10)
  guest::StoreU32(base, uint32_t(r10) + 28180, uint32_t(r11));
loc_8209E680:
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

// sub_8209E698: lifted from generated/ by lift.py.
uint64_t f8209E698(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8209E698 (lifted)");
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
  // bl 0x82290ab8
  // (left out: dead in C++)
  r3 = f82290AB8(base);
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8209e71c
  if (!cr6.eq) goto loc_8209E71C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,7148(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 7148);
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x8209E6D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r4,29432(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 29432);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,-21596(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // beq cr6,0x8209e6f8
  if (cr6.eq) goto loc_8209E6F8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f6b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209F6B8, LIFTED_ARGS(0x8209E6F8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209E6F8:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,-21648(r10)
  guest::StoreU32(base, uint32_t(r10) + -21648, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,29444(r10)
  guest::StoreU32(base, uint32_t(r10) + 29444, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,24848(r10)
  guest::StoreU32(base, uint32_t(r10) + 24848, uint32_t(r11));
loc_8209E71C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8209E730: lifted from generated/ by lift.py.
lifted::Result f8209E730(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8209E730 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82260758
  // (left out: dead in C++)
  { const lifted::Result o = f82260758(base, r1, r13, r3, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8209e77c
  if (!cr6.eq) goto loc_8209E77C;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8209e768
  if (!cr6.eq) goto loc_8209E768;
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // b 0x8209e774
  goto loc_8209E774;
loc_8209E768:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x822910b0
  // (left out: dead in C++)
  { const lifted::Result o = f822910B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,6
  r11 = uint64_t(int64_t(6));
loc_8209E774:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
loc_8209E77C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8209E790: lifted from generated/ by lift.py.
lifted::Result f8209E790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8209E790 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // mr r3,r4
  r3 = r4;
  // addi r11,r11,20692
  r11 = uint64_t(int64_t(int64_t(r11) + 20692));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // bne cr6,0x8209e7d8
  if (!cr6.eq) goto loc_8209E7D8;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,0(r3)
  r10 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r4,-21668(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + -21668);
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, schema::SignInModeInterfaceVtable(base, uint32_t(r10)).force_exit_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209E7CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
loc_8209E7D8:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8209E860: lifted from generated/ by lift.py.
lifted::Result f8209E860(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_8209E860 (lifted)");
  uint64_t r3 = 0, r4 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209E874, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r10,-21596
  r10 = uint64_t(int64_t(int64_t(r10) + -21596));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r8,4(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 0);
  // rlwinm r7,r8,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // lwz r8,8(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 8);
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // rlwinm r8,r8,3,0,28
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // add r9,r9,r11
  r9 = r9 + r11;
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r31,r7,r11
  r31 = r7 + r11;
  // add r30,r8,r11
  r30 = r8 + r11;
  // add r29,r10,r11
  r29 = r10 + r11;
  // lwz r11,4(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209e8e0
  if (cr6.eq) goto loc_8209E8E0;
  // lwz r3,0(r9)
  r3 = guest::LoadU32(base, uint32_t(r9) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209E8D8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8209e964
  if (!cr0.eq) goto loc_8209E964;
loc_8209E8E0:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209e90c
  if (cr6.eq) goto loc_8209E90C;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209E904, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8209e964
  if (!cr0.eq) goto loc_8209E964;
loc_8209E90C:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209e938
  if (cr6.eq) goto loc_8209E938;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209E930, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8209e964
  if (!cr0.eq) goto loc_8209E964;
loc_8209E938:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209e998
  if (cr6.eq) goto loc_8209E998;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209E95C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8209e998
  if (cr0.eq) goto loc_8209E998;
loc_8209E964:
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,24856(r10)
  guest::StoreU32(base, uint32_t(r10) + 24856, uint32_t(r11));
  // lis r10,-32127
  r10 = uint64_t(int64_t(-2105475072));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,24860(r10)
  guest::StoreU32(base, uint32_t(r10) + 24860, uint32_t(r11));
  // bl 0x822603d8
  // (left out: dead in C++)
  { const lifted::Result o = f822603D8(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x822910b0
  // (left out: dead in C++)
  { const lifted::Result o = f822910B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
loc_8209E998:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209E9A0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8209E9A8: lifted from generated/ by lift.py.
lifted::Result f8209E9A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8209E9A8 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,-21668(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -21668);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // blt cr6,0x8209eb88
  if (cr6.lt) goto loc_8209EB88;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x8209eb04
  if (cr6.lt) goto loc_8209EB04;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bge cr6,0x8209ecbc
  if (!cr6.lt) goto loc_8209ECBC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209E9DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r10,-21596
  r10 = uint64_t(int64_t(int64_t(r10) + -21596));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r8,4(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 0);
  // rlwinm r7,r8,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // lwz r8,8(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 8);
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // rlwinm r8,r8,3,0,28
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // add r9,r9,r11
  r9 = r9 + r11;
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r31,r7,r11
  r31 = r7 + r11;
  // add r30,r8,r11
  r30 = r8 + r11;
  // add r29,r10,r11
  r29 = r10 + r11;
  // lwz r11,4(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209ea48
  if (cr6.eq) goto loc_8209EA48;
  // lwz r3,0(r9)
  r3 = guest::LoadU32(base, uint32_t(r9) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209EA40, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8209eacc
  if (!cr0.eq) goto loc_8209EACC;
loc_8209EA48:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209ea74
  if (cr6.eq) goto loc_8209EA74;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209EA6C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8209eacc
  if (!cr0.eq) goto loc_8209EACC;
loc_8209EA74:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209eaa0
  if (cr6.eq) goto loc_8209EAA0;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209EA98, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8209eacc
  if (!cr0.eq) goto loc_8209EACC;
loc_8209EAA0:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209eaf4
  if (cr6.eq) goto loc_8209EAF4;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209EAC4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8209eaf4
  if (cr0.eq) goto loc_8209EAF4;
loc_8209EACC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,7148(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 7148);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x8209EAE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
loc_8209EAF4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209EAFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x8209ecb4
  goto loc_8209ECB4;
loc_8209EB04:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // beq cr6,0x8209ecbc
  if (cr6.eq) goto loc_8209ECBC;
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8209EB1C);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8209eb70
  if (cr0.lt) goto loc_8209EB70;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stb r11,27404(r10)
  guest::StoreU8(base, uint32_t(r10) + 27404, uint8_t(r11));
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8209EB34, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r3,29420(r11)
  guest::StoreU32(base, uint32_t(r11) + 29420, uint32_t(r3));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x822601f0
  // (left out: dead in C++)
  r3 = f822601F0(base, r1, r13, r3);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28180);
  // stw r11,-21644(r10)
  guest::StoreU32(base, uint32_t(r10) + -21644, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // stw r10,88(r11)
  guest::StoreU32(base, uint32_t(r11) + 88, uint32_t(r10));
  // bl 0x8214ba60
  // (left out: dead in C++)
  { const lifted::Result o = f8214BA60(base, r1, r13, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // b 0x8209eb7c
  goto loc_8209EB7C;
loc_8209EB70:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x822910b0
  // (left out: dead in C++)
  { const lifted::Result o = f822910B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,6
  r11 = uint64_t(int64_t(6));
loc_8209EB7C:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
  // b 0x8209ecbc
  goto loc_8209ECBC;
loc_8209EB88:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209EB90, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r10,-21596
  r10 = uint64_t(int64_t(int64_t(r10) + -21596));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r8,4(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 0);
  // rlwinm r7,r8,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // lwz r8,8(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 8);
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // lwz r10,12(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 12);
  // rlwinm r8,r8,3,0,28
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // add r9,r9,r11
  r9 = r9 + r11;
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r31,r7,r11
  r31 = r7 + r11;
  // add r30,r8,r11
  r30 = r8 + r11;
  // add r29,r10,r11
  r29 = r10 + r11;
  // lwz r11,4(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209ebfc
  if (cr6.eq) goto loc_8209EBFC;
  // lwz r3,0(r9)
  r3 = guest::LoadU32(base, uint32_t(r9) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209EBF4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8209ec80
  if (!cr0.eq) goto loc_8209EC80;
loc_8209EBFC:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209ec28
  if (cr6.eq) goto loc_8209EC28;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209EC20, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8209ec80
  if (!cr0.eq) goto loc_8209EC80;
loc_8209EC28:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209ec54
  if (cr6.eq) goto loc_8209EC54;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209EC4C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8209ec80
  if (!cr0.eq) goto loc_8209EC80;
loc_8209EC54:
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8209eca8
  if (cr6.eq) goto loc_8209ECA8;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209EC78, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8209eca8
  if (cr0.eq) goto loc_8209ECA8;
loc_8209EC80:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,7148(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 7148);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeOut(base, LIFTED_ARGS(0x8209EC9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
loc_8209ECA8:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209ECB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_8209ECB4:
  // lis r10,-32157
  r10 = uint64_t(int64_t(-2107441152));
  // stw r11,25836(r10)
  guest::StoreU32(base, uint32_t(r10) + 25836, uint32_t(r11));
loc_8209ECBC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8209ECC8: lifted from generated/ by lift.py.
lifted::Result f8209ECC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8209ECC8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // bl 0x82290ab8
  // (left out: dead in C++)
  r3 = f82290AB8(base);
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8209ed48
  if (!cr6.eq) goto loc_8209ED48;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209ECE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8209ed40
  if (cr6.eq) goto loc_8209ED40;
  // lwz r3,0(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209ED24, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8209ed40
  if (cr0.eq) goto loc_8209ED40;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x822910b0
  // (left out: dead in C++)
  { const lifted::Result o = f822910B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,-21700(r10)
  guest::StoreU32(base, uint32_t(r10) + -21700, uint32_t(r11));
loc_8209ED40:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x8209f318
  // (left out: dead in C++)
  { const lifted::Result o = game::ActionMappers::SetBlocked(base, LIFTED_ARGS(0x8209ED48, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209ED48:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820A6E60: lifted from generated/ by lift.py.
lifted::Result f820A6E60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A6E60 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // mr r31,r3
  r31 = r3;
  // mr r30,r6
  r30 = r6;
  // lwz r11,20(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).length_address());
  // cmplw cr6,r11,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r28), so);
  // bge cr6,0x820a6e8c
  if (!cr6.lt) goto loc_820A6E8C;
  // bl 0x825a6b18
  // (left out: dead in C++)
  { const lifted::Result o = f825A6B18(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_820A6E8C:
  // lwz r11,20(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).length_address());
  // subf r11,r28,r11
  r11 = r11 - r28;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // bge cr6,0x820a6ea0
  if (!cr6.lt) goto loc_820A6EA0;
  // mr r30,r11
  r30 = r11;
loc_820A6EA0:
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).length_address());
  // subfic r10,r11,-1
  ca = uint32_t(r11) <= 4294967295;
  r10 = static_cast<uint64_t>(-1) - r11;
  // cmplw cr6,r10,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r30), so);
  // ble cr6,0x820a6ebc
  if (!cr6.gt) goto loc_820A6EBC;
  // add r10,r11,r30
  r10 = r11 + r30;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x820a6ec0
  if (!cr6.lt) goto loc_820A6EC0;
loc_820A6EBC:
  // bl 0x825a6a20
  // (left out: dead in C++)
  { const lifted::Result o = f825A6A20(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_820A6EC0:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x820a6f50
  if (cr6.eq) goto loc_820A6F50;
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).length_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // add r27,r11,r30
  r27 = r11 + r30;
  // mr r4,r27
  r4 = r27;
  // bl 0x82091540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091540, LIFTED_ARGS(0x820A6EE0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a6f50
  if (cr0.eq) goto loc_820A6F50;
  // lwz r11,24(r29)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x820a6efc
  if (cr6.lt) goto loc_820A6EFC;
  // lwz r8,4(r29)
  r8 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29)).block_address());
  // b 0x820a6f00
  goto loc_820A6F00;
loc_820A6EFC:
  // addi r8,r29,4
  r8 = uint64_t(int64_t(int64_t(r29) + 4));
loc_820A6F00:
  // lwz r9,24(r31)
  r9 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).capacity_address());
  // addi r29,r31,4
  r29 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplwi cr6,r9,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 16, so);
  // blt cr6,0x820a6f18
  if (cr6.lt) goto loc_820A6F18;
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
  // b 0x820a6f1c
  goto loc_820A6F1C;
loc_820A6F18:
  // mr r10,r29
  r10 = r29;
loc_820A6F1C:
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).length_address());
  // mr r6,r30
  r6 = r30;
  // add r5,r8,r28
  r5 = r8 + r28;
  // subf r4,r11,r9
  r4 = r9 - r11;
  // add r3,r11,r10
  r3 = r11 + r10;
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x820A6F34, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r31)
  r11 = guest::LoadU32(base, schema::StdString(base, uint32_t(r31)).capacity_address());
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x820a6f44
  if (cr6.lt) goto loc_820A6F44;
  // lwz r29,0(r29)
  r29 = guest::LoadU32(base, schema::StdString(base, uint32_t(r29) - 4).block_address());
loc_820A6F44:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r27,20(r31)
  guest::StoreU32(base, schema::StdString(base, uint32_t(r31)).length_address(), uint32_t(r27));
  // stbx r11,r29,r27
  guest::StoreU8(base, uint32_t(r29) + uint32_t(r27), uint8_t(r11));
loc_820A6F50:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820CC480: lifted from generated/ by lift.py.
uint64_t f820CC480(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820CC480 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,24(r3)
  r11 = guest::LoadU32(base, schema::CareerCarPart480(base, uint32_t(r3)).unknown_24_address());
  // cmpwi cr6,r11,61
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 61, so);
  // blt cr6,0x820cc498
  if (cr6.lt) goto loc_820CC498;
  // cmpwi cr6,r11,199
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 199, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bltlr cr6
  if (cr6.lt) return r3;
loc_820CC498:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_820CC6A0: lifted from generated/ by lift.py.
uint64_t f820CC6A0(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820CC6A0 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, ctr = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 7, so);
  // bgtlr cr6
  if (cr6.gt) return r3;
  // lis r12,-32256
  r12 = uint64_t(int64_t(-2113929216));
  // addi r12,r12,13000
  r12 = uint64_t(int64_t(int64_t(r12) + 13000));
  // lbzx r0,r12,r4
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r4));
  // lis r12,-32243
  r12 = uint64_t(int64_t(-2113077248));
  // addi r12,r12,-14640
  r12 = uint64_t(int64_t(int64_t(r12) + -14640));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r4)) {
  case 0:
  goto loc_820CC6D0;
  case 1:
  goto loc_820CC6E4;
  case 2:
  goto loc_820CC6F8;
  case 3:
  goto loc_820CC70C;
  case 4:
  goto loc_820CC720;
  case 5:
  goto loc_820CC734;
  case 6:
  goto loc_820CC748;
  case 7:
  goto loc_820CC75C;
  default:
  std::abort();
  }
loc_820CC6D0:
  // cmpwi cr6,r5,5
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 5, so);
  // blt cr6,0x820cc6dc
  if (cr6.lt) goto loc_820CC6DC;
  // li r5,4
  r5 = uint64_t(int64_t(4));
loc_820CC6DC:
  // rlwinm r11,r5,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 5) & 0xFFFFFFE0;
  // b 0x820cc770
  goto loc_820CC770;
loc_820CC6E4:
  // cmpwi cr6,r5,5
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 5, so);
  // blt cr6,0x820cc6f0
  if (cr6.lt) goto loc_820CC6F0;
  // li r5,4
  r5 = uint64_t(int64_t(4));
loc_820CC6F0:
  // addi r11,r5,5
  r11 = uint64_t(int64_t(int64_t(r5) + 5));
  // b 0x820cc76c
  goto loc_820CC76C;
loc_820CC6F8:
  // cmpwi cr6,r5,5
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 5, so);
  // blt cr6,0x820cc704
  if (cr6.lt) goto loc_820CC704;
  // li r5,4
  r5 = uint64_t(int64_t(4));
loc_820CC704:
  // addi r11,r5,10
  r11 = uint64_t(int64_t(int64_t(r5) + 10));
  // b 0x820cc76c
  goto loc_820CC76C;
loc_820CC70C:
  // cmpwi cr6,r5,5
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 5, so);
  // blt cr6,0x820cc718
  if (cr6.lt) goto loc_820CC718;
  // li r5,4
  r5 = uint64_t(int64_t(4));
loc_820CC718:
  // addi r11,r5,15
  r11 = uint64_t(int64_t(int64_t(r5) + 15));
  // b 0x820cc76c
  goto loc_820CC76C;
loc_820CC720:
  // cmpwi cr6,r5,5
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 5, so);
  // blt cr6,0x820cc72c
  if (cr6.lt) goto loc_820CC72C;
  // li r5,4
  r5 = uint64_t(int64_t(4));
loc_820CC72C:
  // addi r11,r5,20
  r11 = uint64_t(int64_t(int64_t(r5) + 20));
  // b 0x820cc76c
  goto loc_820CC76C;
loc_820CC734:
  // cmpwi cr6,r5,5
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 5, so);
  // blt cr6,0x820cc740
  if (cr6.lt) goto loc_820CC740;
  // li r5,4
  r5 = uint64_t(int64_t(4));
loc_820CC740:
  // addi r11,r5,25
  r11 = uint64_t(int64_t(int64_t(r5) + 25));
  // b 0x820cc76c
  goto loc_820CC76C;
loc_820CC748:
  // cmpwi cr6,r5,5
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 5, so);
  // blt cr6,0x820cc754
  if (cr6.lt) goto loc_820CC754;
  // li r5,4
  r5 = uint64_t(int64_t(4));
loc_820CC754:
  // addi r11,r5,30
  r11 = uint64_t(int64_t(int64_t(r5) + 30));
  // b 0x820cc76c
  goto loc_820CC76C;
loc_820CC75C:
  // cmpwi cr6,r5,5
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 5, so);
  // blt cr6,0x820cc768
  if (cr6.lt) goto loc_820CC768;
  // li r5,4
  r5 = uint64_t(int64_t(4));
loc_820CC768:
  // addi r11,r5,35
  r11 = uint64_t(int64_t(int64_t(r5) + 35));
loc_820CC76C:
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
loc_820CC770:
  // add r3,r11,r3
  r3 = r11 + r3;
  // blr
  return r3;
}

// sub_820CC778: lifted from generated/ by lift.py.
uint64_t f820CC778(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820CC778 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, ctr = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // mr r9,r3
  r9 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r11,138
  r11 = uint64_t(int64_t(138));
  // mtctr r11
  ctr = r11;
loc_820CC790:
  // stw r8,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r8));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x820cc790
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_820CC790;
  // cmpwi cr6,r4,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), -1, so);
  // bne cr6,0x820cc96c
  if (!cr6.eq) goto loc_820CC96C;
  // addi r10,r9,24
  r10 = uint64_t(int64_t(int64_t(r9) + 24));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r7,1
  r7 = uint64_t(int64_t(1));
loc_820CC7B0:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::CareerCarParts778(base, uint32_t(r10) - 24).unknown_24_address());
  // cmpwi cr6,r11,61
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 61, so);
  // blt cr6,0x820cc7d4
  if (cr6.lt) goto loc_820CC7D4;
  // cmpwi cr6,r11,199
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 199, so);
  // bge cr6,0x820cc7d4
  if (!cr6.lt) goto loc_820CC7D4;
  // addi r11,r11,-61
  r11 = uint64_t(int64_t(int64_t(r11) + -61));
  // addi r6,r1,-560
  r6 = uint64_t(int64_t(int64_t(r1) + -560));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r6
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r6), uint32_t(r7));
loc_820CC7D4:
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // addi r10,r10,32
  r10 = uint64_t(int64_t(int64_t(r10) + 32));
  // bne 0x820cc7b0
  if (!cr0.eq) goto loc_820CC7B0;
  // addi r11,r9,184
  r11 = uint64_t(int64_t(int64_t(r9) + 184));
  // li r8,5
  r8 = uint64_t(int64_t(5));
loc_820CC7E8:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::CareerCarParts778(base, uint32_t(r11) - 184).unknown_184_address());
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cc80c
  if (cr6.lt) goto loc_820CC80C;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cc80c
  if (!cr6.lt) goto loc_820CC80C;
  // addi r10,r10,-61
  r10 = uint64_t(int64_t(int64_t(r10) + -61));
  // addi r6,r1,-560
  r6 = uint64_t(int64_t(int64_t(r1) + -560));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r10,r6
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r6), uint32_t(r7));
loc_820CC80C:
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // bne 0x820cc7e8
  if (!cr0.eq) goto loc_820CC7E8;
  // addi r11,r9,344
  r11 = uint64_t(int64_t(int64_t(r9) + 344));
  // li r8,5
  r8 = uint64_t(int64_t(5));
loc_820CC820:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::CareerCarParts778(base, uint32_t(r11) - 344).unknown_344_address());
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cc844
  if (cr6.lt) goto loc_820CC844;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cc844
  if (!cr6.lt) goto loc_820CC844;
  // addi r10,r10,-61
  r10 = uint64_t(int64_t(int64_t(r10) + -61));
  // addi r6,r1,-560
  r6 = uint64_t(int64_t(int64_t(r1) + -560));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r10,r6
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r6), uint32_t(r7));
loc_820CC844:
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // bne 0x820cc820
  if (!cr0.eq) goto loc_820CC820;
  // addi r11,r9,504
  r11 = uint64_t(int64_t(int64_t(r9) + 504));
  // li r8,5
  r8 = uint64_t(int64_t(5));
loc_820CC858:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::CareerCarParts778(base, uint32_t(r11) - 504).unknown_504_address());
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cc87c
  if (cr6.lt) goto loc_820CC87C;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cc87c
  if (!cr6.lt) goto loc_820CC87C;
  // addi r10,r10,-61
  r10 = uint64_t(int64_t(int64_t(r10) + -61));
  // addi r6,r1,-560
  r6 = uint64_t(int64_t(int64_t(r1) + -560));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r10,r6
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r6), uint32_t(r7));
loc_820CC87C:
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // bne 0x820cc858
  if (!cr0.eq) goto loc_820CC858;
  // addi r11,r9,664
  r11 = uint64_t(int64_t(int64_t(r9) + 664));
  // li r8,5
  r8 = uint64_t(int64_t(5));
loc_820CC890:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::CareerCarParts778(base, uint32_t(r11) - 664).unknown_664_address());
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cc8b4
  if (cr6.lt) goto loc_820CC8B4;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cc8b4
  if (!cr6.lt) goto loc_820CC8B4;
  // addi r10,r10,-61
  r10 = uint64_t(int64_t(int64_t(r10) + -61));
  // addi r6,r1,-560
  r6 = uint64_t(int64_t(int64_t(r1) + -560));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r10,r6
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r6), uint32_t(r7));
loc_820CC8B4:
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // bne 0x820cc890
  if (!cr0.eq) goto loc_820CC890;
  // addi r11,r9,824
  r11 = uint64_t(int64_t(int64_t(r9) + 824));
  // li r8,5
  r8 = uint64_t(int64_t(5));
loc_820CC8C8:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::CareerCarParts778(base, uint32_t(r11) - 824).unknown_824_address());
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cc8ec
  if (cr6.lt) goto loc_820CC8EC;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cc8ec
  if (!cr6.lt) goto loc_820CC8EC;
  // addi r10,r10,-61
  r10 = uint64_t(int64_t(int64_t(r10) + -61));
  // addi r6,r1,-560
  r6 = uint64_t(int64_t(int64_t(r1) + -560));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r10,r6
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r6), uint32_t(r7));
loc_820CC8EC:
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // bne 0x820cc8c8
  if (!cr0.eq) goto loc_820CC8C8;
  // addi r11,r9,984
  r11 = uint64_t(int64_t(int64_t(r9) + 984));
  // li r8,5
  r8 = uint64_t(int64_t(5));
loc_820CC900:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::CareerCarParts778(base, uint32_t(r11) - 984).unknown_984_address());
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cc924
  if (cr6.lt) goto loc_820CC924;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cc924
  if (!cr6.lt) goto loc_820CC924;
  // addi r10,r10,-61
  r10 = uint64_t(int64_t(int64_t(r10) + -61));
  // addi r6,r1,-560
  r6 = uint64_t(int64_t(int64_t(r1) + -560));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r10,r6
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r6), uint32_t(r7));
loc_820CC924:
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // bne 0x820cc900
  if (!cr0.eq) goto loc_820CC900;
  // addi r11,r9,1144
  r11 = uint64_t(int64_t(int64_t(r9) + 1144));
  // li r9,5
  r9 = uint64_t(int64_t(5));
loc_820CC938:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::CareerCarParts778(base, uint32_t(r11) - 1144).unknown_1144_address());
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cc95c
  if (cr6.lt) goto loc_820CC95C;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cc95c
  if (!cr6.lt) goto loc_820CC95C;
  // addi r10,r10,-61
  r10 = uint64_t(int64_t(int64_t(r10) + -61));
  // addi r8,r1,-560
  r8 = uint64_t(int64_t(int64_t(r1) + -560));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r10,r8
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r8), uint32_t(r7));
loc_820CC95C:
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // bne 0x820cc938
  if (!cr0.eq) goto loc_820CC938;
  // b 0x820ccbc4
  goto loc_820CCBC4;
loc_820CC96C:
  // cmpwi cr6,r4,5
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 5, so);
  // mr r11,r4
  r11 = r4;
  // blt cr6,0x820cc97c
  if (cr6.lt) goto loc_820CC97C;
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_820CC97C:
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r10,24(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 24);
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cc9b0
  if (cr6.lt) goto loc_820CC9B0;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cc9b0
  if (!cr6.lt) goto loc_820CC9B0;
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // addi r11,r11,-61
  r11 = uint64_t(int64_t(int64_t(r11) + -61));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r7));
loc_820CC9B0:
  // cmpwi cr6,r4,5
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 5, so);
  // mr r11,r4
  r11 = r4;
  // blt cr6,0x820cc9c0
  if (cr6.lt) goto loc_820CC9C0;
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_820CC9C0:
  // addi r10,r11,5
  r10 = uint64_t(int64_t(int64_t(r11) + 5));
  // rlwinm r10,r10,5,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24);
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cc9fc
  if (cr6.lt) goto loc_820CC9FC;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cc9fc
  if (!cr6.lt) goto loc_820CC9FC;
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r11,184(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 184);
  // addi r11,r11,-61
  r11 = uint64_t(int64_t(int64_t(r11) + -61));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r7));
loc_820CC9FC:
  // cmpwi cr6,r4,5
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 5, so);
  // mr r11,r4
  r11 = r4;
  // blt cr6,0x820cca0c
  if (cr6.lt) goto loc_820CCA0C;
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_820CCA0C:
  // addi r10,r11,10
  r10 = uint64_t(int64_t(int64_t(r11) + 10));
  // rlwinm r10,r10,5,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24);
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cca48
  if (cr6.lt) goto loc_820CCA48;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cca48
  if (!cr6.lt) goto loc_820CCA48;
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r11,344(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 344);
  // addi r11,r11,-61
  r11 = uint64_t(int64_t(int64_t(r11) + -61));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r7));
loc_820CCA48:
  // cmpwi cr6,r4,5
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 5, so);
  // mr r11,r4
  r11 = r4;
  // blt cr6,0x820cca58
  if (cr6.lt) goto loc_820CCA58;
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_820CCA58:
  // addi r10,r11,15
  r10 = uint64_t(int64_t(int64_t(r11) + 15));
  // rlwinm r10,r10,5,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24);
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820cca94
  if (cr6.lt) goto loc_820CCA94;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820cca94
  if (!cr6.lt) goto loc_820CCA94;
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r11,504(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 504);
  // addi r11,r11,-61
  r11 = uint64_t(int64_t(int64_t(r11) + -61));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r7));
loc_820CCA94:
  // cmpwi cr6,r4,5
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 5, so);
  // mr r11,r4
  r11 = r4;
  // blt cr6,0x820ccaa4
  if (cr6.lt) goto loc_820CCAA4;
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_820CCAA4:
  // addi r10,r11,20
  r10 = uint64_t(int64_t(int64_t(r11) + 20));
  // rlwinm r10,r10,5,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24);
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820ccae0
  if (cr6.lt) goto loc_820CCAE0;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820ccae0
  if (!cr6.lt) goto loc_820CCAE0;
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r11,664(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 664);
  // addi r11,r11,-61
  r11 = uint64_t(int64_t(int64_t(r11) + -61));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r7));
loc_820CCAE0:
  // cmpwi cr6,r4,5
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 5, so);
  // mr r11,r4
  r11 = r4;
  // blt cr6,0x820ccaf0
  if (cr6.lt) goto loc_820CCAF0;
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_820CCAF0:
  // addi r10,r11,25
  r10 = uint64_t(int64_t(int64_t(r11) + 25));
  // rlwinm r10,r10,5,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24);
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820ccb2c
  if (cr6.lt) goto loc_820CCB2C;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820ccb2c
  if (!cr6.lt) goto loc_820CCB2C;
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r11,824(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 824);
  // addi r11,r11,-61
  r11 = uint64_t(int64_t(int64_t(r11) + -61));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r7));
loc_820CCB2C:
  // cmpwi cr6,r4,5
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 5, so);
  // mr r11,r4
  r11 = r4;
  // blt cr6,0x820ccb3c
  if (cr6.lt) goto loc_820CCB3C;
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_820CCB3C:
  // addi r10,r11,30
  r10 = uint64_t(int64_t(int64_t(r11) + 30));
  // rlwinm r10,r10,5,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24);
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820ccb78
  if (cr6.lt) goto loc_820CCB78;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820ccb78
  if (!cr6.lt) goto loc_820CCB78;
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r11,984(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 984);
  // addi r11,r11,-61
  r11 = uint64_t(int64_t(int64_t(r11) + -61));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r7));
loc_820CCB78:
  // cmpwi cr6,r4,5
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 5, so);
  // mr r11,r4
  r11 = r4;
  // blt cr6,0x820ccb88
  if (cr6.lt) goto loc_820CCB88;
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_820CCB88:
  // addi r10,r11,35
  r10 = uint64_t(int64_t(int64_t(r11) + 35));
  // rlwinm r10,r10,5,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r10,24(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 24);
  // cmpwi cr6,r10,61
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 61, so);
  // blt cr6,0x820ccbc4
  if (cr6.lt) goto loc_820CCBC4;
  // cmpwi cr6,r10,199
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 199, so);
  // bge cr6,0x820ccbc4
  if (!cr6.lt) goto loc_820CCBC4;
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r11,1144(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 1144);
  // addi r11,r11,-61
  r11 = uint64_t(int64_t(int64_t(r11) + -61));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r7));
loc_820CCBC4:
  // addi r10,r1,-560
  r10 = uint64_t(int64_t(int64_t(r1) + -560));
  // li r11,138
  r11 = uint64_t(int64_t(138));
loc_820CCBCC:
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x820ccbdc
  if (cr6.eq) goto loc_820CCBDC;
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
loc_820CCBDC:
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bne 0x820ccbcc
  if (!cr0.eq) goto loc_820CCBCC;
  // blr
  return r3;
}

// sub_820F2CA8: lifted from generated/ by lift.py.
uint64_t f820F2CA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820F2CA8 (lifted)");
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
  // lha r31,92(r3)
  r31 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::OpponentItem(base, uint32_t(r3)).rival_0_address()))));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x820f2d0c
  if (cr6.eq) goto loc_820F2D0C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x820F2CD4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r4,r31
  r4 = r31;
  // addis r3,r30,2
  r3 = uint64_t(int64_t(int64_t(r30) + 131072));
  // addi r3,r3,-26120
  r3 = uint64_t(int64_t(int64_t(r3) + -26120));
  // bl 0x82116da8
  // (left out: dead in C++)
  { const lifted::Result o = game::ItemRecords::Item(base, LIFTED_ARGS(0x820F2CE8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,8746
  r11 = uint64_t(int64_t(int64_t(r31) + 8746));
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // lhz r11,2(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 2);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x820f2d08
  if (!cr6.eq) goto loc_820F2D08;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // b 0x820f2d0c
  goto loc_820F2D0C;
loc_820F2D08:
  // bl 0x820f2ca8
  // (left out: dead in C++)
  r3 = f820F2CA8(base, r1, r13, r3);
loc_820F2D0C:
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

// sub_821059D0: lifted from generated/ by lift.py.
uint64_t f821059D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_821059D0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r27,r4
  r27 = r4;
  // mr r22,r3
  r22 = r3;
  // addis r30,r27,5
  r30 = uint64_t(int64_t(int64_t(r27) + 327680));
  // mr r23,r5
  r23 = r5;
  // addi r30,r30,-24608
  r30 = uint64_t(int64_t(int64_t(r30) + -24608));
  // mr r21,r6
  r21 = r6;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::RacerRecordArray(base, uint32_t(r30) - 303072).count_address());
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r20,300
  r20 = uint64_t(int64_t(300));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x82105a78
  if (!cr0.gt) goto loc_82105A78;
loc_82105A1C:
  // mr r4,r31
  r4 = r31;
  // mr r3,r27
  r3 = r27;
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = game::RacerRecords::At(base, LIFTED_ARGS(0x82105A28, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3920(r3)
  r11 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r3)).outcome_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82105a78
  if (cr6.eq) goto loc_82105A78;
  // lwz r11,3924(r3)
  r11 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r3)).finish_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82105a48
  if (!cr6.eq) goto loc_82105A48;
  // addi r24,r24,1
  r24 = uint64_t(int64_t(int64_t(r24) + 1));
  // b 0x82105a54
  goto loc_82105A54;
loc_82105A48:
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82105a54
  if (!cr6.eq) goto loc_82105A54;
  // addi r25,r25,1
  r25 = uint64_t(int64_t(int64_t(r25) + 1));
loc_82105A54:
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::RacerRecordArray(base, uint32_t(r30) - 303072).count_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // lwz r9,788(r3)
  r9 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r3)).e0_sp_address());
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // lwz r10,1760(r3)
  r10 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r3)).e1_sp_address());
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // add r29,r9,r29
  r29 = r9 + r29;
  // add r28,r10,r28
  r28 = r10 + r28;
  // blt cr6,0x82105a1c
  if (cr6.lt) goto loc_82105A1C;
loc_82105A78:
  // cmpwi cr6,r23,0
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // mr r7,r24
  r7 = r24;
  // bne cr6,0x82105a8c
  if (!cr6.eq) goto loc_82105A8C;
  // mr r7,r25
  r7 = r25;
  // beq cr6,0x82105a94
  if (cr6.eq) goto loc_82105A94;
loc_82105A8C:
  // mulli r11,r29,100
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(100))));
  // b 0x82105a98
  goto loc_82105A98;
loc_82105A94:
  // mulli r11,r28,100
  r11 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(100))));
loc_82105A98:
  // li r10,12000
  r10 = uint64_t(int64_t(12000));
  // twllei r26,0
  if (int32_t(r26) == 0 || uint32_t(r26) < 0u) lifted::Trap(0);
  // divw r11,r11,r10
  r11 = uint32_t((int32_t(r10) && !(int32_t(r11) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r11) / int32_t(r10) : 0);
  // rotlwi r10,r11,1
  r10 = __builtin_rotateleft32(uint32_t(r11), 1);
  // divw r11,r11,r26
  r11 = uint32_t((int32_t(r26) && !(int32_t(r11) == INT32_MIN && int32_t(r26) == -1)) ? int32_t(r11) / int32_t(r26) : 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r11,100
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 100, so);
  // andc r10,r26,r10
  r10 = r26 & ~r10;
  // twlgei r10,-1
  if (int32_t(r10) == -1 || uint32_t(r10) > 4294967295u) lifted::Trap(0);
  // bne cr6,0x82105ac8
  if (!cr6.eq) goto loc_82105AC8;
  // li r8,150
  r8 = uint64_t(int64_t(150));
  // b 0x82105ae8
  goto loc_82105AE8;
loc_82105AC8:
  // cmpwi cr6,r11,80
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 80, so);
  // blt cr6,0x82105ad8
  if (cr6.lt) goto loc_82105AD8;
  // li r8,130
  r8 = uint64_t(int64_t(130));
  // b 0x82105ae8
  goto loc_82105AE8;
loc_82105AD8:
  // cmpwi cr6,r11,60
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 60, so);
  // li r8,110
  r8 = uint64_t(int64_t(110));
  // bge cr6,0x82105ae8
  if (!cr6.lt) goto loc_82105AE8;
  // li r8,100
  r8 = uint64_t(int64_t(100));
loc_82105AE8:
  // cmpw cr6,r24,r25
  cr6 = lifted::Compare<int32_t>(int32_t(r24), int32_t(r25), so);
  // ble cr6,0x82105afc
  if (!cr6.gt) goto loc_82105AFC;
  // cmpwi cr6,r23,0
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // bne cr6,0x82105b08
  if (!cr6.eq) goto loc_82105B08;
  // b 0x82105b04
  goto loc_82105B04;
loc_82105AFC:
  // cmpwi cr6,r23,0
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // beq cr6,0x82105b08
  if (cr6.eq) goto loc_82105B08;
loc_82105B04:
  // li r20,0
  r20 = uint64_t(int64_t(0));
loc_82105B08:
  // lwz r9,0(r22)
  r9 = guest::LoadU32(base, schema::DayBlockAt129104(base, uint32_t(r22)).unknown_0_address());
  // subf. r11,r21,r9
  r11 = r9 - r21;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge 0x82105b54
  if (!cr0.lt) goto loc_82105B54;
  // cmpwi cr6,r11,-10000
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -10000, so);
  // bgt cr6,0x82105b24
  if (cr6.gt) goto loc_82105B24;
  // li r11,200
  r11 = uint64_t(int64_t(200));
  // b 0x82105b94
  goto loc_82105B94;
loc_82105B24:
  // cmpwi cr6,r11,-5000
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -5000, so);
  // bgt cr6,0x82105b34
  if (cr6.gt) goto loc_82105B34;
  // li r11,150
  r11 = uint64_t(int64_t(150));
  // b 0x82105b94
  goto loc_82105B94;
loc_82105B34:
  // cmpwi cr6,r11,-3000
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -3000, so);
  // bgt cr6,0x82105b44
  if (cr6.gt) goto loc_82105B44;
  // li r11,130
  r11 = uint64_t(int64_t(130));
  // b 0x82105b94
  goto loc_82105B94;
loc_82105B44:
  // cmpwi cr6,r11,-1000
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1000, so);
  // bgt cr6,0x82105b90
  if (cr6.gt) goto loc_82105B90;
  // li r11,110
  r11 = uint64_t(int64_t(110));
  // b 0x82105b94
  goto loc_82105B94;
loc_82105B54:
  // cmpwi cr6,r11,10000
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10000, so);
  // blt cr6,0x82105b64
  if (cr6.lt) goto loc_82105B64;
  // li r11,70
  r11 = uint64_t(int64_t(70));
  // b 0x82105b94
  goto loc_82105B94;
loc_82105B64:
  // cmpwi cr6,r11,5000
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5000, so);
  // blt cr6,0x82105b74
  if (cr6.lt) goto loc_82105B74;
  // li r11,80
  r11 = uint64_t(int64_t(80));
  // b 0x82105b94
  goto loc_82105B94;
loc_82105B74:
  // cmpwi cr6,r11,3000
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3000, so);
  // blt cr6,0x82105b84
  if (cr6.lt) goto loc_82105B84;
  // li r11,90
  r11 = uint64_t(int64_t(90));
  // b 0x82105b94
  goto loc_82105B94;
loc_82105B84:
  // cmpwi cr6,r11,1000
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1000, so);
  // li r11,95
  r11 = uint64_t(int64_t(95));
  // bge cr6,0x82105b94
  if (!cr6.lt) goto loc_82105B94;
loc_82105B90:
  // li r11,100
  r11 = uint64_t(int64_t(100));
loc_82105B94:
  // mulli r10,r7,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(100))));
  // add r10,r10,r20
  r10 = r10 + r20;
  // mullw r11,r10,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // mullw r11,r11,r8
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r8))));
  // li r10,10000
  r10 = uint64_t(int64_t(10000));
  // divw r3,r11,r10
  r3 = uint32_t((int32_t(r10) && !(int32_t(r11) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r11) / int32_t(r10) : 0);
  // lwz r11,8(r22)
  r11 = guest::LoadU32(base, schema::DayBlockAt129104(base, uint32_t(r22)).unknown_8_address());
  // add r11,r26,r11
  r11 = r26 + r11;
  // stw r11,8(r22)
  guest::StoreU32(base, schema::DayBlockAt129104(base, uint32_t(r22)).unknown_8_address(), uint32_t(r11));
  // add r11,r9,r3
  r11 = r9 + r3;
  // stw r11,0(r22)
  guest::StoreU32(base, schema::DayBlockAt129104(base, uint32_t(r22)).unknown_0_address(), uint32_t(r11));
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return r3;
}

// sub_82106BB0 game::ModeType8Member::Dtor: lifted from generated/ by lift.py.
lifted::Result f82106BB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82106BB0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addis r3,r31,1
  r3 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r3,r3,-22796
  r3 = uint64_t(int64_t(int64_t(r3) + -22796));
  // bl 0x82238368
  // (left out: dead in C++)
  { const lifted::Result o = game::Type8PanelScreen::Dtor(base, LIFTED_ARGS(0x82106BD4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,30912
  r30 = uint64_t(int64_t(int64_t(r31) + 30912));
  // addi r3,r30,11040
  r3 = uint64_t(int64_t(int64_t(r30) + 11040));
  // bl 0x822531f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Type8ColorListPart::Dtor(base, LIFTED_ARGS(0x82106BE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82255198
  // (left out: dead in C++)
  { const lifted::Result o = game::ColorListScreen::Dtor(base, LIFTED_ARGS(0x82106BE8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,19544
  r30 = uint64_t(int64_t(int64_t(r31) + 19544));
  // addi r3,r30,10844
  r3 = uint64_t(int64_t(int64_t(r30) + 10844));
  // bl 0x822544e8
  // (left out: dead in C++)
  { const lifted::Result o = game::Type8ListPart::Dtor(base, LIFTED_ARGS(0x82106BF4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82257548
  // (left out: dead in C++)
  { const lifted::Result o = game::Type8ListBase::Dtor(base, LIFTED_ARGS(0x82106BFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8223bf00
  // (left out: dead in C++)
  { const lifted::Result o = game::Type8Screen0::Dtor(base, LIFTED_ARGS(0x82106C04, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821075E0 game::ModeType8Member::Ctor: lifted from generated/ by lift.py.
uint64_t f821075E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821075E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x8223d2f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Type8Screen0::Ctor(base, LIFTED_ARGS(0x821075F8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,9792
  r3 = uint64_t(int64_t(int64_t(r31) + 9792));
  // bl 0x82107358
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenMachine::Ctor(base, LIFTED_ARGS(0x82107600, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,19544
  r3 = uint64_t(int64_t(int64_t(r31) + 19544));
  // bl 0x8223f140
  // (left out: dead in C++)
  { const lifted::Result o = game::Type8ListScreen::Ctor(base, LIFTED_ARGS(0x82107608, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,30912
  r3 = uint64_t(int64_t(int64_t(r31) + 30912));
  // bl 0x8223dfc0
  // (left out: dead in C++)
  { const lifted::Result o = game::Type8ColorListScreen::Ctor(base, LIFTED_ARGS(0x82107610, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r3,r31,1
  r3 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r3,r3,-22796
  r3 = uint64_t(int64_t(int64_t(r3) + -22796));
  // bl 0x822386f0
  // (left out: dead in C++)
  { const lifted::Result o = game::Type8PanelScreen::Ctor(base, LIFTED_ARGS(0x8210761C, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r3,r31,1
  r3 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r3,r3,-7544
  r3 = uint64_t(int64_t(int64_t(r3) + -7544));
  // bl 0x82107358
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenMachine::Ctor(base, LIFTED_ARGS(0x82107628, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r3,r31,1
  r3 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r3,r3,2096
  r3 = uint64_t(int64_t(int64_t(r3) + 2096));
  // bl 0x82107358
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenMachine::Ctor(base, LIFTED_ARGS(0x82107634, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r3,r31,1
  r3 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r3,r3,11736
  r3 = uint64_t(int64_t(int64_t(r3) + 11736));
  // bl 0x82107358
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenMachine::Ctor(base, LIFTED_ARGS(0x82107640, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 65536));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,21376
  r11 = uint64_t(int64_t(int64_t(r11) + 21376));
  // mr r3,r31
  r3 = r31;
  // stw r10,4(r11)
  guest::StoreU32(base, schema::ModeType8Member(base, uint32_t(r11) - 86912).answer_next_type_address(), uint32_t(r10));
  // stb r10,0(r11)
  guest::StoreU8(base, schema::ModeType8Member(base, uint32_t(r11) - 86912).answer_changed_address(), uint8_t(r10));
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

// sub_82107670 game::ModeType8::Ctor: lifted from generated/ by lift.py.
uint64_t f82107670(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82107670 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // bl 0x82106c20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106C20, LIFTED_ARGS(0x82107688, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r9,r31,36
  r9 = uint64_t(int64_t(int64_t(r31) + 36));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r9,204
  r10 = uint64_t(int64_t(int64_t(r9) + 204));
  // li r8,49
  r8 = uint64_t(int64_t(49));
  // stw r11,200(r9)
  guest::StoreU32(base, schema::ModeType8(base, uint32_t(r9) - 36).handler_count_address(), uint32_t(r11));
  // addi r9,r10,8
  r9 = uint64_t(int64_t(int64_t(r10) + 8));
loc_821076A0:
  // stw r11,-8(r9)
  guest::StoreU32(base, schema::ModeType8(base, uint32_t(r9) - 248).ring_address(), uint32_t(r11));
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // stw r11,-4(r9)
  guest::StoreU32(base, schema::ModeType8(base, uint32_t(r9) - 248).ring_address() + 4, uint32_t(r11));
  // stw r11,0(r9)
  guest::StoreU32(base, schema::ModeType8(base, uint32_t(r9) - 248).ring_address() + 8, uint32_t(r11));
  // addi r9,r9,12
  r9 = uint64_t(int64_t(int64_t(r9) + 12));
  // bge 0x821076a0
  if (!cr0.lt) goto loc_821076A0;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // stw r11,600(r10)
  guest::StoreU32(base, schema::ModeType8(base, uint32_t(r10) - 240).ring_count_address(), uint32_t(r11));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // stw r11,604(r10)
  guest::StoreU32(base, schema::ModeType8(base, uint32_t(r10) - 240).ring_start_address(), uint32_t(r11));
  // addi r9,r9,28000
  r9 = uint64_t(int64_t(int64_t(r9) + 28000));
  // addi r8,r8,27988
  r8 = uint64_t(int64_t(int64_t(r8) + 27988));
  // addi r3,r31,848
  r3 = uint64_t(int64_t(int64_t(r31) + 848));
  // stw r9,0(r31)
  guest::StoreU32(base, schema::ModeType8(base, uint32_t(r31)).vtable_address(), uint32_t(r9));
  // stw r8,32(r31)
  guest::StoreU32(base, schema::ModeType8(base, uint32_t(r31)).vtable32_address(), uint32_t(r8));
  // bl 0x821075e0
  // (left out: dead in C++)
  r3 = game::ModeType8Member::Ctor(base, r1, r13, r3);
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

// sub_821076F8 game::ModeType8::Dtor: lifted from generated/ by lift.py.
lifted::Result f821076F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821076F8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,28000
  r11 = uint64_t(int64_t(int64_t(r11) + 28000));
  // addi r10,r10,27988
  r10 = uint64_t(int64_t(int64_t(r10) + 27988));
  // addi r3,r31,848
  r3 = uint64_t(int64_t(int64_t(r31) + 848));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,32(r31)
  guest::StoreU32(base, uint32_t(r31) + 32, uint32_t(r10));
  // bl 0x82106bb0
  // (left out: dead in C++)
  { const lifted::Result o = game::ModeType8Member::Dtor(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,27116
  r11 = uint64_t(int64_t(int64_t(r11) + 27116));
  // addi r10,r10,27148
  r10 = uint64_t(int64_t(int64_t(r10) + 27148));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,32(r31)
  guest::StoreU32(base, uint32_t(r31) + 32, uint32_t(r10));
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

// sub_82107758 game::ModeType8::DeletingDestructor: lifted from generated/ by lift.py.
lifted::Result f82107758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82107758 (lifted)");
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
  // bl 0x821076f8
  // (left out: dead in C++)
  { const lifted::Result o = game::ModeType8::Dtor(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82107788
  if (cr0.eq) goto loc_82107788;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82107788, r31)); r3 = o.r3; f1 = o.f1; }
loc_82107788:
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

// sub_821097F0: lifted from generated/ by lift.py.
uint64_t f821097F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821097F0 (lifted)");
  uint64_t r0 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v11{}, v12{}, v13{}, v127{};
  lifted::Temp temp{};
  lifted::Vec vTemp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-48(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -48, std::bit_cast<uint64_t>(f30));
  // stfd f31,-40(r1)
  guest::StoreU64(base, uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // li r12,-64
  r12 = uint64_t(int64_t(-64));
  // stvx128 v127,r1,r12
  ea = (uint32_t(r1) + uint32_t(r12)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v127.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stwu r1,-368(r1)
  ea = -368 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // vspltisw128 v127,0
  simde_mm_store_si128((simde__m128i*)v127.u32, simde_mm_set1_epi32(int(0x0)));
  // lwz r11,16(r3)
  r11 = schema::Font(base, uint32_t(r3)).params_bits();
  // addi r31,r3,16464
  r31 = uint64_t(int64_t(int64_t(r3) + 16464));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,64
  r5 = uint64_t(int64_t(64));
  // vupkd3d128 v0,v127,4
  temp.f32 = 3.0f;
  temp.s32 += v127.s16[1];
  vTemp.f32[3] = temp.f32;
  temp.f32 = 3.0f;
  temp.s32 += v127.s16[0];
  vTemp.f32[2] = temp.f32;
  vTemp.f32[1] = 0.0f;
  vTemp.f32[0] = 1.0f;
  v0 = vTemp;
  // mr r3,r31
  r3 = r31;
  // lfs f0,2608(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2608);
  f0 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stfs f0,188(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 188, temp.u32);
  // stfs f0,204(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 204, temp.u32);
  // stfs f0,220(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 220, temp.u32);
  // vpermwi128 v13,v0,234
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x15));
  // stfs f0,236(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 236, temp.u32);
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, schema::FontDrawParams(base, uint32_t(r11)).origin_x_address());
  // lfs f0,10568(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 10568);
  f0 = double(temp.f32);
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::FontDrawParams(base, uint32_t(r11)).x_address());
  // lfs f13,16(r11)
  temp.u32 = guest::LoadU32(base, schema::FontDrawParams(base, uint32_t(r11)).angle_address());
  f13 = double(temp.f32);
  // lwz r10,12(r11)
  r10 = guest::LoadU32(base, schema::FontDrawParams(base, uint32_t(r11)).origin_y_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::FontDrawParams(base, uint32_t(r11)).y_address());
  // vpermwi128 v12,v0,186
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x45));
  // vpermwi128 v11,v0,174
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x51));
  // fmuls f30,f13,f0
  f30 = double(float(f13 * f0));
  // add r29,r10,r11
  r29 = r10 + r11;
  // vpermwi128 v0,v0,171
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x54));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r30,r9,r8
  r30 = r9 + r8;
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v12,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // stvx v11,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8210989C, r31)); r3 = o.r3; f1 = o.f1; }
  // vupkd3d128 v0,v127,4
  temp.f32 = 3.0f;
  temp.s32 += v127.s16[1];
  vTemp.f32[3] = temp.f32;
  temp.f32 = 3.0f;
  temp.s32 += v127.s16[0];
  vTemp.f32[2] = temp.f32;
  vTemp.f32[1] = 0.0f;
  vTemp.f32[0] = 1.0f;
  v0 = vTemp;
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r9,r1,176
  r9 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // vpermwi128 v13,v0,234
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x15));
  // vpermwi128 v12,v0,186
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x45));
  // vpermwi128 v11,v0,174
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x51));
  // vpermwi128 v0,v0,171
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x54));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // mtctr r11
  ctr = r11;
loc_821098E0:
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 0);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // std r11,0(r9)
  guest::StoreU64(base, uint32_t(r9) + 0, r11);
  // addi r9,r9,8
  r9 = uint64_t(int64_t(int64_t(r9) + 8));
  // bdnz 0x821098e0
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_821098E0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f30,160(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  guest::StoreU32(base, uint32_t(r1) + 160, temp.u32);
  // neg r10,r30
  r10 = uint64_t(int64_t(static_cast<int64_t>(-r30)));
  // neg r9,r29
  r9 = uint64_t(int64_t(static_cast<int64_t>(-r29)));
  // addi r5,r1,160
  r5 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r4,r1,176
  r4 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfs f31,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // extsw r11,r10
  r11 = uint64_t(int64_t(int32_t(r10)));
  // extsw r10,r9
  r10 = uint64_t(int64_t(int32_t(r9)));
  // stfs f31,232(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 232, temp.u32);
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // std r11,152(r1)
  guest::StoreU64(base, uint32_t(r1) + 152, r11);
  // std r10,144(r1)
  guest::StoreU64(base, uint32_t(r1) + 144, r10);
  // lfd f0,152(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 152)));
  // lfd f13,144(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 144)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,224(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 224, temp.u32);
  // frsp f0,f13
  f0 = double(float(f13));
  // stfs f0,228(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 228, temp.u32);
  // bl 0x820a2d30
  // (left out: dead in C++)
  { const lifted::Result o = game::Matrix::RotateZ(base, LIFTED_ARGS(0x8210994C, r31)); r3 = o.r3; f1 = o.f1; }
  // vupkd3d128 v0,v127,4
  temp.f32 = 3.0f;
  temp.s32 += v127.s16[1];
  vTemp.f32[3] = temp.f32;
  temp.f32 = 3.0f;
  temp.s32 += v127.s16[0];
  vTemp.f32[2] = temp.f32;
  vTemp.f32[1] = 0.0f;
  vTemp.f32[0] = 1.0f;
  v0 = vTemp;
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r9,r1,240
  r9 = uint64_t(int64_t(int64_t(r1) + 240));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // vpermwi128 v13,v0,234
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x15));
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // vpermwi128 v12,v0,186
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x45));
  // vpermwi128 v11,v0,174
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x51));
  // vpermwi128 v0,v0,171
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x54));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // mtctr r11
  ctr = r11;
loc_82109990:
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 0);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // std r11,0(r9)
  guest::StoreU64(base, uint32_t(r9) + 0, r11);
  // addi r9,r9,8
  r9 = uint64_t(int64_t(int64_t(r9) + 8));
  // bdnz 0x82109990
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82109990;
  // extsw r11,r30
  r11 = uint64_t(int64_t(int32_t(r30)));
  // stfs f31,296(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 296, temp.u32);
  // extsw r10,r29
  r10 = uint64_t(int64_t(int32_t(r29)));
  // addi r5,r1,240
  r5 = uint64_t(int64_t(int64_t(r1) + 240));
  // addi r4,r1,176
  r4 = uint64_t(int64_t(int64_t(r1) + 176));
  // mr r3,r31
  r3 = r31;
  // std r11,144(r1)
  guest::StoreU64(base, uint32_t(r1) + 144, r11);
  // std r10,152(r1)
  guest::StoreU64(base, uint32_t(r1) + 152, r10);
  // lfd f0,144(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 144)));
  // lfd f13,152(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 152)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,288(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 288, temp.u32);
  // frsp f0,f13
  f0 = double(float(f13));
  // stfs f0,292(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 292, temp.u32);
  // bl 0x8209bd50
  // (left out: dead in C++)
  { const lifted::Result o = game::Matrix::Multiply(base, LIFTED_ARGS(0x821099E8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,368
  r1 = uint64_t(int64_t(int64_t(r1) + 368));
  // li r0,-64
  r0 = uint64_t(int64_t(-64));
  // lvx128 v127,r1,r0
  ea = (uint32_t(r1) + uint32_t(r0)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v127.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfd f30,-48(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -48)));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82109D80: lifted from generated/ by lift.py.
lifted::Result f82109D80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82109D80 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a9958
  // (left out: dead in C++)
  r3 = game::wcslen(base, r3);
  // lhz r11,0(r31)
  r11 = guest::LoadU16(base, schema::FontText(base, uint32_t(r31)).unit_address());
  // mr r28,r3
  r28 = r3;
  // mr r10,r31
  r10 = r31;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x82109dd4
  goto loc_82109DD4;
loc_82109DB0:
  // cmplwi cr6,r11,65489
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65489, so);
  // beq cr6,0x82109ddc
  if (cr6.eq) goto loc_82109DDC;
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // bne cr6,0x82109dcc
  if (!cr6.eq) goto loc_82109DCC;
  // clrlwi r11,r29,16
  r11 = uint32_t(r29) & 0xFFFF;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r29,r11,16
  r29 = uint32_t(r11) & 0xFFFF;
loc_82109DCC:
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // lhz r11,0(r10)
  r11 = guest::LoadU16(base, schema::FontText(base, uint32_t(r10) - 2).next_unit_address());
loc_82109DD4:
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82109db0
  if (!cr0.eq) goto loc_82109DB0;
loc_82109DDC:
  // lwz r11,16(r30)
  r11 = schema::Font(base, uint32_t(r30)).params_bits();
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // lfs f0,3140(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 3140);
  f0 = double(temp.f32);
  // lbz r9,92(r11)
  r9 = guest::LoadU8(base, schema::FontDrawParams(base, uint32_t(r11)).unknown_92_address());
  // lfs f13,104(r11)
  temp.u32 = guest::LoadU32(base, schema::FontDrawParams(base, uint32_t(r11)).unknown_104_address());
  f13 = double(temp.f32);
  // lbz r10,93(r11)
  r10 = guest::LoadU8(base, schema::FontDrawParams(base, uint32_t(r11)).unknown_93_address());
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lbz r11,94(r11)
  r11 = guest::LoadU8(base, schema::FontDrawParams(base, uint32_t(r11)).unknown_94_address());
  // fctidz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x8000000000000000ULL) : (f0 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f0)))));
  // stfd f0,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, std::bit_cast<uint64_t>(f0));
  // lbz r8,87(r1)
  r8 = guest::LoadU8(base, uint32_t(r1) + 87);
  // rotlwi r8,r8,8
  r8 = __builtin_rotateleft32(uint32_t(r8), 8);
  // or r9,r8,r9
  r9 = r8 | r9;
  // rlwinm r9,r9,8,0,23
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 8) & 0xFFFFFF00;
  // or r10,r9,r10
  r10 = r9 | r10;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r27,r10,r11
  r27 = r10 | r11;
  // bl 0x82108fe0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82108FE0, LIFTED_ARGS(0x82109E2C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x82108568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82108568, LIFTED_ARGS(0x82109E3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,16(r30)
  r11 = schema::Font(base, uint32_t(r30)).params_bits();
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f13,16(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::FontDrawParams(base, uint32_t(r11)).angle_address());
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f0,31864(r11)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r11) + 31864)));
  // fmul f0,f13,f0
  f0 = f13 * f0;
  // frsp f0,f0
  f0 = double(float(f0));
  // fctiwz f13,f0
  f13 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f13,0,r10
  guest::StoreU32(base, uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f13)));
  // lwa r11,80(r1)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r1) + 80))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // beq cr6,0x82109e80
  if (cr6.eq) goto loc_82109E80;
  // bl 0x821097f0
  // (left out: dead in C++)
  r3 = f821097F0(base, r1, r13, r3);
loc_82109E80:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x82109ee0
  if (cr6.eq) goto loc_82109EE0;
  // addi r29,r30,64
  r29 = uint64_t(int64_t(int64_t(r30) + 64));
loc_82109E8C:
  // lhz r6,0(r31)
  r6 = guest::LoadU16(base, schema::FontText(base, uint32_t(r31)).unit_address());
  // mr r11,r6
  r11 = r6;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // beq cr6,0x82109ed0
  if (cr6.eq) goto loc_82109ED0;
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // beq cr6,0x82109ed0
  if (cr6.eq) goto loc_82109ED0;
  // cmplwi cr6,r11,65494
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65494, so);
  // beq cr6,0x82109ed0
  if (cr6.eq) goto loc_82109ED0;
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // bgt cr6,0x82109ebc
  if (cr6.gt) goto loc_82109EBC;
  // cmplwi cr6,r11,65525
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65525, so);
  // bgt cr6,0x82109ed0
  if (cr6.gt) goto loc_82109ED0;
loc_82109EBC:
  // mr r7,r27
  r7 = r27;
  // lfs f2,4(r29)
  lifted::ScalarFloatMode();
  temp.u32 = uint32_t(schema::Font(base, uint32_t(r29) - 64).load_at(68, 4));
  f2 = double(temp.f32);
  // mr r3,r30
  r3 = r30;
  // lfs f1,0(r29)
  temp.u32 = uint32_t(schema::Font(base, uint32_t(r29) - 64).load_at(64, 4));
  f1 = double(temp.f32);
  // bl 0x82109560
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82109560, LIFTED_ARGS(0x82109ED0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82109ED0:
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r31,r31,2
  r31 = uint64_t(int64_t(int64_t(r31) + 2));
  // addi r29,r29,8
  r29 = uint64_t(int64_t(int64_t(r29) + 8));
  // bne 0x82109e8c
  if (!cr0.eq) goto loc_82109E8C;
loc_82109EE0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821105A8 game::Results::MeanWord0: lifted from generated/ by lift.py.
double f821105A8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821105A8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r9,0(r3)
  r9 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r3)).count_address());
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // fmr f0,f1
  f0 = f1;
  // ble 0x821105ec
  if (!cr0.gt) goto loc_821105EC;
  // addi r10,r3,4
  r10 = uint64_t(int64_t(int64_t(r3) + 4));
  // mr r11,r9
  r11 = r9;
loc_821105C8:
  // lwa r8,0(r10)
  r8 = uint64_t(int64_t(int32_t(guest::LoadU32(base, schema::CareerResults(base, uint32_t(r10) - 4).records_address()))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // std r8,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r8);
  // lfd f13,-16(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // bne 0x821105c8
  if (!cr0.eq) goto loc_821105C8;
loc_821105EC:
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beqlr cr6
  if (cr6.eq) return f1;
  // extsw r11,r9
  r11 = uint64_t(int64_t(int32_t(r9)));
  // std r11,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r11);
  // lfd f13,-16(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fdivs f1,f0,f13
  f1 = double(float(f0 / f13));
  // blr
  return f1;
}

// sub_82110610 game::Results::MeanWord4: lifted from generated/ by lift.py.
double f82110610(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82110610 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r9,0(r3)
  r9 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r3)).count_address());
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // fmr f0,f1
  f0 = f1;
  // ble 0x82110654
  if (!cr0.gt) goto loc_82110654;
  // addi r10,r3,8
  r10 = uint64_t(int64_t(int64_t(r3) + 8));
  // mr r11,r9
  r11 = r9;
loc_82110630:
  // lwa r8,0(r10)
  r8 = uint64_t(int64_t(int32_t(guest::LoadU32(base, schema::CareerResults(base, uint32_t(r10) - 8).records_address() + 4))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // std r8,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r8);
  // lfd f13,-16(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // bne 0x82110630
  if (!cr0.eq) goto loc_82110630;
loc_82110654:
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beqlr cr6
  if (cr6.eq) return f1;
  // extsw r11,r9
  r11 = uint64_t(int64_t(int32_t(r9)));
  // std r11,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r11);
  // lfd f13,-16(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fdivs f1,f0,f13
  f1 = double(float(f0 / f13));
  // blr
  return f1;
}

// sub_82110678 game::Results::MeanFloat8: lifted from generated/ by lift.py.
double f82110678(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82110678 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r9,0(r3)
  r9 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r3)).count_address());
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // fmr f0,f1
  f0 = f1;
  // ble 0x821106ac
  if (!cr0.gt) goto loc_821106AC;
  // addi r10,r3,12
  r10 = uint64_t(int64_t(int64_t(r3) + 12));
  // mr r11,r9
  r11 = r9;
loc_82110698:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r10) - 12).records_address() + 8);
  f13 = double(temp.f32);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // bne 0x82110698
  if (!cr0.eq) goto loc_82110698;
loc_821106AC:
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beqlr cr6
  if (cr6.eq) return f1;
  // extsw r11,r9
  r11 = uint64_t(int64_t(int32_t(r9)));
  // std r11,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r11);
  // lfd f13,-16(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fdivs f1,f0,f13
  f1 = double(float(f0 / f13));
  // blr
  return f1;
}

// sub_821106D0 game::Results::WonPercent: lifted from generated/ by lift.py.
double f821106D0(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821106D0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r3)).count_address());
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x82110704
  if (!cr0.gt) goto loc_82110704;
  // addi r9,r3,20
  r9 = uint64_t(int64_t(int64_t(r3) + 20));
  // mr r10,r11
  r10 = r11;
loc_821106E8:
  // lwz r7,0(r9)
  r7 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r9) - 20).records_address() + 16);
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // beq cr6,0x821106f8
  if (cr6.eq) goto loc_821106F8;
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
loc_821106F8:
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r9,r9,48
  r9 = uint64_t(int64_t(int64_t(r9) + 48));
  // bne 0x821106e8
  if (!cr0.eq) goto loc_821106E8;
loc_82110704:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82110740
  if (cr6.eq) goto loc_82110740;
  // mulli r10,r8,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(100))));
  // divw r9,r10,r11
  r9 = uint32_t((int32_t(r11) && !(int32_t(r10) == INT32_MIN && int32_t(r11) == -1)) ? int32_t(r10) / int32_t(r11) : 0);
  // rotlwi r10,r10,1
  r10 = __builtin_rotateleft32(uint32_t(r10), 1);
  // extsw r9,r9
  r9 = uint64_t(int64_t(int32_t(r9)));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // twllei r11,0
  if (int32_t(r11) == 0 || uint32_t(r11) < 0u) lifted::Trap(0);
  // andc r11,r11,r10
  r11 = r11 & ~r10;
  // std r9,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r9);
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
  // lfd f0,-16(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f1,f0
  f1 = double(float(f0));
  // blr
  return f1;
loc_82110740:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82110750 game::Results::AllWord40Are: lifted from generated/ by lift.py.
uint64_t f82110750(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82110750 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r9,0(r3)
  r9 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r3)).count_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // ble 0x82110780
  if (!cr0.gt) goto loc_82110780;
  // addi r11,r3,44
  r11 = uint64_t(int64_t(int64_t(r3) + 44));
loc_82110764:
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r11) - 44).records_address() + 40);
  // cmpw cr6,r8,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r4), so);
  // bne cr6,0x82110790
  if (!cr6.eq) goto loc_82110790;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r11,48
  r11 = uint64_t(int64_t(int64_t(r11) + 48));
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // blt cr6,0x82110764
  if (cr6.lt) goto loc_82110764;
loc_82110780:
  // addi r11,r9,-15
  r11 = uint64_t(int64_t(int64_t(r9) + -15));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // blr
  return r3;
loc_82110790:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82110798 game::Results::AnyRepeated: lifted from generated/ by lift.py.
uint64_t f82110798(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82110798 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // std r31,-8(r1)
  guest::StoreU64(base, uint32_t(r1) + -8, r31);
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r3)).count_address());
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi r31,0
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // ble 0x82110810
  if (!cr0.gt) goto loc_82110810;
  // addi r8,r1,-68
  r8 = uint64_t(int64_t(int64_t(r1) + -68));
  // addi r10,r3,24
  r10 = uint64_t(int64_t(int64_t(r3) + 24));
loc_821107B4:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x821107ec
  if (!cr6.gt) goto loc_821107EC;
  // lwz r5,0(r10)
  r5 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r10) - 24).records_address() + 20);
  // addi r9,r1,-68
  r9 = uint64_t(int64_t(int64_t(r1) + -68));
  // mr r7,r11
  r7 = r11;
loc_821107C8:
  // lwz r6,-60(r9)
  r6 = guest::LoadU32(base, uint32_t(r9) + -60);
  // cmpw cr6,r6,r5
  cr6 = lifted::Compare<int32_t>(int32_t(r6), int32_t(r5), so);
  // bne cr6,0x821107e0
  if (!cr6.eq) goto loc_821107E0;
  // lwz r6,0(r9)
  r6 = guest::LoadU32(base, uint32_t(r9) + 0);
  // addi r6,r6,1
  r6 = uint64_t(int64_t(int64_t(r6) + 1));
  // stw r6,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r6));
loc_821107E0:
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bne 0x821107c8
  if (!cr0.eq) goto loc_821107C8;
loc_821107EC:
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::CareerResults(base, uint32_t(r10) - 24).records_address() + 20);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // cmpw cr6,r11,r31
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r31), so);
  // stw r9,-60(r8)
  guest::StoreU32(base, uint32_t(r8) + -60, uint32_t(r9));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,0(r8)
  guest::StoreU32(base, uint32_t(r8) + 0, uint32_t(r9));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // blt cr6,0x821107b4
  if (cr6.lt) goto loc_821107B4;
loc_82110810:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x8211083c
  if (!cr6.gt) goto loc_8211083C;
  // addi r9,r1,-68
  r9 = uint64_t(int64_t(int64_t(r1) + -68));
loc_82110820:
  // lwz r8,0(r9)
  r8 = guest::LoadU32(base, uint32_t(r9) + 0);
  // cmpw cr6,r8,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r4), so);
  // bge cr6,0x82110848
  if (!cr6.lt) goto loc_82110848;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // blt cr6,0x82110820
  if (cr6.lt) goto loc_82110820;
loc_8211083C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82110840:
  // ld r31,-8(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -8);
  // blr
  return r3;
loc_82110848:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82110840
  goto loc_82110840;
}

// sub_82110928 game::DayLog::SumRaces: lifted from generated/ by lift.py.
double f82110928(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82110928 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r3,16
  r10 = uint64_t(int64_t(int64_t(r3) + 16));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // li r11,9
  r11 = uint64_t(int64_t(9));
loc_82110938:
  // lwa r9,0(r10)
  r9 = uint64_t(int64_t(int32_t(guest::LoadU32(base, schema::CareerDayLog(base, uint32_t(r10) - 16).records_address() + 12))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,12
  r10 = uint64_t(int64_t(int64_t(r10) + 12));
  // std r9,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r9);
  // lfd f13,-16(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // bne 0x82110938
  if (!cr0.eq) goto loc_82110938;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,8124(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8124);
  f13 = double(temp.f32);
  // fmuls f1,f0,f13
  f1 = double(float(f0 * f13));
  // blr
  return f1;
}

// sub_82110970 game::DayLog::SumWord8: lifted from generated/ by lift.py.
double f82110970(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82110970 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r3,24
  r10 = uint64_t(int64_t(int64_t(r3) + 24));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // li r11,9
  r11 = uint64_t(int64_t(9));
loc_82110980:
  // lwa r9,0(r10)
  r9 = uint64_t(int64_t(int32_t(guest::LoadU32(base, schema::CareerDayLog(base, uint32_t(r10) - 24).records_address() + 20))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,12
  r10 = uint64_t(int64_t(int64_t(r10) + 12));
  // std r9,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r9);
  // lfd f13,-16(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // bne 0x82110980
  if (!cr0.eq) goto loc_82110980;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,8124(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8124);
  f13 = double(temp.f32);
  // fmuls f1,f0,f13
  f1 = double(float(f0 * f13));
  // blr
  return f1;
}

// sub_82110B40 game::Items::AllWonKind5Or4: lifted from generated/ by lift.py.
uint64_t f82110B40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82110B40 (lifted)");
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
  // stwu r1,-3120(r1)
  ea = -3120 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,6048(r3)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r3)).kind5_won_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82110b68
  if (cr6.eq) goto loc_82110B68;
loc_82110B60:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82110bd0
  goto loc_82110BD0;
loc_82110B68:
  // addis r30,r3,2
  r30 = uint64_t(int64_t(int64_t(r3) + 131072));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r30,-26118
  r30 = uint64_t(int64_t(int64_t(r30) + -26118));
loc_82110B74:
  // mr r3,r31
  r3 = r31;
  // bl 0x82117178
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Item(base, LIFTED_ARGS(0x82110B7C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,3008
  r5 = uint64_t(int64_t(3008));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82110B8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x82110bbc
  if (cr6.eq) goto loc_82110BBC;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x82110bb0
  if (!cr6.eq) goto loc_82110BB0;
  // lhz r11,122(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 122);
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // beq cr6,0x82110bbc
  if (cr6.eq) goto loc_82110BBC;
loc_82110BB0:
  // lhz r11,0(r30)
  r11 = guest::LoadU16(base, uint32_t(r30) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82110b60
  if (cr6.eq) goto loc_82110B60;
loc_82110BBC:
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,12
  r30 = uint64_t(int64_t(int64_t(r30) + 12));
  // cmpwi cr6,r31,400
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 400, so);
  // blt cr6,0x82110b74
  if (cr6.lt) goto loc_82110B74;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_82110BD0:
  // addi r1,r1,3120
  r1 = uint64_t(int64_t(int64_t(r1) + 3120));
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

// sub_82110BE8 game::Progress::TablesDoneAtLeast: lifted from generated/ by lift.py.
uint64_t f82110BE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82110BE8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addis r27,r3,2
  r27 = uint64_t(int64_t(int64_t(r3) + 131072));
  // mr r25,r4
  r25 = r4;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r31,2
  r31 = uint64_t(int64_t(2));
  // addi r27,r27,-19080
  r27 = uint64_t(int64_t(int64_t(r27) + -19080));
loc_82110C0C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // bl 0x82117190
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry56(base, LIFTED_ARGS(0x82110C1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r3)).kind_address());
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82110c60
  if (cr6.eq) goto loc_82110C60;
  // mr r29,r27
  r29 = r27;
loc_82110C2C:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r29) - 111992).table_words_address() + 1536);
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82110c40
  if (!cr6.eq) goto loc_82110C40;
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
loc_82110C40:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bl 0x82117190
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry56(base, LIFTED_ARGS(0x82110C54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r3)).kind_address());
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82110c2c
  if (!cr6.eq) goto loc_82110C2C;
loc_82110C60:
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r27,r27,768
  r27 = uint64_t(int64_t(int64_t(r27) + 768));
  // cmpwi cr6,r31,8
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 8, so);
  // blt cr6,0x82110c0c
  if (cr6.lt) goto loc_82110C0C;
  // mulli r10,r26,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(100))));
  // rotlwi r11,r10,1
  r11 = __builtin_rotateleft32(uint32_t(r10), 1);
  // divw r10,r10,r28
  r10 = uint32_t((int32_t(r28) && !(int32_t(r10) == INT32_MIN && int32_t(r28) == -1)) ? int32_t(r10) / int32_t(r28) : 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // twllei r28,0
  if (int32_t(r28) == 0 || uint32_t(r28) < 0u) lifted::Trap(0);
  // andc r11,r28,r11
  r11 = r28 & ~r11;
  // cmpw cr6,r10,r25
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r25), so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
  // bge cr6,0x82110c9c
  if (!cr6.lt) goto loc_82110C9C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82110C9C:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return r3;
}

// sub_82110CA8: lifted from generated/ by lift.py.
uint64_t f82110CA8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82110CA8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // std r31,-8(r1)
  guest::StoreU64(base, uint32_t(r1) + -8, r31);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r31,11
  r31 = uint64_t(int64_t(11));
  // li r6,13
  r6 = uint64_t(int64_t(13));
  // li r7,15
  r7 = uint64_t(int64_t(15));
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // lfs f0,-32208(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32208);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-144(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -144, temp.u32);
  // li r9,21
  r9 = uint64_t(int64_t(21));
  // li r10,24
  r10 = uint64_t(int64_t(24));
  // stw r31,-332(r1)
  guest::StoreU32(base, uint32_t(r1) + -332, uint32_t(r31));
  // stw r6,-324(r1)
  guest::StoreU32(base, uint32_t(r1) + -324, uint32_t(r6));
  // stw r7,-316(r1)
  guest::StoreU32(base, uint32_t(r1) + -316, uint32_t(r7));
  // lfs f0,-32212(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32212);
  f0 = double(temp.f32);
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // stfs f0,-140(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -140, temp.u32);
  // stw r8,-304(r1)
  guest::StoreU32(base, uint32_t(r1) + -304, uint32_t(r8));
  // stw r9,-292(r1)
  guest::StoreU32(base, uint32_t(r1) + -292, uint32_t(r9));
  // stw r10,-280(r1)
  guest::StoreU32(base, uint32_t(r1) + -280, uint32_t(r10));
  // stw r11,-336(r1)
  guest::StoreU32(base, uint32_t(r1) + -336, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8676(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8676);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-136(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -136, temp.u32);
  // lfs f0,-32216(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32216);
  f0 = double(temp.f32);
  // li r11,12
  r11 = uint64_t(int64_t(12));
  // stfs f0,-132(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -132, temp.u32);
  // stw r11,-328(r1)
  guest::StoreU32(base, uint32_t(r1) + -328, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10576(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10576);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-128(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -128, temp.u32);
  // lfs f0,-32220(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32220);
  f0 = double(temp.f32);
  // li r11,14
  r11 = uint64_t(int64_t(14));
  // stfs f0,-124(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -124, temp.u32);
  // stw r11,-320(r1)
  guest::StoreU32(base, uint32_t(r1) + -320, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32224(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32224);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-120(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -120, temp.u32);
  // lfs f0,-32228(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32228);
  f0 = double(temp.f32);
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stfs f0,-116(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -116, temp.u32);
  // stw r11,-312(r1)
  guest::StoreU32(base, uint32_t(r1) + -312, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32232(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32232);
  f0 = double(temp.f32);
  // li r11,17
  r11 = uint64_t(int64_t(17));
  // stfs f0,-112(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -112, temp.u32);
  // stw r11,-308(r1)
  guest::StoreU32(base, uint32_t(r1) + -308, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32236(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32236);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-108(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -108, temp.u32);
  // lfs f0,-32240(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32240);
  f0 = double(temp.f32);
  // li r11,19
  r11 = uint64_t(int64_t(19));
  // stfs f0,-104(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -104, temp.u32);
  // stw r11,-300(r1)
  guest::StoreU32(base, uint32_t(r1) + -300, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32244(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32244);
  f0 = double(temp.f32);
  // li r11,20
  r11 = uint64_t(int64_t(20));
  // stfs f0,-100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -100, temp.u32);
  // stw r11,-296(r1)
  guest::StoreU32(base, uint32_t(r1) + -296, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32248(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32248);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -96, temp.u32);
  // lfs f0,-32252(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32252);
  f0 = double(temp.f32);
  // li r11,22
  r11 = uint64_t(int64_t(22));
  // stfs f0,-92(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -92, temp.u32);
  // stw r11,-288(r1)
  guest::StoreU32(base, uint32_t(r1) + -288, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,9844(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9844);
  f0 = double(temp.f32);
  // li r11,23
  r11 = uint64_t(int64_t(23));
  // stfs f0,-88(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -88, temp.u32);
  // stw r11,-284(r1)
  guest::StoreU32(base, uint32_t(r1) + -284, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32256(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32256);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-272(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -272, temp.u32);
  // lfs f0,8696(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8696);
  f0 = double(temp.f32);
  // stfs f0,-268(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -268, temp.u32);
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r31,-68(r1)
  guest::StoreU32(base, uint32_t(r1) + -68, uint32_t(r31));
  // stw r6,-64(r1)
  guest::StoreU32(base, uint32_t(r1) + -64, uint32_t(r6));
  // stw r11,-80(r1)
  guest::StoreU32(base, uint32_t(r1) + -80, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32260(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32260);
  f0 = double(temp.f32);
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stfs f0,-264(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -264, temp.u32);
  // stw r11,-76(r1)
  guest::StoreU32(base, uint32_t(r1) + -76, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32264(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32264);
  f0 = double(temp.f32);
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stfs f0,-260(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -260, temp.u32);
  // stw r11,-72(r1)
  guest::StoreU32(base, uint32_t(r1) + -72, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32268(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32268);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-256(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -256, temp.u32);
  // lfs f0,26892(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26892);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-252(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -252, temp.u32);
  // lfs f0,8692(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8692);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-248(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -248, temp.u32);
  // lfs f13,-32272(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32272);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-244(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -244, temp.u32);
  // lfs f13,-32276(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32276);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-240(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -240, temp.u32);
  // lfs f13,-32280(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32280);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-236(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -236, temp.u32);
  // lfs f13,-32284(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32284);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-232(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -232, temp.u32);
  // lfs f13,-32288(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32288);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,-228(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -228, temp.u32);
  // lfs f13,13744(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 13744);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-224(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -224, temp.u32);
  // lfs f13,-32292(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32292);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,-220(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -220, temp.u32);
  // lfs f13,3124(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 3124);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,-216(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -216, temp.u32);
  // lfs f13,8120(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8120);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,-400(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -400, temp.u32);
  // lfs f13,26160(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26160);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-396(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -396, temp.u32);
  // lfs f13,-32296(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32296);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-392(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -392, temp.u32);
  // lfs f13,-32300(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32300);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-388(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -388, temp.u32);
  // lfs f13,-32304(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32304);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-384(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -384, temp.u32);
  // lfs f13,-32308(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32308);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,-380(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -380, temp.u32);
  // lfs f13,11316(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11316);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-376(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -376, temp.u32);
  // lfs f13,-32312(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32312);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-372(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -372, temp.u32);
  // lfs f13,-32316(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32316);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-368(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -368, temp.u32);
  // lfs f13,-32320(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32320);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,-364(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -364, temp.u32);
  // lfs f13,10148(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10148);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-360(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -360, temp.u32);
  // lfs f13,-32324(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32324);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-356(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -356, temp.u32);
  // stw r10,-48(r1)
  guest::StoreU32(base, uint32_t(r1) + -48, uint32_t(r10));
  // stfs f0,-208(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -208, temp.u32);
  // li r10,27
  r10 = uint64_t(int64_t(27));
  // stw r7,-60(r1)
  guest::StoreU32(base, uint32_t(r1) + -60, uint32_t(r7));
  // stw r8,-56(r1)
  guest::StoreU32(base, uint32_t(r1) + -56, uint32_t(r8));
  // lfs f13,-32328(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32328);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-352(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -352, temp.u32);
  // stw r9,-52(r1)
  guest::StoreU32(base, uint32_t(r1) + -52, uint32_t(r9));
  // stw r10,-44(r1)
  guest::StoreU32(base, uint32_t(r1) + -44, uint32_t(r10));
  // li r10,30
  r10 = uint64_t(int64_t(30));
  // lfs f13,-32332(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32332);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-348(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -348, temp.u32);
  // stw r10,-40(r1)
  guest::StoreU32(base, uint32_t(r1) + -40, uint32_t(r10));
  // li r10,33
  r10 = uint64_t(int64_t(33));
  // lfs f13,-32336(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32336);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,-344(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -344, temp.u32);
  // stw r10,-36(r1)
  guest::StoreU32(base, uint32_t(r1) + -36, uint32_t(r10));
  // li r10,37
  r10 = uint64_t(int64_t(37));
  // lfs f0,8152(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8152);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-204(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -204, temp.u32);
  // stw r10,-32(r1)
  guest::StoreU32(base, uint32_t(r1) + -32, uint32_t(r10));
  // li r10,41
  r10 = uint64_t(int64_t(41));
  // lfs f0,10704(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10704);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-200(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -200, temp.u32);
  // stw r10,-28(r1)
  guest::StoreU32(base, uint32_t(r1) + -28, uint32_t(r10));
  // li r10,45
  r10 = uint64_t(int64_t(45));
  // lfs f0,9576(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9576);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-196(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -196, temp.u32);
  // stw r10,-24(r1)
  guest::StoreU32(base, uint32_t(r1) + -24, uint32_t(r10));
  // lfs f0,-32716(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32716);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-192(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -192, temp.u32);
  // lfs f0,3396(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 3396);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-188(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -188, temp.u32);
  // lfs f0,-32340(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32340);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-184(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -184, temp.u32);
  // lfs f0,8112(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8112);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-180(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -180, temp.u32);
  // lfs f0,9572(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9572);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-176(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -176, temp.u32);
  // lfs f0,9596(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9596);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-172(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -172, temp.u32);
  // lfs f0,-32344(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32344);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-168(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -168, temp.u32);
  // lfs f0,3860(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 3860);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-164(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -164, temp.u32);
  // lfs f0,-32348(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32348);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-160(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -160, temp.u32);
  // lfs f0,10784(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10784);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-156(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -156, temp.u32);
  // lfs f0,11324(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11324);
  f0 = double(temp.f32);
  // addic. r11,r5,-1
  ca = uint32_t(r5) > 0;
  r11 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stfs f0,-152(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -152, temp.u32);
  // blt 0x821110f8
  if (cr0.lt) goto loc_821110F8;
  // lfs f0,5960(r3)
  temp.u32 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 5912);
  f0 = double(temp.f32);
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
loc_8211105C:
  // addi r9,r1,-144
  r9 = uint64_t(int64_t(int64_t(r1) + -144));
  // lfsx f13,r10,r9
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x82111104
  if (cr6.gt) goto loc_82111104;
  // addi r9,r1,-272
  r9 = uint64_t(int64_t(int64_t(r1) + -272));
  // lfsx f13,r10,r9
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x82111118
  if (!cr6.gt) goto loc_82111118;
  // addi r9,r1,-400
  r9 = uint64_t(int64_t(int64_t(r1) + -400));
  // lfs f13,5952(r3)
  temp.u32 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 5904);
  f13 = double(temp.f32);
  // lfsx f12,r10,r9
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // ble cr6,0x82111120
  if (!cr6.gt) goto loc_82111120;
  // lha r9,66(r3)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 18))));
  // addi r7,r1,-336
  r7 = uint64_t(int64_t(int64_t(r1) + -336));
  // lha r8,70(r3)
  r8 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 22))));
  // mulli r6,r9,100
  r6 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(100))));
  // lwzx r7,r10,r7
  r7 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r7));
  // rotlwi r9,r6,1
  r9 = __builtin_rotateleft32(uint32_t(r6), 1);
  // divw r6,r6,r8
  r6 = uint32_t((int32_t(r8) && !(int32_t(r6) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r6) / int32_t(r8) : 0);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // andc r9,r8,r9
  r9 = r8 & ~r9;
  // cmpw cr6,r7,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r6), so);
  // twlgei r9,-1
  if (int32_t(r9) == -1 || uint32_t(r9) > 4294967295u) lifted::Trap(0);
  // ble cr6,0x82111128
  if (!cr6.gt) goto loc_82111128;
  // addi r9,r1,-208
  r9 = uint64_t(int64_t(int64_t(r1) + -208));
  // lfs f13,5968(r3)
  temp.u32 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 5920);
  f13 = double(temp.f32);
  // lfsx f12,r10,r9
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // ble cr6,0x82111130
  if (!cr6.gt) goto loc_82111130;
  // addi r9,r1,-80
  r9 = uint64_t(int64_t(int64_t(r1) + -80));
  // lha r8,6016(r3)
  r8 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 5968))));
  // lwzx r9,r10,r9
  r9 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // cmpw cr6,r9,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r8), so);
  // ble cr6,0x82111138
  if (!cr6.gt) goto loc_82111138;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // bge 0x8211105c
  if (!cr0.lt) goto loc_8211105C;
loc_821110F8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_821110FC:
  // ld r31,-8(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -8);
  // blr
  return r3;
loc_82111104:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // sth r11,8(r4)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r4)).first_value_address(), uint16_t(r11));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r10,4(r4)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r4)).first_kind_address(), uint32_t(r10));
  // b 0x821110fc
  goto loc_821110FC;
loc_82111118:
  // addi r11,r11,15
  r11 = uint64_t(int64_t(int64_t(r11) + 15));
  // b 0x82111104
  goto loc_82111104;
loc_82111120:
  // addi r11,r11,30
  r11 = uint64_t(int64_t(int64_t(r11) + 30));
  // b 0x82111104
  goto loc_82111104;
loc_82111128:
  // addi r11,r11,45
  r11 = uint64_t(int64_t(int64_t(r11) + 45));
  // b 0x82111104
  goto loc_82111104;
loc_82111130:
  // addi r11,r11,60
  r11 = uint64_t(int64_t(int64_t(r11) + 60));
  // b 0x82111104
  goto loc_82111104;
loc_82111138:
  // addi r11,r11,75
  r11 = uint64_t(int64_t(int64_t(r11) + 75));
  // b 0x82111104
  goto loc_82111104;
}

// sub_82111140: lifted from generated/ by lift.py.
lifted::Result f82111140(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82111140 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // stfd f31,-8(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -8, std::bit_cast<uint64_t>(f31));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8696(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8696);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-336(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -336, temp.u32);
  // lfs f0,10720(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10720);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-332(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -332, temp.u32);
  // lfs f0,-32212(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32212);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-328(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -328, temp.u32);
  // stfs f0,-140(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -140, temp.u32);
  // lfs f13,10576(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10576);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,-324(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -324, temp.u32);
  // lfs f13,9844(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9844);
  f13 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f13,-320(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -320, temp.u32);
  // stfs f13,-128(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -128, temp.u32);
  // lfs f12,-32108(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32108);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,-316(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + -316, temp.u32);
  // lfs f12,8680(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8680);
  f12 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f12,-312(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + -312, temp.u32);
  // lfs f12,-32112(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32112);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,-308(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + -308, temp.u32);
  // lfs f12,11328(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11328);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,-304(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + -304, temp.u32);
  // lfs f12,10152(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10152);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,-300(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + -300, temp.u32);
  // stfs f12,-108(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + -108, temp.u32);
  // lfs f11,11448(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11448);
  f11 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f11,-296(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + -296, temp.u32);
  // stfs f11,-104(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + -104, temp.u32);
  // lfs f10,10736(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10736);
  f10 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f10,-292(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + -292, temp.u32);
  // stfs f10,-100(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + -100, temp.u32);
  // lfs f9,10708(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10708);
  f9 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f9,-288(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + -288, temp.u32);
  // stfs f9,-96(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + -96, temp.u32);
  // lfs f8,11440(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11440);
  f8 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f8,-284(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + -284, temp.u32);
  // stfs f8,-92(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + -92, temp.u32);
  // lfs f7,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f7 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f7,-280(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + -280, temp.u32);
  // stfs f7,-88(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + -88, temp.u32);
  // lfs f6,9588(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9588);
  f6 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f6,-144(r1)
  temp.f32 = float(f6);
  guest::StoreU32(base, uint32_t(r1) + -144, temp.u32);
  // lfs f0,-32216(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32216);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-136(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -136, temp.u32);
  // lfs f0,-32224(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32224);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-132(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -132, temp.u32);
  // lfs f0,-32116(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32116);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-124(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -124, temp.u32);
  // lfs f0,-32120(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32120);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-120(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -120, temp.u32);
  // lfs f0,-32124(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32124);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-116(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -116, temp.u32);
  // lfs f0,11504(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11504);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-112(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -112, temp.u32);
  // lfs f0,3860(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 3860);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-400(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -400, temp.u32);
  // lfs f0,26544(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26544);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-396(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -396, temp.u32);
  // lfs f0,-32128(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32128);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-392(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -392, temp.u32);
  // lfs f0,26548(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26548);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-388(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -388, temp.u32);
  // lfs f0,-32132(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32132);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-384(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -384, temp.u32);
  // lfs f0,7820(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7820);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-380(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -380, temp.u32);
  // lfs f0,-32136(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32136);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-376(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -376, temp.u32);
  // lfs f0,-32140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32140);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-372(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -372, temp.u32);
  // lfs f0,-32144(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32144);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-368(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -368, temp.u32);
  // lfs f0,-32148(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32148);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-364(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -364, temp.u32);
  // lfs f0,7816(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7816);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-360(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -360, temp.u32);
  // lfs f0,-32152(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32152);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-356(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -356, temp.u32);
  // lfs f0,-32156(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32156);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-352(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -352, temp.u32);
  // lfs f0,-32160(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32160);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-348(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -348, temp.u32);
  // lfs f0,-32164(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32164);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-344(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -344, temp.u32);
  // lfs f0,8692(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8692);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-208(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -208, temp.u32);
  // lfs f13,-32168(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32168);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,-204(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -204, temp.u32);
  // lfs f12,11436(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11436);
  f12 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f12,-200(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + -200, temp.u32);
  // lfs f11,-32288(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32288);
  f11 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f11,-196(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + -196, temp.u32);
  // lfs f10,13744(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 13744);
  f10 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f10,-192(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + -192, temp.u32);
  // lfs f9,-32292(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32292);
  f9 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f9,-188(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + -188, temp.u32);
  // lfs f8,3124(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 3124);
  f8 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f8,-184(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + -184, temp.u32);
  // lfs f7,26692(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26692);
  f7 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f7,-180(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + -180, temp.u32);
  // lfs f6,8152(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8152);
  f6 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f6,-176(r1)
  temp.f32 = float(f6);
  guest::StoreU32(base, uint32_t(r1) + -176, temp.u32);
  // lfs f5,11396(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11396);
  f5 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f5,-172(r1)
  temp.f32 = float(f5);
  guest::StoreU32(base, uint32_t(r1) + -172, temp.u32);
  // lfs f4,11384(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11384);
  f4 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f4,-168(r1)
  temp.f32 = float(f4);
  guest::StoreU32(base, uint32_t(r1) + -168, temp.u32);
  // lfs f3,11372(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11372);
  f3 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f3,-164(r1)
  temp.f32 = float(f3);
  guest::StoreU32(base, uint32_t(r1) + -164, temp.u32);
  // lfs f2,8684(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8684);
  f2 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f2,-160(r1)
  temp.f32 = float(f2);
  guest::StoreU32(base, uint32_t(r1) + -160, temp.u32);
  // lfs f1,13736(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 13736);
  f1 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f1,-156(r1)
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r1) + -156, temp.u32);
  // lfs f31,9576(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9576);
  f31 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -80, temp.u32);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stfs f31,-152(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + -152, temp.u32);
  // stfs f13,-76(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + -76, temp.u32);
  // stfs f12,-72(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + -72, temp.u32);
  // lfs f0,10764(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10764);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-272(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -272, temp.u32);
  // stfs f11,-68(r1)
  temp.f32 = float(f11);
  guest::StoreU32(base, uint32_t(r1) + -68, temp.u32);
  // stfs f10,-64(r1)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r1) + -64, temp.u32);
  // stfs f9,-60(r1)
  temp.f32 = float(f9);
  guest::StoreU32(base, uint32_t(r1) + -60, temp.u32);
  // lfs f0,12200(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 12200);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-268(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -268, temp.u32);
  // stfs f8,-56(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + -56, temp.u32);
  // stfs f7,-52(r1)
  temp.f32 = float(f7);
  guest::StoreU32(base, uint32_t(r1) + -52, temp.u32);
  // stfs f6,-48(r1)
  temp.f32 = float(f6);
  guest::StoreU32(base, uint32_t(r1) + -48, temp.u32);
  // lfs f0,8128(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8128);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stfs f0,-264(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -264, temp.u32);
  // stfs f5,-44(r1)
  temp.f32 = float(f5);
  guest::StoreU32(base, uint32_t(r1) + -44, temp.u32);
  // stfs f4,-40(r1)
  temp.f32 = float(f4);
  guest::StoreU32(base, uint32_t(r1) + -40, temp.u32);
  // stfs f3,-36(r1)
  temp.f32 = float(f3);
  guest::StoreU32(base, uint32_t(r1) + -36, temp.u32);
  // lfs f0,-32176(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32176);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,-260(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -260, temp.u32);
  // stfs f2,-32(r1)
  temp.f32 = float(f2);
  guest::StoreU32(base, uint32_t(r1) + -32, temp.u32);
  // stfs f1,-28(r1)
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r1) + -28, temp.u32);
  // stfs f31,-24(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + -24, temp.u32);
  // lfs f0,7132(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7132);
  f0 = double(temp.f32);
  // addic. r11,r5,-1
  ca = uint32_t(r5) > 0;
  r11 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stfs f0,-256(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -256, temp.u32);
  // lfs f0,-32172(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -32172);
  f0 = double(temp.f32);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stfs f0,-252(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -252, temp.u32);
  // lfs f0,-32180(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -32180);
  f0 = double(temp.f32);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stfs f0,-248(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -248, temp.u32);
  // lfs f0,-32184(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -32184);
  f0 = double(temp.f32);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stfs f0,-244(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -244, temp.u32);
  // lfs f0,-32188(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -32188);
  f0 = double(temp.f32);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stfs f0,-240(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -240, temp.u32);
  // lfs f0,-32192(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -32192);
  f0 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stfs f0,-236(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -236, temp.u32);
  // lfs f0,8136(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 8136);
  f0 = double(temp.f32);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stfs f0,-232(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -232, temp.u32);
  // lfs f0,-32196(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -32196);
  f0 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stfs f0,-228(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -228, temp.u32);
  // lfs f0,26980(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 26980);
  f0 = double(temp.f32);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stfs f0,-224(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -224, temp.u32);
  // lfs f0,-32200(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -32200);
  f0 = double(temp.f32);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stfs f0,-220(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -220, temp.u32);
  // lfs f0,-32204(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -32204);
  f0 = double(temp.f32);
  // stfs f0,-216(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + -216, temp.u32);
  // blt 0x82111580
  if (cr0.lt) goto loc_82111580;
  // lfs f13,5936(r3)
  temp.u32 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 5888);
  f13 = double(temp.f32);
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
loc_821114D4:
  // addi r9,r1,-336
  r9 = uint64_t(int64_t(int64_t(r1) + -336));
  // lfsx f0,r10,r9
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x8211158c
  if (cr6.gt) goto loc_8211158C;
  // addi r9,r1,-144
  r9 = uint64_t(int64_t(int64_t(r1) + -144));
  // lfs f0,5928(r3)
  temp.u32 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 5880);
  f0 = double(temp.f32);
  // lfsx f12,r10,r9
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f0
  cr6 = lifted::CompareFloat(f12, f0);
  // bgt cr6,0x821115a0
  if (cr6.gt) goto loc_821115A0;
  // addi r7,r1,-400
  r7 = uint64_t(int64_t(int64_t(r1) + -400));
  // lha r9,60(r3)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 12))));
  // lha r8,70(r3)
  r8 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 22))));
  // mulli r9,r9,100
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(100))));
  // lfsx f12,r10,r7
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r7));
  f12 = double(temp.f32);
  // divw r7,r9,r8
  r7 = uint32_t((int32_t(r8) && !(int32_t(r9) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r9) / int32_t(r8) : 0);
  // rotlwi r9,r9,1
  r9 = __builtin_rotateleft32(uint32_t(r9), 1);
  // extsw r7,r7
  r7 = uint64_t(int64_t(int32_t(r7)));
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // andc r9,r8,r9
  r9 = r8 & ~r9;
  // std r7,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r7);
  // twlgei r9,-1
  if (int32_t(r9) == -1 || uint32_t(r9) > 4294967295u) lifted::Trap(0);
  // lfd f11,-16(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fcmpu cr6,f12,f11
  cr6 = lifted::CompareFloat(f12, f11);
  // ble cr6,0x821115a8
  if (!cr6.gt) goto loc_821115A8;
  // addi r9,r1,-208
  r9 = uint64_t(int64_t(int64_t(r1) + -208));
  // lfsx f12,r10,r9
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // ble cr6,0x821115b0
  if (!cr6.gt) goto loc_821115B0;
  // addi r9,r1,-80
  r9 = uint64_t(int64_t(int64_t(r1) + -80));
  // lfsx f12,r10,r9
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f0
  cr6 = lifted::CompareFloat(f12, f0);
  // ble cr6,0x821115b8
  if (!cr6.gt) goto loc_821115B8;
  // addi r9,r1,-272
  r9 = uint64_t(int64_t(int64_t(r1) + -272));
  // lfs f0,6040(r3)
  temp.u32 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r3)).stats_address() + 5992);
  f0 = double(temp.f32);
  // lfsx f12,r10,r9
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f0
  cr6 = lifted::CompareFloat(f12, f0);
  // ble cr6,0x821115c0
  if (!cr6.gt) goto loc_821115C0;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // bge 0x821114d4
  if (!cr0.lt) goto loc_821114D4;
loc_82111580:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82111584:
  // lfd f31,-8(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -8)));
  // blr
  return {r3, f1};
loc_8211158C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // sth r11,16(r4)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r4)).second_value_address(), uint16_t(r11));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r10,12(r4)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r4)).second_kind_address(), uint32_t(r10));
  // b 0x82111584
  goto loc_82111584;
loc_821115A0:
  // addi r11,r11,15
  r11 = uint64_t(int64_t(int64_t(r11) + 15));
  // b 0x8211158c
  goto loc_8211158C;
loc_821115A8:
  // addi r11,r11,30
  r11 = uint64_t(int64_t(int64_t(r11) + 30));
  // b 0x8211158c
  goto loc_8211158C;
loc_821115B0:
  // addi r11,r11,45
  r11 = uint64_t(int64_t(int64_t(r11) + 45));
  // b 0x8211158c
  goto loc_8211158C;
loc_821115B8:
  // addi r11,r11,60
  r11 = uint64_t(int64_t(int64_t(r11) + 60));
  // b 0x8211158c
  goto loc_8211158C;
loc_821115C0:
  // addi r11,r11,75
  r11 = uint64_t(int64_t(int64_t(r11) + 75));
  // b 0x8211158c
  goto loc_8211158C;
}

// sub_821115C8 game::Pick::IsSpecial: lifted from generated/ by lift.py.
uint64_t f821115C8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_821115C8 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // std r3,16(r1)
  guest::StoreU64(base, uint32_t(r1) + 16, r3);
  // std r4,24(r1)
  guest::StoreU64(base, uint32_t(r1) + 24, r4);
  // std r5,32(r1)
  guest::StoreU64(base, uint32_t(r1) + 32, r5);
  // lwz r11,20(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 20);
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bne cr6,0x821115fc
  if (!cr6.eq) goto loc_821115FC;
  // lha r11,24(r1)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r1) + 24))));
  // cmpwi cr6,r11,17
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 17, so);
  // blt cr6,0x82111620
  if (cr6.lt) goto loc_82111620;
  // cmpwi cr6,r11,71
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 71, so);
  // bge cr6,0x82111620
  if (!cr6.lt) goto loc_82111620;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
loc_821115FC:
  // lwz r11,28(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 28);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x82111620
  if (!cr6.eq) goto loc_82111620;
  // lha r11,32(r1)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r1) + 32))));
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // blt cr6,0x82111620
  if (cr6.lt) goto loc_82111620;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bltlr cr6
  if (cr6.lt) return r3;
loc_82111620:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82112260: lifted from generated/ by lift.py.
lifted::Result f82112260(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82112260 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-64(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -64, std::bit_cast<uint64_t>(f31));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // addi r11,r29,48
  r11 = uint64_t(int64_t(int64_t(r29) + 48));
  // mr r26,r5
  r26 = r5;
  // addi r27,r11,5672
  r27 = uint64_t(int64_t(int64_t(r11) + 5672));
  // addi r28,r11,4948
  r28 = uint64_t(int64_t(int64_t(r11) + 4948));
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r27) - 5720).stats_address() + 5672);
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bne cr6,0x82112824
  if (!cr6.eq) goto loc_82112824;
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r28) - 4996).stats_address() + 4948);
  // cmpwi cr6,r11,15
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 15, so);
  // bne cr6,0x82112824
  if (!cr6.eq) goto loc_82112824;
  // lwz r11,6048(r29)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r29)).kind5_won_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821122f4
  if (cr6.eq) goto loc_821122F4;
  // lhz r11,60(r29)
  r11 = guest::LoadU16(base, schema::CareerSave(base, uint32_t(r29)).stats_address() + 12);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x821122f4
  if (!cr6.eq) goto loc_821122F4;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r29,6
  r29 = uint64_t(int64_t(6));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // stw r29,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r29));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // stb r6,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r6));
  // stw r29,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r29));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r6,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r6));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r11));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // b 0x82112828
  goto loc_82112828;
loc_821122F4:
  // lha r11,60(r29)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 60))));
  // lha r10,70(r29)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 70))));
  // mulli r9,r11,100
  r9 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(100))));
  // rotlwi r11,r9,1
  r11 = __builtin_rotateleft32(uint32_t(r9), 1);
  // divw r9,r9,r10
  r9 = uint32_t((int32_t(r10) && !(int32_t(r9) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r9) / int32_t(r10) : 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // andc r11,r10,r11
  r11 = r10 & ~r11;
  // cmpwi cr6,r9,95
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 95, so);
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,8152(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8152);
  f31 = double(temp.f32);
  // blt cr6,0x82112394
  if (cr6.lt) goto loc_82112394;
  // mr r3,r28
  r3 = r28;
  // bl 0x82110610
  // (left out: dead in C++)
  f1 = game::Results::MeanWord4(base, r1, r3);
  // fmr f12,f1
  lifted::ScalarFloatMode();
  f12 = f1;
  // bl 0x821105a8
  // (left out: dead in C++)
  f1 = game::Results::MeanWord0(base, r1, r3);
  // fadds f0,f1,f12
  lifted::ScalarFloatMode();
  f0 = double(float(f1 + f12));
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // bge cr6,0x82112394
  if (!cr6.lt) goto loc_82112394;
  // li r4,80
  r4 = uint64_t(int64_t(80));
  // mr r3,r29
  r3 = r29;
  // bl 0x82110be8
  // (left out: dead in C++)
  r3 = game::Progress::TablesDoneAtLeast(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82112394
  if (cr0.eq) goto loc_82112394;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r29,6
  r29 = uint64_t(int64_t(6));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // stw r29,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r29));
  // stb r6,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r6));
  // stw r29,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r29));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r6,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r6));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r11));
  // sth r6,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r6));
  // b 0x82112828
  goto loc_82112828;
loc_82112394:
  // lha r7,60(r29)
  r7 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 60))));
  // cmpwi cr6,r7,400
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 400, so);
  // blt cr6,0x82112414
  if (cr6.lt) goto loc_82112414;
  // mulli r10,r7,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(100))));
  // lha r9,70(r29)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 70))));
  // rotlwi r11,r10,1
  r11 = __builtin_rotateleft32(uint32_t(r10), 1);
  // divw r10,r10,r9
  r10 = uint32_t((int32_t(r9) && !(int32_t(r10) == INT32_MIN && int32_t(r9) == -1)) ? int32_t(r10) / int32_t(r9) : 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // twllei r9,0
  if (int32_t(r9) == 0 || uint32_t(r9) < 0u) lifted::Trap(0);
  // andc r11,r9,r11
  r11 = r9 & ~r11;
  // cmpwi cr6,r10,90
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 90, so);
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
  // blt cr6,0x82112414
  if (cr6.lt) goto loc_82112414;
  // lha r11,6018(r29)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 6018))));
  // cmpwi cr6,r11,45
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 45, so);
  // blt cr6,0x82112414
  if (cr6.lt) goto loc_82112414;
  // li r10,2
  r10 = uint64_t(int64_t(2));
loc_821123D8:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821123DC:
  // li r29,6
  r29 = uint64_t(int64_t(6));
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // stw r29,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r29));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // stb r6,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r6));
  // stw r29,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r29));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r6,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r6));
loc_82112400:
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // sth r10,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r10));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r11));
  // b 0x82112828
  goto loc_82112828;
loc_82112414:
  // lha r8,70(r29)
  r8 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 70))));
  // cmpwi cr6,r8,400
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 400, so);
  // blt cr6,0x82112484
  if (cr6.lt) goto loc_82112484;
  // mulli r10,r7,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(100))));
  // rotlwi r11,r10,1
  r11 = __builtin_rotateleft32(uint32_t(r10), 1);
  // divw r10,r10,r8
  r10 = uint32_t((int32_t(r8) && !(int32_t(r10) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r10) / int32_t(r8) : 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // andc r11,r8,r11
  r11 = r8 & ~r11;
  // cmpwi cr6,r10,88
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 88, so);
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
  // blt cr6,0x82112484
  if (cr6.lt) goto loc_82112484;
  // lha r11,66(r29)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 66))));
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // mulli r10,r11,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(100))));
  // rotlwi r11,r10,1
  r11 = __builtin_rotateleft32(uint32_t(r10), 1);
  // divw r10,r10,r8
  r10 = uint32_t((int32_t(r8) && !(int32_t(r10) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r10) / int32_t(r8) : 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r10,15
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 15, so);
  // andc r11,r8,r11
  r11 = r8 & ~r11;
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
  // bgt cr6,0x82112484
  if (cr6.gt) goto loc_82112484;
  // mr r3,r27
  r3 = r27;
  // bl 0x82110928
  // (left out: dead in C++)
  f1 = game::DayLog::SumRaces(base, r1, r3);
  // fcmpu cr6,f1,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f31);
  // blt cr6,0x82112484
  if (cr6.lt) goto loc_82112484;
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // b 0x821123d8
  goto loc_821123D8;
loc_82112484:
  // mulli r10,r7,100
  r10 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(100))));
  // rotlwi r11,r10,1
  r11 = __builtin_rotateleft32(uint32_t(r10), 1);
  // divw r10,r10,r8
  r10 = uint32_t((int32_t(r8) && !(int32_t(r10) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r10) / int32_t(r8) : 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // andc r11,r8,r11
  r11 = r8 & ~r11;
  // cmpwi cr6,r10,90
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 90, so);
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f12,-32716(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32716);
  f12 = double(temp.f32);
  // blt cr6,0x82112500
  if (cr6.lt) goto loc_82112500;
  // lfs f0,5936(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 5936);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x82112500
  if (cr6.lt) goto loc_82112500;
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r28
  r3 = r28;
  // bl 0x82110750
  // (left out: dead in C++)
  r3 = game::Results::AllWord40Are(base, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821124f8
  if (!cr0.eq) goto loc_821124F8;
  // li r4,19
  r4 = uint64_t(int64_t(19));
  // mr r3,r28
  r3 = r28;
  // bl 0x82110750
  // (left out: dead in C++)
  r3 = game::Results::AllWord40Are(base, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821124f8
  if (!cr0.eq) goto loc_821124F8;
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // mr r3,r28
  r3 = r28;
  // bl 0x82110750
  // (left out: dead in C++)
  r3 = game::Results::AllWord40Are(base, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82112500
  if (cr0.eq) goto loc_82112500;
loc_821124F8:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // b 0x821123d8
  goto loc_821123D8;
loc_82112500:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,5992(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 5992);
  f13 = double(temp.f32);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lfs f0,3396(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 3396);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x82112550
  if (!cr6.gt) goto loc_82112550;
  // li r29,6
  r29 = uint64_t(int64_t(6));
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r29,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r29));
  // stb r6,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r6));
  // stw r29,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r29));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r6,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r6));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r11));
  // sth r11,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r11));
  // b 0x82112828
  goto loc_82112828;
loc_82112550:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r28) - 4996).stats_address() + 4948);
  // mr r9,r30
  r9 = r30;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x821125b8
  if (!cr0.gt) goto loc_821125B8;
  // addi r10,r28,48
  r10 = uint64_t(int64_t(int64_t(r28) + 48));
loc_82112564:
  // lwz r8,0(r10)
  r8 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r10) - 5044).stats_address() + 4996);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // add r9,r8,r9
  r9 = r8 + r9;
  // bne 0x82112564
  if (!cr0.eq) goto loc_82112564;
  // cmpwi cr6,r9,8
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 8, so);
  // blt cr6,0x821125b8
  if (cr6.lt) goto loc_821125B8;
  // li r29,6
  r29 = uint64_t(int64_t(6));
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r29,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r29));
  // stb r6,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r6));
  // stw r29,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r29));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r6,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r6));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r11));
  // sth r29,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r29));
  // b 0x82112828
  goto loc_82112828;
loc_821125B8:
  // lfs f0,8(r27)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r27) - 5720).stats_address() + 5680);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x82112608
  if (!cr6.gt) goto loc_82112608;
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r27) - 5720).stats_address() + 5676);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82112608
  if (!cr6.eq) goto loc_82112608;
  // lfs f0,20(r27)
  temp.u32 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r27) - 5720).stats_address() + 5692);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x82112608
  if (!cr6.gt) goto loc_82112608;
  // lwz r11,16(r27)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r27) - 5720).stats_address() + 5688);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82112608
  if (!cr6.eq) goto loc_82112608;
  // lfs f0,32(r27)
  temp.u32 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r27) - 5720).stats_address() + 5704);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x82112608
  if (!cr6.gt) goto loc_82112608;
  // lwz r11,28(r27)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r27) - 5720).stats_address() + 5700);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82112608
  if (!cr6.eq) goto loc_82112608;
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // b 0x821123dc
  goto loc_821123DC;
loc_82112608:
  // mr r3,r27
  r3 = r27;
  // bl 0x82110970
  // (left out: dead in C++)
  f1 = game::DayLog::SumWord8(base, r1, r3);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10704(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10704);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x82112648
  if (cr6.lt) goto loc_82112648;
  // lfs f0,5936(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 5936);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x82112648
  if (cr6.lt) goto loc_82112648;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,5928(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 5928);
  f13 = double(temp.f32);
  // lfs f0,8112(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8112);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82112648
  if (cr6.lt) goto loc_82112648;
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // b 0x821123dc
  goto loc_821123DC;
loc_82112648:
  // mr r3,r29
  r3 = r29;
  // bl 0x82110b40
  // (left out: dead in C++)
  r3 = game::Items::AllWonKind5Or4(base, r1, r13, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82112660
  if (cr0.eq) goto loc_82112660;
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // b 0x821123dc
  goto loc_821123DC;
loc_82112660:
  // mr r3,r28
  r3 = r28;
  // bl 0x82110678
  // (left out: dead in C++)
  f1 = game::Results::MeanFloat8(base, r1, r3);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11320(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 11320);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x821126a8
  if (cr6.gt) goto loc_821126A8;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,5936(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 5936);
  f13 = double(temp.f32);
  // lfs f0,-32100(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32100);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x821126a8
  if (cr6.lt) goto loc_821126A8;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,5928(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 5928);
  f13 = double(temp.f32);
  // lfs f0,-32104(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32104);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x821126a8
  if (cr6.lt) goto loc_821126A8;
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // b 0x821123dc
  goto loc_821123DC;
loc_821126A8:
  // mr r3,r28
  r3 = r28;
  // bl 0x821106d0
  // (left out: dead in C++)
  f1 = game::Results::WonPercent(base, r1, r3);
  // fcmpu cr6,f1,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f31);
  // bge cr6,0x821126c0
  if (!cr6.lt) goto loc_821126C0;
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // b 0x821123dc
  goto loc_821123DC;
loc_821126C0:
  // lha r11,6020(r29)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 6020))));
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // blt cr6,0x821126d4
  if (cr6.lt) goto loc_821126D4;
  // li r10,12
  r10 = uint64_t(int64_t(12));
  // b 0x821123dc
  goto loc_821123DC;
loc_821126D4:
  // mr r3,r27
  r3 = r27;
  // bl 0x82110928
  // (left out: dead in C++)
  f1 = game::DayLog::SumRaces(base, r1, r3);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x82112714
  if (cr6.gt) goto loc_82112714;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,6040(r29)
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 6040);
  f13 = double(temp.f32);
  // lfs f0,-32172(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32172);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82112714
  if (cr6.lt) goto loc_82112714;
  // lha r11,6076(r29)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 6076))));
  // cmpwi cr6,r11,20
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 20, so);
  // ble cr6,0x82112714
  if (!cr6.gt) goto loc_82112714;
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // b 0x821123dc
  goto loc_821123DC;
loc_82112714:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r28) - 4996).stats_address() + 4948);
  // mr r9,r30
  r9 = r30;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x8211274c
  if (!cr0.gt) goto loc_8211274C;
  // addi r10,r28,36
  r10 = uint64_t(int64_t(int64_t(r28) + 36));
loc_82112728:
  // lwz r8,0(r10)
  r8 = guest::LoadU32(base, schema::CareerSave(base, uint32_t(r10) - 5032).stats_address() + 4984);
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x82112738
  if (cr6.eq) goto loc_82112738;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
loc_82112738:
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // bne 0x82112728
  if (!cr0.eq) goto loc_82112728;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x821127d0
  if (!cr6.eq) goto loc_821127D0;
loc_8211274C:
  // mr r3,r29
  r3 = r29;
  // stb r30,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r30));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // sth r30,96(r1)
  guest::StoreU16(base, uint32_t(r1) + 96, uint16_t(r30));
  // li r29,6
  r29 = uint64_t(int64_t(6));
  // sth r30,88(r1)
  guest::StoreU16(base, uint32_t(r1) + 88, uint16_t(r30));
  // mr r5,r26
  r5 = r26;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stb r6,90(r1)
  guest::StoreU8(base, uint32_t(r1) + 90, uint8_t(r6));
  // stw r29,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r29));
  // stb r6,98(r1)
  guest::StoreU8(base, uint32_t(r1) + 98, uint8_t(r6));
  // stw r6,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r6));
  // bl 0x82111140
  // (left out: dead in C++)
  { const lifted::Result o = f82111140(base, r1, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82112824
  if (cr0.eq) goto loc_82112824;
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r29,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r29));
  // mr r9,r31
  r9 = r31;
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stb r6,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r6));
  // stw r29,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r29));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r6,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r6));
  // mtctr r11
  ctr = r11;
loc_821127B4:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r11,0(r9)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r9)).head_address(), uint32_t(r11));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x821127b4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_821127B4;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82112828
  goto loc_82112828;
loc_821127D0:
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r28
  r3 = r28;
  // bl 0x82110798
  // (left out: dead in C++)
  r3 = game::Results::AnyRepeated(base, r1, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821127ec
  if (cr0.eq) goto loc_821127EC;
  // li r10,14
  r10 = uint64_t(int64_t(14));
  // b 0x82112400
  goto loc_82112400;
loc_821127EC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,5976(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 5976);
  f13 = double(temp.f32);
  // lfs f0,9596(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9596);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82112808
  if (cr6.lt) goto loc_82112808;
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // b 0x82112400
  goto loc_82112400;
loc_82112808:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,5984(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r29) + 5984);
  f13 = double(temp.f32);
  // lfs f0,9576(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9576);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82112824
  if (cr6.lt) goto loc_82112824;
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // b 0x82112400
  goto loc_82112400;
loc_82112824:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82112828:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82115550: lifted from generated/ by lift.py.
uint64_t f82115550(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82115550 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -4096);
  // stwu r1,-7088(r1)
  ea = -7088 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r28,r4
  r28 = r4;
  // lhz r11,6328(r31)
  r11 = guest::LoadU16(base, schema::CareerSave(base, uint32_t(r31)).garage_count_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82115640
  if (cr6.eq) goto loc_82115640;
  // addi r3,r1,3120
  r3 = uint64_t(int64_t(int64_t(r1) + 3120));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x8211557C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,6120
  r11 = uint64_t(int64_t(int64_t(r31) + 6120));
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // ori r10,r10,39200
  r10 = r10 | 39200;
  // lwzx r10,r31,r10
  r10 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r10));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lha r9,204(r11)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::CareerSave(base, uint32_t(r11) - 6120).garage_head_address()))));
  // b 0x821155a0
  goto loc_821155A0;
loc_82115598:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_821155A0:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x82115598
  if (!cr0.eq) goto loc_82115598;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 2))));
  // addi r3,r1,3120
  r3 = uint64_t(int64_t(int64_t(r1) + 3120));
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
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x821155C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r3,r1,3408
  r3 = uint64_t(int64_t(int64_t(r1) + 3408));
  // bl 0x820cc778
  // (left out: dead in C++)
  r3 = f820CC778(base, r1, r3, r4);
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x82115640
  if (!cr6.eq) goto loc_82115640;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // mr r30,r26
  r30 = r26;
loc_821155E4:
  // mr r31,r26
  r31 = r26;
loc_821155E8:
  // mr r5,r31
  r5 = r31;
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,3408
  r3 = uint64_t(int64_t(int64_t(r1) + 3408));
  // bl 0x820cc6a0
  // (left out: dead in C++)
  r3 = f820CC6A0(base, r3, r4, r5);
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82115608, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820cc480
  // (left out: dead in C++)
  r3 = f820CC480(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82115628
  if (!cr0.eq) goto loc_82115628;
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,5
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 5, so);
  // blt cr6,0x821155e8
  if (cr6.lt) goto loc_821155E8;
  // b 0x82115634
  goto loc_82115634;
loc_82115628:
  // lwz r10,104(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 104);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x8211564c
  if (!cr6.eq) goto loc_8211564C;
loc_82115634:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,8
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 8, so);
  // blt cr6,0x821155e4
  if (cr6.lt) goto loc_821155E4;
loc_82115640:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82115644:
  // addi r1,r1,7088
  r1 = uint64_t(int64_t(int64_t(r1) + 7088));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
loc_8211564C:
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // addi r29,r10,-61
  r29 = uint64_t(int64_t(int64_t(r10) + -61));
  // mr r27,r26
  r27 = r26;
  // mr r31,r26
  r31 = r26;
  // lwz r11,23712(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 23712);
  // lwz r11,8(r11)
  r11 = schema::DataTables(base, uint32_t(r11)).item_count_bits();
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x821156bc
  if (!cr0.gt) goto loc_821156BC;
loc_8211566C:
  // mr r3,r31
  r3 = r31;
  // bl 0x82117178
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Item(base, LIFTED_ARGS(0x82115674, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r5,3008
  r5 = uint64_t(int64_t(3008));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82115684, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,147(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 147);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x821156a0
  if (!cr6.eq) goto loc_821156A0;
  // lbz r11,146(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 146);
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmpw cr6,r11,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r29), so);
  // beq cr6,0x821156b8
  if (cr6.eq) goto loc_821156B8;
loc_821156A0:
  // lwz r11,23712(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 23712);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // lwz r11,8(r11)
  r11 = schema::DataTables(base, uint32_t(r11)).item_count_bits();
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x8211566c
  if (cr6.lt) goto loc_8211566C;
  // b 0x821156bc
  goto loc_821156BC;
loc_821156B8:
  // lwz r27,3060(r1)
  r27 = guest::LoadU32(base, uint32_t(r1) + 3060);
loc_821156BC:
  // lwz r11,3132(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 3132);
  // cmpw cr6,r27,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r11), so);
  // bne cr6,0x82115640
  if (!cr6.eq) goto loc_82115640;
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // stb r26,0(r28)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r28)).level_address(), uint8_t(r26));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // sth r26,8(r28)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r28)).first_value_address(), uint16_t(r26));
  // addi r9,r29,17
  r9 = uint64_t(int64_t(int64_t(r29) + 17));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r10,4(r28)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r28)).first_kind_address(), uint32_t(r10));
  // stb r11,10(r28)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r28)).first_set_address(), uint8_t(r11));
  // stw r10,12(r28)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r28)).second_kind_address(), uint32_t(r10));
  // sth r26,16(r28)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r28)).second_value_address(), uint16_t(r26));
  // stb r11,18(r28)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r28)).second_set_address(), uint8_t(r11));
  // stw r8,4(r28)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r28)).first_kind_address(), uint32_t(r8));
  // sth r9,8(r28)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r28)).first_value_address(), uint16_t(r9));
  // b 0x82115644
  goto loc_82115644;
}

// sub_82115708: lifted from generated/ by lift.py.
uint64_t f82115708(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82115708 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-4096(r1)
  ea = -4096 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r28,6
  r28 = uint64_t(int64_t(6));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // mr r25,r3
  r25 = r3;
  // mr r24,r5
  r24 = r5;
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // stw r28,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r28));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // stb r29,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r29));
  // stw r28,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r28));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r29,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r29));
  // lhz r11,6328(r25)
  r11 = guest::LoadU16(base, schema::CareerSave(base, uint32_t(r25)).garage_count_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8211581c
  if (cr6.eq) goto loc_8211581C;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x8211575C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r25,6120
  r11 = uint64_t(int64_t(int64_t(r25) + 6120));
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // ori r10,r10,39200
  r10 = r10 | 39200;
  // lwzx r10,r25,r10
  r10 = guest::LoadU32(base, uint32_t(r25) + uint32_t(r10));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lha r9,204(r11)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::CareerSave(base, uint32_t(r11) - 6120).garage_head_address()))));
  // b 0x82115780
  goto loc_82115780;
loc_82115778:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_82115780:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x82115778
  if (!cr0.eq) goto loc_82115778;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 2))));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
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
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x821157A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r3,r1,400
  r3 = uint64_t(int64_t(int64_t(r1) + 400));
  // bl 0x820cc778
  // (left out: dead in C++)
  r3 = f820CC778(base, r1, r3, r4);
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8211581c
  if (!cr6.eq) goto loc_8211581C;
  // mr r26,r30
  r26 = r30;
loc_821157C0:
  // mr r27,r30
  r27 = r30;
loc_821157C4:
  // mr r5,r27
  r5 = r27;
  // mr r4,r26
  r4 = r26;
  // addi r3,r1,400
  r3 = uint64_t(int64_t(int64_t(r1) + 400));
  // bl 0x820cc6a0
  // (left out: dead in C++)
  r3 = f820CC6A0(base, r3, r4, r5);
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x821157E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820cc480
  // (left out: dead in C++)
  r3 = f820CC480(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82115804
  if (!cr0.eq) goto loc_82115804;
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // cmpwi cr6,r27,5
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 5, so);
  // blt cr6,0x821157c4
  if (cr6.lt) goto loc_821157C4;
  // b 0x82115810
  goto loc_82115810;
loc_82115804:
  // lwz r11,104(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 104);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82115828
  if (!cr6.eq) goto loc_82115828;
loc_82115810:
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // cmpwi cr6,r26,8
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 8, so);
  // blt cr6,0x821157c0
  if (cr6.lt) goto loc_821157C0;
loc_8211581C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82115820:
  // addi r1,r1,4096
  r1 = uint64_t(int64_t(int64_t(r1) + 4096));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
loc_82115828:
  // cmpwi cr6,r11,131
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 131, so);
  // bne cr6,0x82115844
  if (!cr6.eq) goto loc_82115844;
  // lwz r11,124(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 124);
  // cmpwi cr6,r11,12
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 12, so);
  // bne cr6,0x8211581c
  if (!cr6.eq) goto loc_8211581C;
  // mr r11,r29
  r11 = r29;
  // b 0x8211585c
  goto loc_8211585C;
loc_82115844:
  // cmpwi cr6,r11,124
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 124, so);
  // bne cr6,0x8211581c
  if (!cr6.eq) goto loc_8211581C;
  // lwz r11,124(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 124);
  // cmpwi cr6,r11,21
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 21, so);
  // bne cr6,0x8211581c
  if (!cr6.eq) goto loc_8211581C;
  // mr r11,r30
  r11 = r30;
loc_8211585C:
  // lha r10,60(r25)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::CareerSave(base, uint32_t(r25)).stats_address() + 12))));
  // lha r9,62(r25)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::CareerSave(base, uint32_t(r25)).stats_address() + 14))));
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // bge cr6,0x821158ac
  if (!cr6.lt) goto loc_821158AC;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r28,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r28));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // li r10,68
  r10 = uint64_t(int64_t(68));
  // stb r29,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r29));
  // stw r28,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r28));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r29,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r29));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r11));
  // bne cr6,0x821158a0
  if (!cr6.eq) goto loc_821158A0;
  // li r10,65
  r10 = uint64_t(int64_t(65));
loc_821158A0:
  // sth r10,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r10));
loc_821158A4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82115820
  goto loc_82115820;
loc_821158AC:
  // cmpwi cr6,r10,200
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 200, so);
  // bge cr6,0x821158ec
  if (!cr6.lt) goto loc_821158EC;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r28,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r28));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // li r10,69
  r10 = uint64_t(int64_t(69));
  // stb r29,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r29));
  // stw r28,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r28));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r29,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r29));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r11));
  // bne cr6,0x821158a0
  if (!cr6.eq) goto loc_821158A0;
  // li r10,66
  r10 = uint64_t(int64_t(66));
  // b 0x821158a0
  goto loc_821158A0;
loc_821158EC:
  // cmpwi cr6,r10,300
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 300, so);
  // blt cr6,0x8211592c
  if (cr6.lt) goto loc_8211592C;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r28,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r28));
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // li r10,70
  r10 = uint64_t(int64_t(70));
  // stb r29,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r29));
  // stw r28,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r28));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r29,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r29));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r11));
  // bne cr6,0x821158a0
  if (!cr6.eq) goto loc_821158A0;
  // li r10,67
  r10 = uint64_t(int64_t(67));
  // b 0x821158a0
  goto loc_821158A0;
loc_8211592C:
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stb r30,80(r1)
  guest::StoreU8(base, uint32_t(r1) + 80, uint8_t(r30));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r28,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r28));
  // mr r5,r24
  r5 = r24;
  // sth r30,88(r1)
  guest::StoreU16(base, uint32_t(r1) + 88, uint16_t(r30));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stb r29,90(r1)
  guest::StoreU8(base, uint32_t(r1) + 90, uint8_t(r29));
  // mr r3,r25
  r3 = r25;
  // stb r29,98(r1)
  guest::StoreU8(base, uint32_t(r1) + 98, uint8_t(r29));
  // sth r11,96(r1)
  guest::StoreU16(base, uint32_t(r1) + 96, uint16_t(r11));
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // bl 0x82110ca8
  // (left out: dead in C++)
  r3 = f82110CA8(base, r1, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8211581c
  if (cr0.eq) goto loc_8211581C;
  // stb r30,0(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).level_address(), uint8_t(r30));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r28,4(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).first_kind_address(), uint32_t(r28));
  // mr r9,r31
  r9 = r31;
  // sth r30,8(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).first_value_address(), uint16_t(r30));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stb r29,10(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).first_set_address(), uint8_t(r29));
  // stw r28,12(r31)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r31)).second_kind_address(), uint32_t(r28));
  // sth r30,16(r31)
  guest::StoreU16(base, schema::CareerPick(base, uint32_t(r31)).second_value_address(), uint16_t(r30));
  // stb r29,18(r31)
  guest::StoreU8(base, schema::CareerPick(base, uint32_t(r31)).second_set_address(), uint8_t(r29));
  // mtctr r11
  ctr = r11;
loc_82115994:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r11,0(r9)
  guest::StoreU32(base, schema::CareerPick(base, uint32_t(r9)).head_address(), uint32_t(r11));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x82115994
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82115994;
  // b 0x821158a4
  goto loc_821158A4;
}

// sub_82119EA8 game::UiNode::CopyFields: lifted from generated/ by lift.py.
uint64_t f82119EA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82119EA8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // lis r9,20553
  r9 = uint64_t(int64_t(1346961408));
  // mr r30,r4
  r30 = r4;
  // ori r8,r9,17152
  r8 = r9 | 17152;
  // lis r9,20562
  r9 = uint64_t(int64_t(1347551232));
  // lis r10,18258
  r10 = uint64_t(int64_t(1196556288));
  // ori r7,r9,19712
  r7 = r9 | 19712;
  // lwz r11,0(r30)
  r11 = schema::UiMovie(base, uint32_t(r30)).file_0_bits();
  // ori r10,r10,20480
  r10 = r10 | 20480;
  // lis r9,21592
  r9 = uint64_t(int64_t(1415053312));
  // mr r31,r5
  r31 = r5;
  // ori r6,r9,21504
  r6 = r9 | 21504;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x82119f20
  if (cr6.eq) goto loc_82119F20;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // beq cr6,0x82119f20
  if (cr6.eq) goto loc_82119F20;
  // cmplw cr6,r11,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r7), so);
  // beq cr6,0x82119f20
  if (cr6.eq) goto loc_82119F20;
  // lis r10,21315
  r10 = uint64_t(int64_t(1396899840));
  // ori r10,r10,17664
  r10 = r10 | 17664;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x82119f20
  if (cr6.eq) goto loc_82119F20;
  // lis r10,21574
  r10 = uint64_t(int64_t(1413873664));
  // ori r10,r10,19712
  r10 = r10 | 19712;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x82119f20
  if (cr6.eq) goto loc_82119F20;
  // cmplw cr6,r11,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r6), so);
  // bne cr6,0x82119fd8
  if (!cr6.eq) goto loc_82119FD8;
loc_82119F20:
  // addi r5,r31,12
  r5 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r4,r30,12
  r4 = uint64_t(int64_t(int64_t(r30) + 12));
  // addi r29,r31,20
  r29 = uint64_t(int64_t(int64_t(r31) + 20));
  // addi r28,r30,20
  r28 = uint64_t(int64_t(int64_t(r30) + 20));
  // addi r27,r31,28
  r27 = uint64_t(int64_t(int64_t(r31) + 28));
  // lwz r3,0(r5)
  r3 = schema::UiMovie(base, uint32_t(r5) - 12).texture_set_bits();
  // addi r26,r30,28
  r26 = uint64_t(int64_t(int64_t(r30) + 28));
  // addi r10,r31,40
  r10 = uint64_t(int64_t(int64_t(r31) + 40));
  // addi r9,r30,40
  r9 = uint64_t(int64_t(int64_t(r30) + 40));
  // addi r25,r31,56
  r25 = uint64_t(int64_t(int64_t(r31) + 56));
  // stw r3,0(r4)
  schema::UiMovie(base, uint32_t(r4) - 12).set_texture_set_bits(uint32_t(r3));
  // addi r3,r30,56
  r3 = uint64_t(int64_t(int64_t(r30) + 56));
  // lwz r5,4(r5)
  r5 = schema::UiMovie(base, uint32_t(r5) - 12).live_scene_bits();
  // stw r5,4(r4)
  schema::UiMovie(base, uint32_t(r4) - 12).set_live_scene_bits(uint32_t(r5));
  // lwz r5,0(r29)
  r5 = schema::UiMovie(base, uint32_t(r29) - 20).saved_scene_bits();
  // stw r5,0(r28)
  schema::UiMovie(base, uint32_t(r28) - 20).set_saved_scene_bits(uint32_t(r5));
  // lwz r5,4(r29)
  r5 = schema::UiMovie(base, uint32_t(r29) - 20).file_scene_bits();
  // stw r5,4(r28)
  schema::UiMovie(base, uint32_t(r28) - 20).set_file_scene_bits(uint32_t(r5));
  // lwz r5,0(r27)
  r5 = schema::UiMovie(base, uint32_t(r27) - 28).textures_bits();
  // stw r5,0(r26)
  schema::UiMovie(base, uint32_t(r26) - 28).set_textures_bits(uint32_t(r5));
  // lwz r5,4(r27)
  r5 = schema::UiMovie(base, uint32_t(r27) - 28).texture_capacity_bits();
  // stw r5,4(r26)
  schema::UiMovie(base, uint32_t(r26) - 28).set_texture_capacity_bits(uint32_t(r5));
  // lfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.u32 = schema::UiMovie(base, uint32_t(r31)).texture_count_bits();
  f0 = double(temp.f32);
  // stfs f0,36(r30)
  temp.f32 = float(f0);
  schema::UiMovie(base, uint32_t(r30)).set_texture_count_bits(temp.u32);
  // lwz r5,0(r10)
  r5 = schema::UiMovie(base, uint32_t(r10) - 40).text_items_bits();
  // stw r5,0(r9)
  schema::UiMovie(base, uint32_t(r9) - 40).set_text_items_bits(uint32_t(r5));
  // lwz r5,4(r10)
  r5 = schema::UiMovie(base, uint32_t(r10) - 40).text_item_capacity_bits();
  // stw r5,4(r9)
  schema::UiMovie(base, uint32_t(r9) - 40).set_text_item_capacity_bits(uint32_t(r5));
  // lwz r5,8(r10)
  r5 = schema::UiMovie(base, uint32_t(r10) - 40).text_item_count_bits();
  // stw r5,8(r9)
  schema::UiMovie(base, uint32_t(r9) - 40).set_text_item_count_bits(uint32_t(r5));
  // lwz r10,12(r10)
  r10 = schema::UiMovie(base, uint32_t(r10) - 40).unknown_52_bits();
  // stw r10,12(r9)
  schema::UiMovie(base, uint32_t(r9) - 40).set_unknown_52_bits(uint32_t(r10));
  // lwz r10,0(r25)
  r10 = schema::UiMovie(base, uint32_t(r25) - 56).unknown_56_bits();
  // stw r10,0(r3)
  schema::UiMovie(base, uint32_t(r3) - 56).set_unknown_56_bits(uint32_t(r10));
  // lwz r10,4(r25)
  r10 = schema::UiMovie(base, uint32_t(r25) - 56).unknown_60_bits();
  // stw r10,4(r3)
  schema::UiMovie(base, uint32_t(r3) - 56).set_unknown_60_bits(uint32_t(r10));
  // lwz r10,8(r25)
  r10 = uint32_t(schema::UiMovie(base, uint32_t(r25) - 56).load_at(64, 4));
  // stw r10,8(r3)
  schema::UiMovie(base, uint32_t(r3) - 56).store_at(64, 4, uint32_t(r10));
  // lfs f0,68(r31)
  temp.u32 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(68, 4));
  f0 = double(temp.f32);
  // stfs f0,68(r30)
  temp.f32 = float(f0);
  schema::UiMovie(base, uint32_t(r30)).store_at(68, 4, temp.u32);
  // lwz r10,72(r31)
  r10 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(72, 4));
  // stw r10,72(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(72, 4, uint32_t(r10));
  // lwz r10,76(r31)
  r10 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(76, 4));
  // stw r10,76(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(76, 4, uint32_t(r10));
  // lwz r10,80(r31)
  r10 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(80, 4));
  // stw r10,80(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(80, 4, uint32_t(r10));
loc_82119FD8:
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // beq cr6,0x8211a0cc
  if (cr6.eq) goto loc_8211A0CC;
  // cmplw cr6,r11,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r7), so);
  // beq cr6,0x8211a024
  if (cr6.eq) goto loc_8211A024;
  // cmplw cr6,r11,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r6), so);
  // bne cr6,0x8211a124
  if (!cr6.eq) goto loc_8211A124;
  // lfs f0,100(r31)
  lifted::ScalarFloatMode();
  temp.u32 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(100, 4));
  f0 = double(temp.f32);
  // stfs f0,100(r30)
  temp.f32 = float(f0);
  schema::UiMovie(base, uint32_t(r30)).store_at(100, 4, temp.u32);
  // lfs f0,104(r31)
  temp.u32 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(104, 4));
  f0 = double(temp.f32);
  // stfs f0,104(r30)
  temp.f32 = float(f0);
  schema::UiMovie(base, uint32_t(r30)).store_at(104, 4, temp.u32);
  // lfs f0,116(r31)
  temp.u32 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(116, 4));
  f0 = double(temp.f32);
  // stfs f0,116(r30)
  temp.f32 = float(f0);
  schema::UiMovie(base, uint32_t(r30)).store_at(116, 4, temp.u32);
  // lfs f0,120(r31)
  temp.u32 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(120, 4));
  f0 = double(temp.f32);
  // stfs f0,120(r30)
  temp.f32 = float(f0);
  schema::UiMovie(base, uint32_t(r30)).store_at(120, 4, temp.u32);
  // lwz r11,124(r31)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(124, 4));
  // stw r11,124(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(124, 4, uint32_t(r11));
  // lwz r11,128(r31)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(128, 4));
  // stw r11,128(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(128, 4, uint32_t(r11));
  // b 0x8211a114
  goto loc_8211A114;
loc_8211A024:
  // lwz r11,100(r31)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(100, 4));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // stw r11,100(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(100, 4, uint32_t(r11));
  // lwz r11,104(r31)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(104, 4));
  // stw r11,104(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(104, 4, uint32_t(r11));
  // lwz r11,108(r31)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(108, 4));
  // stw r11,108(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(108, 4, uint32_t(r11));
  // lwz r11,116(r31)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(116, 4));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8211a124
  if (!cr6.gt) goto loc_8211A124;
loc_8211A04C:
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // bl 0x8259d628
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D628(base, LIFTED_ARGS(0x8211A058, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d628
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D628(base, LIFTED_ARGS(0x8211A068, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,0(r3)
  r9 = guest::LoadU32(base, schema::UiMovieVertex(base, uint32_t(r3)).x_address());
  // addi r8,r3,8
  r8 = uint64_t(int64_t(int64_t(r3) + 8));
  // addi r11,r29,8
  r11 = uint64_t(int64_t(int64_t(r29) + 8));
  // addi r7,r3,24
  r7 = uint64_t(int64_t(int64_t(r3) + 24));
  // addi r10,r29,24
  r10 = uint64_t(int64_t(int64_t(r29) + 24));
  // stw r9,0(r29)
  guest::StoreU32(base, schema::UiMovieVertex(base, uint32_t(r29)).x_address(), uint32_t(r9));
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // lwz r9,4(r3)
  r9 = guest::LoadU32(base, schema::UiMovieVertex(base, uint32_t(r3)).y_address());
  // stw r9,4(r29)
  guest::StoreU32(base, schema::UiMovieVertex(base, uint32_t(r29)).y_address(), uint32_t(r9));
  // lwz r9,0(r8)
  r9 = guest::LoadU32(base, schema::UiMovieVertex(base, uint32_t(r8) - 8).red_address());
  // stw r9,0(r11)
  guest::StoreU32(base, schema::UiMovieVertex(base, uint32_t(r11) - 8).red_address(), uint32_t(r9));
  // lwz r9,4(r8)
  r9 = guest::LoadU32(base, schema::UiMovieVertex(base, uint32_t(r8) - 8).green_address());
  // stw r9,4(r11)
  guest::StoreU32(base, schema::UiMovieVertex(base, uint32_t(r11) - 8).green_address(), uint32_t(r9));
  // lwz r9,8(r8)
  r9 = guest::LoadU32(base, schema::UiMovieVertex(base, uint32_t(r8) - 8).blue_address());
  // stw r9,8(r11)
  guest::StoreU32(base, schema::UiMovieVertex(base, uint32_t(r11) - 8).blue_address(), uint32_t(r9));
  // lfs f0,20(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::UiMovieVertex(base, uint32_t(r3)).alpha_address());
  f0 = double(temp.f32);
  // stfs f0,20(r29)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::UiMovieVertex(base, uint32_t(r29)).alpha_address(), temp.u32);
  // lwz r11,0(r7)
  r11 = guest::LoadU32(base, schema::UiMovieVertex(base, uint32_t(r7) - 24).unknown_24_address());
  // stw r11,0(r10)
  guest::StoreU32(base, schema::UiMovieVertex(base, uint32_t(r10) - 24).unknown_24_address(), uint32_t(r11));
  // lwz r11,4(r7)
  r11 = guest::LoadU32(base, schema::UiMovieVertex(base, uint32_t(r7) - 24).unknown_28_address());
  // stw r11,4(r10)
  guest::StoreU32(base, schema::UiMovieVertex(base, uint32_t(r10) - 24).unknown_28_address(), uint32_t(r11));
  // lwz r11,116(r31)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(116, 4));
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // blt cr6,0x8211a04c
  if (cr6.lt) goto loc_8211A04C;
  // b 0x8211a124
  goto loc_8211A124;
loc_8211A0CC:
  // lfs f0,100(r31)
  lifted::ScalarFloatMode();
  temp.u32 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(100, 4));
  f0 = double(temp.f32);
  // addi r11,r31,116
  r11 = uint64_t(int64_t(int64_t(r31) + 116));
  // stfs f0,100(r30)
  temp.f32 = float(f0);
  schema::UiMovie(base, uint32_t(r30)).store_at(100, 4, temp.u32);
  // addi r10,r30,116
  r10 = uint64_t(int64_t(int64_t(r30) + 116));
  // lfs f0,104(r31)
  temp.u32 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(104, 4));
  f0 = double(temp.f32);
  // stfs f0,104(r30)
  temp.f32 = float(f0);
  schema::UiMovie(base, uint32_t(r30)).store_at(104, 4, temp.u32);
  // lwz r9,108(r31)
  r9 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(108, 4));
  // stw r9,108(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(108, 4, uint32_t(r9));
  // lwz r9,112(r31)
  r9 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(112, 4));
  // stw r9,112(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(112, 4, uint32_t(r9));
  // lwz r9,0(r11)
  r9 = uint32_t(schema::UiMovie(base, uint32_t(r11) - 116).load_at(116, 4));
  // stw r9,0(r10)
  schema::UiMovie(base, uint32_t(r10) - 116).store_at(116, 4, uint32_t(r9));
  // lwz r9,4(r11)
  r9 = uint32_t(schema::UiMovie(base, uint32_t(r11) - 116).load_at(120, 4));
  // stw r9,4(r10)
  schema::UiMovie(base, uint32_t(r10) - 116).store_at(120, 4, uint32_t(r9));
  // lwz r9,8(r11)
  r9 = uint32_t(schema::UiMovie(base, uint32_t(r11) - 116).load_at(124, 4));
  // stw r9,8(r10)
  schema::UiMovie(base, uint32_t(r10) - 116).store_at(124, 4, uint32_t(r9));
  // lwz r11,12(r11)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r11) - 116).load_at(128, 4));
  // stw r11,12(r10)
  schema::UiMovie(base, uint32_t(r10) - 116).store_at(128, 4, uint32_t(r11));
loc_8211A114:
  // lwz r11,132(r31)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(132, 4));
  // stw r11,132(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(132, 4, uint32_t(r11));
  // lwz r11,136(r31)
  r11 = uint32_t(schema::UiMovie(base, uint32_t(r31)).load_at(136, 4));
  // stw r11,136(r30)
  schema::UiMovie(base, uint32_t(r30)).store_at(136, 4, uint32_t(r11));
loc_8211A124:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return r3;
}

// sub_8211A130 game::UiMovie::ResetNodeFields: lifted from generated/ by lift.py.
uint64_t f8211A130(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8211A130 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r5
  r30 = r5;
  // cmpwi cr6,r29,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), -1, so);
  // lwz r3,24(r31)
  r3 = schema::UiMovie(base, uint32_t(r31)).file_scene_bits();
  // beq cr6,0x8211a18c
  if (cr6.eq) goto loc_8211A18C;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211A158, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211A160, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r28,r3
  r28 = r3;
  // lwz r3,16(r31)
  r3 = schema::UiMovie(base, uint32_t(r31)).live_scene_bits();
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211A170, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211A178, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r29,r3
  r29 = r3;
  // lwz r3,20(r31)
  r3 = schema::UiMovie(base, uint32_t(r31)).saved_scene_bits();
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211A188, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8211a1ac
  goto loc_8211A1AC;
loc_8211A18C:
  // mr r4,r30
  r4 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211A194, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r28,r3
  r28 = r3;
  // lwz r3,16(r31)
  r3 = schema::UiMovie(base, uint32_t(r31)).live_scene_bits();
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211A1A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r3,20(r31)
  r3 = schema::UiMovie(base, uint32_t(r31)).saved_scene_bits();
loc_8211A1AC:
  // mr r4,r30
  r4 = r30;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Child(base, LIFTED_ARGS(0x8211A1B4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r5,r28
  r5 = r28;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x82119ea8
  // (left out: dead in C++)
  r3 = game::UiNode::CopyFields(base, r1, r13, r3, r4, r5);
  // mr r5,r28
  r5 = r28;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82119ea8
  // (left out: dead in C++)
  r3 = game::UiNode::CopyFields(base, r1, r13, r3, r4, r5);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82148768: lifted from generated/ by lift.py.
uint64_t f82148768(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82148768 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, ctr = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r11,2(r3)
  r11 = guest::LoadU8(base, schema::SysLinkPacket(base, uint32_t(r3)).unknown_2_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // cmplwi cr6,r11,26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 26, so);
  // bgtlr cr6
  if (cr6.gt) return r3;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-28920
  r12 = uint64_t(int64_t(int64_t(r12) + -28920));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // lis r12,-32235
  r12 = uint64_t(int64_t(-2112552960));
  // addi r12,r12,-30816
  r12 = uint64_t(int64_t(int64_t(r12) + -30816));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_821487A0;
  return r3;
  case 1:
  goto loc_821487A8;
  case 2:
  goto loc_821487B0;
  case 3:
  goto loc_821487B8;
  case 4:
  goto loc_8214880C;
  case 5:
  goto loc_8214880C;
  case 6:
  goto loc_8214880C;
  case 7:
  goto loc_821487B8;
  case 8:
  goto loc_8214880C;
  case 9:
  goto loc_8214880C;
  case 10:
  goto loc_821487C0;
  case 11:
  goto loc_821487C8;
  case 12:
  goto loc_821487D0;
  case 13:
  goto loc_821487D8;
  case 14:
  goto loc_821487E0;
  case 15:
  goto loc_821487E8;
  case 16:
  goto loc_8214880C;
  case 17:
  goto loc_8214880C;
  case 18:
  goto loc_8214880C;
  case 19:
  goto loc_8214880C;
  case 20:
  goto loc_8214880C;
  case 21:
  goto loc_8214880C;
  case 22:
  goto loc_8214880C;
  case 23:
  goto loc_821487F0;
  case 24:
  goto loc_821487F8;
  case 25:
  goto loc_82148800;
  case 26:
  goto loc_82148808;
  default:
  std::abort();
  }
loc_821487A8:
  // li r3,282
  r3 = uint64_t(int64_t(282));
  // blr
  return r3;
loc_821487B0:
  // li r3,250
  r3 = uint64_t(int64_t(250));
  // blr
  return r3;
loc_821487B8:
  // li r3,17
  r3 = uint64_t(int64_t(17));
  // blr
  return r3;
loc_821487C0:
  // li r3,9
  r3 = uint64_t(int64_t(9));
  // blr
  return r3;
loc_821487C8:
  // li r3,302
  r3 = uint64_t(int64_t(302));
  // blr
  return r3;
loc_821487D0:
  // li r3,73
  r3 = uint64_t(int64_t(73));
  // blr
  return r3;
loc_821487D8:
  // li r3,781
  r3 = uint64_t(int64_t(781));
  // blr
  return r3;
loc_821487E0:
  // li r3,497
  r3 = uint64_t(int64_t(497));
  // blr
  return r3;
loc_821487E8:
  // li r3,657
  r3 = uint64_t(int64_t(657));
  // blr
  return r3;
loc_821487F0:
  // li r3,7
  r3 = uint64_t(int64_t(7));
  // blr
  return r3;
loc_821487F8:
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // blr
  return r3;
loc_82148800:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // blr
  return r3;
loc_82148808:
  // li r3,5
  r3 = uint64_t(int64_t(5));
loc_8214880C:
  // blr
  return r3;
loc_frag_821487A0:
  // li r3,258
  r3 = uint64_t(int64_t(258));
  // blr
  return r3;
}

// sub_82148F80: lifted from generated/ by lift.py.
lifted::Result f82148F80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82148F80 (lifted)");
  double f1 = 0.0;
  // b 0x822952c8
  { const lifted::Result o = game::XShowFriendsUI(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82148F88: lifted from generated/ by lift.py.
void f82148F88(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82148F88 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r11,2(r3)
  r11 = guest::LoadU8(base, schema::SysLinkPacket(base, uint32_t(r3)).unknown_2_address());
  // cmplwi cr6,r11,18
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 18, so);
  // bne cr6,0x82148fa0
  if (!cr6.eq) goto loc_82148FA0;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27384(r10)
  guest::StoreU32(base, uint32_t(r10) + 27384, uint32_t(r11));
loc_82148FA0:
  // lbz r11,2(r3)
  r11 = guest::LoadU8(base, schema::SysLinkPacket(base, uint32_t(r3)).unknown_2_address());
  // cmplwi cr6,r11,19
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 19, so);
  // bne cr6,0x82148fb8
  if (!cr6.eq) goto loc_82148FB8;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27388(r10)
  guest::StoreU32(base, uint32_t(r10) + 27388, uint32_t(r11));
loc_82148FB8:
  // lbz r11,2(r3)
  r11 = guest::LoadU8(base, schema::SysLinkPacket(base, uint32_t(r3)).unknown_2_address());
  // cmplwi cr6,r11,20
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 20, so);
  // bnelr cr6
  if (!cr6.eq) return;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27392(r10)
  guest::StoreU32(base, uint32_t(r10) + 27392, uint32_t(r11));
  // blr
  return;
}

// sub_82148FD8: lifted from generated/ by lift.py.
lifted::Result f82148FD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82148FD8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r3
  r11 = r3;
  // addi r31,r10,30752
  r31 = uint64_t(int64_t(int64_t(r10) + 30752));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // addi r10,r10,30320
  r10 = uint64_t(int64_t(int64_t(r10) + 30320));
  // lis r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // ori r4,r4,32778
  r4 = r4 | 32778;
  // lwz r5,28180(r30)
  r5 = guest::LoadU32(base, uint32_t(r30) + 28180);
  // stw r11,292(r10)
  guest::StoreU32(base, uint32_t(r10) + 292, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bl 0x82294f70
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserSetContextEx(base, LIFTED_ARGS(0x82149024, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // lis r4,0
  r4 = uint64_t(int64_t(0));
  // ori r4,r4,32779
  r4 = r4 | 32779;
  // lwz r5,29304(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 29304);
  // bl 0x82294f70
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserSetContextEx(base, LIFTED_ARGS(0x8214903C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,46
  r11 = uint64_t(int64_t(46));
  // stw r11,80(r31)
  guest::StoreU32(base, uint32_t(r31) + 80, uint32_t(r11));
  // lwz r11,28180(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82149058
  if (!cr6.eq) goto loc_82149058;
  // li r11,62
  r11 = uint64_t(int64_t(62));
  // stw r11,80(r31)
  guest::StoreU32(base, uint32_t(r31) + 80, uint32_t(r11));
loc_82149058:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8214ed30
  // (left out: dead in C++)
  { const lifted::Result o = f8214ED30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_821492B8: lifted from generated/ by lift.py.
lifted::Result f821492B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821492B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // lwz r3,27348(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 27348);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x821492e4
  if (cr6.eq) goto loc_821492E4;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x821492DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,27348(r29)
  guest::StoreU32(base, uint32_t(r29) + 27348, uint32_t(r11));
loc_821492E4:
  // lbz r11,12(r30)
  r11 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_12_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82149300
  if (cr0.eq) goto loc_82149300;
  // addi r4,r30,144
  r4 = uint64_t(int64_t(int64_t(r30) + 144));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // rlwinm r5,r11,3,0,28
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // bl 0x825a7830
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7830(base, LIFTED_ARGS(0x82149300, r31)); r3 = o.r3; f1 = o.f1; }
loc_82149300:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r31,r11,25616
  r31 = uint64_t(int64_t(int64_t(r11) + 25616));
  // lis r11,-1
  r11 = uint64_t(int64_t(-65536));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r7,r31
  r7 = r31;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // li r11,63
  r11 = uint64_t(int64_t(63));
  // sth r11,8(r31)
  guest::StoreU16(base, uint32_t(r31) + 8, uint16_t(r11));
  // li r11,64
  r11 = uint64_t(int64_t(64));
  // sth r11,10(r31)
  guest::StoreU16(base, uint32_t(r31) + 10, uint16_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lbz r4,12(r30)
  r4 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_12_address());
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x822950c0
  // (left out: dead in C++)
  { const lifted::Result o = f822950C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,122
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 122, so);
  // bne cr6,0x82149424
  if (!cr6.eq) goto loc_82149424;
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x82149364, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r9,r3
  r9 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r7,r31
  r7 = r31;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r9,27348(r29)
  guest::StoreU32(base, uint32_t(r29) + 27348, uint32_t(r9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lbz r4,12(r30)
  r4 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_12_address());
  // bl 0x822950c0
  // (left out: dead in C++)
  { const lifted::Result o = f822950C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82149424
  if (!cr0.eq) goto loc_82149424;
  // lbz r11,12(r30)
  r11 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_12_address());
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82149424
  if (cr0.eq) goto loc_82149424;
  // lwz r11,27348(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 27348);
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // lis r7,-32255
  r7 = uint64_t(int64_t(-2113863680));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r8,4(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lfd f11,8784(r6)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r6) + 8784)));
  // lfd f12,-31160(r7)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r7) + -31160)));
  // addi r11,r30,192
  r11 = uint64_t(int64_t(int64_t(r30) + 192));
  // lfd f13,8792(r9)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r9) + 8792)));
loc_821493CC:
  // lwz r9,12(r8)
  r9 = guest::LoadU32(base, uint32_t(r8) + 12);
  // add r9,r9,r10
  r9 = r9 + r10;
  // lwz r9,44(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 44);
  // lfd f0,16(r9)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r9) + 16)));
  // stfd f0,0(r11)
  guest::StoreU64(base, schema::SysLinkRecord(base, uint32_t(r11) - 192).unknown_192_address(), std::bit_cast<uint64_t>(f0));
  // lwz r9,12(r8)
  r9 = guest::LoadU32(base, uint32_t(r8) + 12);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // add r9,r9,r10
  r9 = r9 + r10;
  // lwz r9,44(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 44);
  // lfd f0,40(r9)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r9) + 40)));
  // stfd f0,32(r11)
  guest::StoreU64(base, schema::SysLinkRecord(base, uint32_t(r11) - 192).unknown_224_address(), std::bit_cast<uint64_t>(f0));
  // bne cr6,0x8214940c
  if (!cr6.eq) goto loc_8214940C;
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bne cr6,0x8214940c
  if (!cr6.eq) goto loc_8214940C;
  // stfd f12,0(r11)
  guest::StoreU64(base, schema::SysLinkRecord(base, uint32_t(r11) - 192).unknown_192_address(), std::bit_cast<uint64_t>(f12));
  // stfd f11,32(r11)
  guest::StoreU64(base, schema::SysLinkRecord(base, uint32_t(r11) - 192).unknown_224_address(), std::bit_cast<uint64_t>(f11));
loc_8214940C:
  // lbz r9,12(r30)
  r9 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_12_address());
  // addi r5,r5,1
  r5 = uint64_t(int64_t(int64_t(r5) + 1));
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // cmplw cr6,r5,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r9), so);
  // blt cr6,0x821493cc
  if (cr6.lt) goto loc_821493CC;
loc_82149424:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82149430: lifted from generated/ by lift.py.
lifted::Result f82149430(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82149430 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r11,r4,18
  r11 = uint64_t(int64_t(int64_t(r4) + 18));
  // addi r10,r4,64
  r10 = uint64_t(int64_t(int64_t(r4) + 64));
  // rlwinm r26,r11,3,0,28
  r26 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // addi r11,r11,26112
  r11 = uint64_t(int64_t(int64_t(r11) + 26112));
  // ori r10,r10,32778
  r10 = r10 | 32778;
  // mr r30,r3
  r30 = r3;
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // lwzx r10,r9,r30
  r10 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r30));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r10,16(r11)
  guest::StoreU32(base, uint32_t(r11) + 16, uint32_t(r10));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stb r10,8(r11)
  guest::StoreU8(base, uint32_t(r11) + 8, uint8_t(r10));
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // ori r10,r10,32779
  r10 = r10 | 32779;
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r10,28180(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 28180);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // ldx r10,r26,r30
  r10 = guest::LoadU64(base, uint32_t(r26) + uint32_t(r30));
  // stb r9,32(r11)
  guest::StoreU8(base, uint32_t(r11) + 32, uint8_t(r9));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // addi r31,r9,26168
  r31 = uint64_t(int64_t(int64_t(r9) + 26168));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r9,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r9));
  // lis r9,-1
  r9 = uint64_t(int64_t(-65536));
  // stw r9,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r9));
  // rldicl r9,r10,32,32
  r9 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFF;
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // rotlwi r9,r9,0
  r9 = __builtin_rotateleft32(uint32_t(r9), 0);
  // xor r10,r9,r10
  r10 = r9 ^ r10;
  // stw r10,40(r11)
  guest::StoreU32(base, uint32_t(r11) + 40, uint32_t(r10));
  // beq cr6,0x821494e0
  if (cr6.eq) goto loc_821494E0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x821495a0
  if (!cr6.eq) goto loc_821495A0;
loc_821494E0:
  // lbz r11,27410(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 27410);
  // lis r8,8192
  r8 = uint64_t(int64_t(536870912));
  // li r27,2
  r27 = uint64_t(int64_t(2));
  // rotlwi r10,r11,2
  r10 = __builtin_rotateleft32(uint32_t(r11), 2);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // ori r8,r8,5
  r8 = r8 | 5;
  // addi r9,r11,25284
  r9 = uint64_t(int64_t(int64_t(r11) + 25284));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,26016
  r11 = uint64_t(int64_t(int64_t(r11) + 26016));
  // lwzx r9,r10,r9
  r9 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // stw r8,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r8));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // std r8,16(r11)
  guest::StoreU64(base, uint32_t(r11) + 16, r8);
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // stb r8,8(r11)
  guest::StoreU8(base, uint32_t(r11) + 8, uint8_t(r8));
  // lis r8,8192
  r8 = uint64_t(int64_t(536870912));
  // ori r8,r8,4
  r8 = r8 | 4;
  // stw r8,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r8));
  // beq cr6,0x82149538
  if (cr6.eq) goto loc_82149538;
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_82149538:
  // addi r8,r4,79
  r8 = uint64_t(int64_t(int64_t(r4) + 79));
  // std r9,40(r11)
  guest::StoreU64(base, uint32_t(r11) + 40, r9);
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // rlwinm r7,r8,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // addi r9,r9,26160
  r9 = uint64_t(int64_t(int64_t(r9) + 26160));
  // stb r8,32(r11)
  guest::StoreU8(base, uint32_t(r11) + 32, uint8_t(r8));
  // lis r8,8192
  r8 = uint64_t(int64_t(536870912));
  // lwzx r10,r10,r9
  r10 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // ori r8,r8,8
  r8 = r8 | 8;
  // stw r8,48(r11)
  guest::StoreU32(base, uint32_t(r11) + 48, uint32_t(r8));
  // std r10,64(r11)
  guest::StoreU64(base, uint32_t(r11) + 64, r10);
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stb r10,56(r11)
  guest::StoreU8(base, uint32_t(r11) + 56, uint8_t(r10));
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // ori r10,r10,11
  r10 = r10 | 11;
  // stw r10,72(r11)
  guest::StoreU32(base, uint32_t(r11) + 72, uint32_t(r10));
  // lwzx r10,r7,r30
  r10 = guest::LoadU32(base, uint32_t(r7) + uint32_t(r30));
  // stw r10,88(r11)
  guest::StoreU32(base, uint32_t(r11) + 88, uint32_t(r10));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stb r10,80(r11)
  guest::StoreU8(base, uint32_t(r11) + 80, uint8_t(r10));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r10,16(r31)
  guest::StoreU32(base, uint32_t(r31) + 16, uint32_t(r10));
  // li r10,18
  r10 = uint64_t(int64_t(18));
  // stw r10,12(r31)
  guest::StoreU32(base, uint32_t(r31) + 12, uint32_t(r10));
  // stw r11,20(r31)
  guest::StoreU32(base, uint32_t(r31) + 20, uint32_t(r11));
loc_821495A0:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // addi r29,r11,24704
  r29 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x821495B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r7,r29
  r7 = r29;
  // ldx r4,r26,r30
  r4 = guest::LoadU64(base, uint32_t(r26) + uint32_t(r30));
  // mr r6,r31
  r6 = r31;
  // mr r5,r27
  r5 = r27;
  // lwz r3,30752(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 30752);
  // bl 0x825766e0
  // (left out: dead in C++)
  { const lifted::Result o = f825766E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lbz r11,27410(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 27410);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stb r11,27410(r28)
  guest::StoreU8(base, uint32_t(r28) + 27410, uint8_t(r11));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821496F8: lifted from generated/ by lift.py.
void f821496F8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821496F8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).container_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82149708
  if (!cr6.eq) goto loc_82149708;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149708:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address());
  // lbz r11,25(r10)
  r11 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).is_nil_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82149730
  if (cr6.eq) goto loc_82149730;
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).right_address());
  // stw r11,4(r3)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address(), uint32_t(r11));
  // lbz r11,25(r11)
  r11 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).is_nil_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // b 0x8214979c
  goto loc_8214979C;
loc_82149730:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).left_address());
  // lbz r9,25(r11)
  r9 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).is_nil_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8214974c
  if (cr6.eq) goto loc_8214974C;
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address());
  // b 0x82149780
  goto loc_82149780;
loc_82149748:
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
loc_8214974C:
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
  // lbz r10,25(r10)
  r10 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).is_nil_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82149748
  if (cr6.eq) goto loc_82149748;
loc_8214975C:
  // stw r11,4(r3)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address(), uint32_t(r11));
  // blr
  return;
loc_82149764:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address());
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8214978c
  if (!cr6.eq) goto loc_8214978C;
  // stw r11,4(r3)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address(), uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
loc_82149780:
  // lbz r10,25(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 25);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82149764
  if (cr6.eq) goto loc_82149764;
loc_8214978C:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address());
  // lbz r10,25(r10)
  r10 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).is_nil_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214975c
  if (cr6.eq) goto loc_8214975C;
loc_8214979C:
  // twi 31,r0,22
  lifted::Trap(22);
  // blr
  return;
}

// sub_82149830: lifted from generated/ by lift.py.
uint64_t f82149830(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82149830 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r3)).head_address());
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address());
  // lbz r9,25(r11)
  r9 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).is_nil_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x82149880
  if (!cr6.eq) goto loc_82149880;
  // ld r8,0(r4)
  r8 = guest::LoadU64(base, schema::SysLinkId(base, uint32_t(r4)).id_address());
loc_82149848:
  // ld r9,16(r11)
  r9 = guest::LoadU64(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).id_address());
  // cmpld cr6,r9,r8
  cr6 = lifted::Compare<uint64_t>(r9, r8, so);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // blt cr6,0x8214985c
  if (cr6.lt) goto loc_8214985C;
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_8214985C:
  // clrlwi. r9,r9,24
  r9 = uint32_t(r9) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x8214986c
  if (cr0.eq) goto loc_8214986C;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
  // b 0x82149874
  goto loc_82149874;
loc_8214986C:
  // mr r10,r11
  r10 = r11;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
loc_82149874:
  // lbz r9,25(r11)
  r9 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).is_nil_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x82149848
  if (cr6.eq) goto loc_82149848;
loc_82149880:
  // mr r3,r10
  r3 = r10;
  // blr
  return r3;
}

// sub_82149960: lifted from generated/ by lift.py.
uint64_t f82149960(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82149960 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r3)).head_address());
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address());
  // lbz r9,25(r11)
  r9 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).is_nil_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x821499b0
  if (!cr6.eq) goto loc_821499B0;
  // ld r8,0(r4)
  r8 = guest::LoadU64(base, schema::SysLinkId(base, uint32_t(r4)).id_address());
loc_82149978:
  // ld r9,16(r11)
  r9 = guest::LoadU64(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).id_address());
  // cmpld cr6,r8,r9
  cr6 = lifted::Compare<uint64_t>(r8, r9, so);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // blt cr6,0x8214998c
  if (cr6.lt) goto loc_8214998C;
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_8214998C:
  // clrlwi. r9,r9,24
  r9 = uint32_t(r9) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x821499a0
  if (cr0.eq) goto loc_821499A0;
  // mr r10,r11
  r10 = r11;
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
  // b 0x821499a4
  goto loc_821499A4;
loc_821499A0:
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
loc_821499A4:
  // lbz r9,25(r11)
  r9 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).is_nil_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x82149978
  if (cr6.eq) goto loc_82149978;
loc_821499B0:
  // mr r3,r10
  r3 = r10;
  // blr
  return r3;
}

// sub_821499B8: lifted from generated/ by lift.py.
lifted::Result f821499B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821499B8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82149a10
  if (cr6.eq) goto loc_82149A10;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // twllei r31,0
  if (int32_t(r31) == 0 || uint32_t(r31) < 0u) lifted::Trap(0);
  // divwu r11,r11,r31
  r11 = uint32_t(uint32_t(r31) ? uint32_t(r11) / uint32_t(r31) : 0);
  // cmplwi cr6,r11,384
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 384, so);
  // bge cr6,0x82149a10
  if (!cr6.lt) goto loc_82149A10;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,3276
  r11 = uint64_t(int64_t(int64_t(r11) + 3276));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82091200
  // (left out: dead in C++)
  { const lifted::Result o = f82091200(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3264
  r11 = uint64_t(int64_t(int64_t(r11) + 3264));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
loc_82149A10:
  // mulli r3,r31,384
  r3 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(384))));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82149A18, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82149B68: lifted from generated/ by lift.py.
uint64_t f82149B68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82149B68 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // std r3,112(r1)
  guest::StoreU64(base, uint32_t(r1) + 112, r3);
  // mr r31,r5
  r31 = r5;
  // std r4,120(r1)
  guest::StoreU64(base, uint32_t(r1) + 120, r4);
loc_82149B84:
  // lwz r11,112(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 112);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82149b9c
  if (cr6.eq) goto loc_82149B9C;
  // lwz r10,120(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 120);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x82149ba0
  if (cr6.eq) goto loc_82149BA0;
loc_82149B9C:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149BA0:
  // lwz r11,116(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 116);
  // lwz r10,124(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 124);
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
  // beq 0x82149bd8
  if (cr0.eq) goto loc_82149BD8;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::SysOutWord(base, uint32_t(r31)).value_address());
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::SysOutWord(base, uint32_t(r31)).value_address(), uint32_t(r11));
  // bl 0x821497a8
  // (left out: dead in C++)
  f821497A8(base, r3);
  // b 0x82149b84
  goto loc_82149B84;
loc_82149BD8:
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

// sub_82149BF0: lifted from generated/ by lift.py.
void f82149BF0(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82149BF0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // ld r11,0(r4)
  r11 = guest::LoadU64(base, schema::SysLinkId(base, uint32_t(r4)).id_address());
  // std r11,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r11);
  // neg r11,r5
  r11 = uint64_t(int64_t(static_cast<int64_t>(-r5)));
  // lwz r10,-16(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + -16);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x82149c0c
  if (!cr6.eq) goto loc_82149C0C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149C0C:
  // lwz r9,-12(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + -12);
  // mulli r11,r11,384
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(384))));
  // lwz r8,8(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 8);
  // add r11,r11,r9
  r11 = r11 + r9;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bgt cr6,0x82149c30
  if (cr6.gt) goto loc_82149C30;
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x82149c34
  if (!cr6.lt) goto loc_82149C34;
loc_82149C30:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149C34:
  // stw r11,-12(r1)
  guest::StoreU32(base, uint32_t(r1) + -12, uint32_t(r11));
  // ld r11,-16(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + -16);
  // std r11,0(r3)
  guest::StoreU64(base, schema::SysLinkId(base, uint32_t(r3)).id_address(), r11);
  // blr
  return;
}

// sub_82149CA0: lifted from generated/ by lift.py.
uint64_t f82149CA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82149CA0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r4,r5
  r4 = r5;
  // mr r3,r30
  r3 = r30;
  // bl 0x82149960
  // (left out: dead in C++)
  r3 = f82149960(base, r3, r4);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // stw r3,4(r31)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r31)).node_address(), uint32_t(r3));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r31)).container_address(), uint32_t(r11));
  // bne cr6,0x82149ce0
  if (!cr6.eq) goto loc_82149CE0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149CE0:
  // mr r3,r31
  r3 = r31;
  // stw r30,0(r31)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r31)).container_address(), uint32_t(r30));
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

// sub_82149D60: lifted from generated/ by lift.py.
uint64_t f82149D60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82149D60 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // li r10,384
  r10 = uint64_t(int64_t(384));
  // subf r11,r30,r4
  r11 = r4 - r30;
  // mr r31,r4
  r31 = r4;
  // divw r11,r11,r10
  r11 = uint32_t((int32_t(r10) && !(int32_t(r11) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r11) / int32_t(r10) : 0);
  // cmplw cr6,r30,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r4), so);
  // mulli r11,r11,384
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(384))));
  // subf r28,r11,r5
  r28 = r5 - r11;
  // beq cr6,0x82149db0
  if (cr6.eq) goto loc_82149DB0;
  // subf r29,r4,r5
  r29 = r5 - r4;
loc_82149D94:
  // addi r31,r31,-384
  r31 = uint64_t(int64_t(int64_t(r31) + -384));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // add r3,r29,r31
  r3 = r29 + r31;
  // mr r4,r31
  r4 = r31;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82149DA8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x82149d94
  if (!cr6.eq) goto loc_82149D94;
loc_82149DB0:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82149DC0: lifted from generated/ by lift.py.
lifted::Result f82149DC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82149DC0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-1392(r1)
  ea = -1392 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r3,1412(r1)
  guest::StoreU32(base, uint32_t(r1) + 1412, uint32_t(r3));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x82149dfc
  if (!cr6.eq) goto loc_82149DFC;
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // addi r10,r10,-28780
  r10 = uint64_t(int64_t(int64_t(r10) + -28780));
  // stw r10,88(r11)
  guest::StoreU32(base, uint32_t(r11) + 88, uint32_t(r10));
  // b 0x82149fa4
  goto loc_82149FA4;
loc_82149DFC:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // lis r10,-32158
  r10 = uint64_t(int64_t(-2107506688));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r11,-29856(r10)
  guest::StoreU32(base, uint32_t(r10) + -29856, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,29448
  r30 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r4,4(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r9,8(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r4,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r9), so);
  // ble cr6,0x82149e28
  if (!cr6.gt) goto loc_82149E28;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149E28:
  // mr r8,r4
  r8 = r4;
loc_82149E2C:
  // cmplw cr6,r4,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r9), so);
  // ble cr6,0x82149e38
  if (!cr6.gt) goto loc_82149E38;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149E38:
  // subf r11,r8,r9
  r11 = r9 - r8;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82149eb4
  if (cr0.eq) goto loc_82149EB4;
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // blt cr6,0x82149e5c
  if (cr6.lt) goto loc_82149E5C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149E5C:
  // addi r11,r8,8
  r11 = uint64_t(int64_t(int64_t(r8) + 8));
  // addi r10,r1,1412
  r10 = uint64_t(int64_t(int64_t(r1) + 1412));
  // addi r6,r11,4
  r6 = uint64_t(int64_t(int64_t(r11) + 4));
loc_82149E68:
  // lbz r7,0(r11)
  r7 = guest::LoadU8(base, uint32_t(r11) + 0);
  // lbz r3,0(r10)
  r3 = guest::LoadU8(base, uint32_t(r10) + 0);
  // subf. r7,r3,r7
  r7 = r7 - r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // bne 0x82149e88
  if (!cr0.eq) goto loc_82149E88;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r6), so);
  // bne cr6,0x82149e68
  if (!cr6.eq) goto loc_82149E68;
loc_82149E88:
  // cmpwi r7,0
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // bne 0x82149ea0
  if (!cr0.eq) goto loc_82149EA0;
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // blt cr6,0x82149e9c
  if (cr6.lt) goto loc_82149E9C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149E9C:
  // ld r5,0(r8)
  r5 = guest::LoadU64(base, uint32_t(r8) + 0);
loc_82149EA0:
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // blt cr6,0x82149eac
  if (cr6.lt) goto loc_82149EAC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149EAC:
  // addi r8,r8,384
  r8 = uint64_t(int64_t(int64_t(r8) + 384));
  // b 0x82149e2c
  goto loc_82149E2C;
loc_82149EB4:
  // cmpldi cr6,r5,0
  cr6 = lifted::Compare<uint64_t>(r5, 0, so);
  // beq cr6,0x82149fa4
  if (cr6.eq) goto loc_82149FA4;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stb r5,90(r1)
  guest::StoreU8(base, uint32_t(r1) + 90, uint8_t(r5));
  // cmplw cr6,r4,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r9), so);
  // stb r11,82(r1)
  guest::StoreU8(base, uint32_t(r1) + 82, uint8_t(r11));
  // rldicl r11,r5,8,56
  r11 = __builtin_rotateleft64(r5, 8) & 0xFF;
  // stb r11,83(r1)
  guest::StoreU8(base, uint32_t(r1) + 83, uint8_t(r11));
  // rldicl r11,r5,40,24
  r11 = __builtin_rotateleft64(r5, 40) & 0xFFFFFFFFFF;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // rldicl r11,r5,56,8
  r11 = __builtin_rotateleft64(r5, 56) & 0xFFFFFFFFFFFFFF;
  // sth r11,88(r1)
  guest::StoreU16(base, uint32_t(r1) + 88, uint16_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stb r11,91(r1)
  guest::StoreU8(base, uint32_t(r1) + 91, uint8_t(r11));
  // ble cr6,0x82149ef4
  if (!cr6.gt) goto loc_82149EF4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149EF4:
  // mr r31,r4
  r31 = r4;
loc_82149EF8:
  // cmplw cr6,r4,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r9), so);
  // ble cr6,0x82149f04
  if (!cr6.gt) goto loc_82149F04;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149F04:
  // subf r11,r31,r9
  r11 = r9 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82149f94
  if (cr0.eq) goto loc_82149F94;
  // cmplw cr6,r31,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r9), so);
  // blt cr6,0x82149f28
  if (cr6.lt) goto loc_82149F28;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149F28:
  // addi r8,r31,8
  r8 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r10,r1,1412
  r10 = uint64_t(int64_t(int64_t(r1) + 1412));
  // mr r11,r8
  r11 = r8;
  // addi r7,r11,4
  r7 = uint64_t(int64_t(int64_t(r11) + 4));
loc_82149F38:
  // lbz r6,0(r11)
  r6 = guest::LoadU8(base, uint32_t(r11) + 0);
  // lbz r5,0(r10)
  r5 = guest::LoadU8(base, uint32_t(r10) + 0);
  // subf. r6,r5,r6
  r6 = r6 - r5;
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // bne 0x82149f58
  if (!cr0.eq) goto loc_82149F58;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r7), so);
  // bne cr6,0x82149f38
  if (!cr6.eq) goto loc_82149F38;
loc_82149F58:
  // cmpwi r6,0
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // beq 0x82149f80
  if (cr0.eq) goto loc_82149F80;
  // cmplw cr6,r31,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r9), so);
  // blt cr6,0x82149f6c
  if (cr6.lt) goto loc_82149F6C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149F6C:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,0(r8)
  r4 = guest::LoadU32(base, uint32_t(r8) + 0);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x82149F78, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,8(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 8);
  // lwz r4,4(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 4);
loc_82149F80:
  // cmplw cr6,r31,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r9), so);
  // blt cr6,0x82149f8c
  if (cr6.lt) goto loc_82149F8C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82149F8C:
  // addi r31,r31,384
  r31 = uint64_t(int64_t(int64_t(r31) + 384));
  // b 0x82149ef8
  goto loc_82149EF8;
loc_82149F94:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,-29028(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + -29028);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x82149FA4, r31)); r3 = o.r3; f1 = o.f1; }
loc_82149FA4:
  // addi r1,r1,1392
  r1 = uint64_t(int64_t(int64_t(r1) + 1392));
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

// sub_8214A198: lifted from generated/ by lift.py.
void f8214A198(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8214A198 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // addi r5,r11,30752
  r5 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8214a1c4
  if (!cr6.eq) goto loc_8214A1C4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lbz r11,12(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 12);
  // addi r7,r11,1
  r7 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x8214a1c8
  goto loc_8214A1C8;
loc_8214A1C4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_8214A1C8:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r10,r10,29448
  r10 = uint64_t(int64_t(int64_t(r10) + 29448));
  // lwz r6,4(r10)
  r6 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r9,8(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 8);
  // cmplw cr6,r6,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r9), so);
  // ble cr6,0x8214a1e4
  if (!cr6.gt) goto loc_8214A1E4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A1E4:
  // mr r10,r6
  r10 = r6;
loc_8214A1E8:
  // cmplw cr6,r6,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r9), so);
  // ble cr6,0x8214a1f4
  if (!cr6.gt) goto loc_8214A1F4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A1F4:
  // subf r8,r10,r9
  r8 = r9 - r10;
  // cntlzw r8,r8
  r8 = uint32_t(r8) == 0 ? 32 : __builtin_clz(uint32_t(r8));
  // rlwinm r8,r8,27,31,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 27) & 0x1;
  // cntlzw r8,r8
  r8 = uint32_t(r8) == 0 ? 32 : __builtin_clz(uint32_t(r8));
  // rlwinm. r8,r8,27,31,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x8214a25c
  if (cr0.eq) goto loc_8214A25C;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x8214a218
  if (cr6.lt) goto loc_8214A218;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A218:
  // lbz r8,12(r10)
  r8 = guest::LoadU8(base, uint32_t(r10) + 12);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // add r7,r8,r7
  r7 = r8 + r7;
  // blt cr6,0x8214a22c
  if (cr6.lt) goto loc_8214A22C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A22C:
  // lwz r4,292(r10)
  r4 = guest::LoadU32(base, uint32_t(r10) + 292);
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beq cr6,0x8214a248
  if (cr6.eq) goto loc_8214A248;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x8214a244
  if (cr6.lt) goto loc_8214A244;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A244:
  // add r11,r8,r11
  r11 = r8 + r11;
loc_8214A248:
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x8214a254
  if (cr6.lt) goto loc_8214A254;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A254:
  // addi r10,r10,384
  r10 = uint64_t(int64_t(int64_t(r10) + 384));
  // b 0x8214a1e8
  goto loc_8214A1E8;
loc_8214A25C:
  // lwz r10,124(r5)
  r10 = guest::LoadU32(base, uint32_t(r5) + 124);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214a26c
  if (!cr6.gt) goto loc_8214A26C;
  // mr r11,r10
  r11 = r10;
loc_8214A26C:
  // stw r11,132(r5)
  guest::StoreU32(base, uint32_t(r5) + 132, uint32_t(r11));
  // subf r11,r11,r7
  r11 = r7 - r11;
  // stw r11,128(r5)
  guest::StoreU32(base, uint32_t(r5) + 128, uint32_t(r11));
  // blr
  return;
}

// sub_8214A280: lifted from generated/ by lift.py.
uint64_t f8214A280(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8214A280 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stw r4,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r4));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8214a308
  if (!cr6.eq) goto loc_8214A308;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r11,r1,140
  r11 = uint64_t(int64_t(int64_t(r1) + 140));
  // addi r31,r10,29464
  r31 = uint64_t(int64_t(int64_t(r10) + 29464));
  // addi r9,r11,4
  r9 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r10,r31,8
  r10 = uint64_t(int64_t(int64_t(r31) + 8));
loc_8214A2C0:
  // lbz r8,0(r11)
  r8 = guest::LoadU8(base, uint32_t(r11) + 0);
  // lbz r7,0(r10)
  r7 = guest::LoadU8(base, uint32_t(r10) + 0);
  // subf. r8,r7,r8
  r8 = r8 - r7;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x8214a2e0
  if (!cr0.eq) goto loc_8214A2E0;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // bne cr6,0x8214a2c0
  if (!cr6.eq) goto loc_8214A2C0;
loc_8214A2E0:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x8214a2f0
  if (!cr0.eq) goto loc_8214A2F0;
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8214A2EC, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,272(r31)
  guest::StoreU32(base, uint32_t(r31) + 272, uint32_t(r3));
loc_8214A2F0:
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
loc_8214A308:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,29448
  r30 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r6,4(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r9,8(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r6,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r9), so);
  // ble cr6,0x8214a324
  if (!cr6.gt) goto loc_8214A324;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A324:
  // mr r31,r6
  r31 = r6;
loc_8214A328:
  // cmplw cr6,r6,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r9), so);
  // ble cr6,0x8214a334
  if (!cr6.gt) goto loc_8214A334;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A334:
  // subf r11,r31,r9
  r11 = r9 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214a2f0
  if (cr0.eq) goto loc_8214A2F0;
  // cmplw cr6,r31,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r9), so);
  // blt cr6,0x8214a358
  if (cr6.lt) goto loc_8214A358;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A358:
  // addi r11,r31,8
  r11 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r10,r1,140
  r10 = uint64_t(int64_t(int64_t(r1) + 140));
  // addi r7,r11,4
  r7 = uint64_t(int64_t(int64_t(r11) + 4));
loc_8214A364:
  // lbz r8,0(r11)
  r8 = guest::LoadU8(base, uint32_t(r11) + 0);
  // lbz r5,0(r10)
  r5 = guest::LoadU8(base, uint32_t(r10) + 0);
  // subf. r8,r5,r8
  r8 = r8 - r5;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x8214a384
  if (!cr0.eq) goto loc_8214A384;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r7), so);
  // bne cr6,0x8214a364
  if (!cr6.eq) goto loc_8214A364;
loc_8214A384:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x8214a39c
  if (!cr0.eq) goto loc_8214A39C;
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8214A390, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,272(r31)
  guest::StoreU32(base, uint32_t(r31) + 272, uint32_t(r3));
  // lwz r9,8(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 8);
  // lwz r6,4(r30)
  r6 = guest::LoadU32(base, uint32_t(r30) + 4);
loc_8214A39C:
  // cmplw cr6,r31,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r9), so);
  // blt cr6,0x8214a3a8
  if (cr6.lt) goto loc_8214A3A8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A3A8:
  // addi r31,r31,384
  r31 = uint64_t(int64_t(int64_t(r31) + 384));
  // b 0x8214a328
  goto loc_8214A328;
}

// sub_8214A3B0: lifted from generated/ by lift.py.
lifted::Result f8214A3B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8214A3B0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r8,r3,3
  r8 = uint64_t(int64_t(int64_t(r3) + 3));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8214a3e0
  if (!cr6.eq) goto loc_8214A3E0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // ld r9,0(r8)
  r9 = guest::LoadU64(base, schema::SysLinkPacket(base, uint32_t(r8) - 3).sender_id_address());
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // cmpld cr6,r9,r10
  cr6 = lifted::Compare<uint64_t>(r9, r10, so);
  // beq cr6,0x8214a468
  if (cr6.eq) goto loc_8214A468;
loc_8214A3E0:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29448
  r11 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // ble cr6,0x8214a3fc
  if (!cr6.gt) goto loc_8214A3FC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A3FC:
  // mr r11,r9
  r11 = r9;
loc_8214A400:
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // ble cr6,0x8214a40c
  if (!cr6.gt) goto loc_8214A40C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A40C:
  // subf r7,r11,r10
  r7 = r10 - r11;
  // cntlzw r7,r7
  r7 = uint32_t(r7) == 0 ? 32 : __builtin_clz(uint32_t(r7));
  // rlwinm r7,r7,27,31,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 27) & 0x1;
  // cntlzw r7,r7
  r7 = uint32_t(r7) == 0 ? 32 : __builtin_clz(uint32_t(r7));
  // rlwinm. r7,r7,27,31,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // beqlr
  if (cr0.eq) return {r3, f1};
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214a430
  if (cr6.lt) goto loc_8214A430;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A430:
  // ld r7,0(r11)
  r7 = guest::LoadU64(base, uint32_t(r11) + 0);
  // ld r6,0(r8)
  r6 = guest::LoadU64(base, schema::SysLinkPacket(base, uint32_t(r8) - 3).sender_id_address());
  // cmpld cr6,r7,r6
  cr6 = lifted::Compare<uint64_t>(r7, r6, so);
  // beq cr6,0x8214a454
  if (cr6.eq) goto loc_8214A454;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214a44c
  if (cr6.lt) goto loc_8214A44C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A44C:
  // addi r11,r11,384
  r11 = uint64_t(int64_t(int64_t(r11) + 384));
  // b 0x8214a400
  goto loc_8214A400;
loc_8214A454:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214a460
  if (cr6.lt) goto loc_8214A460;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A460:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
loc_8214A468:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lhz r5,1272(r8)
  r5 = guest::LoadU16(base, schema::SysLinkPacket(base, uint32_t(r8) - 3).unknown_1275_address());
  // mr r6,r11
  r6 = r11;
  // addi r3,r10,29856
  r3 = uint64_t(int64_t(int64_t(r10) + 29856));
  // addi r4,r8,8
  r4 = uint64_t(int64_t(int64_t(r8) + 8));
  // b 0x8214fc40
  { const lifted::Result o = f8214FC40(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8214A488: lifted from generated/ by lift.py.
uint64_t f8214A488(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8214A488 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4103
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4103, so);
  // bne cr6,0x8214a604
  if (!cr6.eq) goto loc_8214A604;
  // addi r4,r3,3
  r4 = uint64_t(int64_t(int64_t(r3) + 3));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,72
  r5 = uint64_t(int64_t(72));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214A4B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // ld r8,96(r1)
  r8 = guest::LoadU64(base, uint32_t(r1) + 96);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // ld r11,29464(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 29464);
  // cmpld cr6,r8,r11
  cr6 = lifted::Compare<uint64_t>(r8, r11, so);
  // beq cr6,0x8214a558
  if (cr6.eq) goto loc_8214A558;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // ld r11,30320(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 30320);
  // cmpld cr6,r8,r11
  cr6 = lifted::Compare<uint64_t>(r8, r11, so);
  // beq cr6,0x8214a558
  if (cr6.eq) goto loc_8214A558;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29448
  r11 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // ble cr6,0x8214a4f8
  if (!cr6.gt) goto loc_8214A4F8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A4F8:
  // mr r11,r9
  r11 = r9;
loc_8214A4FC:
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // ble cr6,0x8214a508
  if (!cr6.gt) goto loc_8214A508;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A508:
  // subf r6,r11,r10
  r6 = r10 - r11;
  // cntlzw r6,r6
  r6 = uint32_t(r6) == 0 ? 32 : __builtin_clz(uint32_t(r6));
  // rlwinm r6,r6,27,31,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 27) & 0x1;
  // cntlzw r6,r6
  r6 = uint32_t(r6) == 0 ? 32 : __builtin_clz(uint32_t(r6));
  // rlwinm. r6,r6,27,31,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // beq 0x8214a550
  if (cr0.eq) goto loc_8214A550;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214a52c
  if (cr6.lt) goto loc_8214A52C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A52C:
  // ld r6,0(r11)
  r6 = guest::LoadU64(base, uint32_t(r11) + 0);
  // cmpld cr6,r6,r8
  cr6 = lifted::Compare<uint64_t>(r6, r8, so);
  // bne cr6,0x8214a53c
  if (!cr6.eq) goto loc_8214A53C;
  // mr r7,r11
  r7 = r11;
loc_8214A53C:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214a548
  if (cr6.lt) goto loc_8214A548;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A548:
  // addi r11,r11,384
  r11 = uint64_t(int64_t(int64_t(r11) + 384));
  // b 0x8214a4fc
  goto loc_8214A4FC;
loc_8214A550:
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x8214a604
  if (cr6.eq) goto loc_8214A604;
loc_8214A558:
  // lwz r9,156(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 156);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,144(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 144);
  // addi r4,r1,116
  r4 = uint64_t(int64_t(int64_t(r1) + 116));
  // addi r3,r11,25292
  r3 = uint64_t(int64_t(int64_t(r11) + 25292));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,20
  r5 = uint64_t(int64_t(20));
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // addi r11,r11,24996
  r11 = uint64_t(int64_t(int64_t(r11) + 24996));
  // lwz r9,160(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 160);
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // lwz r9,164(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 164);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 0, temp.u32);
  // lfs f0,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // lwz r9,140(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 140);
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 4, temp.u32);
  // lfs f0,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 8, temp.u32);
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,25268
  r11 = uint64_t(int64_t(int64_t(r11) + 25268));
  // lfs f0,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lwz r10,136(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 136);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 0, temp.u32);
  // stw r10,26012(r9)
  guest::StoreU32(base, uint32_t(r9) + 26012, uint32_t(r10));
  // lwz r10,148(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 148);
  // lfs f0,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lwz r10,152(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 152);
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 4, temp.u32);
  // lfs f0,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 8, temp.u32);
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lfs f0,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 12, temp.u32);
  // lwz r11,112(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 112);
  // stw r11,24732(r10)
  guest::StoreU32(base, uint32_t(r10) + 24732, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214A604, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214A604:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8214A618: lifted from generated/ by lift.py.
lifted::Result f8214A618(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214A618 (lifted)");
  uint64_t r3 = 0, r5 = 0, r8 = 0, r9 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // stw r4,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r4));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r5,r11,29448
  r5 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r4,4(r5)
  r4 = guest::LoadU32(base, uint32_t(r5) + 4);
  // lwz r8,8(r5)
  r8 = guest::LoadU32(base, uint32_t(r5) + 8);
  // cmplw cr6,r4,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r8), so);
  // ble cr6,0x8214a64c
  if (!cr6.gt) goto loc_8214A64C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A64C:
  // mr r9,r4
  r9 = r4;
loc_8214A650:
  // cmplw cr6,r4,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r8), so);
  // ble cr6,0x8214a65c
  if (!cr6.gt) goto loc_8214A65C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A65C:
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
  // beq 0x8214a6e8
  if (cr0.eq) goto loc_8214A6E8;
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x8214a680
  if (cr6.lt) goto loc_8214A680;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A680:
  // addi r11,r9,8
  r11 = uint64_t(int64_t(int64_t(r9) + 8));
  // addi r10,r1,124
  r10 = uint64_t(int64_t(int64_t(r1) + 124));
  // addi r6,r11,4
  r6 = uint64_t(int64_t(int64_t(r11) + 4));
loc_8214A68C:
  // lbz r7,0(r11)
  r7 = guest::LoadU8(base, uint32_t(r11) + 0);
  // lbz r31,0(r10)
  r31 = guest::LoadU8(base, uint32_t(r10) + 0);
  // subf. r7,r31,r7
  r7 = r7 - r31;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // bne 0x8214a6ac
  if (!cr0.eq) goto loc_8214A6AC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r6), so);
  // bne cr6,0x8214a68c
  if (!cr6.eq) goto loc_8214A68C;
loc_8214A6AC:
  // cmpwi r7,0
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // bne 0x8214a6c4
  if (!cr0.eq) goto loc_8214A6C4;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,296(r9)
  guest::StoreU32(base, uint32_t(r9) + 296, uint32_t(r11));
  // lwz r8,8(r5)
  r8 = guest::LoadU32(base, uint32_t(r5) + 8);
  // lwz r4,4(r5)
  r4 = guest::LoadU32(base, uint32_t(r5) + 4);
loc_8214A6C4:
  // lwz r11,296(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 296);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8214a6d4
  if (!cr6.eq) goto loc_8214A6D4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8214A6D4:
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x8214a6e0
  if (cr6.lt) goto loc_8214A6E0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A6E0:
  // addi r9,r9,384
  r9 = uint64_t(int64_t(int64_t(r9) + 384));
  // b 0x8214a650
  goto loc_8214A650;
loc_8214A6E8:
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214a710
  if (cr0.eq) goto loc_8214A710;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r11,30752
  r3 = uint64_t(int64_t(int64_t(r11) + 30752));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // bl 0x8214ed30
  // (left out: dead in C++)
  { const lifted::Result o = f8214ED30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,4100
  r11 = uint64_t(int64_t(4100));
  // stw r11,27304(r10)
  guest::StoreU32(base, uint32_t(r10) + 27304, uint32_t(r11));
loc_8214A710:
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

// sub_8214A728: lifted from generated/ by lift.py.
lifted::Result f8214A728(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8214A728 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,24704(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 24704);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // beq cr6,0x8214a8d4
  if (cr6.eq) goto loc_8214A8D4;
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // lwz r11,27308(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 27308);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x8214a75c
  if (!cr6.eq) goto loc_8214A75C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8214a8d8
  goto loc_8214A8D8;
loc_8214A75C:
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8214a864
  if (cr6.lt) goto loc_8214A864;
  // beq cr6,0x8214a850
  if (cr6.eq) goto loc_8214A850;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x8214a8d4
  if (!cr6.lt) goto loc_8214A8D4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,29448
  r30 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r10,8(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214a78c
  if (!cr6.gt) goto loc_8214A78C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A78C:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r31,r11,29848
  r31 = uint64_t(int64_t(int64_t(r11) + 29848));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214a7a8
  if (cr6.eq) goto loc_8214A7A8;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // beq cr6,0x8214a7ac
  if (cr6.eq) goto loc_8214A7AC;
loc_8214A7A8:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A7AC:
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // subf r10,r3,r10
  r10 = r10 - r3;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm. r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8214a848
  if (cr0.eq) goto loc_8214A848;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8214a7d4
  if (!cr6.eq) goto loc_8214A7D4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A7D4:
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // blt cr6,0x8214a7e4
  if (cr6.lt) goto loc_8214A7E4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A7E4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82149430
  // (left out: dead in C++)
  { const lifted::Result o = f82149430(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r8,0(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // bne cr6,0x8214a7fc
  if (!cr6.eq) goto loc_8214A7FC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A7FC:
  // lwz r10,8(r8)
  r10 = guest::LoadU32(base, uint32_t(r8) + 8);
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214a810
  if (cr6.lt) goto loc_8214A810;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A810:
  // addi r11,r11,384
  r11 = uint64_t(int64_t(int64_t(r11) + 384));
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r9,8(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // ble cr6,0x8214a82c
  if (!cr6.gt) goto loc_8214A82C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A82C:
  // cmplw cr6,r8,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r30), so);
  // beq cr6,0x8214a838
  if (cr6.eq) goto loc_8214A838;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A838:
  // subf r11,r11,r9
  r11 = r9 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214a8d4
  if (cr0.eq) goto loc_8214A8D4;
loc_8214A848:
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // b 0x8214a8d0
  goto loc_8214A8D0;
loc_8214A850:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r11,29464
  r3 = uint64_t(int64_t(int64_t(r11) + 29464));
  // bl 0x82149430
  // (left out: dead in C++)
  { const lifted::Result o = f82149430(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214a890
  goto loc_8214A890;
loc_8214A864:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x8214A86C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r4,r3
  r4 = r3;
  // addi r3,r11,30320
  r3 = uint64_t(int64_t(int64_t(r11) + 30320));
  // bl 0x82149430
  // (left out: dead in C++)
  { const lifted::Result o = f82149430(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214a8cc
  if (cr6.eq) goto loc_8214A8CC;
loc_8214A890:
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,27308(r29)
  guest::StoreU32(base, uint32_t(r29) + 27308, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29448
  r11 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // ble cr6,0x8214a8b4
  if (!cr6.gt) goto loc_8214A8B4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A8B4:
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // std r11,29848(r10)
  guest::StoreU64(base, uint32_t(r10) + 29848, r11);
  // b 0x8214a8d4
  goto loc_8214A8D4;
loc_8214A8CC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_8214A8D0:
  // stw r11,27308(r29)
  guest::StoreU32(base, uint32_t(r29) + 27308, uint32_t(r11));
loc_8214A8D4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_8214A8D8:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214A8E0: lifted from generated/ by lift.py.
uint64_t f8214A8E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8214A8E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r11,r3
  r11 = r3;
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r11)).size_address());
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r11)).head_address());
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // ble cr6,0x8214a904
  if (!cr6.gt) goto loc_8214A904;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A904:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x82149bf0
  // (left out: dead in C++)
  f82149BF0(base, r1, r3, r4, r5);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).container_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8214a92c
  if (!cr0.eq) goto loc_8214A92C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A92C:
  // lwz r3,4(r3)
  r3 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address());
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r11)).size_address());
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // blt cr6,0x8214a940
  if (cr6.lt) goto loc_8214A940;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A940:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8214A950: lifted from generated/ by lift.py.
uint64_t f8214A950(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8214A950 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r4,r5
  r4 = r5;
  // mr r3,r30
  r3 = r30;
  // bl 0x82149830
  // (left out: dead in C++)
  r3 = f82149830(base, r3, r4);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // stw r3,4(r31)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r31)).node_address(), uint32_t(r3));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r31)).container_address(), uint32_t(r11));
  // bne cr6,0x8214a990
  if (!cr6.eq) goto loc_8214A990;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214A990:
  // mr r3,r31
  r3 = r31;
  // stw r30,0(r31)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r31)).container_address(), uint32_t(r30));
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

// sub_8214A9B0: lifted from generated/ by lift.py.
uint64_t f8214A9B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8214A9B0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // bl 0x82149ca0
  // (left out: dead in C++)
  r3 = f82149CA0(base, r1, r13, r3, r4, r5);
  // mr r28,r3
  r28 = r3;
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x8214a950
  // (left out: dead in C++)
  r3 = f8214A950(base, r1, r13, r3, r4, r5);
  // lwz r10,0(r28)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r28)).container_address());
  // addi r9,r31,8
  r9 = uint64_t(int64_t(int64_t(r31) + 8));
  // lwz r8,4(r28)
  r8 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r28)).node_address());
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r9)
  guest::StoreU32(base, schema::SysLinkIdSetRange(base, uint32_t(r9) - 8).last_container_address(), uint32_t(r10));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r11)).container_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r11)).node_address());
  // stw r8,4(r9)
  guest::StoreU32(base, schema::SysLinkIdSetRange(base, uint32_t(r9) - 8).last_node_address(), uint32_t(r8));
  // stw r10,0(r31)
  guest::StoreU32(base, schema::SysLinkIdSetRange(base, uint32_t(r31)).first_container_address(), uint32_t(r10));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::SysLinkIdSetRange(base, uint32_t(r31)).first_node_address(), uint32_t(r11));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_8214AC68: lifted from generated/ by lift.py.
lifted::Result f8214AC68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8214AC68 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-1392(r1)
  ea = -1392 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,83
  r4 = uint64_t(int64_t(int64_t(r1) + 83));
  // addi r3,r11,29856
  r3 = uint64_t(int64_t(int64_t(r11) + 29856));
  // li r11,11
  r11 = uint64_t(int64_t(11));
  // stb r11,82(r1)
  guest::StoreU8(base, uint32_t(r1) + 82, uint8_t(r11));
  // bl 0x8214fdf8
  // (left out: dead in C++)
  { const lifted::Result o = f8214FDF8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8214acd4
  if (!cr6.eq) goto loc_8214ACD4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lwz r4,8(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 8);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x8214ACBC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214ACBC:
  // addi r1,r1,1392
  r1 = uint64_t(int64_t(int64_t(r1) + 1392));
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
loc_8214ACD4:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,29448
  r30 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r10,8(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214acf0
  if (!cr6.gt) goto loc_8214ACF0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214ACF0:
  // mr r31,r11
  r31 = r11;
loc_8214ACF4:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214ad00
  if (!cr6.gt) goto loc_8214AD00;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214AD00:
  // subf r11,r31,r10
  r11 = r10 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214acbc
  if (cr0.eq) goto loc_8214ACBC;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8214ad24
  if (cr6.lt) goto loc_8214AD24;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214AD24:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x8214AD30, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,8(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8214ad40
  if (cr6.lt) goto loc_8214AD40;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214AD40:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // addi r31,r31,384
  r31 = uint64_t(int64_t(int64_t(r31) + 384));
  // b 0x8214acf4
  goto loc_8214ACF4;
}

// sub_8214B0A8: lifted from generated/ by lift.py.
lifted::Result f8214B0A8(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_8214B0A8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-1392(r1)
  ea = -1392 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8214B0C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r3,27352(r11)
  guest::StoreU32(base, uint32_t(r11) + 27352, uint32_t(r3));
  // li r11,4099
  r11 = uint64_t(int64_t(4099));
  // stw r11,27304(r10)
  guest::StoreU32(base, uint32_t(r10) + 27304, uint32_t(r11));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stb r11,82(r1)
  guest::StoreU8(base, uint32_t(r1) + 82, uint8_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r31,r11,29448
  r31 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214b0f8
  if (!cr6.gt) goto loc_8214B0F8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B0F8:
  // mr r30,r11
  r30 = r11;
loc_8214B0FC:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214b108
  if (!cr6.gt) goto loc_8214B108;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B108:
  // subf r11,r30,r10
  r11 = r10 - r30;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214b16c
  if (cr0.eq) goto loc_8214B16C;
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // blt cr6,0x8214b12c
  if (cr6.lt) goto loc_8214B12C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B12C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,296(r30)
  guest::StoreU32(base, uint32_t(r30) + 296, uint32_t(r11));
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8214b144
  if (cr6.lt) goto loc_8214B144;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B144:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,8(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 8);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x8214B150, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // blt cr6,0x8214b160
  if (cr6.lt) goto loc_8214B160;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B160:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // addi r30,r30,384
  r30 = uint64_t(int64_t(int64_t(r30) + 384));
  // b 0x8214b0fc
  goto loc_8214B0FC;
loc_8214B16C:
  // addi r1,r1,1392
  r1 = uint64_t(int64_t(int64_t(r1) + 1392));
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

// sub_8214B200: lifted from generated/ by lift.py.
uint64_t f8214B200(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_8214B200 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r5
  r31 = r5;
  // mr r28,r4
  r28 = r4;
  // mr r27,r6
  r27 = r6;
  // mr r29,r31
  r29 = r31;
  // mr r30,r28
  r30 = r28;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8214b24c
  if (cr6.eq) goto loc_8214B24C;
loc_8214B228:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x8214b240
  if (cr6.eq) goto loc_8214B240;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // mr r4,r27
  r4 = r27;
  // mr r3,r30
  r3 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214B240, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214B240:
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,384
  r30 = uint64_t(int64_t(int64_t(r30) + 384));
  // bne 0x8214b228
  if (!cr0.eq) goto loc_8214B228;
loc_8214B24C:
  // mulli r11,r31,384
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(384))));
  // add r3,r11,r28
  r3 = r11 + r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_8214B260: lifted from generated/ by lift.py.
lifted::Result f8214B260(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214B260 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8214b288
  if (!cr6.eq) goto loc_8214B288;
  // bl 0x821492b8
  // (left out: dead in C++)
  { const lifted::Result o = f821492B8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8214B288:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r4,r31
  r4 = r31;
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,30752
  r3 = uint64_t(int64_t(int64_t(r11) + 30752));
  // bne cr6,0x8214b2ac
  if (!cr6.eq) goto loc_8214B2AC;
  // bl 0x8214e850
  // (left out: dead in C++)
  { const lifted::Result o = f8214E850(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x8214b2b0
  goto loc_8214B2B0;
loc_8214B2AC:
  // bl 0x8214e908
  // (left out: dead in C++)
  { const lifted::Result o = f8214E908(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_8214B2B0:
  // bl 0x8214ac68
  // (left out: dead in C++)
  { const lifted::Result o = f8214AC68(base, r1, r13, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
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

// sub_8214B870: lifted from generated/ by lift.py.
lifted::Result f8214B870(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214B870 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-400(r1)
  ea = -400 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bne cr6,0x8214ba48
  if (!cr6.eq) goto loc_8214BA48;
  // addi r4,r3,3
  r4 = uint64_t(int64_t(int64_t(r3) + 3));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,281
  r5 = uint64_t(int64_t(281));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214B8A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8214b8e4
  if (cr6.lt) goto loc_8214B8E4;
  // beq cr6,0x8214b8d8
  if (cr6.eq) goto loc_8214B8D8;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x8214ba48
  if (!cr6.lt) goto loc_8214BA48;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,-28560
  r11 = uint64_t(int64_t(int64_t(r11) + -28560));
loc_8214B8C4:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r3,4108
  r3 = uint64_t(int64_t(4108));
  // addi r10,r10,30752
  r10 = uint64_t(int64_t(int64_t(r10) + 30752));
  // stw r11,88(r10)
  guest::StoreU32(base, uint32_t(r10) + 88, uint32_t(r11));
  // b 0x8214ba44
  goto loc_8214BA44;
loc_8214B8D8:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,-28600
  r11 = uint64_t(int64_t(int64_t(r11) + -28600));
  // b 0x8214b8c4
  goto loc_8214B8C4;
loc_8214B8E4:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r6,128
  r6 = uint64_t(int64_t(128));
  // addi r31,r11,29464
  r31 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lbz r11,112(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 112);
  // addi r5,r1,113
  r5 = uint64_t(int64_t(int64_t(r1) + 113));
  // li r4,128
  r4 = uint64_t(int64_t(128));
  // addi r3,r31,14
  r3 = uint64_t(int64_t(int64_t(r31) + 14));
  // stb r11,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r11));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214B908, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,32
  r6 = uint64_t(int64_t(32));
  // addi r5,r1,241
  r5 = uint64_t(int64_t(int64_t(r1) + 241));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // addi r3,r31,144
  r3 = uint64_t(int64_t(int64_t(r31) + 144));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214B91C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,273
  r5 = uint64_t(int64_t(int64_t(r1) + 273));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,176
  r3 = uint64_t(int64_t(int64_t(r31) + 176));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214B930, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,297
  r5 = uint64_t(int64_t(int64_t(r1) + 297));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,316
  r3 = uint64_t(int64_t(int64_t(r31) + 316));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214B944, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,313
  r5 = uint64_t(int64_t(int64_t(r1) + 313));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,332
  r3 = uint64_t(int64_t(int64_t(r31) + 332));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214B958, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,329
  r5 = uint64_t(int64_t(int64_t(r1) + 329));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,348
  r3 = uint64_t(int64_t(int64_t(r31) + 348));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214B96C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,345
  r5 = uint64_t(int64_t(int64_t(r1) + 345));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,364
  r3 = uint64_t(int64_t(int64_t(r31) + 364));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214B980, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,88(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 88);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r30,r11,30752
  r30 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // rldicr r11,r11,32,63
  r11 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // mr r3,r30
  r3 = r30;
  // or r11,r10,r11
  r11 = r10 | r11;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // std r11,0(r31)
  guest::StoreU64(base, uint32_t(r31) + 0, r11);
  // lwz r11,100(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 100);
  // stw r11,29312(r10)
  guest::StoreU32(base, uint32_t(r10) + 29312, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,104(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 104);
  // stw r11,29308(r10)
  guest::StoreU32(base, uint32_t(r10) + 29308, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,108(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 108);
  // stw r11,29316(r10)
  guest::StoreU32(base, uint32_t(r10) + 29316, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lhz r10,290(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + 290);
  // stw r11,124(r30)
  guest::StoreU32(base, uint32_t(r30) + 124, uint32_t(r11));
  // stw r11,132(r30)
  guest::StoreU32(base, uint32_t(r30) + 132, uint32_t(r11));
  // lbz r11,289(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 289);
  // rotlwi r11,r11,16
  r11 = __builtin_rotateleft32(uint32_t(r11), 16);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lbz r10,292(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 292);
  // rlwinm r11,r11,8,0,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFFFFFF00;
  // or r11,r10,r11
  r11 = r10 | r11;
  // stw r11,120(r30)
  guest::StoreU32(base, uint32_t(r30) + 120, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,128(r30)
  guest::StoreU32(base, uint32_t(r30) + 128, uint32_t(r11));
  // bl 0x8214ed30
  // (left out: dead in C++)
  { const lifted::Result o = f8214ED30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lbz r11,293(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 293);
  // mr r3,r30
  r3 = r30;
  // lhz r10,294(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + 294);
  // rotlwi r11,r11,16
  r11 = __builtin_rotateleft32(uint32_t(r11), 16);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lbz r10,296(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 296);
  // rlwinm r11,r11,8,0,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFFFFFF00;
  // or r4,r10,r11
  r4 = r10 | r11;
  // bl 0x8214e778
  // (left out: dead in C++)
  { const lifted::Result o = f8214E778(base, r1, r13, r3, r4, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,92(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 92);
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r9,96(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 96);
  // li r3,4097
  r3 = uint64_t(int64_t(4097));
  // rldicr r11,r11,32,63
  r11 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r11,r9,r11
  r11 = r9 | r11;
  // std r11,27376(r10)
  guest::StoreU64(base, uint32_t(r10) + 27376, r11);
loc_8214BA44:
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8214BA48:
  // addi r1,r1,400
  r1 = uint64_t(int64_t(int64_t(r1) + 400));
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

// sub_8214BA60: lifted from generated/ by lift.py.
lifted::Result f8214BA60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8214BA60 (lifted)");
  uint64_t r3 = 0, r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // stw r11,28180(r10)
  guest::StoreU32(base, uint32_t(r10) + 28180, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r31,r11,30752
  r31 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // rlwinm r11,r11,0,28,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFEF;
  // stw r11,80(r31)
  guest::StoreU32(base, uint32_t(r31) + 80, uint32_t(r11));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,29432(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 29432);
  // bl 0x825767d0
  // (left out: dead in C++)
  { const lifted::Result o = f825767D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,100
  r4 = uint64_t(int64_t(int64_t(r1) + 100));
  // mr r3,r31
  r3 = r31;
  // bl 0x8214e818
  // (left out: dead in C++)
  r3 = f8214E818(base, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8214bae0
  if (cr0.eq) goto loc_8214BAE0;
  // addi r4,r1,100
  r4 = uint64_t(int64_t(int64_t(r1) + 100));
  // li r5,60
  r5 = uint64_t(int64_t(60));
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214BAD0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,29432(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 29432);
  // lwz r3,160(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 160);
  // stw r11,84(r31)
  guest::StoreU32(base, uint32_t(r31) + 84, uint32_t(r11));
  // bl 0x82148fd8
  // (left out: dead in C++)
  { const lifted::Result o = f82148FD8(base, r1, r13, r3, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
loc_8214BAE0:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
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

// sub_8214BAF8: lifted from generated/ by lift.py.
lifted::Result f8214BAF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8214BAF8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // cmpwi cr6,r11,4103
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4103, so);
  // bne cr6,0x8214bbdc
  if (!cr6.eq) goto loc_8214BBDC;
  // addi r4,r29,3
  r4 = uint64_t(int64_t(int64_t(r29) + 3));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214BB28, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // ld r7,80(r1)
  r7 = guest::LoadU64(base, uint32_t(r1) + 80);
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29464
  r11 = uint64_t(int64_t(int64_t(r11) + 29464));
  // addi r30,r9,29448
  r30 = uint64_t(int64_t(int64_t(r9) + 29448));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // lwz r9,8(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmpld cr6,r7,r10
  cr6 = lifted::Compare<uint64_t>(r7, r10, so);
  // lwz r8,4(r30)
  r8 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r10,r10,30752
  r10 = uint64_t(int64_t(int64_t(r10) + 30752));
  // lwz r6,4(r10)
  r6 = guest::LoadU32(base, uint32_t(r10) + 4);
  // bne cr6,0x8214bbe4
  if (!cr6.eq) goto loc_8214BBE4;
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // beq cr6,0x8214bb74
  if (cr6.eq) goto loc_8214BB74;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r27,r11,30320
  r27 = uint64_t(int64_t(int64_t(r11) + 30320));
  // b 0x8214bc6c
  goto loc_8214BC6C;
loc_8214BB74:
  // mr r27,r11
  r27 = r11;
loc_8214BB78:
  // lbz r11,12(r27)
  r11 = guest::LoadU8(base, uint32_t(r27) + 12);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214bbdc
  if (cr0.eq) goto loc_8214BBDC;
  // addi r31,r27,256
  r31 = uint64_t(int64_t(int64_t(r27) + 256));
  // addi r30,r27,144
  r30 = uint64_t(int64_t(int64_t(r27) + 144));
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
loc_8214BB94:
  // ld r11,0(r30)
  r11 = guest::LoadU64(base, uint32_t(r30) + 0);
  // ld r10,88(r1)
  r10 = guest::LoadU64(base, uint32_t(r1) + 88);
  // cmpld cr6,r11,r10
  cr6 = lifted::Compare<uint64_t>(r11, r10, so);
  // bne cr6,0x8214bbc4
  if (!cr6.eq) goto loc_8214BBC4;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // lwz r10,29312(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 29312);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8214bbc4
  if (!cr6.eq) goto loc_8214BBC4;
  // li r3,4104
  r3 = uint64_t(int64_t(4104));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8214BBC4:
  // lbz r11,12(r27)
  r11 = guest::LoadU8(base, uint32_t(r27) + 12);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8214bb94
  if (cr6.lt) goto loc_8214BB94;
loc_8214BBDC:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_8214BBE4:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // cmpld cr6,r7,r10
  cr6 = lifted::Compare<uint64_t>(r7, r10, so);
  // bne cr6,0x8214bc00
  if (!cr6.eq) goto loc_8214BC00;
  // mr r27,r11
  r27 = r11;
  // b 0x8214bc6c
  goto loc_8214BC6C;
loc_8214BC00:
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // ble cr6,0x8214bc0c
  if (!cr6.gt) goto loc_8214BC0C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214BC0C:
  // mr r11,r8
  r11 = r8;
loc_8214BC10:
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // ble cr6,0x8214bc1c
  if (!cr6.gt) goto loc_8214BC1C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214BC1C:
  // subf r10,r11,r9
  r10 = r9 - r11;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm. r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8214bc64
  if (cr0.eq) goto loc_8214BC64;
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8214bc40
  if (cr6.lt) goto loc_8214BC40;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214BC40:
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // cmpld cr6,r10,r7
  cr6 = lifted::Compare<uint64_t>(r10, r7, so);
  // bne cr6,0x8214bc50
  if (!cr6.eq) goto loc_8214BC50;
  // mr r27,r11
  r27 = r11;
loc_8214BC50:
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8214bc5c
  if (cr6.lt) goto loc_8214BC5C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214BC5C:
  // addi r11,r11,384
  r11 = uint64_t(int64_t(int64_t(r11) + 384));
  // b 0x8214bc10
  goto loc_8214BC10;
loc_8214BC64:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8214bbdc
  if (cr6.eq) goto loc_8214BBDC;
loc_8214BC6C:
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // beq cr6,0x8214bb78
  if (cr6.eq) goto loc_8214BB78;
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // ble cr6,0x8214bc80
  if (!cr6.gt) goto loc_8214BC80;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214BC80:
  // mr r31,r8
  r31 = r8;
loc_8214BC84:
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // ble cr6,0x8214bc90
  if (!cr6.gt) goto loc_8214BC90;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214BC90:
  // subf r11,r31,r9
  r11 = r9 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214bb78
  if (cr0.eq) goto loc_8214BB78;
  // cmplw cr6,r31,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r9), so);
  // blt cr6,0x8214bcb4
  if (cr6.lt) goto loc_8214BCB4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214BCB4:
  // mr r3,r29
  r3 = r29;
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x8214BCC0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,8(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r31,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r9), so);
  // blt cr6,0x8214bcd0
  if (cr6.lt) goto loc_8214BCD0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214BCD0:
  // lwz r8,4(r30)
  r8 = guest::LoadU32(base, uint32_t(r30) + 4);
  // addi r31,r31,384
  r31 = uint64_t(int64_t(int64_t(r31) + 384));
  // b 0x8214bc84
  goto loc_8214BC84;
}

// sub_8214BCE0: lifted from generated/ by lift.py.
lifted::Result f8214BCE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8214BCE0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // lis r11,8191
  r11 = uint64_t(int64_t(536805376));
  // mr r24,r3
  r24 = r3;
  // ori r11,r11,65534
  r11 = r11 | 65534;
  // mr r26,r5
  r26 = r5;
  // lwz r10,8(r29)
  r10 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).size_address());
  // mr r31,r6
  r31 = r6;
  // mr r27,r7
  r27 = r7;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // blt cr6,0x8214bd58
  if (cr6.lt) goto loc_8214BD58;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,3352
  r4 = uint64_t(int64_t(int64_t(r11) + 3352));
  // bl 0x825931d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825931D0(base, LIFTED_ARGS(0x8214BD24, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x820919b8
  // (left out: dead in C++)
  { const lifted::Result o = f820919B8(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82091200
  // (left out: dead in C++)
  { const lifted::Result o = f82091200(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r11,r11,3300
  r11 = uint64_t(int64_t(int64_t(r11) + 3300));
  // stw r11,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r11));
  // bl 0x820915f8
  // (left out: dead in C++)
  { const lifted::Result o = f820915F8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82091328
  // (left out: dead in C++)
  { const lifted::Result o = game::Std::string::_Tidy(base, LIFTED_ARGS(0x8214BD58, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214BD58:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r28,4(r29)
  r28 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).head_address());
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x82149a30
  // (left out: dead in C++)
  { const lifted::Result o = f82149A30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x8214bd90
  if (cr0.eq) goto loc_8214BD90;
  // stw r28,0(r30)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r30)).left_address(), uint32_t(r28));
  // stw r31,4(r30)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r30)).parent_address(), uint32_t(r31));
  // stw r28,8(r30)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r30)).right_address(), uint32_t(r28));
  // ld r11,0(r27)
  r11 = guest::LoadU64(base, schema::SysLinkId(base, uint32_t(r27)).id_address());
  // stb r25,24(r30)
  guest::StoreU8(base, schema::SysLinkIdSetNode(base, uint32_t(r30)).color_address(), uint8_t(r25));
  // stb r25,25(r30)
  guest::StoreU8(base, schema::SysLinkIdSetNode(base, uint32_t(r30)).is_nil_address(), uint8_t(r25));
  // std r11,16(r30)
  guest::StoreU64(base, schema::SysLinkIdSetNode(base, uint32_t(r30)).id_address(), r11);
loc_8214BD90:
  // lwz r10,8(r29)
  r10 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).size_address());
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).head_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // stw r10,8(r29)
  guest::StoreU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).size_address(), uint32_t(r10));
  // bne cr6,0x8214bdbc
  if (!cr6.eq) goto loc_8214BDBC;
  // stw r30,4(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address(), uint32_t(r30));
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).head_address());
  // stw r30,0(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address(), uint32_t(r30));
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).head_address());
  // b 0x8214bdf4
  goto loc_8214BDF4;
loc_8214BDBC:
  // clrlwi. r11,r26,24
  r11 = uint32_t(r26) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214bde0
  if (cr0.eq) goto loc_8214BDE0;
  // stw r30,0(r31)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r31)).left_address(), uint32_t(r30));
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).head_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // bne cr6,0x8214bdf8
  if (!cr6.eq) goto loc_8214BDF8;
  // stw r30,0(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address(), uint32_t(r30));
  // b 0x8214bdf8
  goto loc_8214BDF8;
loc_8214BDE0:
  // stw r30,8(r31)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r31)).right_address(), uint32_t(r30));
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).head_address());
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // bne cr6,0x8214bdf8
  if (!cr6.eq) goto loc_8214BDF8;
loc_8214BDF4:
  // stw r30,8(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address(), uint32_t(r30));
loc_8214BDF8:
  // addi r11,r30,4
  r11 = uint64_t(int64_t(int64_t(r30) + 4));
  // mr r31,r30
  r31 = r30;
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // b 0x8214beec
  goto loc_8214BEEC;
loc_8214BE08:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11) - 4).parent_address());
  // rotlwi r8,r10,0
  r8 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r9,4(r10)
  r9 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address());
  // lwz r7,0(r9)
  r7 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r9)).left_address());
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // bne cr6,0x8214be74
  if (!cr6.eq) goto loc_8214BE74;
  // lwz r9,8(r9)
  r9 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r9)).right_address());
  // lbz r8,24(r9)
  r8 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r9)).color_address());
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8214be84
  if (cr6.eq) goto loc_8214BE84;
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).right_address());
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x8214be4c
  if (!cr6.eq) goto loc_8214BE4C;
  // mr r31,r10
  r31 = r10;
  // mr r3,r29
  r3 = r29;
  // mr r4,r31
  r4 = r31;
  // bl 0x82149888
  // (left out: dead in C++)
  f82149888(base, r3, r4);
loc_8214BE4C:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r31)).parent_address());
  // mr r3,r29
  r3 = r29;
  // stb r28,24(r11)
  guest::StoreU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).color_address(), uint8_t(r28));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r31)).parent_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address());
  // stb r25,24(r11)
  guest::StoreU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).color_address(), uint8_t(r25));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r31)).parent_address());
  // lwz r4,4(r11)
  r4 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address());
  // bl 0x821498f0
  // (left out: dead in C++)
  f821498F0(base, r3, r4);
  // b 0x8214bee8
  goto loc_8214BEE8;
loc_8214BE74:
  // lwz r9,0(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 0);
  // lbz r8,24(r9)
  r8 = guest::LoadU8(base, uint32_t(r9) + 24);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // bne cr6,0x8214bea8
  if (!cr6.eq) goto loc_8214BEA8;
loc_8214BE84:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stb r28,24(r10)
  guest::StoreU8(base, uint32_t(r10) + 24, uint8_t(r28));
  // stb r28,24(r9)
  guest::StoreU8(base, uint32_t(r9) + 24, uint8_t(r28));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r10,4(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 4);
  // stb r25,24(r10)
  guest::StoreU8(base, uint32_t(r10) + 24, uint8_t(r25));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r31,4(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 4);
  // b 0x8214bee8
  goto loc_8214BEE8;
loc_8214BEA8:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x8214bec4
  if (!cr6.eq) goto loc_8214BEC4;
  // mr r31,r10
  r31 = r10;
  // mr r3,r29
  r3 = r29;
  // mr r4,r31
  r4 = r31;
  // bl 0x821498f0
  // (left out: dead in C++)
  f821498F0(base, r3, r4);
loc_8214BEC4:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // mr r3,r29
  r3 = r29;
  // stb r28,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r28));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stb r25,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r25));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // lwz r4,4(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 4);
  // bl 0x82149888
  // (left out: dead in C++)
  f82149888(base, r3, r4);
loc_8214BEE8:
  // addi r11,r31,4
  r11 = uint64_t(int64_t(int64_t(r31) + 4));
loc_8214BEEC:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lbz r10,24(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 24);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214be08
  if (cr6.eq) goto loc_8214BE08;
  // lwz r11,4(r29)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r29)).head_address());
  // mr r3,r24
  r3 = r24;
  // stw r30,4(r24)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r24)).node_address(), uint32_t(r30));
  // stw r29,0(r24)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r24)).container_address(), uint32_t(r29));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address());
  // stb r28,24(r11)
  guest::StoreU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).color_address(), uint8_t(r28));
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214C708: lifted from generated/ by lift.py.
lifted::Result f8214C708(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8214C708 (lifted)");
  uint64_t r6 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // mr r31,r3
  r31 = r3;
  // mr r27,r5
  r27 = r5;
  // mr r28,r26
  r28 = r26;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r30)).node_address());
  // mr r29,r11
  r29 = r11;
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address());
  // lbz r11,25(r10)
  r11 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).is_nil_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8214c77c
  if (!cr6.eq) goto loc_8214C77C;
  // ld r9,0(r27)
  r9 = guest::LoadU64(base, schema::SysLinkId(base, uint32_t(r27)).id_address());
loc_8214C744:
  // ld r11,16(r10)
  r11 = guest::LoadU64(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).id_address());
  // mr r29,r10
  r29 = r10;
  // cmpld cr6,r9,r11
  cr6 = lifted::Compare<uint64_t>(r9, r11, so);
  // mr r11,r26
  r11 = r26;
  // blt cr6,0x8214c75c
  if (cr6.lt) goto loc_8214C75C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8214C75C:
  // clrlwi. r28,r11,24
  r28 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq 0x8214c76c
  if (cr0.eq) goto loc_8214C76C;
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).left_address());
  // b 0x8214c770
  goto loc_8214C770;
loc_8214C76C:
  // lwz r10,8(r10)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).right_address());
loc_8214C770:
  // lbz r11,25(r10)
  r11 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).is_nil_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214c744
  if (cr6.eq) goto loc_8214C744;
loc_8214C77C:
  // clrlwi. r11,r28,24
  r11 = uint32_t(r28) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // stw r30,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r30));
  // beq 0x8214c7dc
  if (cr0.eq) goto loc_8214C7DC;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r30)).node_address());
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
  // subf r11,r29,r11
  r11 = r11 - r29;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214c7d8
  if (cr0.eq) goto loc_8214C7D8;
  // li r5,1
  r5 = uint64_t(int64_t(1));
loc_8214C7AC:
  // mr r4,r30
  r4 = r30;
  // mr r6,r29
  r6 = r29;
  // mr r7,r27
  r7 = r27;
  // bl 0x8214bce0
  // (left out: dead in C++)
  { const lifted::Result o = f8214BCE0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // stb r26,8(r31)
  guest::StoreU8(base, schema::SysLinkIdSetInsertResult(base, uint32_t(r31)).inserted_address(), uint8_t(r26));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r11)).container_address());
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r11)).node_address());
  // stw r10,0(r31)
  guest::StoreU32(base, schema::SysLinkIdSetInsertResult(base, uint32_t(r31)).container_address(), uint32_t(r10));
  // stw r11,4(r31)
  guest::StoreU32(base, schema::SysLinkIdSetInsertResult(base, uint32_t(r31)).node_address(), uint32_t(r11));
  // b 0x8214c81c
  goto loc_8214C81C;
loc_8214C7D8:
  // bl 0x821496f8
  // (left out: dead in C++)
  f821496F8(base, r3);
loc_8214C7DC:
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // ld r10,0(r27)
  r10 = guest::LoadU64(base, schema::SysLinkId(base, uint32_t(r27)).id_address());
  // ld r11,16(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 16);
  // cmpld cr6,r11,r10
  cr6 = lifted::Compare<uint64_t>(r11, r10, so);
  // mr r11,r26
  r11 = r26;
  // blt cr6,0x8214c7f8
  if (cr6.lt) goto loc_8214C7F8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8214C7F8:
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214c80c
  if (cr0.eq) goto loc_8214C80C;
  // mr r5,r28
  r5 = r28;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // b 0x8214c7ac
  goto loc_8214C7AC;
loc_8214C80C:
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // std r11,0(r31)
  guest::StoreU64(base, schema::SysLinkIdSetInsertResult(base, uint32_t(r31)).whole_address(), r11);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stb r11,8(r31)
  guest::StoreU8(base, schema::SysLinkIdSetInsertResult(base, uint32_t(r31)).inserted_address(), uint8_t(r11));
loc_8214C81C:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214C9A0: lifted from generated/ by lift.py.
lifted::Result f8214C9A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8214C9A0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-544(r1)
  ea = -544 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // std r4,568(r1)
  guest::StoreU64(base, uint32_t(r1) + 568, r4);
  // mr r25,r5
  r25 = r5;
  // mr r4,r6
  r4 = r6;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214C9C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).first_address());
  // li r24,384
  r24 = uint64_t(int64_t(384));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8214c9e0
  if (!cr0.eq) goto loc_8214C9E0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // b 0x8214c9ec
  goto loc_8214C9EC;
loc_8214C9E0:
  // lwz r10,12(r31)
  r10 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).end_address());
  // subf r10,r11,r10
  r10 = r10 - r11;
  // divw r8,r10,r24
  r8 = uint32_t((int32_t(r24) && !(int32_t(r10) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r10) / int32_t(r24) : 0);
loc_8214C9EC:
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x8214ccac
  if (cr6.eq) goto loc_8214CCAC;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8214ca04
  if (!cr6.eq) goto loc_8214CA04;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // b 0x8214ca10
  goto loc_8214CA10;
loc_8214CA04:
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // subf r10,r11,r10
  r10 = r10 - r11;
  // divw r10,r10,r24
  r10 = uint32_t((int32_t(r24) && !(int32_t(r10) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r10) / int32_t(r24) : 0);
loc_8214CA10:
  // lis r9,170
  r9 = uint64_t(int64_t(11141120));
  // ori r9,r9,43690
  r9 = r9 | 43690;
  // subf r10,r10,r9
  r10 = r9 - r10;
  // cmplw cr6,r10,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r25), so);
  // bge cr6,0x8214ca2c
  if (!cr6.lt) goto loc_8214CA2C;
  // bl 0x820a92d0
  // (left out: dead in C++)
  { const lifted::Result o = f820A92D0(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ccac
  goto loc_8214CCAC;
loc_8214CA2C:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8214ca3c
  if (!cr6.eq) goto loc_8214CA3C;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // b 0x8214ca48
  goto loc_8214CA48;
loc_8214CA3C:
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // subf r10,r11,r10
  r10 = r10 - r11;
  // divw r10,r10,r24
  r10 = uint32_t((int32_t(r24) && !(int32_t(r10) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r10) / int32_t(r24) : 0);
loc_8214CA48:
  // add r10,r10,r25
  r10 = r10 + r25;
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // bge cr6,0x8214cb94
  if (!cr6.lt) goto loc_8214CB94;
  // rlwinm r10,r8,31,1,31
  r10 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 31) & 0x7FFFFFFF;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // subf r9,r10,r9
  r9 = r9 - r10;
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x8214ca6c
  if (cr6.lt) goto loc_8214CA6C;
  // add r26,r10,r8
  r26 = r10 + r8;
loc_8214CA6C:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8214ca7c
  if (!cr6.eq) goto loc_8214CA7C;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // b 0x8214ca88
  goto loc_8214CA88;
loc_8214CA7C:
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // subf r10,r11,r10
  r10 = r10 - r11;
  // divw r10,r10,r24
  r10 = uint32_t((int32_t(r24) && !(int32_t(r10) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r10) / int32_t(r24) : 0);
loc_8214CA88:
  // add r10,r10,r25
  r10 = r10 + r25;
  // cmplw cr6,r26,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r10), so);
  // bge cr6,0x8214caac
  if (!cr6.lt) goto loc_8214CAAC;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214caa8
  if (cr6.eq) goto loc_8214CAA8;
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // subf r11,r11,r10
  r11 = r10 - r11;
  // divw r11,r11,r24
  r11 = uint32_t((int32_t(r24) && !(int32_t(r11) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r11) / int32_t(r24) : 0);
loc_8214CAA8:
  // add r26,r11,r25
  r26 = r11 + r25;
loc_8214CAAC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // bl 0x821499b8
  // (left out: dead in C++)
  { const lifted::Result o = f821499B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // lwz r30,4(r31)
  r30 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).first_address());
  // lwz r28,572(r1)
  r28 = guest::LoadU32(base, uint32_t(r1) + 572);
  // mr r29,r27
  r29 = r27;
  // b 0x8214caec
  goto loc_8214CAEC;
loc_8214CACC:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8214cae4
  if (cr6.eq) goto loc_8214CAE4;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214CAE4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214CAE4:
  // addi r30,r30,384
  r30 = uint64_t(int64_t(int64_t(r30) + 384));
  // addi r29,r29,384
  r29 = uint64_t(int64_t(int64_t(r29) + 384));
loc_8214CAEC:
  // cmplw cr6,r30,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r28), so);
  // bne cr6,0x8214cacc
  if (!cr6.eq) goto loc_8214CACC;
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r5,r25
  r5 = r25;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8214b200
  // (left out: dead in C++)
  r3 = f8214B200(base, r1, r13, r4, r5, r6);
  // lwz r29,8(r31)
  r29 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // mr r30,r3
  r30 = r3;
  // cmplw cr6,r28,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r29), so);
  // beq cr6,0x8214cb44
  if (cr6.eq) goto loc_8214CB44;
  // subf r28,r3,r28
  r28 = r28 - r3;
loc_8214CB1C:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x8214cb34
  if (cr6.eq) goto loc_8214CB34;
  // add r4,r28,r30
  r4 = r28 + r30;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214CB34, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214CB34:
  // addi r30,r30,384
  r30 = uint64_t(int64_t(int64_t(r30) + 384));
  // add r11,r28,r30
  r11 = r28 + r30;
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x8214cb1c
  if (!cr6.eq) goto loc_8214CB1C;
loc_8214CB44:
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).first_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x8214cb58
  if (!cr0.eq) goto loc_8214CB58;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x8214cb64
  goto loc_8214CB64;
loc_8214CB58:
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // subf r11,r3,r11
  r11 = r11 - r3;
  // divw r11,r11,r24
  r11 = uint32_t((int32_t(r24) && !(int32_t(r11) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r11) / int32_t(r24) : 0);
loc_8214CB64:
  // add r30,r11,r25
  r30 = r11 + r25;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8214cb74
  if (cr6.eq) goto loc_8214CB74;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8214CB74, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214CB74:
  // mulli r10,r26,384
  r10 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(384))));
  // stw r27,4(r31)
  guest::StoreU32(base, schema::SysLinkVector(base, uint32_t(r31)).first_address(), uint32_t(r27));
  // mulli r11,r30,384
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(384))));
  // add r10,r10,r27
  r10 = r10 + r27;
  // add r11,r11,r27
  r11 = r11 + r27;
  // stw r10,12(r31)
  guest::StoreU32(base, schema::SysLinkVector(base, uint32_t(r31)).end_address(), uint32_t(r10));
  // stw r11,8(r31)
  guest::StoreU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address(), uint32_t(r11));
  // b 0x8214ccac
  goto loc_8214CCAC;
loc_8214CB94:
  // lwz r29,8(r31)
  r29 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // lwz r30,572(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 572);
  // subf r11,r30,r29
  r11 = r29 - r30;
  // divw r11,r11,r24
  r11 = uint32_t((int32_t(r24) && !(int32_t(r11) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r11) / int32_t(r24) : 0);
  // cmplw cr6,r11,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r25), so);
  // bge cr6,0x8214cc38
  if (!cr6.lt) goto loc_8214CC38;
  // mulli r28,r25,384
  r28 = uint64_t(int64_t(static_cast<int64_t>(r25 * static_cast<uint64_t>(384))));
  // add r27,r28,r30
  r27 = r28 + r30;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // beq cr6,0x8214cbe8
  if (cr6.eq) goto loc_8214CBE8;
  // subf r26,r28,r27
  r26 = r27 - r28;
loc_8214CBC0:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8214cbd8
  if (cr6.eq) goto loc_8214CBD8;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // mr r4,r26
  r4 = r26;
  // mr r3,r27
  r3 = r27;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214CBD8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214CBD8:
  // addi r26,r26,384
  r26 = uint64_t(int64_t(int64_t(r26) + 384));
  // addi r27,r27,384
  r27 = uint64_t(int64_t(int64_t(r27) + 384));
  // cmplw cr6,r26,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r29), so);
  // bne cr6,0x8214cbc0
  if (!cr6.eq) goto loc_8214CBC0;
loc_8214CBE8:
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // subf r11,r30,r4
  r11 = r4 - r30;
  // divw r11,r11,r24
  r11 = uint32_t((int32_t(r24) && !(int32_t(r11) == INT32_MIN && int32_t(r24) == -1)) ? int32_t(r11) / int32_t(r24) : 0);
  // subf r5,r11,r25
  r5 = r25 - r11;
  // bl 0x8214b200
  // (left out: dead in C++)
  r3 = f8214B200(base, r1, r13, r4, r5, r6);
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // add r11,r28,r11
  r11 = r28 + r11;
  // subf r29,r28,r11
  r29 = r11 - r28;
  // stw r11,8(r31)
  guest::StoreU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address(), uint32_t(r11));
  // b 0x8214cc2c
  goto loc_8214CC2C;
loc_8214CC18:
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214CC28, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,384
  r30 = uint64_t(int64_t(int64_t(r30) + 384));
loc_8214CC2C:
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // bne cr6,0x8214cc18
  if (!cr6.eq) goto loc_8214CC18;
  // b 0x8214ccac
  goto loc_8214CCAC;
loc_8214CC38:
  // mulli r26,r25,384
  r26 = uint64_t(int64_t(static_cast<int64_t>(r25 * static_cast<uint64_t>(384))));
  // lwz r27,8(r31)
  r27 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // subf r25,r26,r29
  r25 = r29 - r26;
  // mr r28,r25
  r28 = r25;
  // b 0x8214cc6c
  goto loc_8214CC6C;
loc_8214CC4C:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8214cc64
  if (cr6.eq) goto loc_8214CC64;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // mr r4,r28
  r4 = r28;
  // mr r3,r27
  r3 = r27;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214CC64, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214CC64:
  // addi r28,r28,384
  r28 = uint64_t(int64_t(int64_t(r28) + 384));
  // addi r27,r27,384
  r27 = uint64_t(int64_t(int64_t(r27) + 384));
loc_8214CC6C:
  // cmplw cr6,r28,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r29), so);
  // bne cr6,0x8214cc4c
  if (!cr6.eq) goto loc_8214CC4C;
  // mr r5,r29
  r5 = r29;
  // stw r27,8(r31)
  guest::StoreU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address(), uint32_t(r27));
  // mr r4,r25
  r4 = r25;
  // mr r3,r30
  r3 = r30;
  // bl 0x82149d60
  // (left out: dead in C++)
  r3 = f82149D60(base, r1, r13, r3, r4, r5);
  // add r31,r26,r30
  r31 = r26 + r30;
  // b 0x8214cca4
  goto loc_8214CCA4;
loc_8214CC90:
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214CCA0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,384
  r30 = uint64_t(int64_t(int64_t(r30) + 384));
loc_8214CCA4:
  // cmplw cr6,r30,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r31), so);
  // bne cr6,0x8214cc90
  if (!cr6.eq) goto loc_8214CC90;
loc_8214CCAC:
  // addi r1,r1,544
  r1 = uint64_t(int64_t(int64_t(r1) + 544));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214CCB8: lifted from generated/ by lift.py.
lifted::Result f8214CCB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8214CCB8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r5,r4
  r5 = r4;
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x8214a9b0
  // (left out: dead in C++)
  r3 = f8214A9B0(base, r1, r13, r3, r4, r5);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ld r30,104(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + 104);
  // ld r29,96(r1)
  r29 = guest::LoadU64(base, uint32_t(r1) + 96);
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // lbz r6,0(r11)
  r6 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // bl 0x82149b68
  // (left out: dead in C++)
  r3 = f82149B68(base, r1, r13, r3, r4, r5);
  // mr r6,r30
  r6 = r30;
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x8214c828
  // (left out: dead in C++)
  { const lifted::Result o = f8214C828(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,84(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 84);
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214CD20: lifted from generated/ by lift.py.
lifted::Result f8214CD20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8214CD20 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // mr r4,r5
  r4 = r5;
  // mr r29,r3
  r29 = r3;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).first_address());
  // std r4,160(r1)
  guest::StoreU64(base, uint32_t(r1) + 160, r4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214cd5c
  if (cr0.eq) goto loc_8214CD5C;
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // li r8,384
  r8 = uint64_t(int64_t(384));
  // subf r9,r11,r10
  r9 = r10 - r11;
  // divw. r9,r9,r8
  r9 = uint32_t((int32_t(r8) && !(int32_t(r9) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r9) / int32_t(r8) : 0);
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x8214cd64
  if (!cr0.eq) goto loc_8214CD64;
loc_8214CD5C:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // b 0x8214cd94
  goto loc_8214CD94;
loc_8214CD64:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214cd70
  if (!cr6.gt) goto loc_8214CD70;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214CD70:
  // lwz r10,160(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 160);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214cd84
  if (cr6.eq) goto loc_8214CD84;
  // cmplw cr6,r10,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r31), so);
  // beq cr6,0x8214cd88
  if (cr6.eq) goto loc_8214CD88;
loc_8214CD84:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214CD88:
  // lwz r10,164(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 164);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // divw r30,r11,r8
  r30 = uint32_t((int32_t(r8) && !(int32_t(r11) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r11) / int32_t(r8) : 0);
loc_8214CD94:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8214c9a0
  // (left out: dead in C++)
  { const lifted::Result o = f8214C9A0(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).first_address());
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214cdb4
  if (!cr6.gt) goto loc_8214CDB4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214CDB4:
  // mulli r10,r30,384
  r10 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(384))));
  // stw r31,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r31));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // add r10,r10,r11
  r10 = r10 + r11;
  // lwz r9,8(r31)
  r9 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // bgt cr6,0x8214cde4
  if (cr6.gt) goto loc_8214CDE4;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).first_address());
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x8214cde8
  if (!cr6.lt) goto loc_8214CDE8;
loc_8214CDE4:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214CDE8:
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // mr r3,r29
  r3 = r29;
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // std r11,0(r29)
  guest::StoreU64(base, schema::SysLinkId(base, uint32_t(r29)).id_address(), r11);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214CE00: lifted from generated/ by lift.py.
lifted::Result f8214CE00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214CE00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-496(r1)
  ea = -496 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r4,r3,3
  r4 = uint64_t(int64_t(int64_t(r3) + 3));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,301
  r5 = uint64_t(int64_t(301));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214CE1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r23,4
  r23 = uint64_t(int64_t(4));
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // addi r22,r11,144
  r22 = uint64_t(int64_t(int64_t(r11) + 144));
  // addi r24,r11,300
  r24 = uint64_t(int64_t(int64_t(r11) + 300));
  // addi r25,r11,384
  r25 = uint64_t(int64_t(int64_t(r11) + 384));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r21,r11,29856
  r21 = uint64_t(int64_t(int64_t(r11) + 29856));
loc_8214CE3C:
  // lbz r11,104(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 104);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214ced4
  if (cr0.eq) goto loc_8214CED4;
  // addi r30,r1,105
  r30 = uint64_t(int64_t(int64_t(r1) + 105));
  // addi r26,r1,141
  r26 = uint64_t(int64_t(int64_t(r1) + 141));
loc_8214CE54:
  // mr r4,r30
  r4 = r30;
  // mr r3,r25
  r3 = r25;
  // bl 0x8214ccb8
  // (left out: dead in C++)
  { const lifted::Result o = f8214CCB8(base, r1, r13, r3, r4, r7, f1); r3 = o.r3; f1 = o.f1; }
  // addi r27,r1,137
  r27 = uint64_t(int64_t(int64_t(r1) + 137));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lbzx r11,r31,r27
  r11 = guest::LoadU8(base, uint32_t(r31) + uint32_t(r27));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214ceac
  if (cr0.eq) goto loc_8214CEAC;
  // mr r28,r26
  r28 = r26;
loc_8214CE78:
  // ld r11,0(r22)
  r11 = guest::LoadU64(base, uint32_t(r22) + 0);
  // ld r10,0(r28)
  r10 = guest::LoadU64(base, uint32_t(r28) + 0);
  // cmpld cr6,r11,r10
  cr6 = lifted::Compare<uint64_t>(r11, r10, so);
  // bne cr6,0x8214ce98
  if (!cr6.eq) goto loc_8214CE98;
  // mr r5,r30
  r5 = r30;
  // mr r4,r25
  r4 = r25;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8214c708
  // (left out: dead in C++)
  { const lifted::Result o = f8214C708(base, r1, r13, r3, r4, r5, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_8214CE98:
  // lbzx r11,r31,r27
  r11 = guest::LoadU8(base, uint32_t(r31) + uint32_t(r27));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r28,r28,8
  r28 = uint64_t(int64_t(int64_t(r28) + 8));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8214ce78
  if (cr6.lt) goto loc_8214CE78;
loc_8214CEAC:
  // mr r3,r21
  r3 = r21;
  // ld r5,0(r30)
  r5 = guest::LoadU64(base, uint32_t(r30) + 0);
  // lwz r4,0(r24)
  r4 = guest::LoadU32(base, uint32_t(r24) + 0);
  // bl 0x8214ff78
  // (left out: dead in C++)
  { const lifted::Result o = f8214FF78(base, r1, r13, r3, r4, r5, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lbz r11,104(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 104);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r26,r26,64
  r26 = uint64_t(int64_t(int64_t(r26) + 64));
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x8214ce54
  if (cr6.lt) goto loc_8214CE54;
loc_8214CED4:
  // addic. r23,r23,-1
  ca = uint32_t(r23) > 0;
  r23 = uint64_t(int64_t(int64_t(r23) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // addi r25,r25,12
  r25 = uint64_t(int64_t(int64_t(r25) + 12));
  // addi r24,r24,4
  r24 = uint64_t(int64_t(int64_t(r24) + 4));
  // addi r22,r22,8
  r22 = uint64_t(int64_t(int64_t(r22) + 8));
  // bne 0x8214ce3c
  if (!cr0.eq) goto loc_8214CE3C;
  // addi r1,r1,496
  r1 = uint64_t(int64_t(int64_t(r1) + 496));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214CEF0: lifted from generated/ by lift.py.
lifted::Result f8214CEF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8214CEF0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r10,384
  r10 = uint64_t(int64_t(384));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).first_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8214cf20
  if (!cr0.eq) goto loc_8214CF20;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // b 0x8214cf2c
  goto loc_8214CF2C;
loc_8214CF20:
  // lwz r9,8(r31)
  r9 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // subf r9,r11,r9
  r9 = r9 - r11;
  // divw r9,r9,r10
  r9 = uint32_t((int32_t(r10) && !(int32_t(r9) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r9) / int32_t(r10) : 0);
loc_8214CF2C:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214cf6c
  if (cr6.eq) goto loc_8214CF6C;
  // lwz r8,12(r31)
  r8 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).end_address());
  // subf r8,r11,r8
  r8 = r8 - r11;
  // divw r10,r8,r10
  r10 = uint32_t((int32_t(r10) && !(int32_t(r8) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r8) / int32_t(r10) : 0);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // bge cr6,0x8214cf6c
  if (!cr6.lt) goto loc_8214CF6C;
  // lwz r30,8(r31)
  r30 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // cmplwi r30,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq 0x8214cf60
  if (cr0.eq) goto loc_8214CF60;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214CF60, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214CF60:
  // addi r11,r30,384
  r11 = uint64_t(int64_t(int64_t(r30) + 384));
  // stw r11,8(r31)
  guest::StoreU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address(), uint32_t(r11));
  // b 0x8214cf9c
  goto loc_8214CF9C;
loc_8214CF6C:
  // lwz r10,8(r31)
  r10 = guest::LoadU32(base, schema::SysLinkVector(base, uint32_t(r31)).last_address());
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214cf7c
  if (!cr6.gt) goto loc_8214CF7C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214CF7C:
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r31,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r31));
  // mr r6,r4
  r6 = r4;
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r5,0(r11)
  r5 = guest::LoadU64(base, uint32_t(r11) + 0);
  // bl 0x8214cd20
  // (left out: dead in C++)
  { const lifted::Result o = f8214CD20(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_8214CF9C:
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

// sub_8214CFB8: lifted from generated/ by lift.py.
lifted::Result f8214CFB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_8214CFB8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -4096);
  // stwu r1,-4384(r1)
  ea = -4384 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r25,r4
  r25 = r4;
  // addi r28,r3,3
  r28 = uint64_t(int64_t(int64_t(r3) + 3));
  // lwz r11,27304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27304);
  // li r24,2
  r24 = uint64_t(int64_t(2));
  // stb r27,466(r1)
  guest::StoreU8(base, uint32_t(r1) + 466, uint8_t(r27));
  // addi r26,r10,29856
  r26 = uint64_t(int64_t(int64_t(r10) + 29856));
  // cmpwi cr6,r11,4107
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4107, so);
  // beq cr6,0x8214d000
  if (cr6.eq) goto loc_8214D000;
  // addi r11,r11,-4096
  r11 = uint64_t(int64_t(int64_t(r11) + -4096));
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bgt cr6,0x8214d014
  if (cr6.gt) goto loc_8214D014;
loc_8214D000:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,30752
  r30 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8214d030
  if (!cr6.eq) goto loc_8214D030;
loc_8214D014:
  // mr r11,r24
  r11 = r24;
  // srawi r10,r11,24
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0xFFFFFF) != 0);
  r10 = uint64_t(int64_t(int32_t(r11) >> 24));
  // stb r11,470(r1)
  guest::StoreU8(base, uint32_t(r1) + 470, uint8_t(r11));
  // stb r10,467(r1)
  guest::StoreU8(base, uint32_t(r1) + 467, uint8_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,468(r1)
  guest::StoreU16(base, uint32_t(r1) + 468, uint16_t(r10));
  // b 0x8214d228
  goto loc_8214D228;
loc_8214D030:
  // lwz r11,124(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 124);
  // lwz r10,132(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 132);
  // lwz r7,189(r28)
  r7 = guest::LoadU32(base, schema::SysLinkPacket(base, uint32_t(r28) - 3).unknown_192_address());
  // subf r11,r10,r11
  r11 = r11 - r10;
  // lwz r9,128(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 128);
  // lwz r8,120(r30)
  r8 = guest::LoadU32(base, uint32_t(r30) + 120);
  // mullw r11,r11,r7
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r7))));
  // lbz r6,8(r28)
  r6 = guest::LoadU8(base, schema::SysLinkPacket(base, uint32_t(r28) - 3).unknown_11_address());
  // subf r11,r9,r11
  r11 = r11 - r9;
  // add r11,r11,r8
  r11 = r11 + r8;
  // cmplw cr6,r6,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r11), so);
  // ble cr6,0x8214d078
  if (!cr6.gt) goto loc_8214D078;
  // srawi r11,r27,24
  ca = (int32_t(r27) < 0) & ((uint32_t(r27) & 0xFFFFFF) != 0);
  r11 = uint64_t(int64_t(int32_t(r27) >> 24));
  // stb r27,470(r1)
  guest::StoreU8(base, uint32_t(r1) + 470, uint8_t(r27));
  // stb r11,467(r1)
  guest::StoreU8(base, uint32_t(r1) + 467, uint8_t(r11));
  // rldicl r11,r27,56,8
  r11 = __builtin_rotateleft64(r27, 56) & 0xFFFFFFFFFFFFFF;
  // sth r11,468(r1)
  guest::StoreU16(base, uint32_t(r1) + 468, uint16_t(r11));
  // b 0x8214d228
  goto loc_8214D228;
loc_8214D078:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // srawi r10,r11,24
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0xFFFFFF) != 0);
  r10 = uint64_t(int64_t(int32_t(r11) >> 24));
  // stb r11,470(r1)
  guest::StoreU8(base, uint32_t(r1) + 470, uint8_t(r11));
  // stb r10,467(r1)
  guest::StoreU8(base, uint32_t(r1) + 467, uint8_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // ld r11,72(r30)
  r11 = guest::LoadU64(base, uint32_t(r30) + 72);
  // sth r10,468(r1)
  guest::StoreU16(base, uint32_t(r1) + 468, uint16_t(r10));
  // rldicl r10,r11,8,56
  r10 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // stb r11,486(r1)
  guest::StoreU8(base, uint32_t(r1) + 486, uint8_t(r11));
  // stb r10,479(r1)
  guest::StoreU8(base, uint32_t(r1) + 479, uint8_t(r10));
  // rldicl r10,r11,40,24
  r10 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r10,480(r1)
  guest::StoreU32(base, uint32_t(r1) + 480, uint32_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // sth r10,484(r1)
  guest::StoreU16(base, uint32_t(r1) + 484, uint16_t(r10));
  // lwz r11,29312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29312);
  // rlwinm r10,r11,8,24,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFF;
  // stb r11,490(r1)
  guest::StoreU8(base, uint32_t(r1) + 490, uint8_t(r11));
  // stb r10,487(r1)
  guest::StoreU8(base, uint32_t(r1) + 487, uint8_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // sth r10,488(r1)
  guest::StoreU16(base, uint32_t(r1) + 488, uint16_t(r10));
  // lwz r11,29308(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29308);
  // rlwinm r10,r11,8,24,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFF;
  // stb r11,494(r1)
  guest::StoreU8(base, uint32_t(r1) + 494, uint8_t(r11));
  // stb r10,491(r1)
  guest::StoreU8(base, uint32_t(r1) + 491, uint8_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // sth r10,492(r1)
  guest::StoreU16(base, uint32_t(r1) + 492, uint16_t(r10));
  // lwz r11,29316(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29316);
  // rlwinm r10,r11,8,24,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFF;
  // stb r11,498(r1)
  guest::StoreU8(base, uint32_t(r1) + 498, uint8_t(r11));
  // stb r10,495(r1)
  guest::StoreU8(base, uint32_t(r1) + 495, uint8_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,496(r1)
  guest::StoreU16(base, uint32_t(r1) + 496, uint16_t(r10));
  // bl 0x8209f658
  // (left out: dead in C++)
  { const lifted::Result o = game::Players::SignedInSlot(base, LIFTED_ARGS(0x8214D10C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,628
  r3 = uint64_t(int64_t(int64_t(r1) + 628));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214D120, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,500
  r3 = uint64_t(int64_t(int64_t(r1) + 500));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214D130, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,684
  r3 = uint64_t(int64_t(int64_t(r1) + 684));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214D140, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,700
  r3 = uint64_t(int64_t(int64_t(r1) + 700));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214D150, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,716
  r3 = uint64_t(int64_t(int64_t(r1) + 716));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214D160, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,732
  r3 = uint64_t(int64_t(int64_t(r1) + 732));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214D170, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // stb r27,499(r1)
  guest::StoreU8(base, uint32_t(r1) + 499, uint8_t(r27));
  // addi r31,r11,30320
  r31 = uint64_t(int64_t(int64_t(r11) + 30320));
  // rlwinm r11,r29,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 5) & 0xFFFFFFE0;
  // addi r10,r31,14
  r10 = uint64_t(int64_t(int64_t(r31) + 14));
  // addi r3,r1,500
  r3 = uint64_t(int64_t(int64_t(r1) + 500));
  // add r4,r11,r10
  r4 = r11 + r10;
  // addi r10,r31,144
  r10 = uint64_t(int64_t(int64_t(r31) + 144));
  // rlwinm r11,r29,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 3) & 0xFFFFFFF8;
  // ldx r11,r11,r10
  r11 = guest::LoadU64(base, uint32_t(r11) + uint32_t(r10));
  // rldicl r10,r11,32,32
  r10 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFF;
  // stw r11,632(r1)
  guest::StoreU32(base, uint32_t(r1) + 632, uint32_t(r11));
  // stw r10,628(r1)
  guest::StoreU32(base, uint32_t(r1) + 628, uint32_t(r10));
  // bl 0x825a7830
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7830(base, LIFTED_ARGS(0x8214D1AC, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r31,316
  r10 = uint64_t(int64_t(int64_t(r31) + 316));
  // addi r9,r31,332
  r9 = uint64_t(int64_t(int64_t(r31) + 332));
  // addi r8,r31,348
  r8 = uint64_t(int64_t(int64_t(r31) + 348));
  // lwzx r10,r11,r10
  r10 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // addi r7,r31,364
  r7 = uint64_t(int64_t(int64_t(r31) + 364));
  // addi r4,r1,660
  r4 = uint64_t(int64_t(int64_t(r1) + 660));
  // mr r3,r26
  r3 = r26;
  // stw r10,684(r1)
  guest::StoreU32(base, uint32_t(r1) + 684, uint32_t(r10));
  // lwzx r10,r11,r9
  r10 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
  // stw r10,700(r1)
  guest::StoreU32(base, uint32_t(r1) + 700, uint32_t(r10));
  // lwzx r10,r11,r8
  r10 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r8));
  // lwzx r11,r11,r7
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r7));
  // stw r10,716(r1)
  guest::StoreU32(base, uint32_t(r1) + 716, uint32_t(r10));
  // stw r11,732(r1)
  guest::StoreU32(base, uint32_t(r1) + 732, uint32_t(r11));
  // bl 0x8214f910
  // (left out: dead in C++)
  f8214F910(base, r3, r4);
  // lwz r10,120(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 120);
  // lwz r11,124(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 124);
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,676(r1)
  guest::StoreU32(base, uint32_t(r1) + 676, uint32_t(r11));
  // ld r11,0(r31)
  r11 = guest::LoadU64(base, uint32_t(r31) + 0);
  // rldicl r10,r11,8,56
  r10 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // stb r11,478(r1)
  guest::StoreU8(base, uint32_t(r1) + 478, uint8_t(r11));
  // stb r10,471(r1)
  guest::StoreU8(base, uint32_t(r1) + 471, uint8_t(r10));
  // rldicl r10,r11,40,24
  r10 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r10,472(r1)
  guest::StoreU32(base, uint32_t(r1) + 472, uint32_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // lwz r11,80(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 80);
  // rlwinm r11,r11,0,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFE;
  // sth r10,476(r1)
  guest::StoreU16(base, uint32_t(r1) + 476, uint16_t(r10));
  // stw r11,680(r1)
  guest::StoreU32(base, uint32_t(r1) + 680, uint32_t(r11));
loc_8214D228:
  // mr r4,r25
  r4 = r25;
  // addi r3,r1,464
  r3 = uint64_t(int64_t(int64_t(r1) + 464));
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x8214D234, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,467(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 467);
  // lhz r10,468(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + 468);
  // rotlwi r11,r11,16
  r11 = __builtin_rotateleft32(uint32_t(r11), 16);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lbz r10,470(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 470);
  // rlwinm r11,r11,8,0,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFFFFFF00;
  // or r11,r10,r11
  r11 = r10 | r11;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8214d558
  if (!cr6.eq) goto loc_8214D558;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214D268, r31)); r3 = o.r3; f1 = o.f1; }
  // ld r11,0(r28)
  r11 = guest::LoadU64(base, schema::SysLinkPacket(base, uint32_t(r28) - 3).sender_id_address());
  // stw r25,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r25));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lbz r11,8(r28)
  r11 = guest::LoadU8(base, schema::SysLinkPacket(base, uint32_t(r28) - 3).unknown_11_address());
  // stb r11,92(r1)
  guest::StoreU8(base, uint32_t(r1) + 92, uint8_t(r11));
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8214D280, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,352(r1)
  guest::StoreU32(base, uint32_t(r1) + 352, uint32_t(r3));
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_8214D288:
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8214D28C, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r10,r31,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r9,r1,356
  r9 = uint64_t(int64_t(int64_t(r1) + 356));
  // addi r8,r3,-5000
  r8 = uint64_t(int64_t(int64_t(r3) + -5000));
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplwi cr6,r31,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 4, so);
  // stwx r8,r10,r9
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r9), uint32_t(r8));
  // blt cr6,0x8214d288
  if (cr6.lt) goto loc_8214D288;
  // li r6,32
  r6 = uint64_t(int64_t(32));
  // addi r5,r28,141
  r5 = uint64_t(int64_t(int64_t(r28) + 141));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // addi r3,r1,224
  r3 = uint64_t(int64_t(int64_t(r1) + 224));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D2C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,128
  r6 = uint64_t(int64_t(128));
  // addi r5,r28,9
  r5 = uint64_t(int64_t(int64_t(r28) + 9));
  // li r4,128
  r4 = uint64_t(int64_t(128));
  // addi r3,r1,94
  r3 = uint64_t(int64_t(int64_t(r1) + 94));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D2D4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r28,173
  r5 = uint64_t(int64_t(int64_t(r28) + 173));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D2E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r28,193
  r5 = uint64_t(int64_t(int64_t(r28) + 193));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,396
  r3 = uint64_t(int64_t(int64_t(r1) + 396));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D2FC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r28,209
  r5 = uint64_t(int64_t(int64_t(r28) + 209));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,412
  r3 = uint64_t(int64_t(int64_t(r1) + 412));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D310, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r28,225
  r5 = uint64_t(int64_t(int64_t(r28) + 225));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,428
  r3 = uint64_t(int64_t(int64_t(r1) + 428));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D324, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r28,241
  r5 = uint64_t(int64_t(int64_t(r28) + 241));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,444
  r3 = uint64_t(int64_t(int64_t(r1) + 444));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D338, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,189(r28)
  r11 = guest::LoadU32(base, schema::SysLinkPacket(base, uint32_t(r28) - 3).unknown_192_address());
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r26
  r3 = r26;
  // stw r11,372(r1)
  guest::StoreU32(base, uint32_t(r1) + 372, uint32_t(r11));
  // bl 0x8214f938
  // (left out: dead in C++)
  { const lifted::Result o = f8214F938(base, r1, r13, r3, r4, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8214b260
  // (left out: dead in C++)
  { const lifted::Result o = f8214B260(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lbz r11,92(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 92);
  // li r6,128
  r6 = uint64_t(int64_t(128));
  // stb r24,3026(r1)
  guest::StoreU8(base, uint32_t(r1) + 3026, uint8_t(r24));
  // addi r5,r1,94
  r5 = uint64_t(int64_t(int64_t(r1) + 94));
  // li r4,128
  r4 = uint64_t(int64_t(128));
  // addi r3,r1,3036
  r3 = uint64_t(int64_t(int64_t(r1) + 3036));
  // stb r11,3035(r1)
  guest::StoreU8(base, uint32_t(r1) + 3035, uint8_t(r11));
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // rldicl r10,r11,8,56
  r10 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // stb r11,3034(r1)
  guest::StoreU8(base, uint32_t(r1) + 3034, uint8_t(r11));
  // stb r10,3027(r1)
  guest::StoreU8(base, uint32_t(r1) + 3027, uint8_t(r10));
  // rldicl r10,r11,40,24
  r10 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r10,3028(r1)
  guest::StoreU32(base, uint32_t(r1) + 3028, uint32_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,3032(r1)
  guest::StoreU16(base, uint32_t(r1) + 3032, uint16_t(r10));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D394, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,32
  r6 = uint64_t(int64_t(32));
  // addi r5,r1,224
  r5 = uint64_t(int64_t(int64_t(r1) + 224));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // addi r3,r1,3164
  r3 = uint64_t(int64_t(int64_t(r1) + 3164));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D3A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,256
  r5 = uint64_t(int64_t(int64_t(r1) + 256));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,3196
  r3 = uint64_t(int64_t(int64_t(r1) + 3196));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D3BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,396
  r5 = uint64_t(int64_t(int64_t(r1) + 396));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,3212
  r3 = uint64_t(int64_t(int64_t(r1) + 3212));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D3D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,412
  r5 = uint64_t(int64_t(int64_t(r1) + 412));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,3228
  r3 = uint64_t(int64_t(int64_t(r1) + 3228));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D3E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,428
  r5 = uint64_t(int64_t(int64_t(r1) + 428));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,3244
  r3 = uint64_t(int64_t(int64_t(r1) + 3244));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D3F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,444
  r5 = uint64_t(int64_t(int64_t(r1) + 444));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,3260
  r3 = uint64_t(int64_t(int64_t(r1) + 3260));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D40C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,29448
  r30 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x8214d428
  if (!cr6.gt) goto loc_8214D428;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D428:
  // mr r31,r10
  r31 = r10;
loc_8214D42C:
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x8214d438
  if (!cr6.gt) goto loc_8214D438;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D438:
  // subf r10,r31,r11
  r10 = r11 - r31;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm. r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8214d548
  if (cr0.eq) goto loc_8214D548;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x8214d45c
  if (cr6.lt) goto loc_8214D45C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D45C:
  // stb r24,1746(r1)
  guest::StoreU8(base, uint32_t(r1) + 1746, uint8_t(r24));
  // li r6,128
  r6 = uint64_t(int64_t(128));
  // lbz r11,12(r31)
  r11 = guest::LoadU8(base, uint32_t(r31) + 12);
  // addi r5,r31,14
  r5 = uint64_t(int64_t(int64_t(r31) + 14));
  // li r4,128
  r4 = uint64_t(int64_t(128));
  // addi r3,r1,1756
  r3 = uint64_t(int64_t(int64_t(r1) + 1756));
  // stb r11,1755(r1)
  guest::StoreU8(base, uint32_t(r1) + 1755, uint8_t(r11));
  // ld r11,0(r31)
  r11 = guest::LoadU64(base, uint32_t(r31) + 0);
  // rldicl r10,r11,8,56
  r10 = __builtin_rotateleft64(r11, 8) & 0xFF;
  // stb r11,1754(r1)
  guest::StoreU8(base, uint32_t(r1) + 1754, uint8_t(r11));
  // stb r10,1747(r1)
  guest::StoreU8(base, uint32_t(r1) + 1747, uint8_t(r10));
  // rldicl r10,r11,40,24
  r10 = __builtin_rotateleft64(r11, 40) & 0xFFFFFFFFFF;
  // stw r10,1748(r1)
  guest::StoreU32(base, uint32_t(r1) + 1748, uint32_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,1752(r1)
  guest::StoreU16(base, uint32_t(r1) + 1752, uint16_t(r10));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D49C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,32
  r6 = uint64_t(int64_t(32));
  // addi r5,r31,144
  r5 = uint64_t(int64_t(int64_t(r31) + 144));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // addi r3,r1,1884
  r3 = uint64_t(int64_t(int64_t(r1) + 1884));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D4B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r31,176
  r5 = uint64_t(int64_t(int64_t(r31) + 176));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,1916
  r3 = uint64_t(int64_t(int64_t(r1) + 1916));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D4C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r31,316
  r5 = uint64_t(int64_t(int64_t(r31) + 316));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,1932
  r3 = uint64_t(int64_t(int64_t(r1) + 1932));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D4D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r31,332
  r5 = uint64_t(int64_t(int64_t(r31) + 332));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,1948
  r3 = uint64_t(int64_t(int64_t(r1) + 1948));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D4EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r31,348
  r5 = uint64_t(int64_t(int64_t(r31) + 348));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,1964
  r3 = uint64_t(int64_t(int64_t(r1) + 1964));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D500, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r31,364
  r5 = uint64_t(int64_t(int64_t(r31) + 364));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r1,1980
  r3 = uint64_t(int64_t(int64_t(r1) + 1980));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D514, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,1744
  r3 = uint64_t(int64_t(int64_t(r1) + 1744));
  // lwz r4,88(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 88);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x8214D520, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,3024
  r3 = uint64_t(int64_t(int64_t(r1) + 3024));
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x8214aa70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AA70(base, LIFTED_ARGS(0x8214D52C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x8214d53c
  if (cr6.lt) goto loc_8214D53C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D53C:
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 4);
  // addi r31,r31,384
  r31 = uint64_t(int64_t(int64_t(r31) + 384));
  // b 0x8214d42c
  goto loc_8214D42C;
loc_8214D548:
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // bl 0x8214cef0
  // (left out: dead in C++)
  { const lifted::Result o = f8214CEF0(base, r1, r13, r3, r4, r7, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x8214a198
  // (left out: dead in C++)
  f8214A198(base);
loc_8214D558:
  // addi r1,r1,4384
  r1 = uint64_t(int64_t(int64_t(r1) + 4384));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214D560: lifted from generated/ by lift.py.
lifted::Result f8214D560(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8214D560 (lifted)");
  uint64_t r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-768(r1)
  ea = -768 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r4,r3,3
  r4 = uint64_t(int64_t(int64_t(r3) + 3));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,249
  r5 = uint64_t(int64_t(249));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214D57C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r29,r11,29448
  r29 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r9,4(r29)
  r9 = guest::LoadU32(base, uint32_t(r29) + 4);
  // lwz r8,8(r29)
  r8 = guest::LoadU32(base, uint32_t(r29) + 8);
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // ble cr6,0x8214d598
  if (!cr6.gt) goto loc_8214D598;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D598:
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // stw r29,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r29));
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lwz r10,80(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r30,84(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 84);
loc_8214D5B0:
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // ble cr6,0x8214d5bc
  if (!cr6.gt) goto loc_8214D5BC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D5BC:
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214d5cc
  if (cr6.eq) goto loc_8214D5CC;
  // cmplw cr6,r10,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r29), so);
  // beq cr6,0x8214d5d0
  if (cr6.eq) goto loc_8214D5D0;
loc_8214D5CC:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D5D0:
  // subf r11,r30,r8
  r11 = r8 - r30;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214d644
  if (cr0.eq) goto loc_8214D644;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8214d5f4
  if (!cr6.eq) goto loc_8214D5F4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D5F4:
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8214d604
  if (cr6.lt) goto loc_8214D604;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D604:
  // ld r7,0(r30)
  r7 = guest::LoadU64(base, uint32_t(r30) + 0);
  // ld r6,96(r1)
  r6 = guest::LoadU64(base, uint32_t(r1) + 96);
  // cmpld cr6,r7,r6
  cr6 = lifted::Compare<uint64_t>(r7, r6, so);
  // beq cr6,0x8214d628
  if (cr6.eq) goto loc_8214D628;
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8214d620
  if (cr6.lt) goto loc_8214D620;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D620:
  // addi r30,r30,384
  r30 = uint64_t(int64_t(int64_t(r30) + 384));
  // b 0x8214d5b0
  goto loc_8214D5B0;
loc_8214D628:
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8214d638
  if (cr6.lt) goto loc_8214D638;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214D638:
  // mr r31,r30
  r31 = r30;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8214d67c
  if (!cr6.eq) goto loc_8214D67C;
loc_8214D644:
  // lbz r11,104(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 104);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214d790
  if (cr0.eq) goto loc_8214D790;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,352
  r3 = uint64_t(int64_t(int64_t(r1) + 352));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214D660, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,352
  r4 = uint64_t(int64_t(int64_t(r1) + 352));
  // mr r3,r29
  r3 = r29;
  // bl 0x8214cef0
  // (left out: dead in C++)
  { const lifted::Result o = f8214CEF0(base, r1, r13, r3, r4, r7, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x8214a8e0
  // (left out: dead in C++)
  r3 = f8214A8E0(base, r1, r13, r3);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x8214d790
  if (cr0.eq) goto loc_8214D790;
loc_8214D67C:
  // lbz r11,104(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 104);
  // mr. r10,r11
  r10 = r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8214d73c
  if (cr0.eq) goto loc_8214D73C;
  // stb r11,12(r31)
  guest::StoreU8(base, schema::SysLinkRecord(base, uint32_t(r31)).unknown_12_address(), uint8_t(r11));
  // li r6,128
  r6 = uint64_t(int64_t(128));
  // ld r11,96(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 96);
  // addi r5,r1,105
  r5 = uint64_t(int64_t(int64_t(r1) + 105));
  // li r4,128
  r4 = uint64_t(int64_t(128));
  // addi r3,r31,14
  r3 = uint64_t(int64_t(int64_t(r31) + 14));
  // std r11,0(r31)
  guest::StoreU64(base, schema::SysLinkRecord(base, uint32_t(r31)).unknown_0_address(), r11);
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D6A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,32
  r6 = uint64_t(int64_t(32));
  // addi r5,r1,233
  r5 = uint64_t(int64_t(int64_t(r1) + 233));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // addi r3,r31,144
  r3 = uint64_t(int64_t(int64_t(r31) + 144));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D6BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,265
  r5 = uint64_t(int64_t(int64_t(r1) + 265));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,176
  r3 = uint64_t(int64_t(int64_t(r31) + 176));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D6D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,281
  r5 = uint64_t(int64_t(int64_t(r1) + 281));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,316
  r3 = uint64_t(int64_t(int64_t(r31) + 316));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D6E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,297
  r5 = uint64_t(int64_t(int64_t(r1) + 297));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,332
  r3 = uint64_t(int64_t(int64_t(r31) + 332));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D6F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,313
  r5 = uint64_t(int64_t(int64_t(r1) + 313));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,348
  r3 = uint64_t(int64_t(int64_t(r31) + 348));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D70C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,16
  r6 = uint64_t(int64_t(16));
  // addi r5,r1,329
  r5 = uint64_t(int64_t(int64_t(r1) + 329));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,364
  r3 = uint64_t(int64_t(int64_t(r31) + 364));
  // bl 0x825a7270
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_s(base, LIFTED_ARGS(0x8214D720, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,29856
  r3 = uint64_t(int64_t(int64_t(r11) + 29856));
  // bl 0x8214f938
  // (left out: dead in C++)
  { const lifted::Result o = f8214F938(base, r1, r13, r3, r4, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8214b260
  // (left out: dead in C++)
  { const lifted::Result o = f8214B260(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214d790
  goto loc_8214D790;
loc_8214D73C:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,29856
  r3 = uint64_t(int64_t(int64_t(r11) + 29856));
  // bl 0x8214fa20
  // (left out: dead in C++)
  { const lifted::Result o = f8214FA20(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r4,r31
  r4 = r31;
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,30752
  r3 = uint64_t(int64_t(int64_t(r11) + 30752));
  // bne cr6,0x8214d770
  if (!cr6.eq) goto loc_8214D770;
  // bl 0x8214e9c8
  // (left out: dead in C++)
  { const lifted::Result o = f8214E9C8(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214d774
  goto loc_8214D774;
loc_8214D770:
  // bl 0x8214eb28
  // (left out: dead in C++)
  { const lifted::Result o = f8214EB28(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8214D774:
  // mr r5,r30
  r5 = r30;
  // lwz r4,8(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 8);
  // addi r3,r30,384
  r3 = uint64_t(int64_t(int64_t(r30) + 384));
  // bl 0x82149b08
  // (left out: dead in C++)
  r3 = f82149B08(base, r1, r13, r3, r4, r5);
  // lwz r11,8(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 8);
  // addi r11,r11,-384
  r11 = uint64_t(int64_t(int64_t(r11) + -384));
  // stw r11,8(r29)
  guest::StoreU32(base, uint32_t(r29) + 8, uint32_t(r11));
loc_8214D790:
  // bl 0x8214a198
  // (left out: dead in C++)
  f8214A198(base);
  // addi r1,r1,768
  r1 = uint64_t(int64_t(int64_t(r1) + 768));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214D7A0: lifted from generated/ by lift.py.
lifted::Result f8214D7A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_8214D7A0 (lifted)");
  uint64_t r0 = 0, r3 = 0, r4 = 0, r5 = 0, r8 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1552(r1)
  ea = -1552 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r31,r11,25344
  r31 = uint64_t(int64_t(int64_t(r11) + 25344));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r4,r31
  r4 = r31;
  // addi r17,r11,24736
  r17 = uint64_t(int64_t(int64_t(r11) + 24736));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stb r27,114(r1)
  guest::StoreU8(base, uint32_t(r1) + 114, uint8_t(r27));
  // mr r3,r17
  r3 = r17;
  // stw r27,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r27));
  // li r5,260
  r5 = uint64_t(int64_t(260));
  // stw r27,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r27));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214D7E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,260
  r5 = uint64_t(int64_t(260));
  // addi r30,r11,25008
  r30 = uint64_t(int64_t(int64_t(r11) + 25008));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r4,r30
  r4 = r30;
  // addi r29,r11,25752
  r29 = uint64_t(int64_t(int64_t(r11) + 25752));
  // mr r3,r29
  r3 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214D804, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // mr r11,r27
  r11 = r27;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lis r18,-32148
  r18 = uint64_t(int64_t(-2106851328));
  // beq cr6,0x8214d83c
  if (cr6.eq) goto loc_8214D83C;
  // addi r9,r31,4
  r9 = uint64_t(int64_t(int64_t(r31) + 4));
loc_8214D81C:
  // lwz r8,0(r9)
  r8 = guest::LoadU32(base, uint32_t(r9) + 0);
  // lwz r7,27296(r18)
  r7 = guest::LoadU32(base, uint32_t(r18) + 27296);
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // beq cr6,0x8214d83c
  if (cr6.eq) goto loc_8214D83C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214d81c
  if (cr6.lt) goto loc_8214D81C;
loc_8214D83C:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8214d868
  if (!cr6.eq) goto loc_8214D868;
  // cmplwi cr6,r10,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 64, so);
  // bge cr6,0x8214d868
  if (!cr6.lt) goto loc_8214D868;
  // lwz r10,27296(r18)
  r10 = guest::LoadU32(base, uint32_t(r18) + 27296);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r31,4
  r9 = uint64_t(int64_t(int64_t(r31) + 4));
  // stwx r10,r11,r9
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r9), uint32_t(r10));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r10,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r10));
loc_8214D868:
  // lwz r8,0(r30)
  r8 = guest::LoadU32(base, uint32_t(r30) + 0);
  // mr r11,r27
  r11 = r27;
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8214d89c
  if (cr6.eq) goto loc_8214D89C;
  // addi r9,r30,4
  r9 = uint64_t(int64_t(int64_t(r30) + 4));
loc_8214D87C:
  // lwz r7,0(r9)
  r7 = guest::LoadU32(base, uint32_t(r9) + 0);
  // lwz r6,27296(r18)
  r6 = guest::LoadU32(base, uint32_t(r18) + 27296);
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // beq cr6,0x8214d89c
  if (cr6.eq) goto loc_8214D89C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x8214d87c
  if (cr6.lt) goto loc_8214D87C;
loc_8214D89C:
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x8214d8c8
  if (!cr6.eq) goto loc_8214D8C8;
  // cmplwi cr6,r8,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 64, so);
  // bge cr6,0x8214d8c8
  if (!cr6.lt) goto loc_8214D8C8;
  // lwz r9,27296(r18)
  r9 = guest::LoadU32(base, uint32_t(r18) + 27296);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r30,4
  r8 = uint64_t(int64_t(int64_t(r30) + 4));
  // stwx r9,r11,r8
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r8), uint32_t(r9));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r8,r11,1
  r8 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r8,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r8));
loc_8214D8C8:
  // lis r15,-32148
  r15 = uint64_t(int64_t(-2106851328));
  // lis r16,-32158
  r16 = uint64_t(int64_t(-2107506688));
  // lis r14,-32158
  r14 = uint64_t(int64_t(-2107506688));
  // lwz r11,27356(r15)
  r11 = guest::LoadU32(base, uint32_t(r15) + 27356);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r28,r11,30752
  r28 = uint64_t(int64_t(int64_t(r11) + 30752));
  // stw r28,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r28));
  // beq cr6,0x8214d9d0
  if (cr6.eq) goto loc_8214D9D0;
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214dba0
  if (cr6.eq) goto loc_8214DBA0;
  // lwz r3,-29856(r14)
  r3 = guest::LoadU32(base, uint32_t(r14) + -29856);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // bne cr6,0x8214d928
  if (!cr6.eq) goto loc_8214D928;
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,-29860(r16)
  r3 = guest::LoadU32(base, uint32_t(r16) + -29860);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82459ca0
  // (left out: dead in C++)
  { const lifted::Result o = game::accept(base, r1, r13, r3, r4, r5, r7, r8, r9, r10, 0x8214D914); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 0);
  // stw r3,-29856(r14)
  guest::StoreU32(base, uint32_t(r14) + -29856, uint32_t(r3));
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8214daec
  if (cr6.eq) goto loc_8214DAEC;
  // lwz r8,0(r30)
  r8 = guest::LoadU32(base, uint32_t(r30) + 0);
loc_8214D928:
  // mr r11,r27
  r11 = r27;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214d954
  if (cr6.eq) goto loc_8214D954;
  // addi r9,r31,4
  r9 = uint64_t(int64_t(int64_t(r31) + 4));
loc_8214D938:
  // lwz r7,0(r9)
  r7 = guest::LoadU32(base, uint32_t(r9) + 0);
  // cmplw cr6,r7,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r3), so);
  // beq cr6,0x8214d954
  if (cr6.eq) goto loc_8214D954;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214d938
  if (cr6.lt) goto loc_8214D938;
loc_8214D954:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8214d97c
  if (!cr6.eq) goto loc_8214D97C;
  // cmplwi cr6,r10,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 64, so);
  // bge cr6,0x8214d97c
  if (!cr6.lt) goto loc_8214D97C;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r31,4
  r10 = uint64_t(int64_t(int64_t(r31) + 4));
  // stwx r3,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r3));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8214D97C:
  // mr r11,r27
  r11 = r27;
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8214d9a8
  if (cr6.eq) goto loc_8214D9A8;
  // addi r10,r30,4
  r10 = uint64_t(int64_t(int64_t(r30) + 4));
loc_8214D98C:
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplw cr6,r9,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r3), so);
  // beq cr6,0x8214d9a8
  if (cr6.eq) goto loc_8214D9A8;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x8214d98c
  if (cr6.lt) goto loc_8214D98C;
loc_8214D9A8:
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x8214d9d0
  if (!cr6.eq) goto loc_8214D9D0;
  // cmplwi cr6,r8,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 64, so);
  // bge cr6,0x8214d9d0
  if (!cr6.lt) goto loc_8214D9D0;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r30,4
  r10 = uint64_t(int64_t(int64_t(r30) + 4));
  // stwx r3,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r3));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
loc_8214D9D0:
  // lwz r3,-29860(r16)
  r3 = guest::LoadU32(base, uint32_t(r16) + -29860);
loc_8214D9D4:
  // lis r19,-32148
  r19 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r19)
  r11 = guest::LoadU32(base, uint32_t(r19) + 27304);
  // cmpwi cr6,r11,4108
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4108, so);
  // beq cr6,0x8214da00
  if (cr6.eq) goto loc_8214DA00;
  // addi r7,r1,104
  r7 = uint64_t(int64_t(int64_t(r1) + 104));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // mr r4,r17
  r4 = r17;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82459cb8
  // (left out: dead in C++)
  { const lifted::Result o = game::select(base, r1, r13, r3, r4, r5, r6, r7, r9, r10, 0x8214D9FC); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-29860(r16)
  r3 = guest::LoadU32(base, uint32_t(r16) + -29860);
loc_8214DA00:
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r11,27300(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 27300);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8214da28
  if (!cr6.eq) goto loc_8214DA28;
  // mr r4,r29
  r4 = r29;
  // bl 0x82459d20
  // (left out: dead in C++)
  { const lifted::Result o = game::__WSAFDIsSet(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8214DA18); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8214da28
  if (cr0.eq) goto loc_8214DA28;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27300(r31)
  guest::StoreU32(base, uint32_t(r31) + 27300, uint32_t(r11));
loc_8214DA28:
  // mr r4,r17
  r4 = r17;
  // lwz r3,27296(r18)
  r3 = guest::LoadU32(base, uint32_t(r18) + 27296);
  // bl 0x82459d20
  // (left out: dead in C++)
  { const lifted::Result o = game::__WSAFDIsSet(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8214DA34); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // addi r22,r11,26160
  r22 = uint64_t(int64_t(int64_t(r11) + 26160));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r23,-32148
  r23 = uint64_t(int64_t(-2106851328));
  // lis r24,-32148
  r24 = uint64_t(int64_t(-2106851328));
  // lis r25,-32148
  r25 = uint64_t(int64_t(-2106851328));
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
  // lis r21,-32148
  r21 = uint64_t(int64_t(-2106851328));
  // addi r20,r11,27404
  r20 = uint64_t(int64_t(int64_t(r11) + 27404));
  // lis r27,-32148
  r27 = uint64_t(int64_t(-2106851328));
  // beq 0x8214dde8
  if (cr0.eq) goto loc_8214DDE8;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,28664
  r30 = uint64_t(int64_t(int64_t(r11) + 28664));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r29,r11,28184
  r29 = uint64_t(int64_t(int64_t(r11) + 28184));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r28,r11,27416
  r28 = uint64_t(int64_t(int64_t(r11) + 27416));
loc_8214DA7C:
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,27296(r18)
  r3 = guest::LoadU32(base, uint32_t(r18) + 27296);
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1277
  r5 = uint64_t(int64_t(1277));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82459cd8
  // (left out: dead in C++)
  { const lifted::Result o = game::recvfrom(base, r1, r13, r3, r4, r5, r6, r7, r8, r10, 0x8214DA98); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x8214dde4
  if (cr6.eq) goto loc_8214DDE4;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // ble cr6,0x8214dde4
  if (!cr6.gt) goto loc_8214DDE4;
  // lhz r11,82(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 82);
  // cmplwi cr6,r11,1000
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1000, so);
  // bne cr6,0x8214ddd8
  if (!cr6.eq) goto loc_8214DDD8;
  // lbz r11,114(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 114);
  // cmplwi cr6,r11,26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 26, so);
  // bgt cr6,0x8214ddd8
  if (cr6.gt) goto loc_8214DDD8;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-28808
  r12 = uint64_t(int64_t(int64_t(r12) + -28808));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32235
  r12 = uint64_t(int64_t(-2112552960));
  // addi r12,r12,-9492
  r12 = uint64_t(int64_t(int64_t(r12) + -9492));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_8214DC3C;
  case 1:
  goto loc_8214DC4C;
  case 2:
  goto loc_8214DC5C;
  case 3:
  goto loc_8214DDD8;
  case 4:
  goto loc_8214DC7C;
  case 5:
  goto loc_8214DC88;
  case 6:
  goto loc_8214DCA4;
  case 7:
  goto loc_8214DCB4;
  case 8:
  goto loc_8214DC6C;
  case 9:
  goto loc_8214DCC4;
  case 10:
  goto loc_8214DD18;
  case 11:
  goto loc_8214DD54;
  case 12:
  goto loc_8214DD64;
  case 13:
  goto loc_8214DCE4;
  case 14:
  goto loc_8214DCF8;
  case 15:
  goto loc_8214DD08;
  case 16:
  goto loc_8214DCD0;
  case 17:
  goto loc_8214DCDC;
  case 18:
  goto loc_8214DDD8;
  case 19:
  goto loc_8214DDD8;
  case 20:
  goto loc_8214DDD8;
  case 21:
  goto loc_8214DDD0;
  case 22:
  goto loc_8214DDD8;
  case 23:
  goto loc_8214DD74;
  case 24:
  goto loc_8214DD84;
  case 25:
  goto loc_8214DD90;
  case 26:
  goto loc_8214DDAC;
  default:
  std::abort();
  }
loc_8214DAEC:
  // lwz r3,-29860(r16)
  r3 = guest::LoadU32(base, uint32_t(r16) + -29860);
  // mr r11,r27
  r11 = r27;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214db1c
  if (cr6.eq) goto loc_8214DB1C;
  // addi r9,r31,4
  r9 = uint64_t(int64_t(int64_t(r31) + 4));
loc_8214DB00:
  // lwz r8,0(r9)
  r8 = guest::LoadU32(base, schema::SysRecvFromAnswer(base, uint32_t(r9) - 4).unknown_4_address());
  // cmplw cr6,r8,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r3), so);
  // beq cr6,0x8214db1c
  if (cr6.eq) goto loc_8214DB1C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214db00
  if (cr6.lt) goto loc_8214DB00;
loc_8214DB1C:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8214db44
  if (!cr6.eq) goto loc_8214DB44;
  // cmplwi cr6,r10,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 64, so);
  // bge cr6,0x8214db44
  if (!cr6.lt) goto loc_8214DB44;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r31,4
  r10 = uint64_t(int64_t(int64_t(r31) + 4));
  // stwx r3,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r3));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::SysRecvFromAnswer(base, uint32_t(r31)).unknown_0_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::SysRecvFromAnswer(base, uint32_t(r31)).unknown_0_address(), uint32_t(r11));
loc_8214DB44:
  // lwz r9,0(r30)
  r9 = guest::LoadU32(base, uint32_t(r30) + 0);
  // mr r11,r27
  r11 = r27;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8214db74
  if (cr6.eq) goto loc_8214DB74;
  // addi r10,r30,4
  r10 = uint64_t(int64_t(int64_t(r30) + 4));
loc_8214DB58:
  // lwz r8,0(r10)
  r8 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplw cr6,r8,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r3), so);
  // beq cr6,0x8214db74
  if (cr6.eq) goto loc_8214DB74;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x8214db58
  if (cr6.lt) goto loc_8214DB58;
loc_8214DB74:
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x8214d9d4
  if (!cr6.eq) goto loc_8214D9D4;
  // cmplwi cr6,r9,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 64, so);
loc_8214DB80:
  // bge cr6,0x8214d9d4
  if (!cr6.lt) goto loc_8214D9D4;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r30,4
  r10 = uint64_t(int64_t(int64_t(r30) + 4));
  // stwx r3,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r3));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r11));
  // b 0x8214d9d4
  goto loc_8214D9D4;
loc_8214DBA0:
  // lwz r3,-29860(r16)
  r3 = guest::LoadU32(base, uint32_t(r16) + -29860);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8214d9d4
  if (cr6.eq) goto loc_8214D9D4;
  // mr r11,r27
  r11 = r27;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214dbd8
  if (cr6.eq) goto loc_8214DBD8;
  // addi r9,r31,4
  r9 = uint64_t(int64_t(int64_t(r31) + 4));
loc_8214DBBC:
  // lwz r7,0(r9)
  r7 = guest::LoadU32(base, schema::SysRecvFromAnswer(base, uint32_t(r9) - 4).unknown_4_address());
  // cmplw cr6,r7,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r3), so);
  // beq cr6,0x8214dbd8
  if (cr6.eq) goto loc_8214DBD8;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8214dbbc
  if (cr6.lt) goto loc_8214DBBC;
loc_8214DBD8:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8214dc00
  if (!cr6.eq) goto loc_8214DC00;
  // cmplwi cr6,r10,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 64, so);
  // bge cr6,0x8214dc00
  if (!cr6.lt) goto loc_8214DC00;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r31,4
  r10 = uint64_t(int64_t(int64_t(r31) + 4));
  // stwx r3,r11,r10
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), uint32_t(r3));
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::SysRecvFromAnswer(base, uint32_t(r31)).unknown_0_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::SysRecvFromAnswer(base, uint32_t(r31)).unknown_0_address(), uint32_t(r11));
loc_8214DC00:
  // mr r11,r27
  r11 = r27;
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8214dc2c
  if (cr6.eq) goto loc_8214DC2C;
  // addi r10,r30,4
  r10 = uint64_t(int64_t(int64_t(r30) + 4));
loc_8214DC10:
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplw cr6,r9,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r3), so);
  // beq cr6,0x8214dc2c
  if (cr6.eq) goto loc_8214DC2C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x8214dc10
  if (cr6.lt) goto loc_8214DC10;
loc_8214DC2C:
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x8214d9d4
  if (!cr6.eq) goto loc_8214D9D4;
  // cmplwi cr6,r8,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 64, so);
  // b 0x8214db80
  goto loc_8214DB80;
loc_8214DC3C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214cfb8
  // (left out: dead in C++)
  { const lifted::Result o = f8214CFB8(base, r1, r13, r3, r4, r7); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DC4C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214b870
  // (left out: dead in C++)
  { const lifted::Result o = f8214B870(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DC5C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214d560
  // (left out: dead in C++)
  { const lifted::Result o = f8214D560(base, r1, r13, r3, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DC6C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214a280
  // (left out: dead in C++)
  r3 = f8214A280(base, r1, r13, r3, r4);
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DC7C:
  // li r3,4102
  r3 = uint64_t(int64_t(4102));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DC88:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,100(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 100);
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // bl 0x8214ed30
  // (left out: dead in C++)
  { const lifted::Result o = f8214ED30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,4100
  r11 = uint64_t(int64_t(4100));
  // stw r11,27304(r19)
  guest::StoreU32(base, uint32_t(r19) + 27304, uint32_t(r11));
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DCA4:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214a618
  // (left out: dead in C++)
  { const lifted::Result o = f8214A618(base, r1, r13, r4, r6, r7, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DCB4:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214baf8
  // (left out: dead in C++)
  { const lifted::Result o = f8214BAF8(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DCC4:
  // lwz r3,84(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x82149dc0
  // (left out: dead in C++)
  { const lifted::Result o = f82149DC0(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DCD0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_8214DCD4:
  // stw r11,27368(r27)
  guest::StoreU32(base, uint32_t(r27) + 27368, uint32_t(r11));
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DCDC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x8214dcd4
  goto loc_8214DCD4;
loc_8214DCE4:
  // addi r4,r1,131
  r4 = uint64_t(int64_t(int64_t(r1) + 131));
  // mr r3,r28
  r3 = r28;
  // li r5,764
  r5 = uint64_t(int64_t(764));
loc_8214DCF0:
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214DCF4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DCF8:
  // addi r4,r1,131
  r4 = uint64_t(int64_t(int64_t(r1) + 131));
  // mr r3,r29
  r3 = r29;
  // li r5,480
  r5 = uint64_t(int64_t(480));
  // b 0x8214dcf0
  goto loc_8214DCF0;
loc_8214DD08:
  // addi r4,r1,131
  r4 = uint64_t(int64_t(int64_t(r1) + 131));
  // mr r3,r30
  r3 = r30;
  // li r5,640
  r5 = uint64_t(int64_t(640));
  // b 0x8214dcf0
  goto loc_8214DCF0;
loc_8214DD18:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82148768
  // (left out: dead in C++)
  r3 = f82148768(base, r3);
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // subf r11,r11,r31
  r11 = r31 - r11;
  // addis r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 65536));
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // rlwinm r10,r11,24,24,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0xFF;
  // stb r11,1388(r1)
  guest::StoreU8(base, uint32_t(r1) + 1388, uint8_t(r11));
  // stb r10,1387(r1)
  guest::StoreU8(base, uint32_t(r1) + 1387, uint8_t(r10));
  // bl 0x8214a3b0
  // (left out: dead in C++)
  { const lifted::Result o = f8214A3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DD54:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214ce00
  // (left out: dead in C++)
  { const lifted::Result o = f8214CE00(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DD64:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214a488
  // (left out: dead in C++)
  r3 = f8214A488(base, r1, r13, r3);
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DD74:
  // addi r4,r1,115
  r4 = uint64_t(int64_t(int64_t(r1) + 115));
  // mr r3,r20
  r3 = r20;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // b 0x8214dcf0
  goto loc_8214DCF0;
loc_8214DD84:
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // stw r11,27412(r21)
  guest::StoreU32(base, uint32_t(r21) + 27412, uint32_t(r11));
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DD90:
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // stw r11,29308(r26)
  guest::StoreU32(base, uint32_t(r26) + 29308, uint32_t(r11));
  // lbz r11,116(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 116);
  // stw r11,29312(r25)
  guest::StoreU32(base, uint32_t(r25) + 29312, uint32_t(r11));
  // lbz r11,117(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 117);
  // stw r11,29316(r24)
  guest::StoreU32(base, uint32_t(r24) + 29316, uint32_t(r11));
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DDAC:
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // lhz r10,116(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + 116);
  // rotlwi r11,r11,16
  r11 = __builtin_rotateleft32(uint32_t(r11), 16);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lbz r10,118(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 118);
  // rlwinm r11,r11,8,0,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFFFFFF00;
  // or r11,r10,r11
  r11 = r10 | r11;
  // stw r11,4(r22)
  guest::StoreU32(base, uint32_t(r22) + 4, uint32_t(r11));
  // b 0x8214ddd8
  goto loc_8214DDD8;
loc_8214DDD0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27396(r23)
  guest::StoreU32(base, uint32_t(r23) + 27396, uint32_t(r11));
loc_8214DDD8:
  // lhz r11,82(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 82);
  // cmplwi cr6,r11,1000
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1000, so);
  // beq cr6,0x8214da7c
  if (cr6.eq) goto loc_8214DA7C;
loc_8214DDE4:
  // lwz r28,100(r1)
  r28 = guest::LoadU32(base, uint32_t(r1) + 100);
loc_8214DDE8:
  // lwz r11,27356(r15)
  r11 = guest::LoadU32(base, uint32_t(r15) + 27356);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214e184
  if (cr6.eq) goto loc_8214E184;
  // lwz r11,4(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214dfc8
  if (cr6.eq) goto loc_8214DFC8;
  // lwz r3,-29856(r14)
  r3 = guest::LoadU32(base, uint32_t(r14) + -29856);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8214e184
  if (cr6.eq) goto loc_8214E184;
  // mr r4,r17
  r4 = r17;
  // bl 0x82459d20
  // (left out: dead in C++)
  { const lifted::Result o = game::__WSAFDIsSet(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8214DE14); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8214e184
  if (cr0.eq) goto loc_8214E184;
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
loc_8214DE20:
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,-29856(r14)
  r3 = guest::LoadU32(base, uint32_t(r14) + -29856);
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1277
  r5 = uint64_t(int64_t(1277));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82459cd8
  // (left out: dead in C++)
  { const lifted::Result o = game::recvfrom(base, r1, r13, r3, r4, r5, r6, r7, r8, r10, 0x8214DE3C); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8214e184
  if (cr6.eq) goto loc_8214E184;
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble cr6,0x8214e184
  if (!cr6.gt) goto loc_8214E184;
  // lhz r11,82(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 82);
  // cmplwi cr6,r11,1006
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1006, so);
  // bne cr6,0x8214dfb8
  if (!cr6.eq) goto loc_8214DFB8;
  // lbz r11,114(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 114);
  // cmplwi cr6,r11,26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 26, so);
  // bgt cr6,0x8214dfb8
  if (cr6.gt) goto loc_8214DFB8;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-28840
  r12 = uint64_t(int64_t(int64_t(r12) + -28840));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32235
  r12 = uint64_t(int64_t(-2112552960));
  // addi r12,r12,-8564
  r12 = uint64_t(int64_t(int64_t(r12) + -8564));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_8214DE8C;
  case 1:
  goto loc_8214DE9C;
  case 2:
  goto loc_8214DEAC;
  case 3:
  goto loc_8214DFB8;
  case 4:
  goto loc_8214DECC;
  case 5:
  goto loc_8214DED8;
  case 6:
  goto loc_8214DEF4;
  case 7:
  goto loc_8214DF04;
  case 8:
  goto loc_8214DEBC;
  case 9:
  goto loc_8214DF14;
  case 10:
  goto loc_8214DFB8;
  case 11:
  goto loc_8214DFB8;
  case 12:
  goto loc_8214DFB8;
  case 13:
  goto loc_8214DFB8;
  case 14:
  goto loc_8214DFB8;
  case 15:
  goto loc_8214DFB8;
  case 16:
  goto loc_8214DF20;
  case 17:
  goto loc_8214DF2C;
  case 18:
  goto loc_8214DF34;
  case 19:
  goto loc_8214DF34;
  case 20:
  goto loc_8214DF34;
  case 21:
  goto loc_8214DF44;
  case 22:
  goto loc_8214DF50;
  case 23:
  goto loc_8214DF5C;
  case 24:
  goto loc_8214DF70;
  case 25:
  goto loc_8214DF7C;
  case 26:
  goto loc_8214DF98;
  default:
  std::abort();
  }
loc_8214DE8C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214cfb8
  // (left out: dead in C++)
  { const lifted::Result o = f8214CFB8(base, r1, r13, r3, r4, r7); r3 = o.r3; f1 = o.f1; }
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DE9C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214b870
  // (left out: dead in C++)
  { const lifted::Result o = f8214B870(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DEAC:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214d560
  // (left out: dead in C++)
  { const lifted::Result o = f8214D560(base, r1, r13, r3, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DEBC:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214a280
  // (left out: dead in C++)
  r3 = f8214A280(base, r1, r13, r3, r4);
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DECC:
  // li r3,4102
  r3 = uint64_t(int64_t(4102));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DED8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r28
  r3 = r28;
  // bl 0x8214ed30
  // (left out: dead in C++)
  { const lifted::Result o = f8214ED30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,4100
  r11 = uint64_t(int64_t(4100));
  // stw r11,27304(r19)
  guest::StoreU32(base, uint32_t(r19) + 27304, uint32_t(r11));
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DEF4:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214a618
  // (left out: dead in C++)
  { const lifted::Result o = f8214A618(base, r1, r13, r4, r6, r7, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF04:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214baf8
  // (left out: dead in C++)
  { const lifted::Result o = f8214BAF8(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF14:
  // lwz r3,84(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x82149dc0
  // (left out: dead in C++)
  { const lifted::Result o = f82149DC0(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF20:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_8214DF24:
  // stw r11,27368(r27)
  guest::StoreU32(base, uint32_t(r27) + 27368, uint32_t(r11));
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF2C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x8214df24
  goto loc_8214DF24;
loc_8214DF34:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x82148f88
  // (left out: dead in C++)
  f82148F88(base, r3);
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF44:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27396(r23)
  guest::StoreU32(base, uint32_t(r23) + 27396, uint32_t(r11));
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF50:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27400(r31)
  guest::StoreU32(base, uint32_t(r31) + 27400, uint32_t(r11));
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF5C:
  // addi r4,r1,115
  r4 = uint64_t(int64_t(int64_t(r1) + 115));
  // mr r3,r20
  r3 = r20;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214DF6C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF70:
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // stw r11,27412(r21)
  guest::StoreU32(base, uint32_t(r21) + 27412, uint32_t(r11));
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF7C:
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // stw r11,29308(r26)
  guest::StoreU32(base, uint32_t(r26) + 29308, uint32_t(r11));
  // lbz r11,116(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 116);
  // stw r11,29312(r25)
  guest::StoreU32(base, uint32_t(r25) + 29312, uint32_t(r11));
  // lbz r11,117(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 117);
  // stw r11,29316(r24)
  guest::StoreU32(base, uint32_t(r24) + 29316, uint32_t(r11));
  // b 0x8214dfb8
  goto loc_8214DFB8;
loc_8214DF98:
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // lhz r10,116(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + 116);
  // rotlwi r11,r11,16
  r11 = __builtin_rotateleft32(uint32_t(r11), 16);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lbz r10,118(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 118);
  // rlwinm r11,r11,8,0,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFFFFFF00;
  // or r11,r10,r11
  r11 = r10 | r11;
  // stw r11,4(r22)
  guest::StoreU32(base, uint32_t(r22) + 4, uint32_t(r11));
loc_8214DFB8:
  // lhz r11,82(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 82);
  // cmplwi cr6,r11,1006
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1006, so);
  // beq cr6,0x8214de20
  if (cr6.eq) goto loc_8214DE20;
  // b 0x8214e184
  goto loc_8214E184;
loc_8214DFC8:
  // mr r4,r17
  r4 = r17;
  // lwz r3,-29860(r16)
  r3 = guest::LoadU32(base, uint32_t(r16) + -29860);
  // bl 0x82459d20
  // (left out: dead in C++)
  { const lifted::Result o = game::__WSAFDIsSet(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8214DFD4); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8214e184
  if (cr0.eq) goto loc_8214E184;
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
loc_8214DFE0:
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,-29860(r16)
  r3 = guest::LoadU32(base, uint32_t(r16) + -29860);
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1277
  r5 = uint64_t(int64_t(1277));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82459cd8
  // (left out: dead in C++)
  { const lifted::Result o = game::recvfrom(base, r1, r13, r3, r4, r5, r6, r7, r8, r10, 0x8214DFFC); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8214e184
  if (cr6.eq) goto loc_8214E184;
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble cr6,0x8214e184
  if (!cr6.gt) goto loc_8214E184;
  // lhz r11,82(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 82);
  // cmplwi cr6,r11,1006
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1006, so);
  // bne cr6,0x8214e178
  if (!cr6.eq) goto loc_8214E178;
  // lbz r11,114(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 114);
  // cmplwi cr6,r11,26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 26, so);
  // bgt cr6,0x8214e178
  if (cr6.gt) goto loc_8214E178;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-28872
  r12 = uint64_t(int64_t(int64_t(r12) + -28872));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32235
  r12 = uint64_t(int64_t(-2112552960));
  // addi r12,r12,-8116
  r12 = uint64_t(int64_t(int64_t(r12) + -8116));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_8214E04C;
  case 1:
  goto loc_8214E05C;
  case 2:
  goto loc_8214E06C;
  case 3:
  goto loc_8214E178;
  case 4:
  goto loc_8214E08C;
  case 5:
  goto loc_8214E098;
  case 6:
  goto loc_8214E0B4;
  case 7:
  goto loc_8214E0C4;
  case 8:
  goto loc_8214E07C;
  case 9:
  goto loc_8214E0D4;
  case 10:
  goto loc_8214E178;
  case 11:
  goto loc_8214E178;
  case 12:
  goto loc_8214E178;
  case 13:
  goto loc_8214E178;
  case 14:
  goto loc_8214E178;
  case 15:
  goto loc_8214E178;
  case 16:
  goto loc_8214E0E0;
  case 17:
  goto loc_8214E0EC;
  case 18:
  goto loc_8214E0F4;
  case 19:
  goto loc_8214E0F4;
  case 20:
  goto loc_8214E0F4;
  case 21:
  goto loc_8214E104;
  case 22:
  goto loc_8214E110;
  case 23:
  goto loc_8214E11C;
  case 24:
  goto loc_8214E130;
  case 25:
  goto loc_8214E13C;
  case 26:
  goto loc_8214E158;
  default:
  std::abort();
  }
loc_8214E04C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214cfb8
  // (left out: dead in C++)
  { const lifted::Result o = f8214CFB8(base, r1, r13, r3, r4, r7); r3 = o.r3; f1 = o.f1; }
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E05C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214b870
  // (left out: dead in C++)
  { const lifted::Result o = f8214B870(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E06C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214d560
  // (left out: dead in C++)
  { const lifted::Result o = f8214D560(base, r1, r13, r3, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E07C:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214a280
  // (left out: dead in C++)
  r3 = f8214A280(base, r1, r13, r3, r4);
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E08C:
  // li r3,4102
  r3 = uint64_t(int64_t(4102));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E098:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r28
  r3 = r28;
  // bl 0x8214ed30
  // (left out: dead in C++)
  { const lifted::Result o = f8214ED30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,4100
  r11 = uint64_t(int64_t(4100));
  // stw r11,27304(r19)
  guest::StoreU32(base, uint32_t(r19) + 27304, uint32_t(r11));
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E0B4:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214a618
  // (left out: dead in C++)
  { const lifted::Result o = f8214A618(base, r1, r13, r4, r6, r7, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E0C4:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x8214baf8
  // (left out: dead in C++)
  { const lifted::Result o = f8214BAF8(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E0D4:
  // lwz r3,84(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x82149dc0
  // (left out: dead in C++)
  { const lifted::Result o = f82149DC0(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E0E0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_8214E0E4:
  // stw r11,27368(r27)
  guest::StoreU32(base, uint32_t(r27) + 27368, uint32_t(r11));
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E0EC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x8214e0e4
  goto loc_8214E0E4;
loc_8214E0F4:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x82148f88
  // (left out: dead in C++)
  f82148F88(base, r3);
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E104:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27396(r23)
  guest::StoreU32(base, uint32_t(r23) + 27396, uint32_t(r11));
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E110:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27400(r31)
  guest::StoreU32(base, uint32_t(r31) + 27400, uint32_t(r11));
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E11C:
  // addi r4,r1,115
  r4 = uint64_t(int64_t(int64_t(r1) + 115));
  // mr r3,r20
  r3 = r20;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8214E12C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E130:
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // stw r11,27412(r21)
  guest::StoreU32(base, uint32_t(r21) + 27412, uint32_t(r11));
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E13C:
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // stw r11,29308(r26)
  guest::StoreU32(base, uint32_t(r26) + 29308, uint32_t(r11));
  // lbz r11,116(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 116);
  // stw r11,29312(r25)
  guest::StoreU32(base, uint32_t(r25) + 29312, uint32_t(r11));
  // lbz r11,117(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 117);
  // stw r11,29316(r24)
  guest::StoreU32(base, uint32_t(r24) + 29316, uint32_t(r11));
  // b 0x8214e178
  goto loc_8214E178;
loc_8214E158:
  // lbz r11,115(r1)
  r11 = guest::LoadU8(base, uint32_t(r1) + 115);
  // lhz r10,116(r1)
  r10 = guest::LoadU16(base, uint32_t(r1) + 116);
  // rotlwi r11,r11,16
  r11 = __builtin_rotateleft32(uint32_t(r11), 16);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lbz r10,118(r1)
  r10 = guest::LoadU8(base, uint32_t(r1) + 118);
  // rlwinm r11,r11,8,0,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFFFFFF00;
  // or r11,r10,r11
  r11 = r10 | r11;
  // stw r11,4(r22)
  guest::StoreU32(base, uint32_t(r22) + 4, uint32_t(r11));
loc_8214E178:
  // lhz r11,82(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 82);
  // cmplwi cr6,r11,1006
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1006, so);
  // beq cr6,0x8214dfe0
  if (cr6.eq) goto loc_8214DFE0;
loc_8214E184:
  // addi r1,r1,1552
  r1 = uint64_t(int64_t(int64_t(r1) + 1552));
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214E778: lifted from generated/ by lift.py.
lifted::Result f8214E778(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8214E778 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r11,r4
  r11 = r4;
  // lwz r10,140(r31)
  r10 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_140_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // ble cr6,0x8214e7fc
  if (!cr6.gt) goto loc_8214E7FC;
  // cmpwi cr6,r10,11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 11, so);
  // bge cr6,0x8214e7fc
  if (!cr6.lt) goto loc_8214E7FC;
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_80_address());
  // addi r30,r31,92
  r30 = uint64_t(int64_t(int64_t(r31) + 92));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // rlwimi r10,r11,0,27,27
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10) | (r10 & 0xFFFFFFFFFFFFFFEF);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // rlwimi r10,r11,0,21,23
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x700) | (r10 & 0xFFFFFFFFFFFFF8FF);
  // mr r3,r30
  r3 = r30;
  // stw r10,80(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_80_address(), uint32_t(r10));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214E7CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r7,r30
  r7 = r30;
  // lwz r6,124(r31)
  r6 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_124_address());
  // lwz r5,120(r31)
  r5 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_120_address());
  // lwz r4,80(r31)
  r4 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_80_address());
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_0_address());
  // bl 0x82575fe0
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionModify(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x8214e7f4
  if (cr6.eq) goto loc_8214E7F4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8214E7F4);
loc_8214E7F4:
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // stw r11,140(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_140_address(), uint32_t(r11));
loc_8214E7FC:
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

// sub_8214E818: lifted from generated/ by lift.py.
uint64_t f8214E818(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8214E818 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // addi r11,r3,8
  r11 = uint64_t(int64_t(int64_t(r3) + 8));
  // addi r9,r4,8
  r9 = uint64_t(int64_t(int64_t(r4) + 8));
loc_8214E820:
  // lbz r10,0(r4)
  r10 = guest::LoadU8(base, schema::NetId(base, uint32_t(r4)).id_address());
  // lbz r8,0(r11)
  r8 = guest::LoadU8(base, schema::NetIdRecord(base, uint32_t(r11) - 8).id_address());
  // subf. r10,r8,r10
  r10 = r10 - r8;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x8214e840
  if (!cr0.eq) goto loc_8214E840;
  // addi r4,r4,1
  r4 = uint64_t(int64_t(int64_t(r4) + 1));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpw cr6,r4,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r4), int32_t(r9), so);
  // bne cr6,0x8214e820
  if (!cr6.eq) goto loc_8214E820;
loc_8214E840:
  // cntlzw r11,r10
  r11 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // blr
  return r3;
}

// sub_8214E850: lifted from generated/ by lift.py.
lifted::Result f8214E850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8214E850 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r11,-21596(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -21596);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_0_address());
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,292(r30)
  r11 = guest::LoadU32(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_292_address());
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x82576130
  // (left out: dead in C++)
  { const lifted::Result o = f82576130(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8214e8ac
  if (cr0.eq) goto loc_8214E8AC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8214E8A8);
  // b 0x8214e8ec
  goto loc_8214E8EC;
loc_8214E8AC:
  // lbz r8,12(r30)
  r8 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_12_address());
  // lwz r11,292(r30)
  r11 = guest::LoadU32(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_292_address());
  // mr r9,r8
  r9 = r8;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214e8e0
  if (cr6.eq) goto loc_8214E8E0;
  // lwz r11,132(r31)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_132_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r10,124(r31)
  r10 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_124_address());
  // add r11,r11,r8
  r11 = r11 + r8;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // stw r11,132(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_132_address(), uint32_t(r11));
  // ble cr6,0x8214e8e0
  if (!cr6.gt) goto loc_8214E8E0;
  // stw r10,132(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_132_address(), uint32_t(r10));
loc_8214E8E0:
  // lwz r11,128(r31)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_128_address());
  // add r11,r9,r11
  r11 = r9 + r11;
  // stw r11,128(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_128_address(), uint32_t(r11));
loc_8214E8EC:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
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

// sub_8214E908: lifted from generated/ by lift.py.
lifted::Result f8214E908(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8214E908 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // lbz r29,12(r30)
  r29 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_12_address());
  // cmplwi r29,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq 0x8214e950
  if (cr0.eq) goto loc_8214E950;
  // lwz r10,292(r30)
  r10 = guest::LoadU32(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_292_address());
  // addi r4,r30,144
  r4 = uint64_t(int64_t(int64_t(r30) + 144));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // mtctr r29
  ctr = r29;
loc_8214E938:
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x8214e938
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_8214E938;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // rlwinm r5,r29,3,0,28
  r5 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 3) & 0xFFFFFFF8;
  // bl 0x825a7830
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7830(base, LIFTED_ARGS(0x8214E950, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214E950:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_0_address());
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r4,r29
  r4 = r29;
  // bl 0x825761d8
  // (left out: dead in C++)
  { const lifted::Result o = f825761D8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8214e97c
  if (cr0.eq) goto loc_8214E97C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8214E978);
  // b 0x8214e9bc
  goto loc_8214E9BC;
loc_8214E97C:
  // lbz r8,12(r30)
  r8 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_12_address());
  // lwz r11,292(r30)
  r11 = guest::LoadU32(base, schema::SysLinkRecord(base, uint32_t(r30)).unknown_292_address());
  // mr r9,r8
  r9 = r8;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214e9b0
  if (cr6.eq) goto loc_8214E9B0;
  // lwz r11,132(r31)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_132_address());
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r10,124(r31)
  r10 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_124_address());
  // add r11,r11,r8
  r11 = r11 + r8;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // stw r11,132(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_132_address(), uint32_t(r11));
  // ble cr6,0x8214e9b0
  if (!cr6.gt) goto loc_8214E9B0;
  // stw r10,132(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_132_address(), uint32_t(r10));
loc_8214E9B0:
  // lwz r11,128(r31)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_128_address());
  // add r11,r11,r9
  r11 = r11 + r9;
  // stw r11,128(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_128_address(), uint32_t(r11));
loc_8214E9BC:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214E9C8: lifted from generated/ by lift.py.
lifted::Result f8214E9C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214E9C8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // mr r31,r4
  r31 = r4;
  // mr r27,r3
  r27 = r3;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // lbz r11,12(r31)
  r11 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r31)).unknown_12_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214eabc
  if (cr0.eq) goto loc_8214EABC;
  // addi r28,r1,96
  r28 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r30,r31,144
  r30 = uint64_t(int64_t(int64_t(r31) + 144));
  // addi r29,r31,300
  r29 = uint64_t(int64_t(int64_t(r31) + 300));
  // li r25,1
  r25 = uint64_t(int64_t(1));
loc_8214E9FC:
  // lwz r11,80(r27)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r27)).unknown_80_address());
  // lwz r10,0(r29)
  r10 = guest::LoadU32(base, schema::SysLinkRecord(base, uint32_t(r29) - 300).unknown_300_address());
  // stw r10,0(r28)
  guest::StoreU32(base, uint32_t(r28) + 0, uint32_t(r10));
  // rlwinm. r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214eaa0
  if (cr0.eq) goto loc_8214EAA0;
  // lwz r11,140(r27)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r27)).unknown_140_address());
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x8214ea24
  if (cr6.eq) goto loc_8214EA24;
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bne cr6,0x8214eaa0
  if (!cr6.eq) goto loc_8214EAA0;
loc_8214EA24:
  // clrldi r4,r10,32
  r4 = r10 & 0xFFFFFFFF;
  // ld r11,0(r30)
  r11 = guest::LoadU64(base, schema::SysLinkRecord(base, uint32_t(r30) - 144).unknown_144_address());
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // lwz r3,0(r27)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r27)).unknown_0_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stb r25,120(r1)
  guest::StoreU8(base, uint32_t(r1) + 120, uint8_t(r25));
  // ori r10,r10,32778
  r10 = r10 | 32778;
  // stb r25,144(r1)
  guest::StoreU8(base, uint32_t(r1) + 144, uint8_t(r25));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r10,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r10));
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // ori r10,r10,32779
  r10 = r10 | 32779;
  // stw r10,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r10));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // lis r10,-1
  r10 = uint64_t(int64_t(-65536));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r10,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r10));
  // rldicl r10,r11,32,32
  r10 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFF;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // xor r11,r10,r11
  r11 = r10 ^ r11;
  // stw r11,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r11));
  // bl 0x825766e0
  // (left out: dead in C++)
  { const lifted::Result o = f825766E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8214eaa0
  if (cr0.eq) goto loc_8214EAA0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8214EAA0);
loc_8214EAA0:
  // lbz r11,12(r31)
  r11 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r31)).unknown_12_address());
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // cmplw cr6,r26,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r11), so);
  // blt cr6,0x8214e9fc
  if (cr6.lt) goto loc_8214E9FC;
loc_8214EABC:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lbz r4,12(r31)
  r4 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r31)).unknown_12_address());
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,0(r27)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r27)).unknown_0_address());
  // bl 0x82576280
  // (left out: dead in C++)
  { const lifted::Result o = f82576280(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8214eae4
  if (cr0.eq) goto loc_8214EAE4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8214EAE0);
  // b 0x8214eb20
  goto loc_8214EB20;
loc_8214EAE4:
  // lwz r11,292(r31)
  r11 = guest::LoadU32(base, schema::SysLinkRecord(base, uint32_t(r31)).unknown_292_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lbz r11,12(r31)
  r11 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r31)).unknown_12_address());
  // beq cr6,0x8214eb0c
  if (cr6.eq) goto loc_8214EB0C;
  // lwz r10,132(r27)
  r10 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r27)).unknown_132_address());
  // subf. r11,r11,r10
  r11 = r10 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8214eb04
  if (!cr0.eq) goto loc_8214EB04;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8214EB04:
  // stw r11,132(r27)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r27)).unknown_132_address(), uint32_t(r11));
  // b 0x8214eb20
  goto loc_8214EB20;
loc_8214EB0C:
  // lwz r10,128(r27)
  r10 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r27)).unknown_128_address());
  // subf. r11,r11,r10
  r11 = r10 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8214eb1c
  if (!cr0.eq) goto loc_8214EB1C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8214EB1C:
  // stw r11,128(r27)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r27)).unknown_128_address(), uint32_t(r11));
loc_8214EB20:
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214EB28: lifted from generated/ by lift.py.
lifted::Result f8214EB28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214EB28 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // mr r30,r3
  r30 = r3;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // lbz r11,12(r29)
  r11 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r29)).unknown_12_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214ec10
  if (cr0.eq) goto loc_8214EC10;
  // addi r28,r1,96
  r28 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r31,r29,144
  r31 = uint64_t(int64_t(int64_t(r29) + 144));
  // li r27,1
  r27 = uint64_t(int64_t(1));
loc_8214EB58:
  // lwz r11,80(r30)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r30)).unknown_80_address());
  // ld r4,0(r31)
  r4 = guest::LoadU64(base, schema::SysLinkRecord(base, uint32_t(r31) - 144).unknown_144_address());
  // std r4,0(r28)
  guest::StoreU64(base, uint32_t(r28) + 0, r4);
  // rlwinm. r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214ebf8
  if (cr0.eq) goto loc_8214EBF8;
  // lwz r11,140(r30)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r30)).unknown_140_address());
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x8214eb80
  if (cr6.eq) goto loc_8214EB80;
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bne cr6,0x8214ebf8
  if (!cr6.eq) goto loc_8214EBF8;
loc_8214EB80:
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // ld r11,0(r31)
  r11 = guest::LoadU64(base, schema::SysLinkRecord(base, uint32_t(r31) - 144).unknown_144_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r30)).unknown_0_address());
  // ori r10,r10,32778
  r10 = r10 | 32778;
  // stb r27,136(r1)
  guest::StoreU8(base, uint32_t(r1) + 136, uint8_t(r27));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // stb r27,160(r1)
  guest::StoreU8(base, uint32_t(r1) + 160, uint8_t(r27));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r10,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r10));
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // ori r10,r10,32779
  r10 = r10 | 32779;
  // stw r10,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r10));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // lis r10,-1
  r10 = uint64_t(int64_t(-65536));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r10,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r10));
  // rldicl r10,r11,32,32
  r10 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFF;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // xor r11,r10,r11
  r11 = r10 ^ r11;
  // stw r11,168(r1)
  guest::StoreU32(base, uint32_t(r1) + 168, uint32_t(r11));
  // bl 0x825766e0
  // (left out: dead in C++)
  { const lifted::Result o = f825766E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8214ebf8
  if (cr0.eq) goto loc_8214EBF8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8214EBF8);
loc_8214EBF8:
  // lbz r11,12(r29)
  r11 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r29)).unknown_12_address());
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // addi r31,r31,8
  r31 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r28,r28,8
  r28 = uint64_t(int64_t(int64_t(r28) + 8));
  // cmplw cr6,r26,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r11), so);
  // blt cr6,0x8214eb58
  if (cr6.lt) goto loc_8214EB58;
loc_8214EC10:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lbz r4,12(r29)
  r4 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r29)).unknown_12_address());
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r30)).unknown_0_address());
  // bl 0x82576328
  // (left out: dead in C++)
  { const lifted::Result o = f82576328(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8214ec38
  if (cr0.eq) goto loc_8214EC38;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8214EC34);
  // b 0x8214ec74
  goto loc_8214EC74;
loc_8214EC38:
  // lwz r11,292(r29)
  r11 = guest::LoadU32(base, schema::SysLinkRecord(base, uint32_t(r29)).unknown_292_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lbz r11,12(r29)
  r11 = guest::LoadU8(base, schema::SysLinkRecord(base, uint32_t(r29)).unknown_12_address());
  // beq cr6,0x8214ec60
  if (cr6.eq) goto loc_8214EC60;
  // lwz r10,132(r30)
  r10 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r30)).unknown_132_address());
  // subf. r11,r11,r10
  r11 = r10 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8214ec58
  if (!cr0.eq) goto loc_8214EC58;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8214EC58:
  // stw r11,132(r30)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r30)).unknown_132_address(), uint32_t(r11));
  // b 0x8214ec74
  goto loc_8214EC74;
loc_8214EC60:
  // lwz r10,128(r30)
  r10 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r30)).unknown_128_address());
  // subf. r11,r11,r10
  r11 = r10 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8214ec70
  if (!cr0.eq) goto loc_8214EC70;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8214EC70:
  // stw r11,128(r30)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r30)).unknown_128_address(), uint32_t(r11));
loc_8214EC74:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214F910: lifted from generated/ by lift.py.
void f8214F910(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8214F910 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r3,4
  r11 = uint64_t(int64_t(int64_t(r3) + 4));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::SysLinkBlock20(base, uint32_t(r11) - 4).unknown_4_address());
  // stw r10,0(r4)
  guest::StoreU32(base, schema::SysLinkBlock16(base, uint32_t(r4)).unknown_0_address(), uint32_t(r10));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::SysLinkBlock20(base, uint32_t(r11) - 4).unknown_8_address());
  // stw r10,4(r4)
  guest::StoreU32(base, schema::SysLinkBlock16(base, uint32_t(r4)).unknown_4_address(), uint32_t(r10));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::SysLinkBlock20(base, uint32_t(r11) - 4).unknown_12_address());
  // stw r10,8(r4)
  guest::StoreU32(base, schema::SysLinkBlock16(base, uint32_t(r4)).unknown_8_address(), uint32_t(r10));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::SysLinkBlock20(base, uint32_t(r11) - 4).unknown_16_address());
  // stw r11,12(r4)
  guest::StoreU32(base, schema::SysLinkBlock16(base, uint32_t(r4)).unknown_12_address(), uint32_t(r11));
  // blr
  return;
}

// sub_8214F938: lifted from generated/ by lift.py.
lifted::Result f8214F938(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214F938 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
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
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // mr r30,r3
  r30 = r3;
  // addi r31,r4,144
  r31 = uint64_t(int64_t(int64_t(r4) + 144));
  // addi r25,r4,176
  r25 = uint64_t(int64_t(int64_t(r4) + 176));
  // li r26,4
  r26 = uint64_t(int64_t(4));
  // addi r27,r11,29992
  r27 = uint64_t(int64_t(int64_t(r11) + 29992));
loc_8214F95C:
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, schema::SysLinkRecord(base, uint32_t(r25) - 176).unknown_176_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214fa08
  if (cr6.eq) goto loc_8214FA08;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r30)).voice_engine_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // ld r4,0(r31)
  r4 = guest::LoadU64(base, schema::SysLinkRecord(base, uint32_t(r31) - 144).unknown_144_address());
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_register_remote_talker_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214F98C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r30)).voice_engine_address());
  // ld r4,0(r31)
  r4 = guest::LoadU64(base, schema::SysLinkRecord(base, uint32_t(r31) - 144).unknown_144_address());
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // mr r5,r27
  r5 = r27;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_start_remote_processing_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214F9AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r28,r30,4
  r28 = uint64_t(int64_t(int64_t(r30) + 4));
loc_8214F9B4:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r28) - 4).unknown_4_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214f9f8
  if (cr6.eq) goto loc_8214F9F8;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r4,0(r31)
  r4 = guest::LoadU64(base, schema::SysLinkRecord(base, uint32_t(r31) - 144).unknown_144_address());
  // mr r3,r29
  r3 = r29;
  // bl 0x82576ee0
  // (left out: dead in C++)
  { const lifted::Result o = f82576EE0(base, r1, r13, r3, r4, r5, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r30)).voice_engine_address());
  // mr r5,r29
  r5 = r29;
  // subfic r11,r11,0
  ca = uint32_t(r11) <= 0;
  r11 = static_cast<uint64_t>(0) - r11;
  // ld r4,0(r31)
  r4 = guest::LoadU64(base, schema::SysLinkRecord(base, uint32_t(r31) - 144).unknown_144_address());
  // subfe r6,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r6 = ~r11 + r11 + ca;
  ca = temp.u8;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_set_playback_priority_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214F9F8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214F9F8:
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // cmplwi cr6,r29,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 4, so);
  // blt cr6,0x8214f9b4
  if (cr6.lt) goto loc_8214F9B4;
loc_8214FA08:
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // addi r25,r25,4
  r25 = uint64_t(int64_t(int64_t(r25) + 4));
  // addi r31,r31,8
  r31 = uint64_t(int64_t(int64_t(r31) + 8));
  // bne 0x8214f95c
  if (!cr0.eq) goto loc_8214F95C;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214FA20: lifted from generated/ by lift.py.
lifted::Result f8214FA20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214FA20 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r28,r3
  r28 = r3;
  // addi r31,r4,144
  r31 = uint64_t(int64_t(int64_t(r4) + 144));
  // addi r29,r4,176
  r29 = uint64_t(int64_t(int64_t(r4) + 176));
  // li r30,4
  r30 = uint64_t(int64_t(4));
loc_8214FA3C:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::SysLinkRecord(base, uint32_t(r29) - 176).unknown_176_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214fa60
  if (cr6.eq) goto loc_8214FA60;
  // lwz r3,0(r28)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r28)).voice_engine_address());
  // ld r4,0(r31)
  r4 = guest::LoadU64(base, schema::SysLinkRecord(base, uint32_t(r31) - 144).unknown_144_address());
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_unregister_remote_talker_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214FA60, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214FA60:
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // addi r31,r31,8
  r31 = uint64_t(int64_t(int64_t(r31) + 8));
  // bne 0x8214fa3c
  if (!cr0.eq) goto loc_8214FA3C;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214FC40: lifted from generated/ by lift.py.
lifted::Result f8214FC40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8214FC40 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r5
  r30 = r5;
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // mr r28,r6
  r28 = r6;
  // clrlwi. r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214fcbc
  if (cr0.eq) goto loc_8214FCBC;
loc_8214FC64:
  // lhz r11,1(r31)
  r11 = guest::LoadU16(base, schema::SysLinkMessage3(base, uint32_t(r31)).unknown_1_address());
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).voice_engine_address());
  // addi r5,r31,3
  r5 = uint64_t(int64_t(int64_t(r31) + 3));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lbz r11,0(r31)
  r11 = guest::LoadU8(base, schema::SysLinkMessage3(base, uint32_t(r31)).unknown_0_address());
  // lwz r10,0(r3)
  r10 = lifted::VtableWord(base, uint32_t(r3));
  // addi r11,r11,18
  r11 = uint64_t(int64_t(int64_t(r11) + 18));
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // lwz r10,84(r10)
  r10 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r10)).slot_submit_incoming_chat_data_address());
  // ldx r4,r11,r28
  r4 = guest::LoadU64(base, uint32_t(r11) + uint32_t(r28));
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214FC98, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,1(r31)
  r11 = guest::LoadU16(base, schema::SysLinkMessage3(base, uint32_t(r31)).unknown_1_address());
  // clrlwi r10,r30,16
  r10 = uint32_t(r30) & 0xFFFF;
  // subf r10,r11,r10
  r10 = r10 - r11;
  // add r11,r11,r31
  r11 = r11 + r31;
  // addis r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 65536));
  // addi r31,r11,3
  r31 = uint64_t(int64_t(int64_t(r11) + 3));
  // addi r10,r10,-3
  r10 = uint64_t(int64_t(int64_t(r10) + -3));
  // clrlwi. r30,r10,16
  r30 = uint32_t(r10) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne 0x8214fc64
  if (!cr0.eq) goto loc_8214FC64;
loc_8214FCBC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214FCC8: lifted from generated/ by lift.py.
uint64_t f8214FCC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8214FCC8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r29,r3
  r29 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r4
  r31 = r4;
  // mr r30,r5
  r30 = r5;
  // bl 0x8214a950
  // (left out: dead in C++)
  r3 = f8214A950(base, r1, r13, r3, r4, r5);
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r31)).node_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214fd00
  if (cr6.eq) goto loc_8214FD00;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // beq cr6,0x8214fd04
  if (cr6.eq) goto loc_8214FD04;
loc_8214FD00:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214FD04:
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // subf r9,r11,r10
  r9 = r10 - r11;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm. r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x8214fd40
  if (!cr0.eq) goto loc_8214FD40;
  // ld r11,16(r11)
  r11 = guest::LoadU64(base, uint32_t(r11) + 16);
  // ld r9,0(r30)
  r9 = guest::LoadU64(base, schema::SysLinkId(base, uint32_t(r30)).id_address());
  // cmpld cr6,r9,r11
  cr6 = lifted::Compare<uint64_t>(r9, r11, so);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // blt cr6,0x8214fd30
  if (cr6.lt) goto loc_8214FD30;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8214FD30:
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8214fd40
  if (!cr0.eq) goto loc_8214FD40;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // b 0x8214fd4c
  goto loc_8214FD4C;
loc_8214FD40:
  // stw r10,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r10));
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r31,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r31));
loc_8214FD4C:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mr r3,r29
  r3 = r29;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // stw r10,0(r29)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r29)).container_address(), uint32_t(r10));
  // stw r11,4(r29)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r29)).node_address(), uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_8214FD68: lifted from generated/ by lift.py.
uint64_t f8214FD68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8214FD68 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r11,r4,32
  r11 = uint64_t(int64_t(int64_t(r4) + 32));
  // std r5,144(r1)
  guest::StoreU64(base, uint32_t(r1) + 144, r5);
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r31,r11,r3
  r31 = r11 + r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // lwz r30,4(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x8214fcc8
  // (left out: dead in C++)
  r3 = f8214FCC8(base, r1, r13, r3, r4, r5);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).container_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214fdb4
  if (cr0.eq) goto loc_8214FDB4;
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // beq cr6,0x8214fdb8
  if (cr6.eq) goto loc_8214FDB8;
loc_8214FDB4:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214FDB8:
  // lwz r11,4(r3)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address());
  // subf r11,r11,r30
  r11 = r30 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
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

// sub_8214FDF8: lifted from generated/ by lift.py.
lifted::Result f8214FDF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8214FDF8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // mr r31,r4
  r31 = r4;
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).voice_engine_address());
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_get_remote_talkers_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214FE30, r31)); r3 = o.r3; f1 = o.f1; }
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // mr r28,r24
  r28 = r24;
  // stb r24,8(r31)
  guest::StoreU8(base, schema::SysLinkMessage9(base, uint32_t(r31)).unknown_8_address(), uint8_t(r24));
  // lwz r11,456(r29)
  r11 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).machine_id_address());
  // lbz r11,12(r11)
  r11 = guest::LoadU8(base, schema::NetUserTable(base, uint32_t(r11)).count_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214ff6c
  if (cr0.eq) goto loc_8214FF6C;
  // li r26,144
  r26 = uint64_t(int64_t(144));
  // addi r25,r29,4
  r25 = uint64_t(int64_t(int64_t(r29) + 4));
  // addi r23,r31,41
  r23 = uint64_t(int64_t(int64_t(r31) + 41));
loc_8214FE58:
  // stbx r24,r23,r28
  guest::StoreU8(base, uint32_t(r23) + uint32_t(r28), uint8_t(r24));
  // lwz r11,0(r25)
  r11 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r25) - 4).unknown_4_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214ff50
  if (cr6.eq) goto loc_8214FF50;
  // lbz r11,8(r31)
  r11 = guest::LoadU8(base, schema::SysLinkMessage9(base, uint32_t(r31)).unknown_8_address());
  // mr r27,r24
  r27 = r24;
  // lwz r10,456(r29)
  r10 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).machine_id_address());
  // rotlwi r11,r11,3
  r11 = __builtin_rotateleft32(uint32_t(r11), 3);
  // add r11,r11,r31
  r11 = r11 + r31;
  // ldx r10,r26,r10
  r10 = guest::LoadU64(base, uint32_t(r26) + uint32_t(r10));
  // addi r11,r11,9
  r11 = uint64_t(int64_t(int64_t(r11) + 9));
  // std r10,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r10);
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214ff44
  if (cr6.eq) goto loc_8214FF44;
  // addi r30,r1,96
  r30 = uint64_t(int64_t(int64_t(r1) + 96));
loc_8214FE98:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r4,0(r30)
  r4 = guest::LoadU64(base, uint32_t(r30) + 0);
  // mr r3,r28
  r3 = r28;
  // bl 0x82576ee0
  // (left out: dead in C++)
  { const lifted::Result o = f82576EE0(base, r1, r13, r3, r4, r5, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214fef4
  if (cr6.eq) goto loc_8214FEF4;
  // lbz r11,8(r31)
  r11 = guest::LoadU8(base, schema::SysLinkMessage9(base, uint32_t(r31)).unknown_8_address());
  // ld r9,0(r30)
  r9 = guest::LoadU64(base, uint32_t(r30) + 0);
  // add r8,r11,r31
  r8 = r11 + r31;
  // rotlwi r10,r11,3
  r10 = __builtin_rotateleft32(uint32_t(r11), 3);
  // lbz r11,41(r8)
  r11 = guest::LoadU8(base, uint32_t(r8) + 41);
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // add r11,r11,r31
  r11 = r11 + r31;
  // addi r11,r11,45
  r11 = uint64_t(int64_t(int64_t(r11) + 45));
  // std r9,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r9);
  // lbz r11,8(r31)
  r11 = guest::LoadU8(base, schema::SysLinkMessage9(base, uint32_t(r31)).unknown_8_address());
  // add r11,r11,r31
  r11 = r11 + r31;
  // lbz r10,41(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 41);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stb r10,41(r11)
  guest::StoreU8(base, uint32_t(r11) + 41, uint8_t(r10));
  // b 0x8214ff08
  goto loc_8214FF08;
loc_8214FEF4:
  // mr r4,r28
  r4 = r28;
  // ld r5,0(r30)
  r5 = guest::LoadU64(base, uint32_t(r30) + 0);
  // lwz r3,456(r29)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).machine_id_address());
  // bl 0x8214fd68
  // (left out: dead in C++)
  r3 = f8214FD68(base, r1, r13, r3, r4, r5);
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
loc_8214FF08:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // mr r5,r28
  r5 = r28;
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).voice_engine_address());
  // subfic r11,r11,0
  ca = uint32_t(r11) <= 0;
  r11 = static_cast<uint64_t>(0) - r11;
  // ld r4,0(r30)
  r4 = guest::LoadU64(base, uint32_t(r30) + 0);
  // subfe r6,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r6 = ~r11 + r11 + ca;
  ca = temp.u8;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_set_playback_priority_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214FF30, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // blt cr6,0x8214fe98
  if (cr6.lt) goto loc_8214FE98;
loc_8214FF44:
  // lbz r11,8(r31)
  r11 = guest::LoadU8(base, schema::SysLinkMessage9(base, uint32_t(r31)).unknown_8_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stb r11,8(r31)
  guest::StoreU8(base, schema::SysLinkMessage9(base, uint32_t(r31)).unknown_8_address(), uint8_t(r11));
loc_8214FF50:
  // lwz r11,456(r29)
  r11 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).machine_id_address());
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // addi r26,r26,8
  r26 = uint64_t(int64_t(int64_t(r26) + 8));
  // addi r25,r25,4
  r25 = uint64_t(int64_t(int64_t(r25) + 4));
  // lbz r11,12(r11)
  r11 = guest::LoadU8(base, schema::NetUserTable(base, uint32_t(r11)).count_address());
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // blt cr6,0x8214fe58
  if (cr6.lt) goto loc_8214FE58;
loc_8214FF6C:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214FF78: lifted from generated/ by lift.py.
lifted::Result f8214FF78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8214FF78 (lifted)");
  uint64_t r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // mr r30,r5
  r30 = r5;
  // lwz r3,456(r29)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).machine_id_address());
  // bl 0x8214fd68
  // (left out: dead in C++)
  r3 = f8214FD68(base, r1, r13, r3, r4, r5);
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8214ffc0
  if (!cr0.eq) goto loc_8214FFC0;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82576ee0
  // (left out: dead in C++)
  { const lifted::Result o = f82576EE0(base, r1, r13, r3, r4, r5, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214ffc8
  if (cr6.eq) goto loc_8214FFC8;
loc_8214FFC0:
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // b 0x8214ffcc
  goto loc_8214FFCC;
loc_8214FFC8:
  // li r6,0
  r6 = uint64_t(int64_t(0));
loc_8214FFCC:
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).voice_engine_address());
  // mr r4,r30
  r4 = r30;
  // mr r5,r31
  r5 = r31;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,80(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_set_playback_priority_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214FFE8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821653E8: lifted from generated/ by lift.py.
uint64_t f821653E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821653E8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // mr r25,r3
  r25 = r3;
  // ori r10,r11,15088
  r10 = r11 | 15088;
  // mr r30,r4
  r30 = r4;
  // lwz r11,23340(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 23340);
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821654a8
  if (!cr6.eq) goto loc_821654A8;
  // lwz r11,2184(r30)
  r11 = guest::LoadU32(base, schema::RaceProgress(base, uint32_t(r30)).results_count_address());
  // mr r27,r24
  r27 = r24;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x821654a8
  if (!cr6.gt) goto loc_821654A8;
  // addi r31,r30,2972
  r31 = uint64_t(int64_t(int64_t(r30) + 2972));
  // addi r28,r30,48
  r28 = uint64_t(int64_t(int64_t(r30) + 48));
loc_82165434:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::RaceProgress(base, uint32_t(r28) - 48).slot0_entry_address());
  // lwz r3,20(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 20);
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::Get(base, LIFTED_ARGS(0x82165440, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,6148(r3)
  r11 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r3)).car_flags_address());
  // rlwinm. r11,r11,31,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 31) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82165490
  if (cr0.eq) goto loc_82165490;
  // lwz r11,-784(r31)
  r11 = guest::LoadU32(base, schema::RaceProgress(base, uint32_t(r31) - 2972).e0_record_index_address());
  // mulli r10,r11,3936
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(3936))));
  // lwz r11,23340(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 23340);
  // add r29,r10,r11
  r29 = r10 + r11;
  // lwz r3,3960(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 3960);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82165490
  if (cr6.eq) goto loc_82165490;
  // bl 0x82117178
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Item(base, LIFTED_ARGS(0x8216546C, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,88(r3)
  r11 = guest::LoadU16(base, schema::OpponentItem(base, uint32_t(r3)).race_kind_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82165490
  if (!cr6.eq) goto loc_82165490;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::RaceProgress(base, uint32_t(r31) - 2972).e0_sp_address());
  // li r10,12000
  r10 = uint64_t(int64_t(12000));
  // mulli r11,r11,100
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(100))));
  // divw r11,r11,r10
  r11 = uint32_t((int32_t(r10) && !(int32_t(r11) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r11) / int32_t(r10) : 0);
  // cmpwi cr6,r11,50
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 50, so);
  // ble cr6,0x821654bc
  if (!cr6.gt) goto loc_821654BC;
loc_82165490:
  // lwz r11,2184(r30)
  r11 = guest::LoadU32(base, schema::RaceProgress(base, uint32_t(r30)).results_count_address());
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r28,r28,536
  r28 = uint64_t(int64_t(int64_t(r28) + 536));
  // addi r31,r31,972
  r31 = uint64_t(int64_t(int64_t(r31) + 972));
  // cmpw cr6,r27,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r11), so);
  // blt cr6,0x82165434
  if (cr6.lt) goto loc_82165434;
loc_821654A8:
  // stw r24,0(r25)
  guest::StoreU32(base, schema::RaceCheckResult(base, uint32_t(r25)).end_address(), uint32_t(r24));
  // stw r24,4(r25)
  guest::StoreU32(base, schema::RaceCheckResult(base, uint32_t(r25)).reason_address(), uint32_t(r24));
loc_821654B0:
  // mr r3,r25
  r3 = r25;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
loc_821654BC:
  // mr r11,r30
  r11 = r30;
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // mtctr r10
  ctr = r10;
loc_821654CC:
  // stw r9,0(r11)
  guest::StoreU32(base, schema::RaceProgress(base, uint32_t(r11)).racer_record_0_address(), uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x821654cc
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_821654CC;
  // lwz r11,3960(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 3960);
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::RaceProgress(base, uint32_t(r30)).racer_record_0_address(), uint32_t(r11));
  // stw r10,0(r25)
  guest::StoreU32(base, schema::RaceCheckResult(base, uint32_t(r25)).end_address(), uint32_t(r10));
  // stw r9,4(r25)
  guest::StoreU32(base, schema::RaceCheckResult(base, uint32_t(r25)).reason_address(), uint32_t(r9));
  // b 0x821654b0
  goto loc_821654B0;
}

lifted::Result e82091398(uint8_t* base, const lifted::Args& a) {
  return f82091398(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82091BF8(uint8_t* base, const lifted::Args& a) {
  return {f82091BF8(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82091F58(uint8_t* base, const lifted::Args& a) {
  return {f82091F58(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82099F50(uint8_t* base, const lifted::Args& a) {
  return {f82099F50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8209E4F0(uint8_t* base, const lifted::Args& a) {
  f8209E4F0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8209E540(uint8_t* base, const lifted::Args& a) {
  return f8209E540(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8209E610(uint8_t* base, const lifted::Args& a) {
  return f8209E610(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8209E698(uint8_t* base, const lifted::Args& a) {
  return {f8209E698(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e8209E730(uint8_t* base, const lifted::Args& a) {
  return f8209E730(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8209E790(uint8_t* base, const lifted::Args& a) {
  return f8209E790(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8209E860(uint8_t* base, const lifted::Args& a) {
  return f8209E860(base, a.r1, a.r13, a.r[2], a.r[3], a.f[0]);
}

lifted::Result e8209E9A8(uint8_t* base, const lifted::Args& a) {
  return f8209E9A8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8209ECC8(uint8_t* base, const lifted::Args& a) {
  return f8209ECC8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820A6E60(uint8_t* base, const lifted::Args& a) {
  return f820A6E60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820CC480(uint8_t* base, const lifted::Args& a) {
  return {f820CC480(base, a.r[0]), a.f[0]};
}

lifted::Result e820CC6A0(uint8_t* base, const lifted::Args& a) {
  return {f820CC6A0(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820CC778(uint8_t* base, const lifted::Args& a) {
  return {f820CC778(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820F2CA8(uint8_t* base, const lifted::Args& a) {
  return {f820F2CA8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821059D0(uint8_t* base, const lifted::Args& a) {
  return {f821059D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e82106BB0(uint8_t* base, const lifted::Args& a) {
  return f82106BB0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821075E0(uint8_t* base, const lifted::Args& a) {
  return {f821075E0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82107670(uint8_t* base, const lifted::Args& a) {
  return {f82107670(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821076F8(uint8_t* base, const lifted::Args& a) {
  return f821076F8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82107758(uint8_t* base, const lifted::Args& a) {
  return f82107758(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821097F0(uint8_t* base, const lifted::Args& a) {
  return {f821097F0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82109D80(uint8_t* base, const lifted::Args& a) {
  return f82109D80(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e821105A8(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f821105A8(base, a.r1, a.r[0])};
}

lifted::Result e82110610(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82110610(base, a.r1, a.r[0])};
}

lifted::Result e82110678(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82110678(base, a.r1, a.r[0])};
}

lifted::Result e821106D0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f821106D0(base, a.r1, a.r[0])};
}

lifted::Result e82110750(uint8_t* base, const lifted::Args& a) {
  return {f82110750(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82110798(uint8_t* base, const lifted::Args& a) {
  return {f82110798(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82110928(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82110928(base, a.r1, a.r[0])};
}

lifted::Result e82110970(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82110970(base, a.r1, a.r[0])};
}

lifted::Result e82110B40(uint8_t* base, const lifted::Args& a) {
  return {f82110B40(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82110BE8(uint8_t* base, const lifted::Args& a) {
  return {f82110BE8(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82110CA8(uint8_t* base, const lifted::Args& a) {
  return {f82110CA8(base, a.r1, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82111140(uint8_t* base, const lifted::Args& a) {
  return f82111140(base, a.r1, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e821115C8(uint8_t* base, const lifted::Args& a) {
  return {f821115C8(base, a.r1, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82112260(uint8_t* base, const lifted::Args& a) {
  return f82112260(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e82115550(uint8_t* base, const lifted::Args& a) {
  return {f82115550(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82115708(uint8_t* base, const lifted::Args& a) {
  return {f82115708(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82119EA8(uint8_t* base, const lifted::Args& a) {
  return {f82119EA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8211A130(uint8_t* base, const lifted::Args& a) {
  return {f8211A130(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82148768(uint8_t* base, const lifted::Args& a) {
  return {f82148768(base, a.r[0]), a.f[0]};
}

lifted::Result e82148F80(uint8_t* base, const lifted::Args& a) {
  return f82148F80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82148F88(uint8_t* base, const lifted::Args& a) {
  f82148F88(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82148FD8(uint8_t* base, const lifted::Args& a) {
  return f82148FD8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e821492B8(uint8_t* base, const lifted::Args& a) {
  return f821492B8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82149430(uint8_t* base, const lifted::Args& a) {
  return f82149430(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.f[0]);
}

lifted::Result e821496F8(uint8_t* base, const lifted::Args& a) {
  f821496F8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82149830(uint8_t* base, const lifted::Args& a) {
  return {f82149830(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82149960(uint8_t* base, const lifted::Args& a) {
  return {f82149960(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e821499B8(uint8_t* base, const lifted::Args& a) {
  return f821499B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82149B68(uint8_t* base, const lifted::Args& a) {
  return {f82149B68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82149BF0(uint8_t* base, const lifted::Args& a) {
  f82149BF0(base, a.r1, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82149CA0(uint8_t* base, const lifted::Args& a) {
  return {f82149CA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82149D60(uint8_t* base, const lifted::Args& a) {
  return {f82149D60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82149DC0(uint8_t* base, const lifted::Args& a) {
  return f82149DC0(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8214A198(uint8_t* base, const lifted::Args& a) {
  f8214A198(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8214A280(uint8_t* base, const lifted::Args& a) {
  return {f8214A280(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8214A3B0(uint8_t* base, const lifted::Args& a) {
  return f8214A3B0(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e8214A488(uint8_t* base, const lifted::Args& a) {
  return {f8214A488(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8214A618(uint8_t* base, const lifted::Args& a) {
  return f8214A618(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[7], a.f[0]);
}

lifted::Result e8214A728(uint8_t* base, const lifted::Args& a) {
  return f8214A728(base, a.r1, a.r13, a.r[5], a.f[0]);
}

lifted::Result e8214A8E0(uint8_t* base, const lifted::Args& a) {
  return {f8214A8E0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8214A950(uint8_t* base, const lifted::Args& a) {
  return {f8214A950(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8214A9B0(uint8_t* base, const lifted::Args& a) {
  return {f8214A9B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8214AC68(uint8_t* base, const lifted::Args& a) {
  return f8214AC68(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8214B0A8(uint8_t* base, const lifted::Args& a) {
  return f8214B0A8(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e8214B200(uint8_t* base, const lifted::Args& a) {
  return {f8214B200(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e8214B260(uint8_t* base, const lifted::Args& a) {
  return f8214B260(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214B870(uint8_t* base, const lifted::Args& a) {
  return f8214B870(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214BA60(uint8_t* base, const lifted::Args& a) {
  return f8214BA60(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8214BAF8(uint8_t* base, const lifted::Args& a) {
  return f8214BAF8(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8214BCE0(uint8_t* base, const lifted::Args& a) {
  return f8214BCE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8214C708(uint8_t* base, const lifted::Args& a) {
  return f8214C708(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8214C9A0(uint8_t* base, const lifted::Args& a) {
  return f8214C9A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e8214CCB8(uint8_t* base, const lifted::Args& a) {
  return f8214CCB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8214CD20(uint8_t* base, const lifted::Args& a) {
  return f8214CD20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e8214CE00(uint8_t* base, const lifted::Args& a) {
  return f8214CE00(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214CEF0(uint8_t* base, const lifted::Args& a) {
  return f8214CEF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e8214CFB8(uint8_t* base, const lifted::Args& a) {
  return f8214CFB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4]);
}

lifted::Result e8214D560(uint8_t* base, const lifted::Args& a) {
  return f8214D560(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e8214D7A0(uint8_t* base, const lifted::Args& a) {
  return f8214D7A0(base, a.r1, a.r13, a.r[3], a.r[4], a.r[6]);
}

lifted::Result e8214E778(uint8_t* base, const lifted::Args& a) {
  return f8214E778(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8214E818(uint8_t* base, const lifted::Args& a) {
  return {f8214E818(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8214E850(uint8_t* base, const lifted::Args& a) {
  return f8214E850(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8214E908(uint8_t* base, const lifted::Args& a) {
  return f8214E908(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8214E9C8(uint8_t* base, const lifted::Args& a) {
  return f8214E9C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214EB28(uint8_t* base, const lifted::Args& a) {
  return f8214EB28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214F910(uint8_t* base, const lifted::Args& a) {
  f8214F910(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8214F938(uint8_t* base, const lifted::Args& a) {
  return f8214F938(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214FA20(uint8_t* base, const lifted::Args& a) {
  return f8214FA20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214FC40(uint8_t* base, const lifted::Args& a) {
  return f8214FC40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8214FCC8(uint8_t* base, const lifted::Args& a) {
  return {f8214FCC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8214FD68(uint8_t* base, const lifted::Args& a) {
  return {f8214FD68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8214FDF8(uint8_t* base, const lifted::Args& a) {
  return f8214FDF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8214FF78(uint8_t* base, const lifted::Args& a) {
  return f8214FF78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e821653E8(uint8_t* base, const lifted::Args& a) {
  return {f821653E8(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

namespace {

const Registration kEntries[] = {
    {0x82091398, e82091398},
    {0x82091BF8, e82091BF8},
    {0x82091F58, e82091F58},
    {0x82099F50, e82099F50},
    {0x8209E4F0, e8209E4F0},
    {0x8209E540, e8209E540},
    {0x8209E610, e8209E610},
    {0x8209E698, e8209E698},
    {0x8209E730, e8209E730},
    {0x8209E790, e8209E790},
    {0x8209E860, e8209E860},
    {0x8209E9A8, e8209E9A8},
    {0x8209ECC8, e8209ECC8},
    {0x820A6E60, e820A6E60},
    {0x820CC480, e820CC480},
    {0x820CC6A0, e820CC6A0},
    {0x820CC778, e820CC778},
    {0x820F2CA8, e820F2CA8},
    {0x821059D0, e821059D0},
    {0x82106BB0, e82106BB0},
    {0x821075E0, e821075E0},
    {0x82107670, e82107670},
    {0x821076F8, e821076F8},
    {0x82107758, e82107758},
    {0x821097F0, e821097F0},
    {0x82109D80, e82109D80},
    {0x821105A8, e821105A8},
    {0x82110610, e82110610},
    {0x82110678, e82110678},
    {0x821106D0, e821106D0},
    {0x82110750, e82110750},
    {0x82110798, e82110798},
    {0x82110928, e82110928},
    {0x82110970, e82110970},
    {0x82110B40, e82110B40},
    {0x82110BE8, e82110BE8},
    {0x82110CA8, e82110CA8},
    {0x82111140, e82111140},
    {0x821115C8, e821115C8},
    {0x82112260, e82112260},
    {0x82115550, e82115550},
    {0x82115708, e82115708},
    {0x82119EA8, e82119EA8},
    {0x8211A130, e8211A130},
    {0x82148768, e82148768},
    {0x82148F80, e82148F80},
    {0x82148F88, e82148F88},
    {0x82148FD8, e82148FD8},
    {0x821492B8, e821492B8},
    {0x82149430, e82149430},
    {0x821496F8, e821496F8},
    {0x82149830, e82149830},
    {0x82149960, e82149960},
    {0x821499B8, e821499B8},
    {0x82149B68, e82149B68},
    {0x82149BF0, e82149BF0},
    {0x82149CA0, e82149CA0},
    {0x82149D60, e82149D60},
    {0x82149DC0, e82149DC0},
    {0x8214A198, e8214A198},
    {0x8214A280, e8214A280},
    {0x8214A3B0, e8214A3B0},
    {0x8214A488, e8214A488},
    {0x8214A618, e8214A618},
    {0x8214A728, e8214A728},
    {0x8214A8E0, e8214A8E0},
    {0x8214A950, e8214A950},
    {0x8214A9B0, e8214A9B0},
    {0x8214AC68, e8214AC68},
    {0x8214B0A8, e8214B0A8},
    {0x8214B200, e8214B200},
    {0x8214B260, e8214B260},
    {0x8214B870, e8214B870},
    {0x8214BA60, e8214BA60},
    {0x8214BAF8, e8214BAF8},
    {0x8214BCE0, e8214BCE0},
    {0x8214C708, e8214C708},
    {0x8214C9A0, e8214C9A0},
    {0x8214CCB8, e8214CCB8},
    {0x8214CD20, e8214CD20},
    {0x8214CE00, e8214CE00},
    {0x8214CEF0, e8214CEF0},
    {0x8214CFB8, e8214CFB8},
    {0x8214D560, e8214D560},
    {0x8214D7A0, e8214D7A0},
    {0x8214E778, e8214E778},
    {0x8214E818, e8214E818},
    {0x8214E850, e8214E850},
    {0x8214E908, e8214E908},
    {0x8214E9C8, e8214E9C8},
    {0x8214EB28, e8214EB28},
    {0x8214F910, e8214F910},
    {0x8214F938, e8214F938},
    {0x8214FA20, e8214FA20},
    {0x8214FC40, e8214FC40},
    {0x8214FCC8, e8214FCC8},
    {0x8214FD68, e8214FD68},
    {0x8214FDF8, e8214FDF8},
    {0x8214FF78, e8214FF78},
    {0x821653E8, e821653E8},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_036)), true);

}  // namespace

}  // namespace lifted
