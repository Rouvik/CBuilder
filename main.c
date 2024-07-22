#include <stdio.h>

#include "cbuilder/cbuilder.h"

int main()
{
    CBuild_String dir = CBuild_Fs_dir(".", CBUILD_FS_DIRMODE_FILES | CBUILD_FS_DIRMODE_FOLDERS);

    CBuild_String tok = {NULL, 0, 0};
    CBuild_String_tokenizer(&dir, &tok, ":");
    while (tok.str[0] != '\0')
    {
        fwrite(tok.str, tok.len, 1, stdout);
        putc('\n', stdout);

        CBuild_String_tokenizer(&dir, &tok, ":");
    }

    printf("DEBUG: %s\n", dir.str);

    CBuild_String_deinit(&dir);
    return 0;
}