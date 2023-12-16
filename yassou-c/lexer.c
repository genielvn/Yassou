#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "error.h"
#include "lexer.h"

bool debugging = true;

void moveCursor(Lexer *lexer, bool next_line) {
	lexer->current = fgetc(lexer->file);
	if (next_line) {
		lexer->row++;
		lexer->column = 1;
	} else {
		lexer->column++;
	}
	lexer->offset++;
}

Token *createToken(TokenType type, Lexer lexer) {
	Token *token = (Token*)calloc(1, sizeof(Token));
	MEMCHECK;

	token->type = type;
	token->location.row = lexer.cursor.row;
	token->location.column = lexer.cursor.column;
	token->location.offset = lexer.cursor.offset;
	token->length = 1;

	return token;
}

void appendToken(Token *token, Lexer *lexer) {
	if (lexer->symtable == NULL) {
		lexer->symtable = token;
		return;
	}

	if (lexer->last != NULL) {
		lexer->last->next = token;
		token->previous = lexer->last;
	}

	lexer->last = token;
}

void handleString(Lexer *lexer) {
	Token *delim1 = createToken(STR_DELIMITER, *lexer);
	appendToken(delim1, lexer);
	Token *string = createToken(STRING, *lexer);

	do {
		string->length++;
		moveCursor(lexer, false);

		if (lexer->current == '\n')
			//Invoke STR_ERROR
		if (lexer->current == '\\') {
			moveCursor(lexer, false);
			continue;
		}
	} while (lexer != "\"");
	appendToken(string, lexer);
	
	Token *delim2 = createToken(STR_DELIMITER, *lexer);
	appendToken(delim2, lexer);
	moveCursor(lexer, false);
}

void handleComment(Lexer *lexer) {
	Token *comment = createToken(COMMENT, *lexer);
	
	do {
		comment->length++;
		moveCursor(lexer, false);	
	} while (lexer->current != '~' || lexer->current != '\n');

	if (lexer->current == '~') {
		comment->length++;
		moveCursor(lexer, false);	
	}

	appendToken(comment, lexer);
}

void handleCommentOrString(Lexer *lexer) {
	if (lexer->current == '\"')
		handleString(lexer);
	else if (lexer->current == '~')
		handleComment(lexer);

	if (lexer->current != '\n' || lexer->current != ' ')
		//Invoke error since this is noconcat
	
	//Return to handleCharacter()
}

void handleNumber(Lexer *lexer) {
	Token *number = createToken(INTEGER, *lexer);
	do {
		if (isdigit(lexer->current)) {
			number->length++;
		} else if (lexer->current == '.' || number->type == INTEGER) {
			number->length++;
			number->type = DECIMAL;
		} else if (lexer->current == '.' || number->type == DECIMAL) {
			//Invoke error, double decimal point
		}
		moveCursor(lexer, false);
		
		if (isalpha(lexer->current) && lexer->current != 'e') {
			//Invoke error, <number><letter> is prohibited unless 'e'
		}
	} while (isdigit(lexer->current) || lexer->current == '.');

	appendToken(number, lexer);
}

