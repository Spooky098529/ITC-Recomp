// Hashing for our code: XXH3-64, inlined. xxHash comes from the SDK's
// thirdparty/ until we have our own copy (A1 in docs/rexglue-removal.md).

#pragma once

#include <cstddef>
#include <cstdint>

// Inlined, as the SDK's rex/hash.h sets it up. Not XXH_X86DISPATCH: its
// first call writes the dispatch pointers unsynchronized, and we hash on
// several threads.
#ifndef XXH_INLINE_ALL
#define XXH_INLINE_ALL
#endif
#include <xxhash.h>

namespace base {

inline uint64_t Hash64(const void* data, size_t size) { return XXH3_64bits(data, size); }

inline uint64_t Hash64(const void* data, size_t size, uint64_t seed) {
  return XXH3_64bits_withSeed(data, size, seed);
}

}  // namespace base
