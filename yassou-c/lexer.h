#ifndef LEXER_H

#define LEXER_H
#include <stddef.h>
#include <stdbool.h>
#include "global.h"
#include "trie.h"

typedef struct Lexer {
	FILE *file;
	char current;
	Position cursor;
	Token *symtable, *last;
	bool indent;
	Trie *trie;
} Lexer;

Token *tokenize(FILE *input_file);

void freeSymTable(Token *token);

#endif
