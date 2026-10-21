#include "executable.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <string>
#endif

namespace helpers {

std::filesystem::path ExecutableFolder(std::error_code& error) {
  error.clear();
#ifdef _WIN32
  std::wstring name(MAX_PATH, L'\0');
  DWORD length;
  while ((length = GetModuleFileNameW(nullptr, name.data(), DWORD(name.size()))) ==
         name.size()) {
    name.resize(name.size() * 2);
  }
  if (length == 0) {
    error = std::error_code(int(GetLastError()), std::system_category());
    return {};
  }
  std::filesystem::path exe(name.substr(0, length));
#else
  std::filesystem::path exe = std::filesystem::read_symlink("/proc/self/exe", error);
  if (error) {
    return {};
  }
#endif
  return exe.parent_path();
}

}  // namespace helpers
