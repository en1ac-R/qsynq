#include "sync-manager.hpp"

#include <SQLiteCpp/Database.h>
#include <sys/types.h>

#include "../logic/diff-engine/diff-engine.hpp"

SyncManager::SyncManager(FileScanner& fileScanner, FileRepository& fileRepository)
    : fileScanner_(fileScanner), fileRepository_(fileRepository) {}

void SyncManager::run() {
    auto entries = fileScanner_.scan();
    if (isFirstRun_) {
        fileRepository_.apply(entries);
        isFirstRun_ = false;
        return;
    }

    auto snapshot = fileRepository_.loadSnapshot();

    std::unordered_map<std::filesystem::path, FileEntry> currentState;
    std::unordered_map<std::filesystem::path, FileEntry> lastSate;

    for (const auto& fileEntry : entries) {
        currentState.emplace(fileEntry.path, fileEntry);
    }

    for (const auto& fileEntry : snapshot) {
        lastSate.emplace(fileEntry.path, fileEntry);
    }
    auto diffOps = DiffEngine::diff(lastSate, currentState);

    for (const auto& diffOp : diffOps) {
        switch (diffOp.type) {
            case Create:
            case Modify:
                if (diffOp.newEntry) fileRepository_.upsert(diffOp.newEntry.value());
                break;
            case Delete:
                if (diffOp.oldEntry) fileRepository_.remove(diffOp.oldEntry.value().path);
        }
    }
}
