#pragma once

/* ---- Standard Library ---- */
#include <iostream>
#include <cstdio>

/* ---- Definitions ---- */
#ifdef __unix
    #define fopen_s(pFile, filename, mode) ((*(pFile)) = fopen((filename), (mode))) == NULL
#endif

/**
 * opens a file
 * allocates enough memory to store the contents as raw characters
 * returns the buffer of characters
 */
char *read_file(const char *filename)
{
    FILE *f;
    fopen_s(&f, filename, "rb");
    if (f == NULL)
    {
        // Need to set working directory in CLion settings
        printf("FILE <%s> NOT FOUND\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *bfr = (char *) malloc(sizeof(char) * (size + 1));
    if (bfr == NULL)
        return NULL;

    long ret = fread(bfr, 1, size, f);
    if (ret != size)
        return NULL;

    bfr[size] = '\0';
    return bfr;
}

