// The model's load (see model_load.h), load for load and store for store as
// the recompiled functions.

#include "models/model_load.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/xpr.h"
#include "native/buffers.h"
#include "scenery_lists/scenery_lists.h"
#include "shader_objects/shader_objects.h"
#include "strings_and_font/steps.h"

#include <cstdlib>
#include <cstring>
#include <string>

namespace models {

namespace {

using namespace car_physics::ops;
namespace ma = model_animation;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kVersion = 0x00150000;
constexpr uint32_t kShapeBytes = 544;
constexpr uint32_t kMaterialBytes = 512;
// The bits the parse sets in each buffer header's +28 (fetch constant, 1 in
// bits 28-29).
constexpr uint32_t kFetchBits = 0x30000000;
constexpr uint32_t kFetchValue = 0x10000000;
constexpr uint32_t kTextureProviderVtable = 0x8206CCB4;
constexpr uint32_t kTextureProviderSet = 0x82594718;

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Model::Load: {} refused after its checks", what);
    std::abort();
  }
}

std::string Message(uint8_t* base, uint32_t text) {
  const char* host = reinterpret_cast<const char*>(REX_RAW_ADDR(text));
  std::string message(host, strnlen(host, 256));
  while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) {
    message.pop_back();
  }
  return message;
}

uint32_t WithFetchBits(uint32_t word) { return (word & ~kFetchBits) | kFetchValue; }

}  // namespace

bool LoadFits(uint8_t* base, uint32_t file) {
  if (!native::Buffers_NativeMemcpy()) {
    return false;
  }
  const uint32_t section = Load(base, file + 72) + file;
  const uint32_t count = Load(base, section);
  uint32_t pair = section + 32;
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t vertex = pair + 32;
    const uint32_t pixel = Load(base, pair + 4) + vertex;
    if (!c_runtime::XMemCpyBytes(base, vertex, Load(base, vertex + 4)) ||
        !c_runtime::XMemCpyBytes(base, pixel, Load(base, pixel + 4))) {
      return false;
    }
    pair = Load(base, pair + 8) + pixel;
  }
  return true;
}

