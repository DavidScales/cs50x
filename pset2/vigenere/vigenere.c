// Encrypts plaintext into ciphertext using the Vigenere encryption algorithm

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char shifted(char text_letter, int key_letter);

int main(int argc, string argv[])
{
    // validate number of CL args

    if (argc != 2)
    {
        printf("Error: Incorrect number of arguments\nUsage: ./ceasar key\n");
        return 1;
    }

    // validate key input

    string key = argv[1];
    int key_length = strlen(key);
    for (int i = 0; i < key_length; i++)
    {
         if (!isalpha(key[i]))
        {
            printf("Error: 'key' argument must be an alphabetical string\nUsage: ./ceasar key\n");
            return 1;
        }
    }

    // get plaintext

    string plaintext = get_string("plaintext: ");

    // encrypt

    int text_length = strlen(plaintext);
    char ciphertext[text_length + 1]; // remember \0 null terminator!
    char text_letter;
    char key_letter;
    int key_pos = 0;

    for (int i = 0; i <= text_length; i++) // remember \0 null terminator!
    {
        text_letter = plaintext[i];
        if (isalpha(text_letter))
        {
            key_letter = key[key_pos];
            ciphertext[i] = shifted(text_letter, key_letter);
            key_pos = (key_pos + 1) % key_length;
        }
        // else non-alphabetical - preserve
        else
        {
            ciphertext[i] = text_letter;
        }
    }

    // output ciphertext

    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

char shifted(char text_letter, int key_letter)
{
    int upperAsciiStart = 65;
    int lowerAsciiStart = 97;
    int offset;
    if (isupper(text_letter))
    {
        offset = upperAsciiStart;
    }
    else
    {
        offset = lowerAsciiStart;
    }

    // convert from ascii to alphabetical index
    text_letter -= offset;

    // shift
    int shift = tolower(key_letter) - lowerAsciiStart;
    text_letter = (text_letter + shift) % 26;

    // convert back from alphabetical index to ascii
    text_letter += offset;

    return text_letter;
}