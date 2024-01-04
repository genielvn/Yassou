#ifndef TRIE_H
#define TRIE_H

#include <limits.h>
#include "global.h"

typedef struct State {
    struct State *children[CHAR_MAX+1];
    TokenType type;
} State;

State *nextState(State *node, const char ch);

State *generateAutomata(void);

void freeAutomata(State *token);

#endif
