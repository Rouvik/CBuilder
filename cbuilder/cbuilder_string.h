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

#ifndef INCLUDED_CBUILDER_STRING
#define INCLUDED_CBUILDER_STRING

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define CBUILDER_BUF_CHUNK (256)

typedef struct
{
    char *str;   // heap allocated string
    int len;     // length of string
    int buf_len; // actual length of the buffer
} CBuild_String;

/**
 * @brief Initialises the CBuild_String struct with a heap alocated string with appropriate memory
 *
 * @param src The source string to initialise with, must be null terminated, or else will lead to undefined behaviour
 * @return CBuild_String The struct containing data about the string
 */
CBuild_String CBuild_String_init(const char *src)
{
    unsigned int count;
    int len = strlen(src);
    if (len == 0)
    {
        count = 1;
    }
    else
    {
        count = len / CBUILDER_BUF_CHUNK;
        if (len % CBUILDER_BUF_CHUNK) // allocate extra for spill
        {
            count++;
        }
    }

    int buf_len = CBUILDER_BUF_CHUNK * count;
    char *newMem = (char *)malloc(buf_len); // generate and copy the provided string to heap string
    memcpy(newMem, src, len + 1);

    return (CBuild_String){
        newMem,
        len,
        buf_len};
}

/**
 * @brief Initialises the CBuild_String struct with a length of len on a heap alocated string with appropriate memory
 *
 * @param src The source string to initialise with, must be null terminated, or else will lead to undefined behaviour
 * @param len The length of the string to copy and allocate for
 * @return CBuild_String The struct containing data about the string
 */
CBuild_String CBuild_String_initN(const char *src, int len)
{
    unsigned int count;
    if (len == 0)
    {
        count = 1;
    }
    else
    {
        count = len / CBUILDER_BUF_CHUNK;
        if (len % CBUILDER_BUF_CHUNK) // allocate extra for spill
        {
            count++;
        }
    }

    int buf_len = CBUILDER_BUF_CHUNK * count;
    char *newMem = (char *)malloc(buf_len); // generate and copy the provided string to heap string

    int srcLen = strlen(src);
    memcpy(newMem, src, (srcLen > len ? len : srcLen) + 1);

    newMem[len] = '\0'; // make sure the last bit is \0

    return (CBuild_String){
        newMem,
        len,
        buf_len};
}

/**
 * @brief Standard call for cstd free for char *str, also sets the internal states to 0
 *
 * @param str The CBuild_String ref to free
 */
void CBuild_String_deinit(CBuild_String *str)
{
    free(str->str);
    str->len = 0;
    str->buf_len = 0;
}

/**
 * @brief Resets the provided CBuild_String to empty state
 *
 * @param str The CBuilder_String pointer to the String
 */
void CBuild_String_reset(CBuild_String *str)
{
    CBuild_String_deinit(str);
    *str = CBuild_String_init("");
}

/**
 * @brief Copies an existing CBuild_String to a new CBuild_String @n
 *        Note: The returned CBuild_String is same as any other CBuild_String and must be freed with
 *        CBuild_String_deinit(CBuild_String *) call
 *
 * @param str The CBuild_String * to copy from
 * @return CBuild_String The CBuild_String copied to
 */
CBuild_String CBuild_String_copy(CBuild_String *str)
{
    CBuild_String newStr;
    newStr.str = (char *)malloc(str->buf_len); // assign new memory on heap
    newStr.buf_len = str->buf_len;     // copy the buf_len and len
    newStr.len = str->len;

    memcpy(newStr.str, str->str, str->buf_len); // actually copy the string
    return newStr;
}

/**
 * @brief Concatenates and possibly expands str1 with str2 and returns ref to str1
 *
 * @param str1 The first CBuild_String* to concatenate to
 * @param str2 The second CBuild_String* to concatenate from
 * @return CBuild_String* The reference to str1
 */
CBuild_String *CBuild_String_concat(CBuild_String *str1, CBuild_String *str2)
{
    int max_len = str1->len + str2->len + 2;
    if (str1->buf_len < max_len) // not enough size, first allocate memory for concat [+2 for \0 chars]
    {
        int chunk_count = max_len / CBUILDER_BUF_CHUNK;
        if (max_len % CBUILDER_BUF_CHUNK) // allocate extra chunk for spill
        {
            chunk_count++;
        }

        str1->buf_len = CBUILDER_BUF_CHUNK * chunk_count; // set new buf_len
        char *newMem = (char *)malloc(str1->buf_len);
        memcpy(newMem, str1->str, str1->len + 1);
        free(str1->str); // delete the orginal and assign new
        str1->str = newMem;
    }

    memcpy((str1->str + str1->len), str2->str, str2->len + 1); // assumes str1 has enough space

    str1->len += str2->len; // assign the new len

    return str1; // return the str1 reference
}

/**
 * @brief Concatenates and possibly expands str1 with str2 and returns ref to str1
 *
 * @param str1 The first CBuild_String* to concatenate to
 * @param str2 The second char* to concatenate from
 * @return CBuild_String* The reference to str1
 */
CBuild_String *CBuild_String_concatCStr(CBuild_String *str1, const char *str2)
{
    int len2 = strlen(str2);
    int max_len = str1->len + len2;
    if (str1->buf_len < max_len) // not enough size, first allocate memory for concat [+2 for \0 chars]
    {
        int chunk_count = max_len / CBUILDER_BUF_CHUNK;
        if (max_len % CBUILDER_BUF_CHUNK) // allocate extra chunk for spill
        {
            chunk_count++;
        }

        str1->buf_len = CBUILDER_BUF_CHUNK * chunk_count; // set new buf_len
        char *newMem = (char *)malloc(str1->buf_len);
        memcpy(newMem, str1->str, str1->len);
        free(str1->str); // delete the orginal and assign new
        str1->str = newMem;
    }

    memcpy((str1->str + str1->len), str2, len2 + 1); // assumes str1 has enough space

    str1->len += len2; // assign the new length

    return str1; // return the str1 reference
}

