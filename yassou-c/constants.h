#ifndef CONST_H
#define CONST_H

#include <stdbool.h>

// extern bool debugging;

#define DEBUG_MSG(str) if (debugging) printf("%s", str);

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

#endif
