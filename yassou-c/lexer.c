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
}

void appendToken(Token *token, Lexer *lexer) {
	if (lexer->symtable == NULL) {
		lexer->symtable = token;
	}

	if (lexer->last != NULL) {
		lexer->last->next = token;
		token->previous = lexer->last;
	}

	lexer->last = token;
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

void handleSymbol(Lexer *lexer) {
	Trie *next = nextTrie(lexer->trie, lexer->current);
	
	if (next == NULL)
		UNKNOWN_CHARACTER_ERROR(lexer->current, lexer->cursor.row);

	Token *symbol = createToken(next->type, lexer);
	
	if ((next = nextTrie(next, lexer->current)) != NULL) {
		concatenateValueToToken(symbol, lexer);
		symbol->type = next->type;
	}
}

void handleWord(Lexer *lexer) {
	Trie *next = nextTrie(lexer->trie, lexer->current);
	Token *word;

	if (next != NULL) {
		word = createToken(RESERVED_WORD, lexer);
		while ((next = nextTrie(lexer->trie, lexer->current)) != NULL)
			concatenateValueToToken(word, lexer);
	} else {
		word = createToken(IDENTIFIER, lexer);
	}

	if (!isalnum(lexer->current) && lexer->current != '_')
		return;	//reserved word or a one-letter identifier

	word->type = IDENTIFIER;
	while (isalnum(lexer->current) || lexer->current == '_')
		concatenateValueToToken(word, lexer);
}

void handleNumber(Lexer *lexer) {
	Token *number;
	
	if (isdigit(lexer->current))
		number = createToken(INTEGER, lexer);
	else
		number = createToken(DECIMAL, lexer);
	
	while (isdigit(lexer->current) || lexer->current == '.') {
		if (isdigit(lexer->current)) {
			concatenateValueToToken(number, lexer);
		} else if (lexer->current == '.' || number->type == INTEGER) {
			concatenateValueToToken(number, lexer);
			number->type = DECIMAL;
		} else if (lexer->current == '.' || number->type == DECIMAL) {
			MULTIPLE_PERIOD_ERROR(lexer->cursor.row);
		}
	}
	if (isalpha(lexer->current))
		NUMBER_PLUS_LETTER_ERROR(lexer->current, lexer->cursor.row);
	
	if (number->length == 1 && number->type == DECIMAL)
		PERIOD_ONLY_NUMBER_ERROR(lexer->cursor.row);
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

	while (lexer->current != '~' && lexer->current != '\n')
		concatenateValueToToken(comment, lexer);

	if (lexer->current == '~')
		concatenateValueToToken(comment, lexer);
}

void handleCommentOrString(Lexer *lexer) {
	if (lexer->current == '\"')
		handleString(lexer);
	else if (lexer->current == '~')
		handleComment(lexer);

	if (!isspace(lexer->current) && lexer->current != EOF)
		SPACE_REQUIRED_ERROR(lexer->cursor.row);
}

void handleNewline(Lexer *lexer) {
	Token *newline_token = createToken(SENTENCE_BREAK, lexer);
	lexer->indent = true;
	return;
}

void handleIndention(Lexer *lexer) {
	Token *indent_token = createToken(INDENTION, lexer);

	while(lexer->current == ' ' || lexer->current == '\t')
		concatenateValueToToken(indent_token, lexer);

	lexer->indent = false;
}

void handleCharacter(Lexer *lexer) {
	if (iscntrl(lexer->current) && !isspace(lexer->current))
		UNKNOWN_CHARACTER_ERROR(lexer->current, lexer->cursor.row);

	if (lexer->indent && (lexer->current == ' ' || lexer->current == '\t'))
		handleIndention(lexer);

	if (lexer->current == '\n' || lexer->current == EOF) {
		handleNewline(lexer);
		return;
	}

	if (lexer->current == ' ' || lexer->current == '\t')
		moveCursor(lexer, false);	//Ignoring whitespace
	
	lexer->indent = false;
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

	printf("ROW\tCOLUMN\tLENGTH\t%-16s VALUE\n", "TYPE");
	fprintf(debug_file, "ROW\tCOLUMN\tLENGTH\t%-16s VALUE\n", "TYPE");
	for (Token *current = lexer->symtable; current != NULL; current = current->next) {
		fseek(lexer->file, current->location.offset, SEEK_SET);
		char value[current->length+1];
		fread(value, current->length, 1, lexer->file);
		value[current->length] = '\0';

		Position location = current->location;
		char *token_type = TokenTypeStr[current->type];
		size_t length = current->length;

		if (*value == '\n' || current->next == NULL) {
			printf("%d\t%d\t%ld\t%-16s %s\n", location.row, location.column, length, token_type, "\\n");
			fprintf(debug_file, "%d\t%d\t%ld\t%-16s %s\n", location.row, location.column, length, token_type, "\\n");
		} else {
			printf("%d\t%d\t%ld\t%-16s %s\n", location.row, location.column, length, token_type, value);
			fprintf(debug_file, "%d\t%d\t%ld\t%-16s %s\n", location.row, location.column, length, token_type, value);
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
	lexer.cursor = (Position){1, 1, 0}; // row & col: 1-based; offset: 0-based
	lexer.trie = generateTrie();

	while (!feof(lexer.file)) {
		handleCharacter(&lexer);
	}

	if (lexer.last->type != SENTENCE_BREAK)
		createToken(SENTENCE_BREAK, &lexer);

	DEBUG_MSG("Successfully created symtable.");
	
	printTokens(&lexer);
	freeTrie(lexer.trie);
	return lexer.symtable;
}

void freeSymTable(Token *token) {
	if (token == NULL) return;

	freeSymTable(token->next);
	free(token);
}
