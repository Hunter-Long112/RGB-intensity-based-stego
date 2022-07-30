# RGB-intensity-based-stego

## Contributers:
Hunter Long (mia014)
Thomas White (yhy312)
Michael Ginsberg (sfi208)

## ToDo:
+ validate that image horizontal dimension is divisible by 4, if not error out
+ make number of bits to hide in each byte a cmd line arg
+ update readMe so it's an actual readMe, not a ToDo list
+ finish extraction

## Future Improvements:
+ Implement partition scheme to dynamically decide how many bits to hide in a given byte (program will default to this if user does not pass in a number of bits to hide)
+ Implement logic to deal with padding bytes if cover image horizontal dimension is not divisible by 4, so any size cover image can be used
+ encryption of message
+ option to randomize indicator channel with rc4 key stream (for now the user just selects an indicator channel)
