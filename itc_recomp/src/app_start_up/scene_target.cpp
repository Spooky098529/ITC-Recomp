// The scene target's set-up natively (start_up.h,
// docs/functions/app-start-up.md): its surfaces and texture made with the
// graphics resources' plain C++ under the device lock.

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "graphics/resources.h"
#include "graphics/video_output.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

namespace app_start_up {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// A surface's 12-byte parameters (the EDRAM base, the hi-z base, a word),
// in the original's frame.
void Parameters(uint8_t* base, uint32_t at, uint32_t edram) {
  Store(base, at + 0, edram);
  Store(base, at + 4, 0);
  Store(base, at + 8, 0);
}

constexpr uint32_t kColorFormat = 0x1A2201BF;
constexpr uint32_t kDepthFormat = 0x1A220197;
constexpr uint32_t kTextureFormat = 0x182801B6;

}  // namespace

void TilingLargestTile(uint8_t* base, uint32_t tiling, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8209ADD8 Tiling::LargestTile");
  Store(base, out + 0, 0);
  Store(base, out + 4, 0);
  const uint32_t count = Load(base, tiling + 12);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t tile = tiling + 16 + 16 * i;
    const uint32_t width = Load(base, tile + 8) - Load(base, tile + 0);
    const uint32_t height = Load(base, tile + 12) - Load(base, tile + 4);
    if (width > Load(base, out + 0)) {
      Store(base, out + 0, width);
    }
    if (height > Load(base, out + 4)) {
      Store(base, out + 4, height);
    }
  }
}

uint32_t SceneTargetConstruct(uint8_t* base, uint32_t target, uint32_t frame,
                              const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_8209B070 SceneTarget::SceneTarget");
  namespace vo = graphics::video_output;
  Store(base, target + 16, 1024);
  Store(base, target + 28, 0);
  Store(base, target + 20, 576);
  const uint32_t local = frame + 80;
  Store(base, local, vo::LockDevice(base, video_output(0)));
  const uint32_t width = Load(base, target + 16);
  const uint32_t height = Load(base, target + 20);
  Parameters(base, frame + 112, 0);
  Parameters(base, frame + 96, 0);
  Store(base, target + 4,
        graphics::CreateSurface(base, width, height, kColorFormat, 1, frame + 112));
  Store(base, frame + 96, graphics::SurfaceEdramTiles(width, height, kTextureFormat, 1));
  Store(base, frame + 100, 0);
  Store(base, target + 0,
        graphics::CreateSurface(base, width, height, kDepthFormat, 1, frame + 96));
  Store(base, target + 24,
        graphics::CreateTexture(base, 1280, 720, 1, 1, 0, kTextureFormat, 3));
  const uint32_t tiling = Load(base, kDisplayTiling);
  const uint32_t type = Load(base, tiling + 8);
  Parameters(base, frame + 112, 0);
  Parameters(base, frame + 96, 0);
  TilingLargestTile(base, tiling, frame + 88);
  const uint32_t tile_width = Load(base, frame + 88);
  const uint32_t tile_height = Load(base, frame + 92);
  uint32_t w = 0;
  uint32_t h = 0;
  if (type == 0) {
    h = (tile_height + 15) & ~15u;
    w = (tile_width + 79) / 80 * 80;
  } else if (type == 1) {
    h = (tile_height + 7) & ~7u;
    w = (tile_width + 79) / 80 * 80;
  } else if (type == 2) {
    h = (tile_height + 7) & ~7u;
    w = (tile_width + 39) / 40 * 40;
  }
  h = (h + 31) & ~31u;
  w = (w + 31) & ~31u;
  const int32_t multisample = int32_t(type);
  Store(base, target + 12,
        graphics::CreateSurface(base, w, h, kColorFormat, multisample, frame + 112));
  Store(base, frame + 96, graphics::SurfaceEdramTiles(w, h, kColorFormat, multisample));
  Store(base, frame + 100, 0);
  Store(base, target + 8,
        graphics::CreateSurface(base, w, h, kDepthFormat, multisample, frame + 96));
  vo::UnlockDevice(base, video_output(1), local);
  return target;
}

std::optional<uint32_t> SceneTargetCreate(uint8_t* base, uint32_t interface, uint32_t frame,
                                          const VideoOutputCall& video_output) {
  Store(base, kSceneTargetApp, interface);
  const auto block = memory::crt::OperatorNew(base, 32);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8209B378 SceneTarget::Create");
  const uint32_t target =
      *block != 0 ? SceneTargetConstruct(base, *block, frame, video_output) : 0;
  Store(base, kSceneTarget, target);
  return target;
}

}  // namespace app_start_up
