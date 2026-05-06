#pragma once

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include <vector>

#include "../../file-system-layer/file-scanner/types.hpp"

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

   private:
    SQLite::Database& db_;
};
