// The tuning screen's pages (src/tuning_pages/,
// docs/functions/tuning-pages.md) against the recompiled functions. Each
// case fills a world of its own (a block from the allocation layer holding
// the screen, its decal images at +10048, and the save data the screen's
// +18904 points at) with random words from the case's seed, plus the stack
// below the call's stack pointer and the caller's area above it; shapes the
// fields the function tests (the state, the category and item around their
// ranges, the picks, the table rows, the garage list); runs the original
// (with --native_tuning_pages off) or the register steps
// (hooks/tuning_pages.h); and compares the blocks, the stack, the caller's
// area, r1 and all of r3 with harness::Outputs (nothing here is a float, so
// all exactly). The screen runs only in the front end, which attract races
// don't reach: these cases are its check.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/tuning_pages.h"
#include "memory/heap.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_tuning_pages);

namespace harness {

namespace {

constexpr uint32_t kScreenBytes = 0xA000;  // the screen (to +40960: the picks' tables with any byte)
constexpr uint32_t kSaveBytes = 0x1A000;   // the save data (to +0x19924)
constexpr uint32_t kStackBytes = 2048;     // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;     // above it

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

// Movie 13's (and 14's) block: the movie (+0, its +28 the texture table),
// the table's 200 words (+0x100), the texture entries they point at (+0x500,
// 8 bytes each), the movie table's pairs (+0xC00; movie 2 too), a pool of 40
// nodes (+0x1000, each child entry another of the pool; the live scene the
// first).
constexpr uint32_t kMovieBytes = 0xB000;
constexpr uint32_t kNodesAt = 0x1000;
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kTableAt = 0x100;
constexpr uint32_t kSlotsAt = 0x500;
constexpr uint32_t kPairsAt = 0xC00;
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kFades = 0x826A50AC;  // the screen fades' words (to 0x826A50E8)
constexpr uint32_t kFadesBytes = 0x3C;
constexpr uint32_t kQueueAt = 0x9F00;    // the command queue [+10036] in the screen's block

// The car parameters [0x826C2E48] (+0) and the one part table their nine
// pointers name (+0x100: car ids 0 to 25 and levels to 255 stay inside).
constexpr uint32_t kParamsBytes = 0x4000;
constexpr uint32_t kCarParams = 0x826C2E48;

struct World {
  World()
      : screen_block(kScreenBytes), save(kSaveBytes), movie(kMovieBytes), params(kParamsBytes) {}
  bool ok() const {
    return screen_block.address != 0 && save.address != 0 && movie.address != 0 &&
           params.address != 0;
  }
  uint32_t screen() const { return screen_block.address; }
  Block screen_block;
  Block save;
  Block movie;
  Block params;
  uint8_t* base = nullptr;
};

// The globals a case points into its world (or writes), put back after.
struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kFades, kFadesBytes},
    {kCarParams, 4},
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A small signed word in [lo, hi], now and then any word.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  if (r.Below(16) == 0) {
    return uint32_t(r.Next());
  }
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

// The fields the pages' functions test: the state 0 to 14 mostly, the
// category -1 to 7, the item 0 to 11, the picks small (now and then any
// byte), the table rows and the wheel fields small (the word indices grow
// with them, and any word would read far outside the world), the garage
// list as the screen's harness lays it.
void ShapeScreen(uint8_t* base, const World& w, Random& r) {
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 9652, Around(r, 0, 14));
  guest::StoreU32(base, s + 9656, Around(r, -1, 7));
  guest::StoreU8(base, s + 9660, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(12)));
  for (uint32_t k = 9661; k <= 9665; k++) {
    guest::StoreU8(base, s + k, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(6)));
  }
  guest::StoreU32(base, s + 33900, r.Below(6));
  guest::StoreU32(base, s + 33540, Around(r, 0, 2));
  guest::StoreU32(base, s + 33548, r.Below(10));
  guest::StoreU32(base, s + 33564, r.Below(6));
  guest::StoreU32(base, s + 33572, r.Below(5));
  guest::StoreU32(base, s + 25204, r.Below(5) == 0 ? 17 + r.Below(4) : r.Below(17));
  guest::StoreU32(base, s + 25220, r.Below(38));
  guest::StoreU32(base, s + 25168, Around(r, 0, 1));
  guest::StoreU32(base, s + 25172, Around(r, 0, 1));
  guest::StoreU32(base, s + 30724, Around(r, -1, 3));
  // The state history's depth (its words +9756 up to the byte itself at
  // +10008; deeper pushes would write over the queue's pointer).
  guest::StoreU8(base, s + 10008, uint8_t(r.Below(60)));
  // The decal images' states: 0 to 2, now and then any word.
  for (uint32_t i = 0; i < 200; i++) {
    guest::StoreU32(base, s + 10048 + 36 * i + 8, r.Below(3) == 0 ? Around(r, 0, 2) : 2);
  }
  // Category 5's item 6 (the vinyls) now and then.
  if (r.Below(3) == 0) {
    guest::StoreU32(base, s + 9656, 5);
    guest::StoreU8(base, s + 9660, 6);
  }
  // The save data's garage list: 51 nodes of 4 bytes from +6120 (the head
  // halfword at +204), each link and the head a node, each slot 0 to 3; the
  // walk [+0x19920] steps; the cars' ids (+224) 0 to 25, now and then any
  // word for InchListEnd (LoadPageLayouts' table read stays near).
  const uint32_t save = w.save.address, nodes = save + 6120;
  guest::StoreU32(base, s + 18904, save);
  for (uint32_t k = 0; k < 51; k++) {
    guest::StoreU16(base, nodes + 4 * k, uint16_t(r.Below(51)));
    guest::StoreU16(base, nodes + 4 * k + 2, uint16_t(r.Below(4)));
  }
  guest::StoreU16(base, nodes + 204, uint16_t(r.Below(51)));
  guest::StoreU32(base, save + 0x19920, r.Below(6));
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, nodes + 1968 * k + 224, r.Below(26));
  }
}

