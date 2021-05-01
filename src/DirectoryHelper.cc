#include "DirectoryHelper.hh"

void DirectoryHelper::GetTimeAsDirName()
{
    using std::to_string;
    std::time_t time_v = std::time(0);
    std::tm* time_now = std::localtime(&time_v);
    std::string time_str = to_string(time_now->tm_year - 100);
    time_str += (time_now->tm_mon < 10 ? "0" + to_string(time_now->tm_mon+1) : to_string(time_now->tm_mon+1));
    time_str += (time_now->tm_mday < 10 ? "0" + to_string(time_now->tm_mday) : to_string(time_now->tm_mday));
    time_str += (time_now->tm_hour < 10 ? "0" + to_string(time_now->tm_hour) : to_string(time_now->tm_hour));
    time_str += (time_now->tm_min < 10 ? "0" + to_string(time_now->tm_min) : to_string(time_now->tm_min));
    fDirectoryPath = std::string("data/") + time_str + "/";
}
