# prints a pyramid of hashtags based on user input

from cs50 import get_int

def main():
    while True:
        height = get_int("Supply a positive integer no larger than 23: ")
        if height >= 0 and height <= 23:
            break
    for steps in range(1, height + 1):
        print(" " * (height - steps), end="")
        print("#" * steps, end="")
        print("  ", end="")
        print("#" * steps)

if __name__ == "__main__":
    main()