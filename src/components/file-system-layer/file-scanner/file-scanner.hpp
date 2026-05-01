#include <filesystem>
#include <stack>
#include <unordered_set>
#include <vector>

struct FileEntry {
    std::filesystem::path path;
    uint64_t size;
    uint64_t mtime;  // Время последнего изменения в секундах

    explicit FileEntry(std::filesystem::path path, uint64_t size, uint64_t mtime)
        : path(std::move(path)), size(size), mtime(mtime) {}
    FileEntry() = default;
};

// @TODO добавить поддержку IndexDB
class FileScanner {
   public:
    explicit FileScanner(const std::filesystem::path& path,
                         const std::unordered_set<std::filesystem::path>& ignoredPaths = {});
    FileScanner(FileScanner&&) noexcept = default;
    FileScanner& operator=(FileScanner&&) noexcept = default;

    std::vector<FileEntry> scan();

   private:
    void scanDirectory(const std::filesystem::path& dirPath,
                       std::stack<std::filesystem::path>& dirStack,
                       std::vector<FileEntry>& entries);
    FileEntry scanFile(const std::filesystem::path& filePath);
    bool isIgnored(const std::filesystem::path& filePath) const;

   private:
    std::filesystem::path rootPath_;
    std::unordered_set<std::filesystem::path> ignoredPaths_;
};
