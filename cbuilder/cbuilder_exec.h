#ifndef INCLUDED_CBUILDER_EXEC
#define INCLUDED_CBUILDER_EXEC

#include <stdio.h>
#include <stdlib.h>

int CBuild_system(char *command, char *successMsg, char *errorMsg)
{
    int retVal = system(command);
    if (retVal)
    {
        fputs(errorMsg, stderr);
    }
    else
    {
        fputs(successMsg, stdout);
    }

    return retVal;
}

#endif // INCLUDED_CBUILDER_EXEC