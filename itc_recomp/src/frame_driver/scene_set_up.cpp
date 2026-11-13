// The app's scene's set-up (scene_set_up.h, docs/functions/frame-driver.md):
// step for step as the recompiled functions.

#include "frame_driver/scene_set_up.h"

#include "guest.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "frame_driver/frame_driver.h"
#include "graphics/video_output.h"

#include <bit>

namespace frame_driver {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// The original's lfs and stfs of an image constant (through a double).
void StoreFloatOf(uint8_t* base, uint32_t address, uint32_t from) {
  const double value = double(std::bit_cast<float>(Load(base, from)));
  Store(base, address, std::bit_cast<uint32_t>(float(value)));
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

constexpr uint32_t kWorkMem = 0x8296B780;
constexpr uint32_t kDeclarationElements = 0x8265EBE4;  // the original's r29
constexpr uint32_t kDeclarations = 0x8296B780;         // the original's r31

}  // namespace

uint32_t CommandListInit(uint8_t* base, uint32_t list, uint32_t slots,
                         const SceneSetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8258CD28 CommandList::Init");
  const uint32_t old = Load(base, list + 20);
  if (old != 0) {
    calls.operator_delete(old);
    Store(base, list + 20, 0);
  }
  Store(base, list + 24, 0);
  Store(base, list + 28, 0);
  if (slots & 1) {
    slots++;
  }
  const uint32_t bytes = slots > 0x0FFFFFFFu ? 0xFFFFFFFFu : slots << 4;
  const uint32_t memory = calls.operator_new(bytes);
  const uint32_t half = slots >> 1;
  const uint32_t recorded = Load(base, list + 4) ^ 1;
  const uint32_t second = (half << 4) + memory;
  const uint32_t used = Load(base, list + 12);
  Store(base, list + 8, half);
  Store(base, list + 20, memory);
  Store(base, list + 24, memory);
  Store(base, list + 4, recorded);
  Store(base, list + 28, second);
  const uint32_t first_buffer = Load(base, list + ((recorded + 6) << 2));
  Store(base, list + 16, used);
  Store(base, list + 12, 0);
  const uint32_t back = recorded ^ 1;
  Store(base, list + 4, back);
  Store(base, list + 32, first_buffer);
  const uint32_t back_buffer = Load(base, list + ((back + 6) << 2));
  Store(base, list + 16, 0);
  Store(base, list + 12, 0);
  Store(base, list + 32, back_buffer);
  return 1;
}

void ColorRectSetTargets(uint8_t* base, uint32_t queue, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82178B28 ColorRect::SetTargets");
  Store(base, 0x826D83D0, queue);
  Store(base, 0x826D83D4, list);
}

void DrawGlobalsInit(uint8_t* base, uint32_t capacity, uint32_t count,
                     const SceneSetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82589E78 DrawGlobals::Init");
  calls.in_frame(368, 0x82589FAC, [&](uint32_t) {
    calls.set_register(22, capacity);
    calls.set_register(23, count);
    // The identity matrix.
    for (uint32_t row = 0; row < 4; row++) {
      for (uint32_t lane = 0; lane < 4; lane++) {
        Store(base, 0x8265ECA0 + 16 * row + 4 * lane, row == lane ? 0x3F800000u : 0);
      }
    }
    for (const Float4Of& f : kDefaults) {
      if (f.address >= 0x8296B000) {
        continue;  // after the declarations, as the original
      }
      for (uint32_t lane = 0; lane < 4; lane++) {
        StoreFloatOf(base, f.address + 4 * lane, f.from[lane]);
      }
    }
    // The declarations: table k's element pointers at 0x8265EBE4 - 280 +
    // 28k, its declarations at 0x8296B780 + 340 - 28k, seven each.
    static constexpr uint32_t kReturns[11] = {0x82589FAC, 0x82589FC4, 0x82589FDC, 0x82589FF4,
                                              0x8258A00C, 0x8258A024, 0x8258A03C, 0x8258A054,
                                              0x8258A06C, 0x8258A080, 0x8258A094};
    for (uint32_t i = 0; i < 28; i += 4) {
      calls.set_register(30, i);
      for (uint32_t k = 0; k < 11; k++) {
        const uint32_t elements = Load(base, kDeclarationElements - 280 + 28 * k + i);
        Store(base, kDeclarations + 340 - 28 * k + i,
              calls.create_declaration(elements, kReturns[k]));
      }
    }
    for (const Float4Of& f : kDefaults) {
      if (f.address < 0x8296B000) {
        continue;
      }
      for (uint32_t lane = 0; lane < 4; lane++) {
        StoreFloatOf(base, f.address + 4 * lane, f.from[lane]);
      }
    }
    // The work memory.
    Store(base, kWorkMem + 48, capacity);
    Store(base, kWorkMem + 40, calls.alloc_physical(capacity, 0x8258A8FC));
    Store(base, kWorkMem + 44, calls.alloc_physical(Load(base, kWorkMem + 48), 0x8258A918));
    Store(base, kWorkMem + 20, count);
    Store(base, kWorkMem + 32, 0);
    Store(base, kWorkMem + 24, 0);
    Store(base, kWorkMem + 36, 0);
    Store(base, kWorkMem + 28, 0);
    if (count != 0) {
      Store(base, kWorkMem + 12, calls.alloc_virtual(count << 6, 0x8258A944));
      Store(base, kWorkMem + 16, calls.alloc_virtual(Load(base, kWorkMem + 20) << 6, 0x8258A954));
    }
    Store(base, kWorkMem + 4, 0);
    Store(base, kWorkMem + 8, 0);
    StoreFloatOf(base, 0x8265ED20, 0x82001FCC);
    StoreFloatOf(base, 0x8265ED24, 0x82002814);
    Store(base, kWorkMem + 0, 0);
    StoreFloatOf(base, 0x8265ED28, 0x82002CC4);
    StoreFloatOf(base, 0x8265ED2C, kOne);
    Store(base, 0x8296B90C, 0);
    Store(base, 0x8296B900, 1);
    StoreFloatOf(base, 0x8265ED30, kZero);
    StoreFloatOf(base, 0x8265ED34, kZero);
    StoreFloatOf(base, 0x8265ED38, kZero);
    StoreFloatOf(base, 0x8265ED3C, kC01BE4);
  });
}

uint32_t PrimitiveDeclInit(uint8_t* base, const SceneSetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82591858 PrimitiveDecl::Init");
  calls.in_frame(96, 0x82591870, [&](uint32_t) {
    Store(base, 0x82970BC8, calls.create_declaration(0x8206CBB4, 0x82591870));
    Store(base, 0x82970BDC, 1);
  });
  return 1;
}

uint32_t SceneConstruct(uint8_t* base, uint32_t scene, uint32_t block, uint32_t frame,
                        const SceneSetUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82098A98 Scene::Scene");
  namespace vo = graphics::video_output;
  constexpr uint32_t kDelegateVtable = 0x82002850;
  constexpr uint32_t kFunctorBase = 0x82000ABC;
  const uint32_t list = scene + kSceneCommandList;
  const uint32_t queue = scene + kSceneRenderQueue;
  calls.set_register(31, scene);
  calls.set_register(29, list);
  calls.set_register(28, block);
  Store(base, scene + 0, 0x82000F3C);
  command_list::Construct(base, list);
  calls.set_register(30, queue);
  calls.render_queue_construct(queue);
  const uint32_t interface = Load(base, block + 4);
  Store(base, scene + 4, interface);
  const uint32_t driver = calls.vcall(interface, 4, 0, 0, 0x82098AE8);
  const uint32_t getter = Load(base, Load(base, driver) + 12);
  const uint32_t video_output = getter == kGetVideoOutput
                                    ? GetVideoOutput(driver)
                                    : calls.vcall(driver, 12, 0, 0, 0x82098AF8);
  calls.set_register(27, video_output);
  Store(base, frame + 80, vo::LockDevice(base, video_output));
  calls.in_frame(112, 0x82098B14, [&](uint32_t) { CommandListInit(base, list, 512000, calls); });
  calls.vcall(queue, 0, Load(base, frame + 80), 0, 0x82098B2C);
  ColorRectSetTargets(base, queue, list);
  DrawGlobalsInit(base, 0x200000, 16384, calls);
  Store(base, 0x8296B918, list);
  Store(base, 0x8296B91C, queue);
  PrimitiveDeclInit(base, calls);
  Store(base, 0x82970BD8, list);
  Store(base, 0x82970BD4, queue);
  vo::UnlockDevice(base, video_output, frame + 80);
  // Scene::Draw as the frame driver's listener 1.
  Store(base, frame + 92, scene);
  Store(base, frame + 88, kDelegateVtable);
  Store(base, frame + 96, 0x82098940);
  Store(base, frame + 84, 1);
  const uint32_t listener_driver = calls.vcall(Load(base, scene + 4), 4, 0, 0, 0x82098BB4);
  calls.vcall(listener_driver, 4, frame + 84, frame + 88, 0x82098BCC);
  // Scene::Update as tick handler 18; the first delegate destroyed.
  Store(base, frame + 88, kFunctorBase);
  Store(base, frame + 104, kDelegateVtable);
  Store(base, frame + 108, scene);
  Store(base, frame + 112, 0x820988C8);
  Store(base, frame + 84, 18);
  calls.vcall(Load(base, block + 0), 8, frame + 84, frame + 104, 0x82098C18);
  // Scene::OnEvent5 as the listener for event 5; the second destroyed.
  Store(base, frame + 124, scene);
  Store(base, frame + 104, kFunctorBase);
  Store(base, frame + 120, kDelegateVtable);
  Store(base, frame + 128, 0x820989A8);
  Store(base, frame + 84, 5);
  calls.vcall(Load(base, block + 0), 4, frame + 84, frame + 120, 0x82098C64);
  return scene;
}

}  // namespace frame_driver
