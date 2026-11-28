// Compiles HLSL files with D3DCompile (d3dcompiler_47.dll), as the D3D11
// backend does at run time: the check that every shader compiles
// (docs/d3d11-backend.md, step 4). A Windows tool, run under Wine with
// Microsoft's d3dcompiler_47.dll in the prefix (Wine's own is vkd3d-shader's
// compiler, which rejects valid HLSL); built by build.sh beside it.
//
//   hlsl_check.exe [-D NAME=VALUE]... <profile>:<file>...
//
// Prints one line for each file that fails, with the compiler's messages,
// and a count at the end; exits 1 if any failed.

#include <windows.h>
#include <d3dcompiler.h>

#include <cstdio>
#include <string>
#include <vector>

using CompileFn = HRESULT(WINAPI*)(LPCVOID, SIZE_T, LPCSTR, const D3D_SHADER_MACRO*,
                                   ID3DInclude*, LPCSTR, LPCSTR, UINT, UINT, ID3DBlob**,
                                   ID3DBlob**);

int main(int argc, char** argv) {
  HMODULE library = LoadLibraryA("d3dcompiler_47.dll");
  auto compile = library ? reinterpret_cast<CompileFn>(GetProcAddress(library, "D3DCompile"))
                         : nullptr;
  if (!compile) {
    std::printf("d3dcompiler_47.dll or its D3DCompile not found\n");
    return 2;
  }
  std::vector<std::string> names, values;
  std::vector<std::string> jobs;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-D" && i + 1 < argc) {
      std::string define = argv[++i];
      size_t equals = define.find('=');
      names.push_back(define.substr(0, equals));
      values.push_back(equals == std::string::npos ? "1" : define.substr(equals + 1));
    } else {
      jobs.push_back(arg);
    }
  }
  std::vector<D3D_SHADER_MACRO> macros;
  for (size_t i = 0; i < names.size(); ++i) {
    macros.push_back({names[i].c_str(), values[i].c_str()});
  }
  macros.push_back({nullptr, nullptr});
  int failed = 0;
  for (const std::string& job : jobs) {
    size_t colon = job.find(':');
    std::string profile = job.substr(0, colon), path = job.substr(colon + 1);
    FILE* file = std::fopen(path.c_str(), "rb");
    if (!file) {
      std::printf("%s: can't open\n", path.c_str());
      ++failed;
      continue;
    }
    std::string source;
    char buffer[65536];
    for (size_t n; (n = std::fread(buffer, 1, sizeof(buffer), file)) > 0;) {
      source.append(buffer, n);
    }
    std::fclose(file);
    ID3DBlob* code = nullptr;
    ID3DBlob* errors = nullptr;
    HRESULT result = compile(source.data(), source.size(), path.c_str(), macros.data(), nullptr,
                             "main", profile.c_str(), D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &code,
                             &errors);
    if (FAILED(result)) {
      ++failed;
      std::printf("%s: failed (0x%08lX)\n%s\n", path.c_str(), static_cast<unsigned long>(result),
                  errors ? static_cast<const char*>(errors->GetBufferPointer()) : "");
    }
    if (code) code->Release();
    if (errors) errors->Release();
  }
  std::printf("%zu compiled, %d failed\n", jobs.size() - failed, failed);
  return failed ? 1 : 0;
}
