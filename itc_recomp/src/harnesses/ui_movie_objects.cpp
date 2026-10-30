// The UI movies' vectors and objects (src/ui_movies/objects.cpp, adapters in
// src/hooks/ui_movies_game.cpp) against the recompiled functions. They
// allocate and free over the allocation layer, so both sides run with
// --native_heap: the recompiled side (--native_ui_movies_game off) through
// the C runtime heap's adapters. Blocks a case gives are compared by what's
// in them, and freed after it; the cases' own blocks come from the layer.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/archive.h"
#include "hooks/heap.h"
#include "hooks/ui_movies_game.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_string/std_string.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cstring>
#include <iterator>
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

// UiMovie::GetText over a movie whose texts' map holds 0 to 5 ids (1 to 6),
// each with a record or 0, the node's id 0 or 1 to 7, the text count 0 to
// 6, and the word at 0x826C5CD8 (read for an id not there) a record or 0
// (put back after): the id, the movie, r3 (a record of the case's, or a new
// one: its words, its buffer's pages and bytes); new ones freed.
void GetTextCases(Context& h, uint32_t movie) {
  Results results("UiMovie::GetText (sub_8211B660)");
  constexpr uint32_t kMissing = 0x826C5CD8;
  const uint32_t missing = Load(h.base, kMissing);
  const uint32_t id_at = h.data, records = h.data + 64;  // 8 records of 28 bytes
  for (int i = 0; i < 200; i++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      Fill(h.base, h.data, 512);
      RandomFill(r, h.base, movie + 0x9BC0, 0x420);
      RandomFill(r, h.base, records, 8 * 28);
      const uint32_t count = r.Below(6);
      for (uint32_t k = 0; k < count; k++) {
        Store(h.base, movie + 0x9BCC + 8 * k, 1 + r.Below(6));
        Store(h.base, movie + 0x9BCC + 8 * k + 4, r.Below(3) == 0 ? 0 : records + 28 * k);
      }
      Store(h.base, movie + 0x9BCC + 1024, count);
      Store(h.base, movie + 0x9FCC, r.Below(7));
      Store(h.base, id_at, r.Below(3) == 0 ? 0 : 1 + r.Below(7));
      Store(h.base, kMissing, r.Below(2) == 0 ? 0 : records + 28 * 7);
      PPCContext t = CallContext(h);
      t.r3.u64 = movie;
      t.r4.u64 = id_at;
      if (v == 0) {
        RunRecompiled(__imp__sub_8211B660, t, h.base);
      } else {
        sub_8211B660(t, h.base);
      }
      Store(h.base, kMissing, missing);
      out[v].Bytes(Read(h.base, h.data, 512));
      out[v].Bytes(Read(h.base, movie + 0x9BC0, 0x420));
      const uint32_t record = t.r3.u32;
      if (record >= records && record < records + 8 * 28) {
        out[v].Word(1);
        out[v].Word(record - records);
      } else if (memory::IsHeapBlock(record)) {
        const uint32_t buffer = Load(h.base, record);
        out[v].Word(2);
        out[v].Word(memory::Size(record));
        out[v].Word(memory::IsPages(buffer));
        out[v].Bytes(Read(h.base, record + 4, 24));
        out[v].Bytes(Read(h.base, buffer, 4096));
        memory::FreePages(buffer);
        memory::crt::OperatorDelete(record);
      } else {
        out[v].Word(3);
        out[v].Word(record);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// sub_8211E378 over a made-up scene tree (in its own block): a scene with
// 0 to 4 children (some SCE scenes with children and animations of their
// own, two levels down) and 0 to 4 animations, names of 0 to 23 characters
// at the scenes' +84 and 0 to 40 at the animations' +16 (so the names go
// on the heap or stay in place), over a movie built natively whose entries
// in use (0 to 5) may hold heap strings: the entries (their text, length,
// capacity, +28 and +32; in-place ones byte for byte), the count, r3 (the
// movie, a node, the name in the frame, or else a freed block).
struct Tree {
  uint32_t at;
  uint32_t next;
  uint32_t Take(uint32_t bytes) {
    const uint32_t taken = next;
    next += (bytes + 15) & ~15u;
    return taken;
  }
};

void WriteName(Random& r, uint8_t* base, uint32_t at, uint32_t max) {
  const uint32_t n = r.Below(4) == 0 ? max : r.Below(max + 1);
  for (uint32_t k = 0; k < n; k++) {
    guest::StoreU8(base, at + k, uint8_t('a' + r.Below(26)));
  }
  guest::StoreU8(base, at + n, 0);
}

uint32_t MakeScene(Random& r, uint8_t* base, Tree& tree, int depth) {
  constexpr uint32_t kSce = 0x53434500;
  const uint32_t children = r.Below(5), animations = r.Below(5);
  const uint32_t scene = tree.Take(128 + 8 * (children + animations));
  RandomFill(r, base, scene, 128);
  Store(base, scene, kSce);
  WriteName(r, base, scene + 84, 23);
  Store(base, scene + 108, 0);
  Store(base, scene + 112, children);
  Store(base, scene + 116, children);
  Store(base, scene + 120, animations);
  for (uint32_t k = 0; k < children; k++) {
    uint32_t child;
    if (depth < 2 && r.Below(3) == 0) {
      child = MakeScene(r, base, tree, depth + 1);
    } else {
      child = tree.Take(16);
      Store(base, child, r.Below(4) == 0 ? kSce - 1 : uint32_t(r.Next()));
    }
    Store(base, scene + 128 + 8 * k, child);
  }
  for (uint32_t k = 0; k < animations; k++) {
    const uint32_t animation = tree.Take(64);
    RandomFill(r, base, animation, 16);
    WriteName(r, base, animation + 16, 40);
    Store(base, scene + 128 + 8 * (children + k), animation);
  }
  return scene;
}

void NamedEntryOutputs(uint8_t* base, uint32_t entry, Outputs& out) {
  if (Load(base, entry + 24) < 16) {
    out.Bytes(Read(base, entry, 36));
    return;
  }
  const uint32_t length = Load(base, entry + 20);
  out.Bytes(Read(base, entry, 4));
  out.Bytes(Read(base, Load(base, entry + 4), length + 1));
  out.Bytes(Read(base, entry + 20, 16));
}

void AddNamedAnimationsCases(Context& h, uint32_t movie) {
  Results results("sub_8211E378 (a scene's named animations added; sub_8211E318)");
  const uint32_t tree_bytes = 0x4000;
  const uint32_t tree_at = memory::Alloc(tree_bytes, 0);
  for (int i = 0; i < 100; i++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      Fill(h.base, movie, kMovieBytes);
      ui_movies::ConstructMovie(h.base, movie);
      memory::crt::OperatorDeleteArray(Load(h.base, movie + 40) - 16);
      memory::crt::OperatorDeleteArray(Load(h.base, movie + 28));
      const uint32_t used = r.Below(6);
      for (uint32_t k = 0; k < used + 8; k++) {
        if (r.Below(3) == 0) {
          const uint32_t entry = movie + 176 + 36 * k;
          const uint32_t capacity = 16 + r.Below(40);
          const uint32_t block = memory::crt::OperatorNew(h.base, capacity + 1).value_or(0);
          WriteName(r, h.base, block, capacity);
          Store(h.base, entry + 4, block);
          Store(h.base, entry + 20, r.Below(capacity + 1));
          Store(h.base, entry + 24, capacity);
        }
      }
      Store(h.base, movie + 176 + 0x9000, used);
      Fill(h.base, tree_at, tree_bytes);
      Tree tree{tree_at, tree_at};
      const uint32_t scene = MakeScene(r, h.base, tree, 0);
      PPCContext t = CallContext(h);
      const uint32_t name = t.r1.u32 - 240 + 96;
      t.r3.u64 = movie;
      t.r4.u64 = scene;
      if (v == 0) {
        RunRecompiled(__imp__sub_8211E378, t, h.base);
      } else {
        sub_8211E378(t, h.base);
      }
      const uint32_t after = Load(h.base, movie + 176 + 0x9000);
      out[v].Word(after);
      for (uint32_t k = 0; k < std::min(after, 1024u) + 8; k++) {
        NamedEntryOutputs(h.base, movie + 176 + 36 * k, out[v]);
      }
      out[v].Bytes(Read(h.base, tree_at, tree_bytes));
      const uint32_t r3 = t.r3.u32;
      out[v].Word(r3 == movie            ? 0
                  : r3 - tree_at < tree_bytes ? 1 + (r3 - tree_at)
                  : r3 == name             ? 0x10000
                                           : 0x20000);
      for (uint32_t k = 0; k < std::min(after, 1024u) + 8; k++) {
        std_string::Tidy(h.base, movie + 176 + 36 * k, true, 0);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  memory::Free(tree_at);
  results.Report();
}

// --- sub_8211BEE8 (the full-frame effect) ---
//
// sub_8259EBE8 (the quad handed to the recorders) stood in for while the
// cases run: the call's registers (r3 and r1 against the caller's stack)
// and the frame's first 1,152 bytes (the identity, the quad, the level
// description, the matrix and the arrays; the saved registers above) kept.
// CameraPasses::RenderTarget(0) and Texture::GetLevelDesc are the game's
// (they read the render target's texture).
struct EffectCall {
  bool called = false;
  uint32_t r3, r4, r1, lr;
  uint64_t f1;
  std::vector<uint8_t> frame;
};
EffectCall* g_effect_call = nullptr;
uint32_t g_effect_stack = 0;

bool EffectStandIn(uint32_t function, PPCContext& ctx, uint8_t* base) {
  if (function != 0x8259EBE8 || g_effect_call == nullptr) {
    return false;
  }
  EffectCall& call = *g_effect_call;
  call.called = true;
  call.r3 = g_effect_stack - ctx.r3.u32;
  call.r4 = ctx.r4.u32;
  call.r1 = g_effect_stack - ctx.r1.u32;
  call.lr = uint32_t(ctx.lr);
  call.f1 = ctx.f1.u64;
  call.frame = Read(base, ctx.r1.u32, 1152);
  return true;
}

void EffectCases(Context& h) {
  Results results("sub_8211BEE8 (a full-frame effect drawn)");
  constexpr uint32_t kOdd[] = {0x7FC00000, 0xFF800001, 0x7F800000, 0xFF800000,
                               0x00000001, 0x80000000, 0x7F7FFFFF};
  ui_movies_hooks::g_update_callee_stand_in = EffectStandIn;
  const uint32_t primitive = h.data;
  for (int i = 0; i < 300; i++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      auto value = [&](float lo, float hi) {
        if (r.Below(30) == 0) {
          return kOdd[r.Below(std::size(kOdd))];
        }
        const float f = lo + (hi - lo) * float(r.Below(1 << 20)) / float(1 << 20);
        uint32_t bits;
        std::memcpy(&bits, &f, 4);
        return bits;
      };
      Fill(h.base, h.data, kScratchBytes);
      RandomFill(r, h.base, primitive, 128);
      const uint32_t entries = r.Below(3);
      const uint32_t count = r.Below(5) == 0 ? 0 : 1 + r.Below(16);
      Store(h.base, primitive + 112, entries);
      Store(h.base, primitive + 116, count);
      Store(h.base, primitive + 12, value(-640, 640));
      Store(h.base, primitive + 16, value(-360, 360));
      Store(h.base, primitive + 68, value(0, 1));
      for (uint32_t k = 0; k < count; k++) {
        const uint32_t vertex = primitive + 128 + 16 * entries + 32 * k;
        RandomFill(r, h.base, vertex, 32);
        Store(h.base, vertex + 0, value(-1280, 1280));
        Store(h.base, vertex + 4, value(-720, 720));
        Store(h.base, vertex + 24, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2048));
        Store(h.base, vertex + 28, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2048));
      }
      PPCContext t = CallContext(h);
      Fill(h.base, t.r1.u32 - 1600, 1600);
      t.r3.u64 = primitive;
      t.lr = 0x822128F0;
      EffectCall call;
      g_effect_call = &call;
      g_effect_stack = t.r1.u32;
      if (v == 0) {
        RunRecompiled(__imp__sub_8211BEE8, t, h.base);
      } else {
        sub_8211BEE8(t, h.base);
      }
      g_effect_call = nullptr;
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(call.called);
      if (call.called) {
        out[v].Word(call.r3);
        out[v].Word(call.r4);
        out[v].Word(call.r1);
        out[v].Word(call.lr);
        out[v].Double(call.f1);
        // The positions (+384) and coordinates (+896) as floats.
        std::vector<uint8_t>& f = call.frame;
        out[v].Bytes({f.begin(), f.begin() + 384});
        for (uint32_t at = 384; at < 1152; at += 4) {
          const uint32_t word = uint32_t(f[at]) << 24 | uint32_t(f[at + 1]) << 16 |
                                uint32_t(f[at + 2]) << 8 | f[at + 3];
          const bool is_float = (at < 640 || at >= 896) && (at % 16) < 8 + 8 * (at < 640);
          if (is_float) {
            out[v].Float(word);
          } else {
            out[v].Word(word);
          }
        }
      }
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.lr));
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  ui_movies_hooks::g_update_callee_stand_in = nullptr;
  results.Report();
}

// --- The text setters ---
//
// A movie whose live scene (+16) has a sub-scene (entry 0) and nodes
// (entries 1 to 4; the sub-scene's 0 to 3), each node's id (+108) 0 or 1
// to 6, and whose texts' map holds 0 to 5 ids with records of the case's
// (each over a 4 KB buffer of its own: UTF-16 with page breaks, ended by
// 0) or 0; the text count 0 to 6; the word at 0x826C5CD8 (an id not there)
// a record of the case's or 0, put back after. Compared: the scratch area
// (the tree and the records), the buffers, the map (records of the case's
// by offset, new ones by their words and buffer's bytes; new ones freed),
// r3 and f1.
struct SetterFunction {
  const char* name;
  GuestFunction recompiled;
  GuestFunction native;
};

void SetterCases(Context& h, uint32_t movie) {
  const SetterFunction functions[] = {
      {"UiMovie::ClearText (sub_8211C470)", __imp__sub_8211C470, sub_8211C470},
      {"sub_8211C4F0 (a text's scroll delay and speed)", __imp__sub_8211C4F0, sub_8211C4F0},
      {"UiMovie::ScrollText (sub_8211C570)", __imp__sub_8211C570, sub_8211C570},
      {"sub_8211C5F8 (a text's scroll counter reset)", __imp__sub_8211C5F8, sub_8211C5F8},
      {"UiMovie::RevealText (sub_8211D880; sub_8211C930)", __imp__sub_8211D880, sub_8211D880},
      {"sub_8211D8D8 (whether a text has another page; sub_8211C9C0)", __imp__sub_8211D8D8,
       sub_8211D8D8},
  };
  constexpr uint32_t kMissing = 0x826C5CD8;
  constexpr uint32_t kOdd[] = {0x7FC00000, 0xFF800001, 0x7F800000, 0x80000000, 0x00000001};
  const uint32_t missing = Load(h.base, kMissing);
  const uint32_t scene = h.data, sub = h.data + 256, nodes = h.data + 512;  // 8 nodes of 128
  const uint32_t records = h.data + 1536;                                   // 8 of 32
  const uint32_t buffers = memory::Alloc(8 * 4096, 0);
  for (const SetterFunction& f : functions) {
    Results results(f.name);
    for (int i = 0; i < 200; i++) {
      const uint64_t seed = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        Fill(h.base, h.data, kScratchBytes);
        RandomFill(r, h.base, movie, 64);
        RandomFill(r, h.base, movie + 0x9BC0, 0x420);
        Store(h.base, movie + 16, scene);
        Store(h.base, scene + 108, 0);
        Store(h.base, scene + 128, sub);
        Store(h.base, sub + 108, 0);
        for (uint32_t k = 0; k < 8; k++) {
          const uint32_t node = nodes + 128 * k;
          RandomFill(r, h.base, node, 128);
          Store(h.base, node + 108, r.Below(3) == 0 ? 0 : 1 + r.Below(6));
          Store(h.base, (k < 4 ? scene + 136 : sub + 128) + 8 * (k % 4), node);
        }
        for (uint32_t k = 0; k < 8; k++) {
          const uint32_t record = records + 32 * k, buffer = buffers + 4096 * k;
          RandomFill(r, h.base, record, 32);
          Store(h.base, record, buffer);
          Store(h.base, record + 20, r.Below(5) - 1);
          const uint32_t length = r.Below(40);
          for (uint32_t c = 0; c < length; c++) {
            guest::StoreU16(h.base, buffer + 2 * c,
                            r.Below(5) == 0 ? 0xFFD1 : uint16_t(0x20 + r.Below(0x60)));
          }
          guest::StoreU16(h.base, buffer + 2 * length, 0);
          for (uint32_t c = length + 1; c < 64; c++) {
            guest::StoreU16(h.base, buffer + 2 * c, uint16_t(r.Next()));
          }
        }
        const uint32_t count = r.Below(6);
        for (uint32_t k = 0; k < count; k++) {
          Store(h.base, movie + 0x9BCC + 8 * k, 1 + r.Below(6));
          Store(h.base, movie + 0x9BCC + 8 * k + 4, r.Below(4) == 0 ? 0 : records + 32 * k);
        }
        Store(h.base, movie + 0x9BCC + 1024, count);
        Store(h.base, movie + 0x9FCC, r.Below(7));
        Store(h.base, kMissing, r.Below(2) == 0 ? 0 : records + 32 * 7);
        PPCContext t = CallContext(h);
        t.r3.u64 = movie;
        t.r4.u64 = r.Below(2) == 0 ? 0xFFFFFFFFu : 0;
        t.r5.u64 = r.Below(4);
        t.r6.u64 = r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(4);
        t.f1.f64 = r.Below(5) == 0 ? double(std::bit_cast<float>(kOdd[r.Below(std::size(kOdd))]))
                                   : double(r.Below(1 << 20)) / 1000.0;
        if (r.Below(8) == 0) {
          t.f1.u64 = r.Next();  // a double that doesn't fit a float
        }
        if (v == 0) {
          RunRecompiled(f.recompiled, t, h.base);
        } else {
          f.native(t, h.base);
        }
        Store(h.base, kMissing, missing);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Bytes(Read(h.base, movie, 64));
        for (uint32_t k = 0; k < 8; k++) {
          out[v].Bytes(Read(h.base, buffers + 4096 * k, 4096));
        }
        std::vector<uint8_t> map = Read(h.base, movie + 0x9BC0, 0x420);
        const uint32_t pairs = Load(h.base, movie + 0x9BCC + 1024);
        for (uint32_t k = 0; k < pairs && k < 128; k++) {
          const uint32_t at = 0x9BCC + 8 * k + 4;
          const uint32_t value = Load(h.base, movie + at);
          if (value != 0 && memory::IsHeapBlock(value)) {
            const uint32_t buffer = Load(h.base, value);
            out[v].Word(k);
            out[v].Bytes(Read(h.base, value + 4, 24));
            out[v].Word(memory::IsPages(buffer));
            out[v].Bytes(Read(h.base, buffer, 4096));
            memory::FreePages(buffer);
            memory::crt::OperatorDelete(value);
            for (uint32_t b = 0; b < 4; b++) {
              map[at - 0x9BC0 + b] = 0;
            }
          }
        }
        out[v].Bytes(map);
        out[v].Word(t.r3.u32);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("case {}", i));
    }
    results.Report();
  }
  memory::Free(buffers);
}

// --- The movie table's open and delete ---

// The movie table (0x826C5CCC: array, capacity, count) swapped for a case's
// own array while alive, then put back.
class TableSwap {
 public:
  explicit TableSwap(uint8_t* base) : base_(base) {
    for (uint32_t k = 0; k < 3; k++) {
      saved_[k] = Load(base, ui_movies::kMovieTable + 4 * k);
    }
  }
  ~TableSwap() {
    for (uint32_t k = 0; k < 3; k++) {
      Store(base_, ui_movies::kMovieTable + 4 * k, saved_[k]);
    }
  }

 private:
  uint8_t* base_;
  uint32_t saved_[3];
};

// A table of the case's: capacity 1 to 4, up to the capacity of pairs with
// keys 100 + n; `id`'s pair holds `value` when `with_id`. Returns the array.
uint32_t MakeTable(Random& r, uint8_t* base, uint32_t id, bool with_id, uint32_t value) {
  const uint32_t capacity = 1 + r.Below(4);
  const uint32_t array = memory::crt::OperatorNewArray(base, 8 * capacity).value_or(0);
  RandomFill(r, base, array, 8 * capacity);
  uint32_t count = r.Below(capacity + 1);
  if (with_id && count == 0) {
    count = 1;
  }
  const uint32_t at = with_id ? r.Below(count) : count;
  for (uint32_t k = 0; k < count; k++) {
    Store(base, array + 8 * k, k == at ? id : 100 + k);
    Store(base, array + 8 * k + 4, k == at ? value : 0x40000000 + k);
  }
  Store(base, ui_movies::kMovieTable, array);
  Store(base, ui_movies::kMovieTable + 4, capacity);
  Store(base, ui_movies::kMovieTable + 8, count);
  return array;
}

// The table after a case: the array (1 when it's still the case's, else
// its size), capacity, count and the pairs (`movie` as 1); the array freed.
void TableOutputs(uint8_t* base, uint32_t array, uint32_t movie, Outputs& out) {
  const uint32_t now = Load(base, ui_movies::kMovieTable);
  out.Word(now == array ? 1 : memory::IsHeapBlock(now) ? memory::Size(now) : now);
  out.Word(Load(base, ui_movies::kMovieTable + 4));
  const uint32_t count = Load(base, ui_movies::kMovieTable + 8);
  out.Word(count);
  for (uint32_t k = 0; k < count; k++) {
    out.Word(Load(base, now + 8 * k));
    const uint32_t value = Load(base, now + 8 * k + 4);
    out.Word(value != 0 && value == movie ? 1 : value);
  }
  memory::crt::OperatorDeleteArray(now);
  if (now != array && memory::IsHeapBlock(array)) {
    memory::crt::OperatorDeleteArray(array);
  }
}

// UiMovies::Open with the table swapped for the case's (the id not there,
// there with a movie, or there with 0) and the load recorded: r3, the
// frame's +80 and +84, the table, the movie made (as ConstructCases) and
// the load's call (the pages as 1, the movie as 1); everything freed after.
void OpenCases(Context& h) {
  Results results("UiMovies::Open (sub_8211E808)");
  if (!hooks::NativeArchiveQueue()) {
    ITC_LOG_WARN("harness: ui_movie_objects: UiMovies::Open needs the archive's queue native");
    return;
  }
  static constexpr uint32_t kIds[] = {22575, 22574, 22576, 22560, 22595, 22559};
  for (int i = 0; i < 24; i++) {
    const uint64_t seed = h.random.Next();
    const uint32_t id = 7 + h.random.Below(3);
    const uint32_t asset = kIds[h.random.Below(6)];
    const uint32_t kind = h.random.Below(3);  // 0 not there, 1 a movie, 2 a 0
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      TableSwap swap(h.base);
      const uint32_t array = MakeTable(r, h.base, id, kind != 0, kind == 1 ? 0x12345678 : 0);
      ArchiveLoads loads({int64_t(r.Below(1000))});
      PPCContext t = CallContext(h);
      t.r3.u64 = id;
      t.r4.u64 = asset;
      const uint32_t frame = t.r1.u32 - 128;
      if (v == 0) {
        RunRecompiled(__imp__sub_8211E808, t, h.base);
      } else {
        t.r3.u64 = ui_movies::OpenMovie(h.base, id, asset, frame, loads.Native(),
                                        ui_movies_game_hooks::OpenCalls(t, h.base, t.r1.u32));
      }
      out[v].Word(t.r3.u32);
      const uint32_t movie = kind == 1 ? 0 : Load(h.base, frame + 80);
      out[v].Word(Load(h.base, frame + 80) == movie && movie != 0 ? 1 : Load(h.base, frame + 80));
      out[v].Word(Load(h.base, frame + 84));
      TableOutputs(h.base, array, movie, out[v]);
      const std::vector<uint32_t>& calls = loads.calls();
      out[v].Word(uint32_t(calls.size()));
      for (size_t k = 0; k < calls.size(); k++) {
        if (k % 6 == 1 && calls[k] != 0 && memory::IsPages(calls[k])) {
          out[v].Word(1);
          memory::FreePages(calls[k]);
        } else {
          out[v].Word(k % 6 == 3 && calls[k] == movie ? 1 : calls[k]);
        }
      }
      if (movie != 0 && memory::IsHeapBlock(movie)) {
        std::vector<uint32_t> made;
        const uint32_t textures = Load(h.base, movie + 28);
        const uint32_t items = Load(h.base, movie + 40);
        PointerWord(out[v], textures, 0, made);
        PointerWord(out[v], items == 0 ? 0 : items - 16, 0, made);
        out[v].Bytes(MovieBytes(h.base, movie, {28, 40}));
        ui_movies::DestroyMovie(h.base, movie);
        memory::crt::OperatorDelete(movie);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}: kind {}, asset {}", i, kind, asset));
  }
  results.Report();
}

// A movie for UiMovie::Unload: random bytes, built natively, then its three
// file copies (pages or 0), 0 to 4 textures (12-byte blocks or 0), 0 to 3
// queued text items of random bytes, 0 to 3 texts (a record and its pages)
// and +12 = 0. The blocks and pages go in `blocks` and `pages`.
void MakeLoadedMovie(Random& r, uint8_t* base, uint32_t movie, std::vector<uint32_t>& blocks,
                     std::vector<uint32_t>& pages) {
  RandomFill(r, base, movie, kMovieBytes);
  ui_movies::ConstructMovie(base, movie);
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t copy = r.Below(4) == 0 ? 0 : memory::AllocPages(4096 * (1 + r.Below(3)), 4096);
    Store(base, movie + 4 * k, copy);
    pages.push_back(copy);
  }
  const uint32_t textures = Load(base, movie + 28);
  const uint32_t n = r.Below(5);
  for (uint32_t k = 0; k < n; k++) {
    const uint32_t entry = r.Below(4) == 0 ? 0 : memory::crt::OperatorNew(base, 12).value_or(0);
    if (entry != 0) {
      RandomFill(r, base, entry, 12);
    }
    Store(base, textures + 4 * k, entry);
    blocks.push_back(entry);
  }
  Store(base, movie + 36, n);
  const uint32_t m = r.Below(4);
  RandomFill(r, base, Load(base, movie + 40), 128 * m);
  Store(base, movie + 48, m);
  Store(base, movie + 12, 0);
  const uint32_t texts = r.Below(4);
  for (uint32_t k = 0; k < texts; k++) {
    const uint32_t record = memory::crt::OperatorNew(base, 28).value_or(0);
    const uint32_t buffer = memory::AllocPages(4096, 4096);
    RandomFill(r, base, record, 28);
    Store(base, record, buffer);
    Store(base, movie + 0x9BCC + 8 * k, 200 + k);
    Store(base, movie + 0x9BCC + 8 * k + 4, record);
    blocks.push_back(record);
    pages.push_back(buffer);
  }
  Store(base, movie + 0x9FCC, texts);
}

