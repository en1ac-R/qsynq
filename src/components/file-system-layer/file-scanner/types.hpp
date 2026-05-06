#pragma once

#include <filesystem>

struct FileEntry {
    std::filesystem::path path;
    std::int64_t size{0};
    std::int64_t mtime{0};  // Время последнего изменения в секундах

    explicit FileEntry(const std::filesystem::path& path, std::int64_t size, std::int64_t mtime)
        : path(path), size(size), mtime(mtime) {}
    FileEntry() = default;

    bool operator==(const FileEntry& other) const {
        return path == other.path && size == other.size && mtime == other.mtime;
    }
    bool operator!=(const FileEntry& other) const { return !(*this == other); }

    bool isEmpty() const { return path.empty() && size == 0 && mtime == 0; }
};
