#include "transaction.hpp"

#include <iostream>

Transaction::Transaction(ConnectionDB& connection) : connection_(connection) {
    connection_.exec("BEGIN TRANSACTION;");
}

void Transaction::commit() {
    if (isCommited_) return;
    connection_.exec("COMMIT");
    isCommited_ = true;
}

Transaction::~Transaction() {
    if (!isCommited_) {
        try {
            connection_.exec("ROLLBACK");
        } catch (...) {
            std::cerr << "Rollback failed: " << std::endl;
        }
    }
}
