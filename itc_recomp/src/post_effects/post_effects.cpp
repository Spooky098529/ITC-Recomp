// The post effects natively (post_effects.h, docs/functions/post-effects.md).

#include "post_effects/post_effects.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "command_list/command_list.h"
#include "graphics/resources.h"
#include "graphics/video_output.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/car_draw.h"
#include "schema/command_list.h"
#include "schema/course_scenery.h"
#include "schema/graphics_resources.h"
#include "schema/post_effects.h"
#include "schema/race_update.h"
#include "schema/screen_objects.h"

#include <bit>
#include <cmath>

namespace post_effects {

namespace {

using G = schema::PostEffectsGlobals;
using schema::PostEffect;

// The single-precision constants the originals load (lfs): read from the
// image, as they do.
double Constant(uint32_t address) {
  return double(guest::LoadFloat(address));
}

void StoreSingle(uint32_t address, double value) {
  guest::StoreFloat(address, float(value));
}

}  // namespace

uint32_t Get(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82133AE8 PostEffects::Get");
  return guest::LoadU32(G::effects_address() + (index << 2));
}

void MarkUpdated() {
  ITC_NATIVE_RUNNING("sub_82133A08 PostEffects::MarkUpdated");
  for (uint32_t i = 0; i < kCount; i++) {
    const uint32_t effect = guest::LoadU32(G::effects_address() + 4 * i);
    if (effect != 0) {
      PostEffect(effect).set_updated(1);
    }
  }
}

void ToggleNoHistory() {
  ITC_NATIVE_RUNNING("sub_82133A38 PostEffects::ToggleNoHistory");
  for (uint32_t i = 0; i < kCount; i++) {
    const uint32_t effect = guest::LoadU32(G::effects_address() + 4 * i);
    if (effect != 0) {
      const PostEffect e(effect);
      e.set_no_history(e.no_history() == 0 ? 1 : 0);
    }
  }
}

void ToggleScreenObjectHidden(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8212EDE0 ScreenObject::ToggleHidden");
  const schema::ScreenObject screen(object);
  screen.set_hidden(screen.hidden() == 0 ? 1 : 0);
}

uint32_t ToggleRaceEffects(uint32_t race) {
  ITC_NATIVE_RUNNING("sub_8215B690 Race::ToggleEffects");
  const schema::RaceScene scene_object(race);
  uint32_t r3 = 0;
  // The race's screen object, then split screen's two (+31152 to +31160).
  for (uint32_t i = 0; i < 3; i++) {
    r3 = i == 0   ? scene_object.screen_object().address
         : i == 1 ? scene_object.screen_object_first().address
                  : scene_object.screen_object_second().address;
    if (r3 != 0) {
      ToggleScreenObjectHidden(r3);
    }
  }
  ToggleNoHistory();
  const schema::Scenery scenery = G::scenery<schema::Scenery>();
  scenery.set_mask(scenery.mask() ^ 0x4000u);
  const schema::CarScene scene = G::scene<schema::CarScene>();
  scene.set_blink_timers_held(scene.blink_timers_held() == 0 ? 1 : 0);
  return r3;
}

uint32_t DestroyRace(const std::function<uint32_t(uint32_t, uint32_t)>& destroy) {
  ITC_NATIVE_RUNNING("sub_82133A70 PostEffects::DestroyRace");
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < kCount; i++) {
    const uint32_t at = G::effects_address() + 4 * i;
    r3 = guest::LoadU32(at);
    if (r3 == 0 || (PostEffect(r3).flags() & 4) != 0) {
      continue;
    }
    const uint32_t function = PostEffect(r3).vtable().destroy();
    r3 = destroy(function, r3);
    guest::StoreU32(at, 0);
  }
  return r3;
}

