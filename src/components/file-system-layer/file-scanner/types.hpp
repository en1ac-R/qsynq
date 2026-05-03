#pragma once

#include <filesystem>

struct FileEntry {
    std::filesystem::path path;
    uint64_t size;
    uint64_t mtime;  // Время последнего изменения в секундах

    explicit FileEntry(std::filesystem::path path, uint64_t size, uint64_t mtime)
        : path(std::move(path)), size(size), mtime(mtime) {}
    FileEntry() = default;
};
