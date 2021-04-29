#pragma once

#include <ctime>
#include <fstream>
#include <experimental/filesystem>
#include <string>
class DirectoryHelper
{
private:
    inline static std::string fDirectoryPath;
public:
    static void Initialize();
    inline static void CreateDirectory(std::string name);
    inline static std::string GetDirectory();
};

inline void DirectoryHelper::CreateDirectory(std::string name)
{
    std::experimental::filesystem::create_directories(name);
}

inline std::string DirectoryHelper::GetDirectory()
{
    return fDirectoryPath;
}
