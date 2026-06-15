#include "transport-message.hpp"

#include <stdint.h>

#include <cstring>
#include <stdexcept>
#include <vector>

std::vector<uint8_t> TransportMessage::serialize() const {
    std::vector<uint8_t> buffer;

    buffer.push_back(static_cast<uint8_t>(type));

    uint32_t pathLength = static_cast<uint32_t>(path.size());

    auto* lengthBytes = reinterpret_cast<uint8_t*>(&pathLength);

    buffer.insert(buffer.end(), lengthBytes, lengthBytes + sizeof(pathLength));

    buffer.insert(buffer.end(), path.begin(), path.end());

    return buffer;
}

TransportMessage TransportMessage::deserialize(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < sizeof(uint8_t) + sizeof(uint32_t)) {
        throw std::runtime_error("Invalid transport message");
    }

    TransportMessage message;

    message.type = static_cast<MessageType>(buffer[0]);

    uint32_t pathLength;

    std::memcpy(&pathLength, buffer.data() + sizeof(uint8_t), sizeof(uint32_t));

    const std::size_t expectedSize = sizeof(uint8_t) + sizeof(uint32_t) + pathLength;

    if (buffer.size() < expectedSize) {
        throw std::runtime_error("Corrupted transport message");
    }

    message.path.assign(
        reinterpret_cast<const char*>(buffer.data() + sizeof(uint8_t) + sizeof(uint32_t)),
        pathLength);

    return message;
}
