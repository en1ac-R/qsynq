#include "statement-wrapper.hpp"

#include <iostream>
#include <stdexcept>

auto StatementWrapper::stmtClose(sqlite3_stmt* stmt) noexcept -> void {
    if (sqlite3_finalize(stmt) != SQLITE_OK) {
        std::cerr << "stmt close error: " << sqlite3_errmsg(sqlite3_db_handle(stmt)) << std::endl;
    }
}

StatementWrapper::StatementWrapper(sqlite3_stmt* stmt) : stmt_(stmt, stmtClose) {}

auto StatementWrapper::step() -> StmtStepResult {
    int result = sqlite3_step(stmt_.get());
    switch (result) {
        case SQLITE_ROW:
            return ROW;
        case SQLITE_DONE:
            return DONE;
        default:
            throw std::runtime_error("stmt step error: " +
                                     std::string(sqlite3_errmsg(sqlite3_db_handle(stmt_.get()))));
    }
}

auto StatementWrapper::reset() -> void {
    if (sqlite3_reset(stmt_.get()) != SQLITE_OK) {
        throw std::runtime_error("stmt reset error: " +
                                 std::string(sqlite3_errmsg(sqlite3_db_handle(stmt_.get()))));
    }
    if (sqlite3_clear_bindings(stmt_.get()) != SQLITE_OK) {
        throw std::runtime_error("stmt clear bindings error: " +
                                 std::string(sqlite3_errmsg(sqlite3_db_handle(stmt_.get()))));
    }
}

auto StatementWrapper::bind(int index) -> void {
    if (index < 0) {
        throw std::runtime_error("stmt bind index error: index must be non-negative");
    }
    if (sqlite3_bind_null(stmt_.get(), index) != SQLITE_OK) {
        throw std::runtime_error("stmt bind null error: " +
                                 std::string(sqlite3_errmsg(sqlite3_db_handle(stmt_.get()))));
    }
}

auto StatementWrapper::bind(int index, const std::string& value) -> void {
    if (index < 0) {
        throw std::runtime_error("stmt bind index error: index must be non-negative");
    }

    // SQLITE_TRANSIENT: строка полностью копируется в statement
    if (sqlite3_bind_text(stmt_.get(), index, value.c_str(), value.size(), SQLITE_TRANSIENT) !=
        SQLITE_OK) {
        throw std::runtime_error("stmt bind text error: " +
                                 std::string(sqlite3_errmsg(sqlite3_db_handle(stmt_.get()))));
    }
}

auto StatementWrapper::bind(int index, uint64_t value) -> void {
    if (index < 0) {
        throw std::runtime_error("stmt bind index error: index must be non-negative");
    }
    if (sqlite3_bind_int64(stmt_.get(), index, value) != SQLITE_OK) {
        throw std::runtime_error("stmt bind int64 error: " +
                                 std::string(sqlite3_errmsg(sqlite3_db_handle(stmt_.get()))));
    }
}
