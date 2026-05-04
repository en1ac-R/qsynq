#pragma once

#include <cstdint>
#include <memory>

#include "types.hpp"

class StatementWrapper {
   public:
    explicit StatementWrapper(sqlite3_stmt* stmt);
    StatementWrapper(const StatementWrapper&) = delete;
    auto operator=(const StatementWrapper&) -> StatementWrapper& = delete;
    StatementWrapper(StatementWrapper&&) noexcept = default;
    auto operator=(StatementWrapper&&) noexcept -> StatementWrapper& = default;
    ~StatementWrapper() = default;

    auto step() -> StmtStepResult;
    auto reset() -> void;

    auto bind(int index) -> void;
    auto bind(int index, const std::string& value) -> void;
    auto bind(int index, uint64_t value) -> void;

   private:
    static auto stmtClose(sqlite3_stmt* stmt) noexcept -> void;

   private:
    using UniquePointerStmt = std::unique_ptr<sqlite3_stmt, decltype(&stmtClose)>;

   private:
    UniquePointerStmt stmt_;
};
