// Lift sweep batch 026 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

ITC_FLAG_BOOL(native_lifted_026, true, "ITC",
              "Run lift sweep batch 026's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace lifted {

// sub_825D97A0: lifted from generated/ by lift.py.
lifted::Result f825D97A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D97A0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20624
  r3 = uint64_t(int64_t(int64_t(r11) + -20624));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D97B0: lifted from generated/ by lift.py.
lifted::Result f825D97B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D97B0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20616
  r3 = uint64_t(int64_t(int64_t(r11) + -20616));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D97C0: lifted from generated/ by lift.py.
void f825D97C0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D97C0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,28252(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 28252);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-31476(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -31476);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23856(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23856, temp.u32);
  // blr
  return;
}

// sub_825D97E8: lifted from generated/ by lift.py.
void f825D97E8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D97E8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,28464(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 28464);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-31464(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -31464);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23888(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23888, temp.u32);
  // blr
  return;
}

// sub_825D9810: lifted from generated/ by lift.py.
void f825D9810(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9810 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,28496
  r11 = uint64_t(int64_t(int64_t(r11) + 28496));
  // stw r10,164(r11)
  REX_STORE_U32(uint32_t(r11) + 164, uint32_t(r10));
  // stw r10,168(r11)
  REX_STORE_U32(uint32_t(r11) + 168, uint32_t(r10));
  // blr
  return;
}

// sub_825D9828: lifted from generated/ by lift.py.
void f825D9828(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9828 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,28672
  r11 = uint64_t(int64_t(int64_t(r11) + 28672));
  // stw r10,164(r11)
  REX_STORE_U32(uint32_t(r11) + 164, uint32_t(r10));
  // stw r10,168(r11)
  REX_STORE_U32(uint32_t(r11) + 168, uint32_t(r10));
  // blr
  return;
}

// sub_825D9840: lifted from generated/ by lift.py.
lifted::Result f825D9840(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9840 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20608
  r3 = uint64_t(int64_t(int64_t(r11) + -20608));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9850: lifted from generated/ by lift.py.
lifted::Result f825D9850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9850 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20600
  r3 = uint64_t(int64_t(int64_t(r11) + -20600));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9860: lifted from generated/ by lift.py.
void f825D9860(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9860 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,28856(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 28856);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-31412(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -31412);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23892(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23892, temp.u32);
  // blr
  return;
}

// sub_825D9888: lifted from generated/ by lift.py.
void f825D9888(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9888 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,28940(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 28940);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-31164(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -31164);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23896(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23896, temp.u32);
  // blr
  return;
}

// sub_825D98B0: lifted from generated/ by lift.py.
void f825D98B0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D98B0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,28960(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 28960);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-31048(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -31048);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23908(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23908, temp.u32);
  // blr
  return;
}

// sub_825D98D8: lifted from generated/ by lift.py.
void f825D98D8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D98D8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,29064(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 29064);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-31004(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -31004);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23912(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23912, temp.u32);
  // blr
  return;
}

// sub_825D9900: lifted from generated/ by lift.py.
void f825D9900(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9900 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,29080(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 29080);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-30952(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -30952);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23916(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23916, temp.u32);
  // blr
  return;
}

// sub_825D9928: lifted from generated/ by lift.py.
void f825D9928(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9928 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,29096(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 29096);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-30908(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -30908);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23920(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23920, temp.u32);
  // blr
  return;
}

// sub_825D9950: lifted from generated/ by lift.py.
void f825D9950(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9950 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,29112(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 29112);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-30860(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -30860);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23924(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23924, temp.u32);
  // blr
  return;
}

// sub_825D9978: lifted from generated/ by lift.py.
void f825D9978(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9978 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,29128(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 29128);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-30808(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -30808);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23928(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23928, temp.u32);
  // blr
  return;
}

// sub_825D9A10: lifted from generated/ by lift.py.
lifted::Result f825D9A10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9A10 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20592
  r3 = uint64_t(int64_t(int64_t(r11) + -20592));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9A20: lifted from generated/ by lift.py.
lifted::Result f825D9A20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9A20 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20584
  r3 = uint64_t(int64_t(int64_t(r11) + -20584));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9A30: lifted from generated/ by lift.py.
lifted::Result f825D9A30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9A30 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20576
  r3 = uint64_t(int64_t(int64_t(r11) + -20576));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9A40: lifted from generated/ by lift.py.
lifted::Result f825D9A40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9A40 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20568
  r3 = uint64_t(int64_t(int64_t(r11) + -20568));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9A50: lifted from generated/ by lift.py.
lifted::Result f825D9A50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9A50 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20560
  r3 = uint64_t(int64_t(int64_t(r11) + -20560));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9A60: lifted from generated/ by lift.py.
lifted::Result f825D9A60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9A60 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20552
  r3 = uint64_t(int64_t(int64_t(r11) + -20552));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9A70: lifted from generated/ by lift.py.
lifted::Result f825D9A70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9A70 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20544
  r3 = uint64_t(int64_t(int64_t(r11) + -20544));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9A80: lifted from generated/ by lift.py.
lifted::Result f825D9A80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9A80 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20536
  r3 = uint64_t(int64_t(int64_t(r11) + -20536));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9A90: lifted from generated/ by lift.py.
lifted::Result f825D9A90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9A90 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20528
  r3 = uint64_t(int64_t(int64_t(r11) + -20528));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9AA0: lifted from generated/ by lift.py.
lifted::Result f825D9AA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9AA0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20520
  r3 = uint64_t(int64_t(int64_t(r11) + -20520));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9AB0: lifted from generated/ by lift.py.
lifted::Result f825D9AB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9AB0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20512
  r3 = uint64_t(int64_t(int64_t(r11) + -20512));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9AC0: lifted from generated/ by lift.py.
lifted::Result f825D9AC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9AC0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20504
  r3 = uint64_t(int64_t(int64_t(r11) + -20504));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9AD0: lifted from generated/ by lift.py.
lifted::Result f825D9AD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9AD0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20496
  r3 = uint64_t(int64_t(int64_t(r11) + -20496));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9AE0: lifted from generated/ by lift.py.
lifted::Result f825D9AE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9AE0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20488
  r3 = uint64_t(int64_t(int64_t(r11) + -20488));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9AF0: lifted from generated/ by lift.py.
lifted::Result f825D9AF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9AF0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20480
  r3 = uint64_t(int64_t(int64_t(r11) + -20480));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9B00: lifted from generated/ by lift.py.
lifted::Result f825D9B00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9B00 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20472
  r3 = uint64_t(int64_t(int64_t(r11) + -20472));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9B10: lifted from generated/ by lift.py.
lifted::Result f825D9B10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9B10 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20464
  r3 = uint64_t(int64_t(int64_t(r11) + -20464));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9B20: lifted from generated/ by lift.py.
