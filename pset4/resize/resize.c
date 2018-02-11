// Resizes (enlarges) a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // check number of args (assuming correct order of args & valid input)
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy size infile outfile\n");
        return 1;
    }

    // remember size
    int size = atoi(argv[1]);

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // create new BITMAPINFOHEADER for the resized bitmap file
    BITMAPINFOHEADER bi_copy = *(&bi);

    // resize width & height based on user input
    bi_copy.biWidth = bi.biWidth * size;
    bi_copy.biHeight = bi.biHeight * size;

    // determine padding for new bitmap
    int padding_copy = (4 - (bi_copy.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // adjust image size based on new parameters
    bi_copy.biSizeImage = (bi_copy.biWidth * sizeof(RGBTRIPLE) + padding_copy) * -1 * bi_copy.biHeight;

    // create new BITMAPFILEHEADER for the resized bitmap file
    BITMAPFILEHEADER bf_copy = *(&bf);

    // adjust total file size based on new parameters (metadata size + image size)
    bf_copy.bfSize = bf_copy.bfOffBits + bi_copy.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_copy, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_copy, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for reading scanlines in original bitmap
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // enlarge height of outfile by writing each line multiple times
        for (int n = 0; n < size; n++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // enlarge width of outfile by writing duplicate pixels
                for (int k = 0; k < size; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over padding in original bitmap, if any
            fseek(inptr, padding, SEEK_CUR);

            // add appropriate padding to new bitmap
            for (int k = 0; k < padding_copy; k++)
            {
                fputc(0x00, outptr);
            }

            // if it's not the last iteration, seek backwards so we can re-read the infile line again
            if (n < size - 1)
            {
                fseek(inptr, -1 * (padding + bi.biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
            }
        }

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
