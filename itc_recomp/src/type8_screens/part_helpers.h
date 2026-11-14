// The helpers the type 8 list parts share (src/type8_screens/list_part.cpp,
// color_part.cpp; defined in list_part.cpp). See type8_screens.h.

#pragma once

#include "save_data/save_data.h"
#include "type8_screens/type8_screens.h"

#include <cstdint>
#include <optional>

namespace type8_screens::part_detail {

// The frame of `bytes` below `sp` with its back chain written; its address.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes);
// UiMovies::Find with the id stored at `id_word` (the frame word the
// original stores it in); the movie.
uint32_t MovieAt(uint8_t* base, uint32_t id_word, uint32_t id);
// A call through a table or vtable entry (`target`) with r3 and, where the
// original sets them, r4 and r5: type67_screens::SlotNative where it's
// ours, else a guest call. Its r3.
uint64_t CallTarget(uint8_t* base, uint32_t target, uint64_t r3, std::optional<uint64_t> r4,
                    std::optional<uint64_t> r5, uint32_t frame, uint32_t lr, const Calls& calls);
// operator delete on `object` (r4 the flags), a guest call.
void Delete(uint64_t object, uint64_t flags, uint32_t frame, uint32_t lr, const Calls& calls);
// UiMovie::SetNodeText(movie, node, id, r6) with r7 to r10 0, a guest call.
uint64_t SetNodeText(uint32_t movie, uint32_t node, uint32_t id, uint32_t frame, uint32_t lr,
                     const Calls& calls, uint64_t r6 = 0);
// MenuList::Init with its stack ids stored at the frame's +92 and +84: its
// plain C++ where it frees natively, else a guest call.
void InitList(uint8_t* base, uint32_t list, const save_data::MenuListArgs& a, uint32_t frame,
              uint32_t lr, const Calls& calls);
// UiMovie::FinishNamed, PlayNamed(chained 1) and LoopNamed on the named
// animation at `named_word`. LoopNamed's r3.
uint64_t Restart(uint8_t* base, uint32_t movie, uint32_t named_word, uint32_t frame,
                 uint32_t lr_finish, uint32_t lr_loop, const Calls& calls);
// The inner parts' Input (128-byte frame; the step table `steps`) and
// Pick (the page table `pages`), Open's start (its page block reset and
// UiMovie::ResetPose from `frame` returning to `lr`) and Page0's end (both
// answers, `backed` player 0's backed-out word).
uint64_t InnerInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t steps, uint32_t lr_step,
                    uint32_t lr_run, uint32_t sp, const Calls& calls);
uint64_t InnerPick(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t pages,
                   uint32_t sp, uint32_t lr, const Calls& calls);
void OpenBegin(uint8_t* base, uint32_t inner, uint32_t id, uint32_t frame, uint32_t lr,
               const Calls& calls);
void Page0End(uint8_t* base, uint32_t inner, uint64_t first, uint64_t second, uint32_t backed,
              const Calls& calls);

}  // namespace type8_screens::part_detail
