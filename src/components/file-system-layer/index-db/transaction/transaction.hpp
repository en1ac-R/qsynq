#pragma once
#include "../connection/connection.hpp"

class Transaction {
   public:
    explicit Transaction(ConnectionDB& connection);
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    ~Transaction();

    void commit();

   private:
    ConnectionDB& connection_;
    bool isCommited_{false};
};
