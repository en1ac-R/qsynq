#pragma once

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include <vector>

#include "../../file-system-layer/file-scanner/types.hpp"
#include "../../logic/diff-engine/types.hpp"

class FileRepository {
   public:
    explicit FileRepository(SQLite::Database& db);
    FileRepository(const FileRepository&) = delete;
    FileRepository(FileRepository&&) = delete;
    auto operator=(const FileRepository&) -> FileRepository& = delete;
    auto operator=(FileRepository&&) -> FileRepository& = delete;
    ~FileRepository() = default;

    auto loadSnapshot() -> std::vector<FileEntry>;
    auto upsert(const FileEntry& entry) -> void;
    auto remove(const std::filesystem::path& path) -> void;

    // Должен ли он знать вообще про DiffOp??
    auto apply(const std::vector<DiffOp>& ops) -> void;

   private:
    SQLite::Database& db_;
};
