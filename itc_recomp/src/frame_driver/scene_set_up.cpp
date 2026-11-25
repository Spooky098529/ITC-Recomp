// The app's scene's set-up (scene_set_up.h, docs/functions/frame-driver.md):
// step for step as the recompiled functions.

#include "frame_driver/scene_set_up.h"

#include "guest.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "frame_driver/frame_driver.h"
#include "graphics/video_output.h"
#include "schema/command_list.h"
#include "schema/frame_driver.h"
#include "schema/framework.h"

#include <bit>

namespace frame_driver {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
// The original's lfs and stfs of an image constant (through a double).
void StoreFloatOf(uint32_t address, uint32_t from) {
  const double value = double(std::bit_cast<float>(Load(from)));
  Store(address, std::bit_cast<uint32_t>(float(value)));
}

constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kC01BE0 = 0x82001BE0;
constexpr uint32_t kC01BE4 = 0x82001BE4;
constexpr uint32_t kC01FBC = 0x82001FBC;
constexpr uint32_t kC01FD8 = 0x82001FD8;
constexpr uint32_t kC0277C = 0x8200277C;

struct Float4Of {
  uint32_t address;
  uint32_t from[4];
};
// The float4s DrawGlobals::Init stores (worked out from its stores; each
// lane the image constant it loads).
constexpr Float4Of kDefaults[] = {
    {0x8265ECE0, {kZero, kZero, kZero, kOne}},
    {0x8265ECF0, {kOne, kOne, kOne, kOne}},
    {0x8265ED00, {kZero, kZero, kZero, kZero}},
    {0x8296B950, {kOne, kZero, kZero, kOne}},
    {0x8296B960, {kZero, kOne, kZero, kOne}},
    {0x8296B970, {kZero, kZero, kOne, kOne}},
    {0x8296B980, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296B990, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296B9A0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296B9B0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296B9C0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296B9D0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296B9E0, {kC01FD8, kZero, kZero, kOne}},
    {0x8296B9F0, {kZero, kZero, kC01FD8, kOne}},
    {0x8296BA00, {kC0277C, kZero, kZero, kOne}},
    {0x8296BA10, {kZero, kZero, kC0277C, kOne}},
    {0x8296BA60, {kC01FBC, kC01FBC, kC01FBC, kOne}},
    {0x8296BA70, {kC01FBC, kC01FBC, kC01FBC, kOne}},
    {0x8296BA80, {kC01FBC, kC01FBC, kC01FBC, kOne}},
    {0x8296BA90, {kC01FBC, kC01FBC, kC01FBC, kOne}},
    {0x8296BAA0, {kC01FBC, kC01FBC, kC01FBC, kOne}},
    {0x8296BAB0, {kC01FBC, kC01FBC, kC01FBC, kOne}},
    {0x8296BAC0, {kC01FBC, kC01FBC, kC01FBC, kOne}},
    {0x8296BAD0, {kC01FBC, kC01FBC, kC01FBC, kOne}},
    {0x8296BAE0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BAF0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB00, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB10, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB20, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB30, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB40, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB50, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB60, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB70, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB80, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BB90, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BBA0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BBB0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BBC0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BBD0, {kC01BE4, kC01BE4, kC01BE4, kOne}},
    {0x8296BBE0, {kOne, kZero, kZero, kOne}},
    {0x8296BBF0, {kZero, kOne, kZero, kOne}},
    {0x8296BC00, {kZero, kZero, kOne, kOne}},
    {0x8296BC10, {kZero, kZero, kC01BE0, kOne}},
    {0x8296BC60, {kOne, kOne, kOne, kOne}},
    {0x8296BC70, {kOne, kOne, kOne, kOne}},
    {0x8296BC80, {kOne, kOne, kOne, kOne}},
    {0x8296BC90, {kOne, kOne, kOne, kOne}},
    {0x8296BCA0, {kOne, kOne, kOne, kOne}},
    {0x8296BCB0, {kOne, kOne, kOne, kOne}},
    {0x8296BCC0, {kOne, kOne, kOne, kOne}},
    {0x8296BCD0, {kOne, kOne, kOne, kOne}},
};

using G = schema::FrameDriverGlobals;

}  // namespace

