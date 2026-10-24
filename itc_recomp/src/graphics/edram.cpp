// The graphics library's EDRAM allocator (docs/functions/graphics-resources.md):
// see edram.h.

#include "graphics/edram.h"

#include "guest.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"

#include <atomic>
#include <mutex>

namespace graphics::edram {

namespace {

// XMemAlloc's attributes for the object (cleared), also used for its free.
constexpr uint32_t kObjectAttributes = 0x64800000;

// In place of the object's critical section.
std::mutex g_mutex;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Clz(uint32_t value) { return value == 0 ? 32 : uint32_t(__builtin_clz(value)); }

// The bits from `shift` on (most significant first): 0xFFFFFFFF >> shift.
uint32_t From(uint32_t shift) { return (shift & 0x20) ? 0 : 0xFFFFFFFFu >> (shift & 0x1F); }

// The allocator, made on first use (nullopt if XMemAlloc fails).
std::optional<uint32_t> Allocator(uint8_t* base) {
  uint32_t allocator = Load(base, kAllocator);
  if (allocator != 0) {
    return allocator;
  }
  allocator = memory::crt::XMemAlloc(base, kObjectBytes, kObjectAttributes);
  if (allocator == 0) {
    return std::nullopt;
  }
  Store(base, allocator + kFree, kTiles);
  Store(base, allocator + kBitmapEnd, kSentinel);
  // The original's lwarx/stwcx: set the pointer if it's still 0.
  uint32_t* word = reinterpret_cast<uint32_t*>(REX_RAW_ADDR(kAllocator));
  uint32_t expected = 0;
  if (!__atomic_compare_exchange_n(word, &expected, __builtin_bswap32(allocator), false,
                                   __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
    // Another thread made one first: this one goes.
    memory::crt::XMemFree(allocator, kObjectAttributes);
    allocator = Load(base, kAllocator);
  }
  return allocator;
}

}  // namespace

uint32_t NextRun(uint8_t* base, uint32_t allocator, uint32_t& position) {
  ITC_NATIVE_RUNNING("sub_822B5C90 Edram::NextRun");
  const uint32_t bitmap = allocator + kBitmap;
  const uint32_t end = allocator + kBitmapEnd;
  uint32_t word = ((position >> 3) & 0x1FFFFFFC) + bitmap;
  if (word == end) {
    return 0;
  }
  uint32_t bits = Load(base, word);
  // The first free tile from the position on.
  uint32_t start = Clz(From(position & 0x1F) & ~bits);
  if (start == 32) {
    do {
      word += 4;
      bits = Load(base, word);
    } while (bits == 0xFFFFFFFF);
    if (word == end) {
      return 0;
    }
    start = Clz(~bits);
  }
  // The first used tile after it.
  uint32_t end_word = word;
  uint32_t stop = Clz(From(start) & bits);
  if (stop == 32) {
    end_word = word + 4;
    while (Load(base, end_word) == 0) {
      end_word += 4;
    }
    stop = end_word == end ? 0 : Clz(Load(base, end_word));
  }
  const uint32_t first = (uint32_t(int32_t(word - bitmap) >> 2) << 5) + start;
  const uint32_t last = (uint32_t(int32_t(end_word - bitmap) >> 2) << 5) + stop;
  position = first;
  return last - first;
}

void Mark(uint8_t* base, uint32_t allocator, uint32_t start, uint32_t end, bool used) {
  ITC_NATIVE_RUNNING("sub_822B5D70 Edram::Mark");
  const uint32_t start_index = start >> 5;
  const uint32_t end_index = end >> 5;
  uint32_t word = ((start_index + 8) << 2) + allocator;
  const uint32_t end_word = ((end_index + 8) << 2) + allocator;
  const uint32_t start_bit = start & 0x1F;
  const uint32_t end_bit = end & 0x1F;
  if (start_index == end_index) {
    const uint32_t bits = From(start_bit) & ~From(end_bit);
    const uint32_t old = Load(base, word);
    Store(base, word, used ? (bits | old) : (old & ~bits));
    return;
  }
  const uint32_t first = Load(base, word);
  Store(base, word, used ? (From(start_bit) | first) : (first & ~From(start_bit)));
  word += 4;
  while (word < end_word) {
    Store(base, word, used ? 0xFFFFFFFF : 0);
    word += 4;
  }
  const uint32_t last = Load(base, word);
  Store(base, word, used ? (last | ~From(end_bit)) : (From(end_bit) & last));
}

std::optional<Allocation> Alloc(uint8_t* base, uint32_t tiles) {
  ITC_NATIVE_RUNNING("sub_822B5EF8 Edram::Alloc");
  const std::optional<uint32_t> made = Allocator(base);
  if (!made) {
    return std::nullopt;
  }
  const uint32_t allocator = *made;
  uint32_t found = 0xFFFFFFFF;
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (tiles <= Load(base, allocator + kFree)) {
      // The smallest free run that fits (the first of them).
      uint32_t best_start = 0;
      uint32_t best_length = 0xFFFFFFFF;
      uint32_t position = 0;
      uint32_t length = NextRun(base, allocator, position);
      while (length != 0) {
        if (length >= tiles && length < best_length) {
          best_start = position;
          best_length = length;
        }
        position += length;
        length = NextRun(base, allocator, position);
      }
      if (best_length != 0xFFFFFFFF) {
        Store(base, allocator + kFree, Load(base, allocator + kFree) - tiles);
        Mark(base, allocator, best_start, best_start + tiles, true);
        found = best_start;
      }
    }
  }
  return Allocation{found, found != 0xFFFFFFFF};
}

void Free(uint8_t* base, uint32_t start, uint32_t tiles) {
  ITC_NATIVE_RUNNING("sub_822B5EA8 Edram::Free");
  const uint32_t allocator = Load(base, kAllocator);
  std::lock_guard<std::mutex> lock(g_mutex);
  Store(base, allocator + kFree, tiles + Load(base, allocator + kFree));
  Mark(base, allocator, start, start + tiles, false);
}

}  // namespace graphics::edram
