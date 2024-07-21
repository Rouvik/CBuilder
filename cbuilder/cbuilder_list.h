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

#ifndef INCLUDED_CBUILDER_LIST
#define INCLUDED_CBUILDER_LIST

#include <stdio.h>
#include <stdlib.h>

#define MAX_ITERATION 1000000 // dont go above this iteration limit

typedef struct
{
    void *data;
    
    CBuilder_List* next;
} CBuilder_List;

CBuilder_List *CBuilder_List_init(void *data)
{
    CBuilder_List *root = (CBuilder_List *)malloc(sizeof(CBuilder_List));
    root->data = data;
    root->next = NULL;
    return root;
}

void CBuilder_List_deinit(CBuilder_List *root)
{
    while (root != NULL)
    {
        CBuilder_List *fptr = root;
        root = root->next;
        free(fptr);
    }
}

CBuilder_List *CBuilder_List_pushBack(CBuilder_List *root, void *data)
{
    int i = 0;
    while (i++ < MAX_ITERATION)
    {
        if (root->next == NULL)
        {
            break;
        }
        root = root->next;
    }

    if (i > MAX_ITERATION)
    {
        fprintf(stderr, "[CBuilder_List_PushBack Error] Max cap exceeded in pushBack operation with current root: %p, and data: %p", root, data);
        return NULL;
    }
    
    root->next = CBuilder_List_init(data);
    return root->next;
}

#endif // INCLUDED_CBUILDER_LIST