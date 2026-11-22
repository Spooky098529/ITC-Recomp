// The app framework natively (docs/functions/framework.md).

#include "framework/framework.h"

#include "guest.h"

#include "base/log.h"
#include "file_layer/file_layer.h"
#include "framework/run_loop.h"
#include "models/xpr.h"
#include "memory/c_runtime_heap.h"
#include "schema/framework.h"

namespace framework {

uint32_t Flag133(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090760 Framework flag +133");
  return schema::Framework(base, object).restart_requested();
}

uint32_t Flag134(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090768 Framework flag +134");
  return schema::Framework(base, object).app_finished();
}

uint32_t Word60(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090770 word +60");
  return schema::FrameworkGetterSelf(base, object).word_60();
}

uint32_t Word64(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090778 word +64");
  return schema::FrameworkGetterSelf(base, object).word_64();
}

uint32_t Word88(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090788 word +88");
  return schema::FrameworkGetterSelf(base, object).word_88();
}

uint32_t Word68(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8248F030 word +68");
  return schema::FrameworkGetterSelf(base, object).word_68();
}

uint32_t Word44(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_824892F8 word +44");
  return schema::FrameworkGetterSelf(base, object).word_44();
}

uint32_t InputManagerFrom36(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090830 Framework +36 slot 1");
  return Word64(base, self - 12);
}

uint32_t RendererFrom52(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090828 Framework +52 slot 2");
  return Word68(base, self - 44);
}

uint32_t WorkerPoolFrom52(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090838 Framework +52 slot 1");
  return Word44(base, self - 28);
}

uint32_t RendererOrTextFrom24(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090860 Framework +24 slot 4");
  return Word68(base, self - 16);
}

uint32_t SceneFrom52(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090868 Framework +52 slot 3");
  return Word60(base, self - 32);
}

uint32_t RendererFrom36(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_820908E0 Framework +36 slot 2");
  return Word68(base, self - 28);
}

uint32_t WorkerPoolFrom36(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_820908E8 Framework +36 slot 4");
  return Word44(base, self - 12);
}

uint32_t SceneFrom36(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090908 Framework +36 slot 3");
  return Word60(base, self - 16);
}

void DebugFontSetPosition(uint8_t* base, uint32_t x, uint32_t y) {
  ITC_NATIVE_RUNNING("sub_8258BE08 DebugFont::SetPosition");
  using Globals = schema::FrameworkGlobals;
  Globals::set_debug_font_line_x(base, x);
  Globals::set_debug_font_line_y(base, y);
  Globals::set_debug_font_cursor_x(base, x);
  Globals::set_debug_font_cursor_y(base, y);
}

uint64_t DebugFontInit(uint8_t* base, uint32_t data, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258BD58 DebugFont::Init");
  constexpr uint32_t kNew = 0x825A6880, kXprCreate = 0x82596F68;
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  using Globals = schema::FrameworkGlobals;
  Globals::set_debug_font_loaded(base, 1);
  Globals::set_debug_font_cursor_x(base, 0);
  Globals::set_debug_font_cursor_y(base, 0);
  Globals::set_debug_font_line_x(base, 0);
  Globals::set_debug_font_line_y(base, 0);
  Globals::set_debug_font_word_16(base, 0);
  Globals::set_debug_font_line_x(base, 32);
  Globals::set_debug_font_line_y(base, 0);
  Globals::set_debug_font_cursor_x(base, 32);
  Globals::set_debug_font_cursor_y(base, 0);
  Globals::set_debug_font_byte_16(base, 255);
  Globals::set_debug_font_byte_17(base, 255);
  Globals::set_debug_font_byte_18(base, 255);
  Globals::set_debug_font_byte_19(base, 255);
  uint32_t xpr;
  if (const auto block = memory::crt::OperatorNew(base, 24)) {
    xpr = *block;
  } else {
    xpr = uint32_t(call(kNew, frame, 0x8258BDD0, 24, 0, 0));
  }
  if (xpr != 0) {
    models::XprConstruct(base, xpr);
  }
  Globals::set_debug_font_xpr(base, xpr);
  const uint64_t r3 = call(kXprCreate, frame, 0x8258BDF0, xpr, data, 0);
  // lfs and two stfs: the float through a double and back.
  const float scale = float(double(Globals::debug_font_default_scale(base)));
  Globals::set_debug_font_scale_x(base, scale);
  Globals::set_debug_font_scale_y(base, scale);
  return r3;
}

uint32_t DebugTextLoadFont(uint8_t* base, uint32_t text, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82090CC0 DebugText::LoadFont");
  constexpr uint32_t kOpen = 0x820A6AB8, kNew = 0x825A6940, kRead = 0x8258C820;
  constexpr uint32_t kClose = 0x8258C718;
  constexpr uint32_t kName = 0x82000BB0;  // game:\DATA\DEBUG\DEBFONT.XPR
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  const uint32_t file = frame + 96;
  file_layer::GameFileConstruct(base, file);
  guest::StoreU8(base, frame + 80, 0);
  if ((call(kOpen, frame, 0x82090CF8, file, kName, frame + 80) & 0xFF) != 0) {
    const uint32_t size = file_layer::FileGetSize(base, file);
    const schema::DebugText debug_text(base, text);
    debug_text.set_font_size(size);
    uint32_t buffer;
    if (const auto block = memory::crt::OperatorNew(base, size)) {
      buffer = *block;
    } else {
      buffer = uint32_t(call(kNew, frame, 0x82090D10, size, 0, 0));
    }
    guest::StoreU32(base, frame + 84, debug_text.font_size());
    debug_text.set_font_data(buffer);
    call(kRead, frame, 0x82090D2C, file, buffer, frame + 84);
    call(kClose, frame, 0x82090D34, file, 0, 0);
    DebugFontInit(base, debug_text.font_data(), frame, call);
    schema::FrameworkGlobals::set_debug_font_word_40(base, 681);  // sub_8258BE48(681)
  }
  file_layer::GameFileDestruct(base, file);
  return file;
}

void DebugTextSetPosition(uint8_t* base, uint32_t x, uint32_t y) {
  ITC_NATIVE_RUNNING("sub_82091050 DebugText::SetPosition");
  DebugFontSetPosition(base, schema::DebugTextCoordinate(base, x).value(),
                       schema::DebugTextCoordinate(base, y).value());
}

uint32_t VirtualTarget(uint8_t* base, uint32_t object, uint32_t offset) {
  return guest::LoadU32(base, guest::VtableWord(base, object) + offset);
}

uint32_t DelegateSlot0Target(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82096330 Delegate slot 0 thunk");
  return schema::DelegateTarget(base, object).vtable().slot_0();
}

uint32_t DelegateSlot1Target(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82096340 Delegate slot 1 thunk");
  return schema::DelegateTarget(base, object).vtable().slot_1();
}

uint32_t TickHandlerTarget(uint8_t* base, uint32_t delegate) {
  ITC_NATIVE_RUNNING("sub_82099A98 TickHandler::Invoke");
  return schema::RunLoopDelegate(base, delegate).vtable().invoke();
}

VirtualCall FrameworkStartCall(uint8_t* base, uint32_t framework) {
  ITC_NATIVE_RUNNING("sub_820902B0 Framework::Start");
  const schema::FrameworkAppInterface app = schema::Framework(base, framework).app();
  return {app.address, app.vtable().start()};
}

VirtualCall AppInputSourceCall(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82094350 App input source");
  const schema::FrameworkInterface source = schema::AppPart4(base, self).framework_interface();
  return {source.address, source.vtable().slot_1()};
}

VirtualCall AppInputSourceFrom16Call(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_820944C8 App +16 slot +36");
  return AppInputSourceCall(base, self - 12);
}

void FrameworkOnTick(uint8_t* base, uint32_t framework, uint32_t loop, uint32_t key,
                     const OnTickCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820902C8 Framework::OnTick");
  const schema::Framework fw(base, framework);
  guest::StoreU32(base, key, 0);
  calls.get_processor(fw.input_manager(), key);
  const uint32_t app = fw.app().address;
  fw.set_restart_requested(0);
  if ((calls.app_continues(app) & 0xFF) == 0) {
    guest::StoreU32(base, key, 7);
    calls.remove_tick_handler(loop, key);
    fw.set_app_finished(1);
  }
}

std::optional<bool> RunLoopAddRequest(uint8_t* base, uint32_t loop, uint32_t type, uint32_t key,
                                      uint32_t delegate, uint32_t request,
                                      const std::function<uint32_t(uint32_t)>& clone) {
  if (type == 0) {
    ITC_NATIVE_RUNNING("sub_8209AAA8 RunLoop::AddTickHandler");
  } else {
    ITC_NATIVE_RUNNING("sub_8209AB48 RunLoop::AddListener");
  }
  const schema::Request built(base, request);
  built.set_type(type);
  built.set_key(schema::RunLoopHandlerKey(base, key).key());
  built.set_delegate(clone(delegate));
  return RequestsPushBack(base, schema::RunLoop(base, loop).requests_address(), request);
}

void FrameworkConstruct(uint8_t* base, uint32_t framework) {
  ITC_NATIVE_RUNNING("sub_82090420 Framework::Framework");
  static constexpr uint32_t kVtables[16] = {
      0x82000B90, 0x82000B8C, 0x82000B84, 0x82000B80, 0x82000B7C, 0x82000B70,
      0x82000B4C, 0x82000B38, 0x82000B28, 0x82000AFC, 0x82000AF4, 0x82000AEC,
      0x82000AE4, 0x82000AD4, 0x82000ACC, 0x82000AC8};
  // The sixteen interfaces' vtable words, +0 to +60 in order.
  const schema::Framework fw(base, framework);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, fw.vtable_address() + 4 * i, kVtables[i]);
  }
  fw.set_run_loop(0u);
  fw.set_frame_driver(0);
  fw.set_scene(0);
  fw.set_worker_pool(0u);
  fw.set_worker_pool_2(0u);
  fw.set_input_manager(0);
  fw.set_debug_text(0u);
  fw.set_unknown_96(0);
  fw.set_unknown_100(0);
  fw.set_unknown_104(0);
  fw.set_save_devices(0);
  fw.set_users(0);
  fw.set_unknown_132(0);
  fw.set_restart_requested(0);
  fw.set_app_finished(0);
}

