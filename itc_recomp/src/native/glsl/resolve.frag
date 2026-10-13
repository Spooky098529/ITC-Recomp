#version 450
// A resolve: each destination pixel from the source pixel at the same place
// plus an offset, its samples averaged, scaled by the exponent bias.

#ifdef MULTISAMPLE
layout(set = 0, binding = 0) uniform sampler2DMS source;
#else
layout(set = 0, binding = 0) uniform sampler2D source;
#endif

layout(push_constant) uniform Constants {
  ivec2 offset;  // source minus destination
  int samples;   // how many to average (1: sample 0 only)
  float scale;
} constants;

layout(location = 0) out vec4 color;

void main() {
  ivec2 position = ivec2(gl_FragCoord.xy) + constants.offset;
#ifdef MULTISAMPLE
  vec4 sum = vec4(0.0);
  for (int i = 0; i < constants.samples; i++) {
    sum += texelFetch(source, position, i);
  }
  color = sum / float(constants.samples) * constants.scale;
#else
  color = texelFetch(source, position, 0) * constants.scale;
#endif
}