void MeshParse(uint8_t* base, uint32_t model, uint32_t file, const LoadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_825963F0 Mesh::Parse");
  const uint32_t version = Load(base, file);
  if (version != kVersion) {
    ITC_LOG_WARN("Mesh::Parse: the game's debug message: {} ({:#x})", Message(base, 0x8206CDE8),
                 version);
  }
  shader_objects::CreateCalls shader_calls;
  shader_calls.init = calls.init;
  shader_calls.user = calls.user;
  Require(shader_objects::LoadShaders(base, model, Load(base, file + 72) + file, shader_calls),
          "Mesh::LoadShaders");
  // The header block: the file's bytes before the vertex data.
  Store(base, model + 16, AllocVirtual(base, Load(base, file + 64)));
  Require(native::Buffers_Memcpy(base, Load(base, model + 16), file, Load(base, file + 64),
                                 0x82596448),
          "memcpy");
  const uint32_t header = Load(base, model + 16);
  Store(base, model + 28, header);
  Store(base, model + 32, Load(base, header + 20) + header);
  Store(base, model + 36, Load(base, header + 24) + header);
  Store(base, model + 40, Load(base, header + 28) + header);
  Store(base, model + 44, Load(base, header + 32) + header);
  Store(base, model + 404, Load(base, header + 4));
  Store(base, model + 408, Load(base, header + 8));
  const uint32_t shapes = Load(base, header + 12);
  Store(base, model + 412, shapes);
  Store(base, model + 416, Load(base, header + 16));
  // The shapes' pointers made pointers into the header block.
  if (int32_t(shapes) > 0) {
    for (int32_t i = 0; i < int32_t(Load(base, model + 412)); i++) {
      const uint32_t offset = kShapeBytes * uint32_t(i);
      for (const uint32_t field : {396u, 408u, 400u, 404u}) {
        const uint32_t shape = Load(base, model + 40) + offset;
        Store(base, shape + field, Load(base, shape + field) + Load(base, model + 16));
      }
      const uint32_t shape = Load(base, model + 40) + offset;
      const uint32_t weights = Load(base, shape + 408);
      const uint32_t w8 = Load(base, weights + 8);
      const uint32_t w12 = Load(base, weights + 12);
      const uint32_t w16 = Load(base, weights + 16);
      Store(base, weights + 8, Load(base, model + 16) + w8);
      Store(base, weights + 12, Load(base, model + 16) + w12);
      Store(base, weights + 16, Load(base, model + 16) + w16);
      const uint32_t f316 = Load(base, shape + 316);
      const uint32_t f348 = Load(base, shape + 348);
      const uint32_t f380 = Load(base, shape + 380);
      const uint32_t skinned = Load(base, shape + 428);
      Store(base, shape + 316, WithFetchBits(f316));
      Store(base, shape + 348, WithFetchBits(f348));
      Store(base, shape + 380, WithFetchBits(f380));
      if (skinned == 1) {
        Store(base, shape + 348, WithFetchBits(Load(base, shape + 348)));
        Store(base, shape + 380, WithFetchBits(Load(base, shape + 380)));
      }
    }
  }
  // The materials' pointers.
  if (int32_t(Load(base, model + 412)) > 0) {
    for (int32_t i = 0; i < int32_t(Load(base, model + 412)); i++) {
      const uint32_t offset = kShapeBytes * uint32_t(i);
      const uint32_t materials = Load(base, Load(base, model + 40) + offset + 396);
      if (int32_t(Load(base, materials)) <= 0) {
        continue;
      }
      for (int32_t j = 0;;) {
        const uint32_t entry =
            Load(base, Load(base, model + 40) + offset + 396) + kMaterialBytes * uint32_t(j) + 16;
        j++;
        Store(base, entry + 384, Load(base, model + 16) + Load(base, entry + 384));
        const uint32_t again = Load(base, Load(base, model + 40) + offset + 396);
        if (!(j < int32_t(Load(base, again)))) {
          break;
        }
      }
    }
  }
  // The skeleton and the track sets, with the header block's sections.
  auto section = [&](uint32_t field) { return Load(base, file + field); };
  auto call = [&](uint32_t function, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t lr) {
    calls.call(calls.user, function, r3, r4, r5, lr);
  };
  if (Load(base, Load(base, model + 28) + 76) != 0) {
    if (section(36) != 0) {
      const uint32_t at = section(36) + Load(base, model + 16);
      if (!ma::SkeletonBind(base, model + 112, at, 0)) {
        call(0x82599A20, model + 112, at, 0, 0x82596674);
      }
    }
    if (section(40) != 0) {
      ma::SkeletonLoadNodeValues(base, model + 112, Load(base, model + 16) + section(40));
    }
    if (section(48) != 0) {
      const uint32_t at = section(48) + Load(base, model + 16);
      if (!ma::TrackSetSetupFloat(base, model + 324, at)) {
        call(0x825A1BE8, model + 324, at, 0, 0x825966AC);
      }
    }
    if (section(52) != 0) {
      const uint32_t at = Load(base, model + 16) + section(52);
      if (!ma::TrackSetSetupFloat(base, model + 344, at)) {
        call(0x825A1BE8, model + 344, at, 0, 0x825966C8);
      }
    }
    if (section(56) != 0) {
      const uint32_t at = Load(base, model + 16) + section(56);
      if (!ma::TrackSetSetup32(base, model + 364, at)) {
        call(0x825A1218, model + 364, at, 0, 0x825966E4);
      }
    }
    if (section(60) != 0) {
      const uint32_t at = Load(base, model + 16) + section(60);
      if (!ma::TrackSetSetup208(base, model + 384, at)) {
        call(0x825A1A38, model + 384, at, 0, 0x82596700);
      }
    }
  }
  {
    const uint32_t at = section(44) + Load(base, model + 16);
    if (!ma::TrackSetSetupFloat4(base, model + 304, at)) {
      call(0x825A1E08, model + 304, at, 0, 0x82596714);
    }
  }
  if (Load(base, model + 412) == 0) {
    return;
  }
  // The vertex data in physical memory, the shapes' buffers relocated to it.
  const uint32_t vertices = file + section(64);
  const uint32_t data = AllocPhysicalEx(base, section(68), 0, 0xFFFFFFFFu, 4, 0x404);
  Store(base, model + 20, data);
  if (data == 0) {
    ITC_LOG_WARN("Mesh::Parse: the game's debug message: {}", Message(base, 0x8206CDC8));
  }
  Require(native::Buffers_Memcpy(base, Load(base, model + 20), vertices, section(68), 0x8259676C),
          "memcpy");
  if (int32_t(Load(base, model + 412)) <= 0) {
    return;
  }
  for (int32_t i = 0; i < int32_t(Load(base, model + 412)); i++) {
    const uint32_t offset = kShapeBytes * uint32_t(i);
    auto shape = [&] { return Load(base, model + 40) + offset; };
    auto rebase = [&](uint32_t field) {
      Store(base, shape() + field, Load(base, shape() + field) - section(64));
      Store(base, shape() + field, Load(base, shape() + field) + Load(base, model + 20));
    };
    rebase(384);
    RelocateResource(base, shape() + 288, Load(base, model + 20));
    if (Load(base, shape() + 428) != 1) {
      rebase(388);
      RelocateResource(base, shape() + 320, Load(base, model + 20));
      rebase(392);
      RelocateResource(base, shape() + 352, Load(base, model + 20));
    }
    if (int32_t(Load(base, Load(base, shape() + 396))) > 0) {
      for (int32_t j = 0; j < int32_t(Load(base, Load(base, shape() + 396))); j++) {
        RelocateResource(base, Load(base, shape() + 396) + kMaterialBytes * uint32_t(j) + 368,
                         Load(base, model + 20));
      }
    }
  }
}

