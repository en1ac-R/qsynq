#include "diff-engine.hpp"

#include <vector>

#include "types.hpp"

// Возможно стоит выпилить из FileEntry path??
std::vector<DiffOp> DiffEngine::diff(
    const std::unordered_map<std::filesystem::path, FileEntry>& oldState,
    const std::unordered_map<std::filesystem::path, FileEntry>& newState) {
    std::vector<DiffOp> result;
    for (const auto& [path, entry] : newState) {
        auto oldIt = oldState.find(path);
        if (oldIt == oldState.end()) {
            result.emplace_back(DiffType::Create, path, FileEntry(), entry);
            continue;
        }
        const auto& oldEntry = oldIt->second;
        if (entry != oldEntry) {
            result.emplace_back(DiffType::Modify, path, oldEntry, entry);
        }
    }
    for (const auto& [path, entry] : oldState) {
        if (newState.find(path) == newState.end()) {
            result.emplace_back(DiffType::Delete, path, entry, FileEntry());
        }
    }
    return result;
}
