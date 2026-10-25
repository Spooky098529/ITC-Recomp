// The game's exit table (atexit's, c_runtime/exit.h) in the harnesses of
// functions that register clean-ups: the table's fill marked before a side,
// the words the side added kept and the end put back after it (the block
// kept: a grown table stays grown, its words up to the mark copied by
// realloc). Hold c_runtime::ExitMutex() around the case, so no other thread
// adds to the table meanwhile (both sides take it again: it's recursive).

#pragma once

#include "harness.h"

#include "c_runtime/exit.h"

namespace harness::exit_table {

// The table's fill (end - begin) before a side.
inline uint32_t Mark(uint8_t* base) {
  return guest::LoadU32(base, c_runtime::kExitTableEnd) -
         guest::LoadU32(base, c_runtime::kExitTableBegin);
}

// The words added since `mark` (their count, then each), and the end put
// back at the mark.
inline void Take(uint8_t* base, uint32_t mark, Outputs& out) {
  const uint32_t begin = guest::LoadU32(base, c_runtime::kExitTableBegin);
  const uint32_t end = guest::LoadU32(base, c_runtime::kExitTableEnd);
  const uint32_t added = end - begin - mark;
  out.Word(added);
  out.Bytes(Read(base, begin + mark, added <= 64 ? added : 64));
  guest::StoreU32(base, c_runtime::kExitTableEnd, begin + mark);
}

}  // namespace harness::exit_table
