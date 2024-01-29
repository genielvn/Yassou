#ifndef DEBUG_H
#define DEBUG_H

#include "lexer.h"
#include "parser.h"
#include "global.h"

void printTokens(Lexer *lexer);
void recursivePrint(ParseNode *node, size_t level, FILE *file);
void printParseTree(ParseNode *node);
void test();

#endif