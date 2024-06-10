#include <cstdint>
#include <expected>
#include <filesystem>
#include <fstream>
#include <print>
#include <string>

namespace fs = std::filesystem;

enum class FileReadError : std::uint8_t
{
    FileDoesNotExist,
    PathIsNotAFile,
    FileIsNotOpen,
};

std::expected<std::string, FileReadError> ReadFile(const fs::path& filepath)
{
    std::error_code ec;

    if (!fs::exists(filepath, ec))
    {
        return std::unexpected(FileReadError::FileDoesNotExist);
    }

    if (!fs::is_regular_file(filepath, ec))
    {
        return std::unexpected(FileReadError::PathIsNotAFile);
    }

    std::ifstream file(filepath);

    if (!file.is_open())
    {
        return std::unexpected(FileReadError::FileIsNotOpen);
    }

    if (file.is_open())
    {
        using it = std::istreambuf_iterator<char>;
        std::string content((it(file)), it());
        file.close();
        return content;
    }
}

int main (int argc, const char *argv[])
{
    if (argc < 2)
    {
        std::println("Usage: bfck <file.bf>");
        return -1;
    }

    std::println("{}", ReadFile(argv[1]).value_or("Error reading file."));

    std::println("Hello World!");
    return 0;
}
