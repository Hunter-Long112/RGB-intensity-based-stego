#include "encoder.h"
#include <string.h>

int getFileLength(char *path){
    FILE *f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    fclose(f);
    return len;
}

/*
 * encodeDriver: driver function for encode mode of program
 */
void encodeDriver(int channel, char* coverFile, char* messageFile){

    // read contents of the cover file into an array 
    bmpData* cover = initBmpData(coverFile);
    char *messageData = readInFile(messageFile);

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

    // embedding loop
    // read in a single pixel's data
    // compare two non-indicator channels
    // write given # of bits in lowest intensity non-indicator
    // set indicator bit in indciator channel to whichvever channel

    freeBmpData(cover);

    return;
}

void embedData(int channel, bmpData *cover, char *messageData){
    char *header = NULL;
    //Copy cover file header over to new string
    strncpy(header, cover->fileContents, cover->pixelOffset);
}
