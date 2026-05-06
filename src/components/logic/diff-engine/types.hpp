#pragma once

#include <filesystem>
#include <optional>

#include "../../file-system-layer/file-scanner/types.hpp"

enum DiffType { Create, Modify, Delete };

struct DiffOp {
    DiffType type;
    std::filesystem::path path;
    std::optional<FileEntry> oldEntry;
    std::optional<FileEntry> newEntry;
};