void GlowSet(uint32_t glow, uint32_t offset, double value) {
  switch (offset) {
    case 8436: ITC_NATIVE_RUNNING("sub_82136EF8 Glow::Set8436"); break;
    case 8440: ITC_NATIVE_RUNNING("sub_82136F00 Glow::Set8440"); break;
    case 8444: ITC_NATIVE_RUNNING("sub_82136F08 Glow::Set8444"); break;
    case 8448: ITC_NATIVE_RUNNING("sub_82136F10 Glow::Set8448"); break;
    case 8452: ITC_NATIVE_RUNNING("sub_82136F18 Glow::SetStrength"); break;
    case 8456: ITC_NATIVE_RUNNING("sub_82136F20 Glow::Set8456"); break;
    case 8460: ITC_NATIVE_RUNNING("sub_82136F28 Glow::SetHistoryWeight"); break;
    case 8464: ITC_NATIVE_RUNNING("sub_82136F30 Glow::Set8464"); break;
    case 8468: ITC_NATIVE_RUNNING("sub_82136F38 Glow::Set8468"); break;
    case 8472: ITC_NATIVE_RUNNING("sub_82136F40 Glow::Set8472"); break;
    case 8476: ITC_NATIVE_RUNNING("sub_82136F48 Glow::Set8476"); break;
    default: break;
  }
  StoreSingle(glow + offset, value);
}

void GlowSetFromSpeed(uint32_t glow, double speed,
                      const std::function<void(uint32_t, double)>& set) {
  ITC_NATIVE_RUNNING("sub_82133878 Glow::SetFromSpeed");
  // t = (speed - 120) / 130 (as a multiply), clamped to [0, 1] (a NaN
  // stays).
  const double offset = Constant(0x82002C38);
  const double scale = Constant(0x82008844);
  double t = double(float(double(float(speed - offset)) * scale));
  const double low = Constant(0x82000A34);
  const double one = Constant(0x8200882C);
  if (t < low) {
    t = low;
  } else if (t > one) {
    t = one;
  }
  const schema::Glow g(glow);
  set(g.vtable().c33_x_second_setter(),
      double(float(-std::fma(t, Constant(0x82002A64), -one))));
  set(g.vtable().strength_setter(),
      double(float(std::fma(t, Constant(0x82002574), Constant(0x82008830)))));
  set(g.vtable().history_weight_setter(),
      double(float(std::fma(t, Constant(0x82008840), Constant(0x82008834)))));
}

void ColorFilterSetColor0(uint32_t filter, double r, double g, double b) {
  ITC_NATIVE_RUNNING("sub_821350F8 ColorFilter::SetColor0");
  const schema::ColorFilter f(filter);
  f.set_color0_r(float(r));
  f.set_color0_g(float(g));
  f.set_color0_b(float(b));
  f.set_color0_a(float(Constant(0x82000A34)));
}

void ColorFilterSet(uint32_t filter, uint32_t offset, double value) {
  switch (offset) {
    case 8112: ITC_NATIVE_RUNNING("sub_82135138 ColorFilter::Set8112"); break;
    case 8116: ITC_NATIVE_RUNNING("sub_82135140 ColorFilter::Set8116"); break;
    case 8120: ITC_NATIVE_RUNNING("sub_82135148 ColorFilter::Set8120"); break;
    default: break;
  }
  StoreSingle(filter + offset, value);
}

// --- Recording ---

