#include <engine/core.h>
#include <fstream>
#include <filesystem>
#include <limits.h>
#include <unistd.h>

std::string getProgramDirectory()
{
#ifdef __linux__
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX); // read executable path from the proc filesystem

    std::string str(result); // parse it as a string

    while (str.back() != '/') // remove each character until we are left with the directory path
        str.pop_back();
    
    return str; // return the processed string
#else
    #error Unsupported platform. Please implement this function first.
#endif
}

std::string Engine::Core::Filesystem::ParseRelativePath(std::string relative)
{
    std::fstream s(relative.c_str()); // open a stream

    if (!s.fail()) // check if the file exists
    {
        s.close(); // close the stream
        return relative; 
    }

    s.close(); // close the stream

    Engine::Core::Logger::LogDebug("Failed to open relative path " + relative);

    std::string newPath = getProgramDirectory() + relative; // create a new path based on the program directory

    Engine::Core::Logger::LogDebug("Trying " + newPath);

    s = std::fstream(newPath); // reopen the stream but with another path

    if (!s.fail()) // check if the file exists
    {
        s.close(); // close the stream
        return newPath;
    }

    s.close(); // close the stream

    Engine::Core::Logger::LogDebug("Giving up");

    return "err0r"; // give up
}