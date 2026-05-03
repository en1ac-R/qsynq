#include "schema.hpp"

void Schema::init(ConnectionDB& db) {
    db.exec(R"(
           CREATE TABLE IF NOT EXISTS files (
               path TEXT PRIMARY KEY,
               size INTEGER,
               mtime INTEGER
           );
       )");

    db.exec(R"(
           CREATE INDEX IF NOT EXISTS idx_files_path ON files(path);
       )");
}
