#include <optional>

#include "../file-system-layer/file-scanner/file-scanner.hpp"
#include "../index-db/repo/file-repository.hpp"
#include "../logic/diff-engine/types.hpp"

class SyncManager {
   public:
    SyncManager(FileScanner& fileScanner, FileRepository& repo);

    auto run() -> void;

   private:
    FileScanner& fileScanner_;
    FileRepository& fileRepository_;
    bool isFirstRun_ = true;
};