namespace {

constexpr uint32_t kOne = 0x8200882C;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0

// Whether `slots` more fit on the list: each AllocateSlots' check (the
// slots used then below the capacity); the last is the largest unless the
// count wraps, which is left to the original too.
bool Room(uint32_t list, uint32_t slots) {
  const schema::CommandList commands(list);
  const uint32_t used = commands.used();
  return used + slots < commands.capacity() && used + slots >= used;
}

uint32_t Slot(uint32_t list) {
  return command_list::AllocateSlots(list, 1);
}

// A slot with [a, b, c].
uint32_t Slot3(uint32_t list, uint32_t a, uint32_t b, uint32_t c) {
  const uint32_t s = Slot(list);
  guest::StoreU32(s + 0, a);
  guest::StoreU32(s + 4, b);
  guest::StoreU32(s + 8, c);
  return s;
}

// A slot with [a, b] (+8 left as it was).
uint32_t Slot2(uint32_t list, uint32_t a, uint32_t b) {
  const uint32_t s = Slot(list);
  guest::StoreU32(s + 0, a);
  guest::StoreU32(s + 4, b);
  return s;
}

// Command 1 with eight render states, [state, value] each.
void RenderStates(uint32_t list, const uint32_t (&states)[8][2]) {
  Slot3(list, 1, 8, 0);
  for (const auto& state : states) {
    Slot2(list, state[0], state[1]);
  }
}

// Command 7's slot and the 16 bytes `read()` gives (four words).
template <typename Read>
uint32_t PixelConstants(uint32_t list, uint32_t reg, uint32_t count, Read read) {
  Slot3(list, 7, count, (reg << 8) | 0x01010000u | count);
  const uint32_t s = Slot(list);
  uint32_t words[4];
  read(words);
  for (int i = 0; i < 4; i++) {
    guest::StoreU32(s + 4 * i, words[i]);
  }
  return s;
}

uint32_t SingleBits(double value) {
  return std::bit_cast<uint32_t>(float(value));
}

}  // namespace

std::optional<uint32_t> RecordBeginStates(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82133340 PostEffect::RecordBeginStates");
  if (!Room(list, 24)) {
    return std::nullopt;
  }
  Slot3(list, 34, 1, 0);
  Slot(list);  // command 34's one slot, left as it was
  // Command 2: samplers 0-4, states 0 and 4, = 2.
  Slot3(list, 2, 10, 0);
  for (uint32_t sampler = 0; sampler < 5; sampler++) {
    Slot3(list, sampler, 0, 2);
    Slot3(list, sampler, 4, 2);
  }
  RenderStates(list,
               {{96, 0}, {40, 0}, {304, 0}, {324, 1}, {308, 0}, {312, 0}, {316, 0}, {320, 0}});
  Slot3(list, 3, 1, 0);
  return Slot2(list, 0, 0x10001);
}

std::optional<uint32_t> RecordEndStates(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82133608 PostEffect::RecordEndStates");
  if (!Room(list, 13)) {
    return std::nullopt;
  }
  Slot3(list, 35, 1, 0);
  Slot(list);  // command 35's one slot, left as it was
  RenderStates(list,
               {{60, 0}, {40, 1}, {304, 1}, {324, 0}, {308, 1}, {312, 1}, {316, 1}, {320, 1}});
  Slot3(list, 3, 1, 0);
  return Slot2(list, 0, 0x10001);
}

std::optional<uint32_t> RecordTargets(uint32_t list, uint32_t surface,
                                      uint32_t depth) {
  ITC_NATIVE_RUNNING("sub_82133790 PostEffect::RecordTargets");
  if (!Room(list, 7)) {
    return std::nullopt;
  }
  Slot3(list, 23, 4, 0);
  for (uint32_t target = 0; target < 4; target++) {
    Slot2(list, target, surface);
  }
  Slot3(list, 24, 1, 0);
  const uint32_t s = Slot(list);
  guest::StoreU32(s, depth);
  return s;
}

std::optional<uint32_t> RecordPixelConstants(uint32_t list, uint32_t source,
                                             uint32_t reg, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82133D00 PostEffect::RecordPixelConstants");
  if (!Room(list, 2)) {
    return std::nullopt;
  }
  return PixelConstants(list, reg, count, [&](uint32_t (&words)[4]) {
    for (int i = 0; i < 4; i++) {
      words[i] = guest::LoadU32(source + 4 * i);
    }
  });
}

