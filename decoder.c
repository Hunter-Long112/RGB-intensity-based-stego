#include "decoder.h"
#include "bmpInterface.h"

#include <string.h>

/**
 * Authors:
 * Hunter Long (mia014)
 * Thomas White (yhy312)
 * Michael Ginsberg (sfi208)
 */

void writeMsg(char *msg, int msgLen, char* stegoFile);
unsigned char getEmbeddedChar(pixel* pixel, RGB channel);

/*
 * Calculates the remainining embedded bits in a AFTER
 * reading the first 4 bytes (msg length) out of the embedded data.
 */
int calcRemainder(int numLSB){
   if(32%numLSB == 0){
       return 0;
   }
   else{
       return numLSB-32%numLSB;
   }
}

/*
 * Decode & extract the data out of a stego file
 *
 * param RGB indicator - the indicator channel of the stego file
 * param char* stegoFile - the path to the file containing embedded data
 * param int* numLSB - int pointer to the number of LSB's used to embed the data
 */
void decodeDriver(RGB indicator, char* stegoFile, int* numLSB){

    FILE *stegFile = fopen(stegoFile, "r");
    bmpData* stegoFileData = initBmpData(stegoFile);
    // verify that the cover file is a 24 bit bmp file
    if(!isValidBitMap(stegoFileData->fileContents)){
        printf("Error: Cover file is not a 24 bit bmp file, exiting...\n");
        exit(1);
    }

    //Allocate and/or set necessary variables for extraction loop...
    pixel *stegPixel = (pixel *) malloc(sizeof(pixel));
    bmpData* stegoBmp = initBmpData(stegoFile);
    int pixelOffset = stegoBmp->pixelOffset;
    free(stegoBmp);
    unsigned int numDataRead = pixelOffset;
    fseek(stegFile, numDataRead, SEEK_SET);
    int i, j, remainder = 0;
    unsigned char hiddenChar;
    //Extract first 32 bits to get message length
    unsigned int msgLength = 0;

    //Loop enough times to extract 4 bytes from message length...
    //In the case of the LSB being 7 then, you would need to loop 5 times (35)
    //with a remainder of 3 bits
    int timesToLoop = (32/(*numLSB))+((32%(*numLSB)) != 0);
    int hiddenBit = 0;
    for(i = 0; i < timesToLoop; i++){
        readPixel(stegFile, stegPixel);
        hiddenChar = getEmbeddedChar(stegPixel, indicator);
        for(j = (*numLSB)-1; j >= 0; j--){
            //If you have looped enough times to get 32 bits and there is a remainder left, set it
            if(i == timesToLoop-1 && j == calcRemainder(*numLSB)){
                remainder = j;
                hiddenBit = readKthBit(j, hiddenChar);
                msgLength = (msgLength << 1) | hiddenBit;
                break;
            }
            hiddenBit = readKthBit(j, hiddenChar);
            msgLength = (msgLength << 1) | hiddenBit;
        }
    }
    msgLength = swapEndian(msgLength);

    //Hidden message length in bytes extracted, now calculate how many pixels to read
    //and extract
    int pixelsToRead = msgLength*(*numLSB);
    int bitsInByte = 0;
    unsigned int msgIndex = 0;
    unsigned char currentByte = 0;
    char *msg = (char *) malloc(sizeof(char)*msgLength*4+1);
    for(i = 0; i < pixelsToRead; i++){
        if(remainder > 0){
            //Don't read in new pixel if there are remaining bits...
        }
        else readPixel(stegFile, stegPixel);
        hiddenChar = getEmbeddedChar(stegPixel, indicator);
        //If there are 8 bits in the current byte, write it to the msg
        if(bitsInByte > 7){
            bitsInByte = 0;
            msg[msgIndex++] = currentByte;
            currentByte = 0;
        }
        for(j = (*numLSB)-1; j >= 0; j--){
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
    writeMsg(msg, msgLength, stegoFile);

}


/*
 * Writes a message to a file, given the length of the message, the message,
 * and the path to the file.
 */
void writeMsg(char *msg, int msgLen, char *stegoFile){
    char extension[5] = ".txt";
    if(strncmp(msg, "BM", 2) == 0){
        strncpy(extension, ".bmp", 5);
    }
    else if(strncmp(msg, "\x89PNG", 4) == 0){
        strncpy(extension, ".png", 5);
    }
    char msgPath[500] = "ext_";
    int bytesToConcat = strnlen(stegoFile, 400)-4;
    strncat(msgPath, stegoFile, bytesToConcat);
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
