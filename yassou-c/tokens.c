#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
#define SIZE 26
#define CHAR_TO_INDEX(c) ((int)c - (int)'A')

typedef struct Trie
{
    struct Trie *children[SIZE];

    TokenType type;
} Trie;

Trie *createNode()
{
    Trie *node = NULL;

    if (node)
    {
        node->type = IDENTIFIER;
        for (int i = 0; i < SIZE; i++)
            node->children[i] = NULL;
    }

    return node;
}

void insert(Trie *root, const char *key)
{
    int length = strlen(key);
 
    Trie *node = root;
 
    for (int level = 0; level < length; level++)
    {
        int index = CHAR_TO_INDEX(RESERVED_WORDS[level]);
        printf("%d\n", index);
        if (!node->children[index])
            node->children[index] = createNode();
 
        node = node->children[index];
    }
 
    printf("Test insert node");
    // mark last node as leaf
    node->type = RESERVED_WORD;
}

TokenType getType(Trie *root, const char *key)
{
    int length = strlen(key);
    int index;
    Trie *node = root;
 
    for (int level = 0; level < length; level++)
    {
        int index = CHAR_TO_INDEX(key[level]);
 
        if (!node->children[index])
            return false;
 
        node = node->children[index];
    }
 
    return (node->type);
}

void main()
{
    Trie *root = createNode();
    for (int i = 0; i < ARRAY_SIZE(RESERVED_WORDS); i++)
        insert(root, RESERVED_WORDS[i]);
    printf("Test");

    if (getType(root, "SET") == RESERVED_WORD)
    {
        printf("SET is a RESERVED_WORD\n");
    }
}