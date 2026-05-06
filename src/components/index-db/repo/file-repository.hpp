#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "../../file-system-layer/file-scanner/types.hpp"
#include "../connection/connection.hpp"

class FileRepository {
   public:
    explicit FileRepository(ConnectionDB& db);
    FileRepository(const FileRepository&) = delete;
    auto operator=(const FileRepository&) -> FileRepository& = delete;

    auto upsert(const FileEntry& entry) -> void;

   private:
    ConnectionDB& db_;
    StatementWrapper stmt_;
};
