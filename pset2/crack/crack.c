// TODO - top level comment

// TODO - review these
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define _XOPEN_SOURCE // TODO - what is this?
#include <unistd.h>
#include <crypt.h>
#include <math.h>

void generate_plaintext(int length);

int main(int argc, char *argv[])
{
    // validate number of CL args

    if (argc != 2)
    {
        printf("Error: Incorrect number of arguments\nUsage: ./crack hash\n");
        return 1;
    }

    // get hash & salt from input

    // TODO - What's the deal with pointers again?
    char *hash = argv[1]; // assuming valid input :O
    printf("hash: %s\n", hash);
    char salt[] = { hash[0], hash[1], '\0' };
    printf("salt: %s\n", salt);

    // original password completely alphabetical, 5 or less characters
    // for a set of possible passwords (generate up to 5 alpha chars, case sensitive)
        // hash each & compare to the input hash

    // TESTING
    // int comparison = strcmp(hash, crypt("hello", salt));
    // printf("test comparision: %i\n", comparison);

    // TESTING
    generate_plaintext(3);

    // note - this would obviously be better if we started with common passwords or dictionary words

    // print password
    // return 0
    printf("IGNORE: %s\n", argv[1]);
}

    // string hash = crypt(password, "aa");
    // printf("hash:%s\n", hash);

    // TEMP hash aaPwJ9XL9Y99E maps to hello

void generate_plaintext(int length)
{
    // generate starting word - TODO: how to abstract into function w/scope?

    char word[length + 1];
    for (int k = 0; k < length; k++)
    {
        word[k] = 'a';
    }
    word[length] = '\0';
    printf("word: %s\n", word);
    printf("length: %lu\n", strlen(word));

    // cycle through word possibilities
    // doing this like you would code a UI timer - use a counter & convert the value to positions on a clock

    int lowercase_offset = 97;
    int num_letters = 26;
    int count = 0;
    int total = pow(num_letters, length);
    int value_at_pos;
    int counts_for_pos;

    for (int i = 0; i < total; i++)
    {
        count = i;

        for (int pos = 0; pos < length; pos++)
        {
            counts_for_pos = pow(num_letters, (length - pos) - 1);
            value_at_pos = count / counts_for_pos;
            word[pos] = value_at_pos + lowercase_offset;
            count -= counts_for_pos * value_at_pos;
        }

        printf("%i: %s\n", i, word);

        // duplicate for uppercase
    }
}

// 97 - 122 lowercase