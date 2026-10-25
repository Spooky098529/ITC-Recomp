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

}  // namespace

void UiMovieObjects(Context& h) {
  if (!hooks::NativeHeapActive() || !ITC_FLAG(native_c_runtime_heap)) {
    ITC_LOG_INFO("harness: ui_movie_objects: needs --native_heap and --native_c_runtime_heap");
    return;
  }
  MakePairsCases(h);
  VectorCases(h);
}

}  // namespace harness
