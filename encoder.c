#include "encoder.h"
#include <string.h>

int numLSB = 7;

unsigned int getFileLength(char *path){
    FILE *f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    unsigned int len = ftell(f);
    fclose(f);
    return len;
}

/*
 * encodeDriver: driver function for encode mode of program
 */
void encodeDriver(int channel, char* coverFile, char* messageFile){

    // read contents of the cover file into an array 
    bmpData* cover = initBmpData(coverFile);
    //char *messageData = readInFile(messageFile);

    // verify that the cover file is a 24 bit bmp file
    if(!isValidBitMap(cover->fileContents)){
        printf("Error: Cover file is not a 24 bit bmp file, exiting...\n");
        exit(1);
    }

    //Message data too long!
    int msgLen = getFileLength(messageFile);
    if(((cover->imageSize*numLSB)/8) < msgLen){
        fprintf(stderr, "Message file of length %d bytes exceeds capacity of %d.\n", msgLen, (cover->imageSize*numLSB)/8);
        exit(1);
    }

    embedData(channel, cover, messageFile, msgLen);

    freeBmpData(cover);

    return;
}

RGB selectChannel(RGB indicator, pixel *pix){
    switch(indicator){
        //BGR
        case RED:
            if(pix->green > pix->blue){
                writeBitToChannel(indicator, pix, 0, 1);
                return BLUE;
            }
            else{
                writeBitToChannel(indicator, pix, 0, 0);
                return GREEN;
            }
        case GREEN:
            if(pix->red > pix->blue){
                writeBitToChannel(indicator, pix, 0, 0);
                return BLUE;
            }
            else{
                writeBitToChannel(indicator, pix, 0, 1);
                return RED;
            }
        case BLUE:
            if(pix->green > pix->red){
                writeBitToChannel(indicator, pix, 0, 0);
                return RED;
            }
            else{
                writeBitToChannel(indicator, pix, 0, 1);
                return GREEN;
            }
        case INVALID:
            printf("Something went wrong!\n");
            exit(-1);
    }
    return RED;
}

void writeBitToChannel(RGB channel, pixel *pix, int k, int bit){
   if(channel == RED)
       pix->red = writeKthBit(k, pix->red, bit);
   if(channel == GREEN)
       pix->green = writeKthBit(k, pix->green, bit);
   if(channel == BLUE)
       pix->blue = writeKthBit(k, pix->blue, bit);
}


void embedData(enum RGB indicator, bmpData *cover, char *messageData, unsigned int msgLen){
    FILE *coverFile = fopen(cover->fileName, "r");
    FILE *msgFile = fopen(messageData, "r");
    unsigned int msgLength = getFileLength(messageData);

    char *stegData = NULL;
    stegData = malloc(sizeof(char)*(cover->pixelOffset+(cover->imageSize*3)+1));
    fread(stegData, cover->pixelOffset, 1, coverFile);

    int coverFileSize = getFileLength(cover->fileName);

    pixel *coverPixel = (pixel *) malloc(sizeof(pixel));

    int loopCounter = -1;
    int lenCount = 0;
    //Offset into stegData string where we write actual modified pixel data (after pixel offset)
    int msgOffset = cover->pixelOffset;
    char msgByte = 0;
    char* msgLenAsChar = (char *) &msgLength;

    //printf("MSG LENGTH: %u\n", msgLength);
    //printf("COVER LENGTH: %u\n", coverFileSize);
    //printBits(4, msgLenAsChar);
    int numDataRead = cover->pixelOffset;

    do {
        readPixel(coverFile, coverPixel);
        numDataRead += 3;
        RGB channelToUse = selectChannel(indicator, coverPixel);
        for(int i = numLSB-1; i >= 0; i--){
            //Embed message length first
            if(loopCounter < 0){
               loopCounter = 7;
                if(lenCount < 4)
                    msgByte = msgLenAsChar[lenCount++];
                else if(lenCount >= 4) msgByte = fgetc(msgFile);
            }
            int bitToWrite = readKthBit(loopCounter, msgByte);
            writeBitToChannel(channelToUse, coverPixel, i, bitToWrite);
            loopCounter--;
        }
        if(msgOffset > coverFileSize){
            printf("Capacity Hit.\n");
            break;
        }
        stegData[msgOffset++] = coverPixel->blue;
        stegData[msgOffset++] = coverPixel->green;
        stegData[msgOffset++] = coverPixel->red;
    } while(numDataRead < coverFileSize);
    //printf("Pixel data: \n");
    //hexDump(stegData, 320);
    char stegFileName[500] = "hid_";
    strncat(stegFileName, cover->fileName, 500);
    FILE * stegFile = fopen(stegFileName, "w");
    fwrite(stegData, coverFileSize, 1, stegFile);
    fclose(stegFile);
    printf("Stego File Created: %s\n", stegFileName);
}
