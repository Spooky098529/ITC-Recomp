// The race HUD's movie choice and the rival logo's draw (src/race_hud/,
// docs/functions/race-hud.md) against the recompiled functions. Their movie
// calls are answered by a stand-in (ui_movies_hooks::g_update_callee_stand_in,
// asked by the overrides of UiMovie::Update, which UiMovie::Run jumps to,
// UiMovie::ApplyAnimation, which UiMovie::FinishNamed calls, and UiMovie::
// Draw's callees) that keeps what each is handed and answers the update and
// ApplyAnimation with 0 or 1. Compared: the block, the calls (function,
// return address, stack pointer, registers), r3 and lr.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/race_hud.h"
#include "hooks/ui_movies.h"
#include "ui_movies/game.h"

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>

#include <fmt/format.h>

#include <bit>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_hud);

namespace harness {
namespace {

struct Call {
  uint32_t function, lr, r1;
  std::vector<uint64_t> registers;
  uint64_t f1;
};
std::vector<Call>* g_calls = nullptr;
Random g_answers;

bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  Call call{function, uint32_t(ctx.lr), ctx.r1.u32, {}, 0};
  switch (function) {
    case 0x825A08E0:
      call.registers = {ctx.r3.u64, ctx.r4.u64, ctx.r6.u64};
      call.f1 = ctx.f1.u64;
      ctx.r3.u64 = g_answers.Below(2);
      break;
    case 0x8211D2F8:
      call.registers = {ctx.r3.u64, ctx.r4.u64};
      ctx.r3.u64 = g_answers.Below(2);
      break;
    case 0x8259FA00:
    case 0x82119B80:
      call.registers = {ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64};
      break;
    case 0x8211CD90:
      call.registers = {ctx.r3.u64, ctx.r4.u64, ctx.r5.u64};
      break;
    default:
      call.registers = {ctx.r3.u64, ctx.r4.u64};
      break;
  }
  g_calls->push_back(std::move(call));
  return true;
}

// The recompiled version with --native_race_hud off.
void RunRecompiled(GuestFunction recompiled, PPCContext& t, uint8_t* base) {
  const bool native = ITC_FLAG(native_race_hud);
  ITC_FLAG(native_race_hud) = false;
  recompiled(t, base);
  ITC_FLAG(native_race_hud) = native;
}

void Record(Outputs& out, const std::vector<Call>& calls) {
  out.Word(uint32_t(calls.size()));
  for (const Call& call : calls) {
    out.Word(call.function);
    out.Word(call.lr);
    out.Word(call.r1);
    for (uint64_t value : call.registers) {
      out.Word(uint32_t(value >> 32));
      out.Word(uint32_t(value));
    }
    if (call.function == 0x825A08E0) {
      out.Double(call.f1);
    }
  }
}

// RaceHud::RunMovie on a made-up HUD: its state (+448: 0, 1, 2, 4, 5 or a
// random word; 3 left out, its sub_82212908's original never seen in play
// and not stood in), +456 0, 1 or random, its movies (+604, +624, +632) in
// the block (UiMovie::Draw reads their +16 and +20), +444 an object in the
// block with a tick count at +3932.
void RunMovieCases(Context& h, uint32_t block) {
  constexpr uint32_t kBytes = 0x2000;
  Results results("RaceHud::RunMovie (sub_822141E8, with sub_82212908)");
  for (int i = 0; i < 400; i++) {
    const Random start = h.random;
    Outputs out[2];
    uint32_t state = 0;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Random& r = h.random;
      std::vector<Call> calls;
      g_calls = &calls;
      Fill(h.base, block, kBytes);
      const uint32_t hud = block;
      constexpr uint32_t kStates[] = {0, 1, 2, 4, 5};
      state = r.Below(8) == 0 ? (uint32_t(r.Next()) | 8) : kStates[r.Below(5)];
      guest::StoreU32(h.base, hud + 448, state);
      guest::StoreU32(h.base, hud + 456,
                      r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
      for (uint32_t offset : {604u, 624u, 632u}) {
        const uint32_t movie = block + 0x400 + 0x100 * r.Below(8);
        guest::StoreU32(h.base, hud + offset, movie);
        guest::StoreU32(h.base, movie + 16, uint32_t(r.Next()));
        guest::StoreU32(h.base, movie + 20, uint32_t(r.Next()));
      }
      const uint32_t object = block + 0xC00;
      guest::StoreU32(h.base, hud + 444, object);
      guest::StoreU32(h.base, object + 3932, uint32_t(r.Next()));
      g_answers = Random(r.Next());
      PPCContext t = CallContext(h);
      t.r3.u64 = hud;
      t.r4.u64 = r.Below(3) == 0 ? r.Next() : r.Below(2);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        RunRecompiled(__imp__sub_822141E8, t, h.base);
      } else {
        sub_822141E8(t, h.base);
      }
      out[v].Bytes(Read(h.base, block, kBytes));
      Record(out[v], calls);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.lr));
      out[v].Word(t.r1.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}: state {:X}", i, state));
  }
  g_calls = nullptr;
  results.Report();
}

