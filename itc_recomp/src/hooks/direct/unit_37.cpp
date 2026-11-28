// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 37's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "guest.h"
#include "hooks/c_runtime.h"
#include "hooks/strings_and_font.h"
#include "hooks/ui_movies.h"
#include "hooks/ui_movies_game.h"
#include "race_scene/race_scene.h"
#include "schema/race_scene.h"
#include "schema/ui_movies_game.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

// --- The game's side of the UI movies (src/ui_movies/game.h) ---

// Strings::CountLines: r3 unused (the font), r4 = a wide string; r3 the
// line count, a halfword.
ITC_DIRECT_ENTRY(821087D8) { ctx.r3.u64 = ui_movies::CountLines(ctx.r4.u32); }

// The words' capacity change: r3 = the vector, r4 = the new capacity.
ITC_DIRECT_ENTRY(8211A788) {
  if (const auto r3 = ui_movies::WordsReserve(ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *r3;
  } else {
    lifted::original::c8211A788(ctx);
  }
}

// A words vector made: r3 = the vector; returned.
ITC_DIRECT_ENTRY(8211AE18) {
  if (const auto r3 = ui_movies::MakeWords(ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
  } else {
    lifted::original::c8211AE18(ctx);
  }
}

// LinkFree: r3 = an animation list, r4 = a position (16-bit, signed). A
// leaf: r3 left as it was (the list).
ITC_DIRECT_ENTRY(8211B230) {
  ui_movies::LinkFree(ctx.r3.u32, int16_t(uint16_t(ctx.r4.u32)));
}

// A pair appended: r3 = a vector of pairs, r4 = the pair's address (+0 the
// key, +4 the value); r3 as the original leaves it.
ITC_DIRECT_ENTRY(8211B458) {
  const uint32_t key = schema::UiMovieWord(ctx.r4.u32).value();
  const uint32_t value = schema::UiMovieWord(ctx.r4.u32 + 4).value();
  if (const auto r3 = ui_movies::PairsAppend(ctx.r3.u32, key, value)) {
    ctx.r3.u64 = *r3;
  } else {
    lifted::original::c8211B458(ctx);
  }
}

// UiMovie::Unload: r3 = movie, from its 240-byte frame below r1; its
// callees as UiMovies::Delete's (DeleteCalls' unload, whose frame there is
// 128 bytes below the stack pointer it's given: Delete's caller's).
ITC_DIRECT_ENTRY(8211B540) {
  const uint64_t lr = ctx.lr;
  const uint32_t sp = ctx.r1.u32;
  const ui_movies::DeleteCalls calls = ui_movies_game_hooks::DeleteCalls(ctx, sp + 128);
  ctx.r3.u64 = ui_movies::UnloadMovie(ctx.r3.u32, sp - 240, calls.unload);
  ctx.lr = lr;
}

// The text's +24 and +4: r3 = movie, r4 = a text id; the field, or 0.
ITC_DIRECT_ENTRY(8211B7B8) { ctx.r3.u64 = ui_movies::TextField(ctx.r3.u32, ctx.r4.u32, 24); }
ITC_DIRECT_ENTRY(8211B9B8) { ctx.r3.u64 = ui_movies::TextField(ctx.r3.u32, ctx.r4.u32, 4); }

// UiMovie::ScrollOffset: r3 = movie, r4 = the text's draw parameters; f1
// the x.
ITC_DIRECT_ENTRY(8211B800) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = ui_movies::ScrollOffset(ctx.r3.u32, ctx.r4.u32);
}

// The scroll counter advanced: r3 = movie, r4 = a text id; r3 left at the
// movie's texts' map (movie + 0x9BCC, as TextMap::Set leaves it).
ITC_DIRECT_ENTRY(8211B940) {
  const uint32_t movie = ctx.r3.u32;
  ui_movies::AdvanceTextScroll(movie, ctx.r4.u32);
  ctx.r3.u64 = uint32_t(movie + 0x9BCC);
}

// UiMovie::SetNameText: r3 = movie, r4 = a TXT node, r5 = the name record,
// from its 128-byte frame below r1; r3 as SetNameText leaves it.
ITC_DIRECT_ENTRY(8211CAA0) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> r3;
  {
    hooks::FormatGuestCalls calls(ctx);
    r3 = ui_movies::SetNameText(calls.get(), ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  }
  ctx.lr = lr;
  if (r3) {
    ctx.r3.u64 = *r3;
  } else {
    lifted::original::c8211CAA0(ctx);
  }
}

// UiMovie::MoveNamedAnimation: r3 = movie, r4 = a named animation's index,
// r5 = 1 to the looping list, 0 to the queue. r3 the slot appended to;
// with no match, the copy in its 176-byte frame (+80) when the list had
// slots, else the movie (as the moves' adapters, ui_movies_game.cpp).
ITC_DIRECT_ENTRY(8211DCE8) {
  const uint32_t movie = ctx.r3.u32;
  const ui_movies::Moved moved =
      ui_movies::MoveNamedAnimation(movie, ctx.r4.u32, ctx.r5.u32 != 0);
  if (moved.slot) {
    ctx.r3.u64 = *moved.slot;
  } else {
    ctx.r3.u64 = moved.looked ? uint32_t(ctx.r1.u32 - 176 + 80) : movie;
  }
}

// UiMovies::DeleteAll: no arguments; each Delete from its 112-byte frame
// below r1 (as its jump sub_8211E370's adapter). r3 as DeleteAll leaves it.
ITC_DIRECT_ENTRY(8211E2C8) {
  const uint64_t lr = ctx.lr;
  const uint32_t sp = ctx.r1.u32 - 112;
  ctx.r3.u64 =
      ui_movies::DeleteAll(sp, ui_movies_game_hooks::DeleteCalls(ctx, sp), ctx.r3.u32);
  ctx.lr = lr;
}

// --- The UI movie library (src/ui_movies/ui_movies.h) ---

// UiMovie::ApplyTransform: r3 = the draw context, r4 = the node, from its
// 352-byte frame below r1; r3 as the original leaves it.
ITC_DIRECT_ENTRY(8259E6C8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ui_movies::ApplyTransform(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 352);
}

// TextList::Append: r3 = the list, r4 = the item; r3 the slot. A full list
// (its grow, never seen in play) runs the original.
ITC_DIRECT_ENTRY(8259F700) {
  uint32_t r3 = 0;
  if (ui_movies::AppendText(ctx.r3.u32, ctx.r4.u32, &r3)) {
    ctx.r3.u64 = r3;
  } else {
    lifted::original::c8259F700(ctx);
  }
}

// UiMovie::QueueText: r3 = the draw context, r5 = the node, r6 = the
// layer, from its 272-byte frame below r1. A full list runs the original.
ITC_DIRECT_ENTRY(8259F770) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (ui_movies::QueueText(ctx.r3.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32 - 272, &r3)) {
    ctx.r3.u64 = r3;
  } else {
    lifted::original::c8259F770(ctx);
  }
}

