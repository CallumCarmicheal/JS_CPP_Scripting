#pragma once

#include <string>
#include <fstream>
#include <streambuf>

namespace Files {
    static std::string ReadFile(const char* filePath) {
        //return std::string("var x = 12;");

        std::ifstream t(filePath);
        std::string str((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());
        return str;


        // std::ifstream t(filePath);
        // std::string str;

        // t.seekg(0, std::ios::end);   
        // str.reserve(t.tellg());
        // t.seekg(0, std::ios::beg);

        // str.assign((std::istreambuf_iterator<char>(t)),
        //             std::istreambuf_iterator<char>());

        // return str;
    }
}