#include "lexer.h"

void moveCursor(Lexer *lexer, bool next_line) {
	if (feof(lexer->file)) {
		return;
	}
	
	lexer->current = fgetc(lexer->file);
	if (next_line) {
		lexer->cursor.row++;
		lexer->cursor.column = 1;
	} else {
		lexer->cursor.column++;
	}
	lexer->cursor.offset++;

	DEBUG_MSG("Moved cursor to row %d, col %d...", lexer->cursor.row, lexer->cursor.column);
}

void appendToken(Token *token, Lexer *lexer) {
	if (lexer->symtable == NULL) {
		DEBUG_MSG("Appending first token...");
		lexer->symtable = token;
	}

	if (lexer->last != NULL) {
		DEBUG_MSG("Appending next token...");
		lexer->last->next = token;
		token->previous = lexer->last;
	}

	lexer->last = token;
	DEBUG_MSG("Appended token.");
}

Token *createToken(TokenType type, Lexer *lexer) {
	Token *token = (Token*)calloc(1, sizeof(Token));
	MEMCHECK;

	token->type = type;
	token->location.row = lexer->cursor.row;
	token->location.column = lexer->cursor.column;
	token->location.offset = lexer->cursor.offset;
	token->length = 1;

	moveCursor(lexer, (token->type == SENTENCE_BREAK));
	appendToken(token, lexer);

	return token;
}

void concatenateValueToToken(Token *token, Lexer *lexer) {
	token->length++;
	moveCursor(lexer, false);
}

void handleString(Lexer *lexer) {
	Token *delim1 = createToken(STR_DELIMITER, lexer);
	
	if (lexer->current == '\"') {
		Token *delim2 = createToken(STR_DELIMITER, lexer);
		return;
	}
	
	Token *string = createToken(STRING, lexer);
	while (lexer->current != '\"') {
		concatenateValueToToken(string, lexer);

		if (lexer->current == '\n') {
			STRING_ERROR(lexer->cursor.row);
		}
		if (lexer->current == '\\') {
			concatenateValueToToken(string, lexer);
			continue;
		}
	}
	
	Token *delim2 = createToken(STR_DELIMITER, lexer);
}

void handleComment(Lexer *lexer) {
	Token *comment = createToken(COMMENT, lexer);

	do {
		concatenateValueToToken(comment, lexer);
	} while (lexer->current != '~' && lexer->current != '\n');

	if (lexer->current == '~') {
		concatenateValueToToken(comment, lexer);
	}
}

void handleCommentOrString(Lexer *lexer) {
	if (lexer->current == '\"')
		handleString(lexer);
	else if (lexer->current == '~')
		handleComment(lexer);

	if (!isspace(lexer->current)) {
		//Invoke error since this is noconcat	
		SPACE_REQUIRED_ERROR(lexer->cursor.row);
	}
}

void handleNumber(Lexer *lexer) {
	Token *number;
	
	if (isdigit(lexer->current))
		number = createToken(INTEGER, lexer);
	else
		number = createToken(DECIMAL, lexer);
	
	while (isdigit(lexer->current) || lexer->current == '.') {
		DEBUG_MSG("Appending %c...", lexer->current);
		if (isdigit(lexer->current)) {
			concatenateValueToToken(number, lexer);
		} else if (lexer->current == '.' || number->type == INTEGER) {
			concatenateValueToToken(number, lexer);
			number->type = DECIMAL;
		} else if (lexer->current == '.' || number->type == DECIMAL) {
			MULTIPLE_PERIOD_ERROR(lexer->cursor.row);
			//Invoke error, double decimal point
		}
		
		if (isalpha(lexer->current)) {
			NUMBER_PLUS_LETTER_ERROR(lexer->current, lexer->cursor.row);
		}
	}
	
	if (number->length == 1 && number->type == DECIMAL) {
		PERIOD_ONLY_NUMBER_ERROR(lexer->cursor.row);
	}
}

