// The Direct3D 11 backend (backend.h; docs/d3d11-backend.md, step 5).

#include "gpu/d3d11/backend.h"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>
#include <fstream>
#include <map>
#include <thread>

#include <d3dcompiler.h>

#include "base/hash.h"
#include "base/log.h"
#include "native/generated_blit_frag_hlsl.h"
#include "native/generated_gamma_comp_hlsl.h"
#include "native/generated_id_frag_hlsl.h"
#include "native/generated_movie_frag_hlsl.h"
#include "native/generated_resolve_frag_hlsl.h"
#include "native/generated_resolve_ms_frag_hlsl.h"
#include "native/generated_resolve_vert_hlsl.h"
#include "native/generated_swizzle_frag_hlsl.h"

namespace gpu::d3d11 {

namespace {

// The texels' format (a depth image's typeless, for its depth and sampled
// views); also the vertex attributes'.
DXGI_FORMAT TexelFormat(Format format) {
  switch (format) {
    case Format::kR8Unorm: return DXGI_FORMAT_R8_UNORM;
    case Format::kR8G8Unorm: return DXGI_FORMAT_R8G8_UNORM;
    case Format::kR8G8B8A8Unorm: return DXGI_FORMAT_R8G8B8A8_UNORM;
    case Format::kA2B10G10R10Unorm: return DXGI_FORMAT_R10G10B10A2_UNORM;  // R low, as Vulkan's
    case Format::kR16Unorm: return DXGI_FORMAT_R16_UNORM;
    case Format::kR16G16Unorm: return DXGI_FORMAT_R16G16_UNORM;
    case Format::kR16G16B16A16Unorm: return DXGI_FORMAT_R16G16B16A16_UNORM;
    case Format::kR16Float: return DXGI_FORMAT_R16_FLOAT;
    case Format::kR16G16Float: return DXGI_FORMAT_R16G16_FLOAT;
    case Format::kR16G16B16A16Float: return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case Format::kR32Float: return DXGI_FORMAT_R32_FLOAT;
    case Format::kR32G32Float: return DXGI_FORMAT_R32G32_FLOAT;
    case Format::kR32G32B32A32Float: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case Format::kR32Uint: return DXGI_FORMAT_R32_UINT;
    case Format::kR32G32Uint: return DXGI_FORMAT_R32G32_UINT;
    case Format::kR32G32B32Uint: return DXGI_FORMAT_R32G32B32_UINT;
    case Format::kR32G32B32A32Uint: return DXGI_FORMAT_R32G32B32A32_UINT;
    case Format::kD32FloatS8Uint: return DXGI_FORMAT_R32G8X24_TYPELESS;
    case Format::kBC1: return DXGI_FORMAT_BC1_UNORM;
    case Format::kBC2: return DXGI_FORMAT_BC2_UNORM;
    case Format::kBC3: return DXGI_FORMAT_BC3_UNORM;
    case Format::kBC5: return DXGI_FORMAT_BC5_UNORM;
    default: return DXGI_FORMAT_UNKNOWN;
  }
}

constexpr DXGI_FORMAT kDepthView = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
constexpr DXGI_FORMAT kDepthSampled = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;

bool IsDepth(Format format) { return format == Format::kD32FloatS8Uint; }
bool IsCompressed(Format format) {
  return format == Format::kBC1 || format == Format::kBC2 || format == Format::kBC3 ||
         format == Format::kBC5;
}
bool IsInteger(Format format) {
  return format == Format::kR32Uint || format == Format::kR32G32Uint ||
         format == Format::kR32G32B32Uint || format == Format::kR32G32B32A32Uint;
}
// Can be drawn into (cleared, blitted to) as well as copied to.
bool IsRenderable(Format format) {
  return !IsDepth(format) && !IsCompressed(format) && format != Format::kR32G32B32Uint &&
         format != Format::kUndefined;
}

// Bytes a texel (a 4x4 block for the compressed formats), as Vulkan's.
uint32_t TexelBytes(Format format) {
  switch (format) {
    case Format::kR8Unorm: return 1;
    case Format::kR8G8Unorm: case Format::kR16Unorm: case Format::kR16Float: return 2;
    case Format::kR16G16B16A16Unorm: case Format::kR16G16B16A16Float: case Format::kR32G32Float:
    case Format::kR32G32Uint: case Format::kD32FloatS8Uint: case Format::kBC1: return 8;
    case Format::kR32G32B32Uint: return 12;
    case Format::kR32G32B32A32Float: case Format::kR32G32B32A32Uint: case Format::kBC2:
    case Format::kBC3: case Format::kBC5: return 16;
    case Format::kUndefined: return 0;
    default: return 4;
  }
}

// A row's and a layer's bytes in an upload: rows of `row_texels`, `rows` a
// layer (in 4x4 blocks for the compressed formats).
void Pitches(Format format, uint32_t row_texels, uint32_t rows, UINT& row_pitch,
             UINT& layer_pitch) {
  if (IsCompressed(format)) {
    row_pitch = (row_texels + 3) / 4 * TexelBytes(format);
    layer_pitch = (rows + 3) / 4 * row_pitch;
  } else {
    row_pitch = row_texels * TexelBytes(format);
    layer_pitch = rows * row_pitch;
  }
}

// BC blocks decoded (for a compressed image D3D11 can't hold as such): a
// block's 16 texels as RGBA8, or RG8 for BC5.
void DecodeColorBlock(const uint8_t* block, uint8_t out[16][4], bool opaque_only) {
  const uint16_t c0 = uint16_t(block[0] | block[1] << 8), c1 = uint16_t(block[2] | block[3] << 8);
  uint8_t colors[4][4];
  auto expand = [](uint16_t c, uint8_t* rgb) {
    rgb[0] = uint8_t(((c >> 11) & 31) * 255 / 31);
    rgb[1] = uint8_t(((c >> 5) & 63) * 255 / 63);
    rgb[2] = uint8_t((c & 31) * 255 / 31);
  };
  expand(c0, colors[0]);
  expand(c1, colors[1]);
  colors[0][3] = colors[1][3] = 255;
  if (c0 > c1 || opaque_only) {
    for (int i = 0; i < 3; i++) {
      colors[2][i] = uint8_t((2 * colors[0][i] + colors[1][i] + 1) / 3);
      colors[3][i] = uint8_t((colors[0][i] + 2 * colors[1][i] + 1) / 3);
    }
    colors[2][3] = colors[3][3] = 255;
  } else {
    for (int i = 0; i < 3; i++) {
      colors[2][i] = uint8_t((colors[0][i] + colors[1][i] + 1) / 2);
      colors[3][i] = 0;
    }
    colors[2][3] = 255;
    colors[3][3] = 0;
  }
  const uint32_t indices = uint32_t(block[4] | block[5] << 8 | block[6] << 16 | block[7] << 24);
  for (int t = 0; t < 16; t++) {
    std::memcpy(out[t], colors[(indices >> (2 * t)) & 3], 4);
  }
}

// A BC3 or BC5 channel block (8 bytes): 16 values.
void DecodeChannelBlock(const uint8_t* block, uint8_t out[16]) {
  const uint32_t a0 = block[0], a1 = block[1];
  uint8_t values[8] = {uint8_t(a0), uint8_t(a1)};
  if (a0 > a1) {
    for (uint32_t i = 1; i < 7; i++) {
      values[i + 1] = uint8_t(((7 - i) * a0 + i * a1 + 3) / 7);
    }
  } else {
    for (uint32_t i = 1; i < 5; i++) {
      values[i + 1] = uint8_t(((5 - i) * a0 + i * a1 + 2) / 5);
    }
    values[6] = 0;
    values[7] = 255;
  }
  uint64_t bits = 0;
  for (int i = 0; i < 6; i++) {
    bits |= uint64_t(block[2 + i]) << (8 * i);
  }
  for (int t = 0; t < 16; t++) {
    out[t] = values[(bits >> (3 * t)) & 7];
  }
}

// Rows of blocks (row_pitch bytes a row of blocks) into texels, `width` x
// `height` of them, `texel_bytes` (4, or 2 for BC5) each, rows of `out_pitch`.
void DecodeBlocks(Format format, const uint8_t* in, uint32_t row_pitch, uint32_t width,
                  uint32_t height, uint8_t* out, uint32_t out_pitch) {
  const uint32_t block_bytes = format == Format::kBC1 ? 8 : 16;
  for (uint32_t by = 0; by * 4 < height; by++) {
    for (uint32_t bx = 0; bx * 4 < width; bx++) {
      const uint8_t* block = in + size_t(by) * row_pitch + size_t(bx) * block_bytes;
      uint8_t texels[16][4];
      if (format == Format::kBC5) {
        uint8_t red[16], green[16];
        DecodeChannelBlock(block, red);
        DecodeChannelBlock(block + 8, green);
        for (int t = 0; t < 16; t++) {
          texels[t][0] = red[t];
          texels[t][1] = green[t];
        }
      } else if (format == Format::kBC1) {
        DecodeColorBlock(block, texels, false);
      } else {
        DecodeColorBlock(block + 8, texels, true);
        if (format == Format::kBC2) {
          for (int t = 0; t < 16; t++) {
            texels[t][3] = uint8_t(((block[t / 2] >> (4 * (t & 1))) & 15) * 17);
          }
        } else {
          uint8_t alpha[16];
          DecodeChannelBlock(block, alpha);
          for (int t = 0; t < 16; t++) {
            texels[t][3] = alpha[t];
          }
        }
      }
      const uint32_t bytes = format == Format::kBC5 ? 2 : 4;
      for (uint32_t t = 0; t < 16; t++) {
        const uint32_t x = bx * 4 + t % 4, y = by * 4 + t / 4;
        if (x < width && y < height) {
          std::memcpy(out + size_t(y) * out_pitch + size_t(x) * bytes, texels[t], bytes);
        }
      }
    }
  }
}

// Where a frame of `source`'s shape goes in `target`: as large as fits,
// centered (as the Vulkan backend's FitOffsets).
Rect Fit(Extent2D source, Extent2D target) {
  uint64_t width = target.width, height = target.height;
  if (uint64_t(target.width) * source.height > uint64_t(target.height) * source.width) {
    width = (uint64_t(target.height) * source.width + source.height / 2) / source.height;
  } else {
    height = (uint64_t(target.width) * source.height + source.width / 2) / source.width;
  }
  return {int32_t((target.width - width) / 2), int32_t((target.height - height) / 2),
          uint32_t(width), uint32_t(height)};
}

D3D11_BLEND Blend(BlendFactor factor, bool alpha) {
  using F = BlendFactor;
  switch (factor) {
    case F::kZero: return D3D11_BLEND_ZERO;
    case F::kOne: return D3D11_BLEND_ONE;
    // An alpha factor can't be a color one in D3D: the same value's alpha.
    case F::kSrcColor: return alpha ? D3D11_BLEND_SRC_ALPHA : D3D11_BLEND_SRC_COLOR;
    case F::kOneMinusSrcColor: return alpha ? D3D11_BLEND_INV_SRC_ALPHA : D3D11_BLEND_INV_SRC_COLOR;
    case F::kSrcAlpha: return D3D11_BLEND_SRC_ALPHA;
    case F::kOneMinusSrcAlpha: return D3D11_BLEND_INV_SRC_ALPHA;
    case F::kDstColor: return alpha ? D3D11_BLEND_DEST_ALPHA : D3D11_BLEND_DEST_COLOR;
    case F::kOneMinusDstColor:
      return alpha ? D3D11_BLEND_INV_DEST_ALPHA : D3D11_BLEND_INV_DEST_COLOR;
    case F::kDstAlpha: return D3D11_BLEND_DEST_ALPHA;
    case F::kOneMinusDstAlpha: return D3D11_BLEND_INV_DEST_ALPHA;
    // The constant: D3D has one factor for all four (the constant's alpha
    // for color is given by setting the factor's color to its alpha).
    case F::kConstantColor: case F::kConstantAlpha: return D3D11_BLEND_BLEND_FACTOR;
    case F::kOneMinusConstantColor: case F::kOneMinusConstantAlpha:
      return D3D11_BLEND_INV_BLEND_FACTOR;
    case F::kSrcAlphaSaturate: return D3D11_BLEND_SRC_ALPHA_SAT;
  }
  return D3D11_BLEND_ONE;
}

D3D11_BLEND_OP BlendOperation(BlendOp op) {
  switch (op) {
    case BlendOp::kAdd: return D3D11_BLEND_OP_ADD;
    case BlendOp::kSubtract: return D3D11_BLEND_OP_SUBTRACT;
    case BlendOp::kReverseSubtract: return D3D11_BLEND_OP_REV_SUBTRACT;
    case BlendOp::kMin: return D3D11_BLEND_OP_MIN;
    case BlendOp::kMax: return D3D11_BLEND_OP_MAX;
  }
  return D3D11_BLEND_OP_ADD;
}

D3D11_TEXTURE_ADDRESS_MODE Address(AddressMode mode) {
  switch (mode) {
    case AddressMode::kRepeat: return D3D11_TEXTURE_ADDRESS_WRAP;
    case AddressMode::kMirroredRepeat: return D3D11_TEXTURE_ADDRESS_MIRROR;
    case AddressMode::kClampToEdge: return D3D11_TEXTURE_ADDRESS_CLAMP;
    case AddressMode::kClampToBorder: return D3D11_TEXTURE_ADDRESS_BORDER;
    case AddressMode::kMirrorClampToEdge: return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
  }
  return D3D11_TEXTURE_ADDRESS_WRAP;
}

// A clear value's bits as D3D's floats: an integer format's as numbers.
void ClearFloats(Format format, const uint32_t bits[4], float out[4]) {
  for (int i = 0; i < 4; i++) {
    out[i] = IsInteger(format) ? float(bits[i]) : std::bit_cast<float>(bits[i]);
  }
}

}  // namespace

size_t Backend::DescHash::operator()(const PipelineDesc& desc) const {
  return size_t(base::Hash64(&desc, sizeof(desc)));
}

Backend::Backend(const Options& options) : options_(options) {
  frames_ = std::vector<Frame>(std::max<uint32_t>(options.frames_in_flight, 1));
}

Backend::~Backend() {
  if (context_) {
    context_->ClearState();
    context_->Flush();
  }
}

// The device and swapchain are made here, on the window's thread (DXGI
// talks to the window's thread while making a swapchain), then used on the
// render thread alone.
std::string Backend::AttachWindow(SDL_Window* window) {
  window_ = window;
  HWND hwnd = static_cast<HWND>(SDL_GetPointerProperty(
      SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr));
  if (!hwnd) {
    return "D3D11: the window has no HWND";
  }
  if (!CreateDeviceAndSwapchain(hwnd)) {
    return init_error_.empty() ? "D3D11: couldn't create the device" : init_error_;
  }
  return {};
}

bool Backend::CreateDeviceAndSwapchain(HWND hwnd) {
  HMODULE compiler = LoadLibraryA("d3dcompiler_47.dll");
  compile_ = compiler ? reinterpret_cast<CompileFn>(GetProcAddress(compiler, "D3DCompile"))
                      : nullptr;
  if (!compile_) {
    init_error_ = "D3D11: d3dcompiler_47.dll (D3DCompile) not found";
    return false;
  }
  UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
  if (options_.debug) {
    flags |= D3D11_CREATE_DEVICE_DEBUG;
  }
  const D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};
  D3D_FEATURE_LEVEL level{};
  HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels,
                                 UINT(std::size(levels)), D3D11_SDK_VERSION, &device0_, &level,
                                 &context0_);
  if (FAILED(hr) && options_.debug) {
    ITC_LOG_WARN("D3D11: no debug layer; without it");
    flags &= ~D3D11_CREATE_DEVICE_DEBUG;
    hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels,
                           UINT(std::size(levels)), D3D11_SDK_VERSION, &device0_, &level,
                           &context0_);
  }
  if (FAILED(hr)) {
    init_error_ = std::format("D3D11: D3D11CreateDevice: {:08X}", uint32_t(hr));
    return false;
  }
  if (FAILED(device0_.As(&device_)) || FAILED(context0_.As(&context_))) {
    init_error_ = "D3D11: Direct3D 11.1 isn't available";
    return false;
  }
  D3D11_FEATURE_DATA_D3D11_OPTIONS features{};
  device_->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS, &features, sizeof(features));
  if (!features.ConstantBufferOffsetting || !features.MapNoOverwriteOnDynamicConstantBuffer) {
    init_error_ = "D3D11: constant buffer offsets (Direct3D 11.1) aren't supported";
    return false;
  }
  ComPtr<IDXGIDevice> dxgi_device;
  ComPtr<IDXGIAdapter> adapter;
  ComPtr<IDXGIFactory2> factory;
  if (FAILED(device_.As(&dxgi_device)) || FAILED(dxgi_device->GetAdapter(&adapter)) ||
      FAILED(adapter->GetParent(IID_PPV_ARGS(&factory)))) {
    init_error_ = "D3D11: no DXGI factory";
    return false;
  }
  DXGI_ADAPTER_DESC adapter_desc{};
  adapter->GetDesc(&adapter_desc);
  char name[128] = {};
  WideCharToMultiByte(CP_UTF8, 0, adapter_desc.Description, -1, name, sizeof(name) - 1, nullptr,
                      nullptr);
  ITC_LOG_INFO("D3D11: {} (feature level {:X})", name, uint32_t(level));
  ComPtr<IDXGIFactory5> factory5;
  BOOL tearing = FALSE;
  if (SUCCEEDED(factory.As(&factory5)) &&
      SUCCEEDED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &tearing,
                                              sizeof(tearing)))) {
    tearing_ = tearing;
  }
  int width = 0, height = 0;
  SDL_GetWindowSizeInPixels(window_, &width, &height);
  DXGI_SWAP_CHAIN_DESC1 desc{};
  desc.Width = UINT(std::max(width, 1));
  desc.Height = UINT(std::max(height, 1));
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc.BufferCount = 2;
  desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  desc.Flags = tearing_ ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
  hr = factory->CreateSwapChainForHwnd(device_.Get(), hwnd, &desc, nullptr, nullptr, &swapchain_);
  if (FAILED(hr)) {
    init_error_ = std::format("D3D11: CreateSwapChainForHwnd: {:08X}", uint32_t(hr));
    return false;
  }
  factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);  // SDL handles fullscreen
  output_size_ = {desc.Width, desc.Height};
  return UpdateBackBuffer();
}

