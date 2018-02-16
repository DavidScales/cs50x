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
    char *infile_name = argv[1];

    // open input file
    FILE *forensice_image_file = fopen(infile_name, "r");
    if (forensice_image_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile_name);
        return 2;
    }

    // read 512B chunks into a buffer & check for JPEG signature at the beginning of chunk

    int image_count = 0;
    FILE *jpeg_file = NULL;
    char jpeg_filename[8];
    BYTE buffer[512];

    while(fread(&buffer, 1, 512, forensice_image_file) > 0) // last chunk could be < 512
    {
        // if this chunk marks a new image (JPEG signature 0xff 0xd8 0xff 0xe*)
        if (buffer[0] == 255 && buffer[1] == 216 && buffer[2] == 255 && buffer[3] >= 224 && buffer[3] <= 239)
        {
            // if we're already reading an existing JPEG, close that file & increment image count
            if (jpeg_file) {
                fclose(jpeg_file);
                image_count++;
            }
            // open a new file
            sprintf(jpeg_filename, "%03i.jpg", image_count);
            jpeg_file = fopen(jpeg_filename, "w");
            if (jpeg_file == NULL)
            {
                fprintf(stderr, "Could not open %s.\n", jpeg_filename);
                return 2;
            }
        }
        if (jpeg_file)
        {
            fwrite(buffer, 1, 512, jpeg_file);
        }
    }
    if (jpeg_file)
    {
        fclose(jpeg_file);
    }
    return 0;
}