void handleWord(Lexer *lexer) {
	Token *word = createToken(IDENTIFIER, lexer);

	//First check for reserved word
	if (islower(lexer->current) || lexer->current == '_')
		goto skip;

	else if (lexer->current == 'A') {
		if (lexer->current != 'S')
			goto skip;

		concatenateValueToToken(word, lexer);
		word->type = RESERVED_WORD;		//AS
	} else if (lexer->current == 'B') {
		if (lexer->current == 'Y') {
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD;	//BY
			goto skip;
		}

		if (lexer->current != 'O') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'O') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'L') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'E') goto skip;
		concatenateValueToToken(word, lexer);

		
		if (lexer->current != 'A') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'N') goto skip;
		concatenateValueToToken(word, lexer);
		word->type = RESERVED_WORD;	//BOOLEAN
	} else if (lexer->current == 'D') {
		if (lexer->current == 'O') {
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD;	//DO
			goto skip;
		}
			
		if (lexer->current != 'E') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'C') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'I') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'M') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'A') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'L') goto skip;
		concatenateValueToToken(word, lexer);
		word->type = RESERVED_WORD;	//DECIMAL
	} else if (lexer->current == 'E') {
		if (lexer->current != 'L') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'S') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'E') goto skip;
		concatenateValueToToken(word, lexer);
		word->type = RESERVED_WORD;	//ELSE
	} else if (lexer->current == 'F') {
		if (lexer->current == 'A') {
			concatenateValueToToken(word, lexer);
		
			if (lexer->current != 'L') goto skip;
			concatenateValueToToken(word, lexer);
		
			if (lexer->current != 'S') goto skip;
			concatenateValueToToken(word, lexer);
		
			if (lexer->current != 'E') goto skip;
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD;	//FALSE
		} else if (lexer->current == 'O') {
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'R') goto skip;
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD;	//FOR
		}
	} else if (lexer->current == 'I') {
		if (lexer->current == 'F') {
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD;	//IF
			goto skip;
		}

		if (lexer->current != 'N') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current == 'P') {
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'U') goto skip;
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'T')
				goto skip;
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD; //INPUT
		} else if (lexer->current == 'T') {
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'E') goto skip;
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'G') goto skip;
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'E') goto skip;
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'R') goto skip;
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD; //INTEGER
		} else {
			goto skip;
		}
	} else if (lexer->current == 'O') {
		concatenateValueToToken(word, lexer);
		if (lexer->current != 'U') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'T') goto skip;
		concatenateValueToToken(word, lexer);

		if (lexer->current != 'P') goto skip;
		concatenateValueToToken(word, lexer);

		if (lexer->current != 'U') goto skip;
		concatenateValueToToken(word, lexer);
		
		if (lexer->current != 'T') goto skip;
		concatenateValueToToken(word, lexer);
		word->type = RESERVED_WORD;	//OUTPUT
	} else if (lexer->current == 'S') {
		if (lexer->current == 'E') {
			concatenateValueToToken(word, lexer);
			
			if (lexer->current != 'T') goto skip;
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD;	//SET
		} else if (lexer->current == 'T') {
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'R') goto skip;
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'I') goto skip;
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'N') goto skip;
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'G') goto skip;
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD; //STRING
		} else {
			goto skip;
		}
	} else if (lexer->current == 'T') {
		if (lexer->current == 'H') {
			concatenateValueToToken(word, lexer);
			
			if (lexer->current != 'E') goto skip;
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'N') goto skip;
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD;	//THEN
		} else if (lexer->current == 'O') {
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD;	//TO
			goto skip;
		} else if (lexer->current == 'R') {
			concatenateValueToToken(word, lexer);
			
			if (lexer->current != 'U') goto skip;
			concatenateValueToToken(word, lexer);

			if (lexer->current != 'E') goto skip;
			concatenateValueToToken(word, lexer);
			word->type = RESERVED_WORD; //TRUE
		} else {
			goto skip;
		}
	} else if (lexer->current == 'W') {
		if (lexer->current != 'H') goto skip;
		concatenateValueToToken(word, lexer);

		if (lexer->current != 'I') goto skip;
		concatenateValueToToken(word, lexer);

		if (lexer->current != 'L') goto skip;
		concatenateValueToToken(word, lexer);

		if (lexer->current != 'E') goto skip;
		concatenateValueToToken(word, lexer);
		word->type = RESERVED_WORD; //WHILE
	} else {
		goto skip;
	}

	skip:
	//Second check
	if (!isalnum(lexer->current) && lexer->current != '_') {
		return;
	}

	//identifiers: may fail first check but must fail the second check
	word->type = IDENTIFIER;

	while (isalnum(lexer->current) || lexer->current == '_') {
		DEBUG_MSG("Appending %c to token...", lexer->current);
		concatenateValueToToken(word, lexer);
	}

}

