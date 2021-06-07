#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <atomic>
#include <ctime>
#include <regex>
#include <thread>

namespace fs = std::filesystem;

class FileAnalyzer
{
private:
    std::string dirName;
    std::atomic<int> processedFilesCount  = 0;
    std::atomic<int> totalLines           = 0;
    std::atomic<int> blankLines           = 0;
    std::atomic<int> commentLines         = 0;
    std::atomic<int> codeLines            = 0;

    bool parse(const std::string& filePath);

public:
    FileAnalyzer(std::string dir) : dirName(dir) {}
    bool analyze();
    bool changeDirName(const std::string& newDir);
};

