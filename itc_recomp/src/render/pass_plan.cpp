// The frame's pass plan (render/pass_plan.h): the scan, from the command
// handlers' state.

#include "render/pass_plan.h"

#include "guest.h"
#include "render/commands.h"

#include <algorithm>

namespace render {

namespace {

constexpr uint32_t kSavedTargets = 0x82970BC0;  // as commands.cpp's

}  // namespace

uint32_t TargetFormatClass(uint32_t format) {
  // As passes.cpp's ColorFormat groups them.
  switch (format) {
    case 0:   // 8_8_8_8
    case 1:   // 8_8_8_8_GAMMA
      return 0;
    case 2:   // 2_10_10_10
    case 10:  // 2_10_10_10_AS_10_10_10_10
      return 2;
    case 3:   // 2_10_10_10_FLOAT
    case 5:   // 16_16_16_16
    case 7:   // 16_16_16_16_FLOAT
    case 12:  // 2_10_10_10_FLOAT_AS_16_16_16_16
      return 3;
    case 4:  // 16_16
    case 6:  // 16_16_FLOAT
      return 4;
    default:
      return format;  // 32_FLOAT, 32_32_FLOAT, and ones not drawn
  }
}

TargetId SurfaceTarget(uint8_t* base, uint32_t surface, bool depth, bool tiled) {
  const uint32_t surface_info = guest::LoadU32(base, surface + 24);
  const uint32_t info = guest::LoadU32(base, surface + 28);
  TargetId id;
  id.depth = depth;
  id.tiled = tiled;
  id.edram_base = info & 0xFFF;
  id.pitch = surface_info & 0x3FFF;
  id.msaa = (surface_info >> 16) & 3;
  id.format_class = depth ? 0 : TargetFormatClass((info >> 16) & 0xF);
  return id;
}

const char* ResolveKindName(ResolveKind kind) {
  switch (kind) {
    case ResolveKind::kSkipped:
      return "skipped";
    case ResolveKind::kDrawnInto:
      return "drawn into";
    case ResolveKind::kMultisample:
      return "multisample resolve";
    case ResolveKind::kCopy:
      return "copy";
  }
  return "?";
}

void Commands::BeginPlan() {
  Planning& p = planning_;
  for (uint32_t t = 0; t < kTargets; t++) {
    p.targets[t] = state_.targets[t];
    p.saved_targets[t] = saved_targets_[t];
  }
  p.depth = state_.depth;
  p.saved_depth = saved_depth_;
  p.tiling = tiling_;
  p.tile_rects = tile_rects_;
  p.tile_count = tile_count_;
  p.segments.clear();
  plan_.complete = false;
  plan_.resolves.clear();
}

void Commands::PlanCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t count,
                           uint32_t) {
  Planning& p = planning_;
  auto u32 = [&](uint32_t address) { return guest::LoadU32(base, address); };
  switch (id) {
    case 23:
      for (uint32_t i = 0; i < count; i++) {
        if (const uint32_t index = u32(payload + 16 * i); index < kTargets) {
          p.targets[index].address = u32(payload + 16 * i + 4);
        }
      }
      break;
    case 24:
      for (uint32_t i = 0; i < count; i++) {
        p.depth.address = u32(payload + 16 * i);
      }
      break;
    case 25:
      for (uint32_t i = 0; i < count; i++) {
        // The item as Commands::Resolve reads it.
        const uint32_t item = payload + 68 * i, fields = u32(item);
        const int32_t rect[4] = {int32_t(u32(item + 8)), int32_t(u32(item + 12)),
                                 int32_t(u32(item + 16)), int32_t(u32(item + 20))};
        const int32_t point[2] = {int32_t(u32(item + 28)), int32_t(u32(item + 32))};
        PlanResolve(base, u32(item + 4), (fields & 2) ? u32(item + 24) : 0, fields & 1, rect,
                    fields & 4, point, u32(item + 36) || u32(item + 40));
      }
      break;
    case 29:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t mask = u32(payload + 4 * i), saved = u32(kSavedTargets);
        for (uint32_t t = 0; t < kTargets; t++) {
          if (mask & (1u << t)) {
            p.targets[t].address = u32(saved + 4 * t);
          }
        }
        if (mask & 0x80) {
          p.depth.address = u32(saved + 16);
        }
      }
      break;
    case 32:
      if (count) {
        p.tile_count = u32(payload + 4);
        p.tile_rects = u32(payload + 8);
        p.tiling = true;
      }
      break;
    case 33:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t flags = u32(payload), destination = u32(payload + 4);
        for (uint32_t t = 0; p.tile_rects && t < p.tile_count; t++) {
          const uint32_t at = p.tile_rects + 16 * t;
          const int32_t rect[4] = {int32_t(u32(at)), int32_t(u32(at + 4)), int32_t(u32(at + 8)),
                                   int32_t(u32(at + 12))};
          const int32_t point[2] = {rect[0], rect[1]};
          PlanResolve(base, flags, destination, true, rect, true, point, false);
        }
        p.tiling = false;
      }
      break;
    case 34:
      for (uint32_t t = 0; t < kTargets; t++) {
        p.saved_targets[t] = p.targets[t];
      }
      p.saved_depth = p.depth;
      break;
    case 35:
      for (uint32_t t = 0; t < kTargets; t++) {
        p.targets[t] = p.saved_targets[t];
      }
      p.depth = p.saved_depth;
      break;
    default:
      break;
  }
}

void Commands::PlanResolve(uint8_t* base, uint32_t flags, uint32_t destination, bool has_rect,
                           const int32_t rect[4], bool has_point, const int32_t point[2],
                           bool level_or_slice) {
  Planning& p = planning_;
  const bool from_depth = flags & 4;
  const uint32_t source = from_depth ? p.depth.address : p.targets[flags & 3].address;
  PlannedResolve& r = plan_.resolves.emplace_back();
  r.destination = destination;
  r.flags = flags;
  if (!source || !destination || level_or_slice) {
    return;  // skipped, as the renderer skips it
  }
  r.source = SurfaceTarget(base, source, from_depth, p.tiling);
  auto it = std::find_if(p.segments.begin(), p.segments.end(),
                         [&](const auto& s) { return s.first == r.source; });
  if (it == p.segments.end()) {
    p.segments.emplace_back(r.source, 0);
    it = p.segments.end() - 1;
  }
  r.segment = it->second++;
  // The destination's size, from its fetch constant's word 2 (2D).
  const uint32_t size = guest::LoadU32(base, destination + 28 + 8);
  const int32_t width = int32_t(size & 0x1FFF) + 1, height = int32_t((size >> 13) & 0x1FFF) + 1;
  const bool whole = (!has_rect || (rect[0] == 0 && rect[1] == 0 && rect[2] == width &&
                                    rect[3] == height)) &&
                     (!has_point || (point[0] == 0 && point[1] == 0));
  if (r.source.msaa) {
    r.kind = ResolveKind::kMultisample;
  } else if (r.source.tiled || !whole) {
    r.kind = ResolveKind::kCopy;
  } else {
    r.kind = ResolveKind::kDrawnInto;
  }
}

void Commands::EndPlan(bool complete) {
  plan_.complete = complete;
  if (!complete) {
    for (PlannedResolve& r : plan_.resolves) {
      if (r.kind != ResolveKind::kSkipped) {
        r.kind = ResolveKind::kCopy;
      }
    }
  }
  if (backend_) {
    backend_->PassPlanned(plan_);
  }
}

}  // namespace render
