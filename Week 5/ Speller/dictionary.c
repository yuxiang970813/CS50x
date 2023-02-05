// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Word counter
int word_counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word to obtain hash value
    unsigned int hash_value = hash(word);

    // Access hash table, traverse and compare
    for (node *cursor = table[hash_value]; cursor != NULL; cursor = cursor->next)
    {
        // Return true if find, then break
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
            break;
        }
    }
    return false;
}

// Hashes word to a number
// Reference: https://video.cs50.io/nvzVHwrrub0?start=418
unsigned int hash(const char *word)
{
    unsigned int sum = 0;
    unsigned int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        sum += (tolower(word[i]) - 96);
    }
    return ((sum * len) % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dicionary file
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        return false;
    }

    // Read strings from file
    char word_buffer[LENGTH + 1];
    while (fscanf(input, "%s", word_buffer) != EOF)
    {
        // Create a new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return 1;
        }

        // Copy word_buffer into node
        strcpy(n->word, word_buffer);
        n->next = NULL;

        // Update word counter
        word_counter++;

        // Hash word to obtain hash value
        unsigned int hash_value = hash(word_buffer);

        // Insert node into hash table
        n->next = table[hash_value];
        table[hash_value] = n;
    }
    // Close file
    fclose(input);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate hash table
    for (int i = 0; i < N; i++)
    {
        node *tmp = table[i];

        // Traverse and free memory
        while (tmp != NULL)
        {
            node *cursor = tmp->next;
            free(tmp);

            // Move to next node
            tmp = cursor;
        }
    }
    return true;
}