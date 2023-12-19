#ifndef TRIE_H
#define TRIE_H

#include <limits.h>
#include "global.h"

typedef struct Trie {
    struct Trie *children[CHAR_MAX+1];
    TokenType type;
} Trie;

TokenType retrieveTokenType(Trie *root, const char *word);

Trie *generateTrie(void);

void freeTrie(Trie *token);

#endif