bool Backend::UpdateBackBuffer() {
  back_buffer_.Reset();
  ComPtr<ID3D11Texture2D> buffer;
  if (FAILED(swapchain_->GetBuffer(0, IID_PPV_ARGS(&buffer))) ||
      FAILED(device_->CreateRenderTargetView(buffer.Get(), nullptr, &back_buffer_))) {
    ITC_LOG_ERROR("D3D11: the swapchain's image has no target view");
    return false;
  }
  return true;
}

bool Backend::Init() {
  if (!device_) {
    return false;
  }
  for (Frame& frame : frames_) {
    D3D11_QUERY_DESC query{D3D11_QUERY_EVENT, 0};
    if (FAILED(device_->CreateQuery(&query, &frame.done))) {
      return false;
    }
  }
  return CreateOwnResources();
}

void Backend::OpenDiskCache(const std::filesystem::path& directory) {
  cache_directory_ = directory / "d3d11_shaders";
  std::error_code error;
  std::filesystem::create_directories(cache_directory_, error);
}

// Shaders: each HLSL with its defines compiled once, kept in memory and on
// disk by the hash of all of it.
std::vector<uint8_t> Backend::Compile(
    const char* hlsl, const char* profile,
    const std::vector<std::pair<std::string, std::string>>& defines) {
  uint64_t key = base::Hash64(hlsl, std::strlen(hlsl));
  key = base::Hash64(profile, std::strlen(profile), key);
  for (const auto& [name, value] : defines) {
    key = base::Hash64(name.data(), name.size(), key);
    key = base::Hash64(value.data(), value.size(), key ^ 0x9E3779B97F4A7C15ull);
  }
  if (auto found = compiled_.find(key); found != compiled_.end()) {
    return found->second;
  }
  std::filesystem::path file;
  if (!cache_directory_.empty()) {
    file = cache_directory_ / std::format("{:016X}.dxbc", key);
    std::ifstream in(file, std::ios::binary);
    if (in) {
      std::vector<uint8_t> code((std::istreambuf_iterator<char>(in)),
                                std::istreambuf_iterator<char>());
      if (!code.empty()) {
        return compiled_[key] = std::move(code);
      }
    }
  }
  std::vector<D3D_SHADER_MACRO> macros;
  for (const auto& [name, value] : defines) {
    macros.push_back({name.c_str(), value.c_str()});
  }
  macros.push_back({nullptr, nullptr});
  ComPtr<ID3DBlob> code, errors;
  HRESULT hr = compile_(hlsl, std::strlen(hlsl), nullptr, macros.data(), nullptr, "main", profile,
                        0, 0, &code, &errors);
  if (FAILED(hr)) {
    ITC_LOG_ERROR("D3D11: a {} shader didn't compile: {}", profile,
                  errors ? static_cast<const char*>(errors->GetBufferPointer()) : "");
    return {};
  }
  const uint8_t* bytes = static_cast<const uint8_t*>(code->GetBufferPointer());
  std::vector<uint8_t> result(bytes, bytes + code->GetBufferSize());
  if (!file.empty()) {
    std::ofstream out(file, std::ios::binary);
    out.write(reinterpret_cast<const char*>(result.data()), std::streamsize(result.size()));
  }
  return compiled_[key] = std::move(result);
}

bool Backend::MakeOwnPass(OwnPass& pass, const char* hlsl, uint32_t constants_size) {
  std::vector<uint8_t> code = Compile(hlsl, "ps_5_0", {});
  if (code.empty() ||
      FAILED(device_->CreatePixelShader(code.data(), code.size(), nullptr, &pass.pixel))) {
    return false;
  }
  if (constants_size) {
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = (constants_size + 15) & ~15u;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (FAILED(device_->CreateBuffer(&desc, nullptr, &pass.constants))) {
      return false;
    }
    pass.constants_size = constants_size;
  }
  return true;
}