std::optional<uint32_t> FrameworkCreate(uint8_t* base, uint32_t holder) {
  if (!CanAllocate(base)) {
    return std::nullopt;
  }
  const auto block = memory::crt::OperatorNew(base, 136);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820909B8 Framework::Create");
  uint32_t framework = 0;
  if (*block != 0) {
    FrameworkConstruct(base, *block);
    framework = *block;
  }
  schema::FrameworkHolder(base, holder).set_framework(framework);
  return holder;
}

uint32_t PoolMakerConstruct(uint8_t* base, uint32_t maker, uint32_t vtable, uint32_t interface,
                            const std::function<uint32_t(uint32_t)>& pool) {
  if (vtable == 0x82002848) {
    ITC_NATIVE_RUNNING("sub_820AC8F8 PoolMaker::PoolMaker");
  } else {
    ITC_NATIVE_RUNNING("sub_820AC848 PoolMaker2::PoolMaker2");
  }
  // The first maker's layout (PoolMaker, morph-jobs.md) is the same.
  const schema::MorphJobMaker made(base, maker);
  made.set_vtable(vtable);
  made.set_worker_pool(pool(interface));
  return maker;
}

uint32_t FrameStartConstruct(uint8_t* base, uint32_t frame_start, uint32_t block, uint32_t frame,
                             const std::function<void(uint32_t loop, uint32_t offset,
                                                      uint32_t key, uint32_t delegate)>& add) {
  ITC_NATIVE_RUNNING("sub_82092710 FrameStart::FrameStart");
  constexpr uint32_t kDelegateVtable = 0x82002850;
  const uint32_t key = frame + 80;
  const uint32_t tick = frame + 88;
  const uint32_t listener = frame + 104;
  // The delegates are in the frame (Delegate's layout, RunLoopDelegate).
  const schema::RunLoopDelegate tick_delegate(base, tick), listener_delegate(base, listener);
  const schema::FrameStart made(base, frame_start);
  const schema::FrameStartBlock set_up(base, block);
  tick_delegate.set_object(frame_start);
  made.set_vtable(schema::FrameStart::kVtable);
  tick_delegate.set_vtable(kDelegateVtable);
  guest::StoreU32(base, key, 4);
  tick_delegate.set_function(0x82096330);
  made.set_framework_interface(set_up.framework_interface());
  add(set_up.run_loop().address, 8, key, tick);
  // The tick handler's delegate destroyed (its base vtable), the listener's
  // built.
  listener_delegate.set_object(frame_start);
  tick_delegate.set_vtable(0x82000ABC);
  listener_delegate.set_vtable(kDelegateVtable);
  listener_delegate.set_function(0x82096340);
  guest::StoreU32(base, key, 2);
  add(set_up.run_loop().address, 4, key, listener);
  return frame_start;
}