bool ModelLoad(uint8_t* base, uint32_t model, uint32_t file, uint32_t xpr,
               const LoadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82596920 Model::Load");
  if (!LoadFits(base, file) || !ModelRelease(base, model)) {
    return false;
  }
  MeshParse(base, model, file, calls);
  if (xpr != 0) {
    Require(XprCreate(base, model + kModelXpr, xpr).has_value(), "XPR::Create");
    const uint32_t provider = model + kModelTextures;
    const uint32_t set = Load(base, Load(base, provider) + 16);
    if (set != kTextureProviderSet ||
        !scenery_lists::TextureProviderSet(base, provider, 0, model + kModelXpr)) {
      calls.call(calls.user, set, provider, 0, model + kModelXpr, 0x82596980);
    }
    Store(base, model + 48, provider);
  }
  if (Load(base, model + 412) == 0) {
    return true;
  }
  const double one = Lfs(base, kOne);
  const double scale = Lfs(base, 0x820029D8);
  const double add = Lfs(base, 0x82001BE4);
  const double multiply = Lfs(base, 0x82001BDC);
  const double zero = Lfs(base, kZero);
  // A weight quantized: fmadds, fctidz, its low word reloaded (lwz, so
  // unsigned) and converted back (fcfid, frsp), then scaled.
  auto quantize = [&](double weight) {
    const double fused = Fmadds(weight, multiply, add);
    const uint32_t low = uint32_t(strings_and_font::steps::Fctidz(fused));
    return Fmuls(double(float(double(int64_t(low)))), scale);
  };
  for (uint32_t i = 0; i < Load(base, model + 412); i++) {
    const uint32_t shape = Load(base, model + 40) + kShapeBytes * i;
    if (Load(base, shape + 428) != 1) {
      continue;
    }
    const uint32_t weights = Load(base, shape + 408);
    uint32_t counts = Load(base, weights + 8);
    uint32_t at = Load(base, weights + 12);
    if (Load(base, weights + 0) == 0) {
      continue;
    }
    for (uint32_t v = 0; v < Load(base, weights + 0); v++) {
      double sum0 = zero, sum1 = zero, sum2 = zero;
      if (Load(base, counts) - 1 != 0) {
        uint32_t k = 0;
        do {
          const double a = Lfs(base, at + 0);
          const double c = Lfs(base, at + 8);
          const double b = Lfs(base, at + 4);
          const double q0 = quantize(a);
          Stfs(base, at + 0, q0);
          sum0 = Fadds(q0, sum0);
          const double q1 = quantize(b);
          Stfs(base, at + 4, q1);
          sum1 = Fadds(q1, sum1);
          const double q2 = quantize(c);
          Stfs(base, at + 8, q2);
          at += 16;
          k++;
          sum2 = Fadds(q2, sum2);
        } while (k < Load(base, counts) - 1);
      }
      Stfs(base, at + 0, Fsubs(one, sum0));
      Stfs(base, at + 4, Fsubs(one, sum1));
      Stfs(base, at + 8, Fsubs(one, sum2));
      at += 16;
      counts += 4;
    }
  }
  return true;
}

}  // namespace models
