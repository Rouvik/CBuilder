#ifndef INCLUDED_CBUILDER_FS
#define INCLUDED_CBUILDER_FS

#include <stdio.h>
#include <stdint.h>

#include "cbuilder_string.h"

#define CBUILD_FS_DIRMODE_FILES 1
#define CBUILD_FS_DIRMODE_FOLDERS 1 << 1

CBuild_String CBuild_Fs_dir(const char *path, uint8_t mode);

#ifdef _WIN32 // systems with win api

#include <windows.h>

CBuild_String CBuild_Fs_dir(const char *path, uint8_t mode)
{
    WIN32_FIND_DATAA fdFile;
    HANDLE hFind = NULL;

    int len = strlen(path);
    char cpyPath[len + 3 + (path[len - 1] == '\\' || path[len - 1] == '/' ? 0 : 1)];
    memcpy(cpyPath, path, len + 1);

    if (path[len - 1] == '\\' || path[len - 1] == '/')
    {
        strcat(cpyPath, "*.*");
    }
    else
    {
        strcat(cpyPath, "/*.*");
    }

    CBuild_String output = CBuild_String_init("");

    if ((hFind = FindFirstFileA(cpyPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "[CBuilder FS Error] Failed to open path %s\n", path);
        CBuild_String_deinit(&output);
        return (CBuild_String){NULL, 0, 0};
    }

    do
    {
        if (strcmp(fdFile.cFileName, ".") == 0 || strcmp(fdFile.cFileName, "..") == 0) // ignore this 2 stupid cases
        {
            continue;
        }
        
        if ((mode & CBUILD_FS_DIRMODE_FOLDERS) && (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            CBuild_String_concatCStr(&output, fdFile.cFileName);
        }
        else if (mode & CBUILD_FS_DIRMODE_FILES && !(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            CBuild_String_concatCStr(&output, fdFile.cFileName);
        }

        CBuild_String_concatCStr(&output, "|"); // add the ending delimeter
    } while (FindNextFileA(hFind, &fdFile));
    
    FindClose(hFind); // cleanup

    return output;
}

#elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__) // systems supporting dirent.h

#error "Linux and MacOS not implemented yet..."

#else // unsupported system
#error "[CBuilder FS] Unsupported system, if you know about your system, feel free to define _WIN32 for windows __linux__ or __APPLE__ or __MACH__ for linux and mac systems"
#endif

#endif // INCLUDED_CBUILDER_FS