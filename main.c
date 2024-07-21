#include <stdio.h>

#include "cbuilder/cbuilder.h"

int main()
{
    CBuild_String dir = CBuild_Fs_dir(".", CBUILD_FS_DIRMODE_FILES | CBUILD_FS_DIRMODE_FOLDERS);

    CBuild_String tok = {NULL, 0, 0};
    CBuild_String_tokenizer(&dir, &tok, "|");
    while (tok.str[0] != '\0')
    {
        fwrite(tok.str, tok.len, 1, stdout);
        putc('\n', stdout);

        char *dotPos = strchr(tok.str, '.');
        if (dotPos)
        {
            dotPos[2] = '\0';
            printf("DEBUG: %s\n", dotPos);
            
            if (!strcmp(dotPos, ".c"))
            {
                CBuild_String command = CBuild_String_init("");
                CBuild_String_concatCStr(&command, "gcc ");
                
                CBuild_String execName = CBuild_String_initN(tok.str, tok.len);
                
                CBuild_String_concat(&command, &execName);
                CBuild_String_concatCStr(&command, " -o ");
                CBuild_String_concat(&command, &execName);
                CBuild_String_concatCStr(&command, ".exe");

                system(command.str);
                CBuild_String_deinit(&command);
                CBuild_String_deinit(&execName);
            }
        }

        CBuild_String_tokenizer(&dir, &tok, "|");
    }

    CBuild_String_deinit(&dir);
    return 0;
}