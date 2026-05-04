#pragma once
#include "../connection/connection.hpp"

class Schema {
   public:
    static auto init(ConnectionDB& db) -> void;
};
