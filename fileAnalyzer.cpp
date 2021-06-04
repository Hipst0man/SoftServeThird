#include "fileAnalyzer.h"


void fileAnalyzer::parse(const std::string filePath)
{
    std::ifstream fin(filePath, std::ios::binary);
    if(!fin) return;

    std::regex comment {R"(\/\/)"};
    std::regex commentBlockEnd {R"((\*\/))"};
    std::regex commentBlockBegin {R"((\/\*))"};
    std::regex codeBeforeCommentLine {R"(((\w|\s|[;,.=()<>"]|\]|\[)+//(\w|\s|[;,.=()<>"]|\]|\[)*))"};
    std::regex codeBeforeBlockComment {R"(((\w|\s|[;,.=()<>"]|\]|\[)+(\/\*)(\w|\s|[;,.=()<>"]|\]|\[)*))"};
    std::regex codeAfterBlockComment {R"(((\w|\s|[;,.=()<>"]|\]|\[)*(\*\/)(\w|\s|[;,.=()<>"]|\]|\[)+))"};
    std::regex commentsInQuote {R"((")(\w|\s|[/]|[(\/\*)])*(")(\w|\s|[;,.=()<>]|\]|\[)*)"};
    std::regex blankLine {R"(^$|^(\s)*$)"};

    bool isBlockComment = false;
    std::string text;
    
    while(getline(fin, text))
    {
        if(isBlockComment)
        {
            if(std::regex_search(text, commentBlockEnd))
            {
                commentLines++;
                if(std::regex_search(text, codeAfterBlockComment)) 
                {
                    codeLines++;
                }
                isBlockComment = false;
            }
            else
            {
                commentLines++;
            }
            totalLines++;
            continue;
        }

        if(std::regex_match(text, blankLine)) blankLines++;

        else if(std::regex_search(text, comment))
        {
            if(!std::regex_search(text, commentsInQuote))
            {
                commentLines++;
            }

            else if(std::regex_search(text,codeBeforeCommentLine))
            {
                codeLines++;
            }
        }

        else if(std::regex_search(text, commentBlockBegin))
        {
            if(!std::regex_match(text, commentsInQuote)) commentLines++;

            if(std::regex_search(text, codeBeforeBlockComment))
            {
                codeLines++;
            }

            if(!std::regex_search(text, commentBlockEnd))
            {
                isBlockComment = true;
            }
        }

        else
        {
            codeLines++;
        }

        totalLines++;
    }
    
    fin.close();
}


void fileAnalyzer::analyze()
{
    std::vector<std::string> files;
    std::vector<std::thread> threads;
    const std::string cPlusHeader = ".h";
    const std::string cHeader = ".hpp"; 
    const std::string cPlusSource = ".cpp"; 
    const std::string cSource = ".c"; 

    for (const auto& p : fs::recursive_directory_iterator(dirName))
    {
        if(fs::is_regular_file(p))
        {
            if(p.path().extension() == cPlusHeader || p.path().extension() == cHeader || p.path().extension() == cPlusSource || p.path().extension() == cSource)
            {
                files.emplace_back(p.path());
            }
        }
    }

    const unsigned int startTime =  clock();

    for(const auto& el : files)
    {
        threads.emplace_back(std::thread(&fileAnalyzer::parse, this, el));
        processedFilesCount++;
    }

    for(auto& th : threads)
    {
        if(th.joinable())
        {
            th.join();
        }
    }

    const unsigned int endTime = clock();
    unsigned int parseTime = endTime - startTime;

    std::ofstream fout("output.txt");
    
    fout << "Total lines: " << totalLines.load() << "\nComment Lines: " << commentLines.load() 
                << "\nCode lines: " << codeLines.load() << "\nBlank lines: " << blankLines.load() 
                    << "\nTotal files: " << processedFilesCount.load() << "\nTime spent on parsing files: " << parseTime / 1000.0 << "\n";
    fout.close();
}

void fileAnalyzer::changeDirName(const std::string newDir)
{
    if(newDir.empty()) return;
    else 
    {
        dirName = newDir;
    }   
}


