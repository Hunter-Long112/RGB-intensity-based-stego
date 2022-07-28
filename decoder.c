#include "decoder.h"

unsigned char getEmbeddedChar(pixel* pixel, int channel);

void decodeDriver(int channel, char* stegoFile){

    // this is going to be a cmd line arg eventually
    int numBitsPerChannel = 3;

    // initialize bmpData struct
    bmpData* stego = initBmpData(stegoFile);

    // validate that stego file is a 24 bit bmp file
    if(!isValidBitMap(stego->fileContents)){
    printf("Error: Cover file is not a 24 bit bmp file, exiting...\n");
    exit(1);
    }

    // read in first 4 bytes of hidden data, this is the length of the hidden message
    int currentPixelOffset = stego->pixelOffset;
    pixel* currentPixel; 
    unsigned int hiddenMessageLength = 0;
    int bitCount = 0;
    unsigned char embeddedChar;
    int i, j = 0; 
    while(bitCount < 32){
        currentPixel->blue = stego->fileContents[currentPixelOffset];
        currentPixel->green = stego->fileContents[currentPixelOffset + 1];
        currentPixel->red = stego->fileContents[currentPixelOffset + 2];
        embeddedChar = getEmbeddedChar(currentPixel, channel);
        i = 0;
        while(i < numBitsPerChannel && bitCount < 32){
            j = readKthBit(i, embeddedChar);
            hiddenMessageLength = hiddenMessageLength | j;
            hiddenMessageLength = hiddenMessageLength << 1;
            j = 0;
            i++;
            bitCount++;
        }
        currentPixelOffset += 3; 
    }
    printf("%d\n", hiddenMessageLength);

    // after first 4 bytes, every byte extracted until length of message is reached is written to message file

    // extraction loop
        // read in pixel data to struct
        // check indicator channel to find out which channel has embedded data
        // bitwise & with mask (00000111)
        // store discovered bits 
            // int = 0
            // char = 0
            // or with int
            
            // write one bit from int to char
            // shift char over one
            // increment # of bits written to char by one
            // when written 8 bits to char, write to file
            // shift left 3
            // when you've written 8 bits to 
    return;
}

// make this a seperate helper function
// unsigned int getMessageLength(){}

unsigned char getEmbeddedChar(pixel* pixel, int channel){

    unsigned char hiderChar;
    int x = 0;

    if(channel == 1){
        // get LSB of r channel
        x = pixel->red & 0x01;
        // determine channel based on LSB of r (indicator) channel
        switch(x){
            case(0):
                hiderChar = pixel->green;
                break;
            case(1):
                hiderChar = pixel->blue;
                break;
            default:
                printf("Something went wrong!\n");
                exit(-1); 
        }
    }
    else if(channel == 2){
        // get LSB of g channel
        x = pixel->green & 0x01;
        // determine channel based on LSB of g (indicator) channel
        switch(x){
            case(0):
                hiderChar = pixel->blue;
                break;
            case(1):
                hiderChar = pixel->red;
                break;
            default:
                printf("Something went wrong!\n");
                exit(-1); 
        }
    }
    else{
        // get LSB of b channel
        x = pixel->blue & 0x01;
        // determine channel based on LSB of b (indicator) channel
        switch(x){
            case(0):
                hiderChar = pixel->red;
                break;
            case(1):
                hiderChar = pixel->green;
                break;
            default:
                printf("Something went wrong!\n");
                exit(-1);
        }
    }

    return hiderChar;

}

/*
char* readInFile(char* fileName);
int isValidBitMap(char *filedata);
struct bmpData *initBmpData(char *path);
void freeBmpData(struct bmpData *data);
int readKthBit(int k, char data);
int writeKthBit(int k, char data, int value);
*/

/*
 * bmpData: struct to hold important data about the bmp file we are working with
 */
/*
typedef struct bmpData
{
    //  don't forget - stored little endian!!!!
    int imageSize; // number of pixels (find by multiplying image dimensions, bytes 18-21 x bytes 22-25, inclusive in file contents array)
    int pixelOffset; // byte offset into file where pixel data begins (bytes 10-13, inclusive in file contents array)
    char *fileContents;
    char *fileName;
} bmpData;
*/