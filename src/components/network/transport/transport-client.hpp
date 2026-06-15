#pragma once

#include <filesystem>
#include <string>

namespace qsynq {

class TransportClient {
   public:
    bool downloadFile(const std::string& remoteIp, uint16_t port,
                      const std::filesystem::path& remotePath,
                      const std::filesystem::path& localPath);
};

}  // namespace qsynq
