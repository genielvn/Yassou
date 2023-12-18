#ifndef LEXER_H

#define LEXER_H
#include <stddef.h>
#include "constants.h"

typedef struct Position {
	/* row		= line row
	 * column	= line column
	 * offset	= number of characters */
	unsigned int row, column, offset;
} Position;

typedef struct Token {
	TokenType type;					// refer to constants.h
	unsigned int offset;			// offset
	size_t length;					// character length
	struct Token *previous, *next;
	Location location;	// doubly-linked list
} Token;

typedef struct Lexer {
	FILE *file;
	char current;
	Location cursor;
	Token *symtable;
} Lexer;

typedef struct Location {
	int row;
	int column;
	int offset;
} Location;

Token *tokenize(FILE *input_file);

#endif
