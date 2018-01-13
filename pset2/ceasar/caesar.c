// Encrypts plaintext into ciphertext using Ceasar's encryption algorithm

// Note: there is a bug such that every so often a capital "B" is added to the end
// of the ciphertext. It's inconsistent. Assuming it has something to do with the
// end of the array?

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char shifted(char letter, int key, int offset);

int main(int argc, string argv[])
{
    // validate number of CL args

    if (argc != 2)
    {
        printf("Error: Incorrect number of arguments\nUsage: ./ceasar key\n");
        return 1;
    }

    // get plaintext

    string plaintext = get_string("plaintext: ");

    // encrypt

    int key = atoi(argv[1]); // assuming valid :O
    int upperAsciiStart = 65;
    int lowerAsciiStart = 97;
    int text_length = strlen(plaintext);
    char ciphertext[text_length];
    char letter;

    for (int i = 0; i < text_length; i++)
    {
        letter = plaintext[i];
        if (isupper(letter))
        {
            ciphertext[i] = shifted(letter, key, upperAsciiStart);
        }
        else if (islower(letter))
        {
            ciphertext[i] = shifted(letter, key, lowerAsciiStart);
        }
        // else non-alphabetical - preserve
        else
        {
            ciphertext[i] = letter;
        }
    }


    // output ciphertext

    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

char shifted(char letter, int key, int offset)
{
    // convert from ascii to alphabetical index
    letter -= offset;

    // shift
    letter = (letter + key) % 26;

    // convert back from alphabetical index to ascii
    letter += offset;

    return letter;
}