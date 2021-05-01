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
    inline static void GetTimeAsDirName();
    inline static void SetDirName(std::string name);
    inline static void CreateDirectory();
    inline static void CreateDirectory(int idEvent);
    inline static std::string GetDirectory();
};

inline void DirectoryHelper::SetDirName(std::string name)
{
    fDirectoryPath = std::string("data/") + name + "/";
}

inline void DirectoryHelper::CreateDirectory()
{
    std::experimental::filesystem::create_directories(fDirectoryPath);
}

inline void DirectoryHelper::CreateDirectory(int idEvent)
{
    std::experimental::filesystem::create_directories(fDirectoryPath + std::to_string(idEvent));
}

inline std::string DirectoryHelper::GetDirectory()
{
    return fDirectoryPath;
}