bool Backend::CreateOwnResources() {
  std::vector<uint8_t> code = Compile(k_hlsl_resolve_vert, "vs_5_0", {});
  if (code.empty() || FAILED(device_->CreateVertexShader(code.data(), code.size(), nullptr,
                                                         &fullscreen_vertex_))) {
    return false;
  }
  if (!MakeOwnPass(resolve_, k_hlsl_resolve_frag, 20) ||
      !MakeOwnPass(resolve_ms_, k_hlsl_resolve_ms_frag, 20) ||
      !MakeOwnPass(movie_, k_hlsl_movie_frag, 16) || !MakeOwnPass(blit_, k_hlsl_blit_frag, 32) ||
      !MakeOwnPass(swizzle_, k_hlsl_swizzle_frag, 24)) {
    ITC_LOG_ERROR("D3D11: the backend's own shaders couldn't be made");
    return false;
  }
  code = Compile(k_hlsl_id_frag, "ps_5_0", {});
  if (code.empty() ||
      FAILED(device_->CreatePixelShader(code.data(), code.size(), nullptr, &id_pixel_))) {
    return false;
  }
  D3D11_BUFFER_DESC buffer{};
  buffer.ByteWidth = 16;
  buffer.Usage = D3D11_USAGE_DYNAMIC;
  buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  if (FAILED(device_->CreateBuffer(&buffer, nullptr, &id_constants_))) {
    return false;
  }
  buffer.Usage = D3D11_USAGE_DEFAULT;
  buffer.CPUAccessFlags = 0;
  const int32_t zero[4] = {};
  D3D11_SUBRESOURCE_DATA initial{zero, 0, 0};
  if (FAILED(device_->CreateBuffer(&buffer, &initial, &vertex_info_buffer_))) {
    return false;
  }
  vertex_info_[0] = vertex_info_[1] = 0;
  // The triangle fans' indices (made per draw).
  buffer.ByteWidth = 4u << 20;
  buffer.Usage = D3D11_USAGE_DYNAMIC;
  buffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
  buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  if (FAILED(device_->CreateBuffer(&buffer, nullptr, &fan_indices_))) {
    return false;
  }
  fan_index_at_ = buffer.ByteWidth;  // the first use discards
  D3D11_SAMPLER_DESC sampler{};
  sampler.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  sampler.AddressU = sampler.AddressV = sampler.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampler.MaxLOD = 0.0f;
  if (FAILED(device_->CreateSamplerState(&sampler, &point_sampler_))) {
    return false;
  }
  sampler.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
  if (FAILED(device_->CreateSamplerState(&sampler, &linear_sampler_))) {
    return false;
  }
  D3D11_RASTERIZER_DESC raster{};
  raster.FillMode = D3D11_FILL_SOLID;
  raster.CullMode = D3D11_CULL_NONE;
  raster.DepthClipEnable = TRUE;
  raster.ScissorEnable = TRUE;
  if (FAILED(device_->CreateRasterizerState(&raster, &own_rasterizer_))) {
    return false;
  }
  D3D11_DEPTH_STENCIL_DESC depth{};
  if (FAILED(device_->CreateDepthStencilState(&depth, &no_depth_))) {
    return false;
  }
  D3D11_BLEND_DESC blend{};
  blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  if (FAILED(device_->CreateBlendState(&blend, &no_blend_))) {
    return false;
  }
  return true;
}

// Frames: each slot's frame waited for (an event query) before its slot's
// resources are used again.
void Backend::WaitFrame(Frame& frame) {
  if (!frame.submitted) {
    return;
  }
  while (context_->GetData(frame.done.Get(), nullptr, 0, 0) == S_FALSE) {
    std::this_thread::yield();
  }
  frame.submitted = false;
}

void Backend::FinishReadbacks(Frame& frame) {
  for (Readback& r : frame.readbacks) {
    D3D11_MAPPED_SUBRESOURCE mapped{};
    if (FAILED(context_->Map(r.staging.Get(), 0, D3D11_MAP_READ, 0, &mapped))) {
      continue;
    }
    const uint32_t out_bytes = r.depth ? 4 : r.texel_bytes;
    uint8_t* out = r.buffer->memory.get() + r.offset;
    for (uint32_t y = 0; y < r.height; y++) {
      const uint8_t* row = static_cast<const uint8_t*>(mapped.pData) + size_t(y) * mapped.RowPitch;
      if (r.depth) {  // the depth of each 8-byte texel
        for (uint32_t x = 0; x < r.width; x++) {
          std::memcpy(out + (size_t(y) * r.width + x) * 4, row + size_t(x) * 8, 4);
        }
      } else {
        std::memcpy(out + size_t(y) * r.width * out_bytes, row, size_t(r.width) * out_bytes);
      }
    }
    context_->Unmap(r.staging.Get(), 0);
  }
  frame.readbacks.clear();
}

uint32_t Backend::BeginFrame() {
  Frame& frame = frames_[frame_index_];
  WaitFrame(frame);
  FinishReadbacks(frame);
  // The sets bound now may be this slot's old ones: unbound with them.
  stage_sets_[0] = stage_sets_[1] = nullptr;
  ApplyStageResources(true);
  ApplyStageResources(false);
  frame.set_cache.clear();
  frame.sets.clear();
  return frame_index_;
}

bool Backend::EndFrame(Image* shown, Extent2D size) {
  Frame& frame = frames_[frame_index_];
  if (window_resized_.exchange(false)) {
    int width = 0, height = 0;
    SDL_GetWindowSizeInPixels(window_, &width, &height);
    if (width > 0 && height > 0 &&
        (uint32_t(width) != output_size_.width || uint32_t(height) != output_size_.height)) {
      context_->OMSetRenderTargets(0, nullptr, nullptr);
      back_buffer_.Reset();
      context_->Flush();
      HRESULT hr = swapchain_->ResizeBuffers(0, UINT(width), UINT(height), DXGI_FORMAT_UNKNOWN,
                                             tearing_ ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0);
      if (FAILED(hr)) {
        ITC_LOG_ERROR("D3D11: ResizeBuffers: {:08X}", uint32_t(hr));
      } else {
        output_size_ = {uint32_t(width), uint32_t(height)};
      }
      UpdateBackBuffer();
    }
  }
  bool shown_ok = false;
  ImageObject& source = Object(shown);
  if (back_buffer_ && source.srv_2d && size.width && size.height) {
    // The frame keeps its shape: black bars where the window's differs.
    const float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    context_->ClearRenderTargetView(back_buffer_.Get(), black);
    const Rect to = Fit(size, output_size_);
    const float constants[8] = {float(to.x), float(to.y), float(to.width), float(to.height),
                                0.0f, 0.0f, float(size.width) / float(source.desc.width),
                                float(size.height) / float(source.desc.height)};
    BeginOwnPass(back_buffer_.Get(), to, blit_, constants);
    ID3D11ShaderResourceView* view = source.srv_2d.Get();
    ID3D11SamplerState* sampler = linear_sampler_.Get();
    context_->PSSetShaderResources(0, 1, &view);
    context_->PSSetSamplers(0, 1, &sampler);
    context_->Draw(3, 0);
    EndOwnPass();
    context_->OMSetRenderTargets(0, nullptr, nullptr);
    shown_ok = true;
  }
  HRESULT hr = swapchain_->Present(0, tearing_ ? DXGI_PRESENT_ALLOW_TEARING : 0);
  if (FAILED(hr)) {
    ITC_LOG_ERROR("D3D11: Present: {:08X}", uint32_t(hr));
    shown_ok = false;
  }
  context_->End(frame.done.Get());
  frame.submitted = true;
  frame_index_ = (frame_index_ + 1) % uint32_t(frames_.size());
  return shown_ok;
}

void Backend::WaitIdle() {
  if (!context_) {
    return;
  }
  ComPtr<ID3D11Query> query;
  D3D11_QUERY_DESC desc{D3D11_QUERY_EVENT, 0};
  if (SUCCEEDED(device_->CreateQuery(&desc, &query))) {
    context_->End(query.Get());
    while (context_->GetData(query.Get(), nullptr, 0, 0) == S_FALSE) {
      std::this_thread::yield();
    }
  }
  for (Frame& frame : frames_) {
    frame.submitted = false;
    FinishReadbacks(frame);
  }
}

bool Backend::CanFilter(Format format) const {
  UINT support = 0;
  DXGI_FORMAT dxgi = IsDepth(format) ? kDepthSampled : TexelFormat(format);
  return SUCCEEDED(device_->CheckFormatSupport(dxgi, &support)) &&
         (support & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE);
}

Sampler* Backend::CreateSampler(const SamplerDesc& desc) {
  D3D11_SAMPLER_DESC info{};
  const bool anisotropic = desc.max_anisotropy >= 1.0f;
  if (anisotropic) {
    info.Filter = D3D11_FILTER_ANISOTROPIC;
    info.MaxAnisotropy = UINT(std::clamp(desc.max_anisotropy, 1.0f, 16.0f));
  } else {
    const bool min = desc.min_filter == Filter::kLinear, mag = desc.mag_filter == Filter::kLinear,
               mip = desc.mip_mode == MipMode::kLinear;
    info.Filter = D3D11_FILTER((min ? 0x10 : 0) | (mag ? 0x4 : 0) | (mip ? 0x1 : 0));
  }
  info.AddressU = Address(desc.address[0]);
  info.AddressV = Address(desc.address[1]);
  info.AddressW = Address(desc.address[2]);
  info.MipLODBias = desc.mip_lod_bias;
  info.ComparisonFunc = D3D11_COMPARISON_NEVER;
  const float border = desc.border == BorderColor::kOpaqueWhite ? 1.0f : 0.0f;
  info.BorderColor[0] = info.BorderColor[1] = info.BorderColor[2] = info.BorderColor[3] = border;
  info.MinLOD = desc.min_lod;
  info.MaxLOD = desc.max_lod >= 1000.0f ? D3D11_FLOAT32_MAX : desc.max_lod;
  ComPtr<ID3D11SamplerState> sampler;
  if (FAILED(device_->CreateSamplerState(&info, &sampler))) {
    return nullptr;
  }
  samplers_.push_back(sampler);
  return reinterpret_cast<Sampler*>(sampler.Get());
}

ID3D11SamplerState* Backend::SamplerFor(Filter filter) {
  return filter == Filter::kLinear ? linear_sampler_.Get() : point_sampler_.Get();
}

// Images.

