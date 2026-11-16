// See image_load.h. The format, for this one file (its header read with
// the offsets the XEX2 format has): the header's optional fields (+24, 15
// of them) give the file format (0x3FF: AES-128 encryption, basic
// compression: blocks of data then zeros) and the load address (0x10201);
// the security info (+16's offset) holds the image size (+4) and the
// image's key (+0x150), itself encrypted with the retail key; the data
// starts at +8's offset, encrypted with CBC from a zero IV across all the
// blocks. Plug in data: no other XEX layout is read.

#include "app/image_load.h"

#include "base/flags.h"
#include "base/log.h"

extern "C" {
#include <libavutil/aes.h>
#include <libavutil/mem.h>
#include <libavutil/sha.h>
}

#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iterator>
#include <memory>

ITC_FLAG_BOOL(check_image_load, false, "ITC",
              "Load default.xex with our own loader (src/app/image_load.cpp) as well and log "
              "where its image differs from the SDK's in guest memory (E2 in "
              "docs/rexglue-removal.md)");

namespace app {

namespace {

// The US release's default.xex.
constexpr std::array<uint8_t, 20> kSha1 = {0x32, 0x93, 0x2d, 0xe0, 0xc0, 0x37, 0xab,
                                           0x28, 0x19, 0x2d, 0xfb, 0xed, 0x31, 0x4a,
                                           0xc5, 0xef, 0xcf, 0x40, 0x0f, 0x16};
// The retail key the image keys are encrypted with.
constexpr std::array<uint8_t, 16> kRetailKey = {0x20, 0xB1, 0x85, 0xA5, 0x9D, 0x28,
                                                0xFD, 0xC3, 0x40, 0x58, 0x3F, 0xBB,
                                                0x08, 0x96, 0xBF, 0x91};

constexpr uint32_t kFileFormatKey = 0x3FF;
constexpr uint32_t kLoadAddressKey = 0x10201;
constexpr uint16_t kEncryptionNormal = 1;
constexpr uint16_t kCompressionBasic = 1;
constexpr uint32_t kSecurityImageSize = 4;
constexpr uint32_t kSecurityImageKey = 0x150;

uint32_t Be32(const std::vector<uint8_t>& d, size_t at) {
  return (uint32_t(d[at]) << 24) | (uint32_t(d[at + 1]) << 16) | (uint32_t(d[at + 2]) << 8) |
         d[at + 3];
}

uint16_t Be16(const std::vector<uint8_t>& d, size_t at) {
  return uint16_t((d[at] << 8) | d[at + 1]);
}

struct AesFree {
  void operator()(AVAES* aes) const { av_free(aes); }
};

}  // namespace

bool LoadImage(const std::filesystem::path& path, Image& image, std::string& error) {
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    error = "can't open " + path.string();
    return false;
  }
  const std::vector<uint8_t> file{std::istreambuf_iterator<char>(in), {}};
  {
    std::unique_ptr<AVSHA, decltype(&av_free)> sha(av_sha_alloc(), &av_free);
    std::array<uint8_t, 20> digest{};
    av_sha_init(sha.get(), 160);
    av_sha_update(sha.get(), file.data(), file.size());
    av_sha_final(sha.get(), digest.data());
    if (digest != kSha1) {
      error = "not the US release's default.xex (its SHA-1 differs)";
      return false;
    }
  }
  const uint32_t data_offset = Be32(file, 8);
  const uint32_t security = Be32(file, 16);
  const uint32_t count = Be32(file, 20);
  uint32_t file_format = 0;
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t key = Be32(file, 24 + 8 * i);
    const uint32_t value = Be32(file, 28 + 8 * i);
    if (key == kFileFormatKey) {
      file_format = value;
    } else if (key == kLoadAddressKey) {
      image.load_address = value;
    }
  }
  const uint32_t format_size = Be32(file, file_format);
  if (Be16(file, file_format + 4) != kEncryptionNormal ||
      Be16(file, file_format + 6) != kCompressionBasic || image.load_address == 0) {
    error = "unexpected file format";
    return false;
  }
  const uint32_t image_size = Be32(file, security + kSecurityImageSize);

  // The image key, then the data.
  std::unique_ptr<AVAES, AesFree> aes(av_aes_alloc());
  std::array<uint8_t, 16> key{};
  av_aes_init(aes.get(), kRetailKey.data(), 128, 1);
  av_aes_crypt(aes.get(), key.data(), file.data() + security + kSecurityImageKey, 1, nullptr, 1);
  av_aes_init(aes.get(), key.data(), 128, 1);
  const size_t data_size = (file.size() - data_offset) & ~size_t(15);
  std::vector<uint8_t> data(data_size);
  std::array<uint8_t, 16> iv{};
  av_aes_crypt(aes.get(), data.data(), file.data() + data_offset, int(data_size / 16), iv.data(),
               1);

  // The basic blocks: (data bytes, zero bytes) pairs.
  image.bytes.assign(image_size, 0);
  size_t from = 0, to = 0;
  for (uint32_t at = file_format + 8; at + 8 <= file_format + format_size; at += 8) {
    const uint32_t bytes = Be32(file, at);
    const uint32_t zeros = Be32(file, at + 4);
    if (from + bytes > data.size() || to + bytes + zeros > image.bytes.size()) {
      error = "a block past the data or the image";
      return false;
    }
    std::memcpy(image.bytes.data() + to, data.data() + from, bytes);
    from += bytes;
    to += bytes + zeros;
  }
  ITC_LOG_INFO("image load: {} ({} bytes at {:08X})", path.filename().string(), image_size,
               image.load_address);
  return true;
}

void CheckImageAgainstGuest(const Image& image, const uint8_t* base) {
  const uint8_t* guest = base + image.load_address;
  size_t ranges = 0, bytes = 0;
  for (size_t i = 0; i < image.bytes.size();) {
    if (image.bytes[i] == guest[i]) {
      i++;
      continue;
    }
    size_t end = i;
    while (end < image.bytes.size() && image.bytes[end] != guest[end]) {
      end++;
    }
    if (ranges < 64) {
      ITC_LOG_INFO("image load: differs at {:08X}-{:08X}", image.load_address + i,
                   image.load_address + end - 1);
    }
    ranges++;
    bytes += end - i;
    i = end;
  }
  ITC_LOG_INFO("image load: {} ranges ({} bytes) differ from the SDK's image", ranges, bytes);
}

}  // namespace app
