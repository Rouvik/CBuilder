#include <stdio.h>

#include "cbuilder/cbuilder.h"

int main()
{
    CBuild_String buildDir = CBuild_String_init("./sample/build/");

    CBuild_String dir = CBuild_Fs_dir("./sample/code", "/*.*", CBUILD_FS_DIRMODE_FOLDERS, ":");

    CBuild_String depFolder = {NULL, 0, 0};
    CBuild_String_tokenizer(&dir, &depFolder, ":");
    while (depFolder.str[0] != '\0')
    {
        CBuild_String srcPath = CBuild_String_init("./sample/code/");
        CBuild_String_concatN(&srcPath, &depFolder, depFolder.len);
        CBuild_String_concatCStr(&srcPath, "/");
        CBuild_String_concatN(&srcPath, &depFolder, depFolder.len);

        CBuild_String outPath = CBuild_String_copy(&buildDir);
        CBuild_String_concatN(&outPath, &depFolder, depFolder.len);
        CBuild_String_concatCStr(&outPath, ".o");

        CBuild_String_concatCStr(&srcPath, ".cpp");

        CBuild_String command = CBuild_String_init("g++ ");
        CBuild_String_concat(&command, &srcPath);
        CBuild_String_concatCStr(&command, " -c -o ");
        CBuild_String_concat(&command, &outPath);

        printf("SRC: %s\nOUT: %s\nCMD: %s\n", srcPath.str, outPath.str, command.str);

        if (system(command.str))
        {
            fprintf(stderr, "Failed to compile: %s\n", srcPath.str);
        }

        CBuild_String_deinit(&srcPath);
        CBuild_String_deinit(&outPath);
        CBuild_String_deinit(&command);

        CBuild_String_tokenizer(&dir, &depFolder, ":");
    }

    CBuild_String_deinit(&dir);

    CBuild_String buildDeps = CBuild_Fs_dir("./sample/code", "/*.*", CBUILD_FS_DIRMODE_FILES, " ");
    CBuild_String finalCommand = CBuild_String_init("g++ ./sample/main.cpp ");
    CBuild_String_concat(&finalCommand, &buildDeps);
    CBuild_String_concatCStr(&finalCommand, "-o ./sample/build/main.exe");
    
    CBuild_system(finalCommand.str, "100% Compiled successfully!\n", "Failed to compile main.cpp\n");
    
    CBuild_String_deinit(&buildDeps);
    CBuild_String_deinit(&finalCommand);
    
    return 0;
}