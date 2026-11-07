// The type 6 and type 7 modes' screen constructors as plain C++
// (screen_constructors.h, docs/functions/screen-constructors.md), read from
// the generated code.

#include "screen_constructors/screen_constructors.h"

#include "base/log.h"
#include "boot_mode/boot_mode.h"
#include "guest.h"

namespace screen_constructors {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// `value` in the words from `from` to `to` (both included).
void Fill(uint8_t* base, uint32_t from, uint32_t to, uint32_t value) {
  for (uint32_t a = from; a <= to; a += 4) {
    Store(base, a, value);
  }
}

constexpr uint32_t kNone = 0xFFFFFFFF;  // -1
// The empty base many classes share (as ListNode's in archive.md).
constexpr uint32_t kEmptyBaseVtable = 0x82010B5C;

}  // namespace

// --- The panels and their groups ---

void NodeBase(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82258FB0 NodeBase::NodeBase");
  Store(base, object + 4, 0);
  Store(base, object + 0, kEmptyBaseVtable);
}

void PanelGroupBase(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_8224EEA8 PanelGroupBase::PanelGroupBase");
  NodeBase(base, group);
  Store(base, group + 0, 0x82010974);
  Fill(base, group + 8, group + 32, kNone);
}

void PanelCBase(uint8_t* base, uint32_t panel) {
  ITC_NATIVE_RUNNING("sub_8224D6D0 PanelCBase::PanelCBase");
  Store(base, panel + 0, 0x820111C0);
  Store(base, panel + 4, 1);
  Store(base, panel + 8, kNone);
  Store(base, panel + 12, 0);
  Store(base, panel + 16, kNone);
  Fill(base, panel + 24, panel + 140, kNone);
  Fill(base, panel + 148, panel + 244, kNone);
  Fill(base, panel + 260, panel + 272, kNone);
  Fill(base, panel + 276, panel + 316, 0);
}

void PanelDBase(uint8_t* base, uint32_t panel) {
  ITC_NATIVE_RUNNING("sub_8224CFA0 PanelDBase::PanelDBase");
  Store(base, panel + 4, 0);
  Store(base, panel + 0, kEmptyBaseVtable);
  Fill(base, panel + 8, panel + 80, kNone);
  Store(base, panel + 88, 0);
}

void PanelEBase(uint8_t* base, uint32_t panel) {
  ITC_NATIVE_RUNNING("sub_8224EF78 PanelEBase::PanelEBase");
  Store(base, panel + 0, 0);
  Store(base, panel + 4, 0);
  Fill(base, panel + 8, panel + 148, kNone);
  Store(base, panel + 152, 0);
  Store(base, panel + 156, 0);
}

void PanelE(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82230800 PanelE::PanelE");
  PanelEBase(base, panel);
  Store(base, panel + 0, group);  // no vtable
  Store(base, panel + 4, 4);
  Store(base, panel + 16, 4);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, panel + 20 + 4 * i, 5 + i);   // +20 to +32: 5 to 8
    Store(base, panel + 124 + 4 * i, 5 + i);  // +124 to +136: 5 to 8
  }
  Store(base, panel + 60, 9);
  Store(base, panel + 64, 5);
  Store(base, panel + 68, 15);
  Store(base, panel + 72, 16);
  Store(base, panel + 76, 80);
  Store(base, panel + 80, 81);
  Store(base, panel + 88, 82);
  Store(base, panel + 92, 62);
  Store(base, panel + 96, 67);
  Store(base, panel + 100, 68);
  Store(base, panel + 104, 72);
  Store(base, panel + 112, 2);
  Store(base, panel + 116, 83);
  Store(base, panel + 120, 3);
  Store(base, panel + 140, kNone);
  Store(base, panel + 144, 1);
  Store(base, panel + 148, 3);
}

void PanelFBase(uint8_t* base, uint32_t panel) {
  ITC_NATIVE_RUNNING("sub_8224FA88 PanelFBase::PanelFBase");
  Store(base, panel + 4, 0);
  Store(base, panel + 0, kEmptyBaseVtable);
  Fill(base, panel + 24, panel + 136, kNone);
  Store(base, panel + 140, 0);
}

void PanelSetBase(uint8_t* base, uint32_t set_base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224AD28 PanelSetBase::PanelSetBase");
  Store(base, set_base + 0, 0x820110C4);
  // The original clears eleven words below its r1 (it has no frame) and
  // reads ten of them back into its fields.
  Fill(base, sp - 48, sp - 8, 0);
  Store(base, set_base + 128, 0);
  Store(base, set_base + 120, 0);
  Store(base, set_base + 200, Load(base, sp - 44));
  Store(base, set_base + 192, Load(base, sp - 40));
  Store(base, set_base + 784, Load(base, sp - 36));
  Store(base, set_base + 1300, Load(base, sp - 32));
  Store(base, set_base + 1816, Load(base, sp - 28));
  Store(base, set_base + 1876, Load(base, sp - 24));
  const uint32_t at_20 = Load(base, sp - 20);
  Store(base, set_base + 224, kNone);
  Store(base, set_base + 204, at_20);
  Store(base, set_base + 208, Load(base, sp - 16));
  Store(base, set_base + 212, Load(base, sp - 12));
  Store(base, set_base + 216, Load(base, sp - 8));
  Store(base, set_base + 220, 0);
  Fill(base, set_base + 228, set_base + 248, 0);
  Store(base, set_base + 1916, 0);
  Store(base, set_base + 1920, 1);
  Store(base, set_base + 4, 0);
  Store(base, set_base + 12, 0);
  Store(base, set_base + 1928, 1);
  Fill(base, set_base + 16, set_base + 28, 0);
  Store(base, kPanelWord, 0);
  Store(base, set_base + 1932, kNone);
  Store(base, set_base + 1936, 1);
  Store(base, set_base + 1940, 1);
  Store(base, set_base + 1944, 0);
  Store(base, set_base + 1948, 1);
  Store(base, set_base + 1952, kNone);
  Fill(base, set_base + 1892, set_base + 1912, 0);
  Store(base, set_base + 1956, 0);
  Fill(base, set_base + 1960, set_base + 1968, kNone);
}

