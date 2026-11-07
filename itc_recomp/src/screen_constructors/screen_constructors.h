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
// sub_82252290 ListScreen::ListScreen: ScreenMachine on +4, then its words.
void ListScreen(uint8_t* base, uint32_t screen);
// sub_822523F8 ListPartBase::ListPartBase and sub_822525E0
// ListPartInnerBase::ListPartInnerBase (leaves), sub_822526A0
// ListPartInner::ListPartInner and sub_82252908 ListPart::ListPart.
void ListPartBase(uint8_t* base, uint32_t part);
void ListPartInnerBase(uint8_t* base, uint32_t inner);
void ListPartInner(uint8_t* base, uint32_t inner);
void ListPart(uint8_t* base, uint32_t part);

}  // namespace screen_constructors
