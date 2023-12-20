#ifndef TRIE_H
#define TRIE_H

#include <limits.h>
#include "global.h"

typedef struct Trie {
    struct Trie *children[CHAR_MAX+1];
    TokenType type;
} Trie;

Trie *nextTrie(Trie *node, const char ch);

Trie *generateTrie(void);

void freeTrie(Trie *token);

#endif
