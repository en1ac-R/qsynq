#include "sync-manager.hpp"

#include <SQLiteCpp/Database.h>
#include <sys/types.h>

#include "../index-db/initializer/schema.hpp"
#include "../logic/diff-engine/diff-engine.hpp"

const std::filesystem::path PATH_TO_DB = "/";

SyncManager::SyncManager(FileScanner& fileScanner, FileRepository& fileRepository)
    : fileScanner_(fileScanner), fileRepository_(fileRepository) {}

void SyncManager::run() {
    SQLite::Database db(PATH_TO_DB);
    FileRepository repo(db);

    auto entries = fileScanner_.scan();
    if (isFirstRun_) {
        DatabaseInitializer::init(db);

        repo.apply(entries);
        isFirstRun_ = false;
        return;
    }

    auto snapshot = repo.loadSnapshot();

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
                if (diffOp.newEntry) repo.upsert(diffOp.newEntry.value());
                break;
            case Delete:
                if (diffOp.oldEntry) repo.remove(diffOp.oldEntry.value().path);
        }
    }
}