bool Backend::MakeViews(ImageObject& o) {
  const ImageDesc& desc = o.desc;
  const bool depth = IsDepth(desc.format);
  const bool multisample = desc.samples > 1;
  const UINT bind = [&] {
    D3D11_TEXTURE2D_DESC d{};
    if (desc.type == ImageType::k3D) {
      D3D11_TEXTURE3D_DESC d3{};
      static_cast<ID3D11Texture3D*>(o.resource.Get())->GetDesc(&d3);
      return d3.BindFlags;
    }
    static_cast<ID3D11Texture2D*>(o.resource.Get())->GetDesc(&d);
    return d.BindFlags;
  }();
  if (bind & D3D11_BIND_SHADER_RESOURCE) {
    D3D11_SHADER_RESOURCE_VIEW_DESC srv{};
    srv.Format = depth ? kDepthSampled : o.format;
    if (desc.type == ImageType::k3D) {
      srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
      srv.Texture3D.MipLevels = desc.levels;
    } else if (multisample) {
      srv.ViewDimension = desc.layers > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY
                                          : D3D11_SRV_DIMENSION_TEXTURE2DMS;
      srv.Texture2DMSArray.ArraySize = desc.layers;
    } else if (desc.type == ImageType::kCube) {
      srv.ViewDimension = desc.layers > 6 ? D3D11_SRV_DIMENSION_TEXTURECUBEARRAY
                                          : D3D11_SRV_DIMENSION_TEXTURECUBE;
      srv.TextureCubeArray.MipLevels = desc.levels;
      srv.TextureCubeArray.NumCubes = desc.layers / 6;
    } else if (desc.type == ImageType::k2DArray || desc.layers > 1) {
      srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
      srv.Texture2DArray.MipLevels = desc.levels;
      srv.Texture2DArray.ArraySize = desc.layers;
    } else {
      srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      srv.Texture2D.MipLevels = desc.levels;
    }
    if (HRESULT hr = device_->CreateShaderResourceView(o.resource.Get(), &srv, &o.srv); FAILED(hr)) {
      ITC_LOG_ERROR("D3D11: an image's sampled view couldn't be made: {:08X}", uint32_t(hr));
      return false;
    }
    if (desc.type != ImageType::k3D && desc.layers == 1) {
      if (srv.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D ||
          srv.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2DMS) {
        o.srv_2d = o.srv;
      } else {
        D3D11_SHADER_RESOURCE_VIEW_DESC plain = srv;
        plain.ViewDimension =
            multisample ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
        plain.Texture2D.MostDetailedMip = 0;
        plain.Texture2D.MipLevels = desc.levels;
        device_->CreateShaderResourceView(o.resource.Get(), &plain, &o.srv_2d);
      }
    }
    if (desc.type != ImageType::k3D && !multisample && !depth) {
      D3D11_SHADER_RESOURCE_VIEW_DESC array = srv;
      array.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
      array.Texture2DArray = {0, desc.levels, 0, desc.layers};
      device_->CreateShaderResourceView(o.resource.Get(), &array, &o.srv_array);
    }
  }
  if (bind & D3D11_BIND_RENDER_TARGET) {
    D3D11_RENDER_TARGET_VIEW_DESC rtv{};
    rtv.Format = o.format;
    if (desc.type == ImageType::k3D) {
      rtv.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
      rtv.Texture3D = {0, 0, UINT(-1)};
    } else if (multisample) {
      rtv.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    } else if (desc.layers > 1) {
      rtv.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
      rtv.Texture2DArray = {0, 0, 1};
    } else {
      rtv.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    }
    if (HRESULT hr = device_->CreateRenderTargetView(o.resource.Get(), &rtv, &o.rtv); FAILED(hr)) {
      ITC_LOG_ERROR("D3D11: an image's target view couldn't be made: {:08X}", uint32_t(hr));
      return false;
    }
  }
  if (bind & D3D11_BIND_DEPTH_STENCIL) {
    D3D11_DEPTH_STENCIL_VIEW_DESC dsv{};
    dsv.Format = kDepthView;
    if (multisample) {
      dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    } else if (desc.layers > 1) {
      dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
      dsv.Texture2DArray = {0, 0, 1};
    } else {
      dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    }
    if (HRESULT hr = device_->CreateDepthStencilView(o.resource.Get(), &dsv, &o.dsv); FAILED(hr)) {
      ITC_LOG_ERROR("D3D11: an image's depth view couldn't be made: {:08X}", uint32_t(hr));
      return false;
    }
  }
  if (bind & D3D11_BIND_UNORDERED_ACCESS) {
    D3D11_UNORDERED_ACCESS_VIEW_DESC uav{};
    uav.Format = o.format;
    uav.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    if (HRESULT hr = device_->CreateUnorderedAccessView(o.resource.Get(), &uav, &o.uav); FAILED(hr)) {
      ITC_LOG_ERROR("D3D11: an image's storage view couldn't be made: {:08X}", uint32_t(hr));
      return false;
    }
  }
  return true;
}

Image* Backend::CreateImage(const ImageDesc& desc, const void* initial) {
  auto object = std::make_unique<ImageObject>();
  ImageObject& o = *object;
  o.desc = desc;
  o.format = TexelFormat(desc.format);
  if (o.format == DXGI_FORMAT_UNKNOWN) {
    return nullptr;
  }
  if (IsCompressed(desc.format) && (desc.width % 4 || desc.height % 4)) {
    o.decoded = true;
    o.format = desc.format == Format::kBC5 ? DXGI_FORMAT_R8G8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;
  }
  const bool depth = IsDepth(desc.format);
  UINT bind = 0;
  if (desc.usage & kImageSampled) bind |= D3D11_BIND_SHADER_RESOURCE;
  if ((desc.usage & kImageColorTarget) ||
      ((desc.usage & kImageCopyDest) && IsRenderable(desc.format))) {
    bind |= D3D11_BIND_RENDER_TARGET;
  }
  if ((desc.usage & kImageDepthTarget) || depth) bind |= D3D11_BIND_DEPTH_STENCIL;
  if (desc.usage & kImageStorage) bind |= D3D11_BIND_UNORDERED_ACCESS;
  HRESULT hr;
  if (desc.type == ImageType::k3D) {
    D3D11_TEXTURE3D_DESC info{};
    info.Width = desc.width;
    info.Height = desc.height;
    info.Depth = desc.depth;
    info.MipLevels = desc.levels;
    info.Format = o.format;
    info.Usage = D3D11_USAGE_DEFAULT;
    info.BindFlags = bind & ~UINT(D3D11_BIND_DEPTH_STENCIL);
    ComPtr<ID3D11Texture3D> texture;
    hr = device_->CreateTexture3D(&info, nullptr, &texture);
    o.resource = texture;
  } else {
    D3D11_TEXTURE2D_DESC info{};
    info.Width = desc.width;
    info.Height = desc.height;
    info.MipLevels = desc.levels;
    info.ArraySize = desc.layers;
    info.Format = o.format;
    info.SampleDesc.Count = desc.samples;
    info.Usage = D3D11_USAGE_DEFAULT;
    info.BindFlags = bind;
    info.MiscFlags = desc.type == ImageType::kCube ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
    ComPtr<ID3D11Texture2D> texture;
    hr = device_->CreateTexture2D(&info, nullptr, &texture);
    o.resource = texture;
  }
  if (FAILED(hr) || !MakeViews(o)) {
    ITC_LOG_ERROR("D3D11: an image ({}x{}x{}, format {}, {} levels, {} layers) couldn't be made: "
                  "{:08X}",
                  desc.width, desc.height, desc.depth, int(desc.format), desc.levels, desc.layers,
                  uint32_t(hr));
    return nullptr;
  }
  const uint32_t blocks_wide = IsCompressed(desc.format) ? (desc.width + 3) / 4 : desc.width;
  const uint32_t blocks_high = IsCompressed(desc.format) ? (desc.height + 3) / 4 : desc.height;
  o.memory_size = uint64_t(blocks_wide) * blocks_high * TexelBytes(desc.format) *
                  (desc.type == ImageType::k3D ? desc.depth : desc.layers) * desc.samples * 4 / 3;
  if (initial && o.decoded) {
    UINT row_pitch, layer_pitch;
    Pitches(desc.format, desc.width, desc.height, row_pitch, layer_pitch);
    const uint32_t bytes = desc.format == Format::kBC5 ? 2 : 4;
    std::vector<uint8_t> texels(size_t(desc.width) * desc.height * bytes);
    for (uint32_t layer = 0; layer < desc.layers; layer++) {
      DecodeBlocks(desc.format, static_cast<const uint8_t*>(initial) + size_t(layer) * layer_pitch,
                   row_pitch, desc.width, desc.height, texels.data(), desc.width * bytes);
      context_->UpdateSubresource(o.resource.Get(), D3D11CalcSubresource(0, layer, desc.levels),
                                  nullptr, texels.data(), desc.width * bytes, 0);
    }
  } else if (initial) {
    // Every layer's first level, tightly packed one after another.
    UINT row_pitch, layer_pitch;
    Pitches(desc.format, desc.width, desc.height, row_pitch, layer_pitch);
    const uint8_t* bytes = static_cast<const uint8_t*>(initial);
    if (desc.type == ImageType::k3D) {
      context_->UpdateSubresource(o.resource.Get(), 0, nullptr, bytes, row_pitch, layer_pitch);
    } else {
      for (uint32_t layer = 0; layer < desc.layers; layer++) {
        context_->UpdateSubresource(o.resource.Get(),
                                    D3D11CalcSubresource(0, layer, desc.levels), nullptr,
                                    bytes + size_t(layer) * layer_pitch, row_pitch, layer_pitch);
      }
    }
  }
  return reinterpret_cast<Image*>(object.release());
}

void Backend::DestroyImage(Image* image) {
  if (!image) {
    return;
  }
  ImageObject* o = &Object(image);
  if (o == color_image_ || o == depth_image_) {
    context_->OMSetRenderTargets(0, nullptr, nullptr);
    color_image_ = depth_image_ = nullptr;
    color_target_ = nullptr;
    depth_target_ = nullptr;
  }
  delete o;
}

// Buffers: the GPU's own are D3D11 buffers; host-visible ones are memory of
// ours, copied into a dynamic buffer where the GPU reads them (constants).
Buffer* Backend::CreateBuffer(const BufferDesc& desc) {
  auto object = std::make_unique<BufferObject>();
  BufferObject& o = *object;
  o.desc = desc;
  if (desc.host_visible) {
    o.memory.reset(new (std::nothrow) uint8_t[desc.size]);
    if (!o.memory) {
      return nullptr;
    }
  }
  D3D11_BUFFER_DESC info{};
  info.ByteWidth = UINT(desc.size);
  if (desc.host_visible && (desc.usage & kBufferUniform)) {
    info.ByteWidth = UINT((desc.size + 255) & ~uint64_t(255));
    info.Usage = D3D11_USAGE_DYNAMIC;
    info.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    info.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  } else if (!desc.host_visible) {
    info.Usage = D3D11_USAGE_DEFAULT;
    info.BindFlags = ((desc.usage & kBufferVertex) ? D3D11_BIND_VERTEX_BUFFER : 0) |
                     ((desc.usage & kBufferIndex) ? D3D11_BIND_INDEX_BUFFER : 0) |
                     ((desc.usage & kBufferUniform) ? D3D11_BIND_CONSTANT_BUFFER : 0) |
                     ((desc.usage & kBufferStorage) ? D3D11_BIND_SHADER_RESOURCE : 0);
    if (desc.usage & kBufferStorage) {
      info.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
    }
  }
  if (info.BindFlags) {
    HRESULT hr = device_->CreateBuffer(&info, nullptr, &o.buffer);
    if (FAILED(hr)) {
      ITC_LOG_ERROR("D3D11: a buffer of {} bytes couldn't be made: {:08X}", desc.size,
                    uint32_t(hr));
      return nullptr;
    }
  }
  return reinterpret_cast<Buffer*>(object.release());
}

void Backend::DestroyBuffer(Buffer* buffer) {
  if (buffer) {
    delete &Object(buffer);
  }
}

