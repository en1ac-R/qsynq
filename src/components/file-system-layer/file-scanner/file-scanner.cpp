#include "file-scanner.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <system_error>

FileScanner::FileScanner(const std::filesystem::path& path,
                         const std::unordered_set<std::filesystem::path>& ignoredPaths)
    : rootPath_(path), ignoredPaths_(ignoredPaths) {}

auto FileScanner::scan() -> std::vector<FileEntry> {
    std::vector<FileEntry> entries;
    std::stack<std::filesystem::path> paths;

    paths.push(rootPath_);

    while (!paths.empty()) {
        auto dirPath = paths.top();

        paths.pop();
        scanDirectory(dirPath, paths, entries);
    }

    return entries;
}

auto FileScanner::scanDirectory(const std::filesystem::path& dirPath,
                                std::stack<std::filesystem::path>& dirStack,
                                std::vector<FileEntry>& fileEntries) -> void {
    std::error_code ec;
    std::filesystem::directory_iterator it(dirPath, ec);
    std::filesystem::directory_iterator end;

    if (ec) {
        std::cerr << "scanDirectory error: " << dirPath << ": " << ec.message() << std::endl;
        return;
    }

    for (; it != end; it.increment(ec)) {
        if (ec) {
            std::cerr << "scanDirectory error: " << dirPath << ": " << ec.message() << std::endl;
            break;
        }
        const auto& entry = *it;

        try {
            const auto& path = entry.path();

            if (isIgnored(path)) continue;
            if (entry.is_symlink()) continue;

            if (entry.is_directory())
                dirStack.push(path);
            else if (entry.is_regular_file())
                fileEntries.push_back(scanFile(path));

        } catch (const std::exception& e) {
            std::cerr << "entry error " << entry.path() << ": " << e.what() << std::endl;
            continue;
        }
    }
}

auto FileScanner::scanFile(const std::filesystem::path& filePath) -> FileEntry {
    std::int64_t size = static_cast<std::int64_t>(std::filesystem::file_size(filePath));

    auto ftime = std::filesystem::last_write_time(filePath);

    auto secs = std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch()).count();

    return FileEntry{filePath, size, static_cast<std::int64_t>(secs)};
}

auto FileScanner::isIgnored(const std::filesystem::path& filePath) const -> bool {
    const auto pathName = filePath.filename();
    if (pathName == "." || pathName == "..") return true;

    return ignoredPaths_.count(filePath.lexically_normal()) != 0;
}
