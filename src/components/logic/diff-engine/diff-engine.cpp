#include "diff-engine.hpp"

#include <optional>
#include <vector>

#include "types.hpp"

// Возможно стоит выпилить из FileEntry path??
auto DiffEngine::diff(const std::unordered_map<std::filesystem::path, FileEntry>& oldState,
                      const std::unordered_map<std::filesystem::path, FileEntry>& newState)
    -> std::vector<DiffOp> {
    std::vector<DiffOp> result;

    result.reserve(oldState.size() + newState.size());

    for (const auto& [path, entry] : newState) {
        auto oldIt = oldState.find(path);
        if (oldIt == oldState.end()) {
            result.emplace_back(DiffOp{DiffType::Create, path, std::nullopt, entry});
            continue;
        }
        const auto& oldEntry = oldIt->second;
        if (entry != oldEntry) {
            result.emplace_back(DiffOp{DiffType::Modify, path, oldEntry, entry});
        }
    }
    for (const auto& [path, entry] : oldState) {
        if (newState.find(path) == newState.end()) {
            result.emplace_back(DiffOp{DiffType::Delete, path, entry, std::nullopt});
        }
    }
    return result;
}
