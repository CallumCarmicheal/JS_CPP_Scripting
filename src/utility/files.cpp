#include "files.h"

#include <string>
#include <stdio.h>
#include <fstream>
#include <streambuf>

#ifdef __WINDOWS__
	#include <windows.h>
	#include <stdlib.h>
	#include <shlwapi.h>
	#include <atlstr.h>

	#define FILE_SEP "\\"
#endif

#ifdef _WIN32 
   #include <io.h> 
   #define access    _access_s
#else
   #include <unistd.h>
#endif
#ifdef __WINDOWS__
bool Files::GetBasePath(char* szFilePath) {
    return PathRemoveFileSpec(szFilePath);
}

std::string Files::ResolvePath(const char* filePath) {
    DWORD  retval=0;
    BOOL   success; 
    TCHAR  buffer[4096]=TEXT(""); 
    TCHAR  buf[4096]=TEXT(""); 
    TCHAR** lppPart={NULL};

    retval = GetFullPathName(
        filePath,
        4096,
        buffer,
        lppPart);
    
    if (retval == 0) {
        // Handle an error condition.
        //printf ("GetFullPathName failed (%d)\n", GetLastError());
        return {};
    } else {
        // _tprintf(TEXT("The full path name is:  %s\n"), buffer);
        // if (lppPart != NULL && *lppPart != 0) {
        //     _tprintf(TEXT("The final component in the path name is:  %s\n"), *lppPart);
        // }

        return std::string(buffer);
    }
}
#endif    

bool Files::Exists(const char* filePath) {
    return access( filePath, 0 ) == 0;
}

std::string Files::GetActiveExecutablePath() {
    char result[ MAX_PATH ];
    return std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
}

std::string Files::Read(const char* filePath) {
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