/**
 * @brief Concatenates and possibly expands str1 with str2 and returns ref to str1 for count number of bytes
 *        also it automatically attaches a \0 character at the end of the string of str1
 *
 * @param str1 The first CBuild_String* to concatenate to
 * @param str2 The second CBuild_String* to concatenate from
 * @param count The number of characters to concatenate excluding the ending \0
 * @return CBuild_String* The reference to str1
 */
CBuild_String *CBuild_String_concatN(CBuild_String *str1, CBuild_String *str2, int count)
{
    int max_len = str1->len + str2->len + 2;
    if (str1->buf_len < max_len) // not enough size, first allocate memory for concat [+2 for \0 chars]
    {
        int chunk_count = max_len / CBUILDER_BUF_CHUNK;
        if (max_len % CBUILDER_BUF_CHUNK) // allocate extra chunk for spill
        {
            chunk_count++;
        }

        str1->buf_len = CBUILDER_BUF_CHUNK * chunk_count; // set new buf_len
        char *newMem = (char *)malloc(str1->buf_len);
        memcpy(newMem, str1->str, str1->len + 1);
        free(str1->str); // delete the orginal and assign new
        str1->str = newMem;
    }

    if (count < str2->len)
    {
        memcpy((str1->str + str1->len), str2->str, count); // assumes str1 has enough space
        *(str1->str + str1->len + count + 1) = '\0'; // make sure it is null terminated
        str1->len += count; // assign the new len
    }
    else
    {
        memcpy((str1->str + str1->len), str2->str, str2->len); // assumes str1 has enough space
        *(str1->str + str1->len + str2->len + 1) = '\0'; // make sure it is null terminated
        str1->len += str2->len; // assign the new len
    }

    return str1; // return the str1 reference
}

/**
 * @brief Concatenates and possibly expands str1 with str2 and returns ref to str1 for count number of bytes
 *        also it automatically attaches a \0 character at the end of the string of str1
 *
 * @param str1 The first CBuild_String* to concatenate to
 * @param str2 The second char* to concatenate from
 * @param count The number of characters excluding the ending \0
 * @return CBuild_String* The reference to str1
 */
CBuild_String *CBuild_String_concatCStrN(CBuild_String *str1, char *str2, int count)
{
    int len2 = strlen(str2);
    int max_len = str1->len + len2;
    if (str1->buf_len < max_len) // not enough size, first allocate memory for concat [+2 for \0 chars]
    {
        int chunk_count = max_len / CBUILDER_BUF_CHUNK;
        if (max_len % CBUILDER_BUF_CHUNK) // allocate extra chunk for spill
        {
            chunk_count++;
        }

        str1->buf_len = CBUILDER_BUF_CHUNK * chunk_count; // set new buf_len
        char *newMem = (char *)malloc(str1->buf_len);
        memcpy(newMem, str1->str, str1->len);
        free(str1->str); // delete the orginal and assign new
        str1->str = newMem;
    }

    if (count < len2)
    {
        memcpy((str1->str + str1->len), str2, count); // assumes str1 has enough space
        *(str1->str + str1->len + count + 1) = '\0'; // make sure it is null terminated
        str1->len += count; // assign the new len
    }
    else
    {
        memcpy((str1->str + str1->len), str2, len2); // assumes str1 has enough space
        *(str1->str + str1->len + len2 + 1) = '\0'; // make sure it is null terminated
        str1->len += len2; // assign the new len
    }

    return str1; // return the str1 reference
}

inline int isDelim(char ch, char *delim)
{
    while (*delim != '\0')
    {
        if (ch == *delim)
        {
            return 1;
        }

        delim++;
    }

    return 0;
}

/**
 * @brief Tokenizes an instance of CBuild_String with the delimeters from char *delim string
 *
 * @param original The original CBuild_String * to read from, must be passed each call, and must not be changed during calls
 * @param prevToken The previous token read from the tokenizer, must be set to {NULL, 0, 0} initially to begin with the process,
 *                  also this function is reentrant due to CBuild_String *prevToken @n
 *                  Node: During first call, prevToken len and buf_len may not be initialised
 * @param delim The const char *delim string where each character is a delimeter
 */
void CBuild_String_tokenizer(CBuild_String *original, CBuild_String *prevToken, const char *delim)
{
    if (prevToken->str == NULL) // first time tokenize
    {
        prevToken->str = original->str;
        prevToken->len = 0;
        prevToken->buf_len = 0; // assign the buffer length as 0 this proves that this string is a container string
    }
    else
    {
        prevToken->str += prevToken->len + 1; // update to new possible token location
    }

    char *ptr = prevToken->str;
    while (strchr(delim, *ptr) != NULL && *ptr != '\0') // make sure ptr points to a valid string loc
    {
        ptr++;
    }

    prevToken->str = ptr; // assign the new pointer location to tokenize from
    int len = 0;
    while (strchr(delim, *ptr) == NULL && ptr - original->str < original->len)
    {
        len++;
        ptr++;
    }

    prevToken->len = len;
}

#endif // INCLUDED_CBUILDER_STRING