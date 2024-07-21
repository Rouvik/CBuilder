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