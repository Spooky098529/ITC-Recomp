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
// The panels' vtable (PanelGroupBase's too).
constexpr uint32_t kPanelVtable = 0x82010974;

// Panel6C's and Panel7C's words.
void PanelC(uint8_t* base, uint32_t panel, uint32_t group, uint32_t vtable, uint32_t w20,
            uint32_t w24, uint32_t w32) {
  PanelCBase(base, panel);
  Store(base, panel + 12, group);
  Store(base, panel + 20, w20);
  Store(base, panel + 0, vtable);
  Store(base, panel + 4, 0);
  Store(base, panel + 24, w24);
  Store(base, panel + 32, w32);
  Store(base, panel + 40, 109);
  Fill(base, panel + 44, panel + 56, kNone);
  Store(base, panel + 60, 112);
  Store(base, panel + 68, 5);
  Store(base, panel + 72, 6);
  Store(base, panel + 108, 15);
  Store(base, panel + 112, 16);
  Store(base, panel + 124, 0);
  Store(base, panel + 128, 43);
  Store(base, panel + 132, 13);
  Store(base, panel + 136, 14);
  Store(base, panel + 140, 15);
  Store(base, panel + 144, 5);
  Store(base, panel + 152, 0);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, panel + 160 + 4 * i, 5 + i);  // +160 to +172: 5 to 8
  }
  Store(base, panel + 176, 11);
  Store(base, panel + 180, 12);
  Store(base, panel + 216, 9);
  Store(base, panel + 220, 10);
  Store(base, panel + 224, 13);
  Store(base, panel + 228, 14);
  Store(base, panel + 272, 4);
  Store(base, panel + 276, 123);
  Store(base, panel + 280, 112);
}

// Panel6D's and Panel7D's words (+28 to +40: `w28` up).
void PanelD(uint8_t* base, uint32_t panel, uint32_t group, uint32_t w12, uint32_t w24,
            uint32_t w28, uint32_t w60) {
  PanelDBase(base, panel);
  Store(base, panel + 0, kPanelVtable);
  Store(base, panel + 4, group);
  Store(base, panel + 12, w12);
  Store(base, panel + 16, 110);
  Store(base, panel + 20, 111);
  Store(base, panel + 24, w24);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, panel + 28 + 4 * i, w28 + i);
  }
  Store(base, panel + 60, w60);
  Store(base, panel + 64, 0);
  Store(base, panel + 68, 1);
  Store(base, panel + 80, 2);
}

// Panel6F's and Panel7F's words (the same in both).
void PanelF(uint8_t* base, uint32_t panel, uint32_t group) {
  PanelFBase(base, panel);
  Store(base, panel + 0, kPanelVtable);
  Store(base, panel + 4, group);
  Store(base, panel + 28, 3);
  Store(base, panel + 32, 6);
  Store(base, panel + 36, 0);
  Store(base, panel + 40, 0);
  Store(base, panel + 44, 5);
  Store(base, panel + 48, 6);
  Store(base, panel + 52, 7);
  Store(base, panel + 84, 4);
  Store(base, panel + 88, 1);
  Store(base, panel + 92, 0);
  Store(base, panel + 96, 5);
  Store(base, panel + 100, 15);
  Store(base, panel + 104, 16);
  Store(base, panel + 108, 29);
  Store(base, panel + 112, 30);
  Store(base, panel + 116, 79);
  Store(base, panel + 120, 73);
  Store(base, panel + 124, 2);
  Store(base, panel + 132, 75);
  Store(base, panel + 136, 83);
}

// The three PanelGroups' words.
void PanelGroupWords(uint8_t* base, uint32_t group, uint32_t w4, uint32_t w12) {
  PanelGroupBase(base, group);
  Store(base, group + 4, w4);
  Store(base, group + 0, 0x82010E8C);
  Store(base, group + 12, w12);
  Store(base, group + 32, 0);
}

// The node tables' four pairs (0, -1) at +428 to +456.
void NodeTablePairs(uint8_t* base, uint32_t table) {
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, table + 428 + 8 * i, 0);
    Store(base, table + 432 + 8 * i, kNone);
  }
}

// slw: 1 shifted left by `amount`'s low six bits (0 from 32 up).
uint32_t Bit(uint32_t amount) {
  return (amount & 0x20) != 0 ? 0 : 1u << (amount & 0x1F);
}

// Type7ListScreen's items' records (28 bytes; +0 a count, +12 and +16 bit
// numbers).
constexpr uint32_t kType7Records = 0x826370A0;
constexpr uint32_t kType7RecordBytes = 28;
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

void Panel6C(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82231CD0 Panel6C::Panel6C");
  PanelC(base, panel, group, 0x82010980, 7, 0, 5);
}

void Panel7C(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_822416D8 Panel7C::Panel7C");
  PanelC(base, panel, group, 0x82010EE4, 6, 4, 3);
}

void Panel6D(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82231E40 Panel6D::Panel6D");
  PanelD(base, panel, group, 22, 96, 105, 21);
}

void Panel7D(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_822418B8 Panel7D::Panel7D");
  PanelD(base, panel, group, 20, 94, 103, 19);
}

void Panel6F(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_822308E8 Panel6F::Panel6F");
  PanelF(base, panel, group);
}

void Panel7F(uint8_t* base, uint32_t panel, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82240268 Panel7F::Panel7F");
  PanelF(base, panel, group);
}

void PanelGroup(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82240110 PanelGroup::PanelGroup");
  PanelGroupWords(base, group, 4, 1);
}