// Movie 13 and 14 (one movie named by both) and its texture table, the
// movie table pointed at its pairs (now and then not found: UiMovies::Find
// answers the missing movie's word, the same movie); the screen's command
// queue; the screen fades' counts 0 most of the time.
void ShapeMovie(uint8_t* base, const World& w, Random& r) {
  const uint32_t movie = w.movie.address;
  guest::StoreU32(base, movie + 28, movie + kTableAt);
  for (uint32_t k = 0; k < 200; k++) {
    guest::StoreU32(base, movie + kTableAt + 4 * k, movie + kSlotsAt + 8 * r.Below(200));
  }
  guest::StoreU32(base, movie + kPairsAt, 13);
  guest::StoreU32(base, movie + kPairsAt + 4, movie);
  guest::StoreU32(base, movie + kPairsAt + 8, 14);
  guest::StoreU32(base, movie + kPairsAt + 12, movie);
  guest::StoreU32(base, movie + kPairsAt + 16, 2);
  guest::StoreU32(base, movie + kPairsAt + 20, movie);
  guest::StoreU32(base, ui_movies::kMovieTable, movie + kPairsAt);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 3);
  const auto node = [&](uint32_t n) { return movie + kNodesAt + kNodeBytes * n; };
  guest::StoreU32(base, movie + 16, node(0));
  for (uint32_t n = 0; n < kNodeCount; n++) {
    guest::StoreU32(base, node(n) + 108, r.Below(20));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node(n) + 128 + 8 * e, node(r.Below(kNodeCount)));
    }
  }
  guest::StoreU32(base, kMissingMovie, movie);
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 10036, s + kQueueAt);
  for (uint32_t i = 0; i < kFadesBytes; i += 4) {
    guest::StoreU32(base, kFades + i, r.Below(4) == 0 ? uint32_t(r.Next()) : 0);
  }
}

// The car parameters: their nine part tables' pointers at the one table.
void ShapeParams(uint8_t* base, const World& w) {
  const uint32_t params = w.params.address;
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, params + 16 + 4 * i, params + 0x100);
  }
  guest::StoreU32(base, kCarParams, params);
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// Stack bytes left out of the comparison besides the top register saves:
// [from, to) bytes below the call's stack pointer (a callee's register
// saves, which its native version doesn't write).
using Holes = std::vector<std::pair<uint32_t, uint32_t>>;

