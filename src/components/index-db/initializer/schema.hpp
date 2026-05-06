#pragma once
#include <SQLiteCpp/SQLiteCpp.h>

class DatabaseInitializer {
   public:
    static auto init(SQLite::Database& db) -> void;
};
