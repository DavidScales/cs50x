// Attempts to crack a hashed password (CL input arg) with brute force guess-and-check

// Usage ./crack hash

// Notes:
// * Assumes passwords are 5 alphabetical letters or less
// * Assumes passwords are hashed with C's DES-based crypt function
// * Guess-and-check simply loops through all possible combinations - it would of course
//   be more performant to start with common passwords or dictionary words

// Reference: https://docs.cs50.net/2018/x/psets/2/crack/crack.html

// TODO - review headers
// TODO - abstract into functions?
// TODO - review strings vs char arrays vs points to char arrays

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define _XOPEN_SOURCE
#include <unistd.h>
#include <crypt.h>
#include <math.h>

int main(int argc, char *argv[])
{
    // validate number of CL args

    if (argc != 2)
    {
        printf("Error: Incorrect number of arguments\nUsage: ./crack hash\n");
        return 1;
    }

    // get hash & salt from input

    char *hash = argv[1]; // assuming valid input :O
    char salt[] = { hash[0], hash[1], '\0' }; // salt for crypt is first two letters of hash

    // for passwords of length 1 through 5

    for (int length = 1, max_length = 5; length <= max_length; length++)
    {

        // create a starting word, e.g. "aaa"

        char word[length + 1];
        for (int k = 0; k < length; k++)
        {
           word[k] = 'a';
        }
        word[length] = '\0';

        // cycle through word possibilities
        // doing this like you would code a UI timer - use a counter & convert the value to positions on a clock
        // e.g., 0 == "aaa", 1 == "aab", 2 == "aac", etc.

        int count;
        int num_letters = 26;
        int total = pow(num_letters, length);

        // for each possible combination of letters e.g., "aaa" through "zzz"

        for (int i = 0; i < total; i++)
        {
            count = i;

            // assign letters to each position in the word

            int value_at_pos;
            int counts_for_pos;
            int lowercase_offset = 97;

            for (int pos = 0; pos < length; pos++)
            {
                counts_for_pos = pow(num_letters, (length - pos) - 1);
                value_at_pos = count / counts_for_pos;
                word[pos] = value_at_pos + lowercase_offset;
                count -= counts_for_pos * value_at_pos;
            }

            // hash the word & compare agaisnt user supplied hash

            if (strcmp(hash, crypt(word, salt)) == 0)
            {
                printf("%s\n", word);
                return 0;
            }

            // duplicate for uppercase
        }
    }

    printf("Couldn't decrypt hash :(\n");
    return 2;
}

    // TEMP hash aaPwJ9XL9Y99E maps to hello

    // 97 - 122 lowercase