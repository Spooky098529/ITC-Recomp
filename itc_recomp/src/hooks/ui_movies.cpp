// The UI movie library's functions natively: adapters in front of
// src/ui_movies/ (docs/functions/ui-movies.md).

#include "guest.h"

#include <bit>
#include <optional>

#include "base/flags.h"
#include "base/log.h"
#include "hooks/c_runtime.h"
#include "hooks/guest_call.h"
#include "hooks/ui_movies.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

ITC_FLAG_BOOL(native_ui_movies, true, "ITC",
              "Run the UI movie library's functions (the movies the HUD and the menus are made "
              "of: the accessors "
              "sub_8259D618, sub_8259D628, sub_8259D648, "
              "UiMovie::Child sub_8259D658 and sub_8259D670; the chunk and file helpers "
              "sub_8259D5C8, sub_825A0EB0 and sub_825A0CD0; UiMovie::SetRenderer sub_8259D798; "
              "the draw context reset sub_8259D930; the recorders sub_8259DAD0, sub_8259E098 and sub_8259D7B0; the "
              "file fix-ups sub_825A0CF8 with sub_825A0C60, and UiMovie::FixUpFile "
              "sub_825A0EF0 with sub_825A0E40, sub_825A0ED8, sub_8259D688 and sub_825A6570, "
              "the helpers without adapters (the flag switches their callers); "
              "sub_8259EBE8 and sub_8259DAC0, which call the recorders; UiMovie::RestorePose "
              "sub_82119B80; UiMovie::DrawPicture sub_8259ED40 and UiMovie::DrawPrimitive "
              "sub_8259F348, which hand their quads to sub_8259EBE8; UiMovie::Draw sub_8259FA00 "
              "with the walk under it, which has no adapters (the flag switches Draw): "
              "UiMovie::DrawNode sub_8259F848, UiMovie::ApplyTransform sub_8259E6C8 with "
              "sub_8259D730, the draw context copy sub_8259D9F8, the queued texts' copy "
              "sub_8259E620, append sub_8259F700 (its grow the original's) and "
              "UiMovie::QueueText sub_8259F770; UiMovie::ApplyAnimation sub_825A08E0 (its tracks "
              "through sub_825A0548's override; the accessors only it calls, without adapters: "
              "sub_825A6508, sub_825A6518, sub_825A6528, sub_825A6540, sub_825A6558); UiMovie::ApplyTrack sub_825A0548, with native "
              "atexit, and under it, without adapters (the flag switches ApplyTrack), "
              "UiMovie::TrackProperty sub_8259FE68, the keys sub_8259FA20, sub_8259FB30 "
              "and sub_8259FC18, and the accessor sub_8259D600); UiMovie::Update sub_8211D2F8 (its "
              "calls through their overrides, from its override's wrapper in "
              "src/frame_pacing/per_frame_logic.cpp) "
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

UI_MOVIE_ACCESSOR(8259D618, ui_movies::PrimitiveTexture(object, i))
UI_MOVIE_ACCESSOR(8259D628, ui_movies::PrimitiveVertex(base, object, i))
UI_MOVIE_ACCESSOR(8259D648, ui_movies::PictureTexture(object, i))
UI_MOVIE_ACCESSOR(8259D658, ui_movies::Child(base, object, i))
UI_MOVIE_ACCESSOR(8259D670, ui_movies::SceneAnimation(base, object, i))
#undef UI_MOVIE_ACCESSOR

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

// The keys (sub_8259FA20, sub_8259FB30, sub_8259FC18) have no adapters:
// only the native ApplyTrack reaches them in play.


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

namespace {
// ApplyTrack's key functions through the translated code (ui_movies::KeyCall),
// from a frame like the original's (224 bytes) with its return addresses.
struct GuestKeys {
  PPCContext& ctx;
  uint8_t* base;
  ui_movies::KeyCall Call() { return {&GuestKeys::Run, this}; }
  static ui_movies::KeyResult Run(void* self, uint32_t function, uint32_t k0, uint32_t k1,
                                  double t) {
    auto& keys = *static_cast<GuestKeys*>(self);
    PPCContext& ctx = keys.ctx;
    uint8_t* base = keys.base;
    uint32_t return_address = 0;
    switch (function) {
      case 0x8259FB30: return_address = 0x825A0840; break;
      case 0x8259FC18: return_address = 0x825A0700; break;
      case 0x8259FD40: return_address = 0x825A0670; break;
      case 0x8259FAA8: return_address = 0x825A065C; break;
      default: ITC_LOG_ERROR("ui_movies: ApplyTrack: no key function {:08X}", function); return {};
    }
    const uint64_t lr = ctx.lr;
    guest_call::InFrame(ctx, base, 224, return_address, [&](uint32_t) {
      ctx.r3.u64 = k0;
      ctx.r4.u64 = k1;
      ctx.fpscr.disableFlushMode();
      ctx.f1.f64 = t;
      switch (function) {
        case 0x8259FB30: __imp__sub_8259FB30(ctx, base); break;
        case 0x8259FC18: __imp__sub_8259FC18(ctx, base); break;
        case 0x8259FD40: sub_8259FD40(ctx, base); break;
        default: sub_8259FAA8(ctx, base); break;
      }
    });
    ctx.lr = lr;
    return {ctx.r3.u64, ctx.f1.f64};
  }
};
}  // namespace

namespace ui_movies_hooks {
void ApplyTrackNative(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  hooks::GuestAtexit atexit{ctx, base};
  GuestKeys keys{ctx, base};
  ui_movies::ApplyTrack(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r5.u32, ctx.r6.u32,
                        ctx.r1.u32 - 224, atexit.Call(), keys.Call());
}

void ApplyTrack(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_ui_movies) || !hooks::NativeAtexit()) {
    __imp__sub_825A0548(ctx, base);
    return;
  }
  ApplyTrackNative(ctx, base);
}
}  // namespace ui_movies_hooks

