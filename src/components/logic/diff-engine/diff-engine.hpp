#pragma once

#include <unordered_map>
#include <vector>

#include "../../file-system-layer/file-scanner/types.hpp"
#include "types.hpp"

class DiffEngine {
   public:
    static auto diff(const std::unordered_map<std::filesystem::path, FileEntry>& oldState,
                     const std::unordered_map<std::filesystem::path, FileEntry>& newState)
        -> std::vector<DiffOp>;
};
