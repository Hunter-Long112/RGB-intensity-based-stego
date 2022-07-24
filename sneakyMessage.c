#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void printUsage(char** argv);
void parseArgs(int argc, char** argv, int* channel, int* mode, char** coverFile, char** messageFile, char** stegoFile);

#define programName "Sneaky Message"

/*
 * main: driver function for Sneaky Message program
 */
int main(int argc, char** argv){

    // if no arguements are passed in, print usage statement
    if(argc == 1){
        printUsage(argv);
    }

    // get arguements from command line
        // -c = cover image file
        // -h = message file
        // -m = mode (encode / decode) 0 -> encode, 1 -> decode, 2 -> invalid
        // -s stego file
        // -i = indicator channel (r, g, or b) 0 -> R, 1 -> G, 2 -> B, 3 -> invalid
    int* channel = (int*)malloc(sizeof(int));
    int* mode = (int*)malloc(sizeof(int));
    char** coverFile = (char**)malloc(sizeof(char*));
    char** messageFile = (char**)malloc(sizeof(char*));
    char** stegoFile = (char**)malloc(sizeof(char*));
    parseArgs(argc, argv, channel, mode, coverFile, messageFile, stegoFile);
    // printf("channel = %d\nmode = %d\ncoverFile = %s\nmessageFile = %s\nstegoFile = %s\n", *channel, *mode, *coverFile, *messageFile, *stegoFile);


    // inital error check based on command line arguements
    if(*mode == 0){
        if(access(*coverFile, R_OK|W_OK) != 0){
            printf("Cover file validation failed, check that file exists and has read/write permissions.\n");
            exit(-1);
        }
        if(access(*messageFile, R_OK) != 0){
            printf("Message file validation failed, check that file exists and has read permissions.\n");
            exit(-1);
        }
        if(*channel == 3){
            printf("Invalid indicator channel selected, please choose r, g, or b.\n");
            exit(-1);
        }
    }
    else if(*mode == 1){
        if(access(*stegoFile, R_OK) != 0){
            printf("Stego file validation failed, check that file exists and has read permissions.\n");
            exit(-1);
        }
        if(*channel == 3){
            printf("Invalid indicator channel selected, please choose r, g, or b.\n");
            exit(-1);
        }
    }
    else if(*mode == 2){
        printf("Invalid mode selected, please choose encode or decode.\n");
        exit(-1);
    }
    else{
        printf("Something went wrong!\n");
        exit(-1);
    }

    // call functions based on args
    if(*mode == 0){
        //hide message
        // printf("channel = %d\nmode = %d\ncoverFile = %s\nmessageFile = %s\nstegoFile = %s\n", *channel, *mode, *coverFile, *messageFile, *stegoFile);
        return 0;
    }
    else if(*mode == 1){
        //extract message
        // printf("channel = %d\nmode = %d\ncoverFile = %s\nmessageFile = %s\nstegoFile = %s\n", *channel, *mode, *coverFile, *messageFile, *stegoFile);
        return 0;
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
 */
void parseArgs(int argc, char** argv, int* channel, int* mode, char** coverFile, char** messageFile, char** stegoFile){
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
                    *mode = 0;
                }
                else if(!strcmp(optarg, "decode")){
                    *mode = 1;
                }
                else{
                    *mode = 2;
                }
                break;
            case 's':
                *stegoFile = optarg;
                break;
            case 'i':
                if(!strcmp(optarg, "r")){
                    *channel = 0;
                }
                else if(!strcmp(optarg, "g")){
                    *channel = 1;
                }
                else if(!strcmp(optarg, "b")){
                    *channel = 2;
                }
                else{
                    *channel = 3;
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


