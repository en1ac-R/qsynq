#include "transport-server.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>

TransportServer::TransportServer(uint16_t port) : port_(port) {}

TransportServer::~TransportServer() { stop(); }

void TransportServer::start() {
    running_ = true;

    worker_ = std::thread(&TransportServer::acceptLoop, this);
}

void TransportServer::stop() {
    running_ = false;

    if (serverFd_ >= 0) close(serverFd_);

    if (worker_.joinable()) worker_.join();
}

void TransportServer::acceptLoop() {
    serverFd_ = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    bind(serverFd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    listen(serverFd_, 16);

    while (running_) {
        int client = accept(serverFd_, nullptr, nullptr);

        if (client < 0) continue;

        char request[1024]{};

        ssize_t len = recv(client, request, sizeof(request), 0);

        if (len <= 0) {
            close(client);
            continue;
        }

        std::string req(request, len);

        if (req.starts_with("GET ")) {
            std::string path = req.substr(4);

            std::ifstream file(path, std::ios::binary);

            char buffer[4096];

            while (file) {
                file.read(buffer, sizeof(buffer));

                auto count = file.gcount();

                if (count > 0) {
                    send(client, buffer, count, 0);
                }
            }
        }

        close(client);
    }
}
