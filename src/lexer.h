#ifndef LEXER_H

#define LEXER_H
#include <stddef.h>
#include <stdbool.h>
#include "global.h"
#include "trie.h"

typedef struct IndentNode {
	int data;
	struct IndentNode *next;
} IndentNode;

typedef struct Lexer {
	FILE *file;
	char current;
	Position cursor;
	Token *symtable, *last;
	bool indent;
	IndentNode *indent_stack;
	State *trie;
} Lexer;

Token *tokenize(FILE *input_file);

void freeSymTable(Token *token);

#endif