// Uploads: copied in place (D3D11 orders them with the frame's commands).
void Backend::UploadImage(Buffer* staging, Image* image, const ImageUpload* regions,
                          uint32_t count) {
  ImageObject& o = Object(image);
  const uint8_t* memory = Object(staging).memory.get();
  for (uint32_t i = 0; i < count; i++) {
    const ImageUpload& r = regions[i];
    UINT row_pitch, layer_pitch;
    Pitches(o.desc.format, r.row_texels, r.rows, row_pitch, layer_pitch);
    if (o.decoded) {
      const uint32_t bytes = o.desc.format == Format::kBC5 ? 2 : 4;
      std::vector<uint8_t> texels(size_t(r.width) * r.height * bytes);
      DecodeBlocks(o.desc.format, memory + r.offset, row_pitch, r.width, r.height, texels.data(),
                   r.width * bytes);
      D3D11_BOX box{0, 0, 0, r.width, r.height, 1};
      context_->UpdateSubresource(o.resource.Get(),
                                  D3D11CalcSubresource(r.level, r.layer, o.desc.levels), &box,
                                  texels.data(), r.width * bytes, 0);
      continue;
    }
    D3D11_BOX box{0, 0, 0, r.width, r.height, 1};
    if (IsCompressed(o.desc.format)) {  // in whole blocks
      box.right = (box.right + 3) & ~3u;
      box.bottom = (box.bottom + 3) & ~3u;
    }
    const UINT subresource = o.desc.type == ImageType::k3D
                                 ? r.level
                                 : D3D11CalcSubresource(r.level, r.layer, o.desc.levels);
    if (o.desc.type == ImageType::k3D) {
      box.back = std::max<UINT>(o.desc.depth >> r.level, 1);
    }
    context_->UpdateSubresource(o.resource.Get(), subresource, &box, memory + r.offset, row_pitch,
                                layer_pitch);
  }
  Written(o);
}

void Backend::UploadBuffer(Buffer* staging, Buffer* buffer, const BufferUpload& region) {
  BufferObject& to = Object(buffer);
  if (!to.buffer) {
    std::memcpy(to.memory.get() + region.dest_offset,
                Object(staging).memory.get() + region.source_offset, region.size);
    return;
  }
  D3D11_BOX box{UINT(region.dest_offset), 0, 0, UINT(region.dest_offset + region.size), 1, 1};
  context_->UpdateSubresource(to.buffer.Get(), 0, &box,
                              Object(staging).memory.get() + region.source_offset, 0, 0);
}

Shader* Backend::CreateShader(const ShaderDesc& desc) {
  if (!desc.hlsl) {
    ITC_LOG_ERROR("D3D11: a shader without HLSL");
    return nullptr;
  }
  const bool vertex = desc.stage == ShaderStage::kVertex;
  std::vector<uint8_t> code = Compile(desc.hlsl, vertex ? "vs_5_0" : "ps_5_0", desc.defines);
  if (code.empty()) {
    return nullptr;
  }
  auto object = std::make_unique<ShaderObject>();
  ShaderObject& o = *object;
  o.stage = desc.stage;
  HRESULT hr = vertex ? device_->CreateVertexShader(code.data(), code.size(), nullptr, &o.vertex)
                      : device_->CreatePixelShader(code.data(), code.size(), nullptr, &o.pixel);
  if (FAILED(hr)) {
    ITC_LOG_ERROR("D3D11: a compiled shader couldn't be made: {:08X}", uint32_t(hr));
    return nullptr;
  }
  o.bytecode = std::move(code);
  o.bindings = desc.bindings;
  o.attributes = desc.attributes;
  o.textures = uint32_t(desc.textures.size());
  o.samplers = desc.samplers;
  shaders_.push_back(std::move(object));
  return reinterpret_cast<Shader*>(shaders_.back().get());
}

Pipeline* Backend::GetPipeline(const PipelineDesc& desc, bool* made) {
  auto [it, added] = pipelines_.try_emplace(desc);
  if (!added) {
    return it->second ? reinterpret_cast<Pipeline*>(it->second.get()) : nullptr;
  }
  if (made) {
    *made = true;
  }
  auto object = std::make_unique<PipelineObject>();
  PipelineObject& o = *object;
  o.desc = desc;
  o.vertex = desc.vertex ? &Object(desc.vertex) : nullptr;
  o.pixel = desc.pixel ? &Object(desc.pixel) : nullptr;
  if (!o.vertex) {
    return nullptr;
  }
  // The vertex input: attribute n's semantic is TEXCOORDn (SPIRV-Cross's).
  std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
  for (const VertexAttribute& a : o.vertex->attributes) {
    bool per_instance = false;
    for (const VertexBinding& b : o.vertex->bindings) {
      if (b.binding == a.binding) {
        per_instance = b.per_instance;
      }
    }
    elements.push_back({"TEXCOORD", a.location, TexelFormat(a.format), a.binding, a.offset,
                        per_instance ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA,
                        per_instance ? 1u : 0u});
  }
  for (const VertexBinding& b : o.vertex->bindings) {
    if (b.binding < 32) {
      o.strides[b.binding] = b.stride;
    }
  }
  if (!elements.empty()) {
    HRESULT hr = device_->CreateInputLayout(elements.data(), UINT(elements.size()),
                                            o.vertex->bytecode.data(), o.vertex->bytecode.size(),
                                            &o.layout);
    if (FAILED(hr)) {
      ITC_LOG_ERROR("D3D11: an input layout couldn't be made: {:08X}", uint32_t(hr));
      return nullptr;
    }
  }
  switch (Topology(desc.topology)) {
    case Topology::kPointList: o.topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
    case Topology::kLineList: o.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
    case Topology::kLineStrip: o.topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
    case Topology::kTriangleList: o.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
    case Topology::kTriangleStrip: o.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
    case Topology::kTriangleFan:  // as lists of indices made for it
      o.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
      o.fan = true;
      break;
  }
  o.culls_all = CullMode(desc.cull_mode) == CullMode::kFrontAndBack;
  D3D11_DEPTH_STENCIL_DESC depth{};
  depth.DepthEnable = desc.depth && desc.z_enable;
  depth.DepthWriteMask = desc.z_write ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
  depth.DepthFunc = D3D11_COMPARISON_FUNC(desc.z_func + 1);  // the GPU's order is D3D's minus one
  if (FAILED(device_->CreateDepthStencilState(&depth, &o.depth))) {
    return nullptr;
  }
  D3D11_BLEND_DESC blend{};
  D3D11_RENDER_TARGET_BLEND_DESC& target = blend.RenderTarget[0];
  target.BlendEnable = desc.blend_enable;
  target.SrcBlend = Blend(BlendFactor(desc.src_color), false);
  target.DestBlend = Blend(BlendFactor(desc.dst_color), false);
  target.BlendOp = BlendOperation(BlendOp(desc.color_op));
  target.SrcBlendAlpha = Blend(BlendFactor(desc.src_alpha), true);
  target.DestBlendAlpha = Blend(BlendFactor(desc.dst_alpha), true);
  target.BlendOpAlpha = BlendOperation(BlendOp(desc.alpha_op));
  target.RenderTargetWriteMask =
      Format(desc.color_format) == Format::kUndefined ? 0 : UINT8(desc.write_mask & 0xF);
  auto alpha_constant = [](uint32_t f) {
    return BlendFactor(f) == BlendFactor::kConstantAlpha ||
           BlendFactor(f) == BlendFactor::kOneMinusConstantAlpha;
  };
  o.constant_alpha = desc.blend_enable && (alpha_constant(desc.src_color) ||
                                           alpha_constant(desc.dst_color));
  if (FAILED(device_->CreateBlendState(&blend, &o.blend))) {
    return nullptr;
  }
  const std::string layout = std::format("t{}s{}/t{}s{}", o.vertex->textures, o.vertex->samplers,
                                         o.pixel ? o.pixel->textures : 0,
                                         o.pixel ? o.pixel->samplers : 0);
  auto& interned = resource_layouts_[layout];
  if (!interned) {
    interned.reset(new char);
  }
  o.resource_layout = interned.get();
  it->second = std::move(object);
  return reinterpret_cast<Pipeline*>(it->second.get());
}

TextureSet* Backend::MakeTextureSet(Shader* shader, ShaderStage stage, View* const* views,
                                    uint32_t view_count, Sampler* const* samplers,
                                    uint32_t sampler_count, bool* found) {
  Frame& frame = frames_[frame_index_];
  std::string key(sizeof(void*) * (view_count + sampler_count) + 1, '\0');
  key[0] = char(stage);
  std::memcpy(key.data() + 1, views, sizeof(void*) * view_count);
  std::memcpy(key.data() + 1 + sizeof(void*) * view_count, samplers,
              sizeof(void*) * sampler_count);
  if (auto it = frame.set_cache.find(key); it != frame.set_cache.end()) {
    if (found) {
      *found = true;
    }
    return reinterpret_cast<TextureSet*>(it->second);
  }
  auto set = std::make_unique<TextureSetObject>();
  for (uint32_t i = 0; i < view_count; i++) {
    set->views.push_back(reinterpret_cast<ViewObject*>(views[i]));
  }
  for (uint32_t i = 0; i < sampler_count; i++) {
    set->samplers.push_back(reinterpret_cast<ID3D11SamplerState*>(samplers[i]));
  }
  TextureSetObject* result = set.get();
  frame.sets.push_back(std::move(set));
  frame.set_cache.emplace(std::move(key), result);
  return reinterpret_cast<TextureSet*>(result);
}

bool Backend::DescribeConstants(Buffer* buffer, const uint32_t ranges[4]) {
  BufferObject& o = Object(buffer);
  if (!o.buffer || !o.memory) {
    return false;
  }
  std::copy(ranges, ranges + 4, o.ranges);
  return true;
}

// A stage's textures as bound (a view that's a target now is left out: D3D11
// would unbind the target for it).
void Backend::ApplyStageResources(bool vertex_stage) {
  const int s = vertex_stage ? 0 : 1;
  ID3D11ShaderResourceView* views[32] = {};
  ID3D11SamplerState* samplers[16] = {};
  uint32_t count = 0, sampler_count = 0;
  if (TextureSetObject* set = stage_sets_[s]) {
    count = std::min<uint32_t>(uint32_t(set->views.size()), 32);
    for (uint32_t i = 0; i < count; i++) {
      ViewObject* view = set->views[i];
      if (!view || view->image == color_image_ || view->image == depth_image_) {
        continue;
      }
      views[i] = view->swizzled && view->copy ? view->copy->srv.Get() : view->srv.Get();
    }
    sampler_count = std::min<uint32_t>(uint32_t(set->samplers.size()), 16 - std::min(count, 16u));
    std::copy(set->samplers.begin(), set->samplers.begin() + sampler_count, samplers);
  }
  const uint32_t clear = std::max(count, stage_view_counts_[s]);
  if (vertex_stage) {
    context_->VSSetShaderResources(0, clear, views);
    if (sampler_count) context_->VSSetSamplers(count, sampler_count, samplers);
  } else {
    context_->PSSetShaderResources(0, clear, views);
    if (sampler_count) context_->PSSetSamplers(count, sampler_count, samplers);
  }
  stage_view_counts_[s] = count;
}