void handleWord(Lexer *lexer) {
	Token *word = createToken(IDENTIFIER, *lexer);

	//First check for reserved word
	
	if (islower(lexer->current) || lexer->current == '_')
		goto skip;

	else if (lexer->current == 'A') {
		moveCursor(lexer, false);
		if (lexer->current != 'S')
			goto skip;

		word->type = RESERVED_WORD;		//AS
	} else if (lexer->current == 'B') {
		moveCursor(lexer, false);
		if (lexer->current == 'Y') {
			word->length++;
			moveCursor(lexer, false);
			word->type = RESERVED_WORD;	//BY
			goto skip;
		}

		if (lexer->current != 'O') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'O') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'L') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'E') goto skip;

		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'A') goto skip;

		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'N') goto skip;
		word->length++;
		word->type = RESERVED_WORD;	//BOOLEAN
	} else if (lexer->current == 'D') {
		moveCursor(lexer, false);
		if (lexer->current == 'O') {
			word->length++;
			moveCursor(lexer, false);
			word->type = RESERVED_WORD;	//DO
			goto skip;
		}
			
		if (lexer->current != 'E') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'C') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'I') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'M') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'A') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'L') goto skip;
		word->length++;
		word->type = RESERVED_WORD;	//DECIMAL
	} else if (lexer->current == 'E') {
		moveCursor(lexer, false);
		if (lexer->current != 'L') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'S') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'E') goto skip;
		word->length++;
		word->type = RESERVED_WORD;	//ELSE
	} else if (lexer->current == 'F') {
		moveCursor(lexer, false);
		if (lexer->current == 'A') {
			word->length++;
		
			moveCursor(lexer, false);
			if (lexer->current != 'L') goto skip;
			word->length++;
		
			moveCursor(lexer, false);
			if (lexer->current != 'S') goto skip;
			word->length++;
		
			moveCursor(lexer, false);
			if (lexer->current != 'E') goto skip;
			word->length++;
			word->type = RESERVED_WORD;	//FALSE
		} else if (lexer->current == 'O') {
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'R') goto skip;
			word->length++;
			word->type = RESERVED_WORD;	//FOR
		}
	} else if (lexer->current == 'I') {
		moveCursor(lexer, false);
		if (lexer->current == 'F') {
			word->length++;
			word->type = RESERVED_WORD;	//IF
			moveCursor(lexer, false);
			goto skip;
		}

		if (lexer->current != 'N') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current == 'P') {
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'U') goto skip;
			word->length++;

			if (lexer->current != 'T')
				goto skip;
			word->length++;
			word->type = RESERVED_WORD; //INPUT
		} else if (lexer->current == 'T') {
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'E') goto skip;
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'G') goto skip;
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'E') goto skip;
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'R') goto skip;
			word->length++;
			word->type = RESERVED_WORD; //INTEGER
		} else {
			goto skip;
		}
	} else if (lexer->current == 'O') {
		moveCursor(lexer, false);
		if (lexer->current != 'U') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'T') goto skip;
		word->length++;

		moveCursor(lexer, false);
		if (lexer->current != 'P') goto skip;
		word->length++;

		moveCursor(lexer, false);
		if (lexer->current != 'U') goto skip;
		word->length++;
		
		moveCursor(lexer, false);
		if (lexer->current != 'T') goto skip;
		word->length++;
		word->type = RESERVED_WORD;	//OUTPUT
	} else if (lexer->current == 'S') {
		moveCursor(lexer, false);
		if (lexer->current == 'E') {
			word->length++;
			
			moveCursor(lexer, false);
			if (lexer->current != 'T') goto skip;
			word->length++;
			word->type = RESERVED_WORD;	//SET
		} else if (lexer->current == 'T') {
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'R') goto skip;
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'I') goto skip;
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'N') goto skip;
			word->length++;

			if (lexer->current != 'G') goto skip;
			word->length++;
			word->type = RESERVED_WORD; //STRING
		} else {
			goto skip;
		}
	} else if (lexer->current == 'T') {
		moveCursor(lexer->current);
		if (lexer->current == 'H') {
			word->length++;
			
			moveCursor(lexer, false);
			if (lexer->current != 'E') goto skip;
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'N') goto skip;
			word->length++;
			word->type = RESERVED_WORD;	//THEN
		} else if (lexer->current == 'O') {
			word->length++;
			moveCursor(lexer, false);
			word->type = RESERVED_WORD;	//TO
			goto skip;
		} else if (lexer->current == 'R') {
			word->length++;
			
			moveCursor(lexer, false);
			if (lexer->current != 'U') goto skip;
			word->length++;

			moveCursor(lexer, false);
			if (lexer->current != 'E') goto skip;
			word->length++;
			word->type = RESERVED_WORD; //TRUE
		} else {
			goto skip;
		}
	} else if (lexer->current == 'W') {
		moveCursor(lexer->current);
		if (lexer->current != 'H') goto skip;
		word->length++;

		moveCursor(lexer->current);
		if (lexer->current != 'I') goto skip;
		word->length++;

		moveCursor(lexer->current);
		if (lexer->current != 'L') goto skip;
		word->length++;

		moveCursor(lexer->current);
		if (lexer->current != 'E') goto skip;
		word->length++;
		word->type = RESERVED_WORD; //WHILE
	} else {
		goto skip
	}

	moveCursor(lexer, false);

	skip:
	//Second check for reserved word
	if (!isalnum(lexer->current) && lexer->current != '_') {
		appendToken(word, lexer);	//Word is reserved
		return;
	}

	//identifiers: may fail first check but must fail the second check
	word->type = IDENTIFIER;
	while (isalnum(lexer->current) || lexer->current == '_') {
		word->length++;
	}

	appendToken(word, lexer);
}

