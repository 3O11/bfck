#include "Utils.hpp"

namespace BFCK::Utils
{
    std::expected<std::string, FileReadError> ReadFile(const fs::path& filepath)
    {
        std::error_code ec;

        if (!fs::exists(filepath, ec)) {
            return std::unexpected(FileReadError::FileDoesNotExist);
        }

        if (!fs::is_regular_file(filepath, ec)) {
            return std::unexpected(FileReadError::PathIsNotAFile);
        }

        std::ifstream file(filepath);

        if (!file.is_open()) {
            return std::unexpected(FileReadError::CouldNotOpenFile);
        }

        using it = std::istreambuf_iterator<char>;
        std::string content((it(file)), it());
        file.close();
        return content;
    }
}
