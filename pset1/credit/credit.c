#include <cs50.h>
#include <stdio.h>
#include <math.h>

int digit_at_position(long long num, int pos);

int main(void)
{
    // prompt user for card number
    string prompt = "Card number: ";
    long long card = get_long_long("%s", prompt);

    // validate positive non-zero
    if (card <= 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // validate checksum

    int total_num_digits = floor(log10(llabs(card))) + 1;
    int sum = 0;
    int doubled;

    // for every other card digit, starting second from the last
    for (int i = 1; i < total_num_digits; i += 2)
    {
        // double the digit
        doubled = digit_at_position(card, i) * 2;

        // if the product has two digits, add both digits to the sum
        if (doubled >= 10)
        {
            sum += doubled / 10 + doubled % 10;
        }
        // otherwise add the single digit to the sum
        else
        {
            sum += doubled;
        }
    }

    // for the remaining card digits
    for (int i = 0; i < total_num_digits; i += 2)
    {
        // add digit to the sum
        sum += digit_at_position(card, i);
    }

    // reject if the last digit of the sum is not zero
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // validate length & starting digits (type)

    int first_digit = digit_at_position(card, total_num_digits - 1);
    int second_digit = digit_at_position(card, total_num_digits - 2);
    int leading = first_digit * 10 + second_digit;

    if (total_num_digits == 15 && (leading == 34 || leading == 37))
    {
        printf("AMEX\n");
    }
    else if (total_num_digits == 16 && (leading >= 51 && leading <= 55))
    {
        printf("MASTERCARD\n");
    }
    else if ((total_num_digits == 16 || total_num_digits == 13) && first_digit == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}

// returns the (int) digit at a particular position - zero-indexed, but starts from right, e.g, 543210
int digit_at_position(long long num, int pos)
{
    long long int truncated_num = num / (long long int) pow(10, pos);
    return (int) (truncated_num % 10);
}

