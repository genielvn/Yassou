#ifndef CONST_H
#define CONST_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"

typedef enum TokenType {
	SENTENCE_BREAK,		// NEWLINE
	INDENT,
	COMMA,				// ,
	EXPR_BEGIN,			// (
	EXPR_TERMINATE,		// )
	STR_DELIMITER,		// "
	
	PLUS,				// +
	MINUS,				// -
	MULTIPLY,			// *
	MODULO,				// %
	RAISE,				// ^
	OR,					// |
	AND,				// &
	
	DIVIDE,				// /
	ASSIGNMENT,			// =
	LESS_THAN,			// <
	GREATER_THAN,		// >
	NOT,				// !

	FLOOR_DIVIDE,		// //
	EQUALITY,			// ==
	LT_EQUAL,			// <=
	GT_EQUAL,			// >=
	NOT_EQUAL,			// !=

	COMMENT,			// ~
	STRING,				// string data excluding ""

	IDENTIFIER,
	RESERVED_WORD,
	
	INTEGER,			// [0-9]+
	DECIMAL				// ([0-9])*.[0-9]+
} TokenType;

typedef struct Position {
	unsigned int row, column, offset;
} Position;

typedef struct Token {
	TokenType type;
	int offset;						//0-based offset from file
	size_t length;					//string length
	struct Token *previous, *next;	//doubly-linked list
	Position location;
} Token;

typedef struct Interpreter {
	bool debugging;					//debugging file and stderr
	Token *symtable;				//token list
	FILE *file;						//input file (must be .yass)
	char *file_name;
} Interpreter;

extern Interpreter *interpreter;

#define DEBUG_MSG(str, ...) \
	do { \
		if (interpreter->debugging) \
			printf("DEBUG: " str " \n", ##__VA_ARGS__); \
	} while (0);

#endif
