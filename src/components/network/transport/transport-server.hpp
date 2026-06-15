#pragma once

#include <atomic>
#include <thread>

class TransportServer {
   public:
    explicit TransportServer(uint16_t port);

    ~TransportServer();

    void start();
    void stop();

   private:
    void acceptLoop();

   private:
    uint16_t port_;

    int serverFd_{-1};

    std::atomic_bool running_{false};

    std::thread worker_;
};