uint32_t DebugTextConstruct(uint8_t* base, uint32_t text, uint32_t block, uint32_t stack_word,
                            const std::function<void(uint32_t interface, uint32_t job)>& post) {
  ITC_NATIVE_RUNNING("sub_82090EA8 DebugText::DebugText");
  const schema::DebugText made(base, text);
  const schema::DebugTextBlock set_up(base, block);
  made.set_vtable(schema::DebugText::kVtable);
  made.set_framework_interface(set_up.framework_interface());
  made.set_job_vtable(0x820066DC);
  made.set_job_vtable_4(0x82002908);
  made.set_job_unknown_8(0);
  made.set_job_state(0);
  made.set_job_unknown_16(0);
  made.set_job_unknown_17(1);
  made.set_job_unknown_18(0);
  made.set_job_unknown_20(stack_word);
  made.set_job_delegate_vtable(0x82002850);
  made.set_job_delegate_object(text);
  made.set_job_delegate_function(0x82090CC0);  // the debug font's load
  made.set_font_data(0);
  schema::FrameworkGlobals::set_debug_text(base, text);
  post(set_up.worker_pool().address, made.job_vtable_address());
  return text;
}

std::optional<uint32_t> DebugTextCreate(
    uint8_t* base, uint32_t block, uint32_t stack_word,
    const std::function<void(uint32_t interface, uint32_t job)>& post) {
  if (!CanAllocate(base)) {
    return std::nullopt;
  }
  const auto text = memory::crt::OperatorNew(base, 52);
  if (!text) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82091160 DebugText::Create");
  if (*text == 0) {
    return 0u;
  }
  return DebugTextConstruct(base, *text, block, stack_word, post);
}

}  // namespace framework