// What's left of a movie after Unload: which blocks and pages were freed,
// the movie's bytes (its pointers masked) and its text list's block; then
// destroyed and whatever's left freed.
void UnloadedOutputs(uint8_t* base, uint32_t movie, const std::vector<uint32_t>& blocks,
                     const std::vector<uint32_t>& pages, bool movie_freed, Outputs& out) {
  for (uint32_t block : blocks) {
    out.Word(block != 0 && memory::IsHeapBlock(block));
  }
  for (uint32_t page : pages) {
    out.Word(page != 0 && memory::IsPages(page));
  }
  if (!movie_freed) {
    std::vector<uint32_t> masked = {0, 4, 8, 28, 40};
    for (uint32_t k = 0; k < 3; k++) {
      masked.push_back(0x9BCC + 8 * k + 4);
    }
    out.Bytes(MovieBytes(base, movie, masked));
    out.Bytes(Read(base, Load(base, movie + 40), 128 * 4));
    ui_movies::DestroyMovie(base, movie);
  }
  for (uint32_t block : blocks) {
    if (block != 0 && memory::IsHeapBlock(block)) {
      memory::crt::OperatorDelete(block);
    }
  }
  for (uint32_t page : pages) {
    if (page != 0 && memory::IsPages(page)) {
      memory::FreePages(page);
    }
  }
}

