#include <cs50.h>
#include <stdio.h>
#include <math.h>

int leading_digits(long long card, int num_leading_digits);

int main(void)
{
    // prompt user for card number
    string prompt = "Card number: ";
    long long card = get_long_long("%s", prompt);

    // validate positive non-zero
    if (card <= 0) {
        printf("INVALID\n");
        return 0;
    }

    // validate length & starting digits
    int total_num_digits = floor(log10(llabs(card))) + 1;
    int leading = leading_digits(card, 2);
    string type;

    if (total_num_digits == 15 && (leading == 34 || leading == 37))
    {
        type ="AMEX\n";
    }
    else if (total_num_digits == 16 && (leading >= 51 && leading <= 55))
    {
        type ="MASTERCARD\n";
    }
    else if ((total_num_digits == 16 || total_num_digits == 13) && (leading >= 40 && leading <= 49))
    {
        type ="VISA\n";
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }

    // validate checksum

    // print status

    // return 0;
}

// returns leading digits
int leading_digits(long long card, int num_leading_digits) {
    // probably a better way to do this, but C is... pretty annoying ;)
    // modulo doesn't appear to be working with long long data type
    while (card > pow(10, num_leading_digits))
    {
        card /= 10;
    }
    return card;
}

