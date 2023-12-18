#ifndef LEXER_H

#define LEXER_H
#include <stddef.h>
#include <stdbool.h>
#include "global.h"

typedef struct Lexer {
	FILE *file;
	char current;
	Position cursor;
	Token *symtable, *last;
	bool indent;
} Lexer;

Token *tokenize(FILE *input_file);

#endif
