#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "tokens.h"

Trie *createNode()
{
    Trie *node = (Trie*)malloc(sizeof(Trie));
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
        int index = CHAR_TO_INDEX(key[level]);
        printf("%d %c\n", index, key[level]);         
        if (!node->children[index])
            node->children[index] = createNode();
 
        node = node->children[index];
    }
 
    // mark last node as leaf
    node->type = RESERVED_WORD;
}

TokenType getType(Trie *root, const char *key)
{
    int length = strlen(key);
    Trie *node = root;
 
    for (int level = 0; level < length; level++)
    {
        int index = CHAR_TO_INDEX(key[level]);
        printf("%c = %d -> ", key[level], !node->children[index]);
        if (!node->children[index])
            return IDENTIFIER;
 
        node = node->children[index];
    }
    
    return (node->type);
}

Trie* getReservedWordTrie()
{
    Trie *root = createNode();
    for (int i = 0; i < ARRAY_SIZE(RESERVED_WORDS); i++)
    {
        printf("==== %s ====\n", RESERVED_WORDS[i]);
        insert(root, RESERVED_WORDS[i]);
    }

    return root;
}

