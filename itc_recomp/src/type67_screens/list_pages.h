// The list screens' page and step helpers (defined in titled_list.cpp, used
// by the titled list and the color list): the frames, the movie, the menu
// sound, the page block's moves, the screen fades' fade-out and the tables of
// member functions Draw and Update build on first use. See type67_screens.h.

#pragma once

#include "type67_screens/type67_screens.h"

#include <cstdint>
#include <optional>

namespace type67_screens::pages {

// The frame of `bytes` below `sp` with its back chain written; its address.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes);
// The screen's movie: [+9680] at the frame's +80, then UiMovies::Find.
uint32_t MovieOf(uint8_t* base, uint32_t screen, uint32_t frame);
// MenuSound::Play(cue)'s plain C++ from `frame`; its r3.
uint64_t PlaySound(uint8_t* base, uint32_t cue, uint32_t frame, const Calls& calls);
// A guest call with r3 only (r4 and the others as the last call left them).
uint64_t Call1(const Calls& calls, uint32_t function, uint64_t r3, uint32_t frame, uint32_t lr);
// UiMovie::PlayNamed(movie, named, chained, 0, 0, 0). Its r3.
uint64_t Play(const Calls& calls, uint32_t movie, uint32_t named, uint32_t chained,
              uint32_t frame, uint32_t lr);
// The page block (+9644 the page, +9648 the page after, +9652 the page
// before, +9656 the step, +9660 the step queued): "page P" and "page P after
// step S" (+9652 = the page now when it isn't P).
void ToPage(uint8_t* base, uint32_t screen, uint32_t page);
void ToStepThenPage(uint8_t* base, uint32_t screen, uint32_t page, uint32_t step);
// ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0): its plain C++, r4 left 0;
// the passes (FadeOut's r3).
uint64_t FadeOut(uint8_t* base, const Calls& calls);
// A table entry (`target`, the this offset already added to r3) called from
// `frame` returning to `lr` with r4: the plain C++ where it's one of ours,
// else a guest call with r3, r4 and, when given, r5.
uint64_t CallEntry(uint8_t* base, uint32_t target, uint64_t r3, uint64_t r4,
                   std::optional<uint64_t> r5, uint32_t frame, uint32_t lr, const Calls& calls);
// A table of `count` member functions built on first use (the guard's bit 0
// clear): the (function, 0) pairs written to the frame's +80 on, the guard's
// bit 0 set, the pairs copied into the table. Whether it was built.
bool BuildTable(uint8_t* base, uint32_t frame, uint32_t guard, uint32_t table,
                const uint32_t* entries, uint32_t count);

}  // namespace type67_screens::pages
