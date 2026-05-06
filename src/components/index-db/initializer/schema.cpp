#include "schema.hpp"

auto DatabaseInitializer::init(SQLite::Database& db) -> void {
    db.exec(R"(
           CREATE TABLE IF NOT EXISTS files (
               path TEXT PRIMARY KEY,
               size INTEGER NOT NULL,
               mtime INTEGER NOT NULL
           );
       )");
}
