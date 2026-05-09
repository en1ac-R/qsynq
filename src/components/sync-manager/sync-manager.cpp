#include "sync-manager.hpp"

#include <sys/types.h>

#include "../logic/diff-engine/diff-engine.hpp"

SyncManager::SyncManager(FileScanner& fileScanner, FileRepository& fileRepository)
    : fileScanner_(fileScanner), fileRepository_(fileRepository) {}

void SyncManager::run() {}
