#include "transport-client.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>

namespace qsynq {

bool TransportClient::downloadFile(const std::string& remoteIp, uint16_t port,
                                   const std::filesystem::path& remotePath,
                                   const std::filesystem::path& localPath) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) return false;

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    inet_pton(AF_INET, remoteIp.c_str(), &addr.sin_addr);

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(sock);
        return false;
    }

    std::string request = "GET " + remotePath.string();

    send(sock, request.data(), request.size(), 0);

    std::ofstream out(localPath, std::ios::binary);

    char buffer[4096];

    ssize_t bytes;

    while ((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        out.write(buffer, bytes);
    }

    close(sock);

    return true;
}

}  // namespace qsynq