std::optional<uint32_t> RecordPixelBool(uint32_t list, uint32_t value,
                                        uint32_t reg) {
  ITC_NATIVE_RUNNING("sub_82133D70 PostEffect::RecordPixelBool");
  if (!Room(list, 2)) {
    return std::nullopt;
  }
  Slot3(list, 7, 1, (reg << 8) | 0x01020000u | 1u);
  const uint32_t s = command_list::AllocateBytes(list, 4, false);
  guest::StoreU32(s, value);
  return s;
}

std::optional<uint32_t> RecordTexelSize(uint32_t list, double width,
                                        double height) {
  ITC_NATIVE_RUNNING("sub_82133DD8 PostEffect::RecordTexelSize");
  if (!Room(list, 2)) {
    return std::nullopt;
  }
  const double one = Constant(kOne);
  const double zero = Constant(kZero);
  const uint32_t c32[4] = {SingleBits(double(float(one / width))),
                           SingleBits(double(float(one / height))), SingleBits(zero),
                           SingleBits(zero)};
  return PixelConstants(list, 32, 1, [&](uint32_t (&words)[4]) {
    for (int i = 0; i < 4; i++) {
      words[i] = c32[i];
    }
  });
}

uint32_t AddQuad(uint32_t effect, uint32_t width, uint32_t height, double value,
                 bool at_view) {
  ITC_NATIVE_RUNNING("sub_82133E30 PostEffect::AddQuad");
  const PostEffect e(effect);
  const uint64_t buffer = e.vertex_buffer();
  const uint64_t used = e.vertex_count();
  e.set_vertex_count(uint32_t(used + 3));
  const uint32_t v = uint32_t((buffer * 100 + used) * 40 + e.vertices_address());
  uint32_t x = 0, y = 0;
  if (at_view) {
    x = e.x();
    y = e.y();
  }
  const double one = Constant(kOne);
  const double zero = Constant(kZero);
  // The integers as the original converts them (unsigned 32-bit, through
  // a 64-bit signed conversion), rounded to single.
  const double x0 = double(float(double(int64_t(x))));
  const double y0 = double(float(double(int64_t(y))));
  const double w = double(float(double(int64_t(width))));
  const double h = double(float(double(int64_t(height))));
  const double x1 = double(float(w + x0));
  const double y1 = double(float(h + y0));
  // Each vertex: position, 1, 1, value x3, 1, then its texture coordinates.
  const double vertices[3][10] = {
      {x0, y0, one, one, value, value, value, one, zero, zero},
      {x1, y0, one, one, value, value, value, one, one, zero},
      {x0, y1, one, one, value, value, value, one, zero, one},
  };
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      StoreSingle(v + 40 * i + 4 * j, vertices[i][j]);
    }
  }
  return v;
}

std::optional<uint32_t> RecordResolve(uint32_t effect, uint32_t list,
                                      uint32_t texture, uint32_t flags, uint32_t more_flags,
                                      bool by_view) {
  ITC_NATIVE_RUNNING("sub_82133F58 PostEffect::RecordResolve");
  if (!Room(list, 6)) {
    return std::nullopt;
  }
  Slot3(list, 25, 1, 0);
  const uint32_t item = command_list::AllocateResolve(list, false);
  if (more_flags != 0) {
    flags |= more_flags << 26;
  }
  guest::StoreU32(item + 0, 2);
  if (by_view) {
    guest::StoreU32(item + 0, 3);
    const PostEffect e(effect);
    guest::StoreU32(item + 8, e.x());
    guest::StoreU32(item + 12, e.y());
    guest::StoreU32(item + 16, e.width() + e.x());
    guest::StoreU32(item + 20, e.height() + e.y());
  }
  guest::StoreU32(item + 4, flags);
  guest::StoreU32(item + 24, texture);
  guest::StoreU32(item + 36, 0);
  guest::StoreU32(item + 40, 0);
  guest::StoreU32(item + 52, 0);
  StoreSingle(item + 48, Constant(kZero));
  guest::StoreU8(item + 44, 0);
  guest::StoreU8(item + 45, 0);
  guest::StoreU8(item + 46, 0);
  guest::StoreU8(item + 47, 255);
  guest::StoreU32(item + 56, 0x18280186);
  guest::StoreU32(item + 60, more_flags);
  guest::StoreU32(item + 64, 0x1A220197);
  return item;
}

