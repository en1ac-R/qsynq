#pragma once

#include <unordered_map>
#include <vector>

#include "../../common/file-entry.hpp"
#include "types.hpp"

class DiffEngine {
   public:
    static auto diff(const std::unordered_map<std::filesystem::path, FileEntry>& oldState,
                     const std::unordered_map<std::filesystem::path, FileEntry>& newState)
        -> std::vector<DiffOp>;
    static auto diffToFileEntry(const std::vector<DiffOp>& diffs) -> std::vector<DiffOp>;
};
