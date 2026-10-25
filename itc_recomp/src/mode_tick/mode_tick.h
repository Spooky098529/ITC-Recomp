// What the mode manager's tick (Modes::Tick, sub_82107910) runs besides the
// current mode's slots, as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/mode-tick.md, the globals in
// docs/known-data.md): the per-mode settings, the sign-in watch, the
// billboards' ring, the screen fades and the menu music. Their globals are
// shared with the recompiled code, so these work on guest memory, with the
// originals' loads and stores in their order. Adapters: src/hooks/mode_tick.cpp.

#pragma once

#include <cstdint>

namespace mode_tick {

// --- The billboards' ring ---

// The ring half (0 or 1) and the vertex count Billboards::BuildVertices
// writes (docs/functions/sprites.md).
constexpr uint32_t kRingHalf = 0x826A5098;
constexpr uint32_t kRingCount = 0x826A509C;

// sub_8209BD18 Billboards::FlipRing: the other half, its count 0.
void BillboardsFlipRing(uint8_t* base);

// --- The screen fades (two layers) ---

constexpr uint32_t kFadeOutDurations = 0x826A50AC;
constexpr uint32_t kFadeInDurations = 0x826A50B4;
constexpr uint32_t kFadeColors = 0x826A50BC;  // three words a layer
constexpr uint32_t kFadeAlphas = 0x826A50D4;
constexpr uint32_t kFadeInCounts = 0x826A50DC;
constexpr uint32_t kFadeOutCounts = 0x826A50E4;

// sub_8209CDE8 ScreenFades::Init: both layers' words 0.
void ScreenFadesInit(uint8_t* base);
// sub_8209D038 ScreenFades::FadeIn: `layer` fades in from the color over
// `passes`; the other layer's (layer 0's for any layer but 0) counts and
// alpha 0.
void ScreenFadesFadeIn(uint8_t* base, uint32_t passes, uint32_t red, uint32_t green,
                       uint32_t blue, uint32_t layer);
// sub_8209D0C8 ScreenFades::FadeOut: `layer` fades out to the color over
// `passes`.
void ScreenFadesFadeOut(uint8_t* base, uint32_t passes, uint32_t red, uint32_t green,
                        uint32_t blue, uint32_t layer);

// --- The sign-in watch and the players ---

// Player n's controller's user (four words), the signed-in users' mask
// (written by Network::Update, docs/functions/system-services.md).
constexpr uint32_t kPlayerUsers = 0x826BABA4;
constexpr uint32_t kSignedInMask = 0x826C7954;

// sub_8209F658 Players::SignedInSlot: the player's user's index among the
// signed-in users, or -1.
int32_t PlayersSignedInSlot(uint8_t* base, uint32_t player);

// The action mappers' entries (8 bytes each, four; action-mappers.md).
constexpr uint32_t kActionMapperEntries = 0x826BAB80;

// sub_8209F318 ActionMappers::SetBlocked: each entry's +4 = value.
void ActionMappersSetBlocked(uint8_t* base, uint32_t value);

}  // namespace mode_tick
