#include "Utils.hpp"

#include <fstream>
#include <print>

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

    ScopedStopwatch::ScopedStopwatch(const std::string& name)
        : mName(name), mStart(Clock::now())
    {}

    ScopedStopwatch::~ScopedStopwatch()
    {
        auto end = Clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - mStart);
        std::println("{} took {}ms", mName, duration.count());
    }
}