void Backend::BindResources(Pipeline* pipeline, TextureSet* vertex, TextureSet* pixel,
                            Buffer* constants, const uint32_t offsets[4]) {
  PipelineObject& p = Object(pipeline);
  // Swizzled views drawn again where their images changed (own passes,
  // before the constants are bound), then the constants and textures.
  for (TextureSet* set : {vertex, pixel}) {
    if (set) {
      for (ViewObject* view : reinterpret_cast<TextureSetObject*>(set)->views) {
        if (view && view->swizzled && view->copied != view->image->generation) {
          RefreshSwizzle(*view);
        }
      }
    }
  }
  // The constant blocks: their ranges copied from our memory into the
  // buffer the shaders read (no frame in flight reads the slot's ranges).
  BufferObject& c = Object(constants);
  ID3D11Buffer* buffers[4];
  UINT first[4], number[4];
  D3D11_MAPPED_SUBRESOURCE mapped{};
  const D3D11_MAP map = c.mapped_once ? D3D11_MAP_WRITE_NO_OVERWRITE : D3D11_MAP_WRITE_DISCARD;
  if (SUCCEEDED(context_->Map(c.buffer.Get(), 0, map, 0, &mapped))) {
    c.mapped_once = true;
    const uint64_t total = (c.desc.size + 255) & ~uint64_t(255);
    for (int i = 0; i < 4; i++) {
      const uint64_t size = std::min<uint64_t>((c.ranges[i] + 15) & ~15u, c.desc.size - offsets[i]);
      std::memcpy(static_cast<uint8_t*>(mapped.pData) + offsets[i], c.memory.get() + offsets[i],
                  size);
      buffers[i] = c.buffer.Get();
      first[i] = offsets[i] / 16;
      // A multiple of 16 constants, within the buffer.
      number[i] = UINT(std::min<uint64_t>((c.ranges[i] + 255) / 256 * 16, total / 16 - first[i]));
      number[i] &= ~15u;
    }
    context_->Unmap(c.buffer.Get(), 0);
    context_->VSSetConstantBuffers1(0, 4, buffers, first, number);
    if (!p.id_pixel) {
      context_->PSSetConstantBuffers1(0, 4, buffers, first, number);
      std::copy(buffers, buffers + 4, pixel_constants_);
      std::copy(first, first + 4, pixel_first_);
      std::copy(number, number + 4, pixel_number_);
    }
  }
  stage_sets_[0] = reinterpret_cast<TextureSetObject*>(vertex);
  stage_sets_[1] = reinterpret_cast<TextureSetObject*>(pixel);
  ApplyStageResources(true);
  ApplyStageResources(false);
}

void Backend::ApplyTargets() {
  context_->OMSetRenderTargets(color_target_ ? 1 : 0, &color_target_, depth_target_);
}

void Backend::BeginPass(const PassTarget& color, const PassTarget& depth, const Rect& area) {
  color_image_ = color.image ? &Object(color.image) : nullptr;
  depth_image_ = depth.image ? &Object(depth.image) : nullptr;
  color_target_ = color_image_ ? color_image_->rtv.Get() : nullptr;
  depth_target_ = depth_image_ ? depth_image_->dsv.Get() : nullptr;
  ApplyTargets();
  in_pass_ = true;
  const D3D11_RECT rect{area.x, area.y, LONG(area.x + int32_t(area.width)),
                        LONG(area.y + int32_t(area.height))};
  if (color_image_ && color.load == LoadOp::kClear && color_target_) {
    float value[4];
    ClearFloats(color_image_->desc.format, color.clear_color, value);
    context_->ClearView(color_target_, value, &rect, 1);  // the pass's area alone, as Vulkan's
  }
  if (depth_image_ && depth.load == LoadOp::kClear && depth_target_) {
    // D3D11 clears all of a depth view.
    context_->ClearDepthStencilView(depth_target_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                    depth.clear_depth, UINT8(depth.clear_stencil));
  }
  if (color_image_) Written(*color_image_);
  if (depth_image_) Written(*depth_image_);
  // Binding the targets unbinds them where they were textures: the stages'
  // textures again (the targets left out).
  ApplyStageResources(true);
  ApplyStageResources(false);
}

void Backend::EndPass() {
  in_pass_ = false;
  color_image_ = depth_image_ = nullptr;
  color_target_ = nullptr;
  depth_target_ = nullptr;
  context_->OMSetRenderTargets(0, nullptr, nullptr);
}

void Backend::SetViewport(const Viewport& v) {
  viewport_ = {v.x, v.y, v.width, v.height, v.min_depth, v.max_depth};
  context_->RSSetViewports(1, &viewport_);
}

void Backend::SetScissor(const Rect& r) {
  scissor_ = {r.x, r.y, LONG(r.x + int32_t(r.width)), LONG(r.y + int32_t(r.height))};
  context_->RSSetScissorRects(1, &scissor_);
}

void Backend::SetDepthBias(float constant, float slope) {
  if (constant != bias_constant_ || slope != bias_slope_) {
    bias_constant_ = constant;
    bias_slope_ = slope;
    raster_dirty_ = true;
  }
}

void Backend::SetBlendConstants(const float constants[4]) {
  std::copy(constants, constants + 4, blend_constants_);
  blend_dirty_ = true;
}

void Backend::BindPipeline(Pipeline* pipeline) {
  PipelineObject* p = &Object(pipeline);
  if (p != pipeline_) {
    pipeline_ = p;
    pipeline_dirty_ = raster_dirty_ = blend_dirty_ = vertex_buffers_dirty_ = true;
  }
}

void Backend::BindVertexBuffers(Buffer* buffer, uint32_t count, const uint64_t* offsets) {
  vertex_buffer_ = &Object(buffer);
  vertex_buffer_count_ = std::min<uint32_t>(count, 32);
  std::copy(offsets, offsets + vertex_buffer_count_, vertex_offsets_);
  vertex_buffers_dirty_ = true;
}

void Backend::BindIndexBuffer(Buffer* buffer, uint64_t offset, bool index_32bit) {
  index_buffer_ = &Object(buffer);
  index_offset_ = offset;
  index_32bit_ = index_32bit;
  index_dirty_ = true;
}

// What changed since the last draw, applied.
bool Backend::FlushDraw() {
  PipelineObject* p = pipeline_;
  if (!p) {
    return false;
  }
  if (pipeline_dirty_) {
    context_->IASetInputLayout(p->layout.Get());
    context_->IASetPrimitiveTopology(p->topology);
    context_->VSSetShader(p->vertex->vertex.Get(), nullptr, 0);
    context_->PSSetShader(p->pixel ? p->pixel->pixel.Get() : p->id_pixel.Get(), nullptr, 0);
    context_->OMSetDepthStencilState(p->depth.Get(), 0);
    ID3D11Buffer* info = vertex_info_buffer_.Get();
    context_->VSSetConstantBuffers(4, 1, &info);
    pipeline_dirty_ = false;
  }
  if (blend_dirty_) {
    float factor[4] = {blend_constants_[0], blend_constants_[1], blend_constants_[2],
                       blend_constants_[3]};
    if (p->constant_alpha) {  // the constant's alpha for color too
      factor[0] = factor[1] = factor[2] = factor[3];
    }
    context_->OMSetBlendState(p->blend.Get(), factor, 0xFFFFFFFF);
    blend_dirty_ = false;
  }
  if (raster_dirty_) {
    // Depth bias only with a depth target (as Vulkan's pipelines enable
    // it); D3D's constant is a whole number of the depth format's units.
    const int32_t bias = p->desc.depth ? int32_t(std::lround(bias_constant_)) : 0;
    const float slope = p->desc.depth ? bias_slope_ : 0.0f;
    const uint64_t key = (uint64_t(uint32_t(bias)) << 32) ^ std::bit_cast<uint32_t>(slope) ^
                         (uint64_t(p->desc.cull_mode & 3) << 61) ^
                         (uint64_t(p->desc.front_clockwise & 1) << 60);
    ComPtr<ID3D11RasterizerState>& state = rasterizers_[key];
    if (!state) {
      D3D11_RASTERIZER_DESC raster{};
      raster.FillMode = D3D11_FILL_SOLID;
      const CullMode cull = CullMode(p->desc.cull_mode);
      raster.CullMode = cull == CullMode::kFront  ? D3D11_CULL_FRONT
                        : cull == CullMode::kBack ? D3D11_CULL_BACK
                                                  : D3D11_CULL_NONE;
      // The frame on screen is the same in both (the vertex shaders' Y
      // flip and D3D's viewport cancel), and both name winding as seen
      // there.
      raster.FrontCounterClockwise = p->desc.front_clockwise ? FALSE : TRUE;
      raster.DepthBias = bias;
      raster.SlopeScaledDepthBias = slope;
      raster.DepthClipEnable = TRUE;
      raster.ScissorEnable = TRUE;
      device_->CreateRasterizerState(&raster, &state);
    }
    context_->RSSetState(state.Get());
    raster_dirty_ = false;
  }
  if (vertex_buffers_dirty_ && vertex_buffer_ && vertex_buffer_->buffer) {
    ID3D11Buffer* buffers[32];
    UINT strides[32], offsets[32];
    for (uint32_t i = 0; i < vertex_buffer_count_; i++) {
      buffers[i] = vertex_buffer_->buffer.Get();
      strides[i] = p->strides[i];
      offsets[i] = UINT(vertex_offsets_[i]);
    }
    context_->IASetVertexBuffers(0, vertex_buffer_count_, buffers, strides, offsets);
    vertex_buffers_dirty_ = false;
  }
  if (index_dirty_ && index_buffer_ && index_buffer_->buffer) {
    context_->IASetIndexBuffer(index_buffer_->buffer.Get(),
                               index_32bit_ ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
                               UINT(index_offset_));
    index_dirty_ = false;
  }
  return true;
}

void Backend::Draw(uint32_t vertices, uint32_t instances, uint32_t first_vertex,
                   uint32_t first_instance) {
  if (!pipeline_ || pipeline_->culls_all || !FlushDraw()) {
    return;
  }
  // D3D's vertex ID counts from the first vertex already; its instance ID
  // doesn't from the first instance.
  if (vertex_info_[0] != 0 || vertex_info_[1] != int32_t(first_instance)) {
    vertex_info_[0] = 0;
    vertex_info_[1] = int32_t(first_instance);
    const int32_t info[4] = {vertex_info_[0], vertex_info_[1], 0, 0};
    context_->UpdateSubresource(vertex_info_buffer_.Get(), 0, nullptr, info, 0, 0);
  }
  if (!pipeline_->fan) {
    context_->DrawInstanced(vertices, instances, first_vertex, first_instance);
    return;
  }
  if (vertices < 3) {
    return;
  }
  // A fan as a list: (first, first + i, first + i + 1).
  const uint32_t count = 3 * (vertices - 2);
  const UINT bytes = count * 4;
  D3D11_BUFFER_DESC desc;
  fan_indices_->GetDesc(&desc);
  if (bytes > desc.ByteWidth) {
    return;
  }
  D3D11_MAP map = D3D11_MAP_WRITE_NO_OVERWRITE;
  if (fan_index_at_ + bytes > desc.ByteWidth) {
    map = D3D11_MAP_WRITE_DISCARD;
    fan_index_at_ = 0;
  }
  D3D11_MAPPED_SUBRESOURCE mapped{};
  if (FAILED(context_->Map(fan_indices_.Get(), 0, map, 0, &mapped))) {
    return;
  }
  uint32_t* out = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(mapped.pData) + fan_index_at_);
  for (uint32_t i = 1; i + 1 < vertices; i++) {
    *out++ = first_vertex;
    *out++ = first_vertex + i;
    *out++ = first_vertex + i + 1;
  }
  context_->Unmap(fan_indices_.Get(), 0);
  context_->IASetIndexBuffer(fan_indices_.Get(), DXGI_FORMAT_R32_UINT, 0);
  context_->DrawIndexedInstanced(count, instances, fan_index_at_ / 4, 0, first_instance);
  fan_index_at_ += bytes;
  index_dirty_ = true;  // the renderer's index buffer again at the next indexed draw
}

