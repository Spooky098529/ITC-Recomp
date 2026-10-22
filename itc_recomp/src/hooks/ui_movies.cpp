// The UI movie library's functions natively: adapters in front of
// src/ui_movies/ (docs/functions/ui-movies.md).

#include "guest.h"

#include <bit>
#include <optional>

#include "base/flags.h"
#include "base/log.h"
#include "hooks/guest_call.h"
#include "ui_movies/ui_movies.h"

ITC_FLAG_BOOL(native_ui_movies, true, "ITC",
              "Run the UI movie library's functions (the movies the HUD and the menus are made "
              "of: the accessors sub_825A6508, sub_825A6518, sub_825A6528, sub_825A6540, "
              "sub_825A6558, sub_8259D600, sub_8259D618, sub_8259D628, sub_8259D648, "
              "UiMovie::Child sub_8259D658 and sub_8259D670; the chunk and file helpers "
              "sub_8259D5C8, sub_825A0EB0 and sub_825A0CD0; UiMovie::SetRenderer sub_8259D798; "
              "the draw context reset sub_8259D930; the keys sub_8259FA20, sub_8259FB30 and "
              "sub_8259FC18; the recorders sub_8259DAD0, sub_8259E098 and sub_8259D7B0; the "
              "file fix-ups sub_825A0CF8 with sub_825A0C60, and UiMovie::FixUpFile "
              "sub_825A0EF0 with sub_825A0E40, sub_825A0ED8, sub_8259D688 and sub_825A6570) "
              "natively instead of through the translated code (same results; checked by "
              "--run_harnesses=ui_movies)");

// AllocateSlots's diagnostics (blend_state.cpp) note each slot's caller: with
// any of them on, the recorders run the originals.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

namespace {
bool SlotDiagnostics() {
  return ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
         ITC_FLAG(log_recording_by_pass);
}
}  // namespace

// An accessor: r3 = the object, r4 = i; returns the address (or the pointer)
// `native` gives.
#define UI_MOVIE_ACCESSOR(address, expression)    \
  extern "C" REX_FUNC(sub_##address) {            \
    REX_FUNC_PROLOGUE();                          \
    if (!ITC_FLAG(native_ui_movies)) {            \
      __imp__sub_##address(ctx, base);            \
      return;                                     \
    }                                             \
    const uint32_t object = ctx.r3.u32;           \
    const uint32_t i = ctx.r4.u32;                \
    (void)base;                                   \
    ctx.r3.u64 = (expression);                    \
  }

UI_MOVIE_ACCESSOR(825A6508, ui_movies::NodeTrack(object, i))
UI_MOVIE_ACCESSOR(825A6518, ui_movies::Key(object, i))
UI_MOVIE_ACCESSOR(825A6528, ui_movies::NodeTrackList(base, object, i))
UI_MOVIE_ACCESSOR(825A6540, ui_movies::KeyList(base, object, i))
UI_MOVIE_ACCESSOR(825A6558, ui_movies::SubAnimation(base, object, i))
UI_MOVIE_ACCESSOR(8259D600, ui_movies::VertexOffset(object, i))
UI_MOVIE_ACCESSOR(8259D618, ui_movies::PrimitiveTexture(object, i))
UI_MOVIE_ACCESSOR(8259D628, ui_movies::PrimitiveVertex(base, object, i))
UI_MOVIE_ACCESSOR(8259D648, ui_movies::PictureTexture(object, i))
UI_MOVIE_ACCESSOR(8259D658, ui_movies::Child(base, object, i))
UI_MOVIE_ACCESSOR(8259D670, ui_movies::SceneAnimation(base, object, i))
#undef UI_MOVIE_ACCESSOR

// sub_8259D5C8: r3 = a chunk, r4 = a tag; the next chunk with that tag.
extern "C" REX_FUNC(sub_8259D5C8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259D5C8(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::FindChunk(base, ctx.r3.u32, ctx.r4.u32);
}

// sub_825A0EB0: r3 = a movie file; its scene, or 0.
extern "C" REX_FUNC(sub_825A0EB0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A0EB0(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::FileScene(base, ctx.r3.u32);
}

// sub_825A0CD0: r3 = an XMD file, r4 = i; its entry i, or 0.
extern "C" REX_FUNC(sub_825A0CD0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A0CD0(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::XmdEntry(base, ctx.r3.u32, ctx.r4.u32);
}

// UiMovie::SetRenderer: r3 = the renderer, r4 = the command list.
extern "C" REX_FUNC(sub_8259D798) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259D798(ctx, base);
    return;
  }
  ui_movies::SetRenderer(base, ctx.r3.u32, ctx.r4.u32);
}

// sub_8259D930: r3 = a draw context, reset; returns it.
extern "C" REX_FUNC(sub_8259D930) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259D930(ctx, base);
    return;
  }
  ui_movies::ResetDrawContext(base, ctx.r3.u32);
}

// The keys: r3, r4 = the two keys, f1 = t.
// sub_8259FA20: an integer track's eased value in r3.
extern "C" REX_FUNC(sub_8259FA20) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259FA20(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ui_movies::EaseInteger(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}

// sub_8259FB30: a float track's value in f1. r3 and r4, which tan leaves
// as its second _ctrlfp does, aren't read by the one caller (sub_825A0548)
// and are left as they were.
extern "C" REX_FUNC(sub_8259FB30) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_ui_movies)) {
    ctx.fpscr.disableFlushMode();
    if (const std::optional<double> value =
            ui_movies::CubicFloat(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64)) {
      ctx.f1.f64 = *value;
      return;
    }
  }
  __imp__sub_8259FB30(ctx, base);
}

