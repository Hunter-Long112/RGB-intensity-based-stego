# RGB-intensity-based-stego

## Contributers:
Hunter Long (mia014)
Thomas White (yhy312)
Michael Ginsberg (sfi208)

## ToDo:
+ General:
    + come up with / implement partion scheme (function that decides how many bits to hide in a given pixel channel)
    + read in file header, validate that it is a 24 bit bmp, find offset to pixels
    + struct to store file header
    + struct to store a give pixel
    + read / write bits to file from / in a given pixel
    + print the name of the file generated to stdout

+ Hiding:
    + Figure out way to embed the length of hidden message in cover file w/ hidden message (I think we can just add the length of the hidden message in bits to the beginning of the hidden message in a standardized way)
    + The actual hiding loop

+ Extracting:
    + Extraction loop

+ Possible features once hiding and extraction are working:
    + encryption of message
    + option to randomize indicator channel with rc4 key stream (for now the user just selects an indicator channel)
