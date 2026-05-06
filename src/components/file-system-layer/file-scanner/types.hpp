#pragma once

#include <filesystem>

struct FileEntry {
    std::filesystem::path path;
    uint64_t size{0};
    uint64_t mtime{0};  // Время последнего изменения в секундах

    explicit FileEntry(std::filesystem::path path, uint64_t size, uint64_t mtime)
        : path(std::move(path)), size(size), mtime(mtime) {}
    FileEntry() = default;

    bool operator==(const FileEntry& other) const {
        return path == other.path && size == other.size && mtime == other.mtime;
    }
    bool operator!=(const FileEntry& other) const { return !(*this == other); }

    bool isEmpty() const { return path.empty() && size == 0 && mtime == 0; }
};
