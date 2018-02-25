# calculates a credit card checksum

from cs50 import get_int
from math import log10, floor


def main():
    # prompt user for card number
    ccNum = get_int("Card number: ")

    # validate positive non-zero
    if ccNum <= 0:
        print("INVALID")
        return 0

    # validate checksum
    ccList = [int(digit) for digit in str(ccNum)] # probably ineffecient to convert between string, mathematical approach likely better
    everyOtherDigit = ccList[-2::-2] # starting with second to last digit
    everyOtherDigitDoubled = [double(digit) for digit in everyOtherDigit]
    remainingDigits = ccList[-1::-2]
    finalSum = sum(everyOtherDigitDoubled) + sum(remainingDigits)
    if (not finalSum % 10 == 0):
        print("INVALID")
        return 0

    # validate length & starting digits
    numDigits = int(log10(ccNum)) + 1
    leadingDigits = ccList[0] * 10 + ccList[1]
    if numDigits == 15 and (leadingDigits == 34 or leadingDigits == 37):
        print("AMEX")
    elif numDigits == 16 and (leadingDigits >= 51 and leadingDigits <= 55):
        print("MASTERCARD")
    elif (numDigits == 16 or numDigits == 13) and ccList[0] == 4:
        print("VISA")
    else:
        print("INVALID")

    return 0

def double(digit):
    '''doubles the digit and returns the sum of the resulting digits'''
    doubled = digit * 2
    if doubled >= 10:
        return floor(doubled / 10) + doubled % 10
    return doubled


if __name__ == "__main__":
    main()