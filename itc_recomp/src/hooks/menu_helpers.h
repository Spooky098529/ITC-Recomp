// The front end's menu helpers' register steps (src/menu_helpers/,
// docs/functions/menu-helpers.md), shared by the adapters
// (src/hooks/menu_helpers.cpp, behind --native_menu_helpers) and the harness
// (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// Menu::Step sub_82186568: r3 = the menu.
void MenuStep(PPCContext& ctx, uint8_t* base);
// Menu::Pressed sub_821864C8: r3 = the menu, r4 = the action id.
void MenuPressed(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