// `saves` the original's register saves' bytes below the stack pointer.
void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, uint32_t saves, const Holes& holes,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    std::vector<std::vector<uint8_t>> saved;
    for (const Global& g : kGlobals) {
      saved.push_back(Read(h.base, g.address, g.bytes));
    }
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.screen_block.address, w.screen_block.bytes, r);
    WriteRandom(h.base, w.save.address, w.save.bytes, r);
    WriteRandom(h.base, w.movie.address, w.movie.bytes, r);
    WriteRandom(h.base, w.params.address, w.params.bytes, r);
    ShapeScreen(h.base, w, r);
    ShapeMovie(h.base, w, r);
    ShapeParams(h.base, w);
    t.r3.u64 = w.screen();
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    if (v == 0) {
      const bool flag = ITC_FLAG(native_tuning_pages);
      ITC_FLAG(native_tuning_pages) = false;
      recompiled(t, h.base);
      ITC_FLAG(native_tuning_pages) = flag;
    } else {
      native(t, h.base);
    }
    for (const auto& [from, to] : holes) {
      for (uint32_t a = sp - to; a < sp - from; a++) {
        guest::StoreU8(h.base, a, 0);
      }
    }
    out[v].Bytes(Read(h.base, w.screen_block.address, w.screen_block.bytes));
    out[v].Bytes(Read(h.base, w.save.address, w.save.bytes));
    out[v].Bytes(Read(h.base, w.movie.address, w.movie.bytes));
    out[v].Bytes(Read(h.base, w.params.address, w.params.bytes));
    for (size_t i = 0; i < saved.size(); i++) {
      out[v].Bytes(Read(h.base, kGlobals[i].address, kGlobals[i].bytes));
      Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
    }
    out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - saves));
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, uint32_t saves = 0,
           const Holes& holes = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, saves, holes,
         fmt::format("case {}", c));
  }
  results.Report();
}

const SetupFn kScreenOnly = [](PPCContext&, Random&, const World&) {};

// A word with a signaling NaN made quiet (the originals' lfs and stfs quiet
// it, the host's float copies may not; not the game's data).
uint32_t Quiet(uint32_t word) {
  if ((word & 0x7F800000u) == 0x7F800000u && (word & 0x007FFFFFu) != 0) {
    word |= 0x00400000u;
  }
  return word;
}
void QuietRange(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, Quiet(guest::LoadU32(base, address + i)));
  }
}

// The vinyls: the screen's records and the garage cars' decal records
// without signaling NaNs, now and then small ints and plain floats (and
// negative angles) in them.
const SetupFn kVinyls = [](PPCContext&, Random& r, const World& w) {
  const uint32_t s = w.screen();
  QuietRange(w.base, s + 30880, 31220 - 30880);
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t c = w.save.address + 6120 + 1968 * k + 212;
    QuietRange(w.base, c + 96, 192);
    if (r.Below(2) == 0) {
      for (uint32_t i = 0; i < 6; i++) {
        const uint32_t record = c + 96 + 32 * i;
        guest::StoreFloat(w.base, record + 0, float(int32_t(r.Below(720)) - 360));
        guest::StoreFloat(w.base, record + 4, float(int32_t(r.Below(720)) - 360));
        guest::StoreFloat(w.base, record + 8, float(r.Below(1000)) / 4.0f - 100.0f);
        guest::StoreFloat(w.base, record + 12, float(r.Below(1000)) / 8.0f);
        guest::StoreFloat(w.base, record + 16, float(r.Below(100)) / 16.0f);
      }
    }
  }
  if (r.Below(2) == 0) {
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t record = s + 30880 + 24 * i;
      guest::StoreU32(w.base, record + 0, Around(r, -300, 300));
      guest::StoreU32(w.base, record + 4, Around(r, -300, 300));
      guest::StoreFloat(w.base, record + 8, float(r.Below(100)) / 16.0f);
      guest::StoreFloat(w.base, record + 12, float(int32_t(r.Below(720)) - 360));
      guest::StoreU32(w.base, record + 16, Around(r, 0, 1));
    }
  }
};

