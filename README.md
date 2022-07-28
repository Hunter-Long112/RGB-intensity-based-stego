# RGB-intensity-based-stego

## Contributers:
Hunter Long (mia014)
Thomas White (yhy312)
Michael Ginsberg (sfi208)

## ToDo:
+ General:
    + come up with / implement partion scheme (function that decides how many bits to hide in a given pixel channel)
    + read in file header, validate that it is a 24 bit bmp, find offset to pixels
        + note to future Hunter: finish validation, read important info into bmpData struct, next step make loop to iterate through pixels 
        (struct for individual pixel data)
    + struct to store file header
    + struct to store a give pixel
    + read / write bits to file from / in a given pixel
    + print the name of the file generated to stdout
    + before implementing partition scheme, make it so user can specify number of bits to hide, then once partition is implemented it becomes optional and defaults to partition

+ Hiding:
    + Figure out way to embed the length of hidden message in cover file w/ hidden message (I think we can just add the length of the hidden message in bits to the beginning of the hidden message in a standardized way)
    + The actual hiding loop

+ Extracting:
    + Extraction loop

+ Possible features once hiding and extraction are working:
    + encryption of message
    + option to randomize indicator channel with rc4 key stream (for now the user just selects an indicator channel)


+ each row of pixels have to be divisable by for, check for padding (number of bytes in row have to be divisible by 4)
+ each pixel is 3 bytes ONLY 
+ can stipulate file size



user chooses indicator channel arbitrarily > choose lower of the two non indicator channels to hide in > LSB of indicator channel indicates which channel is hidden in, if it is 0, the channel after it in the cycle is hidden in, if it is 1 it is the channel before it
The Cycle: . . .R G B R G B. . . 

+ dont forget to check for pading bytes at the end of each horizontal row of pixels

+ number of bits to embed in seletced channels is hardcoded 3 for now
+ need to make it passed in as parameter
+ then partition scheme for dynamic decision making

+ implement logic for padding bytes
