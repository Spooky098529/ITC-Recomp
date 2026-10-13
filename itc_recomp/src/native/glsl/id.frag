#version 450
// The draw ID pass: each pixel gets the number of the draw that wrote it.

layout(push_constant) uniform Constants {
  uint id;
} constants;

layout(location = 0) out uint id;

void main() {
  id = constants.id;
}
