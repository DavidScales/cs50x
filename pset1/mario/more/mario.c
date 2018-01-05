#include <stdio.h>
#include <cs50.h>

void print_repeated(string s, int n);
void print_spaces(int n);
void print_blocks(int n);

int main(void)
{
    // promt user, continue until a valid value is given
    int height;
    string prompt = "Supply a positive integer no larger than 23: ";
    do
    {
        height = get_int("%s\n", prompt);
    }
    while (height < 0 || height > 23);

    // loop to build structure
    int steps = 1;
    while (steps <= height)
    {
        print_spaces(height - steps);
        print_blocks(steps);
        print_spaces(2);
        print_blocks(steps);
        printf("\n");
        steps++;
    }
}

// print a string multiple times
void print_repeated(string s, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s", s);
    }
}

// print n spaces
void print_spaces(int n)
{
    print_repeated(" ", n);
}

// print n blocks
void print_blocks(int n)
{
    print_repeated("#", n);
}