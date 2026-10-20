#include "display/gamma.h"

#include <algorithm>
#include <climits>
#include <cmath>

namespace display {
namespace {

// The constants as the image has them (hex: exact). Single-precision ones
// are floats there, the others doubles; several doubles are float values.
constexpr float kSrgbScale = 0x1.0e147ap+0f;      // 1.055
constexpr float kSrgbOffset = 0x1.c28f5cp-5f;     // 0.055
constexpr float kSrgbInverseScale = 0x1.e54edep-1f;  // 1 / 1.055
constexpr float kSrgbLinearScale = 0x1.9d70a4p+3f;   // 12.92
constexpr float kSrgbInverseLinearScale = 0x1.3d0722p-4f;  // 1 / 12.92
constexpr double kSrgbEncodeKnee = 0x1.9a5c37387b719p-9;   // 0.0031308
constexpr double kSrgbDecodeKnee = 0x1.4b5dcc63f1412p-5;   // 0.04045
constexpr double kSrgbEncodePower = 0x1.aaaaaap-2;         // float(1 / 2.4)
constexpr double kSrgbDecodePower = 0x1.333334p+1;         // float(2.4)

constexpr float kBt709Scale = 0x1.195810p+0f;      // 1.099
constexpr float kBt709Offset = 0x1.958106p-4f;     // 0.099
constexpr float kBt709InverseScale = 0x1.d1e0cap-1f;  // 1 / 1.099
constexpr float kBt709LinearScale = 0x1.2p+2f;        // 4.5
constexpr float kBt709InverseLinearScale = 0x1.c71c72p-3f;  // 1 / 4.5
constexpr double kBt709EncodeKnee = 0x1.26e978d4fdf3bp-6;   // 0.018
constexpr double kBt709DecodeKnee = 0x1.4c985f06f6944p-4;   // 0.0812
constexpr double kBt709EncodePower = 0x1.ccccccp-2;         // float(0.45)
constexpr double kBt709DecodePower = 0x1.1c71c8p+1;         // float(1 / 0.45)

constexpr float kInverse1023 = 0x1.00401p-10f;  // float(1 / 1023)

// x * scale - offset rounded once, then to float (the PowerPC's fmsubs, as
// the recompiler computes it).
float MultiplySubtract(float x, float scale, float offset) {
  return float(std::fma(double(x), double(scale), -double(offset)));
}

// The PowerPC's fctiwz: toward zero, NaN and too small to INT_MIN, too big
// to INT_MAX.
int32_t TruncateToInt32(double x) {
  if (std::isnan(x) || x < double(INT_MIN)) {
    return INT_MIN;
  }
  if (x >= double(INT_MAX)) {
    return INT_MAX;
  }
  return int32_t(x);
}

}  // namespace

double Pow(double x, double y) {
  return std::pow(x, y);
}

float Srgb(float x, bool to_linear) {
  if (to_linear) {
    if (x > kSrgbDecodeKnee) {
      return float(Pow((x + kSrgbOffset) * kSrgbInverseScale, kSrgbDecodePower));
    }
    return x * kSrgbInverseLinearScale;
  }
  if (x > kSrgbEncodeKnee) {
    return MultiplySubtract(float(Pow(x, kSrgbEncodePower)), kSrgbScale, kSrgbOffset);
  }
  return x * kSrgbLinearScale;
}

float Bt709(float x, bool to_linear) {
  if (to_linear) {
    if (x >= kBt709DecodeKnee) {
      return float(Pow((x + kBt709Offset) * kBt709InverseScale, kBt709DecodePower));
    }
    return x * kBt709InverseLinearScale;
  }
  if (x >= kBt709EncodeKnee) {
    return MultiplySubtract(float(Pow(x, kBt709EncodePower)), kBt709Scale, kBt709Offset);
  }
  return x * kBt709LinearScale;
}

GammaCurve MakeGammaCurve(const Gamma& gamma, bool inverse) {
  GammaCurve curve;
  for (int i = 0; i < 1024; i++) {
    float x = float(i) * kInverse1023;
    if (!inverse) {
      x = Srgb(x, true);
    }
    if (gamma.type == 1) {
      x = Srgb(x, inverse);
    } else if (gamma.type == 3) {
      x = float(Pow(x, inverse ? gamma.power : 1.0f / gamma.power));
    } else {
      x = Bt709(x, inverse);
    }
    if (inverse) {
      x = Srgb(x, false);
    }
    int32_t value = TruncateToInt32(std::fma(double(x), 1023.0, 0.5));
    curve[i] = uint16_t(std::clamp(value, 0, 1023));
  }
  return curve;
}

GammaRamp ApplyGammaCurve(const GammaCurve& curve, const GammaRamp& ramp) {
  GammaRamp out;
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 256; i++) {
      out[c][i] = uint16_t(curve[ramp[c][i] >> 6] << 6);
    }
  }
  return out;
}

GammaRampPwl ApplyGammaCurve(const GammaCurve& curve, const GammaRampPwl& ramp) {
  GammaRampPwl out;
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 128; i++) {
      uint32_t start = ramp[c][i].base >> 6;
      uint32_t end = std::min<uint32_t>(start + (ramp[c][i].delta >> 6), 1023);
      out[c][i].base = uint16_t(curve[start] << 6);
      // Wraps when the curve falls, as the game's does.
      out[c][i].delta = uint16_t((uint32_t(curve[end]) - curve[start]) << 6);
    }
  }
  return out;
}

}  // namespace display
