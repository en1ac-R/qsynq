#include "sync-manager.hpp"

#include <sys/types.h>

#include "../logic/diff-engine/diff-engine.hpp"

SyncManager::SyncManager(FileScanner& fileScanner, FileRepository& fileRepository)
    : fileScanner_(fileScanner), fileRepository_(fileRepository) {}

void SyncManager::run() {
    auto oldState = fileRepository_.getSnapshot();

    // @todo Возможно изначально после сканирования стоит получать unordered_map
    std::unordered_map<std::filesystem::path, FileEntry> newState;
    for (const auto& entry : fileScanner_.scan()) {
        newState.emplace(entry.path, entry);
    }

    auto diffs = DiffEngine::diff(oldState, newState);

    if (diffs.empty()) return;

    fileRepository_.apply(diffs);
}
