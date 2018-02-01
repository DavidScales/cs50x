// #include <cs50.h>
// #include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

int main(void)
{
    FILE* writeme = fopen("writeme.txt", "w"); // r, w, a
    if (!writeme)
    {
        printf("Couldn't create file\n");
        return 1;
    }

    FILE* readme = fopen("readme.txt", "r");
    if (!readme)
    {
        printf("Couldn't find file\n");
        return 1;
    }

    char ch;
    while ((ch = fgetc(readme)) != EOF)
    {
        printf("%c",ch);
        fputc(ch, writeme);
    }

    // ---

    FILE* sample = fopen("sample.txt", "r"); // r, w, a
    if (!sample)
    {
        printf("Couldn't create file\n");
        return 1;
    }

    // in the stack
    char stack_buffer[5];
    fread(stack_buffer, sizeof(char), 5, sample);
    printf("%c\n", stack_buffer[0]);

    // in the head
    char* heap_buffer = malloc(sizeof(char) * 5);
    fread(heap_buffer, sizeof(char), 5, sample);
    printf("%c\n", heap_buffer[0]);

    // like fgetc
    char x;
    fread(&x, sizeof(char), 1, sample);
    printf("%c\n", x);

    // char ptr[5];
    // while(fread(ptr, 5, 1, in) == 1)
    // {
    //     fwrite(ptr, 5, 1, out);
    // }

    fclose(writeme);
    fclose(readme);
    fclose(sample);

    // see also
    // fgets, fputs, fprintf, fseek, ftell, foef, ferror

    return 0;
}