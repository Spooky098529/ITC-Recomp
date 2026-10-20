#include "strings.h"

namespace helpers {

Conversion PlanConversion(int32_t length, int32_t destination_length) {
  if (destination_length == 0) {
    return Conversion::kMeasure;
  }
  if (length < 0 || destination_length < length) {
    return Conversion::kTooSmall;
  }
  return Conversion::kConvert;
}

void Widen(const uint8_t* in, uint16_t* out, size_t count) {
  for (size_t i = 0; i < count; i++) {
    out[i] = in[i];
  }
}

void Narrow(const uint16_t* in, uint8_t* out, size_t count) {
  for (size_t i = 0; i < count; i++) {
    out[i] = in[i] < 256 ? uint8_t(in[i]) : uint8_t('?');
  }
}

bool FormatWithoutArguments(const std::u16string& format, std::u16string* out) {
  out->clear();
  for (size_t i = 0; i < format.size(); i++) {
    if (format[i] != u'%') {
      *out += format[i];
    } else if (i + 1 < format.size() && format[i + 1] == u'%') {
      *out += u'%';
      i++;
    } else {
      return false;
    }
  }
  return true;
}

}  // namespace helpers
