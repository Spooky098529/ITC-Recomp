#include "image_dump.h"

#include "guest.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "base/log.h"
#include "base/flags.h"

ITC_FLAG_STRING(dump_image, "", "ITC",
                "Write the loaded executable image (0x82000000-0x82980000, "
                "big-endian as in guest memory) to this file at the first frame, "
                "then exit");

namespace {

// default.xex's image: .rdata from 0x82000600, .text from 0x82090000, .data
// up to 0x8297833C.
constexpr uint32_t kImageStart = 0x82000000;
constexpr uint32_t kImageEnd = 0x82980000;

}  // namespace

void debug::ImageDump_OnPass(uint8_t* base) {
  std::string path = ITC_FLAG(dump_image);
  if (path.empty()) {
    return;
  }
  std::vector<uint8_t> image(kImageEnd - kImageStart);
  for (uint32_t i = 0; i < image.size(); i++) {
    image[i] = guest::LoadU8(base, kImageStart + i);
  }
  FILE* f = std::fopen(path.c_str(), "wb");
  if (!f || std::fwrite(image.data(), 1, image.size(), f) != image.size()) {
    ITC_LOG_ERROR("dump_image: can't write {}", path);
  } else {
    ITC_LOG_INFO("dump_image: wrote {} bytes from {:08X} to {}", image.size(), kImageStart, path);
  }
  if (f) {
    std::fclose(f);
  }
  std::_Exit(0);
}
