// The course's point lights: four, each on when its color's w is over 0.5,
// each with a falloff 1 / (a*d^2 + b*d + c) (PntLightAtten) and a spot cone
// (SpotLightCone: full inside the cosine x, (cosine - y) * z outside).

const int kCourseLights = 4;

bool course_light_on(int i) { return PntLightColor(i).w > 0.5; }

// How a light reaches a point: whether it does (inside the spot cone's
// fade), the direction to it, and its strength.
struct LightReach {
  bool reaches;
  vec3 direction;
  float strength;
};

LightReach course_light_reach(int i, vec3 position) {
  vec3 to_light = PntLightPos(i).xyz - position;
  float distance2 = dot(to_light, to_light);
  vec3 direction = to_light * inversesqrt(distance2);
  vec3 falloff = PntLightAtten(i).xyz;
  float attenuation = 1.0 / (falloff.x * distance2 + falloff.y * sqrt(distance2) + falloff.z);
  vec4 cone = SpotLightCone(i);
  float cosine = dot(direction, SpotLightDir(i).xyz);
  float spot = cosine > cone.x ? 1.0 : (cosine - cone.y) * cone.z;
  return LightReach(spot > 0.0, direction, spot * attenuation);
}