void Backend::DrawIndexed(uint32_t indices, uint32_t instances, uint32_t first_index,
                          int32_t base_vertex, uint32_t first_instance) {
  if (!pipeline_ || pipeline_->culls_all) {
    return;
  }
  if (pipeline_->fan) {
    if (!warned_indexed_fan_) {
      warned_indexed_fan_ = true;
      ITC_LOG_WARN("D3D11: an indexed triangle fan, not drawn (not done yet)");
    }
    return;
  }
  if (!FlushDraw()) {
    return;
  }
  if (vertex_info_[0] != base_vertex || vertex_info_[1] != int32_t(first_instance)) {
    vertex_info_[0] = base_vertex;
    vertex_info_[1] = int32_t(first_instance);
    const int32_t info[4] = {vertex_info_[0], vertex_info_[1], 0, 0};
    context_->UpdateSubresource(vertex_info_buffer_.Get(), 0, nullptr, info, 0, 0);
  }
  context_->DrawIndexedInstanced(indices, instances, first_index, base_vertex, first_instance);
}

// Copies and clears in the frame (outside passes).

void Backend::ClearColor(Image* image, const uint32_t bits[4]) {
  ImageObject& o = Object(image);
  if (!o.rtv) {
    return;
  }
  float value[4];
  ClearFloats(o.desc.format, bits, value);
  context_->ClearRenderTargetView(o.rtv.Get(), value);
  Written(o);
}

void Backend::ClearDepthStencil(Image* image, float depth, uint32_t stencil, bool clear_depth,
                                bool clear_stencil) {
  ImageObject& o = Object(image);
  if (!o.dsv) {
    return;
  }
  context_->ClearDepthStencilView(
      o.dsv.Get(), (clear_depth ? D3D11_CLEAR_DEPTH : 0) | (clear_stencil ? D3D11_CLEAR_STENCIL : 0),
      depth, UINT8(stencil));
  Written(o);
}

void Backend::CopyImage(Image* source, Image* dest, uint32_t width, uint32_t height) {
  ImageObject& from = Object(source);
  ImageObject& to = Object(dest);
  // Depth and multisampled images copy whole subresources only.
  const bool whole = IsDepth(from.desc.format) || from.desc.samples > 1;
  if (whole) {
    if (from.desc.width != to.desc.width || from.desc.height != to.desc.height) {
      if (!warned_copy_) {
        warned_copy_ = true;
        ITC_LOG_WARN("D3D11: a depth or multisampled copy between sizes, not made");
      }
      return;
    }
    context_->CopySubresourceRegion(to.resource.Get(), 0, 0, 0, 0, from.resource.Get(), 0,
                                    nullptr);
  } else {
    D3D11_BOX box{0, 0, 0, width, height, 1};
    context_->CopySubresourceRegion(to.resource.Get(), 0, 0, 0, 0, from.resource.Get(), 0, &box);
  }
  Written(to);
}

void Backend::BlitImage(Image* source, const Rect& from, Image* dest, const Rect& to,
                        Filter filter) {
  ImageObject& s = Object(source);
  ImageObject& d = Object(dest);
  if (!s.srv_2d || !d.rtv) {
    return;
  }
  const float constants[8] = {float(to.x), float(to.y), float(to.width), float(to.height),
                              float(from.x) / float(s.desc.width),
                              float(from.y) / float(s.desc.height),
                              float(from.width) / float(s.desc.width),
                              float(from.height) / float(s.desc.height)};
  BeginOwnPass(d.rtv.Get(), to, blit_, constants);
  ID3D11ShaderResourceView* view = s.srv_2d.Get();
  ID3D11SamplerState* sampler = SamplerFor(filter);
  context_->PSSetShaderResources(0, 1, &view);
  context_->PSSetSamplers(0, 1, &sampler);
  context_->Draw(3, 0);
  EndOwnPass();
  Written(d);
}

void Backend::ResolveImage(Image* source, Image* dest, uint32_t width, uint32_t height) {
  ImageObject& from = Object(source);
  ImageObject& to = Object(dest);
  if (from.desc.width == to.desc.width && from.desc.height == to.desc.height) {
    context_->ResolveSubresource(to.resource.Get(), 0, from.resource.Get(), 0, from.format);
  } else {
    // Through an image of the source's size, then the corner copied.
    if (!resolve_temp_ || resolve_temp_->desc.width != from.desc.width ||
        resolve_temp_->desc.height != from.desc.height ||
        resolve_temp_->desc.format != from.desc.format) {
      ImageDesc desc = from.desc;
      desc.samples = 1;
      desc.usage = kImageCopySource | kImageCopyDest;
      resolve_temp_.reset(reinterpret_cast<ImageObject*>(CreateImage(desc)));
      if (!resolve_temp_) {
        return;
      }
    }
    context_->ResolveSubresource(resolve_temp_->resource.Get(), 0, from.resource.Get(), 0,
                                 from.format);
    D3D11_BOX box{0, 0, 0, width, height, 1};
    context_->CopySubresourceRegion(to.resource.Get(), 0, 0, 0, 0, resolve_temp_->resource.Get(),
                                    0, &box);
  }
  Written(to);
}

void Backend::CopyBufferToImage(Buffer* buffer, Image* image, const ImageUpload* regions,
                                uint32_t count) {
  UploadImage(buffer, image, regions, count);
}

void Backend::CopyImageToBuffer(Image* image, Buffer* buffer, uint64_t offset, uint32_t width,
                                uint32_t height) {
  ImageObject& o = Object(image);
  BufferObject& b = Object(buffer);
  if (!b.memory || o.desc.type == ImageType::k3D || o.desc.samples > 1) {
    return;
  }
  const bool depth = IsDepth(o.desc.format);
  // A depth image's copy is of all of it (D3D11's rule).
  D3D11_TEXTURE2D_DESC desc{};
  desc.Width = depth ? o.desc.width : width;
  desc.Height = depth ? o.desc.height : height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = o.format;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_STAGING;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
  Readback r;
  if (FAILED(device_->CreateTexture2D(&desc, nullptr, &r.staging))) {
    return;
  }
  if (depth) {
    context_->CopySubresourceRegion(r.staging.Get(), 0, 0, 0, 0, o.resource.Get(), 0, nullptr);
  } else {
    D3D11_BOX box{0, 0, 0, width, height, 1};
    context_->CopySubresourceRegion(r.staging.Get(), 0, 0, 0, 0, o.resource.Get(), 0, &box);
  }
  r.buffer = &b;
  r.offset = offset;
  r.width = width;
  r.height = height;
  r.texel_bytes = TexelBytes(o.desc.format);
  r.depth = depth;
  frames_[frame_index_].readbacks.push_back(std::move(r));
}

// Views: D3D11's have no swizzle; a swizzled one is a copy of its image
// through the swizzle (docs/d3d11-backend.md, open question 4), drawn again
// when the image changes.
View* Backend::CreateView(Image* image, const ViewDesc& desc) {
  auto object = std::make_unique<ViewObject>();
  ViewObject& v = *object;
  ImageObject& o = Object(image);
  v.image = &o;
  v.desc = desc;
  const bool identity = desc.swizzle[0] == Swizzle::kR && desc.swizzle[1] == Swizzle::kG &&
                        desc.swizzle[2] == Swizzle::kB && desc.swizzle[3] == Swizzle::kA;
  const bool depth = IsDepth(o.desc.format);
  D3D11_SHADER_RESOURCE_VIEW_DESC srv{};
  srv.Format = depth ? kDepthSampled : o.format;
  auto dimension = [&](const ImageObject& of, D3D11_SHADER_RESOURCE_VIEW_DESC& d) {
    const uint32_t levels = of.desc.levels, layers = of.desc.layers;
    if (desc.type == ImageType::k3D) {
      d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
      d.Texture3D.MipLevels = levels;
    } else if (of.desc.samples > 1) {
      d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
      d.Texture2DMSArray.ArraySize = layers;
    } else if (desc.type == ImageType::kCube) {
      d.ViewDimension =
          layers > 6 ? D3D11_SRV_DIMENSION_TEXTURECUBEARRAY : D3D11_SRV_DIMENSION_TEXTURECUBE;
      d.TextureCubeArray.MipLevels = levels;
      d.TextureCubeArray.NumCubes = layers / 6;
    } else if (desc.type == ImageType::k2DArray || layers > 1) {
      d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
      d.Texture2DArray.MipLevels = levels;
      d.Texture2DArray.ArraySize = layers;
    } else {
      d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      d.Texture2D.MipLevels = levels;
    }
  };
  dimension(o, srv);
  if (FAILED(device_->CreateShaderResourceView(o.resource.Get(), &srv, &v.srv))) {
    return nullptr;
  }
  if (!identity && !depth) {
    if (IsInteger(o.desc.format) || o.desc.type == ImageType::k3D || o.desc.samples > 1 ||
        !o.srv_array) {
      if (!warned_swizzle_) {
        warned_swizzle_ = true;
        ITC_LOG_WARN("D3D11: a swizzled view of format {} type {}, not swizzled",
                     int(o.desc.format), int(o.desc.type));
      }
    } else {
      // The copy: the same shape, in a format it can be drawn into.
      ImageDesc copy = o.desc;
      copy.format = IsCompressed(o.desc.format)  // (a decoded one too)
                        ? (o.desc.format == Format::kBC5 ? Format::kR8G8Unorm
                                                         : Format::kR8G8B8A8Unorm)
                        : o.desc.format;
      copy.usage = kImageSampled | kImageColorTarget;
      Image* made = CreateImage(copy);
      if (made) {
        v.copy.reset(&Object(made));
        D3D11_SHADER_RESOURCE_VIEW_DESC copy_srv{};
        copy_srv.Format = v.copy->format;
        dimension(*v.copy, copy_srv);
        v.copy->srv.Reset();
        device_->CreateShaderResourceView(v.copy->resource.Get(), &copy_srv, &v.copy->srv);
        v.swizzled = true;
      }
    }
  }
  return reinterpret_cast<View*>(object.release());
}

void Backend::DestroyView(View* view) {
  if (view) {
    delete reinterpret_cast<ViewObject*>(view);
  }
}

