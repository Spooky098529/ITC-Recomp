// XGraphics' image blitter natively (docs/functions/texture-compression.md):
// Blitter::Blitter sub_82368D20, Blitter::Release sub_82368D30,
// Blitter::CopyRows sub_82369860, Blitter::ConvertRows sub_823699F8 and
// Blitter::Init sub_8236C630, as the translated code does them, on the blit
// object and the descriptions in guest memory.

#include "texture_compression/blitter.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "helpers/debug_output.h"
#include "memory/c_runtime_heap.h"
#include "schema/texture_compression.h"

namespace texture_compression {

namespace {

constexpr uint32_t kMaxAlloc = 0xFFFFF000;  // malloc's largest size

// A codec deleted through its slot 0 (r4 = 1): the destructors' plain C++
// for the plain and DXT codecs, else (YUV's) through `call`. Its r3.
uint32_t DeleteCodec(uint8_t* base, uint32_t codec, uint32_t frame, const GuestCall& call) {
  const uint32_t function = schema::ImageCodec(base, codec).vtable().destroy();
  std::optional<uint32_t> r3;
  if (function == kImageCodecDelete) {
    r3 = ImageCodecDelete(base, codec, 1);
  } else if (function == kDxtCodecDelete) {
    r3 = DxtCodecDelete(base, codec, 1);
  }
  return r3 ? *r3 : call(function, frame, codec, 1, 0, 0);
}

// memcpy (sub_825A7410): host side where c_runtime::Memcpy takes it (outside
// the physical views, whose copies memcpy's override records for the
// renderer), else the function through `call`.
void Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t frame,
          const GuestCall& call) {
  if (to < 0xA0000000u && c_runtime::InOneView(to, bytes) && c_runtime::InOneView(from, bytes) &&
      !c_runtime::MayShare(to, bytes, from, bytes) && c_runtime::Memcpy(base, to, from, bytes)) {
    return;
  }
  call(kMemcpy, frame, to, from, bytes, 0);
}

}  // namespace

void BlitterConstruct(uint8_t* base, uint32_t blitter) {
  ITC_NATIVE_RUNNING("sub_82368D20 Blitter::Blitter");
  const schema::Blitter b(base, blitter);
  b.set_destination(0);
  b.set_source(0);
}

uint32_t BlitterRelease(uint8_t* base, uint32_t blitter, uint32_t stack, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82368D30 Blitter::Release");
  const uint32_t frame = stack - kReleaseFrameBytes;
  const schema::Blitter b(base, blitter);
  uint32_t r3 = b.destination().address;
  if (r3 != 0) {
    DeleteCodec(base, r3, frame, call);
  }
  r3 = b.source().address;
  if (r3 != 0) {
    r3 = DeleteCodec(base, r3, frame, call);
  }
  return r3;
}

uint32_t BlitterCopyRows(uint8_t* base, uint32_t blitter, uint32_t stack, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82369860 Blitter::CopyRows");
  const uint32_t frame = stack - kCopyRowsFrameBytes;
  const schema::Blitter blit(base, blitter);
  const schema::ImageCodec src = blit.source();       // r10
  const schema::ImageCodec dst = blit.destination();  // r9
  if (((src.format() ^ dst.format()) & ~0x100u) != 0 || src.color_key() != 0 ||
      dst.width() != src.width() || dst.height() != src.height() || dst.depth() != src.depth() ||
      dst.srgb() != src.srgb()) {
    return kFail;
  }
  if (dst.has_bits() == 0) {
    return call(kCopyBlocks, frame, blitter, 0, 0, 0);
  }
  if (dst.palette_set() != 0) {
    uint32_t a = dst.palette();
    uint32_t b = src.palette();
    if (a != b) {
      const uint32_t end = a + 1024;
      int32_t difference;
      do {
        difference = int32_t(guest::LoadU8(base, a)) - int32_t(guest::LoadU8(base, b));
        if (difference != 0) break;
        a++;
        b++;
      } while (a != end);
      if (difference != 0) {
        return kFail;
      }
    }
  }
  dst.set_srgb(0);
  blit.source().set_srgb(0);
  auto to_codec = [&]() { return blit.destination(); };
  auto from_codec = [&]() { return blit.source(); };
  for (uint32_t z = 0; z < to_codec().depth(); z++) {
    uint32_t from = from_codec().slice_pitch() * z + from_codec().data().address;
    uint32_t to = to_codec().slice_pitch() * z + to_codec().data().address;
    for (uint32_t y = 0; y < to_codec().height(); y++) {
      Copy(base, to, from, to_codec().row_bytes(), frame, call);
      to += to_codec().row_pitch();
      from += from_codec().row_pitch();
    }
  }
  return 0;
}

