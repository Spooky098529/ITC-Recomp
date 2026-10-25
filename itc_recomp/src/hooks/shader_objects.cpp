// The shader objects natively: adapters in front of src/shader_objects/
// (docs/functions/shader-objects.md).

#include "guest.h"

#include "base/flags.h"
#include "shader_objects/shader_objects.h"

ITC_FLAG_BOOL(native_shader_objects, true, "ITC",
              "Run the graphics library's shader objects (docs/functions/shader-objects.md: "
              "VertexShader::Init sub_822A8740 (an object in the physical views, or a fetch "
              "table sharing memory with its fields, never seen in play, the original's), "
              "PixelShader::Init sub_822A85B8) natively instead of through the translated code "
              "(the same writes; checked by --run_harnesses=shader_objects)");

// VertexShader::Init: r3 = the object, r4 = the microcode. No return value.
extern "C" REX_FUNC(sub_822A8740) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_shader_objects) ||
      !shader_objects::VertexShaderInit(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_822A8740(ctx, base);
  }
}

// PixelShader::Init: r3 = the object, r4 = the microcode. No return value.
extern "C" REX_FUNC(sub_822A85B8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_shader_objects)) {
    __imp__sub_822A85B8(ctx, base);
    return;
  }
  shader_objects::PixelShaderInit(base, ctx.r3.u32, ctx.r4.u32);
}
