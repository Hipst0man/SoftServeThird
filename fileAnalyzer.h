#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <atomic>

namespace fs = std::filesystem;

class fileAnalyzer
{
private:
    std::string dirName;
    std::atomic<int> processedFilesCount  = 0;
    std::atomic<int> totalLines           = 0;
    std::atomic<int> blankLines           = 0;
    std::atomic<int> commentLines         = 0;
    std::atomic<int> codeLines            = 0;

    void parse(const std::string filePath);

public:
    fileAnalyzer(std::string dir) : dirName(dir) {}
    void analyze();
    void changeDirName(const std::string newDir);
};