// UiMovie::Unload over MakeLoadedMovie's movies: what's freed, the movie,
// its frame's +80 (the last item copied and cleared), r3 (a text record by
// its number, the movie as 0).
void UnloadCases(Context& h, uint32_t movie) {
  Results results("UiMovie::Unload (sub_8211B540)");
  for (int i = 0; i < 40; i++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<uint32_t> blocks, pages;
      MakeLoadedMovie(r, h.base, movie, blocks, pages);
      PPCContext t = CallContext(h);
      t.r3.u64 = movie;
      const uint32_t frame = t.r1.u32 - 240;
      Fill(h.base, frame + 80, 128);
      if (v == 0) {
        RunRecompiled(__imp__sub_8211B540, t, h.base);
      } else {
        t.r3.u64 = ui_movies::UnloadMovie(
            h.base, movie, frame,
            ui_movies_game_hooks::DeleteCalls(t, h.base, t.r1.u32 + 128).unload);
      }
      out[v].Bytes(Read(h.base, frame + 80, 128));
      uint32_t r3 = t.r3.u32 == movie ? 0 : 0xFFFFFFFFu;
      for (size_t k = 0; k < blocks.size(); k++) {
        if (blocks[k] != 0 && t.r3.u32 == blocks[k]) {
          r3 = uint32_t(k) + 1;
        }
      }
      out[v].Word(r3);
      UnloadedOutputs(h.base, movie, blocks, pages, false, out[v]);
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// UiMovies::Delete with the table swapped for the case's (the id not there,
// there with a loaded movie, or there with 0): what's freed, the table, the
// frame's +80 and +84, r3 (the table as 0, an entry by its offset in the
// array, the empty entry as 1).
void DeleteCases(Context& h) {
  Results results("UiMovies::Delete (sub_8211E258; sub_8211B540)");
  for (int i = 0; i < 40; i++) {
    const uint64_t seed = h.random.Next();
    const uint32_t id = 7 + h.random.Below(3);
    const uint32_t kind = h.random.Below(4);  // 0 not there, 1 a 0, else a movie
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      TableSwap swap(h.base);
      std::vector<uint32_t> blocks, pages;
      uint32_t movie = 0;
      if (kind >= 2) {
        movie = memory::crt::OperatorNew(h.base, kMovieBytes).value_or(0);
        MakeLoadedMovie(r, h.base, movie, blocks, pages);
      }
      const uint32_t array = MakeTable(r, h.base, id, kind != 0, movie);
      PPCContext t = CallContext(h);
      t.r3.u64 = id;
      const uint32_t frame = t.r1.u32 - 128;
      if (v == 0) {
        RunRecompiled(__imp__sub_8211E258, t, h.base);
      } else {
        t.r3.u64 = ui_movies::DeleteMovie(h.base, id, frame,
                                          ui_movies_game_hooks::DeleteCalls(t, h.base, t.r1.u32));
      }
      const uint32_t now = Load(h.base, ui_movies::kMovieTable);
      const uint32_t r3 = t.r3.u32;
      out[v].Word(r3 == ui_movies::kMovieTable    ? 0
                  : r3 == 0x826C5CDC              ? 1
                  : r3 >= now && r3 < now + 64    ? 0x100 + (r3 - now)
                                                  : r3);
      out[v].Word(Load(h.base, frame + 80) == movie && movie != 0 ? 1 : Load(h.base, frame + 80));
      out[v].Word(Load(h.base, frame + 84));
      const bool freed = movie != 0 && !memory::IsHeapBlock(movie);
      out[v].Word(freed);
      TableOutputs(h.base, array, movie, out[v]);
      if (movie != 0) {
        UnloadedOutputs(h.base, movie, blocks, pages, freed, out[v]);
        if (!freed) {
          memory::crt::OperatorDelete(movie);
        }
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}: kind {}", i, kind));
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
  GetTextCases(h, movie);
  AddNamedAnimationsCases(h, movie);
  SetterCases(h, movie);
  UnloadCases(h, movie);
  memory::Free(movie);
  OpenCases(h);
  DeleteCases(h);
  EffectCases(h);
}

}  // namespace harness
