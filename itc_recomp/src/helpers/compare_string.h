// XAPI's (kernel32's) string compares, statically linked into the game, as
// plain C++ over guest memory (B6's pure helpers, docs/functions/mode-tick.md;
// the strings and the character type tables are the game's, in its image).
// Native callers call these directly; they have no adapters while only native
// code calls them in play.

#pragma once

#include <cstdint>

namespace helpers {

// The character type table (a word a byte: 0x10000 the bytes the word sort
// skips first, 0x20000 the symbols, 0x100000 the upper case letters) and the
// answers for strings equal but for what was skipped.
constexpr uint32_t kCharacterTypes = 0x82638340;
constexpr uint32_t kSkippedAnswers = 0x82638740;

// sub_82297100 CompareStringA: r3 as the original leaves it (all 64 bits):
// 1 less, 2 equal, 3 greater, or 0 with SetLastError(87) (for the thread's
// `pcr`) for a null string or a length below -1. Flags 1 (upper case folded),
// 4 (symbols skipped) and 0x1000 (no word sort) as the original reads them.
uint64_t CompareStringA(uint8_t* base, uint32_t pcr, uint32_t flags, uint32_t string1,
                        int32_t length1, uint32_t string2, int32_t length2);

// sub_82296130 lstrcmpA: CompareStringA(0, a, -1, b, -1) - 2, or (where that
// fails: a null string) its own compare: a null first string -1 (0 when both
// are), a null second 1. r3 as the original leaves it.
uint64_t Lstrcmp(uint8_t* base, uint32_t pcr, uint32_t a, uint32_t b);

}  // namespace helpers
