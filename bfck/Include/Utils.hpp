#ifndef BFCK_UTILS_HPP
#define BFCK_UTILS_HPP

#include "Base.hpp"

namespace BFCK::Utils
{
    enum class FileReadError : std::uint8_t
    {
        FileDoesNotExist,
        PathIsNotAFile,
        CouldNotOpenFile,
    };

    std::expected<std::string, FileReadError> ReadFile(const fs::path& filepath);
}

#endif //BFCK_UTILS_HPP
