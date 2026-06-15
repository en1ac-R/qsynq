#pragma once

#include <filesystem>
#include <functional>

enum class FileEventType { Created, Modified, Deleted };

struct FileEvent {
    std::filesystem::path path;
    FileEventType type;
};

class FileWatcher {
   public:
    using EventCallback = std::function<void(const FileEvent&)>;

    explicit FileWatcher(const std::filesystem::path& directory);
    ~FileWatcher();

    void start();
    void stop();

    void setCallback(EventCallback callback);

   private:
    void watchLoop();

   private:
    std::filesystem::path directory_;

    int inotifyFd_{-1};
    int watchDescriptor_{-1};

    bool running_{false};

    EventCallback callback_;
};
