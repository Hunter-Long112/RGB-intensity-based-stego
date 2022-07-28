#include "bmpInterface.h"

/*
 * readInFile: function to read in the contents of a file as bytes, stores contents in char array
 */
char* readInFile(char* fileName){

    FILE* filePtr;
    char* fileContents;
    long fileLength;

    // open file for reading in binary mode
    filePtr = fopen(fileName, "rb");

    // get length of file by jumping to the end and recording byte offset
    fseek(filePtr, 0, SEEK_END);
    fileLength = ftell(filePtr);
    rewind(filePtr);

    // allocate enough memory to store contents of file
    fileContents = (char *)malloc(fileLength * sizeof(char));

    // read in contents of file
    fread(fileContents, fileLength, 1, filePtr);

    // close file
    fclose(filePtr);

    return fileContents;

}

/*
 * isValidBitMap: function to verify if a file is a 24 bit bmp file
 * Author: Professor Ortiz (all we did was add check to ensure it is specifically a 24 bit bmp)
 */
int isValidBitMap(char *filedata){

    printf("Magic Number:%c%c\n", filedata[0], filedata[1]);
	if( filedata[0] != 'B' || filedata[1] != 'M') return 0;

    // validate that file is 24 bits per pixel
    // set integer to 0 to be used to extract bits per pixel from file
    int x = 0;
    x = x & 0x00000000;
    // bitwise or low byte of x with most significant byte of 'bits per pixel' from file
    x = x | filedata[29];
    // shift left 8 times (this puts most significant byte of 'bits per pixel' in the correct place and clears the low byte of x)
    x = x << 8;
    // bitwise or low byte of x with lest significant byte of 'bits per pixel' from file, x is now the value of bits per pixel
    x = x | filedata[28];
    printf("Bits Per Pixel: %d (0x%x)\n", x, x);
    if (x != 24) return 0;

	return 1;
}

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
