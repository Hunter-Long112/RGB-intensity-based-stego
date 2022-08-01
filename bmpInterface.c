#include "bmpInterface.h"

/**
 * Authors:
 * Hunter Long (mia014)
 * Thomas White (yhy312)
 * Michael Ginsberg (sfi208)
 */

/*
 * swapEndian: function to swap the data stored in an int from little endian to big endian 
 */
int swapEndian(int value){
    //Q1Q2Q3Q4
    //Q4Q3Q2Q1
    int q1 = (value & 0xff000000) >> 6*4;
    int q2 = (value & 0x00ff0000) >> 2*4;
    int q3 = (value & 0x0000ff00) << 2*4;
    int q4 = (value & 0x000000ff) << 6*4;
    return q4 | q3 | q2 | q1;
}

/*
 * hexDump: prints a byte stream in hexidecimal format (mock of hexdump tool)
 */
void hexDump(char *data, int numBytes){
    for(int i = 0; i < numBytes; i++){
        if(i % 16 == 0)
            printf("\n%08x: ", i);
        if(i % 8 == 0)
            printf(" ");
        if(i == 320){
            break;
        }
        printf("%02x ", (unsigned char) data[i]);
    }
    printf("\n");
}

/*
 * readInFile: function to read in the contents of a file as bytes, stores contents in char array
 */
char* readInFile(char* fileName){

    FILE* filePtr;
    char* fileContents;
    long fileLength;
    filePtr = fopen(fileName, "rb");                            // open file for reading in binary mode
    fseek(filePtr, 0, SEEK_END);                                // get length of file by jumping to the end and recording byte offset
    fileLength = ftell(filePtr);
    rewind(filePtr);
    fileContents = (char *)malloc(fileLength * sizeof(char)+1); // allocate enough memory to store contents of file
    fread(fileContents, fileLength, 1, filePtr);                // read in contents of file
    fclose(filePtr);                                            // close file
    return fileContents;

}

/*
 * isValidBitMap: function to verify if a file is a 24 bit bmp file
 */
int isValidBitMap(char *filedata){

    if( filedata[0] != 'B' || filedata[1] != 'M') return 0;

    // validate that file is 24 bits per pixel
    int x = 0;
    x = x & 0x00000000;     // set integer to 0 to be used to extract bits per pixel from file
    x = x | filedata[29];   // bitwise or low byte of x with most significant byte of 'bits per pixel' from file
    x = x << 8;             // put most significant byte of 'bits per pixel' in the correct place and clear the low byte of x
    x = x | filedata[28];   // bitwise or low byte of x with lest significant byte of 'bits per pixel' from file, x is now the value of bits per pixel
    if (x != 24) return 0;

    // verify that the horizontal dimension of the file is divisible by 4, so that there are no padding bytes
    char *horizontalFileDimension = &filedata[18];
    unsigned int i = *((unsigned int *) horizontalFileDimension);
    if((i % 4) != 0){
        printf("The cover file you've selected does not have a horizontal dimension that is divisible by four, so there are padding bytes added to each row.\n");
        printf("Please select a cover image that has a horizontal dimension divisible by four.\n");
        exit(-1);
    }

    return 1;
}

/*
 * initBmpData: given a path to a bmp file, will initialize a bmpData struct with info about / contents
 * of the bmp file and return a pointer to the struct
 */
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
 * freeBmpData: given a pointer to a bmpData struct, frees it from memory
 */
void freeBmpData(bmpData *data){
    free(data->fileContents);
    free(data);
}

/*
 * readKthBit: reads in a specific, single bit from a given byte (char data)
 */
int readKthBit(int k, char data){
    return (data >> k) & 1;
}

/*
 * writeKthBit: writes a specific, single bit to a byte (char data) at bit location k
 * returns the modified int value 
 */
int writeKthBit(int k, char data, int value){
    int mask = 1 << k;
    return ((data & ~mask) | (value << k));
}

/*
 * readPixel: reads the values of the color channels of the next pixel in a file into a given pixel struct
 */
int readPixel(FILE *coverFile, pixel *newPixel){
    int ch = 0;
    if((ch = fgetc(coverFile)) != EOF){
        newPixel->blue = ch;
    }
    else return EOF;
    if((ch = fgetc(coverFile)) != EOF){
        newPixel->green = ch;
    }
    else return EOF;
    if((ch = fgetc(coverFile)) != EOF){
        newPixel->red = ch;
    }
    else return EOF;
    return 0;
}

/*
 * printBits: prints size number of bytes worth of bits starting at the address passed in as ptr
 */
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}
