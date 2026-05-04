#pragma once

#include <filesystem>
#include <memory>

#include "statement-wrapper.hpp"

class ConnectionDB {
   public:
    explicit ConnectionDB(const std::filesystem::path& path);
    ConnectionDB(const ConnectionDB&) = delete;
    auto operator=(const ConnectionDB&) -> ConnectionDB& = delete;
    ConnectionDB(ConnectionDB&&) noexcept = default;
    auto operator=(ConnectionDB&&) -> ConnectionDB& = default;
    ~ConnectionDB() = default;

    // Используется для редактирования schema
    auto exec(const std::string& sql) -> void;
    // Используется для работы с пользовательскими данными
    auto prepare(const std::string& sql) -> StatementWrapper;

   private:
    static auto sqliteClose(sqlite3* db) noexcept -> void;

   private:
    using UniquePointerDb = std::unique_ptr<sqlite3, decltype(&sqliteClose)>;

   private:
    UniquePointerDb db_;
};
