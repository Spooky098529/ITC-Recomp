// The data load at boot natively: adapters in front of src/data_load/
// (docs/functions/data-load.md).

#include "base/flags.h"
#include "data_load/data_load.h"
#include "guest.h"
#include "hooks/app_start_up.h"
#include "hooks/archive.h"
#include "hooks/c_runtime.h"
#include "hooks/data_load.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "ui_movies/game.h"

ITC_FLAG_BOOL(native_data_load, true, "ITC",
              "Run the data load at boot (docs/functions/data-load.md: Defaults::Init "
              "sub_820CCDC0 with Bits224::Fill sub_820CCD10 and Bits480::Fill sub_820CCD68; "
              "Record5232::Record5232 sub_82104C20 with Member1136::Member1136 sub_82102888, "
              "Member4080::Member4080 sub_82104690 and Part464::Part464 sub_821042A0; with "
              "--native_heap, RaceData::Create sub_82102208, InputRecordings::Create "
              "sub_82105508 with InputRecordings::InputRecordings sub_821054A8 and "
              "Replay::Create sub_82105560; with native atexit, the static replays' set-ups "
              "Replay826D8520::StaticInit sub_825D9E60 and Replay82816E50::StaticInit sub_825D9F68; with them and the archive's queue native, "
              "BootData::Load sub_82106618 and BootData::CreateObjects sub_82105868 (their callees "
              "through their adapters), LoadingScreen::Load sub_82247B98 with "
              "LoadingScreen::AddMovie sub_82247AA0, and sub_82291328 with sub_822912A0 (the "
              "renderer's objects through [0x826BABA0]'s slots, as the original); no adapters for the fills, the records' members "
              "InputRecordings::InputRecordings, LoadingScreen::AddMovie and sub_822912A0: only their callers' plain C++ calls them) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=data_load)");

namespace dl = data_load;

extern "C" REX_FUNC(sub_820CCDC0) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x820CCDC0)) {
    return;
  }
  if (!ITC_FLAG(native_data_load)) {
    __imp__sub_820CCDC0(ctx, base);
    return;
  }
  // r3 as lis and addi leave the address: sign-extended.
  ctx.r3.s64 = int32_t(dl::DefaultsInit(base, ctx.r1.u32 - 128));
}

extern "C" REX_FUNC(sub_82104C20) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_data_load)) {
    __imp__sub_82104C20(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = dl::Record5232Construct(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_82102208) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x82102208)) {
    return;
  }
  if (ITC_FLAG(native_data_load) && hooks::NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    if (const auto data = dl::RaceDataCreate(base)) {
      ctx.r3.u64 = *data;
      return;
    }
  }
  __imp__sub_82102208(ctx, base);
}

extern "C" REX_FUNC(sub_82105508) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x82105508)) {
    return;
  }
  if (ITC_FLAG(native_data_load) && hooks::NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    if (const auto block = dl::InputRecordingsCreate(base)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_82105508(ctx, base);
}

extern "C" REX_FUNC(sub_82105560) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x82105560)) {
    return;
  }
  if (ITC_FLAG(native_data_load) && hooks::NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    if (const auto r3 = dl::ReplayCreate(base)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_82105560(ctx, base);
}

// The static replays' set-ups (no direct callers: _cinit's initializer
// tables, so the adapters stay): no arguments; r3 atexit's.
extern "C" REX_FUNC(sub_825D9E60) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_data_load) || !hooks::NativeAtexit()) {
    __imp__sub_825D9E60(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  hooks::GuestAtexit guest{ctx, base};
  ctx.r3.s64 = dl::StaticReplayAInit(base, guest.Call());
}

extern "C" REX_FUNC(sub_825D9F68) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_data_load) || !hooks::NativeAtexit()) {
    __imp__sub_825D9F68(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  hooks::GuestAtexit guest{ctx, base};
  ctx.r3.s64 = dl::StaticReplayBInit(base, guest.Call());
}

// --- The movie sets and the load ---

ITC_FLAG_DECLARE(bool, native_ui_movies_game);
ITC_FLAG_DECLARE(bool, native_c_runtime_heap);

namespace {

bool BootNative() {
  return ITC_FLAG(native_data_load) && hooks::NativeHeapActive() && hooks::NativeArchiveQueue();
}

// A call through slot `slot` of r3's vtable, with the original's return
// address.
void SlotCall(PPCContext& ctx, uint8_t* base, uint32_t slot, uint32_t return_address) {
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 4 * slot);
  ctx.lr = return_address;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
}

// The movie sets' calls, from the adder's frame (112 bytes, `construct_at`
// the original's return address after UiMovie::UiMovie) or the load's.
dl::MovieCalls MovieCalls(PPCContext& ctx, uint8_t* base, uint32_t construct_at) {
  dl::MovieCalls calls;
  calls.construct = [&ctx, base, construct_at](uint32_t movie) {
    if (ITC_FLAG(native_ui_movies_game) && ITC_FLAG(native_c_runtime_heap)) {
      if (const auto made = ui_movies::ConstructMovie(base, movie)) {
        return *made;
      }
    }
    guest_call::InFrame(ctx, base, 112, construct_at, [&](uint32_t) {
      ctx.r3.u64 = movie;
      sub_8211E1C0(ctx, base);  // UiMovie::UiMovie
    });
    return ctx.r3.u32;
  };
  calls.load = hooks::NativeArchiveLoad(base);
  calls.render_object = [&ctx, base](int which) {
    guest_call::InFrame(ctx, base, 112, 0, [&](uint32_t) {
      ctx.r3.u64 = guest::LoadU32(base, dl::kRenderApp);
      SlotCall(ctx, base, 8, which == 0 ? 0x822913AC : 0x822913D8);
      SlotCall(ctx, base, which == 0 ? 1 : 2, which == 0 ? 0x822913BC : 0x822913E8);
    });
    return ctx.r3.u32;
  };
  return calls;
}

}  // namespace

