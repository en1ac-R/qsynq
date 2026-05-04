#pragma once
#include "../connection/connection.hpp"

class Transaction {
   public:
    explicit Transaction(ConnectionDB& connection);
    Transaction(const Transaction&) = delete;
    auto operator=(const Transaction&) -> Transaction& = delete;
    Transaction(Transaction&&) noexcept = delete;
    auto operator=(Transaction&&) -> Transaction& = delete;
    ~Transaction();

    auto commit() -> void;

   private:
    ConnectionDB& connection_;
    bool isCommited_{false};
};