// UiMovie::DrawNode: r3 = the draw context, r4 = the scene, r5 = the node,
// r6 = the layer (its register step, ui_movies.cpp).
ITC_DIRECT_ENTRY(8259F848) { ui_movies_hooks::RecordDrawNode(ctx); }

// The keys: r3 = k0, r4 = k1, f1 = t.
ITC_DIRECT_ENTRY(8259FA20) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ui_movies::EaseInteger(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}
// f1 the value.
ITC_DIRECT_ENTRY(8259FB30) {
  ctx.fpscr.disableFlushMode();
  if (const auto f1 = ui_movies::CubicFloat(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64)) {
    ctx.f1.f64 = *f1;
  } else {
    lifted::original::c8259FB30(ctx);
  }
}
// r3 the value.
ITC_DIRECT_ENTRY(8259FC18) {
  ctx.fpscr.disableFlushMode();
  if (const auto r3 = ui_movies::CubicInteger(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64)) {
    ctx.r3.u64 = *r3;
  } else {
    lifted::original::c8259FC18(ctx);
  }
}

// UiMovie::TrackProperty: r3 = the property id (64 bits); r3 its
// descriptor's address. Its frame 128 bytes below r1; atexit through the
// native one or the guest call.
ITC_DIRECT_ENTRY(8259FE68) {
  const uint64_t lr = ctx.lr;
  hooks::GuestAtexit atexit{ctx};
  ctx.r3.u64 = ui_movies::TrackProperty(ctx.r3.u64, ctx.r1.u32 - 128, atexit.Call());
  ctx.lr = lr;
}

// --- The race scene (src/race_scene/race_scene.h) ---

// RaceScene::DestroyCars: r3 = the object + 16. r3 left as it was (its
// last callee, the array destructor iterator, isn't read after it).
ITC_DIRECT_ENTRY(8215D548) {
  ctx.fpscr.disableFlushMode();
  race_scene::DestroyCars(ctx.r3.u32);
}

// RaceScene::Create: no arguments; r3 the scene made ([0x826D82D4], as
// RaceScene::RaceScene leaves it).
ITC_DIRECT_ENTRY(8215D738) {
  ctx.fpscr.disableFlushMode();
  race_scene::SceneCreate();
  ctx.r3.u64 = schema::RaceSceneGlobals::scene<schema::View>().address;
}

// Glows::SetParams: r3 = the 11-float entry; r3 the last glow, f1 the
// last float given (the entry's +40). Where the glows' setters aren't the
// plain C++'s, the original.
ITC_DIRECT_ENTRY(8228F0B0) {
  ctx.fpscr.disableFlushMode();
  if (!race_scene::EffectParamsReady()) {
    lifted::original::c8228F0B0(ctx);
    return;
  }
  const uint32_t entry = ctx.r3.u32;
  ctx.r3.u64 = race_scene::GlowsSetParams(entry);
  ctx.f1.f64 = double(guest::LoadFloat(entry + 40));
}
