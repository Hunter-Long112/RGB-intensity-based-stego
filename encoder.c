#include "encoder.h"
#include <string.h>

int numLSB = 3;

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
    if(msgLen >= (cover->imageSize/2)){
        fprintf(stderr, "Message file of length %d bytes exceeds capacity of %d.\n", msgLen, (cover->imageSize/2));
        exit(1);
    }

    embedData(channel, cover, messageFile, msgLen);

    // embedding loop
    // read in a single pixel's data
    // compare two non-indicator channels
    // write given # of bits in lowest intensity non-indicator
    // set indicator bit in indciator channel to whichvever channel

    freeBmpData(cover);

    return;
}

RGB selectChannel(RGB indicator, pixel *pix){
    switch(indicator){
        case RED:
            if(pix->green > pix->blue)
                return BLUE;
            else
                return GREEN;
        case GREEN:
            if(pix->red > pix->blue)
                return BLUE;
            else
                return RED;
        case BLUE:
            if(pix->green > pix->red)
                return RED;
            else
                return GREEN;
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

    char *stegData = NULL;
    stegData = malloc(sizeof(char)*(cover->pixelOffset+(cover->imageSize*3)+1));
    fread(stegData, cover->pixelOffset, 1, coverFile);

    pixel *coverPixel = (pixel *) malloc(sizeof(pixel));

    int loopCounter = 0;
    char msgByte = 0;
    while(readPixel(coverFile, coverPixel) != EOF){
        RGB channelToUse = selectChannel(indicator, coverPixel);
        for(int i = numLSB-1; i >= 0; i--){
            if(loopCounter < 0){
               loopCounter = 7;
               msgByte = fgetc(msgFile);
            }
            int bitToWrite = readKthBit(loopCounter, msgByte);
            //TODO write modified 'coverPixel' to stegData string
            //
            //also, test by embedding 4 'A's for hunter to extract
            writeBitToChannel(channelToUse, coverPixel, i, bitToWrite);
            loopCounter--;
        }
    }
}
