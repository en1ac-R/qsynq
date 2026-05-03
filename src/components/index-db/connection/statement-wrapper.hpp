#pragma once

#include <cstdint>
#include <memory>

#include "types.hpp"

class StatementWrapper {
   public:
    explicit StatementWrapper(sqlite3_stmt* stmt);
    StatementWrapper(const StatementWrapper&) = delete;
    StatementWrapper& operator=(const StatementWrapper&) = delete;

    StmtStepResult step();
    void reset();

    void bind(int index);
    void bind(int index, const std::string& value);
    void bind(int index, uint64_t value);

   private:
    static void stmtClose(sqlite3_stmt* stmt) noexcept;

   private:
    using UniquePointerStmt = std::unique_ptr<sqlite3_stmt, decltype(&stmtClose)>;

   private:
    UniquePointerStmt stmt_;
};
