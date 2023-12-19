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
	INDENTION,
	COMMA,				// ,
	EXPR_BEGIN,			// (
	EXPR_TERMINATE,		// )
	
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

	STR_DELIMITER,		// "
	COMMENT,			// ~
	STRING,				// string data excluding ""

	IDENTIFIER,
	RESERVED_WORD,
	
	INTEGER,			// [0-9]+
	DECIMAL				// ([0-9])*.[0-9]+
} TokenType;

static char TokenTypeStr[29][16] = {
	"SENTENCE_BREAK", "INDENTION", "COMMA", "EXPR_BEGIN",
	"EXPR_TERMINATE", "PLUS", "MINUS", "MULTIPLY",
	"MODULO", "RAISE", "OR", "AND", "DIVIDE",
	"ASSIGNMENT", "LESS_THAN", "GREATER_THAN", "NOT",
	"FLOOR_DIVIDE", "EQUALITY", "LT_EQUAL", "GT_EQUAL",
	"NOT_EQUAL", "STR_DELIMITER", "COMMENT", "STRING",
	"IDENTIFIER", "RESERVED_WORD", "INTEGER", "DECIMAL"
};

static char RESERVED_WORDS[18][10] = {
	"FOR", "TO", "BY", "DO", "WHILE",
	"IF", "ELSE", "THEN", "INPUT", "OUTPUT",
	"INTEGER", "STRING", "DECIMAL", "BOOLEAN",
	"TRUE", "FALSE",
	"SET", "AS"
};

static char SYMBOLS[20][3] = {
	",", "(", ")", "+", "-", "*",
	"%", "^", "|", "&", "/", "=",
	"<", ">", "!", "//", "==",
	"<=", ">=", "!="
};

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
