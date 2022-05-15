// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

#define HASHTABLE 676 // 26*26 = 676

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = HASHTABLE;

// Number of words in dictionary
unsigned int count = 0;

// Hash table
node *table[HASHTABLE];

node *CreateNode(const char *word);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int hash_index1 = hash(word);
    node *temp = table[hash_index1];
    while (temp != NULL)
    {
        if (!strcasecmp(temp->word, word))
        {
            return true;
        }
        temp = temp->next;
    }
    // TODO
    return false;
}

// Hashes word to a number looking into 2 first letters
unsigned int hash(const char *word)
{
    
    char letter1 = *word;
    char letter2 = *(word + 1);
    
    // if only one letter
    if (letter2 == '\n' || letter2 == '\0' || letter2 == '\'') 
    {
        letter2 = letter1;
    }
    
    if (isupper(letter1))
    {
        letter1 = tolower(letter1);
    }
    if (isupper(letter2))
    {
        letter2 = tolower(letter2);
    }

    
    return (((int)(letter1) + (int)(letter2)) - 194 + ((int)(letter1) - 97) * 25);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // sets hash table array to NILL
    for (int i = 0; i < N ; i++)
    {
        table[i] = NULL;
    }
    
    // open dictionary for read
    FILE *read = fopen(dictionary, "r");
    if (read == NULL)
    {
        printf("Unable to open dictionary.");
        return false;
    }
    // prepare to read dictionary
    char word[LENGTH + 1];
    char c;
    int index = 0;
    
    // read dictionary
    while (fread(&c, sizeof(char), 1, read))
    {
        word[index] = c;
        index++;
        
        // compleate word 
        if (c == '\n')
        {
            // end string
            word[--index] = '\0';
            
            unsigned int hash_index = hash(word);
            
            // build hash table
            HashTable(word, hash_index);
            
            // prepare for new word
            index = 0;
        }
    }
   
    // check for errors
    if (ferror(read))
    {
        fclose(read);
        printf("Error reading %s.\n", dictionary);
        return false;
    }
    else
    {
        fclose(read);
        return true;
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // global var numbword is updated every time node is allocated 
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *temp = table[i];
        while (temp != NULL)
        {
            temp = temp->next;
            free(table[i]);
            table[i] = temp;
        }
    }
   
    return true;
}

// allocate node and write data in
node *CreateNode(const char *word)
{
    node *n = malloc(sizeof(node));
    assert(n && "bad malloc");
    
    strcpy(n->word, word);
    n->next = NULL;
    
    return n;
}

// build hash table
void HashTable(char *word, unsigned int hash)
{
    if (table[hash] == NULL)
    {
        table[hash] = CreateNode(word);
        count++;
    }
    else
    {
        node *temp = table[hash];
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = CreateNode(word); 
        count++;
    }
}