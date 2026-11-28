#version 450
#extension GL_EXT_samplerless_texture_functions : require
// A view's swizzle for the D3D11 backend (its views have none, as Vulkan's
// do: docs/d3d11-backend.md, open question 4): one level and layer of an
// image copied into the same of its swizzled copy, each component the
// image's R, G, B or A, or 0 or 1. Drawn with resolve.vert over the level.

layout(set = 0, binding = 0) uniform texture2DArray source;

layout(push_constant) uniform Constants {
  ivec4 select;  // a component each: 0 to 3 the image's R to A, 4 zero, 5 one
  int level, layer;
} constants;

layout(location = 0) out vec4 color;

float Component(vec4 texel, int select) {
  return select < 4 ? texel[select] : float(select == 5);
}

void main() {
  vec4 texel = texelFetch(source, ivec3(ivec2(gl_FragCoord.xy), constants.layer), constants.level);
  color = vec4(Component(texel, constants.select.x), Component(texel, constants.select.y),
               Component(texel, constants.select.z), Component(texel, constants.select.w));
}
