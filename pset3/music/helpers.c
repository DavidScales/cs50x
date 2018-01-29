// Helper functions for music

#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    // parse string fraction & return number of eigths
    int numerator = atoi(strtok(fraction, "/"));
    int denominator = atoi(strtok(NULL, "/"));
    return numerator * 8 / denominator;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    // parse note
    // note is YX or YzX, e.g. A#5 or A5
    int length = strlen(note);
    int ascii_offset = 48;
    char letter = note[0];
    char accidental = '\0';
    int octave = 0;
    if (length == 2)
    {
        octave = note[1] - ascii_offset;
    }
    else
    {
        accidental = note[1];
        octave = note[2] - ascii_offset;
    }

    // semitones from A4 to AX
    int octave_diff = (octave - 4) * 12; // some magic music numbers

    // semitones from A4 to Y4
    // use switch since letters are out-of-order & inconsistent in music T.T
    int letter_diff = 0;
    switch(letter) {
        case 'A':
            letter_diff = 0;
            break;
        case 'B'  :
            letter_diff = 2;
            break;
        case 'C'  :
            letter_diff = -9;
            break;
        case 'D'  :
            letter_diff = -7;
            break;
        case 'E'  :
            letter_diff = -5;
            break;
        case 'F'  :
            letter_diff = -4;
            break;
        case 'G'  :
            letter_diff = -2;
            break;
    }

    // semitones offset by accidents
    int accident_diff = 0;
    if (accidental == '#')
    {
        accident_diff = 1;
    }
    else if (accidental == 'b')
    {
        accident_diff = -1;
    }

    // sum semitones
    float semitones = octave_diff + letter_diff + accident_diff;

    // frequency = 2^(n / 12) * 440, where n is # of semitones from A4
    return round(pow(2, semitones / 12) * 440);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    // check if s is empty string, e.g., ""
    if (!strlen(s))
    {
        return true;
    }
    else
    {
        return false;
    }
}
