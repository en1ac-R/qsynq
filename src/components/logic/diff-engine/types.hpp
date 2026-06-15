#pragma once

#include <filesystem>
#include <optional>

#include "../../common/file-entry.hpp"

enum DiffType { Create, Modify, Delete };

struct DiffOp {
    DiffType type;
    std::filesystem::path path;
    std::optional<FileEntry> oldEntry;
    std::optional<FileEntry> newEntry;
};
