#ifndef _bmpinterface_h
#define _bmpinterface_h

#include <stdio.h>
#include <stdlib.h>

char* readInFile(char* fileName);
int isValidBitMap(char *filedata);
struct bmpData *initBmpData(char *path);
void freeBmpData(struct bmpData *data);
int readKthBit(int k, char data);
int writeKthBit(int k, char data, int value);

enum RGB {INVALID, RED, GREEN, BLUE};
typedef enum RGB RGB;

typedef struct pixel {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

/*
 * bmpData: struct to hold important data about the bmp file we are working with
 */
typedef struct bmpData
{
    //  don't forget - stored little endian!!!!
    int imageSize; // number of pixels (find by multiplying image dimensions, bytes 18-21 x bytes 22-25, inclusive in file contents array)
    int pixelOffset; // byte offset into file where pixel data begins (bytes 10-13, inclusive in file contents array)
    char *fileContents;
    char *fileName;
} bmpData;

int readPixel(FILE *coverFile, pixel *newPixel);

#endif