std::optional<uint32_t> BlitterConvertRows(uint8_t* base, uint32_t blitter, uint32_t stack,
                                           const GuestCall& call) {
  const schema::Blitter blit(base, blitter);
  const schema::ImageCodec src = blit.source();
  const schema::ImageCodec dst = blit.destination();
  const uint32_t width = dst.width();
  const bool same =
      width == src.width() && dst.height() == src.height() && dst.depth() == src.depth();
  const uint32_t bytes = width > 0x0FFFFFFF ? 0xFFFFFFFF : width << 4;
  if (same && bytes > kMaxAlloc) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_823699F8 Blitter::ConvertRows");
  if (!same) {
    return kFail;
  }
  const uint32_t frame = stack - kConvertRowsFrameBytes;
  const auto made = memory::crt::OperatorNew(base, bytes);
  const uint32_t row = made ? *made : 0;  // r28
  if (row == 0) {
    ITC_LOG_ERROR("Blitter::ConvertRows: operator new refused the row");
    return kFail;
  }
  auto to_codec = [&]() { return blit.destination(); };
  auto from_codec = [&]() { return blit.source(); };
  if (to_codec().srgb() != 0 && from_codec().srgb() != 0) {
    to_codec().set_srgb(0);
    from_codec().set_srgb(0);
  }
  for (uint32_t z = 0; z < to_codec().depth(); z++) {
    for (uint32_t y = 0; y < to_codec().height(); y++) {
      const uint32_t decoder = from_codec().address;
      const uint32_t decode = schema::ImageCodec(base, decoder).vtable().decode_row();
      std::optional<uint32_t> done;
      if (decode == kDecodeA8R8G8B8Row) {
        done = DecodeA8R8G8B8Row(base, decoder, y, z, row);
      }
      if (!done) {
        call(decode, frame, decoder, y, z, row);
      }
      const uint32_t encoder = to_codec().address;
      const uint32_t encode = schema::ImageCodec(base, encoder).vtable().encode_row();
      done.reset();
      if (encode == kDxtEncodeRow) {
        done = DxtEncodeRow(base, encoder, y, z, row, frame, call);
      }
      if (!done) {
        call(encode, frame, encoder, y, z, row);
      }
    }
  }
  if (!memory::crt::OperatorDelete(row)) {
    ITC_LOG_ERROR("Blitter::ConvertRows: operator delete refused {:08X}", row);
  }
  return 0;
}

uint32_t BlitterInit(uint8_t* base, uint32_t blitter, uint32_t destination, uint32_t source,
                     uint32_t filter, uint32_t stack, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8236C630 Blitter::Init");
  const uint32_t frame = stack - kInitFrameBytes;
  const schema::Blitter b(base, blitter);
  b.set_filter(filter);
  b.set_source(0);
  b.set_destination(0);
  if ((filter & 0xFFFF) - 1 > 4 || (filter & 0xFF800000) != 0) {
    helpers::DebugOutput(nullptr);  // "Invalid image filter"
    helpers::DebugOutput(nullptr);
    return kInvalidCall;
  }
  const schema::ImageDescription to_description(base, destination);
  schema::ImageDescription(base, source).set_srgb(filter & 0x200000);
  to_description.set_srgb(filter & 0x400000);
  to_description.set_dither(filter & 0x80000);
  to_description.set_diffusion(filter & 0x100000);
  auto create = [&](uint32_t d) {
    const auto codec = CreateCodec(base, d, frame, call);
    return codec ? *codec : call(kCreate, frame, d, 0, 0, 0);
  };
  uint32_t result = 0;  // r30
  bool made = false;
  const uint32_t to = create(destination);
  b.set_destination(to);
  if (to != 0) {
    const uint32_t from = create(source);
    b.set_source(from);
    made = from != 0;
    if (made) {
      const uint32_t prepare_to = b.destination().address;
      const auto prepared = PrepareCodecs(base, prepare_to, from);
      result = prepared ? *prepared : call(kPrepare, frame, prepare_to, from, 0, 0);
      if (int32_t(result) >= 0) {
        static constexpr uint32_t kFilters[8] = {0x82369B48, 0x82369DB8, 0x82369FF8,
                                                 0x8236A4F0, 0x8236AB00, 0x8236AEA8,
                                                 0x8236BF40, 0x8236B7A0};
        bool taken = int32_t(BlitterCopyRows(base, blitter, frame, call)) >= 0;
        if (!taken) {
          const auto converted = BlitterConvertRows(base, blitter, frame, call);
          taken = int32_t(converted ? *converted : call(kConvertRows, frame, blitter, 0, 0, 0)) >= 0;
        }
        for (uint32_t i = 0; !taken && i < 8; i++) {
          taken = int32_t(call(kFilters[i], frame, blitter, 0, 0, 0)) >= 0;
        }
        if (taken) {
          result = 0;
        } else {
          helpers::DebugOutput(nullptr);  // "No suitable image filter found"
          helpers::DebugOutput(nullptr);
          result = kFail;
        }
      }
    }
  }
  if (!made) {
    helpers::DebugOutput(nullptr);  // "Unsupported image format"
    helpers::DebugOutput(nullptr);
    result = kFail;
  }
  if (const uint32_t codec = b.destination().address; codec != 0) {
    DeleteCodec(base, codec, frame, call);
    b.set_destination(0);
  }
  if (const uint32_t codec = b.source().address; codec != 0) {
    DeleteCodec(base, codec, frame, call);
    b.set_source(0);
  }
  return result;
}

}  // namespace texture_compression
