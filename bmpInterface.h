#ifndef _bmpinterface_h
#define _bmpinterface_h

#include <stdio.h>
#include <stdlib.h>

/**
 * Authors:
 * Hunter Long (mia014)
 * Thomas White (yhy312)
 * Michael Ginsberg (sfi208)
 */

char* readInFile(char* fileName);
int isValidBitMap(char *filedata);
struct bmpData *initBmpData(char *path);
void freeBmpData(struct bmpData *data);
int readKthBit(int k, char data);
int writeKthBit(int k, char data, int value);
int readPixel(FILE *coverFile, pixel *newPixel);
void hexDump(char *data, int numBytes);
int swapEndian(int value);
void printBits(size_t const size, void const * const ptr);

enum RGB {INVALID, RED, GREEN, BLUE};
typedef enum RGB RGB;

/*
 * pixel: struct to hold the three color channel values of a given pixel
 */
typedef struct pixel {
   unsigned char red;   // red color channel value
   unsigned char green; // green color channel value
   unsigned char blue;  // blue color channel value
} pixel;

/*
 * bmpData: struct to hold important data about the bmp file we are working with
 */
typedef struct bmpData
{
    int imageSize;      // number of pixels in image
    int pixelOffset;    // byte offset into file where pixel data begins
    char *fileContents; // array that stores the contents of the bmp file (cover or stego)
    char *fileName;     // name of bmp file
} bmpData;

#endif
