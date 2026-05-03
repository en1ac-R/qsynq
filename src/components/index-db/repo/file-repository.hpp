#pragma once

#include "../../file-system-layer/file-scanner/types.hpp"
#include "../connection/connection.hpp"

class FileRepository {
   public:
    explicit FileRepository(ConnectionDB& db);
    FileRepository(const FileRepository&) = delete;
    FileRepository& operator=(const FileRepository&) = delete;

    void upsert(const FileEntry& entry);

   private:
    ConnectionDB& db_;
    StatementWrapper stmt_;
};
