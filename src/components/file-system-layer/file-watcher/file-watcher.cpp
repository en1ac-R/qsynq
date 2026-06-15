#include "file-watcher.hpp"

#include <sys/inotify.h>
#include <unistd.h>

#include <array>
#include <cstring>

namespace {

constexpr uint32_t WATCH_MASK = IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM;

}

FileWatcher::FileWatcher(const std::filesystem::path& directory) : directory_(directory) {}

FileWatcher::~FileWatcher() { stop(); }

void FileWatcher::setCallback(EventCallback callback) { callback_ = std::move(callback); }

void FileWatcher::start() {
    if (running_) {
        return;
    }

    inotifyFd_ = inotify_init1(0);

    if (inotifyFd_ < 0) {
        throw std::runtime_error("Failed to initialize inotify");
    }

    watchDescriptor_ = inotify_add_watch(inotifyFd_, directory_.c_str(), WATCH_MASK);

    if (watchDescriptor_ < 0) {
        close(inotifyFd_);

        throw std::runtime_error("Failed to add inotify watch");
    }

    running_ = true;

    watchLoop();
}

void FileWatcher::stop() {
    if (!running_) {
        return;
    }

    running_ = false;

    if (watchDescriptor_ >= 0) {
        inotify_rm_watch(inotifyFd_, watchDescriptor_);
    }

    if (inotifyFd_ >= 0) {
        close(inotifyFd_);
    }

    watchDescriptor_ = -1;
    inotifyFd_ = -1;
}

void FileWatcher::watchLoop() {
    constexpr std::size_t BUFFER_SIZE = 1024 * (sizeof(inotify_event) + 64);

    std::array<char, BUFFER_SIZE> buffer{};

    while (running_) {
        ssize_t bytesRead = read(inotifyFd_, buffer.data(), buffer.size());

        if (bytesRead <= 0) {
            continue;
        }

        std::size_t offset = 0;

        while (offset < static_cast<std::size_t>(bytesRead)) {
            auto* event = reinterpret_cast<inotify_event*>(buffer.data() + offset);

            FileEvent fileEvent;

            fileEvent.path = directory_ / std::string(event->name);

            if (event->mask & IN_CREATE || event->mask & IN_MOVED_TO) {
                fileEvent.type = FileEventType::Created;
            } else if (event->mask & IN_MODIFY) {
                fileEvent.type = FileEventType::Modified;
            } else if (event->mask & IN_DELETE || event->mask & IN_MOVED_FROM) {
                fileEvent.type = FileEventType::Deleted;
            } else {
                offset += sizeof(inotify_event) + event->len;

                continue;
            }

            if (callback_) {
                callback_(fileEvent);
            }

            offset += sizeof(inotify_event) + event->len;
        }
    }
}
