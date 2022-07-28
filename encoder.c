#include "encoder.h"

bmpData *initBmpData(char *path){
    bmpData* data = malloc(sizeof(bmpData));
    data->fileContents = readInFile(path);
    data->fileName = path;

    int bitsPerPixel = *((short *) &data->fileContents[28]);

    char *fileDimensionOne = &data->fileContents[18];
    int x = *((int *) fileDimensionOne);

    char *fileDimensionTwo = &data->fileContents[22];
    int y = *((int *) fileDimensionTwo);

    data->imageSize = x * y;

    char *fileOffset = &data->fileContents[10];
    int offset = *((int *) fileOffset);
    data->pixelOffset = offset;
    return data;
}

/*
 * encodeDriver: driver function for encode mode of program
 */
void encodeDriver(int channel, char* coverFile, char* messageFile){

    // read contents of the cover file into an array 
    char* coverFileContents;
    coverFileContents = readInFile(coverFile);

    // int test = 0;
    // printf("test at start: %d\n", test);
    // test = test | coverFileContents[10];
    // printf("Data Offset: %d (0x%x)\n", test, test);

    // {
    // //  don't forget - stored little endian!!!!
    // int imageSize; // number of pixels (find by multiplying image dimensions, bytes 18-21 x bytes 22-25, inclusive in file contents array)
    // int pixelOffset; // byte offset into file where pixel data begins (bytes 10-13, inclusive in file contents array)
    // } bmpData;

    // verify that the cover file is a 24 bit bmp file
    if(!isValidBitMap(coverFileContents)){
        printf("Error: Cover file is not a 24 bit bmp file, exiting...\n");
    }

    // embedding loop
    // read in a single pixel's data
    // compare two non-indicator channels
    // write given # of bits in lowest intensity non-indicator
    // set indicator bit in indciator channel to whichvever channel

    free(coverFileContents);

    return;
}
