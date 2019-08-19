#pragma once

#include <string>

namespace Files {
    std::string GetActiveExecutablePath();
    std::string Read(const char* filePath);
    std::string ResolvePath(const char* filePath);

    bool        GetBasePath(char* szFilePath);
    bool        Exists(const char* filePath);
}