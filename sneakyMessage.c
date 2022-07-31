#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "encoder.h"
#include "decoder.h"

void printUsage(char** argv);
void parseArgs(int argc, char** argv, RGB* channel, int* mode, char** coverFile, char** messageFile, char** stegoFile);

#define programName "Sneaky Message"
#define invalid 0
#define encode 1
#define decode 2
#define r 1
#define g 2
#define b 3

/*
 * main: driver function for Sneaky Message program
 */
int main(int argc, char** argv){

    // if no arguements are passed in, print usage statement
    if(argc == 1){
        printUsage(argv);
    }

    // initialize variables to hold cmd line args and parse
    RGB* channel = (RGB*)malloc(sizeof(RGB));
    int* mode = (int*)malloc(sizeof(int));
    char** coverFile = (char**)malloc(sizeof(char*));
    char** messageFile = (char**)malloc(sizeof(char*));
    char** stegoFile = (char**)malloc(sizeof(char*));
    parseArgs(argc, argv, channel, mode, coverFile, messageFile, stegoFile);
    //printf("channel = %d\nmode = %d\ncoverFile = %s\nmessageFile = %s\nstegoFile = %s\n", *channel, *mode, *coverFile, *messageFile, *stegoFile);

    // Check for valid channel selection
    if(*channel == invalid){
        printf("Invalid indicator channel selected, please choose r, g, or b.\n");
        exit(-1);
    }

    // call functions based on selected mode (encode or decode)
    if(*mode == encode){
        if(access(*coverFile, R_OK|W_OK) != 0){
            printf("Cover file validation failed, check that file exists and has read/write permissions.\n");
            exit(-1);
        }
        if(access(*messageFile, R_OK) != 0){
            printf("Message file validation failed, check that file exists and has read permissions.\n");
            exit(-1);
        }
        // call encoder driver function
        encodeDriver(*channel, *coverFile, *messageFile);
    }
    else if(*mode == decode){
        if(access(*stegoFile, R_OK) != 0){
            printf("Stego file validation failed, check that file exists and has read permissions.\n");
            exit(-1);
        }
        // call decoder driver function
        decodeDriver(*channel, *stegoFile);
    }
    else if(*mode == invalid){
        printf("Invalid mode selected, please choose encode or decode.\n");
        exit(-1);
    }
    else{
        printf("Something went wrong!\n");
        exit(-1);
    }

    free(channel);
    free(mode);
    free(coverFile);
    free(messageFile);
    free(stegoFile);
    return 0;
}

/*
 * printUsage: prints the programs usage statement
 */
void printUsage(char** argv){
    printf("\n%s\nContributers: Hunter Long, Thomas White, and Michael Ginsberg\n\n", programName);
    printf("To hide a message file inside a 24 bit BMP file:\n\t%s -m encode -c <cover file> -h <message file> -i <indicator channel (r, g, or b)>\n\n", argv[0]);
    printf("To extract a hidden message from a 24 bit BMP file:\n\t%s -m decode -s <stego file> -i <indicator channel (r, g, or b)>\n\n", argv[0]);
    exit(0);
}

/*
 * parseArgs: parses the command line arguements
 * -c = cover image file
 * -h = message file
 * -s = stego file
 * -i = indicator channel (r, g, or b)
 * -m = mode (encode or decode)
 */
void parseArgs(int argc, char** argv, RGB* channel, int* mode, char** coverFile, char** messageFile, char** stegoFile){
    int c;
    while((c = getopt(argc, argv, "c:h:m:s:i:")) != -1){
        switch(c){
            case 'c':
                *coverFile = optarg;
                break;
            case 'h':
                *messageFile = optarg;
                break;
            case 'm':
                if(!strcmp(optarg, "encode")){
                    *mode = encode;
                }
                else if(!strcmp(optarg, "decode")){
                    *mode = decode;
                }
                else{
                    *mode = invalid;
                }
                break;
            case 's':
                *stegoFile = optarg;
                break;
            case 'i':
                if(!strcmp(optarg, "r")){
                    *channel = RED;
                }
                else if(!strcmp(optarg, "g")){
                    *channel = GREEN;
                }
                else if(!strcmp(optarg, "b")){
                    *channel = BLUE;
                }
                else{
                    *channel = INVALID;
                }
                break;
            case '?':
                if(optopt == 'c' || optopt == 'h' || optopt == 'm' || optopt == 's' || optopt == 'i'){
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                else if (isprint (optopt)){
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                }
                else{
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
                exit(-1);
            default:
                exit(-1);
        }
    }
    return;
}


