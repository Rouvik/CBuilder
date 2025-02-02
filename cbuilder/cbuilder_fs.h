/*
MIT License

Copyright (c) 2024 Rouvik Maji

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef INCLUDED_CBUILDER_FS
#define INCLUDED_CBUILDER_FS

#include <stdio.h>
#include <stdint.h>

#include "cbuilder_string.h"

#define CBUILD_FS_DIRMODE_FILES 1
#define CBUILD_FS_DIRMODE_FOLDERS 1 << 1

CBuild_String CBuild_Fs_dir(const char *path, const char *mask, uint8_t mode, const char *delim);

#ifdef _WIN32 // systems with win api

#include <windows.h>

CBuild_String CBuild_Fs_dir(const char *path, const char *mask, uint8_t mode, const char *delim)
{
    WIN32_FIND_DATAA fdFile;
    HANDLE hFind = NULL;

    if (mask[0] == '\0') // make sure mask is not error type
    {
        mask = "*.*";
    }

    int pathLen = strlen(path);
    int maskLen = strlen(mask);

    char cpyPath[pathLen + maskLen + 1];
    memcpy(cpyPath, path, pathLen);
    memcpy(cpyPath + pathLen, mask, maskLen + 1);

    CBuild_String output = CBuild_String_init("");

    if ((hFind = FindFirstFileA(cpyPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "[CBuilder FS Error] Failed to open path %s\n", cpyPath);
        CBuild_String_deinit(&output);
        return (CBuild_String){NULL, 0, 0};
    }

    do
    {
        if ((fdFile.cFileName[0] == '.') && (fdFile.cFileName[1] == '\0' || fdFile.cFileName[1] == '.'))
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

        CBuild_String_concatCStr(&output, (char *)delim); // add the ending delimeter
    } while (FindNextFileA(hFind, &fdFile));

    FindClose(hFind); // cleanup

    return output;
}

#elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__) // systems supporting dirent.h

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
CBuild_String CBuild_Fs_dir(const char *path, const char *mask, uint8_t mode, const char *delim)
{
    if (mask[0] == '/') // if first character is / then ignore
    {
        mask++;
    }

    struct dirent *dirEntry;
    DIR *dir;
    struct stat fileStatus;

    dir = opendir(path);
    if (!dir)
    {
        fprintf(stderr, "[CBuilder FS Error] %s\n", strerror(errno));
        return (CBuild_String){NULL, 0, 0}; // return nothing
    }

    CBuild_String output = CBuild_String_init("");

    while ((dirEntry = readdir(dir)) != NULL)
    {
        // ignore . and .. entries
        if ((dirEntry->d_name[0] == '.') && (dirEntry->d_name[1] == '\0' || dirEntry->d_name[1] == '.'))
        {
            continue;
        }

        char *dotPos = strchr(mask, '.') + 1;
        if (!dotPos)
        {
            dotPos = (char *)mask + strlen(mask);
        }

        char *fileExt = strrchr(dirEntry->d_name, '.') + 1;
        int fileNameLen = fileExt - dirEntry->d_name - 1;
        if (fileExt == NULL + 1)
        {
            int dirLen = strlen(dirEntry->d_name);
            fileExt = dirEntry->d_name + dirLen;
            fileNameLen = dirLen;
        }
        
        char *fileName;
        if (fileExt == dirEntry->d_name + 1)
        {
            fileName = "";
        }
        else
        {
            fileName = (char *)alloca(fileNameLen + 1);
            memcpy(fileName, dirEntry->d_name, fileNameLen);
            fileName[fileNameLen] = '\0';
        }
        
        int maskLen = strlen(mask);
        if (mask[0] != '*' && strncmp(mask, fileName, fileNameLen - 1))
        {
            continue;
        }

        if (dotPos[0] != '*' && strcmp(dotPos, fileExt))
        {
            continue;
        }

        stat(dirEntry->d_name, &fileStatus);
        if ((mode & CBUILD_FS_DIRMODE_FOLDERS) && (fileStatus.st_mode & S_IFDIR))
        {
            CBuild_String_concatCStr(&output, dirEntry->d_name);
        }
        else if ((mode & CBUILD_FS_DIRMODE_FILES) && !(fileStatus.st_mode & S_IFDIR))
        {
            CBuild_String_concatCStr(&output, dirEntry->d_name);
        }

        CBuild_String_concatCStr(&output, (char *)delim);
    }

    closedir(dir); // cleanup

    return output;
}

#else // unsupported system
#error "[CBuilder FS] Unsupported system, if you know about your system, feel free to define _WIN32 for windows __linux__ or __APPLE__ or __MACH__ for linux and mac systems"
#endif

#endif // INCLUDED_CBUILDER_FS