lifted::Result f825D9B20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9B20 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20456
  r3 = uint64_t(int64_t(int64_t(r11) + -20456));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9B30: lifted from generated/ by lift.py.
lifted::Result f825D9B30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9B30 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20448
  r3 = uint64_t(int64_t(int64_t(r11) + -20448));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9B40: lifted from generated/ by lift.py.
lifted::Result f825D9B40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9B40 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20440
  r3 = uint64_t(int64_t(int64_t(r11) + -20440));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9B50: lifted from generated/ by lift.py.
lifted::Result f825D9B50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9B50 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,30752
  r3 = uint64_t(int64_t(int64_t(r11) + 30752));
  // bl 0x8214e608
  // (left out: dead in C++)
  r3 = f8214E608(base, r1, r13, r3);
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20432
  r3 = uint64_t(int64_t(int64_t(r11) + -20432));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D9B88: lifted from generated/ by lift.py.
lifted::Result f825D9B88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9B88 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20400
  r3 = uint64_t(int64_t(int64_t(r11) + -20400));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9B98: lifted from generated/ by lift.py.
lifted::Result f825D9B98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9B98 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,29856
  r3 = uint64_t(int64_t(int64_t(r11) + 29856));
  // bl 0x8214f660
  // (left out: dead in C++)
  f8214F660(base, r3);
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20416
  r3 = uint64_t(int64_t(int64_t(r11) + -20416));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D9BD0: lifted from generated/ by lift.py.
lifted::Result f825D9BD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9BD0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // addi r31,r11,30320
  r31 = uint64_t(int64_t(int64_t(r11) + 30320));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r30,3
  r30 = uint64_t(int64_t(3));
  // addi r31,r31,384
  r31 = uint64_t(int64_t(int64_t(r31) + 384));
loc_825D9C04:
  // mr r3,r31
  r3 = r31;
  // bl 0x8214aa18
  // (left out: dead in C++)
  { const lifted::Result o = f8214AA18(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,12
  r31 = uint64_t(int64_t(int64_t(r31) + 12));
  // bge 0x825d9c04
  if (!cr0.lt) goto loc_825D9C04;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20320
  r3 = uint64_t(int64_t(int64_t(r11) + -20320));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D9C58: lifted from generated/ by lift.py.
lifted::Result f825D9C58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9C58 (lifted)");
  uint64_t r3 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,255
  r10 = uint64_t(int64_t(255));
  // addi r9,r11,31160
  r9 = uint64_t(int64_t(int64_t(r11) + 31160));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r11,r9
  r11 = r9;
loc_825D9C6C:
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r8,260(r11)
  REX_STORE_U32(uint32_t(r11) + 260, uint32_t(r8));
  // addi r11,r11,264
  r11 = uint64_t(int64_t(int64_t(r11) + 264));
  // bge 0x825d9c6c
  if (!cr0.lt) goto loc_825D9C6C;
  // mr r11,r8
  r11 = r8;
  // lis r8,1
  r8 = uint64_t(int64_t(65536));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // ori r8,r8,2048
  r8 = r8 | 2048;
  // addi r3,r10,-20240
  r3 = uint64_t(int64_t(int64_t(r10) + -20240));
  // stwx r11,r9,r8
  REX_STORE_U32(uint32_t(r9) + uint32_t(r8), uint32_t(r11));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9CA0: lifted from generated/ by lift.py.
void f825D9CA0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9CA0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-29784(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -29784);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-27976(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27976);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-32324(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -32324, temp.u32);
  // blr
  return;
}

// sub_825D9CC8: lifted from generated/ by lift.py.
void f825D9CC8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9CC8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-29260(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -29260);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-27916(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27916);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-32048(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -32048, temp.u32);
  // blr
  return;
}

// sub_825D9CF0: lifted from generated/ by lift.py.
lifted::Result f825D9CF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9CF0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20136
  r3 = uint64_t(int64_t(int64_t(r11) + -20136));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9D00: lifted from generated/ by lift.py.
void f825D9D00(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9D00 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-27796(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27796);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-27692(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27692);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-32040(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -32040, temp.u32);
  // blr
  return;
}

// sub_825D9D28: lifted from generated/ by lift.py.
void f825D9D28(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9D28 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-27780(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27780);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-26708(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -26708);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-32036(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -32036, temp.u32);
  // blr
  return;
}

// sub_825D9D50: lifted from generated/ by lift.py.
void f825D9D50(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9D50 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-27764(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27764);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-26664(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -26664);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-32032(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -32032, temp.u32);
  // blr
  return;
}

// sub_825D9D78: lifted from generated/ by lift.py.
void f825D9D78(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9D78 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-27748(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27748);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-26648(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -26648);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-32028(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -32028, temp.u32);
  // blr
  return;
}

// sub_825D9DA0: lifted from generated/ by lift.py.
void f825D9DA0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9DA0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-27732(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27732);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-26632(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -26632);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-32020(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -32020, temp.u32);
  // blr
  return;
}

// sub_825D9DC8: lifted from generated/ by lift.py.
void f825D9DC8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9DC8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-27716(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27716);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-26340(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -26340);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-32016(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -32016, temp.u32);
  // blr
  return;
}

// sub_825D9DF0: lifted from generated/ by lift.py.
void f825D9DF0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9DF0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-27700(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27700);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-26268(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -26268);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-32008(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -32008, temp.u32);
  // blr
  return;
}

// sub_825D9E18: lifted from generated/ by lift.py.
void f825D9E18(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9E18 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lfs f0,-27628(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27628);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-26148(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -26148);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-31796(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -31796, temp.u32);
  // blr
  return;
}

// sub_825D9E40: lifted from generated/ by lift.py.
lifted::Result f825D9E40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9E40 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20128
  r3 = uint64_t(int64_t(int64_t(r11) + -20128));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9E50: lifted from generated/ by lift.py.
lifted::Result f825D9E50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9E50 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20120
  r3 = uint64_t(int64_t(int64_t(r11) + -20120));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9EC0: lifted from generated/ by lift.py.
lifted::Result f825D9EC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9EC0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20088
  r3 = uint64_t(int64_t(int64_t(r11) + -20088));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9ED0: lifted from generated/ by lift.py.
lifted::Result f825D9ED0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9ED0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20080
  r3 = uint64_t(int64_t(int64_t(r11) + -20080));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9EE0: lifted from generated/ by lift.py.
lifted::Result f825D9EE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9EE0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20072
  r3 = uint64_t(int64_t(int64_t(r11) + -20072));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9F48: lifted from generated/ by lift.py.
lifted::Result f825D9F48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9F48 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20064
  r3 = uint64_t(int64_t(int64_t(r11) + -20064));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9F58: lifted from generated/ by lift.py.
lifted::Result f825D9F58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9F58 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20056
  r3 = uint64_t(int64_t(int64_t(r11) + -20056));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9FC8: lifted from generated/ by lift.py.
