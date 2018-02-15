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

    int chunks = 0;
    int image_count = 0;
    // FILE *outptr = NULL;
    int temp_started = 0;
    char filename[8];
    BYTE buffer[512];

    while(fread(&buffer, 512, 1, inptr) != 0)
    {
        // JPEG signature 0xff 0xd8 0xff 0xe*
        if (buffer[0] == 255 && buffer[1] == 216 && buffer[2] == 255 && buffer[3] >= 224 && buffer[3] <= 239)
        {
            if (temp_started)
            {
                sprintf(filename, "%03i.jpg", image_count);
                printf("%i bytes in %s\n", chunks * 512, filename);
                chunks = 0;
                image_count++;
            }
            else
            {
                temp_started = 1;
                printf("first image found %i bytes into disk\n", chunks * 512);
                chunks = 1;
            }

            // if outptr is isnt null, write previous file, open a new one
            // if outptr is null (first image), just open a new one

            // int *ptr = malloc();
            // write to file
            // free(ptr);

            // FILE *outptr = fopen("a.jpg", "w");
            // if (outptr == NULL)
            // {
            //     fprintf(stderr, "Could not open %s.\n", "a.jpg");
            //     return 2;
            // }
            // fwrite(inptr, 512 * chunks, 1, outptr);
        }
        chunks++;
    }
    sprintf(filename, "%03i.jpg", image_count);
    printf("%i bytes in %s\n", chunks * 512, filename);

    // Once I have the JPEG's ID'ed, I need to read files into a dynamic buffer
    // and then write them to files

    // success
    return 0;
}
