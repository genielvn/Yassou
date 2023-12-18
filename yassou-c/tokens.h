#include "global.h"

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
#define SIZE (26)
#define CHAR_TO_INDEX(c) ((int)c - (int)'A')

typedef struct Trie
{
    struct Trie *children[SIZE];
    TokenType type;
} Trie;

char RESERVED_WORDS[][18] = {
	"FOR", "TO", "BY", "DO", "WHILE",
	"IF", "ELSE", "THEN", "INPUT", "OUTPUT",
	"INTEGER", "STRING", "DECIMAL", "BOOLEAN",
	"TRUE", "FALSE", 
	"SET", "AS"
};


Trie* getReservedWordTrie();

Trie *reserved_word_trie = getReservedWordTrie();
