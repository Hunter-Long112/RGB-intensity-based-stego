#include "encoder.h"
#include <string.h>

/**
 * Authors:
 * Hunter Long (mia014)
 * Thomas White (yhy312)
 * Michael Ginsberg (sfi208)
 */

/*
 * When passed the path to a file as a string, it returns an int containing
 * the length of the file.
 */
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
void encodeDriver(int channel, char* coverFile, char* messageFile, int* numLSB){

    // read contents of the cover file into an array
    bmpData* cover = initBmpData(coverFile);

    // verify that the cover file is a 24 bit bmp file
    if(!isValidBitMap(cover->fileContents)){
        printf("Error: Cover file is not a 24 bit bmp file, exiting...\n");
        exit(1);
    }

    //Message data too long!
    int msgLen = getFileLength(messageFile);
    if(((cover->imageSize*(*numLSB))/8) < msgLen){
        fprintf(stderr, "Message file of length %d bytes exceeds capacity of %d.\n", msgLen, (cover->imageSize*(*numLSB))/8);
        exit(1);
    }

    embedData(channel, cover, messageFile, numLSB);

    freeBmpData(cover);

    return;
}

/*
 * Given the indicator channel and a pixel struct, this function returns
 * the channel that should contain the embedded data.
 */
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

/**
 * Provided an RGB channel, a pixel struct, an index (k), and a bit to write
 * this function writes the bit to the kth index of the provided channel.
 */
void writeBitToChannel(RGB channel, pixel *pix, int k, int bit){
   if(channel == RED)
       pix->red = writeKthBit(k, pix->red, bit);
   if(channel == GREEN)
       pix->green = writeKthBit(k, pix->green, bit);
   if(channel == BLUE)
       pix->blue = writeKthBit(k, pix->blue, bit);
}


/**
 * Embeds data read from one file into a cover image, then writes that to another file.
 *
 * param bmpData *cover - bmpData pointer to the bmpData struct of the cover image
 * param char *messageData - string containing the data of the message to embed
 * param RGB indicator - the indicator channel of the stego file
 * param int *numLSB - int pointer to the number of LSB's to embed in
 */
void embedData(enum RGB indicator, bmpData *cover, char *messageData, int* numLSB){
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
    int msgOffset = cover->pixelOffset;
    char msgByte = 0;
    char* msgLenAsChar = (char *) &msgLength;

    int numDataRead = cover->pixelOffset;
    unsigned int bytesWritten = 0;

    //Embedding loop - start by embedding 4 bytes containing length of message
    //Read in a pixel, find the channel to embed in, and write the data
    do {
        readPixel(coverFile, coverPixel);
        numDataRead += 3;
        RGB channelToUse = selectChannel(indicator, coverPixel);
        //Embed LSB bits at a time
        for(int i = (*numLSB)-1; i >= 0; i--){
            //Embed message length first
            if(loopCounter < 0){
               loopCounter = 7;
               bytesWritten++;
               //lenCount refers to the number of message length bytes embedded
                if(lenCount < 4)
                    msgByte = msgLenAsChar[lenCount++];
                else if(lenCount >= 4) msgByte = fgetc(msgFile);
            }
            int bitToWrite = readKthBit(loopCounter, msgByte);
            if(bytesWritten-4 <= msgLength){
                writeBitToChannel(channelToUse, coverPixel, i, bitToWrite);
            }
            loopCounter--;
        }
        if(msgOffset > coverFileSize){
            printf("Capacity Hit.\n");
            break;
        }
        //Finally, write the embedded data to the stegData string
        stegData[msgOffset++] = coverPixel->blue;
        stegData[msgOffset++] = coverPixel->green;
        stegData[msgOffset++] = coverPixel->red;
    } while(numDataRead < coverFileSize);

    //Then write to an output file
    char* dashOffset = strrchr(cover->fileName, '/');
    char stegFileName[500] = "hid_";

    //If cover file came from a directory, avoid it in the output file name:
    if(dashOffset != NULL)
        strncat(stegFileName, dashOffset+1, 500);
    else
        strncat(stegFileName, cover->fileName, 500);
    FILE * stegFile = fopen(stegFileName, "w");
    fwrite(stegData, coverFileSize, 1, stegFile);
    fclose(stegFile);
    printf("Stego File Created: %s\n", stegFileName);
}