// --- The glare's definition ---

namespace {

// An lfs of a constant, as the value a view's setter stores (stfs): its
// bits through a double, as the originals copy them.
float Single(uint32_t from) {
  return float(Constant(from));
}
// A single read through a view, copied the same way (through a double).
float Single(float value) {
  return float(double(value));
}

}  // namespace

void GlareDefConstruct(uint32_t glare) {
  ITC_NATIVE_RUNNING("sub_8212ED78 GlareDef::GlareDef");
  const schema::GlareDef g(glare);
  const schema::StarDef star(g.star_address());
  for (uint32_t i = 0; i < 8; i++) {
    schema::GlowStarColor(star.colors_address() + 16 * i).set_a(Single(0x82000A30));
  }
  star.set_inclination(Single(kZero));
  star.set_line_count(0);
  star.set_rotation(0);
  g.set_glare_luminance(Single(kZero));
  g.set_bloom_luminance(Single(kZero));
  g.set_ghost_luminance(Single(kZero));
  g.set_star_luminance(Single(kZero));
  g.set_star_inclination(Single(kZero));
  g.set_chromatic_aberration_luminance(Single(kZero));
  g.set_afterimage_sensitivity(Single(kZero));
  g.set_afterimage_ratio(Single(kZero));
  g.set_afterimage_luminance(Single(kZero));
}

uint32_t GlareDefInit(uint32_t glare, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_8212EC90 GlareDef::Init");
  const schema::GlareTableEntry entry(G::glare_table_address() + type * 44u);
  const schema::GlareDef g(glare);
  // The entry's floats as they are, the star type left out.
  g.set_glare_luminance(Single(entry.glare_luminance()));
  g.set_bloom_luminance(Single(entry.bloom_luminance()));
  g.set_ghost_luminance(Single(entry.ghost_luminance()));
  g.set_ghost_distortion(Single(entry.ghost_distortion()));
  g.set_star_luminance(Single(entry.star_luminance()));
  g.set_star_inclination(Single(entry.star_inclination()));
  g.set_chromatic_aberration_luminance(Single(entry.chromatic_aberration_luminance()));
  g.set_afterimage_sensitivity(Single(entry.afterimage_sensitivity()));
  g.set_afterimage_ratio(Single(entry.afterimage_ratio()));
  g.set_afterimage_luminance(Single(entry.afterimage_luminance()));
  const schema::StarTableEntry star(G::star_table_address() + entry.star_type() * 28u);
  StarDefInit(g.star_address(), star.line_count(), star.passes(),
              double(star.sample_length()), double(star.attenuation()),
              double(star.inclination()), star.rotation());
  return 0;
}

