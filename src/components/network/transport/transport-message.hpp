#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class MessageType : uint8_t { GetFile = 1, FileData = 2, Error = 3 };

struct TransportMessage {
    MessageType type;
    std::string path;

    std::vector<uint8_t> serialize() const;

    static TransportMessage deserialize(const std::vector<uint8_t>& buffer);
};