namespace {
// ApplyAnimation's tracks (ui_movies::TrackCall): sub_825A0548 (its override)
// from the level's frame, with the original's return address.
struct GuestTracks {
  PPCContext& ctx;
  uint8_t* base;
  ui_movies::TrackCall Call() { return {&GuestTracks::Run, this}; }
  static void Run(void* self, uint32_t frame, uint32_t node, uint32_t track, double t,
                  uint32_t k0, uint32_t k1) {
    auto& tracks = *static_cast<GuestTracks*>(self);
    PPCContext& ctx = tracks.ctx;
    uint8_t* base = tracks.base;
    const uint64_t stack = ctx.r1.u64, lr = ctx.lr;
    ctx.r1.u64 = frame;
    ctx.r3.u64 = node;
    ctx.r4.u64 = track;
    ctx.r5.u64 = k0;
    ctx.r6.u64 = k1;
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = t;
    ctx.lr = 0x825A0AF4;
    sub_825A0548(ctx, base);
    ctx.r1.u64 = stack;
    ctx.lr = lr;
  }
};
}  // namespace

void ui_movies_hooks::ApplyAnimationNative(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestTracks tracks{ctx, base};
  const uint32_t node = ctx.r3.u32, animation = ctx.r4.u32, loop = ctx.r6.u32;
  const double time = ctx.f1.f64;
  ctx.r3.u64 =
      ui_movies::ApplyAnimation(base, node, animation, time, loop, ctx.r1.u32 - 224, tracks.Call());
}

// UiMovie::ApplyAnimation: r3 = the node, r4 = the animation, f1 = the time,
// r6 = loop; r3 = 1 while playing.
extern "C" REX_FUNC(sub_825A08E0) {
  REX_FUNC_PROLOGUE();
  if (ui_movies_hooks::g_update_callee_stand_in != nullptr &&
      ui_movies_hooks::g_update_callee_stand_in(0x825A08E0, ctx, base)) {
    return;
  }
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_825A08E0(ctx, base);
    return;
  }
  ui_movies_hooks::ApplyAnimationNative(ctx, base);
}