uint32_t StarDefInit(uint32_t star, uint32_t lines, uint32_t passes,
                     double sample_length, double attenuation, double inclination,
                     uint32_t rotation) {
  ITC_NATIVE_RUNNING("sub_8212EA98 StarDef::Init");
  const schema::StarDef s(star);
  s.set_rotation(rotation);
  s.set_inclination(float(inclination));
  s.set_line_count(lines);
  // 360 / lines (as a single), times pi / 180.
  const double count = double(float(double(int64_t(uint64_t(lines)))));
  const double turn = double(float(Constant(0x82002244) / count));
  const double step = double(float(turn * Constant(0x82002948)));
  // Its count read again each time, as the original does (the lines' words
  // never reach +0).
  for (uint32_t i = 0; i < s.line_count(); i++) {
    const schema::StarLine line(s.lines_address() + 16 * i);
    line.set_sample_length(float(sample_length));
    line.set_attenuation(float(attenuation));
    line.set_passes(passes);
    const double index = double(float(double(int64_t(uint64_t(i)))));
    line.set_angle(float(double(float(index * step))));
  }
  // The eight colors (r, g, b, 0), from the image's constants.
  constexpr uint32_t k05 = 0x82001BE4, k08 = 0x82001070, k03 = 0x8200296C, k10 = 0x82000A30,
                     k02 = 0x82002208, k06 = 0x82002A14, k07 = 0x82002A3C;
  const uint32_t colors[8][3] = {{k05, k05, k05}, {k08, k03, k03}, {k10, k02, k02},
                                 {k05, k02, k06}, {k02, k02, k10}, {k02, k03, k07},
                                 {k02, k06, k02}, {k03, k05, k03}};
  for (uint32_t i = 0; i < 8; i++) {
    const schema::GlowStarColor color(s.colors_address() + 16 * i);
    color.set_r(Single(colors[i][0]));
    color.set_g(Single(colors[i][1]));
    color.set_b(Single(colors[i][2]));
    color.set_a(Single(kZero));
  }
  return 0;
}

// --- The render textures and surfaces ---

namespace {

// Whether operator delete's plain C++ frees `block` (0, or a block of the
// layer's).
bool Deletable(uint32_t block) {
  return block == 0 || memory::IsHeapBlock(block);
}

}  // namespace

std::optional<uint32_t> RenderTextureRelease(uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82133070 RenderTexture::Release");
  const schema::RenderTexture t(entry);
  const uint32_t users = t.users();
  if (users == 0) {
    return entry;
  }
  if (users - 1 != 0) {
    t.set_users(users - 1);
    return entry;
  }
  if (!Deletable(t.header().address)) {
    return std::nullopt;
  }
  t.set_users(0);
  memory::FreePhysical(t.memory());
  const uint32_t header = t.header().address;
  memory::crt::OperatorDelete(header);
  t.set_memory(0);
  t.set_header(0u);
  return header;
}

std::optional<uint32_t> RenderTexturesRelease(uint32_t at) {
  ITC_NATIVE_RUNNING("sub_821332F0 RenderTextures::Release");
  const schema::RenderTextureRef ref(at);
  const schema::RenderTexture t = ref.entry();
  const uint32_t entry = t.address;
  // What the frees would leave to the original: checked first, as nothing
  // is done then. (RenderTexture::Release writes only +20, +24 and +28.)
  if (entry == 0 || (t.users() == 1 && !Deletable(t.header().address))) {
    return std::nullopt;
  }
  if (int32_t(t.id()) == 20 && !Deletable(entry)) {
    return std::nullopt;
  }
  RenderTextureRelease(entry);
  const schema::RenderTexture own = ref.entry();
  if (int32_t(own.id()) == 20) {
    memory::crt::OperatorDelete(own.address);
    ref.set_entry(0u);
  }
  return own.address;
}

uint32_t ReleaseSurface(uint32_t frame, uint32_t at, const SurfaceCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82133C60 PostEffect::ReleaseSurface");
  namespace vo = graphics::video_output;
  const uint32_t local = frame + 80;
  guest::StoreU32(local, vo::LockDevice(calls.video_output(0)));
  calls.release(guest::LoadU32(at));
  guest::StoreU32(at, 0);
  const uint32_t video_output = calls.video_output(1);
  vo::UnlockDevice(video_output, local);
  return video_output;
}

