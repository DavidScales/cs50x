// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>

#include "dictionary.h"

// Define linked list (could be a seperate file)
typedef struct _node
{
    char word[LENGTH + 1];
    struct _node *next;
}
node;

// Init global variables
unsigned int num_words = 0;
node *hashtable[BUCKETS] = {NULL};

// Hashes a word into hashtable key
// from - http://www.cse.yorku.ca/~oz/hash.html (modified to use const input, be case insensitive, and modulo output)
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(c);

    return hash % BUCKETS;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Get bucket key from word, search through linked list in bucket
    unsigned long key = hash(word);
    node *cursor = hashtable[key];
    while (cursor != NULL)
    {
        // If the word is found, return true
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE* dictionary_file = fopen(dictionary, "r");
    if (!dictionary_file)
    {
        printf("Couldn't open %s\n", dictionary);
        return false;
    }

    // Build the hashtable (globally defined)
    char word[LENGTH + 1];
    unsigned long key;
    while (fscanf(dictionary_file, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        strcpy(new_node->word, word);
        key = hash(word);
        new_node->next = hashtable[key];
        hashtable[key] = new_node;
        num_words++; // keeping track of word count here
    }

    fclose(dictionary_file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // For investigating hashtable distribution
    // for (int i = 0; i < BUCKETS; i++)
    // {
    //     printf("Bucket %i: ", i);
    //     int count = 0;
    //     node *cursor = hashtable[i];
    //     while (cursor != NULL)
    //     {
    //         count++;
    //         cursor = cursor->next;
    //     }
    //     printf("%i words\n", count);
    // }
    return num_words; // ;)
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Traverse linked list in each bucket, freeing nodes along the way
    for (int i = 0; i < BUCKETS; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    // [DONE] checked with valgrind
    return true;
}
