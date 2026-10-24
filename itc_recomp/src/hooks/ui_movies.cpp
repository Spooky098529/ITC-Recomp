// The UI movie library's functions natively: adapters in front of
// src/ui_movies/ (docs/functions/ui-movies.md).

#include "guest.h"

#include <bit>
#include <optional>

#include "base/flags.h"
#include "base/log.h"
#include "hooks/guest_call.h"
#include "hooks/ui_movies.h"
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
              "sub_825A0EF0 with sub_825A0E40, sub_825A0ED8, sub_8259D688 and sub_825A6570, "
              "the helpers without adapters (the flag switches their callers); "
              "sub_8259EBE8 and sub_8259DAC0, which call the recorders; UiMovie::RestorePose "
              "sub_82119B80; UiMovie::ApplyTransform sub_8259E6C8, with sub_8259D730; the draw "
              "context copy sub_8259D9F8; the queued texts' copy sub_8259E620, append "
              "sub_8259F700 (its grow the original's) and UiMovie::QueueText sub_8259F770; "
              "UiMovie::DrawPicture sub_8259ED40 and UiMovie::DrawPrimitive sub_8259F348, which "
              "hand their quads to sub_8259EBE8; UiMovie::Draw sub_8259FA00 with "
              "UiMovie::DrawNode sub_8259F848, which has no adapter: the flag switches Draw) "
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

// UiMovie::ApplyTransform: r3 = the draw context, r4 = the node; the
// original's 352-byte frame below r1.
extern "C" REX_FUNC(sub_8259E6C8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259E6C8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ui_movies::ApplyTransform(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 352);
}

// sub_8259D9F8: r3 = a new draw context, r4 = its parent's; returns r3.
extern "C" REX_FUNC(sub_8259D9F8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259D9F8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ui_movies::CopyDrawContext(base, ctx.r3.u32, ctx.r4.u32);
}

// sub_8259E620: r3 = the destination, r4 = a text item; returns r3.
extern "C" REX_FUNC(sub_8259E620) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259E620(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ui_movies::CopyQueuedText(base, ctx.r3.u32, ctx.r4.u32);
}

