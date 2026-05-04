#include "connection.hpp"

#include <sqlite3.h>

#include <iostream>
#include <stdexcept>

auto ConnectionDB::sqliteClose(sqlite3* db) noexcept -> void {
    if (!db) return;

    if (sqlite3_close(db) != SQLITE_OK) {
        std::cerr << "sqlite close error\n";
    }
}

ConnectionDB::ConnectionDB(const std::filesystem::path& path) : db_(nullptr, sqliteClose) {
    sqlite3* rawDb = nullptr;
    int result = sqlite3_open(path.c_str(), &rawDb);

    if (result != SQLITE_OK) {
        if (rawDb) {
            sqliteClose(rawDb);
        }
        throw std::runtime_error("db open error: " + std::string(sqlite3_errmsg(nullptr)));
    }
    db_.reset(rawDb);
}

auto ConnectionDB::exec(const std::string& sql) -> void {
    char* errMsg = nullptr;
    int result = sqlite3_exec(db_.get(), sql.c_str(), nullptr, nullptr, &errMsg);

    if (result != SQLITE_OK) {
        std::string error = errMsg ? errMsg : sqlite3_errmsg(db_.get());
        sqlite3_free(errMsg);

        throw std::runtime_error("exec error: " + error);
    }

    if (errMsg) {
        sqlite3_free(errMsg);
    }
}

auto ConnectionDB::prepare(const std::string& sql) -> StatementWrapper {
    sqlite3_stmt* stmt = nullptr;

    int result = sqlite3_prepare_v2(db_.get(), sql.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        throw std::runtime_error("prepare error: " + std::string(sqlite3_errmsg(db_.get())));
    }

    return StatementWrapper(stmt);
}
