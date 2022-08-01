#ifndef _encoder_h
#define _encoder_h

#include <stdio.h>
#include <stdlib.h>

#include "bmpInterface.h"

unsigned int getFileLength(char *path);
void encodeDriver(int channel, char* coverFile, char* messageFile, int* numLSB);
RGB selectChannel(RGB indicator, pixel *pix);
void writeBitToChannel(RGB channel, pixel *pix, int k, int bit);
void embedData(enum RGB indicator, bmpData *cover, char *messageData, int* numLSB);

#endif
