#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "error.h"
#include "lexer.h"

bool debugging = true;

Token *createToken(TokenType type, Position position, char *value) {
	Token *token = (Token*)calloc(1, sizeof(Token));
	MEMCHECK;

	token->type = type;
	token->offset = 
	token->value = value;

	return token;
}

Token *appendToken(Token *last, Token *token) {
	if (last == NULL)
		last->next = token;
	token->previous = last;
	return token;
}

void handleCharacter(current, cursor, symtable) {
	Token *last = symtable;

	if (current == '\n')
		last = createToken(SENTENCE_BREAK, cursor, last);
}

Token *tokenize(FILE *input_file) {
	Token *symtable;
	Position *cursor = (Position*)malloc(1, sizeof(Position));
	MEMCHECK;

	cursor->row = 1;
	cursor->column = 1;

	char current;
	for (;(current = fgetc(input_file)) != EOF; cursor->column++) {
		printf("%c\t%d\n", current, cursor->column+1);
		handleCharacter(current, cursor, symtable);
	}

	DEBUG_MSG("DEBUG: Successfully created symtable.\n");

	return symtable;
}

int main(void) {
	//Temporary
	FILE *file = fopen("test.yass", "r");
	Token *symtable = tokenize(file);
	fclose(file);
}