void PanelGroup6B(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82231C68 PanelGroup6B::PanelGroup6B");
  PanelGroupWords(base, group, 28, 4);
}

void PanelGroup7B(uint8_t* base, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82241680 PanelGroup7B::PanelGroup7B");
  PanelGroupWords(base, group, 28, 2);
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

void PairedNodeTable(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82247C60 PairedNodeTable::PairedNodeTable");
  NodeTable(base, table);
  Store(base, table + 0, 0x82010FF8);
  NodeTablePairs(base, table);
}

void ColorNodeTable(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82247D08 ColorNodeTable::ColorNodeTable");
  NodeTable(base, table);
  Store(base, table + 0, 0x82011000);
  NodeTablePairs(base, table);  // +460 to +472 not written
  Fill(base, table + 476, table + 548, kNone);
  Fill(base, table + 552, table + 564, 255);
  Store(base, table + 568, 205);
  Store(base, table + 572, 235);
  Store(base, table + 576, 67);
  Store(base, table + 580, 255);
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

void ColorListScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_822550F8 ColorListScreen::ColorListScreen");
  ListScreen(base, screen);
  Store(base, screen + 0, 0x820114E0);
  for (uint32_t offset : {10204u, 10212u, 10276u, 10284u, 10348u, 10356u, 10420u, 10428u}) {
    Store(base, screen + offset, 0);
  }
  ColorNodeTable(base, screen + 10432);
  const uint32_t count = Load(base, screen + 9680);
  Store(base, screen + 9672, screen + 10432);
  Store(base, screen + 10436, count);
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, screen + 11016 + 4 * i, 545 + i);  // +11016 to +11032: 545 to 549
  }
  Store(base, screen + 11036, kNone);
}

void Type7ListScreen(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_82242BE8 Type7ListScreen::Type7ListScreen");
  ColorListScreen(base, screen);
  Store(base, screen + 0, 0x82010F18);
  ListPart(base, screen + 11040);
  Store(base, screen + 9676, screen + 11040);
  static constexpr uint32_t kWords[][2] = {
      {9832, 165},  {9840, 166},  {9844, 168},  {9848, 167},  {9852, 169},  {9856, 170},
      {9860, 0},    {9868, 47},   {9872, 171},  {9876, 5},    {9884, 82},   {9892, 6},
      {9896, 7},    {9900, 87},   {9904, 139},  {9908, 140},  {9912, 100},  {9916, 101},
      {9920, 20},   {9924, 21},   {9728, 6},    {9724, 16},   {9720, 11},   {10916, 79},
      {10920, 76},  {10924, 77},  {10932, 78},  {10936, 80},  {10940, 81},  {10952, 67},
      {10956, 64},  {10960, 65},  {10968, 66},  {10972, 68},  {10976, 69},
      {10440, 12},  // the node table's +8 and +12, stored directly
      {10444, 11},
  };
  for (const auto& w : kWords) {
    Store(base, screen + w[0], w[1]);
  }
  const uint32_t table = Load(base, screen + 9672);
  Store(base, screen + 9716, 10);
  Store(base, screen + 9712, 0);
  Store(base, screen + 9800, 0);
  Store(base, screen + 9752, 8);
  Store(base, screen + 9756, 9);
  Store(base, screen + 9804, 1);
  Store(base, screen + 9768, 3);
  Store(base, screen + 9748, 0);
  Store(base, table + 296, 6);
  for (uint32_t i = 1; i < 4; i++) {
    Store(base, Load(base, screen + 9672) + 296 + 4 * i, 6 + i);  // +300 to +308: 7 to 9
  }
  Store(base, screen + 9784, 2);
  Store(base, screen + 9788, 1);
  // 15 items, through the table at +10432 directly: +16 + 4i = 48 + i,
  // +156 + 4i = 1 + i, +312 + 4i = the record's mask.
  for (uint32_t i = 0; i < 15; i++) {
    const uint32_t item = screen + 10448 + 4 * i;
    Store(base, item, 48 + i);
    Store(base, item + 140, 1 + i);
    const uint32_t record = kType7Records + kType7RecordBytes * i;
    const int32_t count = int32_t(Load(base, record));
    uint32_t mask = 0;
    if (count >= 2) {
      mask = Bit(Load(base, record + 16)) | Bit(Load(base, record + 12));
    } else if (count != 0) {
      mask = Bit(Load(base, record + 12));
    }
    Store(base, item + 296, mask);
  }
  Store(base, screen + 10852, 5);
  Store(base, screen + 10856, 20);
  for (uint32_t i = 0; i < 3; i++) {
    Store(base, screen + 9936 + 4 * i, 129 + i);
    Store(base, screen + 9976 + 4 * i, 90 + i);
    Store(base, screen + 10016 + 4 * i, 10 + i);
  }
  Store(base, screen + 9736, 9);
  Store(base, screen + 9808, 0);
  Store(base, screen + 9732, 2);
  Store(base, screen + 9740, 1);
  Store(base, screen + 9816, 0);
  Store(base, screen + 9744, 21);
  Store(base, screen + 10056, 141);
  Store(base, screen + 9776, 0x826371A4);
  Store(base, screen + 10060, 142);
  Store(base, screen + 10064, 143);
  Store(base, screen + 10096, 155);
  Store(base, screen + 10104, 157);
  Store(base, screen + 9812, 4);
  Store(base, screen + 10100, 156);
  Store(base, screen + 9828, 0);
  Store(base, screen + 9684, 0);
  Store(base, screen + 9928, 152);
  Store(base, screen + 9932, 151);
}

}  // namespace screen_constructors
