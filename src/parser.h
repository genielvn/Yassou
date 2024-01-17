#ifndef PARSER_H

#define PARSER_H
#include "global.h"

typedef struct Parser {
	Token *current;
	ParseNode *root;
} Parser;

ParseNode *parseTokens(Token *token);

#endif
