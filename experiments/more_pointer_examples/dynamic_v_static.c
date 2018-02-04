#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char runtime_intput = argv[1][0];
    int size = atoi(&runtime_intput);
    char arr[size];
    arr[0] = 'x';
    printf("%c\n", arr[0]);
    return argc;
}

// DYNAMIC DOESN"T MEAN AT RUNTIME
// https://stackoverflow.com/questions/8385322/difference-between-static-memory-allocation-and-dynamic-memory-allocation