// DecalImages::FreeTexture: entry i 0 to 60 (now and then any word, with
// upper bits); its texture held by a kept slot (mode 1), by another of
// entries 33 to 50 (mode 2), or by nothing (mode 0: a real header of the
// layer's pages, or 0, and real physical data, or 0, freed through
// Texture::Free on both sides). The allocations' addresses differ between
// the two runs, so words equal to them (in the blocks and r3) are compared as
// tokens; whatever is left alive is freed after.
void FreeTextures(Context& h, const World& w) {
  Results results("DecalImages::FreeTexture");
  for (int c = 0; c < 60; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      WriteRandom(h.base, w.screen_block.address, w.screen_block.bytes, r);
      const uint32_t images = w.screen() + 10048;
      const uint32_t mode = r.Below(3);
      uint64_t i = mode == 2 ? 33 + r.Below(18) : r.Below(61);
      if (r.Below(10) == 0) {
        i |= r.Next() << 32;
      }
      const uint32_t entry = images + 36 * uint32_t(i);
      uint32_t header = 0, data = 0;
      if (mode == 0) {
        header = r.Below(5) == 0 ? 0 : memory::AllocPages(52, 4096);
        data = r.Below(4) == 0 ? 0 : memory::AllocPhysical(0x1000, 0, 0xFFFFFFFF, 0, 0x404);
        guest::StoreU32(h.base, entry + 28, header);
        guest::StoreU32(h.base, entry + 32, data);
      } else {
        const uint32_t texture = uint32_t(r.Next());
        guest::StoreU32(h.base, entry + 28, texture);
        if (mode == 1) {
          guest::StoreU32(h.base, images + 7236 + 8 * r.Below(200), texture);
        } else {
          uint32_t j = 33 + r.Below(18);
          if (j == uint32_t(i)) {
            j = j == 50 ? 33 : j + 1;
          }
          guest::StoreU32(h.base, images + 36 * j + 28, texture);
        }
      }
      t.r3.u64 = images;
      t.r4.u64 = i;
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        const bool flag = ITC_FLAG(native_tuning_pages);
        ITC_FLAG(native_tuning_pages) = false;
        __imp__sub_821FE6A0(t, h.base);
        ITC_FLAG(native_tuning_pages) = flag;
      } else {
        hooks::TuningPagesFreeTexture(t, h.base);
      }
      // The allocations as tokens.
      auto token = [&](uint32_t word) -> uint32_t {
        if (word != 0 && word == header) return 0xA110C001u;
        if (word != 0 && word == data) return 0xA110C002u;
        return word;
      };
      for (uint32_t a = w.screen(); a < w.screen() + kScreenBytes; a += 4) {
        guest::StoreU32(h.base, a, token(guest::LoadU32(h.base, a)));
      }
      out[v].Bytes(Read(h.base, w.screen_block.address, w.screen_block.bytes));
      out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes + kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(token(t.r3.u32));
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      const bool header_alive = header != 0 && memory::IsPages(header);
      out[v].Word(header_alive);
      if (header_alive) {
        memory::FreePages(header);
      }
      // The data is still allocated unless Texture::Free cleared its word.
      const bool data_alive = data != 0 && guest::LoadU32(h.base, entry + 32) == 0xA110C002u;
      out[v].Word(data_alive);
      if (data_alive) {
        memory::FreePhysical(data);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void TuningPages(Context& h) {
  World w;
  w.base = h.base;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: tuning_pages: no memory for the world");
    return;
  }
  // The pages' counts and owned flags.
  Group(h, w, "TuningScreen::LevelCount", 30, kScreenOnly, __imp__sub_821CC728,
        hooks::TuningPagesLevelCount);
  Group(h, w, "TuningScreen::TypeCount", 30, kScreenOnly, __imp__sub_821CC7B8,
        hooks::TuningPagesTypeCount);
  Group(h, w, "TuningScreen::PageCursor", 30, kScreenOnly, __imp__sub_821CCD38,
        hooks::TuningPagesPageCursor);
  Group(h, w, "TuningScreen::PageItemCount", 30, kScreenOnly, __imp__sub_821CCDB0,
        hooks::TuningPagesPageItemCount);
  Group(h, w, "TuningScreen::IsPickOwned", 50, kScreenOnly, __imp__sub_821CCE30,
        hooks::TuningPagesIsPickOwned);
  Group(h, w, "TuningScreen::MarkOwned", 50, kScreenOnly, __imp__sub_821CD498,
        hooks::TuningPagesMarkOwned);
  // TestAndSetWheelFlag: wheel numbers -2 to 180, now and then any word
  // (with upper bits).
  Group(
      h, w, "TuningScreen::TestAndSetWheelFlag", 40,
      [](PPCContext& t, Random& r, const World&) {
        t.r4.u64 = r.Below(8) == 0 ? r.Next() : uint64_t(int64_t(Around(r, -2, 180)));
      },
      __imp__sub_821CDF98, hooks::TuningPagesTestAndSetWheelFlag);
  Group(h, w, "TuningScreen::SetWheelListEnd", 30, kScreenOnly, __imp__sub_821E7DB0,
        hooks::TuningPagesSetWheelListEnd);
  // InchListEnd: the selected car's id 0 to 25, now and then any word.
  Group(
      h, w, "TuningScreen::InchListEnd", 40,
      [](PPCContext&, Random& r, const World& w) {
        if (r.Below(6) == 0) {
          const uint32_t nodes = w.save.address + 6120;
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(w.base, nodes + 1968 * k + 224, uint32_t(r.Next()));
          }
        }
      },
      __imp__sub_821EF288, hooks::TuningPagesInchListEnd);

  // The vinyl editor's set-up.
  Group(h, w, "TuningScreen::SetUpVinylBusyIcon", 10, kScreenOnly, __imp__sub_821FE560,
        hooks::TuningPagesSetUpVinylBusyIcon);
  // AllLoaded: r3 the decal images; none loading a third of the time.
  Group(
      h, w, "DecalImages::AllLoaded", 30,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.screen() + 10048;
        if (r.Below(3) == 0) {
          for (uint32_t i = 0; i < 200; i++) {
            const uint32_t state = w.screen() + 10048 + 36 * i + 8;
            if (guest::LoadU32(w.base, state) == 1) {
              guest::StoreU32(w.base, state, 2);
            }
          }
        }
      },
      __imp__sub_821FE5D8, hooks::TuningPagesAllLoaded);
  Group(h, w, "TuningScreen::KeepListTextures", 20, kScreenOnly, __imp__sub_821FE7D8,
        hooks::TuningPagesKeepListTextures);
  Group(h, w, "TuningScreen::SetEditorLimits", 30, kScreenOnly, __imp__sub_821FE420,
        hooks::TuningPagesSetEditorLimits);
  Group(h, w, "TuningScreen::LoadPageLayouts", 20, kScreenOnly, __imp__sub_822045F8,
        hooks::TuningPagesLoadPageLayouts);

  // The decal images' textures (r30, r31 and lr's saves 24 bytes,
  // __savegprlr_29's 32, r31 and lr's 16). r3 the images (now and then with
  // upper bits), the textures 0 to 199 (the movie's table).
  FreeTextures(h, w);

  // Movie 2's nodes (r30, r31 and lr's saves 24 bytes; r31 and lr's 16).
  Group(
      h, w, "TuningScreen::ShowPricePanel", 30,
      [](PPCContext&, Random& r, const World& w) {
        constexpr uint32_t kStates[] = {16, 9, 10, 11, 14, 5, 15, 17};
        guest::StoreU32(w.base, w.screen() + 9652, kStates[r.Below(8)]);
        guest::StoreU32(w.base, w.screen() + 25204, r.Below(3) == 0 ? 0 : r.Below(17));
      },
      __imp__sub_821D9E68, hooks::TuningPagesShowPricePanel, 24);
  Group(
      h, w, "TuningScreen::SetBrandLogo", 30,
      [](PPCContext&, Random& r, const World& w) {
        guest::StoreU32(w.base, w.screen() + 25204, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(18));
      },
      __imp__sub_821EED98, hooks::TuningPagesSetBrandLogo, 16);
  // HsvToRgb: r4 the out (four floats), r5 the in (h, s, v) in the screen's
  // block; h 0 to 1.25 mostly (now and then negative, infinite, a quiet
  // NaN), s 0.0 or -0.0 now and then, v 0 to 1.
  Group(
      h, w, "Color::HsvToRgb", 60,
      [](PPCContext& t, Random& r, const World& w) {
        const uint32_t in = w.screen() + 0x200, out = w.screen() + 0x100;
        t.r4.u64 = out;
        t.r5.u64 = in;
        constexpr uint32_t kOdd[] = {0xBF000000u, 0x7F800000u, 0xFF800000u, 0x7FC00000u,
                                     0x3F800000u, 0x00000000u};
        guest::StoreU32(w.base, in + 0,
                        r.Below(6) == 0 ? kOdd[r.Below(6)]
                                        : std::bit_cast<uint32_t>(float(r.Below(1250)) / 1000.0f));
        const uint32_t sat = r.Below(5);
        guest::StoreU32(w.base, in + 4,
                        sat == 0 ? 0u : sat == 1 ? 0x80000000u
                                               : std::bit_cast<uint32_t>(float(r.Below(1001)) / 1000.0f));
        guest::StoreU32(w.base, in + 8, std::bit_cast<uint32_t>(float(r.Below(1001)) / 1000.0f));
        QuietRange(w.base, in + 12, 4);
      },
      __imp__sub_8224AB50, hooks::TuningPagesHsvToRgb);

  // The vinyls' records (LoadVinyls' __savegprlr_27: 48 bytes).
  Group(h, w, "TuningScreen::StoreVinyls", 40, kVinyls, __imp__sub_82203F20,
        hooks::TuningPagesStoreVinyls);
  Group(h, w, "TuningScreen::LoadVinyls", 40, kVinyls, __imp__sub_822040A8,
        hooks::TuningPagesLoadVinyls, 48);
  auto restore = [](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = w.screen() + 10048;
    if (r.Below(6) == 0) {
      t.r3.u64 |= r.Next() << 32;
    }
    t.r4.u64 = Around(r, -1, 4);
    t.r5.u64 = r.Below(200);
  };
  Group(h, w, "DecalImages::RestoreVinylTexture", 30, restore, __imp__sub_82204698,
        hooks::TuningPagesRestoreVinylTexture, 24);
  Group(h, w, "DecalImages::RestoreStickerTexture", 30, restore, __imp__sub_821F89B8,
        hooks::TuningPagesRestoreStickerTexture, 24);
  Group(
      h, w, "DecalImages::BindVinylTexture", 40,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.screen() + 10048;
        t.r4.u64 = Around(r, -2, 45);
        t.r5.u64 = Around(r, -1, 2);
      },
      __imp__sub_82204730, hooks::TuningPagesBindVinylTexture, 32);
  // The restores: RestoreMovieTexture's __savegprlr_29 (32 bytes) with its
  // callees' (24, 112 bytes below); RestoreVinylMovie's (32) with
  // RestoreStickerTexture's (24, 128 below); RestoreStickerMovie's r31 and
  // lr (16) with RestoreMovieTexture's (32, 96 below) and its callees' (24,
  // 208 below).
  auto images = [](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = w.screen() + 10048;
    if (r.Below(6) == 0) {
      t.r3.u64 |= r.Next() << 32;
    }
  };
  Group(
      h, w, "DecalImages::RestoreMovieTexture", 30,
      [images](PPCContext& t, Random& r, const World& w) {
        images(t, r, w);
        t.r4.u64 = Around(r, -1, 4);
        t.r5.u64 = r.Below(200);
      },
      __imp__sub_822081A0, hooks::TuningPagesRestoreMovieTexture, 32, {{112, 136}});
  Group(h, w, "DecalImages::RestoreVinylMovie", 20, images, __imp__sub_822081D8,
        hooks::TuningPagesRestoreVinylMovie, 32, {{128, 152}});
  Group(h, w, "DecalImages::RestoreStickerMovie", 20, images, __imp__sub_821F6DD8,
        hooks::TuningPagesRestoreStickerMovie, 16, {{96, 128}, {208, 232}});
  Group(h, w, "TuningScreen::BindVinylTextures", 20, kScreenOnly, __imp__sub_82203D40,
        hooks::TuningPagesBindVinylTextures, 16);
  // EnterEditor: the fades' counts 0 and the images loaded most of the
  // time, category 5's items 6 and 7 mostly; BindVinylTextures' saves (its
  // frame 96 bytes below) out too.
  Group(
      h, w, "TuningScreen::EnterEditor", 40,
      [](PPCContext& t, Random& r, const World& w) {
        const uint32_t s = w.screen();
        if (r.Below(4) != 0) {
          guest::StoreU32(w.base, kFades + 0x30, 0);
          guest::StoreU32(w.base, kFades + 0x38, 0);
        }
        if (r.Below(4) != 0) {
          for (uint32_t i = 0; i < 200; i++) {
            if (guest::LoadU32(w.base, s + 10048 + 36 * i + 8) == 1) {
              guest::StoreU32(w.base, s + 10048 + 36 * i + 8, 2);
            }
          }
        }
        if (r.Below(5) != 0) {
          guest::StoreU32(w.base, s + 9656, 5);
          guest::StoreU8(w.base, s + 9660, uint8_t(r.Below(4) == 0 ? r.Below(9) : 6 + r.Below(2)));
        }
        guest::StoreU32(w.base, s + 9652, r.Below(3) == 0 ? Around(r, 20, 62) : 60);
        if (r.Below(4) == 0) {
          t.r3.u64 |= r.Next() << 32;
        }
      },
      __imp__sub_821D84F8, hooks::TuningPagesEnterEditor, 16, {{96, 112}});

  // The current choices: the body kit [+36588] 0 a quarter of the time,
  // else a level to 255 (r31 and lr's saves 16 bytes).
  Group(
      h, w, "TuningScreen::SetAeroAvailability", 40,
      [](PPCContext&, Random& r, const World& w) {
        guest::StoreU32(w.base, w.screen() + 36588, r.Below(4) == 0 ? 0 : r.Below(256));
      },
      __imp__sub_821DE160, hooks::TuningPagesSetAeroAvailability, 16);
  // SetCurrentChoice: categories -1 to 7 and items 0 to 10 (now and then any
  // byte), the indices [+33900] and [+33576] small, the wheels' +25196 0
  // half the time, else 1 to 3; r3 now and then with upper bits (lr's save
  // 8 bytes; SetAeroAvailability's r31 and lr, 96 bytes below, out too).
  Group(
      h, w, "TuningScreen::SetCurrentChoice", 120,
      [](PPCContext& t, Random& r, const World& w) {
        const uint32_t s = w.screen();
        guest::StoreU32(w.base, s + 9656, Around(r, -1, 7));
        guest::StoreU8(w.base, s + 9660, uint8_t(r.Below(10) == 0 ? r.Next() : r.Below(11)));
        // A third of the time the paths with calls or more stores: the body
        // kit, the wheels, category 3's item 0, the interior's item 3.
        if (r.Below(3) == 0) {
          static constexpr uint8_t kPicks[4][2] = {{4, 9}, {5, 0}, {3, 0}, {6, 3}};
          const auto& pick = kPicks[r.Below(4)];
          guest::StoreU32(w.base, s + 9656, pick[0]);
          guest::StoreU8(w.base, s + 9660, pick[1]);
        }
        if (r.Below(4) == 0) {
          guest::StoreU8(w.base, s + 9661 + r.Below(4), 0);
        }
        guest::StoreU32(w.base, s + 33900, r.Below(8));
        guest::StoreU32(w.base, s + 33576, r.Below(8));
        guest::StoreU32(w.base, s + 25196, r.Below(2) == 0 ? 0 : 1 + r.Below(3));
        if (r.Below(6) == 0) {
          t.r3.u64 |= r.Next() << 32;
        }
      },
      __imp__sub_821E1DB0, hooks::TuningPagesSetCurrentChoice, 8, {{96, 112}});
}

}  // namespace harness
