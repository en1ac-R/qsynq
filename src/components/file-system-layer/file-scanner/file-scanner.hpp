#include <filesystem>
#include <stack>
#include <unordered_set>
#include <vector>

#include "types.hpp"

// @TODO добавить поддержку IndexDB
class FileScanner {
   public:
    explicit FileScanner(const std::filesystem::path& path,
                         const std::unordered_set<std::filesystem::path>& ignoredPaths = {});

    auto scan() -> std::vector<FileEntry>;

   private:
    auto scanDirectory(const std::filesystem::path& dirPath,
                       std::stack<std::filesystem::path>& dirStack, std::vector<FileEntry>& entries)
        -> void;

    auto scanFile(const std::filesystem::path& filePath) -> FileEntry;
    auto isIgnored(const std::filesystem::path& filePath) const -> bool;

   private:
    std::filesystem::path rootPath_;
    std::unordered_set<std::filesystem::path> ignoredPaths_;
};