uint32_t CommandListInit(uint32_t list, uint32_t slots,
                         const SceneSetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8258CD28 CommandList::Init");
  const schema::CommandList l(list);
  const uint32_t old = l.allocation();
  if (old != 0) {
    calls.operator_delete(old);
    l.set_allocation(0);
  }
  l.set_buffer0(0);
  l.set_buffer1(0);
  if (slots & 1) {
    slots++;
  }
  const uint32_t bytes = slots > 0x0FFFFFFFu ? 0xFFFFFFFFu : slots << 4;
  const uint32_t memory = calls.operator_new(bytes);
  const uint32_t half = slots >> 1;
  const uint32_t recorded = l.buffer() ^ 1;
  const uint32_t second = (half << 4) + memory;
  const uint32_t used = l.used();
  l.set_capacity(half);
  l.set_allocation(memory);
  l.set_buffer0(memory);
  l.set_buffer(recorded);
  l.set_buffer1(second);
  // buffer0 + 4 x the buffer: buffer0 or buffer1.
  const uint32_t first_buffer = Load(l.buffer0_address() + 4 * recorded);
  l.set_last_used(used);
  l.set_used(0);
  const uint32_t back = recorded ^ 1;
  l.set_buffer(back);
  l.set_next_chunk(first_buffer);
  const uint32_t back_buffer = Load(l.buffer0_address() + 4 * back);
  l.set_last_used(0);
  l.set_used(0);
  l.set_next_chunk(back_buffer);
  return 1;
}

void ColorRectSetTargets(uint32_t queue, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82178B28 ColorRect::SetTargets");
  G::set_color_rect_queue(queue);
  G::set_color_rect_list(list);
}

void DrawGlobalsInit(uint32_t capacity, uint32_t count,
                     const SceneSetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82589E78 DrawGlobals::Init");
  calls.in_frame(368, 0x82589FAC, [&](uint32_t) {
    calls.set_register(22, capacity);
    calls.set_register(23, count);
    // The identity matrix.
    for (uint32_t row = 0; row < 4; row++) {
      for (uint32_t lane = 0; lane < 4; lane++) {
        Store(G::identity_matrix_address() + 16 * row + 4 * lane, row == lane ? 0x3F800000u : 0);
      }
    }
    for (const Float4Of& f : kDefaults) {
      if (f.address >= 0x8296B000) {
        continue;  // after the declarations, as the original
      }
      for (uint32_t lane = 0; lane < 4; lane++) {
        StoreFloatOf(f.address + 4 * lane, f.from[lane]);
      }
    }
    // The declarations: table k's element pointers at 0x8265EBE4 - 280 +
    // 28k, its declarations at 0x8296B780 + 340 - 28k (0x8296B7BC + 28 x
    // (10 - k)), seven each.
    static constexpr uint32_t kReturns[11] = {0x82589FAC, 0x82589FC4, 0x82589FDC, 0x82589FF4,
                                              0x8258A00C, 0x8258A024, 0x8258A03C, 0x8258A054,
                                              0x8258A06C, 0x8258A080, 0x8258A094};
    for (uint32_t i = 0; i < 28; i += 4) {
      calls.set_register(30, i);
      for (uint32_t k = 0; k < 11; k++) {
        const uint32_t elements = Load(G::declaration_elements_address() + 28 * k + i);
        Store(G::declarations_address() + 28 * (10 - k) + i,
              calls.create_declaration(elements, kReturns[k]));
      }
    }
    for (const Float4Of& f : kDefaults) {
      if (f.address < 0x8296B000) {
        continue;
      }
      for (uint32_t lane = 0; lane < 4; lane++) {
        StoreFloatOf(f.address + 4 * lane, f.from[lane]);
      }
    }
    // The work memory.
    G::set_work_capacity(capacity);
    Store(G::work_start_address() + 0, calls.alloc_physical(capacity, 0x8258A8FC));
    Store(G::work_start_address() + 4,
          calls.alloc_physical(G::work_capacity(), 0x8258A918));
    G::set_work_count(count);
    Store(G::work_bytes_used_address() + 0, 0);
    Store(G::work_headers_used_address() + 0, 0);
    Store(G::work_bytes_used_address() + 4, 0);
    Store(G::work_headers_used_address() + 4, 0);
    if (count != 0) {
      G::set_work_virtual0(calls.alloc_virtual(count << 6, 0x8258A944));
      G::set_work_virtual1(calls.alloc_virtual(G::work_count() << 6, 0x8258A954));
    }
    Store(G::work_cleared_address() + 0, 0);
    Store(G::work_cleared_address() + 4, 0);
    const uint32_t defaults = G::draw_defaults_address();
    StoreFloatOf(defaults + 0, 0x82001FCC);
    StoreFloatOf(defaults + 4, 0x82002814);
    G::set_work_buffer(0);
    StoreFloatOf(defaults + 8, 0x82002CC4);
    StoreFloatOf(defaults + 12, kOne);
    G::set_sort_key_a(0);
    G::set_sort_key_b(1);
    StoreFloatOf(defaults + 16, kZero);
    StoreFloatOf(defaults + 20, kZero);
    StoreFloatOf(defaults + 24, kZero);
    StoreFloatOf(defaults + 28, kC01BE4);
  });
}

