#pragma once

#include <filesystem>

#include "../../file-system-layer/file-scanner/types.hpp"

enum DiffType { Create, Modify, Delete };

struct DiffOp {
    DiffType type;
    std::filesystem::path path;
    FileEntry old_entry;
    FileEntry new_entry;

    DiffOp(DiffType type, std::filesystem::path path, FileEntry old_entry, FileEntry new_entry)
        : type(type),
          path(std::move(path)),
          old_entry(std::move(old_entry)),
          new_entry(std::move(new_entry)) {}

    DiffOp() = default;
};
