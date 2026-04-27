#pragma once

#include <filesystem>
#include <memory>

#include "statement-wrapper.hpp"

class ConnectionDB {
   public:
    explicit ConnectionDB(const std::filesystem::path& path);
    ConnectionDB(const ConnectionDB&) = delete;
    ConnectionDB& operator=(const ConnectionDB&) = delete;

    // Используется для редактирования schema
    void exec(const std::string& sql);
    // Используется для работы с пользовательскими данными
    StatementWrapper prepare(const std::string& sql);

   private:
    static void sqliteClose(sqlite3* db) noexcept;

   private:
    using UniquePointerDb = std::unique_ptr<sqlite3, decltype(&sqliteClose)>;

   private:
    UniquePointerDb db_;
};