uint32_t PrimitiveDeclInit(const SceneSetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82591858 PrimitiveDecl::Init");
  calls.in_frame(96, 0x82591870, [&](uint32_t) {
    G::set_primitive_declaration(calls.create_declaration(0x8206CBB4, 0x82591870));
    G::set_primitive_declaration_ready(1);
  });
  return 1;
}

uint32_t SceneConstruct(uint32_t scene, uint32_t block, uint32_t frame,
                        const SceneSetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82098A98 Scene::Scene");
  namespace vo = graphics::video_output;
  constexpr uint32_t kDelegateVtable = 0x82002850;
  constexpr uint32_t kFunctorBase = 0x82000ABC;
  const schema::AppScene s(scene);
  const schema::AppSceneInitBlock init(block);
  const uint32_t list = s.command_list_address();
  const uint32_t queue = s.render_queue_address();
  calls.set_register(31, scene);
  calls.set_register(29, list);
  calls.set_register(28, block);
  s.set_vtable(schema::AppScene::kVtable);
  command_list::Construct(list);
  calls.set_register(30, queue);
  calls.render_queue_construct(queue);
  const uint32_t interface = init.framework_interface().address;
  s.set_framework_interface(interface);
  const uint32_t driver = calls.vcall(interface, 4, 0, 0, 0x82098AE8);
  const uint32_t getter =
      schema::FrameDriverInterface(driver).vtable().get_video_output();
  const uint32_t video_output = getter == kGetVideoOutput
                                    ? GetVideoOutput(driver)
                                    : calls.vcall(driver, 12, 0, 0, 0x82098AF8);
  calls.set_register(27, video_output);
  Store(frame + 80, vo::LockDevice(video_output));
  calls.in_frame(112, 0x82098B14, [&](uint32_t) { CommandListInit(list, 512000, calls); });
  calls.vcall(queue, 0, Load(frame + 80), 0, 0x82098B2C);
  ColorRectSetTargets(queue, list);
  DrawGlobalsInit(0x200000, 16384, calls);
  G::set_scene_list(list);
  G::set_scene_queue(queue);
  PrimitiveDeclInit(calls);
  G::set_scene_list_2(list);
  G::set_scene_queue_2(queue);
  vo::UnlockDevice(video_output, frame + 80);
  // Scene::Draw as the frame driver's listener 1.
  const schema::FrameDriverListenerKey key(frame + 84);
  const schema::RunLoopDelegate draw(frame + 88);
  const schema::RunLoopDelegate update(frame + 104);
  const schema::RunLoopDelegate on_event(frame + 120);
  draw.set_object(scene);
  draw.set_vtable(kDelegateVtable);
  draw.set_function(0x82098940);
  key.set_key(1);
  const uint32_t listener_driver =
      calls.vcall(s.framework_interface().address, 4, 0, 0, 0x82098BB4);
  calls.vcall(listener_driver, 4, key.address, draw.address, 0x82098BCC);
  // Scene::Update as tick handler 18; the first delegate destroyed.
  draw.set_vtable(kFunctorBase);
  update.set_vtable(kDelegateVtable);
  update.set_object(scene);
  update.set_function(0x820988C8);
  key.set_key(18);
  calls.vcall(init.run_loop().address, 8, key.address, update.address, 0x82098C18);
  // Scene::OnEvent5 as the listener for event 5; the second destroyed.
  on_event.set_object(scene);
  update.set_vtable(kFunctorBase);
  on_event.set_vtable(kDelegateVtable);
  on_event.set_function(0x820989A8);
  key.set_key(5);
  calls.vcall(init.run_loop().address, 4, key.address, on_event.address, 0x82098C64);
  return scene;
}

}  // namespace frame_driver
