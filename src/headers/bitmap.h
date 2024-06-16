#pragma once

/* ---- Standard Library ---- */
#include <cstdio>
#include <cerrno>

/* ---- OpenGL Headers ---- */
#include "glad/glad.h"

/* ---- Windows Library Files ---- */
#ifdef _WIN32
#include <windows.h>
#include <wingdi.h>
#endif

/* ---- Definitions ---- */
#ifdef __unix
    #define fopen_s(pFile, filename, mode) ((*(pFile)) = fopen((filename), (mode))) == NULL
    typedef int errno_t;

    #ifndef BMP
        #define BMP

        // The data types in this section are aliases for C/C++ primitive data types.
        #define LONG    int
        #define DWORD   unsigned int
        #define WORD    unsigned short

        // The BITMAPFILEHEADER structure contains information about the type, size,
        // and layout of a file that contains a DIB [device-independent bitmap].
        typedef struct tagBITMAPFILEHEADER
        {
            WORD    bfType;
            DWORD   bfSize;
            WORD    bfReserved1;
            WORD    bfReserved2;
            DWORD   bfOffBits;
        } __attribute__((__packed__)) BITMAPFILEHEADER;

        // The BITMAPINFOHEADER structure contains information about the
        // dimensions and color format of a DIB [device-independent bitmap]
        typedef struct tagBITMAPINFOHEADER
        {
            DWORD   biSize;
            LONG    biWidth;
            LONG    biHeight;
            WORD    biPlanes;
            WORD    biBitCount;
            DWORD   biCompression;
            DWORD   biSizeImage;
            LONG    biXPelsPerMeter;
            LONG    biYPelsPerMeter;
            DWORD   biClrUsed;
            DWORD   biClrImportant;
        } __attribute__((__packed__)) BITMAPINFOHEADER;
    #endif // BMP
#endif

GLuint loadbitmap(const char *filename, unsigned char *&pixelBuffer, BITMAPINFOHEADER *infoHeader, BITMAPFILEHEADER *fileHeader)
{
    FILE *bitmapFile;

    errno_t err = fopen_s(&bitmapFile, filename, "rb");

    if (err != 0 || bitmapFile == NULL)
    {
        printf("loadbitmap - open failed for %s\n", filename);
        return NULL;
    }

    fread(fileHeader, sizeof(BITMAPFILEHEADER), 1, bitmapFile);

    if (fileHeader->bfType != 0x4D42)
    {
        printf("loadbitmap - type failed \n");
        return NULL;
    }

    fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, bitmapFile);

    if (infoHeader->biBitCount < 24)
    {
        printf("loadbitmap - bitcount failed = %d\n", infoHeader->biBitCount);
        return NULL;
    }

    fseek(bitmapFile, fileHeader->bfOffBits, SEEK_SET);

    int nBytes = infoHeader->biWidth * infoHeader->biHeight * 3;
    pixelBuffer = new unsigned char[nBytes];
    fread(pixelBuffer, sizeof(unsigned char), nBytes, bitmapFile);

    fclose(bitmapFile);

    for (int i = 0; i < nBytes; i += 3)
    {
        unsigned char tmp = pixelBuffer[i];
        pixelBuffer[i] = pixelBuffer[i + 2];
        pixelBuffer[i + 2] = tmp;
    }

    printf("loadbitmap - Loaded %s | width=%d, height=%d | color-bits=%d\n",
           filename, infoHeader->biWidth, infoHeader->biHeight, infoHeader->biBitCount);

    return 0;
}

