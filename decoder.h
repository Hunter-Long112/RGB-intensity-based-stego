#ifndef _decoder_h
#define _decoder_h

#include <stdio.h>
#include <stdlib.h>

#include "bmpInterface.h"

void decodeDriver(RGB channel, char* stegoFile);
unsigned char getEmbeddedChar(pixel* pixel, RGB channel);

#endif