ui_movies_hooks::UpdateCalleeStandIn ui_movies_hooks::g_update_callee_stand_in = nullptr;

// UiMovie::Update: its calls made as the original's (r1 its frame, lr the
// return address after each call), the rest in src/ui_movies/update.cpp.
void ui_movies_hooks::Update(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_ui_movies)) {
    __imp__sub_8211D2F8(ctx, base);
    return;
  }
  const uint32_t movie = ctx.r3.u32;
  const bool draw_only = ctx.r4.s32 != 0;
  const uint32_t queue = movie + ui_movies::kMovieQueue;
  const uint32_t looping = movie + ui_movies::kMovieLooping;
  const uint64_t lr = ctx.lr;
  bool playing = false;
  ctx.fpscr.disableFlushMode();
  guest_call::InFrame(ctx, base, ui_movies::kUpdateFrame, 0x8211D330, [&](uint32_t frame) {
    const uint32_t local = frame + ui_movies::kUpdateLocal;
    auto word = [&](uint32_t address) { return guest::LoadU32(base, address); };
    auto count = [&](uint32_t list) { return int32_t(int16_t(guest::LoadU16(base, list + 136))); };
    // UiMovie::RestorePose(movie, live, rest, 0, 0).
    auto restore_pose = [&](uint32_t live, uint32_t rest, uint32_t return_address) {
      ctx.r3.u64 = movie;
      ctx.r4.u64 = live;
      ctx.r5.u64 = rest;
      ctx.r6.u64 = 0;
      ctx.r7.u64 = 0;
      ctx.lr = return_address;
      sub_82119B80(ctx, base);
    };
    // UiMovie::ApplyAnimation on a slot (+12 the tree, +16 the animation,
    // +20 the time); its r3.
    auto apply = [&](uint32_t slot, uint64_t loop, uint32_t return_address) {
      ctx.r3.u64 = word(slot + 12);
      ctx.r4.u64 = word(slot + 16);
      ctx.r6.u64 = loop;
      ctx.fpscr.disableFlushMode();
      ctx.f1.f64 = double(guest::LoadFloat(base, slot + 20));
      ctx.lr = return_address;
      sub_825A08E0(ctx, base);
      ctx.fpscr.disableFlushMode();
      return ctx.r3.s32;
    };
    // AnimationList::Insert of a slot at the local list's end.
    auto keep = [&](uint32_t slot, uint32_t return_address) {
      ctx.r3.u64 = local;
      ctx.r4.u64 = slot;
      ctx.r5.s64 = count(local);
      ctx.lr = return_address;
      sub_8211B4D0(ctx, base);
    };
    // The drawing: UiMovie::Draw, DrawTexts, and sub_8259DAC0 with 678 and
    // 148.
    auto draw = [&](uint64_t texts_r5, const uint32_t (&returns)[4]) {
      ctx.r3.u64 = movie + 64;
      ctx.r4.u64 = word(movie + ui_movies::kMovieLive);
      ctx.r5.u64 = movie + ui_movies::kMovieTextures;
      ctx.r6.u64 = movie + 40;
      ctx.r7.s64 = -1;
      ctx.lr = returns[0];
      sub_8259FA00(ctx, base);
      ctx.r3.u64 = movie;
      ctx.r4.u64 = movie + 40;
      ctx.r5.u64 = texts_r5;
      ctx.lr = returns[1];
      sub_8211CD90(ctx, base);
      for (uint32_t k = 0; k < 2; k++) {
        ctx.r3.u64 = movie + 64;
        ctx.r4.u64 = k == 0 ? 678 : 148;
        ctx.lr = returns[2 + k];
        sub_8259DAC0(ctx, base);
      }
    };
    restore_pose(word(movie + ui_movies::kMovieSaved), word(movie + ui_movies::kMovieLive),
                 0x8211D330);
    ui_movies::UpdateBegin(base, local);
    if (draw_only) {
      // The queue's first chain applied, then the looping animations; no
      // time steps.
      const int32_t queued = count(queue);
      for (int32_t i = 0; i < queued; i++) {
        const uint32_t slot = ui_movies::SlotOf(base, queue, ui_movies::LinkAt(base, queue, i));
        apply(slot, word(slot + 28), 0x8211D3E4);
        if (int32_t(word(slot + 24)) == 0) {
          break;
        }
      }
      for (uint32_t i = 0; i < uint32_t(count(looping)); i++) {
        apply(ui_movies::SlotOf(base, looping, ui_movies::LinkAt(base, looping, i)), 0,
              0x8211D468);
      }
      draw(1, {0x8211D498, 0x8211D4A8, 0x8211D4B4, 0x8211D4C0});
      restore_pose(word(movie + ui_movies::kMovieLive), word(movie + ui_movies::kMovieSaved),
                   0x8211D4D8);
      playing = true;
      return;
    }
    // The queue's first chain: each slot applied and its time stepped
    // (unless paused, +36); a finished chained slot with +32 set moved to
    // the local list.
    int32_t queued = count(queue);
    if (queued == 0) {
      playing = true;
    } else {
      int32_t applied = 0;
      for (int32_t i = 0; i < queued; i++) {
        const uint32_t slot = ui_movies::SlotOf(base, queue, ui_movies::LinkAt(base, queue, i));
        const int32_t result = apply(slot, word(slot + 28), 0x8211D578);
        applied++;
        if (int32_t(word(slot + 36)) == 0) {
          ui_movies::StepQueuedTime(base, slot);
        }
        if (result != 0) {
          playing = true;
        }
        if (int32_t(word(slot + 24)) == 0) {
          break;
        }
        if (result == 0 && int32_t(word(slot + 32)) != 0) {
          keep(slot, 0x8211D608);
          ctx.r3.u64 = queue;
          ctx.r4.u64 = uint32_t(i);
          ctx.lr = 0x8211D614;
          sub_8211B080(ctx, base);
          applied--;
          queued--;
          i--;
        }
      }
      // A chain that has finished: the slots applied moved to the local
      // list.
      if (!playing && guest::LoadU16(base, queue + 136) != 0 && applied != 0) {
        do {
          const uint32_t link = ui_movies::LinkAt(base, queue, 0);
          keep(ui_movies::SlotOf(base, queue, link), 0x8211D674);
          ui_movies::UnlinkQueueFirst(base, queue);
          applied--;
        } while (applied != 0);
      }
    }
    // The looping animations, their times stepped and wrapped.
    for (uint32_t i = 0; i < uint32_t(count(looping)); i++) {
      const uint32_t slot = ui_movies::SlotOf(base, looping, ui_movies::LinkAt(base, looping, i));
      apply(slot, 0, 0x8211D720);
      if (int32_t(word(slot + 36)) == 0) {
        ui_movies::StepLoopingTime(base, slot);
      }
    }
    draw(0, {0x8211D78C, 0x8211D79C, 0x8211D7A8, 0x8211D7B4});
    restore_pose(word(movie + ui_movies::kMovieLive), word(movie + ui_movies::kMovieSaved),
                 0x8211D7CC);
    // The slots moved off the queue: applied once more, after the drawing,
    // and dropped; the pose saved again.
    int32_t kept = count(local);
    if (kept != 0) {
      do {
        const uint16_t first = guest::LoadU16(base, local + 132);
        const uint32_t link = local + (uint32_t(int32_t(int16_t(first))) << 2);
        apply(ui_movies::SlotOf(base, local, link), 0, 0x8211D810);
        kept--;
        ui_movies::UnlinkLocalFirst(base, local, link, first);
      } while (kept != 0);
      restore_pose(word(movie + ui_movies::kMovieSaved), word(movie + ui_movies::kMovieLive),
                   0x8211D858);
      if (guest::LoadU16(base, queue + 136) != 0) {
        playing = true;
      }
    }
  });
  ctx.lr = lr;
  ctx.r3.u64 = playing ? 1 : 0;
}