void Backend::RefreshSwizzle(ViewObject& v) {
  ImageObject& from = *v.image;
  ImageObject& to = *v.copy;
  const uint32_t levels = to.desc.levels, layers = to.desc.layers;
  if (v.copy_targets.empty()) {
    v.copy_targets.resize(size_t(levels) * layers);
    for (uint32_t level = 0; level < levels; level++) {
      for (uint32_t layer = 0; layer < layers; layer++) {
        D3D11_RENDER_TARGET_VIEW_DESC rtv{};
        rtv.Format = to.format;
        rtv.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        rtv.Texture2DArray = {level, layer, 1};
        device_->CreateRenderTargetView(to.resource.Get(), &rtv,
                                        &v.copy_targets[size_t(level) * layers + layer]);
      }
    }
  }
  struct {
    int32_t select[4];
    int32_t level, layer;
  } constants;
  for (int i = 0; i < 4; i++) {
    constants.select[i] = int32_t(v.desc.swizzle[i]);  // kR..kA 0-3, kZero 4, kOne 5
  }
  for (uint32_t level = 0; level < levels; level++) {
    const Rect area{0, 0, std::max(to.desc.width >> level, 1u), std::max(to.desc.height >> level, 1u)};
    for (uint32_t layer = 0; layer < layers; layer++) {
      ID3D11RenderTargetView* target = v.copy_targets[size_t(level) * layers + layer].Get();
      if (!target) {
        continue;
      }
      constants.level = int32_t(level);
      constants.layer = int32_t(layer);
      BeginOwnPass(target, area, swizzle_, &constants);
      ID3D11ShaderResourceView* view = from.srv_array.Get();
      context_->PSSetShaderResources(0, 1, &view);
      context_->Draw(3, 0);
      EndOwnPass();
    }
  }
  v.copied = from.generation;
}

// The backend's own passes: a full-screen triangle into one target, the
// renderer's state put back after.
void Backend::BeginOwnPass(ID3D11RenderTargetView* target, const Rect& area, OwnPass& pass,
                           const void* constants) {
  ID3D11ShaderResourceView* none[3] = {};
  context_->PSSetShaderResources(0, 3, none);
  context_->OMSetRenderTargets(1, &target, nullptr);
  const D3D11_VIEWPORT viewport{float(area.x), float(area.y), float(area.width),
                                float(area.height), 0.0f, 1.0f};
  const D3D11_RECT scissor{area.x, area.y, LONG(area.x + int32_t(area.width)),
                           LONG(area.y + int32_t(area.height))};
  context_->RSSetViewports(1, &viewport);
  context_->RSSetScissorRects(1, &scissor);
  context_->RSSetState(own_rasterizer_.Get());
  context_->OMSetBlendState(no_blend_.Get(), nullptr, 0xFFFFFFFF);
  context_->OMSetDepthStencilState(no_depth_.Get(), 0);
  context_->IASetInputLayout(nullptr);
  context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  context_->VSSetShader(fullscreen_vertex_.Get(), nullptr, 0);
  context_->PSSetShader(pass.pixel.Get(), nullptr, 0);
  if (vertex_info_[0] != 0 || vertex_info_[1] != 0) {
    vertex_info_[0] = vertex_info_[1] = 0;
    const int32_t info[4] = {};
    context_->UpdateSubresource(vertex_info_buffer_.Get(), 0, nullptr, info, 0, 0);
  }
  ID3D11Buffer* info = vertex_info_buffer_.Get();
  context_->VSSetConstantBuffers(4, 1, &info);
  if (pass.constants) {
    D3D11_MAPPED_SUBRESOURCE mapped{};
    if (SUCCEEDED(context_->Map(pass.constants.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) {
      std::memcpy(mapped.pData, constants, pass.constants_size);
      context_->Unmap(pass.constants.Get(), 0);
    }
    ID3D11Buffer* buffer = pass.constants.Get();
    context_->PSSetConstantBuffers(0, 1, &buffer);
  }
}

void Backend::EndOwnPass() {
  ID3D11ShaderResourceView* none[3] = {};
  context_->PSSetShaderResources(0, 3, none);
  context_->OMSetRenderTargets(0, nullptr, nullptr);
  // The renderer's state again: its targets, viewport and scissor, and what
  // the next draw sets (pipeline, constants, textures).
  if (in_pass_) {
    ApplyTargets();
  }
  context_->RSSetViewports(1, &viewport_);
  context_->RSSetScissorRects(1, &scissor_);
  pipeline_dirty_ = raster_dirty_ = blend_dirty_ = true;
  stage_view_counts_[1] = std::max<uint32_t>(stage_view_counts_[1], 3);
  ApplyStageResources(false);
  if (pixel_constants_[0]) {  // the pass's block 0 replaced the draw's
    context_->PSSetConstantBuffers1(0, 4, pixel_constants_, pixel_first_, pixel_number_);
  }
}

// The renderer's own passes.

bool Backend::DrawResolve(const ResolveDraw& draw) {
  ImageObject& source = Object(draw.source);
  ImageObject& dest = Object(draw.dest);
  if (!source.srv_2d || !dest.rtv) {
    return false;
  }
  const bool multisample = source.desc.samples > 1;
  const struct {
    int32_t offset[2];
    int32_t samples;
    float scale;
    int32_t swap_red_blue;
  } constants{{draw.offset[0], draw.offset[1]}, draw.samples, draw.scale,
              draw.swap_red_blue ? 1 : 0};
  BeginOwnPass(dest.rtv.Get(), draw.area, multisample ? resolve_ms_ : resolve_, &constants);
  ID3D11ShaderResourceView* view = source.srv_2d.Get();
  ID3D11SamplerState* sampler = point_sampler_.Get();
  context_->PSSetShaderResources(0, 1, &view);
  context_->PSSetSamplers(0, 1, &sampler);
  context_->Draw(3, 0);
  EndOwnPass();
  Written(dest);
  return true;
}

bool Backend::DrawMovie(Image* const planes[3], Image* target_image, const Rect& area) {
  ImageObject& target = Object(target_image);
  if (!target.rtv) {
    return false;
  }
  const float constants[4] = {float(area.x), float(area.y), float(area.width),
                              float(area.height)};
  BeginOwnPass(target.rtv.Get(), area, movie_, constants);
  ID3D11ShaderResourceView* views[3];
  ID3D11SamplerState* samplers[3];
  for (int i = 0; i < 3; i++) {
    views[i] = Object(planes[i]).srv_2d.Get();
    samplers[i] = linear_sampler_.Get();
  }
  context_->PSSetShaderResources(0, 3, views);
  context_->PSSetSamplers(0, 3, samplers);
  context_->Draw(3, 0);
  EndOwnPass();
  Written(target);
  return true;
}

bool Backend::ApplyGamma(Image* image, Extent2D size, const uint16_t table[256]) {
  ImageObject& o = Object(image);
  if (gamma_failed_ || !o.uav) {
    return false;
  }
  if (!gamma_) {
    gamma_failed_ = true;  // until made
    D3D11_FEATURE_DATA_FORMAT_SUPPORT2 support{DXGI_FORMAT_R16G16B16A16_FLOAT, 0};
    if (FAILED(device_->CheckFeatureSupport(D3D11_FEATURE_FORMAT_SUPPORT2, &support,
                                            sizeof(support))) ||
        !(support.OutFormatSupport2 & D3D11_FORMAT_SUPPORT2_UAV_TYPED_LOAD)) {
      ITC_LOG_ERROR("D3D11: --xbox_gamma needs typed UAV loads of RGBA16F, which this GPU lacks");
      return false;
    }
    std::vector<uint8_t> code = Compile(k_hlsl_gamma_comp, "cs_5_0", {});
    if (code.empty() ||
        FAILED(device_->CreateComputeShader(code.data(), code.size(), nullptr, &gamma_))) {
      return false;
    }
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = 256 * 4;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
    if (FAILED(device_->CreateBuffer(&desc, nullptr, &gamma_table_))) {
      return false;
    }
    D3D11_SHADER_RESOURCE_VIEW_DESC srv{};
    srv.Format = DXGI_FORMAT_R32_TYPELESS;
    srv.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    srv.BufferEx = {0, 256, D3D11_BUFFEREX_SRV_FLAG_RAW};
    if (FAILED(device_->CreateShaderResourceView(gamma_table_.Get(), &srv, &gamma_table_view_))) {
      return false;
    }
    desc = {};
    desc.ByteWidth = 16;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    if (FAILED(device_->CreateBuffer(&desc, nullptr, &gamma_constants_))) {
      return false;
    }
    gamma_failed_ = false;
  }
  if (!gamma_uploaded_) {  // the table, once
    uint32_t entries[256];
    std::copy(table, table + 256, entries);
    context_->UpdateSubresource(gamma_table_.Get(), 0, nullptr, entries, 0, 0);
    gamma_uploaded_ = true;
  }
  D3D11_MAPPED_SUBRESOURCE mapped{};
  if (SUCCEEDED(context_->Map(gamma_constants_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) {
    const int32_t extent[4] = {int32_t(size.width), int32_t(size.height), 0, 0};
    std::memcpy(mapped.pData, extent, sizeof(extent));
    context_->Unmap(gamma_constants_.Get(), 0);
  }
  ID3D11UnorderedAccessView* uav = o.uav.Get();
  ID3D11ShaderResourceView* views[2] = {nullptr, gamma_table_view_.Get()};
  ID3D11Buffer* constants = gamma_constants_.Get();
  context_->CSSetShader(gamma_.Get(), nullptr, 0);
  context_->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);
  context_->CSSetShaderResources(0, 2, views);
  context_->CSSetConstantBuffers(0, 1, &constants);
  context_->Dispatch((size.width + 7) / 8, (size.height + 7) / 8, 1);
  ID3D11UnorderedAccessView* no_uav = nullptr;
  ID3D11ShaderResourceView* no_views[2] = {};
  context_->CSSetUnorderedAccessViews(0, 1, &no_uav, nullptr);
  context_->CSSetShaderResources(0, 2, no_views);
  Written(o);
  return true;
}

// The draw ID pass: the draw's vertex side with id.frag.
Pipeline* Backend::GetIdPipeline(const PipelineDesc& desc) {
  PipelineDesc key = desc;
  key.pixel = nullptr;
  key.blend_enable = key.src_color = key.dst_color = key.color_op = 0;
  key.src_alpha = key.dst_alpha = key.alpha_op = key.write_mask = 0;
  key.color_format = key.samples = 0;
  key.depth = 1;
  auto [it, added] = id_pipelines_.try_emplace(key);
  if (!added) {
    return it->second ? reinterpret_cast<Pipeline*>(it->second.get()) : nullptr;
  }
  PipelineDesc build = key;
  build.write_mask = 0xF;
  build.color_format = uint32_t(Format::kR32Uint);
  build.samples = 1;
  Pipeline* base = GetPipeline(build);  // its vertex input and states
  if (!base) {
    return nullptr;
  }
  auto object = std::make_unique<PipelineObject>(Object(base));
  object->id_pixel = id_pixel_;
  const std::string layout = std::format("id t{}s{}", object->vertex->textures,
                                         object->vertex->samplers);
  auto& interned = resource_layouts_[layout];
  if (!interned) {
    interned.reset(new char);
  }
  object->resource_layout = interned.get();
  it->second = std::move(object);
  return reinterpret_cast<Pipeline*>(it->second.get());
}

void Backend::SetDrawId(uint32_t id) {
  D3D11_MAPPED_SUBRESOURCE mapped{};
  if (SUCCEEDED(context_->Map(id_constants_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) {
    const uint32_t values[4] = {id, 0, 0, 0};
    std::memcpy(mapped.pData, values, sizeof(values));
    context_->Unmap(id_constants_.Get(), 0);
  }
  ID3D11Buffer* buffer = id_constants_.Get();
  context_->PSSetConstantBuffers(0, 1, &buffer);
}

}  // namespace gpu::d3d11
