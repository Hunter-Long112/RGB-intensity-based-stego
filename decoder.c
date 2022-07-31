#include "decoder.h"
#include "bmpInterface.h"

#include <string.h>

void writeMsg(char *msg, int msgLen);

int numLsb = 3;

unsigned char getEmbeddedChar(pixel* pixel, RGB channel);

void decodeDriver(RGB indicator, char* stegoFile){

    FILE *stegFile = fopen(stegoFile, "r");
    bmpData* stegoFileData = initBmpData(stegoFile);
    // verify that the cover file is a 24 bit bmp file
    if(!isValidBitMap(stegoFileData->fileContents)){
        printf("Error: Cover file is not a 24 bit bmp file, exiting...\n");
        exit(1);
    }

    pixel *stegPixel = (pixel *) malloc(sizeof(pixel));
    unsigned int numDataRead = 54;
    fseek(stegFile, numDataRead, SEEK_SET);
    int i, j, remainder = 0;
    unsigned char hiddenChar;
    //Extract first 32 bits to get message length
    unsigned int msgLength = 0;
    int timesToLoop = ((32/numLsb + 32%numLsb)-1);
    for(i = 0; i < timesToLoop; i++){
        readPixel(stegFile, stegPixel);
        hiddenChar = getEmbeddedChar(stegPixel, indicator);
        for(j = numLsb-1; j >= 0; j--){
            if(i == timesToLoop-1 && j == (numLsb-(32%numLsb)-1)){
                remainder = j+1;
                break;
            }
            int hiddenBit = readKthBit(j, hiddenChar);
            msgLength = (msgLength << 1) | hiddenBit;
        }
    }
    printf("msgLength: %08x %u\nmsgLength (swap): %08x %u\n", msgLength, msgLength, swapEndian(msgLength), swapEndian(msgLength));
    msgLength = swapEndian(msgLength);

    //Hidden message length in bytes extracted, now calculate how many pixels to read
    //and extract
    int pixelsToRead = msgLength*numLsb;
    int bitsInByte = 0;
    unsigned int msgIndex = 0;
    unsigned char currentByte = 0;
    char *msg = (char *) malloc(sizeof(char)*msgLength+1);
    for(i = 0; i < pixelsToRead; i++){
        if(remainder > 0){
            //Don't read in new pixel...
        }
        else readPixel(stegFile, stegPixel);
        hiddenChar = getEmbeddedChar(stegPixel, indicator);
        if(bitsInByte > 7){
            bitsInByte = 0;
            msg[msgIndex++] = currentByte;
            currentByte = 0;
        }
        for(j = numLsb-1; j >= 0; j--){
            if(remainder > 0){
                j = remainder-1;
                remainder = 0;
            }
            if(bitsInByte > 7){
                remainder = j+1;
                break;
            }
            int hiddenBit = readKthBit(j, hiddenChar);
            currentByte = (currentByte << 1) | hiddenBit;
            bitsInByte++;
        }
    }
    //printf("Message Hexdump:\n");
    //hexDump(msg, 100);
    //printf("Message:\n");
    //printf("%s\n", msg);
    writeMsg(msg, msgLength);

}


void writeMsg(char *msg, int msgLen){
    char extension[5] = ".txt";
    if(strncmp(msg, "BM", 2) == 0){
        strncpy(extension, ".bmp", 5);
    }
    char msgPath[25] = "extracted_message";
    strncat(msgPath, extension, 5);
    FILE *msgFile = fopen(msgPath, "w");
    fwrite(msg, msgLen, 1, msgFile);
    fclose(msgFile);
    printf("Wrote extracted message to %s.\n", msgPath);
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
