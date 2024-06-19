#ifndef BFCK_UTILS_HPP
#define BFCK_UTILS_HPP

#include "Base.hpp"

#include <chrono>
#include <expected>

namespace BFCK::Utils
{
    enum class FileReadError : std::uint8_t
    {
        FileDoesNotExist,
        PathIsNotAFile,
        CouldNotOpenFile,
    };

    std::expected<std::string, FileReadError> ReadFile(const fs::path& filepath);

    class ScopedStopwatch
    {
    public:
        ScopedStopwatch(const std::string& name);
        ~ScopedStopwatch();
    private:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;

        std::string mName;
        TimePoint   mStart;
    };
}

#endif //BFCK_UTILS_HPP
