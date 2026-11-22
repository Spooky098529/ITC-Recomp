// The scene target's set-up natively (start_up.h,
// docs/functions/app-start-up.md): its surfaces and texture made with the
// graphics resources' plain C++ under the device lock.

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "graphics/resources.h"
#include "graphics/video_output.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/app_start_up.h"
#include "schema/scene_target.h"

namespace app_start_up {

namespace {

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
  const schema::Tiling block(base, tiling);
  const schema::TilingTileSize largest(base, out);
  largest.set_width(0);
  largest.set_height(0);
  const uint32_t count = block.tile_count();
  for (uint32_t i = 0; i < count; i++) {
    const schema::TilingTile tile(base, block.tiles_address() + 16 * i);
    const uint32_t width = tile.right() - tile.left();
    const uint32_t height = tile.bottom() - tile.top();
    if (width > largest.width()) {
      largest.set_width(width);
    }
    if (height > largest.height()) {
      largest.set_height(height);
    }
  }
}

uint32_t SceneTargetConstruct(uint8_t* base, uint32_t target, uint32_t frame,
                              const VideoOutputCall& video_output) {
  ITC_NATIVE_RUNNING("sub_8209B070 SceneTarget::SceneTarget");
  namespace vo = graphics::video_output;
  const schema::SceneTarget self(base, target);
  self.set_width(1024);
  self.set_mode(0);
  self.set_height(576);
  const uint32_t local = frame + 80;
  Store(base, local, vo::LockDevice(base, video_output(0)));
  const uint32_t width = self.width();
  const uint32_t height = self.height();
  Parameters(base, frame + 112, 0);
  Parameters(base, frame + 96, 0);
  self.set_color(graphics::CreateSurface(base, width, height, kColorFormat, 1, frame + 112));
  Store(base, frame + 96, graphics::SurfaceEdramTiles(width, height, kTextureFormat, 1));
  Store(base, frame + 100, 0);
  self.set_depth(graphics::CreateSurface(base, width, height, kDepthFormat, 1, frame + 96));
  self.set_texture(graphics::CreateTexture(base, 1280, 720, 1, 1, 0, kTextureFormat, 3));
  const auto tiling = schema::AppStartUpGlobals::display_tiling(base);
  const uint32_t type = tiling.multisample_type();
  Parameters(base, frame + 112, 0);
  Parameters(base, frame + 96, 0);
  TilingLargestTile(base, tiling.address, frame + 88);
  const schema::TilingTileSize largest(base, frame + 88);
  const uint32_t tile_width = largest.width();
  const uint32_t tile_height = largest.height();
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
  self.set_tile_color(
      graphics::CreateSurface(base, w, h, kColorFormat, multisample, frame + 112));
  Store(base, frame + 96, graphics::SurfaceEdramTiles(w, h, kColorFormat, multisample));
  Store(base, frame + 100, 0);
  self.set_tile_depth(
      graphics::CreateSurface(base, w, h, kDepthFormat, multisample, frame + 96));
  vo::UnlockDevice(base, video_output(1), local);
  return target;
}

std::optional<uint32_t> SceneTargetCreate(uint8_t* base, uint32_t interface, uint32_t frame,
                                          const VideoOutputCall& video_output) {
  schema::AppStartUpGlobals::set_scene_target_app(base, interface);
  const auto block = memory::crt::OperatorNew(base, 32);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8209B378 SceneTarget::Create");
  const uint32_t target =
      *block != 0 ? SceneTargetConstruct(base, *block, frame, video_output) : 0;
  schema::AppStartUpGlobals::set_scene_target(base, target);
  return target;
}

}  // namespace app_start_up