void hooks::LoadingScreenLoadSteps(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  const int64_t r3 = dl::LoadingScreenLoad(base, [&](uint32_t id) -> int64_t {
    const uint32_t at = id == 22575 ? 0x82247BC8 : id == 22574 ? 0x82247BD0 : 0x82247BD8;
    if (const auto serial = dl::LoadingScreenAddMovie(base, id, MovieCalls(ctx, base, 0x82247AD0))) {
      return *serial;
    }
    guest_call::InFrame(ctx, base, 96, at, [&](uint32_t) {
      ctx.r3.u64 = id;
      __imp__sub_82247AA0(ctx, base);  // LoadingScreen::AddMovie
    });
    return ctx.r3.s32 < 0 ? -1 : int64_t(ctx.r3.u32);
  });
  ctx.lr = lr;
  hooks::SetArchiveSerial(ctx, r3);
}

void hooks::OtherMoviesLoadSteps(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  const dl::MovieCalls calls = MovieCalls(ctx, base, 0x822912D0);
  const uint32_t r3 = dl::OtherMoviesLoad(
      base,
      [&](uint32_t id) -> int64_t {
        if (const auto serial = dl::OtherMoviesAdd(base, id, calls)) {
          return *serial;
        }
        guest_call::InFrame(ctx, base, 112, 0x82291350, [&](uint32_t) {
          ctx.r3.u64 = id;
          __imp__sub_822912A0(ctx, base);
        });
        return ctx.r3.s32 < 0 ? -1 : int64_t(ctx.r3.u32);
      },
      calls.render_object);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// LoadingScreen::Load: no arguments; r3 the last serial.
extern "C" REX_FUNC(sub_82247B98) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x82247B98)) {
    return;
  }
  if (!BootNative()) {
    __imp__sub_82247B98(ctx, base);
    return;
  }
  hooks::LoadingScreenLoadSteps(ctx, base);
}

// sub_82291328: no arguments; r3 the renderer.
extern "C" REX_FUNC(sub_82291328) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x82291328)) {
    return;
  }
  if (!BootNative()) {
    __imp__sub_82291328(ctx, base);
    return;
  }
  hooks::OtherMoviesLoadSteps(ctx, base);
}

namespace {

// The callees through their adapters, from a frame of `frame_bytes` as the
// original's (its link register kept above it, as its prologue does).
template <typename Body>
void InBootFrame(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes, Body body) {
  const uint64_t lr = ctx.lr;
  guest::StoreU32(base, ctx.r1.u32 - 8, uint32_t(lr));
  guest_call::InFrame(ctx, base, frame_bytes, 0, [&](uint32_t frame) {
    const dl::BootCall call = [&](uint32_t target, std::optional<uint32_t> r3,
                                  uint32_t return_address) {
      if (r3) {
        ctx.r3.u64 = *r3;
      }
      ctx.lr = return_address;
      REX_CALL_INDIRECT_FUNC(target);
      return ctx.r3.u32;
    };
    body(frame, call);
  });
  ctx.lr = lr;
}

}  // namespace

void hooks::CreateObjectsSteps(PPCContext& ctx, uint8_t* base) {
  uint32_t r3 = 0;
  InBootFrame(ctx, base, 96, [&](uint32_t, const dl::BootCall& call) {
    r3 = dl::CreateObjects(base, call);
  });
  ctx.r3.u64 = r3;
}

void hooks::BootDataLoadSteps(PPCContext& ctx, uint8_t* base) {
  std::optional<uint32_t> r3;
  InBootFrame(ctx, base, 128, [&](uint32_t frame, const dl::BootCall& call) {
    r3 = dl::BootDataLoad(base, frame, call);
  });
  if (r3) {
    ctx.r3.u64 = *r3;
  }
}

// BootData::CreateObjects: no arguments; r3 DataTables::Load's.
extern "C" REX_FUNC(sub_82105868) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x82105868)) {
    return;
  }
  if (!BootNative()) {
    __imp__sub_82105868(ctx, base);
    return;
  }
  hooks::CreateObjectsSteps(ctx, base);
}

// BootData::Load: r3 not read; r3 as the original leaves it.
extern "C" REX_FUNC(sub_82106618) {
  REX_FUNC_PROLOGUE();
  if (!BootNative()) {
    __imp__sub_82106618(ctx, base);
    return;
  }
  hooks::BootDataLoadSteps(ctx, base);
}
