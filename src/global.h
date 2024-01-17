#ifndef CONST_H
#define CONST_H

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"

typedef enum TokenType {
	INVALID = 0,
	SENTENCE_BREAK,		// NEWLINE
	INDENT,
	DEDENT,

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
	
	INTEGER,			// [0-9]+
	DECIMAL,				// ([0-9])*.[0-9]+

	IDENTIFIER,

	RESERVED_FOR,
	RESERVED_TO,
	RESERVED_BY,
	RESERVED_DO,
	RESERVED_WHILE,

	RESERVED_IF,
	RESERVED_ELSE,
	RESERVED_THEN,

	RESERVED_INPUT,
	RESERVED_OUTPUT,

	RESERVED_INTEGER,
	RESERVED_STRING,
	RESERVED_DECIMAL,
	RESERVED_BOOLEAN,
	
	RESERVED_TRUE,
	RESERVED_FALSE,

	RESERVED_SET,
	RESERVED_AS,

} TokenType;

static char TokenTypeStr[50][30] = {
	"INVALID", "SENTENCE_BREAK", "INDENT",
	"DEDENT", "COMMA", "EXPR_BEGIN",
	"EXPR_TERMINATE", "PLUS", "MINUS",
	"MULTIPLY", "MODULO", "RAISE",
	"OR", "AND", "DIVIDE", "ASSIGNMENT",
	"LESS_THAN", "GREATER_THAN", "NOT",
	"FLOOR_DIVIDE", "EQUALITY", "LT_EQUAL",
	"GT_EQUAL", "NOT_EQUAL", "STR_DELIMITER",
	"COMMENT", "STRING", "INTEGER", "DECIMAL", 
	"IDENTIFIER", "RESERVED_FOR", "RESERVED_TO",
	"RESERVED_BY", "RESERVED_DO", "RESERVED_WHILE",
	"RESERVED_IF", "RESERVED_ELSE", "RESERVED_THEN",
	"RESERVED_INPUT", "RESERVED_OUTPUT",
	"RESERVED_INTEGER",	"RESERVED_STRING",
	"RESERVED_DECIMAL", "RESERVED_BOOLEAN",
	"RESERVED_TRUE", "RESERVED_FALSE",
	"RESERVED_SET", "RESERVED_AS"

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

typedef enum Grammar {
	INVALID_SYNTAX = 0,
	SENTENCE_END,		// NEWLINE
	INDENT_DEL,
	DEDENT_DEL,

	COMMA_DEL,				// ,
	EXPR_BEGIN_DEL,			// (
	EXPR_TERMINATE_DEL,		// )
	
	PLUS_OP,				// +
	MINUS_OP,				// -
	MULTIPLY_OP,			// *
	MODULO_OP,				// %
	RAISE_OP,				// ^
	OR_OP,					// |
	AND_OP,				// &
	
	DIVIDE_OP,				// /
	ASSIGNMENT_OP,			// =
	LESS_THAN_OP,			// <
	GREATER_THAN_OP,		// >
	NOT_OP,				// !

	FLOOR_DIVIDE_OP,		// //
	EQUALITY_OP,			// ==
	LT_EQUAL_OP,			// <=
	GT_EQUAL_OP,			// >=
	NOT_EQUAL_OP,			// !=

	STR_DEL,		// "
	COMMENT_DEL,			// ~
	STRING_LITERAL,				// string data excluding ""
	
	INTEGER_LITERAL,			// [0-9]+
	DECIMAL_LITERAL,				// ([0-9])*.[0-9]+

	IDENTIFIER_VAR,

	FOR_RES,
	TO_RES,
	BY_RES,
	DO_RES,
	WHILE_RES,

	IF_RES,
	ELSE_RES,
	THEN_RES,

	INPUT_RES,
	OUTPUT_RES,

	INTEGER_RES,
	STRING_RES,
	DECIMAL_RES,
	BOOLEAN_RES,

	STRING,
	
	TRUE_LIT,
	FALSE_LIT,

	SET_RES,
	AS_RES,

	STATEMENTS,
	DECLARATION_STATEMENT,
	ASSIGN_STATEMENT,
	INPUT_STATEMENT,
	OUTPUT_STATEMENT,
	CONDITIONAL_STATEMENT,
	ITERATION_STATEMENT,
	BLANK_STATEMENT,
	SUB_STATEMENTS,
	
	VARIABLES,
	VAR_TYPE,
	VARIABLE,
	VALUES,

	INPUT,
	OUTPUT,

	INITIAL_COND,
	MEDIAL_COND,
	FINAL_COND,
	PREDICATE_COND,

	ASSIGN_EXPR,

	EXPRESSION,
	COMPARAND,
	COMPARISON,
	TERM,
	ADD_SUB_OP,
	FACTOR,
	MULT_DIV_OP,
	OPERAND,
	EXP_OP,
	NUMBER,
	BOOLEAN
	
	//Insert other nonterminals here
} Grammar;

static char GrammarStr[101][30] = {
	"INVALID_SYNTAX", "SENTENCE_END", "INDENT_DEL",
	"DEDENT_DEL", "COMMA_DEL", "EXPR_BEGIN_DEL",
	"EXPR_TERMINATE_DEL", "PLUS_OP", "MINUS_OP",
	"MULTIPLY_OP", "MODULO_OP", "RAISE_OP",
	"OR_OP", "AND_OP", "DIVIDE_OP", "ASSIGNMENT_OP",
	"LESS_THAN_OP", "GREATER_THAN_OP", "NOT_OP",
	"FLOOR_DIVIDE_OP", "EQUALITY_OP", "LT_EQUAL_OP",
	"GT_EQUAL_OP", "NOT_EQUAL_OP", "STR_DEL",
	"COMMENT_DEL", "STRING_LITERAL", "INTEGER_LITERAL",
   	"DECIMAL_LITERAL", "IDENTIFIER_VAR", "FOR_RES",
	"TO_RES", "BY_RES", "DO_RES", "WHILE_RES",
	"IF_RES", "ELSE_RES", "THEN_RES",
	"INPUT_RES", "OUTPUT_RES", "INTEGER_RES",
	"STRING_RES", "DECIMAL_RES", "BOOLEAN_RES", "STRING"
	"TRUE_RES", "FALSE_RES", "SET_RES", "AS_RES",
	"STATEMENTS", "DECLARATION_STATEMENT", "ASSIGN_STATEMENT",
	"INPUT_STATEMENT", "OUTPUT_STATEMENT", "CONDITIONAL_STATEMENT",
	"ITERATION_STATEMENT", "BLANK_STATEMENT", "SUB_STATEMENTS",
	"VARIABLES", "VAR_TYPE", "VARIABLE", "VALUES",
	"INPUT", "OUTPUT", "INITIAL_COND", "MEDIAL_COND", "FINAL_COND",
	"PREDICATE_COND", "ASSIGN_EXPR", "EXPRESSION", "COMPARAND",
	"COMPARISON", "TERM", "ADD_SUB_OP", "FACTOR", "MULT_DIV_OP",
	"OPERAND", "EXP_OP", "NUMBER", "BOOLEAN"
};

typedef struct Position {
	unsigned int row, column, offset;
} Position;

typedef struct Token {
	TokenType type;
	//int offset;						//0-based offset from file
	size_t length;					//string length
	Position location;
	struct Token *previous, *next;	//doubly-linked list
} Token;

typedef struct ParseNode {
    Grammar grammar;
	size_t length;
	Position location;				//same from the token
	struct ParseNode *first_child;
	struct ParseNode *next_sibling;
} ParseNode;

typedef struct Interpreter {
	bool debugging;					//debugging file and stderr
	Token *symtable;				//token list
	ParseNode *parse_tree;
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
