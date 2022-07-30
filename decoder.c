#include "decoder.h"
#include "bmpInterface.h"

unsigned char getEmbeddedChar(pixel* pixel, RGB channel);

void decodeDriver(RGB channel, char* stegoFile){

    // this is going to be a cmd line arg eventually
    int numLsb = 3;

    // initialize bmpData struct
    bmpData* stego = initBmpData(stegoFile);

    // validate that stego file is a 24 bit bmp file
    if(!isValidBitMap(stego->fileContents)){
        printf("Error: Cover file is not a 24 bit bmp file, exiting...\n");
        exit(1);
    }

    // read in first 4 bytes of hidden data, this is the length of the hidden message
    pixel* currentPixel = (pixel *) malloc(sizeof(pixel));
    unsigned int hiddenMessageLength = 0;
    int bitCount = 0;
    unsigned char embeddedChar;
    int i, j = 0;

    FILE *stegFile = fopen(stego->fileName, "rb");
    fseek(stegFile, stego->pixelOffset, SEEK_SET);
    while(bitCount < 32){
        readPixel(stegFile, currentPixel);
        embeddedChar = getEmbeddedChar(currentPixel, channel);
        i = 0;
        while(i < numLsb && bitCount < 32){
            j = readKthBit(numLsb-1-i, embeddedChar);
            hiddenMessageLength = hiddenMessageLength << 1;
            hiddenMessageLength = hiddenMessageLength | j;
            i++;
            bitCount++;
        }
    }
    hiddenMessageLength = swapEndian(hiddenMessageLength);
    printBits(4, &hiddenMessageLength);
    printf("%u\n", hiddenMessageLength);
    printf("%x\n", hiddenMessageLength);

    // after first 4 bytes, every byte extracted until length of message is reached is written to message file

    return;
}

/*
 * getEmbeddedChar: determines which channel of a given pixel has data embedded in it based on the indicator channel
 */
unsigned char getEmbeddedChar(pixel* pixel, RGB channel){
    int x = 0;
    if(channel == RED){
        // get LSB of r channel
        x = pixel->red & 0x01;
        // determine channel based on LSB of r (indicator) channel
        switch(x){
        case(0):
            return pixel->green;
        case(1):
            return pixel->blue;
        default:
            printf("Something went wrong!\n");
            exit(-1);
        }
    }
    else if(channel == GREEN){
        // get LSB of g channel
        x = pixel->green & 0x01;
        // determine channel based on LSB of g (indicator) channel
        switch(x){
        case(0):
            return pixel->blue;
        case(1):
            return pixel->red;
        default:
            printf("Something went wrong!\n");
            exit(-1);
        }
    }
    else if(channel == BLUE){
        // get LSB of b channel
        x = pixel->blue & 0x01;
        // determine channel based on LSB of b (indicator) channel
        switch(x){
        case(0):
            return pixel->red;
        case(1):
            return pixel->green;
        default:
            printf("Something went wrong!\n");
            exit(-1);
        }
    }
    else{
        printf("Something went wrong!\n");
        exit(-1);
    }
}