void PanelSet(uint8_t* base, uint32_t set, uint32_t f, uint32_t e, uint32_t d, uint32_t c,
              uint32_t group_b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8224B660 PanelSet::PanelSet");
  Store(base, set + 0, 0x820110C8);
  PanelSetBase(base, set + 4, sp - kPanelSetFrame);
  // The panels get the word as it reads after each store (not checked for 0).
  Store(base, kPanelWord, 0);
  Store(base, set + 1900, f);
  Store(base, f + 140, Load(base, kPanelWord));
  Store(base, set + 1904, e);
  Store(base, e + 156, Load(base, kPanelWord));
  Store(base, set + 1908, d);
  Store(base, d + 88, Load(base, kPanelWord));
  Store(base, set + 1912, c);
  Store(base, c + 316, Load(base, kPanelWord));
  Store(base, set + 1960, group_b);
}

void GroupPair(uint8_t* base, uint32_t pair, uint32_t group_a, uint32_t group_b) {
  ITC_NATIVE_RUNNING("sub_82249C08 GroupPair::GroupPair");
  Store(base, pair + 8, group_a);
  Store(base, pair + 12, group_b);
  Store(base, pair + 0, 0x82011084);
}

// --- The screens' bases ---

void MachineScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_8224CF58 MachineScreen::MachineScreen");
  boot_mode::ScreenMachineConstruct(base, screen + 4);
  Store(base, screen + 9652, 0);
  Store(base, screen + 0, 0x82011168);
}

void Type6Screen0(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_82235BD8 Type6Screen0::Type6Screen0");
  boot_mode::ScreenMachineConstruct(base, screen);
}

void Type7Screen0(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_82245FF8 Type7Screen0::Type7Screen0");
  boot_mode::ScreenMachineConstruct(base, screen + 4);
  Store(base, screen + 0, kEmptyBaseVtable);
}

// --- The list screens' parts ---

void NodeTable(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82247BE8 NodeTable::NodeTable");
  Store(base, table + 0, 0x82010FF0);
  Fill(base, table + 8, table + 308, kNone);
  Fill(base, table + 312, table + 416, 0);
  Store(base, table + 420, kNone);
  Store(base, table + 424, kNone);
}

void ListScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_82252290 ListScreen::ListScreen");
  boot_mode::ScreenMachineConstruct(base, screen + 4);
  Store(base, screen + 0, 0x82011218);
  Store(base, screen + 9676, 0);
  Store(base, screen + 9680, 1);
  Store(base, screen + 9684, 1);
  Store(base, screen + 9708, 0);
  Fill(base, screen + 9712, screen + 9768, kNone);
  Store(base, screen + 9772, 0);
  Store(base, screen + 9776, 0);  // +9780 not written
  Store(base, screen + 9784, 0);
  Store(base, screen + 9788, 1);
  // +9792 to +9932, then five runs of ten words from +9936 (to +10132).
  Fill(base, screen + 9792, screen + 10132, kNone);
  Store(base, screen + 10136, 0);
  Store(base, screen + 10140, kNone);
}

void ListPartBase(uint8_t* base, uint32_t part) {
  ITC_NATIVE_RUNNING("sub_822523F8 ListPartBase::ListPartBase");
  Store(base, part + 0, 0x820112A0);
  Fill(base, part + 4, part + 16, 0);
}

void ListPartInnerBase(uint8_t* base, uint32_t inner) {
  ITC_NATIVE_RUNNING("sub_822525E0 ListPartInnerBase::ListPartInnerBase");
  Store(base, inner + 0, 0x820112CC);
  Store(base, inner + 32, 0);
  Store(base, inner + 36, 0);
}

void ListPartInner(uint8_t* base, uint32_t inner) {
  ITC_NATIVE_RUNNING("sub_822526A0 ListPartInner::ListPartInner");
  ListPartInnerBase(base, inner);
  Store(base, inner + 0, 0x820112F8);
  Store(base, inner + 100, 0);
  Store(base, inner + 108, 0);
  Store(base, inner + 112, 3);
  Store(base, inner + 116, 0);
}

void ListPart(uint8_t* base, uint32_t part) {
  ITC_NATIVE_RUNNING("sub_82252908 ListPart::ListPart");
  ListPartBase(base, part);
  Store(base, part + 0, 0x82011314);
  ListPartInner(base, part + 20);
  Store(base, part + 12, 16);
  Store(base, part + 16, part + 20);
}

}  // namespace screen_constructors
