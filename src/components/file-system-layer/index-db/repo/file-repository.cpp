#include "file-repository.hpp"

FileRepository::FileRepository(ConnectionDB& db)
    : db_(db),
      stmt_(db_.prepare("INSERT OR REPLACE INTO files (path, size, mtime) VALUES (?, ?, ?)")) {}

void FileRepository::upsert(const FileEntry& entry) {
    stmt_.reset();
    stmt_.bind(1, entry.path);
    stmt_.bind(2, entry.size);
    stmt_.bind(3, entry.mtime);
    stmt_.step();
}
