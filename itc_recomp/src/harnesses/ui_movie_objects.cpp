// The UI movies' vectors and objects (src/ui_movies/objects.cpp, adapters in
// src/hooks/ui_movies_game.cpp) against the recompiled functions. They
// allocate and free over the allocation layer, so both sides run with
// --native_heap: the recompiled side (--native_ui_movies_game off) through
// the C runtime heap's adapters. Blocks a case gives are compared by what's
// in them, and freed after it; the cases' own blocks come from the layer.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_string/std_string.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_ui_movies_game);
ITC_FLAG_DECLARE(bool, native_c_runtime_heap);

namespace harness {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// Runs the recompiled version with --native_ui_movies_game off, so what it
// calls of ours runs the originals too (the heap's adapters stay native).
void RunRecompiled(GuestFunction recompiled, PPCContext& t, uint8_t* base) {
  const bool native = ITC_FLAG(native_ui_movies_game);
  ITC_FLAG(native_ui_movies_game) = false;
  recompiled(t, base);
  ITC_FLAG(native_ui_movies_game) = native;
}

// A pointer word: 0, the case's own block given at `given` (1), a block
// the layer gave (2, kept in `made` to free), or the word itself.
void PointerWord(Outputs& out, uint32_t word, uint32_t given, std::vector<uint32_t>& made) {
  if (word == 0) {
    out.Word(0);
  } else if (word == given) {
    out.Word(1);
  } else if (memory::IsHeapBlock(word)) {
    out.Word(2);
    out.Word(memory::Size(word));
    made.push_back(word);
  } else {
    out.Word(3);
    out.Word(word);
  }
}

// --- The vectors ---

// sub_8211B0E8: the movie table's vector (r3) made.
void MakePairsCases(Context& h) {
  Results results("sub_8211B0E8 (the movie table's vector made)");
  for (int i = 0; i < 8; i++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, 64);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data + 4 * (i % 4);
      if (v == 0) {
        RunRecompiled(__imp__sub_8211B0E8, t, h.base);
      } else {
        sub_8211B0E8(t, h.base);
      }
      std::vector<uint32_t> made;
      const uint32_t vector = h.data + 4 * (i % 4);
      PointerWord(out[v], Load(h.base, vector), 0, made);
      if (!made.empty()) {
        out[v].Bytes(Read(h.base, made[0], 512));
      }
      out[v].Bytes(Read(h.base, h.data, 64));
      out[v].Word(t.r3.u32 == vector);
      for (uint32_t block : made) {
        memory::crt::OperatorDeleteArray(block);
      }
      Store(h.base, vector, 0);
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// The pairs' and words' appends: a vector in the scratch area over an array
// the case allocates (capacity, count and items random, the count often at
// the capacity so the append grows it; capacity 0 with a count of 0 too),
// the call, then the vector's words and its array's items compared (the
// array as whether it's the case's own or a new block), r3 as the vector,
// the case's array (freed by the growth) or itself.
struct VectorFunction {
  const char* name;
  GuestFunction recompiled;
  GuestFunction native;
  uint32_t item;  // 8 for pairs, 4 for words
  bool by_key;    // sub_8211C168: the key may be there
};

void VectorCases(Context& h) {
  const VectorFunction functions[] = {
      {"sub_8211C168 (a pair set; sub_8211B458, sub_8211B160)", __imp__sub_8211C168,
       sub_8211C168, 8, true},
      {"sub_8211AE68 (a word appended; sub_8211A788)", __imp__sub_8211AE68, sub_8211AE68, 4,
       false},
  };
  for (const VectorFunction& f : functions) {
    Results results(f.name);
    for (int i = 0; i < 400; i++) {
      Random r(h.random.Next());
      const uint32_t capacity = r.Below(6) == 0 ? 0 : 1 + r.Below(12);
      const uint32_t count = capacity == 0 ? 0 : (r.Below(2) == 0 ? capacity : r.Below(capacity + 1));
      std::vector<uint32_t> items((capacity == 0 ? 1 : capacity) * f.item / 4);
      for (uint32_t& word : items) {
        word = r.Below(3) == 0 ? r.Below(6) : uint32_t(r.Next());
      }
      const uint32_t key = r.Below(2) == 0 ? r.Below(6) : uint32_t(r.Next());
      const uint32_t value = uint32_t(r.Next());
      const uint32_t vector = h.data, key_at = h.data + 64, value_at = h.data + 68;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, 128);
        const uint32_t array = memory::crt::OperatorNewArray(h.base, uint32_t(items.size()) * 4)
                                   .value_or(0);
        for (size_t k = 0; k < items.size(); k++) {
          Store(h.base, array + 4 * uint32_t(k), items[k]);
        }
        Store(h.base, vector, array);
        Store(h.base, vector + 4, capacity);
        Store(h.base, vector + 8, count);
        Store(h.base, key_at, key);
        Store(h.base, value_at, value);
        PPCContext t = CallContext(h);
        t.r3.u64 = vector;
        t.r4.u64 = f.by_key ? key_at : value_at;
        t.r5.u64 = value_at;
        if (v == 0) {
          RunRecompiled(f.recompiled, t, h.base);
        } else {
          f.native(t, h.base);
        }
        std::vector<uint32_t> made;
        const uint32_t now = Load(h.base, vector);
        PointerWord(out[v], now, array, made);
        const uint32_t new_capacity = Load(h.base, vector + 4);
        out[v].Word(new_capacity);
        out[v].Word(Load(h.base, vector + 8));
        const uint32_t words = (now == array ? uint32_t(items.size()) * 4
                                             : new_capacity * f.item);
        out[v].Bytes(Read(h.base, now, words));
        out[v].Bytes(Read(h.base, h.data + 12, 116));
        out[v].Word(t.r3.u32 == vector ? 0 : t.r3.u32 == array ? 1 : t.r3.u32);
        memory::crt::OperatorDeleteArray(now);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: capacity {}, count {}, key {:X}", i, capacity, count, key));
    }
    results.Report();
  }
}

// --- The movie's constructor, destructor and texts ---

constexpr uint32_t kMovieBytes = 0xA010;  // the 40,976 bytes UiMovies::Open allocates

// The movie's bytes with the words at `masked` offsets replaced by 0 (the
// pointers, compared by what they point to).
std::vector<uint8_t> MovieBytes(uint8_t* base, uint32_t movie,
                                const std::vector<uint32_t>& masked) {
  std::vector<uint8_t> bytes = Read(base, movie, kMovieBytes);
  for (uint32_t offset : masked) {
    for (uint32_t k = 0; k < 4; k++) {
      bytes[offset + k] = 0;
    }
  }
  return bytes;
}

void RandomFill(Random& r, uint8_t* base, uint32_t at, uint32_t bytes) {
  for (uint32_t k = 0; k + 4 <= bytes; k += 4) {
    Store(base, at + k, uint32_t(r.Next()));
  }
}

// UiMovie::UiMovie over a movie of random bytes: the movie (its textures'
// and items' pointers compared by their blocks), the items' block's 65,552
// bytes and the textures' block's size; both freed after.
void ConstructCases(Context& h, uint32_t movie) {
  Results results("UiMovie::UiMovie (sub_8211E1C0; sub_8211AE18, sub_8211B018, sub_8211D210)");
  for (int i = 0; i < 4; i++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomFill(r, h.base, movie, kMovieBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = movie;
      if (v == 0) {
        RunRecompiled(__imp__sub_8211E1C0, t, h.base);
      } else {
        sub_8211E1C0(t, h.base);
      }
      std::vector<uint32_t> made;
      const uint32_t textures = Load(h.base, movie + 28);
      const uint32_t items = Load(h.base, movie + 40);
      PointerWord(out[v], textures, 0, made);
      PointerWord(out[v], items == 0 ? 0 : items - 16, 0, made);
      out[v].Bytes(MovieBytes(h.base, movie, {28, 40}));
      if (items != 0 && memory::IsHeapBlock(items - 16)) {
        out[v].Bytes(Read(h.base, items - 16, 65552));
      }
      out[v].Word(t.r3.u32 == movie);
      for (uint32_t block : made) {
        memory::crt::OperatorDeleteArray(block);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// UiMovie::~UiMovie over a movie built natively, with random named
// animations' strings on the heap (some with a null block), a text list of
// its own (none, or 0 to 4 items) and its textures: the movie (the strings'
// blocks' pointers masked, as _Tidy leaves their last three bytes), which
// blocks were freed, r3.
void DestroyCases(Context& h, uint32_t movie) {
  Results results("UiMovie::~UiMovie (sub_8211E160; sub_8211D1B8, sub_8211AF90)");
  for (int i = 0; i < 40; i++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      Fill(h.base, movie, kMovieBytes);
      ui_movies::ConstructMovie(h.base, movie);
      std::vector<uint32_t> blocks;  // the case's, to see which were freed
      std::vector<uint32_t> masked = {28, 40};
      const uint32_t strings = r.Below(30);
      std::vector<uint32_t> entries;  // the strings on the heap
      for (uint32_t k = 0; k < strings; k++) {
        const uint32_t entry = movie + 176 + 36 * r.Below(1024);
        const uint32_t capacity = 16 + r.Below(40);
        const bool null_block = r.Below(8) == 0;
        const uint32_t length = r.Below(capacity + 1);
        if (Load(h.base, entry + 24) >= 16) {
          continue;  // already one of the case's
        }
        const uint32_t block =
            null_block ? 0 : memory::crt::OperatorNew(h.base, capacity + 1).value_or(0);
        if (block != 0) {
          RandomFill(r, h.base, block, capacity + 1);
          blocks.push_back(block);
        }
        Store(h.base, entry + 4, block);
        Store(h.base, entry + 20, length);
        Store(h.base, entry + 24, capacity);
        masked.push_back(entry - movie + 4);
        entries.push_back(entry);
      }
      const uint32_t items = Load(h.base, movie + 40);
      const uint32_t list = r.Below(4);
      if (list != 0) {
        memory::crt::OperatorDeleteArray(items - 16);
        if (list == 1) {
          Store(h.base, movie + 40, 0);
        } else {
          const uint32_t n = r.Below(5);
          const uint32_t block = memory::crt::OperatorNewArray(h.base, 16 + 128 * n).value_or(0);
          RandomFill(r, h.base, block, 16 + 128 * n);
          Store(h.base, block, n);
          Store(h.base, movie + 40, block + 16);
          blocks.push_back(block);
        }
      } else {
        blocks.push_back(items - 16);
      }
      const uint32_t textures = Load(h.base, movie + 28);
      blocks.push_back(textures);
      PPCContext t = CallContext(h);
      t.r3.u64 = movie;
      if (v == 0) {
        RunRecompiled(__imp__sub_8211E160, t, h.base);
      } else {
        sub_8211E160(t, h.base);
      }
      out[v].Word(uint32_t(blocks.size()));
      for (uint32_t block : blocks) {
        out[v].Word(memory::IsHeapBlock(block));
      }
      std::vector<uint8_t> bytes = MovieBytes(h.base, movie, masked);
      for (uint32_t entry : entries) {
        out[v].Word(guest::LoadU8(h.base, entry + 4));
      }
      out[v].Word(Load(h.base, movie + 40) == 0);
      out[v].Bytes(bytes);
      out[v].Word(t.r3.u32 == textures ? 1 : t.r3.u32);
      for (uint32_t block : blocks) {
        if (memory::IsHeapBlock(block)) {
          memory::crt::OperatorDelete(block);
        }
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// sub_8211AC90 over a movie whose texts' map holds 0 to 6 records (each 28
// bytes from operator new over a 4 KB buffer from VirtualAlloc's pages):
// the map and the count, which records and buffers were freed, r3.
void FreeTextsCases(Context& h, uint32_t movie) {
  Results results("sub_8211AC90 (a movie's texts freed)");
  for (int i = 0; i < 40; i++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      RandomFill(r, h.base, movie, kMovieBytes);
      const uint32_t count = r.Below(7);
      std::vector<uint32_t> records, buffers;
      for (uint32_t k = 0; k < count; k++) {
        const uint32_t record = memory::crt::OperatorNew(h.base, 28).value_or(0);
        const uint32_t buffer = memory::AllocPages(4096, 4096);
        RandomFill(r, h.base, record, 28);
        Store(h.base, record, buffer);
        Store(h.base, movie + 0x9BCC + 8 * k + 4, record);
        records.push_back(record);
        buffers.push_back(buffer);
      }
      Store(h.base, movie + 0x9FCC, count);
      PPCContext t = CallContext(h);
      t.r3.u64 = movie;
      if (v == 0) {
        RunRecompiled(__imp__sub_8211AC90, t, h.base);
      } else {
        sub_8211AC90(t, h.base);
      }
      std::vector<uint32_t> masked;
      for (uint32_t k = 0; k < count; k++) {
        out[v].Word(memory::IsHeapBlock(records[k]));
        out[v].Word(memory::IsPages(buffers[k]));
        masked.push_back(0x9BCC + 8 * k + 4);
      }
      out[v].Bytes(MovieBytes(h.base, movie, masked));
      uint32_t r3 = t.r3.u32 == movie ? 0 : 0xFFFFFFFFu;
      for (uint32_t k = 0; k < count; k++) {
        if (t.r3.u32 == records[k]) {
          r3 = k + 1;
        }
      }
      out[v].Word(r3);
      for (uint32_t k = 0; k < count; k++) {
        if (memory::IsHeapBlock(records[k])) {
          memory::crt::OperatorDelete(records[k]);
        }
        if (memory::IsPages(buffers[k])) {
          memory::FreePages(buffers[k]);
        }
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

}  // namespace

void UiMovieObjects(Context& h) {
  if (!hooks::NativeHeapActive() || !ITC_FLAG(native_c_runtime_heap)) {
    ITC_LOG_INFO("harness: ui_movie_objects: needs --native_heap and --native_c_runtime_heap");
    return;
  }
  MakePairsCases(h);
  VectorCases(h);
  const uint32_t movie = memory::Alloc(kMovieBytes, 0);
  ConstructCases(h, movie);
  DestroyCases(h, movie);
  FreeTextsCases(h, movie);
  memory::Free(movie);
}

}  // namespace harness