void handleSymbol(Lexer *lexer) {
	Trie *next = nextTrie(lexer->trie, lexer->current);
	
	if (next == NULL)
		UNKNOWN_CHARACTER_ERROR(lexer->current, lexer->cursor.row);

	Token *symbol = createToken(next->type, lexer);
	
	if ((next = nextTrie(next, lexer->current)) != NULL) {
		concatenateValueToToken(symbol, lexer);
		symbol->type = next->type;
	}

	DEBUG_MSG("Appended a symbol.");	
}

void handleCharacter(Lexer *lexer) {
	//Check whether character is invalid
	if (iscntrl(lexer->current) && !isspace(lexer->current)) {
		UNKNOWN_CHARACTER_ERROR(lexer->current, lexer->cursor.row);
	}

	//Start of indention
	if (lexer->indent && (lexer->current == ' ' || lexer->current == '\t')) {
		Token *indent_token = createToken(INDENT, lexer);
		DEBUG_MSG("Indent Mode.");

		//Succeeding indentions
		while (lexer->current == ' ' || lexer->current == '\t') {
			concatenateValueToToken(indent_token, lexer);
			//Add 1 to last->length
		}

		DEBUG_MSG("Indent Mode end.\n");
	} else if (lexer->current == ' ' || lexer->current == '\t') {
		moveCursor(lexer, false);
		DEBUG_MSG("Whitespace encountered.");
	}

	lexer->indent = false;
	if (lexer->current == '\n' || lexer->current == EOF) {
		Token *newline_token = createToken(SENTENCE_BREAK, lexer);
		DEBUG_MSG("Appended a sentence break token.");
		return;
	}

	if (lexer->current == '\"' || lexer->current == '~')
		handleCommentOrString(lexer);

	if (isdigit(lexer->current) || lexer->current == '.')
		handleNumber(lexer);	// Integers and decimals

	else if (isalpha(lexer->current) || lexer->current == '_')
		handleWord(lexer);		// Identifiers and reserved words

	else if (isgraph(lexer->current))
		handleSymbol(lexer);	// Symbols and other delimiters
}

void printTokens(Lexer *lexer) {
	if (!interpreter->debugging) return;
	FILE *debug_file = fopen(".lexer_debug.txt", "w+");
	DEBUG_FILE_CHECK(debug_file);
	// Test file creation

	printf("ROW\tCOL\t%-20s\tLENGTH\n", "VALUE");
	fprintf(debug_file, "ROW\tCOL\t%-20s\tLENGTH\n", "VALUE");
	for (Token *current = lexer->symtable; current != NULL; current = current->next) {
		fseek(lexer->file, current->location.offset, SEEK_SET);
		char value[current->length+1];
		fread(value, current->length, 1, lexer->file);
		value[current->length] = '\0';
		if (*value == '\n') {
			printf("%d\t%d\t%-20s\t%ld\n", current->location.row, current->location.column, "\\n", current->length);
			fprintf(debug_file, "%d\t%d\t%-20s\t%ld\n", current->location.row, current->location.column, "\\n", current->length);
		} else if (*value == EOF) {
			printf("%d\t%d\t%-20s\t%ld\n", current->location.row, current->location.column, "EOF", current->length);
			fprintf(debug_file, "%d\t%d\t%-20s\t%ld\n", current->location.row, current->location.column, "EOF", current->length);
		} else {
			printf("%d\t%d\t%-20s\t%ld\n", current->location.row, current->location.column, value, current->length);
			fprintf(debug_file, "%d\t%d\t%-20s\t%ld\n", current->location.row, current->location.column, value, current->length);
		}
	}

	DEBUG_MSG("See \".lexer_debug.txt\" for token symbol table.");
	fclose(debug_file);
}

Token *tokenize(FILE *input_file) {
	Lexer lexer;
	lexer.file = input_file;
	lexer.current = fgetc(lexer.file);
	lexer.symtable = lexer.last = NULL;
	lexer.cursor.row = 1;	// 1-based, offset is 0-based
	lexer.cursor.column = 1;
	lexer.cursor.offset = 0;
	lexer.trie = generateTrie();

	while (!feof(lexer.file)) {
		DEBUG_MSG("%c\t%d", lexer.current, lexer.cursor.column);
		handleCharacter(&lexer);
	}

	DEBUG_MSG("Successfully created symtable.");
	
	printTokens(&lexer);
	freeTrie(lexer.trie);
	return lexer.symtable;
}

void freeSymTable(Token *token) {
	if (token == NULL) return;

	freeSymTable(token->next);
	DEBUG_MSG("Freeing tokens...");
	free(token);
}
