// The folder the executable is in, as plain C++: where cache/ and saves/
// go.

#pragma once

#include <filesystem>
#include <system_error>

namespace helpers {

// The executable's folder, or an empty path with `error` set.
std::filesystem::path ExecutableFolder(std::error_code& error);

}  // namespace helpers
