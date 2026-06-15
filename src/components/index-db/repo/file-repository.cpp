#include "file-repository.hpp"

#include <SQLiteCpp/Statement.h>
#include <SQLiteCpp/Transaction.h>

FileRepository::FileRepository(SQLite::Database& db) : db_(db) {}

auto FileRepository::loadSnapshot() -> std::vector<FileEntry> {
    std::vector<FileEntry> result;

    SQLite::Statement query(db_, R"(
           SELECT path, size, mtime
           FROM files
       )");

    while (query.executeStep()) {
        FileEntry entry;

        entry.path = query.getColumn(0).getString();
        entry.size = query.getColumn(1).getInt64();
        entry.mtime = query.getColumn(2).getInt64();

        result.emplace_back(std::move(entry));
    }

    return result;
}

auto FileRepository::upsert(const FileEntry& entry) -> void {
    SQLite::Statement stmt(db_, R"(
        INSERT INTO files(path, size, mtime)
        VALUES (?, ?, ?)
        ON CONFLICT(path) DO UPDATE SET
            size = excluded.size,
            mtime = excluded.mtime
    )");

    stmt.bind(1, entry.path.string());
    stmt.bind(2, static_cast<std::int64_t>(entry.size));
    stmt.bind(3, static_cast<std::int64_t>(entry.mtime));

    stmt.exec();
}

auto FileRepository::remove(const std::filesystem::path& path) -> void {
    SQLite::Statement stmt(db_, R"(
        DELETE FROM files
        WHERE path = ?
    )");

    stmt.bind(1, path.string());

    stmt.exec();
}

auto FileRepository::apply(const std::vector<FileEntry>& entries) -> void {
    SQLite::Transaction transaction(db_);

    for (const auto& entry : entries) {
        upsert(entry);
    }

    transaction.commit();
}
