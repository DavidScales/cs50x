// Recovers JPEGs from a forensic image
// https://docs.cs50.net/2018/x/psets/4/recover/recover.html

#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover forensic_image\n");
        return 1;
    }

    // cache filename
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // read 512B chunks into a buffer & check for JPEG signature at the beginning of chunk

    int counter = 0;
    BYTE buffer[512];

    while(fread(&buffer, 512, 1, inptr) != 0)
    {
        // JPEG signature 0xff 0xd8 0xff 0xe*
        if (buffer[0] == 255 && buffer[1] == 216 && buffer[2] == 255 && buffer[3] >= 224 && buffer[3] <= 239)
        {
            counter++;
            printf("%i ", buffer[0]);
            printf("%i ", buffer[1]);
            printf("%i ", buffer[2]);
            printf("%i ", buffer[3]);
            printf("\n");
        }
    }
    printf("%i potential JPEGs found\n", counter);

    // Once I have the JPEG's ID'ed, I need to read files into a dynamic buffer
    // and then write them to files

    // success
    return 0;
}