// sub_8259F700: r3 = a text list, r4 = an item; the original when the list
// is full (its grow).
extern "C" REX_FUNC(sub_8259F700) {
  REX_FUNC_PROLOGUE();
  uint32_t r3 = 0;
  if (!ITC_FLAG(native_ui_movies) || ui_movies::TextListFull(base, ctx.r3.u32)) {
    __imp__sub_8259F700(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ui_movies::AppendText(base, ctx.r3.u32, ctx.r4.u32, &r3);
  ctx.r3.u64 = r3;
}

// UiMovie::QueueText: r3 = the draw context, r5 = the text node, r6 = the
// layer; the original's 272-byte frame below r1. The original when the text
// list is full.
extern "C" REX_FUNC(sub_8259F770) {
  REX_FUNC_PROLOGUE();
  uint32_t r3 = 0;
  if (ITC_FLAG(native_ui_movies)) {
    ctx.fpscr.disableFlushMode();
    if (ui_movies::QueueText(base, ctx.r3.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32 - 272, &r3)) {
      ctx.r3.u64 = r3;
      return;
    }
  }
  __imp__sub_8259F770(ctx, base);
}

// UiMovie::DrawPicture and DrawPrimitive: r3 = the draw context, r5 = the
// node, r6 = the layer (src/hooks/ui_movies.h).
extern "C" REX_FUNC(sub_8259ED40) {
  REX_FUNC_PROLOGUE();
  ui_movies_hooks::DrawPicture(ctx, base);
}
extern "C" REX_FUNC(sub_8259F348) {
  REX_FUNC_PROLOGUE();
  ui_movies_hooks::DrawPrimitive(ctx, base);
}

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

// The file fix-ups: r3 = the file; each returns its result in r3. Their
// helpers (sub_825A0C60, sub_825A0E40, sub_825A0ED8, sub_8259D688 and
// sub_825A6570) have no adapters: only these reach them, natively.
extern "C" REX_FUNC(sub_825A0CF8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A0CF8(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::CheckXmd(base, ctx.r3.u32);
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

// sub_8259EBE8: r3 = a quad or primitive, r4 = its key, f1 = a depth (passed
// on to sub_8259D7B0, which doesn't read it): recorded untextured or
// textured by its +84, then, when clipped (+100), the resets with key r4 + 1.
// The recorders (ours, above) are called from a frame like the original's
// (112 bytes) with its return addresses.
extern "C" REX_FUNC(sub_8259EBE8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259EBE8(ctx, base);
    return;
  }
  const uint32_t quad = ctx.r3.u32, key = ctx.r4.u32;
  const double depth = ctx.f1.f64;
  const uint64_t lr = ctx.lr;
  const bool textured = guest::LoadU32(base, quad + 84) != 0;
  guest_call::InFrame(ctx, base, 112, textured ? 0x8259EC24 : 0x8259EC1C, [&](uint32_t) {
    ctx.r3.u64 = quad;
    ctx.r4.u64 = key;
    ctx.f1.f64 = depth;
    if (textured) {
      sub_8259DAD0(ctx, base);
    } else {
      sub_8259E098(ctx, base);
    }
  });
  if (guest::LoadU32(base, quad + 100) != 0) {
    guest_call::InFrame(ctx, base, 112, 0x8259EC3C, [&](uint32_t) {
      ctx.r3.u64 = uint64_t(key) + 1;
      ctx.fpscr.disableFlushMode();
      ctx.f1.f64 = depth;
      sub_8259D7B0(ctx, base);
    });
  }
  ctx.lr = lr;
}

namespace ui_movies_hooks {

namespace {
// The quad built natively, then handed to sub_8259EBE8 from a frame of
// `frame_bytes` (the original's) returning to `return_address`.
template <typename Build>
void BuildAndRecord(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes,
                    uint32_t return_address, Build build) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const ui_movies::QuadCall call = build(ctx.r1.u32 - frame_bytes);
  guest_call::InFrame(ctx, base, frame_bytes, return_address, [&](uint32_t) {
    ctx.r3.u64 = call.quad;
    ctx.r4.u64 = call.key;
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = call.depth;
    sub_8259EBE8(ctx, base);
  });
  ctx.lr = lr;
}
}  // namespace

void DrawPicture(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259ED40(ctx, base);
    return;
  }
  // Without a texture the original faults (its size read at guest address
  // 4; never so in play): it's left to run.
  if (ui_movies::PictureTextureOf(base, ctx.r3.u32, ctx.r5.u32) == 0) {
    __imp__sub_8259ED40(ctx, base);
    return;
  }
  RecordPicture(ctx, base);
}

void RecordPicture(PPCContext& ctx, uint8_t* base) {
  const uint32_t context = ctx.r3.u32, picture = ctx.r5.u32, layer = ctx.r6.u32;
  BuildAndRecord(ctx, base, 1088, 0x8259F338, [&](uint32_t frame) {
    return ui_movies::BuildPicture(base, context, picture, layer, frame);
  });
}

void DrawPrimitive(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259F348(ctx, base);
    return;
  }
  RecordPrimitive(ctx, base);
}

void RecordPrimitive(PPCContext& ctx, uint8_t* base) {
  const uint32_t context = ctx.r3.u32, primitive = ctx.r5.u32, layer = ctx.r6.u32;
  BuildAndRecord(ctx, base, 1152, 0x8259F6EC, [&](uint32_t frame) {
    return ui_movies::BuildPrimitive(base, context, primitive, layer, frame);
  });
}

namespace {
// DrawNode's calls that need the guest context: from the level's frame
// (`stack`, the r1 the original has there) with its return address.
struct Walk {
  PPCContext* ctx;
  uint8_t* base;
};
uint32_t CallFromWalk(void* user, ui_movies::DrawCall kind, uint32_t context, uint32_t scene,
                      uint32_t node, uint64_t layer, uint32_t stack, uint32_t return_address) {
  Walk& walk = *static_cast<Walk*>(user);
  PPCContext& ctx = *walk.ctx;
  const uint64_t r1 = ctx.r1.u64, lr = ctx.lr;
  ctx.r1.u64 = stack;
  ctx.lr = return_address;
  ctx.r3.u64 = context;
  ctx.r4.u64 = scene;
  ctx.r5.u64 = node;
  ctx.r6.u64 = layer;
  switch (kind) {
    case ui_movies::DrawCall::kPicture:
      DrawPicture(ctx, walk.base);
      break;
    case ui_movies::DrawCall::kPrimitive:
      DrawPrimitive(ctx, walk.base);
      break;
    case ui_movies::DrawCall::kText:
      __imp__sub_8259F770(ctx, walk.base);
      break;
  }
  ctx.r1.u64 = r1;
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u32;
}
}  // namespace

void Draw(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259FA00(ctx, base);
    return;
  }
  ITC_NATIVE_RUNNING("sub_8259FA00 UiMovie::Draw");
  const uint32_t context = ctx.r3.u32;
  guest::StoreU32(base, context + 104, ctx.r7.u32);
  guest::StoreU32(base, context + 96, ctx.r5.u32);
  guest::StoreU32(base, context + 100, ctx.r6.u32);
  ctx.r5.u64 = ctx.r4.u64;
  ctx.r6.u64 = 0;
  RecordDrawNode(ctx, base);
}

void RecordDrawNode(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  Walk walk{&ctx, base};
  ctx.r3.u64 = ui_movies::DrawNode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u64,
                                   ctx.r1.u32, CallFromWalk, &walk);
}

void ResetAfterMovie(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8259DAC0(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r4.u64;
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(std::bit_cast<float>(guest::LoadU32(base, kClipResetDepth)));
  sub_8259D7B0(ctx, base);
}

void RestorePose(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_82119B80(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::RestorePose(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                      ctx.r7.u32);
}
}  // namespace ui_movies_hooks
