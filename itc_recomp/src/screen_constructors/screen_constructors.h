// The type 6 and type 7 modes' screen constructors as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/screen-constructors.md, the blocks' layouts in
// docs/known-data.md: "type 6 screens", "type 7 screens"). The screens are
// guest data shared with the recompiled code, so these store into guest
// memory what the originals store (constants, `this`-relative addresses and
// their arguments). Where an original leaves words in the stack (NodeScreen,
// PanelSetBase), they are written at the same places from `sp`, the
// original's r1 on entry (callees' entry r1 is `sp` less the caller's frame,
// as the originals' calls have it). The originals' saved registers and back
// chains aren't written. Adapters: src/hooks/screen_constructors.cpp
// (--native_screen_constructors).

#pragma once

#include <cstdint>

namespace screen_constructors {

// The word the panel sets clear and copy into their panels (known-data.md).
constexpr uint32_t kPanelWord = 0x82815EAC;

// The frames of the originals that call others with stack leftovers below
// them.
constexpr uint32_t kPanelSetFrame = 112;

// --- The panels and their groups (the panel screens' members) ---

// sub_82258FB0 NodeBase::NodeBase: +0 = vtable 0x82010B5C, +4 = 0.
void NodeBase(uint8_t* base, uint32_t object);
// sub_8224EEA8 PanelGroupBase::PanelGroupBase: NodeBase, vtable 0x82010974,
// +8 to +32 = -1.
void PanelGroupBase(uint8_t* base, uint32_t group);
// sub_8224D6D0 PanelCBase::PanelCBase (a leaf).
void PanelCBase(uint8_t* base, uint32_t panel);
// sub_8224CFA0 PanelDBase::PanelDBase (a leaf).
void PanelDBase(uint8_t* base, uint32_t panel);
// sub_8224EF78 PanelEBase::PanelEBase (a leaf) and sub_82230800
// PanelE::PanelE (`group`: group A).
void PanelEBase(uint8_t* base, uint32_t panel);
void PanelE(uint8_t* base, uint32_t panel, uint32_t group);
// sub_8224FA88 PanelFBase::PanelFBase (a leaf).
void PanelFBase(uint8_t* base, uint32_t panel);
// sub_82231CD0 Panel6C::Panel6C and sub_822416D8 Panel7C::Panel7C (320
// bytes; `group`: group B): PanelCBase, then their words (the two differ in
// the vtable, +20, +24 and +32).
void Panel6C(uint8_t* base, uint32_t panel, uint32_t group);
void Panel7C(uint8_t* base, uint32_t panel, uint32_t group);
// sub_82231E40 Panel6D::Panel6D and sub_822418B8 Panel7D::Panel7D (92
// bytes; `group`: group B): PanelDBase, then their words.
void Panel6D(uint8_t* base, uint32_t panel, uint32_t group);
void Panel7D(uint8_t* base, uint32_t panel, uint32_t group);
// sub_822308E8 Panel6F::Panel6F and sub_82240268 Panel7F::Panel7F (144
// bytes; `group`: group A): PanelFBase, then the same words (two copies of
// the code).
void Panel6F(uint8_t* base, uint32_t panel, uint32_t group);
void Panel7F(uint8_t* base, uint32_t panel, uint32_t group);
// sub_82240110 PanelGroup::PanelGroup (group A), sub_82231C68
// PanelGroup6B::PanelGroup6B and sub_82241680 PanelGroup7B::PanelGroup7B
// (group B): PanelGroupBase, then +4, +12 and +32.
void PanelGroup(uint8_t* base, uint32_t group);
void PanelGroup6B(uint8_t* base, uint32_t group);
void PanelGroup7B(uint8_t* base, uint32_t group);
// sub_8224AD28 PanelSetBase::PanelSetBase (a leaf without a frame: eleven
// words below `sp` cleared, ten read back into the base) and sub_8224B660
// PanelSet::PanelSet (panels F, E, D and C, group B; the panels written
// without null checks).
void PanelSetBase(uint8_t* base, uint32_t set_base, uint32_t sp);
void PanelSet(uint8_t* base, uint32_t set, uint32_t f, uint32_t e, uint32_t d, uint32_t c,
              uint32_t group_b, uint32_t sp);
// sub_82249C08 GroupPair::GroupPair: +0 = vtable, +8 = group A, +12 =
// group B (+4 not written).
void GroupPair(uint8_t* base, uint32_t pair, uint32_t group_a, uint32_t group_b);

// --- The screens' bases ---

// sub_8224CF58 MachineScreen::MachineScreen: ScreenMachine on +4, vtable,
// +9652 = 0.
void MachineScreen(uint8_t* base, uint32_t screen);
// sub_82235BD8 Type6Screen0::Type6Screen0: ScreenMachine on the screen.
void Type6Screen0(uint8_t* base, uint32_t screen);
// sub_82245FF8 Type7Screen0::Type7Screen0: ScreenMachine on +4, +0 =
// vtable 0x82010B5C.
void Type7Screen0(uint8_t* base, uint32_t screen);

// --- The list screens' parts ---

// sub_82247BE8 NodeTable::NodeTable (a leaf).
void NodeTable(uint8_t* base, uint32_t table);
// sub_82247C60 PairedNodeTable::PairedNodeTable (460 bytes) and
// sub_82247D08 ColorNodeTable::ColorNodeTable (584 bytes): NodeTable, then
// their words.
void PairedNodeTable(uint8_t* base, uint32_t table);
void ColorNodeTable(uint8_t* base, uint32_t table);
// sub_82252290 ListScreen::ListScreen: ScreenMachine on +4, then its words.
void ListScreen(uint8_t* base, uint32_t screen);
// sub_822523F8 ListPartBase::ListPartBase and sub_822525E0
// ListPartInnerBase::ListPartInnerBase (leaves), sub_822526A0
// ListPartInner::ListPartInner and sub_82252908 ListPart::ListPart.
void ListPartBase(uint8_t* base, uint32_t part);
void ListPartInnerBase(uint8_t* base, uint32_t inner);
void ListPartInner(uint8_t* base, uint32_t inner);
void ListPart(uint8_t* base, uint32_t part);
// sub_822550F8 ColorListScreen::ColorListScreen: ListScreen, then a
// ColorNodeTable at +10432 (+9672 its address, its +4 = +9680 as read).
void ColorListScreen(uint8_t* base, uint32_t screen);
// sub_82242BE8 Type7ListScreen::Type7ListScreen (11,180 bytes):
// ColorListScreen, a ListPart at +11040, then its words, some through the
// node table at [+9672] (read again for each store) and 15 items' masks from
// the 28-byte records at 0x826370A0.
void Type7ListScreen(uint8_t* base, uint32_t screen);

}  // namespace screen_constructors