uint64_t f825D9FC8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825D9FC8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,14144
  r3 = uint64_t(int64_t(int64_t(r11) + 14144));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // lfs f13,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // stfs f13,92(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // stfs f13,104(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // stfs f0,112(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 112, temp.u32);
  // stfs f13,116(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 120, temp.u32);
  // stfs f0,124(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // stfs f13,128(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 128, temp.u32);
  // stfs f0,132(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 132, temp.u32);
  // stfs f0,136(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 136, temp.u32);
  // stfs f0,140(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 140, temp.u32);
  // bl 0x820bfbb8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820BFBB8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_825DA050: lifted from generated/ by lift.py.
uint64_t f825DA050(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825DA050 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // addi r31,r11,-32304
  r31 = uint64_t(int64_t(int64_t(r11) + -32304));
  // addi r3,r31,48
  r3 = uint64_t(int64_t(int64_t(r31) + 48));
  // bl 0x822936e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822936E0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,112(r31)
  REX_STORE_U32(uint32_t(r31) + 112, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825DA090: lifted from generated/ by lift.py.
uint64_t f825DA090(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825DA090 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0;
  double f1 = 0.0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // li r4,109
  r4 = uint64_t(int64_t(109));
  // addi r3,r11,19924
  r3 = uint64_t(int64_t(int64_t(r11) + 19924));
  // b 0x82411640
  r3 = f82411640(base, r1, r13, r3, r4);
  return r3;
}

// sub_825DA0B8: lifted from generated/ by lift.py.
lifted::Result f825DA0B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825DA0B8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // addi r11,r11,-11020
  r11 = uint64_t(int64_t(int64_t(r11) + -11020));
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x825db954
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlInitializeCriticalSection, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20024
  r3 = uint64_t(int64_t(int64_t(r11) + -20024));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DA0F0: lifted from generated/ by lift.py.
lifted::Result f825DA0F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825DA0F0 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32155
  r11 = uint64_t(int64_t(-2107310080));
  // addi r11,r11,-10988
  r11 = uint64_t(int64_t(int64_t(r11) + -10988));
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x825db954
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlInitializeCriticalSection, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20000
  r3 = uint64_t(int64_t(int64_t(r11) + -20000));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DA128: lifted from generated/ by lift.py.
uint64_t f825DA128(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825DA128 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // li r5,44
  r5 = uint64_t(int64_t(44));
  // addi r3,r11,30336
  r3 = uint64_t(int64_t(int64_t(r11) + 30336));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x825a71d0
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_825DA140: lifted from generated/ by lift.py.
lifted::Result f825DA140(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825DA140 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32154
  r11 = uint64_t(int64_t(-2107244544));
  // addi r11,r11,-9744
  r11 = uint64_t(int64_t(int64_t(r11) + -9744));
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x825db954
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlInitializeCriticalSection, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19976
  r3 = uint64_t(int64_t(int64_t(r11) + -19976));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DA178: lifted from generated/ by lift.py.
lifted::Result f825DA178(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825DA178 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,-18128
  r3 = uint64_t(int64_t(int64_t(r11) + -18128));
  // bl 0x8258e548
  // (left out: dead in C++)
  { const lifted::Result o = f8258E548(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19952
  r3 = uint64_t(int64_t(int64_t(r11) + -19952));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DA1B0: lifted from generated/ by lift.py.
lifted::Result f825DA1B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA1B0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19936
  r3 = uint64_t(int64_t(int64_t(r11) + -19936));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA1C0: lifted from generated/ by lift.py.
lifted::Result f825DA1C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA1C0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19928
  r3 = uint64_t(int64_t(int64_t(r11) + -19928));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA1D0: lifted from generated/ by lift.py.
lifted::Result f825DA1D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA1D0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19920
  r3 = uint64_t(int64_t(int64_t(r11) + -19920));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA1E0: lifted from generated/ by lift.py.
lifted::Result f825DA1E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA1E0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19912
  r3 = uint64_t(int64_t(int64_t(r11) + -19912));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA1F0: lifted from generated/ by lift.py.
lifted::Result f825DA1F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA1F0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19904
  r3 = uint64_t(int64_t(int64_t(r11) + -19904));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA200: lifted from generated/ by lift.py.
lifted::Result f825DA200(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA200 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19896
  r3 = uint64_t(int64_t(int64_t(r11) + -19896));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA210: lifted from generated/ by lift.py.
lifted::Result f825DA210(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA210 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-19888
  r3 = uint64_t(int64_t(int64_t(r10) + -19888));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-18096
  r11 = uint64_t(int64_t(int64_t(r11) + -18096));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,44(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA238: lifted from generated/ by lift.py.
lifted::Result f825DA238(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA238 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-19880
  r3 = uint64_t(int64_t(int64_t(r10) + -19880));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-18048
  r11 = uint64_t(int64_t(int64_t(r11) + -18048));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,44(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA260: lifted from generated/ by lift.py.
lifted::Result f825DA260(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA260 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-19872
  r3 = uint64_t(int64_t(int64_t(r10) + -19872));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-18000
  r11 = uint64_t(int64_t(int64_t(r11) + -18000));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,44(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA288: lifted from generated/ by lift.py.
lifted::Result f825DA288(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA288 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-19864
  r3 = uint64_t(int64_t(int64_t(r10) + -19864));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-17952
  r11 = uint64_t(int64_t(int64_t(r11) + -17952));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,44(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f0,60(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 60, temp.u32);
  // stfs f0,76(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 76, temp.u32);
  // stfs f0,92(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 92, temp.u32);
  // stfs f0,108(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 108, temp.u32);
  // stfs f0,124(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 124, temp.u32);
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA2C8: lifted from generated/ by lift.py.
lifted::Result f825DA2C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA2C8 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-19856
  r3 = uint64_t(int64_t(int64_t(r10) + -19856));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-17824
  r11 = uint64_t(int64_t(int64_t(r11) + -17824));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,44(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f0,60(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 60, temp.u32);
  // stfs f0,76(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 76, temp.u32);
  // stfs f0,92(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 92, temp.u32);
  // stfs f0,108(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 108, temp.u32);
  // stfs f0,124(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 124, temp.u32);
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA308: lifted from generated/ by lift.py.
lifted::Result f825DA308(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA308 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-19848
  r3 = uint64_t(int64_t(int64_t(r10) + -19848));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-17696
  r11 = uint64_t(int64_t(int64_t(r11) + -17696));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,44(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f0,60(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 60, temp.u32);
  // stfs f0,76(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 76, temp.u32);
  // stfs f0,92(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 92, temp.u32);
  // stfs f0,108(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 108, temp.u32);
  // stfs f0,124(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 124, temp.u32);
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA348: lifted from generated/ by lift.py.
lifted::Result f825DA348(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA348 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-19840
  r3 = uint64_t(int64_t(int64_t(r10) + -19840));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-17568
  r11 = uint64_t(int64_t(int64_t(r11) + -17568));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,44(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f0,60(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 60, temp.u32);
  // stfs f0,76(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 76, temp.u32);
  // stfs f0,92(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 92, temp.u32);
  // stfs f0,108(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 108, temp.u32);
  // stfs f0,124(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 124, temp.u32);
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA388: lifted from generated/ by lift.py.
lifted::Result f825DA388(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA388 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-19832
  r3 = uint64_t(int64_t(int64_t(r10) + -19832));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-17440
  r11 = uint64_t(int64_t(int64_t(r11) + -17440));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,44(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f0,60(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 60, temp.u32);
  // stfs f0,76(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 76, temp.u32);
  // stfs f0,92(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 92, temp.u32);
  // stfs f0,108(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 108, temp.u32);
  // stfs f0,124(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 124, temp.u32);
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA3C8: lifted from generated/ by lift.py.
lifted::Result f825DA3C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA3C8 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-19824
  r3 = uint64_t(int64_t(int64_t(r10) + -19824));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-17312
  r11 = uint64_t(int64_t(int64_t(r11) + -17312));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,44(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f0,60(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 60, temp.u32);
  // stfs f0,76(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 76, temp.u32);
  // stfs f0,92(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 92, temp.u32);
  // stfs f0,108(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 108, temp.u32);
  // stfs f0,124(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 124, temp.u32);
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA408: lifted from generated/ by lift.py.
lifted::Result f825DA408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA408 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19816
  r3 = uint64_t(int64_t(int64_t(r11) + -19816));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA418: lifted from generated/ by lift.py.
lifted::Result f825DA418(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA418 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19808
  r3 = uint64_t(int64_t(int64_t(r11) + -19808));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA428: lifted from generated/ by lift.py.
lifted::Result f825DA428(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA428 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19800
  r3 = uint64_t(int64_t(int64_t(r11) + -19800));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA438: lifted from generated/ by lift.py.
lifted::Result f825DA438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825DA438 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,-17140
  r3 = uint64_t(int64_t(int64_t(r11) + -17140));
  // bl 0x8258e548
  // (left out: dead in C++)
  { const lifted::Result o = f8258E548(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19792
  r3 = uint64_t(int64_t(int64_t(r11) + -19792));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DA470: lifted from generated/ by lift.py.
lifted::Result f825DA470(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825DA470 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,-17100
  r3 = uint64_t(int64_t(int64_t(r11) + -17100));
  // bl 0x8258dcc8
  // (left out: dead in C++)
  { const lifted::Result o = f8258DCC8(base, r1, r13, r3, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19776
  r3 = uint64_t(int64_t(int64_t(r11) + -19776));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DA4A8: lifted from generated/ by lift.py.
lifted::Result f825DA4A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825DA4A8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,-17096
  r3 = uint64_t(int64_t(int64_t(r11) + -17096));
  // bl 0x8258e548
  // (left out: dead in C++)
  { const lifted::Result o = f8258E548(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19760
  r3 = uint64_t(int64_t(int64_t(r11) + -19760));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DA4E0: lifted from generated/ by lift.py.
lifted::Result f825DA4E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA4E0 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,12
  r3 = uint64_t(int64_t(12));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825da500
  if (cr6.eq) goto loc_825DA500;
  // stw r3,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r3));
loc_825DA500:
  // addi r11,r3,4
  r11 = uint64_t(int64_t(int64_t(r3) + 4));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825da510
  if (cr6.eq) goto loc_825DA510;
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
loc_825DA510:
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r11,r11,-17068
  r11 = uint64_t(int64_t(int64_t(r11) + -17068));
  // stw r3,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r3));
  // addi r3,r10,-19744
  r3 = uint64_t(int64_t(int64_t(r10) + -19744));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DA5C0: lifted from generated/ by lift.py.
lifted::Result f825DA5C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DA5C0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19496
  r3 = uint64_t(int64_t(int64_t(r11) + -19496));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DA648: lifted from generated/ by lift.py.
void f825DA648(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DA648 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // addi r8,r11,-19660
  r8 = uint64_t(int64_t(int64_t(r11) + -19660));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,3908
  r11 = uint64_t(int64_t(int64_t(r11) + 3908));
  // lwz r9,-16(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + -16);
  // mr r7,r9
  r7 = r9;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
loc_825DA664:
  // lbz r10,0(r9)
  r10 = REX_LOAD_U8(uint32_t(r9) + 0);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x825da664
  if (!cr6.eq) goto loc_825DA664;
  // subf r7,r7,r9
  r7 = r9 - r7;
  // lwz r10,-12(r8)
  r10 = REX_LOAD_U32(uint32_t(r8) + -12);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
  // addi r9,r7,-1
  r9 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // mr r7,r10
  r7 = r10;
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
loc_825DA694:
  // lbz r9,0(r10)
  r9 = REX_LOAD_U8(uint32_t(r10) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x825da694
  if (!cr6.eq) goto loc_825DA694;
  // subf r7,r7,r10
  r7 = r10 - r7;
  // lwz r9,-8(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + -8);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // addi r10,r7,-1
  r10 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r9,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r9));
  // mr r7,r9
  r7 = r9;
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
loc_825DA6C4:
  // lbz r10,0(r9)
  r10 = REX_LOAD_U8(uint32_t(r9) + 0);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x825da6c4
  if (!cr6.eq) goto loc_825DA6C4;
  // subf r7,r7,r9
  r7 = r9 - r7;
  // lwz r10,-4(r8)
  r10 = REX_LOAD_U32(uint32_t(r8) + -4);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r9));
  // addi r9,r7,-1
  r9 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // mr r7,r10
  r7 = r10;
  // stw r9,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r9));
loc_825DA6F4:
  // lbz r9,0(r10)
  r9 = REX_LOAD_U8(uint32_t(r10) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x825da6f4
  if (!cr6.eq) goto loc_825DA6F4;
  // lwz r9,0(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + 0);
  // subf r8,r7,r10
  r8 = r10 - r7;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,44(r11)
  REX_STORE_U32(uint32_t(r11) + 44, uint32_t(r10));
  // addi r10,r8,-1
  r10 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r9,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r9));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // mr r10,r9
  r10 = r9;
loc_825DA724:
  // lbz r8,0(r9)
  r8 = REX_LOAD_U8(uint32_t(r9) + 0);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // bne cr6,0x825da724
  if (!cr6.eq) goto loc_825DA724;
  // subf r9,r10,r9
  r9 = r9 - r10;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r10));
  // addi r10,r9,-1
  r10 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // blr
  return;
}

// sub_825DA750: lifted from generated/ by lift.py.
lifted::Result f825DA750(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825DA750 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f31,-32(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -32, std::bit_cast<uint64_t>(f31));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f2,8792(r11)
  f2 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 8792)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f31,22232(r11)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 22232)));
  // fmr f1,f31
  f1 = f31;
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r30,-32249
  r30 = uint64_t(int64_t(-2113470464));
  // fmr f1,f31
  f1 = f31;
  // lfd f2,8784(r11)
  f2 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 8784)));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r31,r11,3968
  r31 = uint64_t(int64_t(int64_t(r11) + 3968));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // fmr f2,f31
  f2 = f31;
  // fmr f1,f31
  f1 = f31;
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,4(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmr f1,f31
  f1 = f31;
  // lfd f2,-31160(r11)
  f2 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -31160)));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,8(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 8, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // fmr f1,f31
  f1 = f31;
  // lfd f2,-18120(r11)
  f2 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -18120)));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,12(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 12, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // fmr f1,f31
  f1 = f31;
  // lfd f2,-9632(r11)
  f2 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -9632)));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,16(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 16, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmr f1,f31
  f1 = f31;
  // lfd f2,8840(r11)
  f2 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 8840)));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,20(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 20, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // fmr f1,f31
  f1 = f31;
  // lfd f2,-9640(r11)
  f2 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -9640)));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,24(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 24, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32252
  r11 = uint64_t(int64_t(-2113667072));
  // fmr f1,f31
  f1 = f31;
  // lfd f2,12936(r11)
  f2 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 12936)));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,28(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 28, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // frsp f13,f0
  f13 = double(float(f0));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmr f1,f31
  f1 = f31;
  // lfd f2,8832(r11)
  f2 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 8832)));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,32(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 32, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lfs f0,8796(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8796);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,36(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-32(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825DA8F8: lifted from generated/ by lift.py.
lifted::Result f825DA8F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825DA8F8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f31,-32(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -32, std::bit_cast<uint64_t>(f31));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r31,r11,3968
  r31 = uint64_t(int64_t(int64_t(r11) + 3968));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // lfs f31,8848(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8848);
  f31 = double(temp.f32);
  // fmuls f1,f0,f31
  f1 = double(float(f0 * f31));
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lfs f13,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // addi r30,r11,4008
  r30 = uint64_t(int64_t(int64_t(r11) + 4008));
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,4(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 4, temp.u32);
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,12(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,8(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 8, temp.u32);
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,12(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 12, temp.u32);
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 16, temp.u32);
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,20(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 20, temp.u32);
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,28(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 28);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,24(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 24, temp.u32);
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,32(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 32);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,28(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 28, temp.u32);
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,36(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f13 = double(temp.f32);
  // fmuls f1,f13,f31
  f1 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,32(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 32, temp.u32);
  // bl 0x825a8a50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8A50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // stfs f0,36(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 36, temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-32(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825DAA30: lifted from generated/ by lift.py.
double f825DAA30(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825DAA30 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f30,-40(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f30));
  // stfd f31,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r31,r11,3968
  r31 = uint64_t(int64_t(int64_t(r11) + 3968));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // lfs f31,8124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8124);
  f31 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // lfs f30,8852(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8852);
  f30 = double(temp.f32);
  // fmuls f1,f0,f30
  f1 = double(float(f0 * f30));
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r30,r11,4048
  r30 = uint64_t(int64_t(int64_t(r11) + 4048));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f13,f30
  f1 = double(float(f13 * f30));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f13,f30
  f1 = double(float(f13 * f30));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,4(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 4, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,12(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f13,f30
  f1 = double(float(f13 * f30));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,8(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 8, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f13,f30
  f1 = double(float(f13 * f30));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,12(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 12, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f13,f30
  f1 = double(float(f13 * f30));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,16(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 16, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f13,f30
  f1 = double(float(f13 * f30));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,20(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 20, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,28(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 28);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f13,f30
  f1 = double(float(f13 * f30));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,24(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 24, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lfs f13,32(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 32);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f13,f30
  f1 = double(float(f13 * f30));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,28(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 28, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lfs f13,36(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f13 = double(temp.f32);
  // fmr f0,f1
  f0 = f1;
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f13,f30
  f1 = double(float(f13 * f30));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,32(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 32, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,36(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 36, temp.u32);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f30,-40(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // lfd f31,-32(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return f1;
}

// sub_825DABC8: lifted from generated/ by lift.py.
lifted::Result f825DABC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825DABC8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,4224
  r3 = uint64_t(int64_t(int64_t(r11) + 4224));
  // bl 0x825a6bd0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6BD0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19416
  r3 = uint64_t(int64_t(int64_t(r11) + -19416));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DAC00: lifted from generated/ by lift.py.
lifted::Result f825DAC00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DAC00 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19400
  r3 = uint64_t(int64_t(int64_t(r11) + -19400));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DAC10: lifted from generated/ by lift.py.
lifted::Result f825DAC10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825DAC10 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,4408
  r3 = uint64_t(int64_t(int64_t(r11) + 4408));
  // bl 0x825a6bd0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6BD0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19320
  r3 = uint64_t(int64_t(int64_t(r11) + -19320));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DAC48: lifted from generated/ by lift.py.
lifted::Result f825DAC48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DAC48 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-19304
  r3 = uint64_t(int64_t(int64_t(r11) + -19304));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DACA8: lifted from generated/ by lift.py.
uint64_t f825DACA8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825DACA8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f1 = 0.0;
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // addi r3,r11,832
  r3 = uint64_t(int64_t(int64_t(r11) + 832));
  // b 0x8258e530
  f8258E530(base, r3);
  return r3;
}

// sub_825DACB8: lifted from generated/ by lift.py.
lifted::Result f825DACB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DACB8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,-21752
  r3 = uint64_t(int64_t(int64_t(r11) + -21752));
  // b 0x82294d38
  { const lifted::Result o = f82294D38(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

namespace {

lifted::Result e825D97A0(uint8_t* base, const lifted::Args& a) {
  return f825D97A0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D97B0(uint8_t* base, const lifted::Args& a) {
  return f825D97B0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D97C0(uint8_t* base, const lifted::Args& a) {
  f825D97C0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D97E8(uint8_t* base, const lifted::Args& a) {
  f825D97E8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9810(uint8_t* base, const lifted::Args& a) {
  f825D9810(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9828(uint8_t* base, const lifted::Args& a) {
  f825D9828(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9840(uint8_t* base, const lifted::Args& a) {
  return f825D9840(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9850(uint8_t* base, const lifted::Args& a) {
  return f825D9850(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9860(uint8_t* base, const lifted::Args& a) {
  f825D9860(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9888(uint8_t* base, const lifted::Args& a) {
  f825D9888(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D98B0(uint8_t* base, const lifted::Args& a) {
  f825D98B0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D98D8(uint8_t* base, const lifted::Args& a) {
  f825D98D8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9900(uint8_t* base, const lifted::Args& a) {
  f825D9900(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9928(uint8_t* base, const lifted::Args& a) {
  f825D9928(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9950(uint8_t* base, const lifted::Args& a) {
  f825D9950(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9978(uint8_t* base, const lifted::Args& a) {
  f825D9978(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9A10(uint8_t* base, const lifted::Args& a) {
  return f825D9A10(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9A20(uint8_t* base, const lifted::Args& a) {
  return f825D9A20(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9A30(uint8_t* base, const lifted::Args& a) {
  return f825D9A30(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9A40(uint8_t* base, const lifted::Args& a) {
  return f825D9A40(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9A50(uint8_t* base, const lifted::Args& a) {
  return f825D9A50(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9A60(uint8_t* base, const lifted::Args& a) {
  return f825D9A60(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9A70(uint8_t* base, const lifted::Args& a) {
  return f825D9A70(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9A80(uint8_t* base, const lifted::Args& a) {
  return f825D9A80(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9A90(uint8_t* base, const lifted::Args& a) {
  return f825D9A90(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9AA0(uint8_t* base, const lifted::Args& a) {
  return f825D9AA0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9AB0(uint8_t* base, const lifted::Args& a) {
  return f825D9AB0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9AC0(uint8_t* base, const lifted::Args& a) {
  return f825D9AC0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9AD0(uint8_t* base, const lifted::Args& a) {
  return f825D9AD0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9AE0(uint8_t* base, const lifted::Args& a) {
  return f825D9AE0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9AF0(uint8_t* base, const lifted::Args& a) {
  return f825D9AF0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9B00(uint8_t* base, const lifted::Args& a) {
  return f825D9B00(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9B10(uint8_t* base, const lifted::Args& a) {
  return f825D9B10(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9B20(uint8_t* base, const lifted::Args& a) {
  return f825D9B20(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9B30(uint8_t* base, const lifted::Args& a) {
  return f825D9B30(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9B40(uint8_t* base, const lifted::Args& a) {
  return f825D9B40(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9B50(uint8_t* base, const lifted::Args& a) {
  return f825D9B50(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9B88(uint8_t* base, const lifted::Args& a) {
  return f825D9B88(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9B98(uint8_t* base, const lifted::Args& a) {
  return f825D9B98(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9BD0(uint8_t* base, const lifted::Args& a) {
  return f825D9BD0(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9C58(uint8_t* base, const lifted::Args& a) {
  return f825D9C58(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825D9CA0(uint8_t* base, const lifted::Args& a) {
  f825D9CA0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9CC8(uint8_t* base, const lifted::Args& a) {
  f825D9CC8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9CF0(uint8_t* base, const lifted::Args& a) {
  return f825D9CF0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9D00(uint8_t* base, const lifted::Args& a) {
  f825D9D00(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9D28(uint8_t* base, const lifted::Args& a) {
  f825D9D28(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9D50(uint8_t* base, const lifted::Args& a) {
  f825D9D50(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9D78(uint8_t* base, const lifted::Args& a) {
  f825D9D78(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9DA0(uint8_t* base, const lifted::Args& a) {
  f825D9DA0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9DC8(uint8_t* base, const lifted::Args& a) {
  f825D9DC8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9DF0(uint8_t* base, const lifted::Args& a) {
  f825D9DF0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9E18(uint8_t* base, const lifted::Args& a) {
  f825D9E18(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9E40(uint8_t* base, const lifted::Args& a) {
  return f825D9E40(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9E50(uint8_t* base, const lifted::Args& a) {
  return f825D9E50(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9EC0(uint8_t* base, const lifted::Args& a) {
  return f825D9EC0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9ED0(uint8_t* base, const lifted::Args& a) {
  return f825D9ED0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9EE0(uint8_t* base, const lifted::Args& a) {
  return f825D9EE0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9F48(uint8_t* base, const lifted::Args& a) {
  return f825D9F48(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9F58(uint8_t* base, const lifted::Args& a) {
  return f825D9F58(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9FC8(uint8_t* base, const lifted::Args& a) {
  return {f825D9FC8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e825DA050(uint8_t* base, const lifted::Args& a) {
  return {f825DA050(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e825DA090(uint8_t* base, const lifted::Args& a) {
  return {f825DA090(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e825DA0B8(uint8_t* base, const lifted::Args& a) {
  return f825DA0B8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825DA0F0(uint8_t* base, const lifted::Args& a) {
  return f825DA0F0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825DA128(uint8_t* base, const lifted::Args& a) {
  return {f825DA128(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e825DA140(uint8_t* base, const lifted::Args& a) {
  return f825DA140(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825DA178(uint8_t* base, const lifted::Args& a) {
  return f825DA178(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825DA1B0(uint8_t* base, const lifted::Args& a) {
  return f825DA1B0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA1C0(uint8_t* base, const lifted::Args& a) {
  return f825DA1C0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA1D0(uint8_t* base, const lifted::Args& a) {
  return f825DA1D0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA1E0(uint8_t* base, const lifted::Args& a) {
  return f825DA1E0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA1F0(uint8_t* base, const lifted::Args& a) {
  return f825DA1F0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA200(uint8_t* base, const lifted::Args& a) {
  return f825DA200(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA210(uint8_t* base, const lifted::Args& a) {
  return f825DA210(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825DA238(uint8_t* base, const lifted::Args& a) {
  return f825DA238(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825DA260(uint8_t* base, const lifted::Args& a) {
  return f825DA260(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825DA288(uint8_t* base, const lifted::Args& a) {
  return f825DA288(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825DA2C8(uint8_t* base, const lifted::Args& a) {
  return f825DA2C8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825DA308(uint8_t* base, const lifted::Args& a) {
  return f825DA308(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825DA348(uint8_t* base, const lifted::Args& a) {
  return f825DA348(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825DA388(uint8_t* base, const lifted::Args& a) {
  return f825DA388(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825DA3C8(uint8_t* base, const lifted::Args& a) {
  return f825DA3C8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825DA408(uint8_t* base, const lifted::Args& a) {
  return f825DA408(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA418(uint8_t* base, const lifted::Args& a) {
  return f825DA418(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA428(uint8_t* base, const lifted::Args& a) {
  return f825DA428(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA438(uint8_t* base, const lifted::Args& a) {
  return f825DA438(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825DA470(uint8_t* base, const lifted::Args& a) {
  return f825DA470(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825DA4A8(uint8_t* base, const lifted::Args& a) {
  return f825DA4A8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825DA4E0(uint8_t* base, const lifted::Args& a) {
  return f825DA4E0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA5C0(uint8_t* base, const lifted::Args& a) {
  return f825DA5C0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DA648(uint8_t* base, const lifted::Args& a) {
  f825DA648(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DA750(uint8_t* base, const lifted::Args& a) {
  return f825DA750(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e825DA8F8(uint8_t* base, const lifted::Args& a) {
  return f825DA8F8(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825DAA30(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825DAA30(base, a.r1, a.r13)};
}

lifted::Result e825DABC8(uint8_t* base, const lifted::Args& a) {
  return f825DABC8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825DAC00(uint8_t* base, const lifted::Args& a) {
  return f825DAC00(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DAC10(uint8_t* base, const lifted::Args& a) {
  return f825DAC10(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825DAC48(uint8_t* base, const lifted::Args& a) {
  return f825DAC48(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DACA8(uint8_t* base, const lifted::Args& a) {
  return {f825DACA8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e825DACB8(uint8_t* base, const lifted::Args& a) {
  return f825DACB8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

const Registration kEntries[] = {
    {0x825D97A0, e825D97A0},
    {0x825D97B0, e825D97B0},
    {0x825D97C0, e825D97C0},
    {0x825D97E8, e825D97E8},
    {0x825D9810, e825D9810},
    {0x825D9828, e825D9828},
    {0x825D9840, e825D9840},
    {0x825D9850, e825D9850},
    {0x825D9860, e825D9860},
    {0x825D9888, e825D9888},
    {0x825D98B0, e825D98B0},
    {0x825D98D8, e825D98D8},
    {0x825D9900, e825D9900},
    {0x825D9928, e825D9928},
    {0x825D9950, e825D9950},
    {0x825D9978, e825D9978},
    {0x825D9A10, e825D9A10},
    {0x825D9A20, e825D9A20},
    {0x825D9A30, e825D9A30},
    {0x825D9A40, e825D9A40},
    {0x825D9A50, e825D9A50},
    {0x825D9A60, e825D9A60},
    {0x825D9A70, e825D9A70},
    {0x825D9A80, e825D9A80},
    {0x825D9A90, e825D9A90},
    {0x825D9AA0, e825D9AA0},
    {0x825D9AB0, e825D9AB0},
    {0x825D9AC0, e825D9AC0},
    {0x825D9AD0, e825D9AD0},
    {0x825D9AE0, e825D9AE0},
    {0x825D9AF0, e825D9AF0},
    {0x825D9B00, e825D9B00},
    {0x825D9B10, e825D9B10},
    {0x825D9B20, e825D9B20},
    {0x825D9B30, e825D9B30},
    {0x825D9B40, e825D9B40},
    {0x825D9B50, e825D9B50},
    {0x825D9B88, e825D9B88},
    {0x825D9B98, e825D9B98},
    {0x825D9BD0, e825D9BD0},
    {0x825D9C58, e825D9C58},
    {0x825D9CA0, e825D9CA0},
    {0x825D9CC8, e825D9CC8},
    {0x825D9CF0, e825D9CF0},
    {0x825D9D00, e825D9D00},
    {0x825D9D28, e825D9D28},
    {0x825D9D50, e825D9D50},
    {0x825D9D78, e825D9D78},
    {0x825D9DA0, e825D9DA0},
    {0x825D9DC8, e825D9DC8},
    {0x825D9DF0, e825D9DF0},
    {0x825D9E18, e825D9E18},
    {0x825D9E40, e825D9E40},
    {0x825D9E50, e825D9E50},
    {0x825D9EC0, e825D9EC0},
    {0x825D9ED0, e825D9ED0},
    {0x825D9EE0, e825D9EE0},
    {0x825D9F48, e825D9F48},
    {0x825D9F58, e825D9F58},
    {0x825D9FC8, e825D9FC8},
    {0x825DA050, e825DA050},
    {0x825DA090, e825DA090},
    {0x825DA0B8, e825DA0B8},
    {0x825DA0F0, e825DA0F0},
    {0x825DA128, e825DA128},
    {0x825DA140, e825DA140},
    {0x825DA178, e825DA178},
    {0x825DA1B0, e825DA1B0},
    {0x825DA1C0, e825DA1C0},
    {0x825DA1D0, e825DA1D0},
    {0x825DA1E0, e825DA1E0},
    {0x825DA1F0, e825DA1F0},
    {0x825DA200, e825DA200},
    {0x825DA210, e825DA210},
    {0x825DA238, e825DA238},
    {0x825DA260, e825DA260},
    {0x825DA288, e825DA288},
    {0x825DA2C8, e825DA2C8},
    {0x825DA308, e825DA308},
    {0x825DA348, e825DA348},
    {0x825DA388, e825DA388},
    {0x825DA3C8, e825DA3C8},
    {0x825DA408, e825DA408},
    {0x825DA418, e825DA418},
    {0x825DA428, e825DA428},
    {0x825DA438, e825DA438},
    {0x825DA470, e825DA470},
    {0x825DA4A8, e825DA4A8},
    {0x825DA4E0, e825DA4E0},
    {0x825DA5C0, e825DA5C0},
    {0x825DA648, e825DA648},
    {0x825DA750, e825DA750},
    {0x825DA8F8, e825DA8F8},
    {0x825DAA30, e825DAA30},
    {0x825DABC8, e825DABC8},
    {0x825DAC00, e825DAC00},
    {0x825DAC10, e825DAC10},
    {0x825DAC48, e825DAC48},
    {0x825DACA8, e825DACA8},
    {0x825DACB8, e825DACB8},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_026)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_825D97A0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D97A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D97A0);
}

extern "C" REX_FUNC(sub_825D97B0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D97B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D97B0);
}

extern "C" REX_FUNC(sub_825D97C0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D97C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D97C0);
}

extern "C" REX_FUNC(sub_825D97E8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D97E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D97E8);
}

extern "C" REX_FUNC(sub_825D9810) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9810(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9810);
}

extern "C" REX_FUNC(sub_825D9828) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9828(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9828);
}

extern "C" REX_FUNC(sub_825D9840) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9840(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9840);
}

extern "C" REX_FUNC(sub_825D9850) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9850(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9850);
}

extern "C" REX_FUNC(sub_825D9860) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9860(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9860);
}

extern "C" REX_FUNC(sub_825D9888) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9888(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9888);
}

extern "C" REX_FUNC(sub_825D98B0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D98B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D98B0);
}

extern "C" REX_FUNC(sub_825D98D8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D98D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D98D8);
}

extern "C" REX_FUNC(sub_825D9900) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9900(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9900);
}

extern "C" REX_FUNC(sub_825D9928) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9928(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9928);
}

extern "C" REX_FUNC(sub_825D9950) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9950(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9950);
}

extern "C" REX_FUNC(sub_825D9978) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9978(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9978);
}

extern "C" REX_FUNC(sub_825D9A10) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9A10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9A10);
}

extern "C" REX_FUNC(sub_825D9A20) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9A20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9A20);
}

extern "C" REX_FUNC(sub_825D9A30) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9A30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9A30);
}

extern "C" REX_FUNC(sub_825D9A40) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9A40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9A40);
}

extern "C" REX_FUNC(sub_825D9A50) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9A50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9A50);
}

extern "C" REX_FUNC(sub_825D9A60) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9A60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9A60);
}

extern "C" REX_FUNC(sub_825D9A70) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9A70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9A70);
}

extern "C" REX_FUNC(sub_825D9A80) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9A80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9A80);
}

extern "C" REX_FUNC(sub_825D9A90) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9A90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9A90);
}

extern "C" REX_FUNC(sub_825D9AA0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9AA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9AA0);
}

extern "C" REX_FUNC(sub_825D9AB0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9AB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9AB0);
}

extern "C" REX_FUNC(sub_825D9AC0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9AC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9AC0);
}

extern "C" REX_FUNC(sub_825D9AD0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9AD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9AD0);
}

extern "C" REX_FUNC(sub_825D9AE0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9AE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9AE0);
}

extern "C" REX_FUNC(sub_825D9AF0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9AF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9AF0);
}

extern "C" REX_FUNC(sub_825D9B00) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9B00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9B00);
}

extern "C" REX_FUNC(sub_825D9B10) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9B10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9B10);
}

extern "C" REX_FUNC(sub_825D9B20) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9B20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9B20);
}

extern "C" REX_FUNC(sub_825D9B30) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9B30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9B30);
}

extern "C" REX_FUNC(sub_825D9B40) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9B40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9B40);
}

extern "C" REX_FUNC(sub_825D9B50) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9B50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9B50);
}

extern "C" REX_FUNC(sub_825D9B88) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9B88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9B88);
}

extern "C" REX_FUNC(sub_825D9B98) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9B98(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9B98);
}

extern "C" REX_FUNC(sub_825D9BD0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9BD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9BD0);
}

extern "C" REX_FUNC(sub_825D9C58) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9C58(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9C58);
}

extern "C" REX_FUNC(sub_825D9CA0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9CA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9CA0);
}

extern "C" REX_FUNC(sub_825D9CC8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9CC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9CC8);
}

extern "C" REX_FUNC(sub_825D9CF0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9CF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9CF0);
}

extern "C" REX_FUNC(sub_825D9D00) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9D00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9D00);
}

extern "C" REX_FUNC(sub_825D9D28) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9D28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9D28);
}

extern "C" REX_FUNC(sub_825D9D50) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9D50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9D50);
}

extern "C" REX_FUNC(sub_825D9D78) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9D78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9D78);
}

extern "C" REX_FUNC(sub_825D9DA0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9DA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9DA0);
}

extern "C" REX_FUNC(sub_825D9DC8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9DC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9DC8);
}

extern "C" REX_FUNC(sub_825D9DF0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9DF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9DF0);
}

extern "C" REX_FUNC(sub_825D9E18) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9E18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9E18);
}

extern "C" REX_FUNC(sub_825D9E40) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9E40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9E40);
}

extern "C" REX_FUNC(sub_825D9E50) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9E50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9E50);
}

extern "C" REX_FUNC(sub_825D9EC0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9EC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9EC0);
}

extern "C" REX_FUNC(sub_825D9ED0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9ED0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9ED0);
}

extern "C" REX_FUNC(sub_825D9EE0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9EE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9EE0);
}

extern "C" REX_FUNC(sub_825D9F48) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9F48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9F48);
}

extern "C" REX_FUNC(sub_825D9F58) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9F58(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9F58);
}

extern "C" REX_FUNC(sub_825D9FC8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825D9FC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9FC8);
}

extern "C" REX_FUNC(sub_825DA050) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA050(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA050);
}

extern "C" REX_FUNC(sub_825DA090) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA090(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA090);
}

extern "C" REX_FUNC(sub_825DA0B8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA0B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA0B8);
}

extern "C" REX_FUNC(sub_825DA0F0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA0F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA0F0);
}

extern "C" REX_FUNC(sub_825DA128) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA128(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA128);
}

extern "C" REX_FUNC(sub_825DA140) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA140(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA140);
}

extern "C" REX_FUNC(sub_825DA178) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA178(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA178);
}

extern "C" REX_FUNC(sub_825DA1B0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA1B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA1B0);
}

extern "C" REX_FUNC(sub_825DA1C0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA1C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA1C0);
}

extern "C" REX_FUNC(sub_825DA1D0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA1D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA1D0);
}

extern "C" REX_FUNC(sub_825DA1E0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA1E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA1E0);
}

extern "C" REX_FUNC(sub_825DA1F0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA1F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA1F0);
}

extern "C" REX_FUNC(sub_825DA200) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA200(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA200);
}

extern "C" REX_FUNC(sub_825DA210) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA210(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA210);
}

extern "C" REX_FUNC(sub_825DA238) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA238(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA238);
}

extern "C" REX_FUNC(sub_825DA260) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA260(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA260);
}

extern "C" REX_FUNC(sub_825DA288) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA288(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA288);
}

extern "C" REX_FUNC(sub_825DA2C8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA2C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA2C8);
}

extern "C" REX_FUNC(sub_825DA308) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA308(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA308);
}

extern "C" REX_FUNC(sub_825DA348) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA348(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA348);
}

extern "C" REX_FUNC(sub_825DA388) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA388(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA388);
}

extern "C" REX_FUNC(sub_825DA3C8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA3C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA3C8);
}

extern "C" REX_FUNC(sub_825DA408) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA408(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA408);
}

extern "C" REX_FUNC(sub_825DA418) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA418(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA418);
}

extern "C" REX_FUNC(sub_825DA428) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA428(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA428);
}

extern "C" REX_FUNC(sub_825DA438) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA438(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA438);
}

extern "C" REX_FUNC(sub_825DA470) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA470(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA470);
}

extern "C" REX_FUNC(sub_825DA4A8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA4A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA4A8);
}

extern "C" REX_FUNC(sub_825DA4E0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA4E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA4E0);
}

extern "C" REX_FUNC(sub_825DA5C0) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA5C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA5C0);
}

extern "C" REX_FUNC(sub_825DA648) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA648(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA648);
}

extern "C" REX_FUNC(sub_825DA750) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA750(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA750);
}

extern "C" REX_FUNC(sub_825DA8F8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DA8F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA8F8);
}

extern "C" REX_FUNC(sub_825DAA30) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DAA30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DAA30);
}

extern "C" REX_FUNC(sub_825DABC8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DABC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DABC8);
}

extern "C" REX_FUNC(sub_825DAC00) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DAC00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DAC00);
}

extern "C" REX_FUNC(sub_825DAC10) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DAC10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DAC10);
}

extern "C" REX_FUNC(sub_825DAC48) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DAC48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DAC48);
}

extern "C" REX_FUNC(sub_825DACA8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DACA8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DACA8);
}

extern "C" REX_FUNC(sub_825DACB8) {
  if (!ITC_FLAG(native_lifted_026)) {
    __imp__sub_825DACB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DACB8);
}