// sub_8259FC18: an integer track's cubic value in r3.
extern "C" REX_FUNC(sub_8259FC18) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_ui_movies)) {
    ctx.fpscr.disableFlushMode();
    if (const std::optional<uint32_t> value =
            ui_movies::CubicInteger(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64)) {
      ctx.r3.u64 = *value;
      return;
    }
  }
  __imp__sub_8259FC18(ctx, base);
}

namespace {
// Hands a recorded chunk to the renderer (vtable slot 4: r3 = the renderer,
// r4 = the chunk, r5 = the key, f1 = the float at `depth`), from a frame like
// the original's, with its return address.
void Submit(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes, uint32_t return_address,
            uint32_t chunk, uint32_t key, uint32_t depth) {
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, frame_bytes, return_address, [&](uint32_t) {
    const uint32_t renderer = guest::LoadU32(base, ui_movies::kRenderer);
    ctx.r3.u64 = renderer;
    ctx.r4.u64 = chunk;
    ctx.r5.u64 = key;
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(std::bit_cast<float>(guest::LoadU32(base, depth)));
    REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, renderer) + 16));
  });
  ctx.lr = lr;
}

constexpr uint32_t kQuadDepth = 0x82000A34;        // 0.0 (the caller's f1 isn't read)
constexpr uint32_t kClipResetDepth = 0x82001C00;
constexpr uint32_t kStateResetDepth = 0x82001BFC;

// A quad recorder: r3 = the quad, r4 = the key (f1 isn't read).
bool RecordQuad(PPCContext& ctx, uint8_t* base, bool textured, uint32_t return_address) {
  if (!ITC_FLAG(native_ui_movies) || SlotDiagnostics()) {
    return false;
  }
  const uint32_t key = ctx.r4.u32;
  const std::optional<uint32_t> chunk = ui_movies::RecordQuad(
      base, guest::LoadU32(base, ui_movies::kCommandList), ctx.r3.u32, textured);
  if (!chunk) {
    return false;
  }
  Submit(ctx, base, 160, return_address, *chunk, key, kQuadDepth);
  return true;
}
}  // namespace

// sub_8259DAD0: a textured quad or primitive recorded and handed on.
extern "C" REX_FUNC(sub_8259DAD0) {
  REX_FUNC_PROLOGUE();
  if (!RecordQuad(ctx, base, true, 0x8259E090)) {
    __imp__sub_8259DAD0(ctx, base);
  }
}

// sub_8259E098: the same, untextured.
extern "C" REX_FUNC(sub_8259E098) {
  REX_FUNC_PROLOGUE();
  if (!RecordQuad(ctx, base, false, 0x8259E618)) {
    __imp__sub_8259E098(ctx, base);
  }
}

// sub_8259D7B0: r3 = a key (f1 isn't read): the clip reset to the whole
// screen and the state reset, each recorded and handed on.
extern "C" REX_FUNC(sub_8259D7B0) {
  REX_FUNC_PROLOGUE();
  const uint32_t list = guest::LoadU32(base, ui_movies::kCommandList);
  if (!ITC_FLAG(native_ui_movies) || SlotDiagnostics() ||
      !ui_movies::ChunksFit(base, list,
                            {ui_movies::kResetChunkSlots, ui_movies::kResetChunkSlots})) {
    __imp__sub_8259D7B0(ctx, base);
    return;
  }
  const uint32_t key = ctx.r3.u32;
  Submit(ctx, base, 144, 0x8259D888, ui_movies::RecordClipReset(base, list), key,
         kClipResetDepth);
  // The renderer only takes the chunk (it doesn't record into the list), so
  // the second still fits; checked again all the same.
  const uint32_t list_now = guest::LoadU32(base, ui_movies::kCommandList);
  if (!ui_movies::ChunksFit(base, list_now, {ui_movies::kResetChunkSlots})) {
    ITC_LOG_ERROR("ui_movies: the state reset chunk no longer fits after the clip reset's");
    return;
  }
  Submit(ctx, base, 144, 0x8259D924, ui_movies::RecordStateReset(base, list_now), key,
         kStateResetDepth);
}

// The file fix-ups: r3 = the file (and r4 = the movie file for the scene's
// and the animation's); each returns its result in r3.
extern "C" REX_FUNC(sub_825A0C60) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A0C60(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::IsXmd(base, ctx.r3.u32) ? 1 : 0;
}

extern "C" REX_FUNC(sub_825A0CF8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A0CF8(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::CheckXmd(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_825A0E40) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A0E40(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::IsMovieFile(base, ctx.r3.u32) ? 1 : 0;
}

extern "C" REX_FUNC(sub_825A0ED8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A0ED8(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::FileAnimation(base, ctx.r3.u32, ctx.r4.u32);
}

extern "C" REX_FUNC(sub_8259D688) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259D688(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::FixUpScene(base, ctx.r3.u32, ctx.r4.u32);
}

extern "C" REX_FUNC(sub_825A6570) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A6570(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::FixUpAnimation(base, ctx.r3.u32, ctx.r4.u32);
}

// UiMovie::FixUpFile.
extern "C" REX_FUNC(sub_825A0EF0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A0EF0(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::FixUpFile(base, ctx.r3.u32);
}