std::optional<uint32_t> TargetFilterConstruct(uint32_t frame, uint32_t filter,
                                              uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82135AA8 TargetFilter::TargetFilter");
  // memcpy's override records destinations in the physical views: those
  // copies are the original's.
  const schema::TargetFilter f(filter);
  if (f.kind_address() >= 0xA0000000u) {
    return std::nullopt;
  }
  f.set_vtable(PostEffect::kVtable);  // the base vtable
  if (!c_runtime::Memcpy(f.kind_address(), entry, 36)) {
    return std::nullopt;
  }
  f.set_reset(1);
  f.set_vtable(schema::TargetFilter::kVtable);
  f.set_no_history(0);
  guest::StoreU32(frame + 80, 0);
  guest::StoreU32(frame + 84, 0);
  f.set_updated(0);
  f.set_skipped(0);
  f.set_vertex_count(0);
  f.set_vertex_buffer(0);
  f.set_surface(0u);
  const uint32_t height = f.height();
  const uint32_t width = f.width();
  guest::StoreU32(frame + 88, 0);
  f.set_surface(graphics::CreateSurface(width, height, 0x18280186, 0, frame + 80));
  return filter;
}

uint32_t InitEdramBase() {
  ITC_NATIVE_RUNNING("sub_825D99D0 PostEffects::InitEdramBase");
  const uint32_t tiles = graphics::SurfaceEdramTiles(1024, 576, 0x1A2201BF, 1);
  G::set_edram_base(tiles);
  return tiles;
}

// --- The destructors ---

uint32_t GlowDestroy(uint32_t glow, const DestroyCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82136E30 Glow::~Glow");
  const schema::Glow g(glow);
  g.set_vtable(schema::Glow::kVtable);
  for (uint32_t at = g.surface_1_address(); at >= g.quarter_surface_address(); at -= 4) {
    calls.release_surface(at);
  }
  uint32_t r3 = 0;
  for (uint32_t at = g.history_address(); at >= g.frame_texture_address(); at -= 4) {
    r3 = calls.release_texture(at);
  }
  g.set_vtable(PostEffect::kVtable);
  return r3;
}

uint32_t ColorFilterDestroy(uint32_t filter, const DestroyCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821351A8 ColorFilter::~ColorFilter");
  const schema::ColorFilter f(filter);
  f.set_vtable(schema::ColorFilter::kVtable);
  const uint32_t r3 = calls.release_texture(f.frame_texture_address());
  f.set_vtable(PostEffect::kVtable);
  return r3;
}

uint32_t MotionBlurDestroy(uint32_t blur, const DestroyCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82136390 MotionBlur::~MotionBlur");
  const schema::MotionBlur b(blur);
  b.set_vtable(schema::MotionBlur::kVtable);
  calls.release_surface(b.depth_surface_address());
  calls.release_surface(b.velocity_surface_address());
  calls.release_texture(b.frame_texture_address());
  const uint32_t r3 = calls.release_texture(b.velocity_texture_address());
  b.set_vtable(PostEffect::kVtable);
  return r3;
}

std::optional<uint32_t> DeleteEffect(EffectKind kind, uint32_t effect,
                                     uint32_t flags, const DestroyCalls& calls) {
  switch (kind) {
    case EffectKind::kGlow: ITC_NATIVE_RUNNING("sub_821389C8 Glow::~Glow (deleting)"); break;
    case EffectKind::kColorFilter:
      ITC_NATIVE_RUNNING("sub_82138570 ColorFilter::~ColorFilter (deleting)");
      break;
    case EffectKind::kMotionBlur:
      ITC_NATIVE_RUNNING("sub_821385C0 MotionBlur::~MotionBlur (deleting)");
      break;
  }
  const bool del = (flags & 1) != 0;
  if (del && !Deletable(effect)) {
    return std::nullopt;
  }
  switch (kind) {
    case EffectKind::kGlow: GlowDestroy(effect, calls); break;
    case EffectKind::kColorFilter: ColorFilterDestroy(effect, calls); break;
    case EffectKind::kMotionBlur: MotionBlurDestroy(effect, calls); break;
  }
  if (del) {
    memory::crt::OperatorDelete(effect);
  }
  return effect;
}

}  // namespace post_effects
