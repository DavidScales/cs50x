// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// for testing
#include <stdio.h>

// could be a seperate file?
typedef struct _node
{
    char word[LENGTH + 1];
    struct _node *next;
}
node;

node *hashtable[BUCKETS] = {NULL};

// hash function from - http://www.cse.yorku.ca/~oz/hash.html (modified to use const input, be case insensitive, and modulo output)
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
    // get bucket for word, search through linked list in bucket
    unsigned long key = hash(word);
    node *cursor = hashtable[key];
    while (cursor != NULL)
    {
        // if the word is found, return true
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
    // open the dictionary file
    FILE* dictionary_file = fopen(dictionary, "r");
    if (!dictionary_file)
    {
        printf("Couldn't open %s\n", dictionary);
        return false;
    }

    // build the hashtable (globally defined)
    char word[LENGTH + 1];
    unsigned long key;
    while (fscanf(dictionary_file, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            // unload
            return false;
        }
        strcpy(new_node->word, word);
        key = hash(word);
        new_node->next = hashtable[key];
        hashtable[key] = new_node;
    }
    // investigate hash table distribution?
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    // check with valgrind
    return false;
}
