#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <csignal>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

constexpr const char* DEFAULT_SOCKET_PATH = "/tmp/qsynqd.sock";
volatile std::sig_atomic_t running = 1;

void handleSignal(int) { running = 0; }

std::string socketPath() {
    if (const char* envPath = std::getenv("QSYNQ_SOCKET")) {
        if (*envPath != '\0') {
            return envPath;
        }
    }

    return DEFAULT_SOCKET_PATH;
}

std::string escapeJson(const std::string& value) {
    std::string escaped;
    escaped.reserve(value.size());

    for (char ch : value) {
        switch (ch) {
            case '"':
                escaped += "\\\"";
                break;
            case '\\':
                escaped += "\\\\";
                break;
            case '\n':
                escaped += "\\n";
                break;
            case '\r':
                escaped += "\\r";
                break;
            case '\t':
                escaped += "\\t";
                break;
            default:
                escaped += ch;
                break;
        }
    }

    return escaped;
}

std::optional<std::string> jsonStringValue(const std::string& json, const std::string& key) {
    const std::string needle = "\"" + key + "\"";
    const auto keyPos = json.find(needle);
    if (keyPos == std::string::npos) {
        return std::nullopt;
    }

    const auto colonPos = json.find(':', keyPos + needle.size());
    if (colonPos == std::string::npos) {
        return std::nullopt;
    }

    const auto firstQuote = json.find('"', colonPos + 1);
    if (firstQuote == std::string::npos) {
        return std::nullopt;
    }

    std::string value;
    bool escaped = false;

    for (auto i = firstQuote + 1; i < json.size(); ++i) {
        const char ch = json[i];

        if (escaped) {
            switch (ch) {
                case 'n':
                    value += '\n';
                    break;
                case 'r':
                    value += '\r';
                    break;
                case 't':
                    value += '\t';
                    break;
                default:
                    value += ch;
                    break;
            }

            escaped = false;
            continue;
        }

        if (ch == '\\') {
            escaped = true;
            continue;
        }

        if (ch == '"') {
            return value;
        }

        value += ch;
    }

    return std::nullopt;
}

std::string okResponse(const std::string& body) { return "{\"ok\":true," + body + "}\n"; }

std::string errorResponse(const std::string& message) {
    return "{\"ok\":false,\"error\":\"" + escapeJson(message) + "\"}\n";
}

std::string makeFoldersJson(const std::set<std::string>& folders) {
    std::ostringstream out;
    out << "\"folders\":[";

    bool first = true;
    for (const auto& folder : folders) {
        if (!first) {
            out << ',';
        }

        out << '"' << escapeJson(folder) << '"';
        first = false;
    }

    out << ']';
    return out.str();
}

std::string handleRequest(const std::string& request, std::set<std::string>& folders,
                          const std::string& boundSocketPath) {
    const auto command = jsonStringValue(request, "command");
    if (!command) {
        return errorResponse("missing command");
    }

    if (*command == "ping") {
        return okResponse("\"message\":\"pong\"");
    }

    if (*command == "status") {
        return okResponse("\"state\":\"running\"," + makeFoldersJson(folders));
    }

    if (*command == "config") {
        return okResponse("\"socket\":\"" + escapeJson(boundSocketPath) + "\"");
    }

    if (*command == "add") {
        const auto path = jsonStringValue(request, "path");
        if (!path || path->empty()) {
            return errorResponse("missing path");
        }

        folders.insert(std::filesystem::absolute(*path).lexically_normal().string());
        return okResponse("\"message\":\"folder added\"," + makeFoldersJson(folders));
    }

    if (*command == "stop") {
        running = 0;
        return okResponse("\"message\":\"stopping\"");
    }

    return errorResponse("unknown command: " + *command);
}

int createServerSocket(const std::string& path) {
    if (path.size() >= sizeof(sockaddr_un::sun_path)) {
        throw std::runtime_error("socket path is too long");
    }

    const int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverFd < 0) {
        throw std::runtime_error(std::string("failed to create unix socket: ") + std::strerror(errno));
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    std::strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

    if (unlink(path.c_str()) < 0 && errno != ENOENT) {
        close(serverFd);
        throw std::runtime_error("failed to remove existing unix socket: " + path + ": " +
                                 std::strerror(errno));
    }

    if (bind(serverFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        const std::string error = std::strerror(errno);
        close(serverFd);
        throw std::runtime_error("failed to bind unix socket: " + path + ": " + error);
    }

    if (listen(serverFd, 16) < 0) {
        const std::string error = std::strerror(errno);
        close(serverFd);
        throw std::runtime_error("failed to listen on unix socket: " + error);
    }

    return serverFd;
}

std::string readRequest(int clientFd) {
    std::string request;
    char buffer[1024]{};

    while (true) {
        const ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break;
        }

        request.append(buffer, static_cast<std::size_t>(bytesRead));
        if (request.find('\n') != std::string::npos) {
            break;
        }
    }

    return request;
}

}  // namespace

int main() {
    const auto path = socketPath();
    std::set<std::string> folders;

    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    try {
        const int serverFd = createServerSocket(path);
        std::cout << "qsynqd listening on " << path << std::endl;

        while (running) {
            const int clientFd = accept(serverFd, nullptr, nullptr);
            if (clientFd < 0) {
                if (running) {
                    std::cerr << "accept failed" << std::endl;
                }
                continue;
            }

            const auto request = readRequest(clientFd);
            const auto response = handleRequest(request, folders, path);
            send(clientFd, response.data(), response.size(), 0);
            close(clientFd);
        }

        close(serverFd);
        unlink(path.c_str());
    } catch (const std::exception& error) {
        std::cerr << "qsynqd: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}
