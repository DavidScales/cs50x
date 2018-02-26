# Encrypts plaintext into ciphertext using Ceasar's encryption algorithm

import sys
from cs50 import get_string

LOWER_LETTERS = 'abcdefghijklmnopqrstuvwxyz'
UPPER_LETTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
LENGTH = 26

def main(argv):

    # validate CL input

    if not len(argv) == 2:
        print("Error: Incorrect number of arguments\nUsage: python ceasar.py key")
        return 1

    try:
        key = int(argv[1]) # raises ValueError if int() fails
        if key <= 0:
            raise ValueError()
    except ValueError:
        print("Error: Incorrect input - key must be a positive integer\nUsage: python ceasar.py key");
        return 1;

    # get plaintext
    plaintext = get_string("plain text: ")

    # encrypt
    ciphertext = [shifted(letter, key) for letter in plaintext]
    print("ciphertext: {}".format(''.join(ciphertext)))

def shifted(letter, key):
    """Returns a letter shifted within it's alphabet. If the letter is not in the alphabet, returns the input letter"""

    position = LOWER_LETTERS.find(letter)
    if position > -1:
        new_position = (position + key) % LENGTH
        return LOWER_LETTERS[new_position]

    position = UPPER_LETTERS.find(letter)
    if position > -1:
        new_position = (position + key) % LENGTH
        return UPPER_LETTERS[new_position]

    return letter


if __name__ == "__main__":
    main(sys.argv)