void handleSymbol(Lexer *lexer) {
	Token *symbol;	//Can be a symbol or a delimiter

	switch(lexer->current) {
		case ',':
			symbol = createToken(COMMA, *lexer);
			break;
		case '(':
			symbol = createToken(EXPR_BEGIN, *lexer);
			break;
		case ')':
			symbol = createToken(EXPR_TERMINATE, *lexer);
			break;
		case '+':
			symbol = createToken(PLUS, *lexer);
			break;
		case '-':
			symbol = createToken(MINUS, *lexer);
			break;
		case '*':
			symbol = createToken(MULTIPLY, *lexer);
			break;
		case '%':
			symbol = createToken(MODULO, *lexer);
			break;
		case '^':
			symbol = createToken(RAISE, *lexer);
			break;
		case '|':
			symbol = createToken(OR, *lexer);
			break;
		case '&':
			symbol = createToken(AND, *lexer);
			break;
	
		case '/':
			symbol = createToken(DIVIDE, *lexer);
			moveCursor(lexer, false);
			if (lexer->current == '/') {
				symbol->length++;
				symbol->FLOOR_DIVIDE;
			}
			break;
		case '=':
			symbol = createToken(ASSIGNMENT, *lexer);
			moveCursor(lexer, false);
			if (lexer->current == '=') {
				symbol->length++;
				symbol->EQUALITY;
			}
			break;
		case '<':
			symbol = createToken(LESS_THAN, *lexer);
			moveCursor(lexer, false);
			if (lexer->current == '=') {
				symbol->length++;
				symbol->LT_EQUAL;
			}
			break;
		case '>':
			symbol = createToken(GREATER_THAN, *lexer);
			moveCursor(lexer, false);
			if (lexer->current == '=') {
				symbol->length++;
				symbol->GT_EQUAL;
			}
			break;
		case '!':
			symbol = createToken(NOT, *lexer);
			moveCursor(lexer, false);
			if (lexer->current == '=') {
				symbol->length++;
				symbol->NOT_EQUAL;
			}
			break;
		}
	}
	moveCursor(lexer, false);
	appendToken(symbol, lexer);
}

void handleOtherValidCharacters(Lexer *lexer) {
	if (isalpha(lexer->current))
		handleWord(lexer);			//Identifier and reserved words
	else if (isnum(lexer->current) || lexer->current == ".")
		handleNumber(lexer);		//Integers and decimals
	else if (isgraph(lexer->current))
		handleSymbol(lexer);		//Delimiters and symbols
}

void handleCharacter(Lexer *lexer) {
	//Check whether character is invalid
	if (iscntrl(lexer->current))
		//invoke_error(...)

	//Start of indention
	if (lexer->current == ' ' || lexer->current == '\t') {
		Token *indent_token = createToken(INDENT, lexer);
		appendToken(indent_token, lexer);
		moveCursor(lexer, false);
	}

	//Succeeding indentions
	while (current == ' ' || current == '\t') {
		//Add 1 to last->length
		lexer->current = fgetc(lexer->file);
		lexer->cursor.column++;
		lexer->cursor.offset++;
	}

	if (current == '\"' || current == '~')
		handleCommentOrString(current, cursor, last);

	if (isalnum(current) || current == ' ' || current == '\t')
		handleOtherValidCharacters(lexer);
	
	//Symbol that may or may not be included in input alphabet
	else if (isgraph(current)){
		//Check validity since it may not be part of input alphabet
		checkValidity(current);
		//Then go to Normal mode like above
		handleOtherValidCharacters(lexer);
	}
	
	if (current == '\n' || current == EOF) {
		Token *newline_token = createToken(SENTENCE_BREAK, lexer);
		appendToken(newline_token, lexer);
		moveCursor(lexer, true);
	}
}

Token *tokenize(FILE *input_file) {
	Lexer lexer;
	lexer.file = input_file;
	lexer.current = fgetc(lexer.file);
	lexer.cursor.row = 1;	//row and column are 1-based, offset is 0-based
	lexer.cursor.column = 1;
	lexer.cursor.offset = 0;

	while (lexer.current != EOF) {
		printf("%c\t%d\n", lexer.current, lexer.cursor.column);
		handleCharacter(&lexer);
		//Code goes to this line if and only if current was \n or
		//current is invalid character.
	}

	DEBUG_MSG("DEBUG: Successfully created symtable.\n");
	return lexer.symtable;
}

int main(void) {
	//Temporary
	FILE *file = fopen("test.yass", "r");
	Token *symtable = tokenize(file);
	fclose(file);
}
