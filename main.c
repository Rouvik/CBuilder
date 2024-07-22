#include <stdio.h>

#include "cbuilder/cbuilder.h"

int main()
{
    CBuild_String dir = CBuild_Fs_dir(".", "/*.c", CBUILD_FS_DIRMODE_FILES | CBUILD_FS_DIRMODE_FOLDERS, ":");
    
    printf("OUTPUT: %s\n", dir.str);

    CBuild_String_deinit(&dir);
    return 0;
}