// The rival logo's draw on a made-up entry (+4 flags, bit 8 shown or not,
// bit 16 and the others random; +8 an id) and movie: the movie table
// (0x826C5CCC, put back after each run) pointed at made-up pairs (the id
// there, or not: then the word at 0x826C5CDC, pointed at the movie too),
// the movie's lists empty or with a queued slot for the named animation
// (FinishNamed natively on both sides: its own cases are ui_movies_game's).
// Compared also: the original's frame word +80 (the id it stores).
void RivalLogoCases(Context& h, uint32_t block) {
  constexpr uint32_t kGlobals = 0x826C5CCC, kGlobalBytes = 0x14;
  const std::vector<uint8_t> globals = Read(h.base, kGlobals, kGlobalBytes);
  constexpr uint32_t kBytes = 0xB000;
  Results results("RivalLogo::Draw (sub_82211A28)");
  for (int i = 0; i < 400; i++) {
    const Random start = h.random;
    Outputs out[2];
    uint32_t flags = 0;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Random& r = h.random;
      std::vector<Call> calls;
      g_calls = &calls;
      Fill(h.base, block, kBytes);
      const uint32_t movie = block;
      const uint32_t entry = block + 0xA000;
      const uint32_t pairs = block + 0xA100;
      const uint32_t animation = block + 0xA200;
      ui_movies::InitAnimationLists(h.base, movie);
      const uint32_t id = r.Below(4);
      const uint32_t named = r.Below(3);
      flags = uint32_t(r.Next());
      guest::StoreU32(h.base, entry + 4, flags);
      guest::StoreU32(h.base, entry + 8, id);
      // Pairs {k, a movie}: k from 0 to 2 (so id 3 isn't found).
      for (uint32_t k = 0; k < 3; k++) {
        guest::StoreU32(h.base, pairs + 8 * k, k);
        guest::StoreU32(h.base, pairs + 8 * k + 4, k == id ? movie : block + 0x100 * k);
      }
      guest::StoreU32(h.base, kGlobals, pairs);
      guest::StoreU32(h.base, kGlobals + 4, 3);
      guest::StoreU32(h.base, kGlobals + 8, r.Below(4));
      guest::StoreU32(h.base, kGlobals + 16, movie);
      guest::StoreU32(h.base, animation + 12, std::bit_cast<uint32_t>(float(r.Below(40)) / 4.0f));
      if (r.Below(2) == 0) {
        ui_movies::Slot slot{};
        slot[0] = slot[1] = 0xFFFFFFFFu;
        slot[2] = r.Below(2) == 0 ? id : named;
        slot[3] = uint32_t(r.Next());
        slot[4] = animation;
        slot[7] = r.Below(2);
        ui_movies::Append(h.base, movie + ui_movies::kMovieQueue, slot);
      }
      g_answers = Random(r.Next());
      PPCContext t = CallContext(h);
      const uint32_t frame_word = t.r1.u32 - 112 + 80;
      Fill(h.base, frame_word, 4);
      t.r3.u64 = entry;
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        RunRecompiled(__imp__sub_82211A28, t, h.base);
      } else {
        race_hud_hooks::DrawRivalLogo(t, h.base);
      }
      out[v].Bytes(Read(h.base, block, kBytes));
      out[v].Bytes(Read(h.base, frame_word, 4));
      Record(out[v], calls);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.lr));
      out[v].Word(t.r1.u32);
      Write(h.base, kGlobals, globals.data(), kGlobalBytes);
    }
    results.Check(out[0], out[1], fmt::format("case {}: flags {:X}", i, flags));
  }
  g_calls = nullptr;
  results.Report();
}

}  // namespace

void RaceHud(Context& h) {
  auto* memory = rex::system::kernel_state()->memory();
  const uint32_t bytes = 0xC000;
  const uint32_t block = memory->SystemHeapAlloc(bytes);
  if (!block) {
    ITC_LOG_ERROR("harness: race_hud: no block from the system heap");
    return;
  }
  ui_movies_hooks::g_update_callee_stand_in = StandIn;
  RunMovieCases(h, block);
  RivalLogoCases(h, block);
  ui_movies_hooks::g_update_callee_stand_in = nullptr;
  memory->SystemHeapFree(block);
}

}  // namespace harness
