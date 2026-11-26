// SBX's normals target (--xegtao; passes.cpp, "View targets"): the
// surface's smooth (interpolated) world-space normal, at location 1
// (xe_out_normal, game/xe_interface.glsl), as R32G32_UINT: .x the normal *
// 0.5 + 0.5 packed 11_11_10 (as XeGTAO packs its own), .y the fragment's
// clip w (its view distance) as float bits, or the sign bit alone where the
// draw takes no AO (xe_normal_distance). Every hand-written shader writes
// the distance with no normal (.x 0) in xe_color_output; the materials with
// a normal write it after that. The view distance is the AO's depth
// (glsl/xegtao/xegtao_depth.comp), and the merge
// (glsl/xegtao/xegtao_normals.comp) takes the normal where that distance is
// the depth's. Dropped where the pipeline has no second target.

uint pack_normal(vec3 unit) {
  unit = clamp(unit * 0.5 + 0.5, 0.0, 1.0);
  return uint(unit.x * 2047.0 + 0.5) | (uint(unit.y * 2047.0 + 0.5) << 11) |
         (uint(unit.z * 1023.0 + 0.5) << 22);
}

void write_normal(vec3 world_normal) {
  float length_squared = dot(world_normal, world_normal);
  xe_out_normal = uvec2(length_squared > 1e-12
                            ? pack_normal(world_normal * inversesqrt(length_squared))
                            : 0u,
                        xe_normal_distance());
}
