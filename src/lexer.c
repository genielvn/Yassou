#include "lexer.h"

void moveCursor(Lexer *lexer, bool next_line) {
	if (feof(lexer->file))
		return;
	
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

Token *createToken(TokenType type, Lexer *lexer, bool move) {
	Token *token = (Token*)calloc(1, sizeof(Token));
	MEMCHECK;

	token->type = type;
	token->location.row = lexer->cursor.row;
	token->location.column = lexer->cursor.column;
	token->location.offset = lexer->cursor.offset;
	token->length = 1;

	if (move)
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

	Token *symbol = createToken(next->type, lexer, true);
	
	if (!feof(lexer->file) && (next = nextTrie(next, lexer->current)) != NULL) {
		concatenateValueToToken(symbol, lexer);
		symbol->type = next->type;
	}
}

void handleWord(Lexer *lexer) {
	Trie *next = nextTrie(lexer->trie, lexer->current);
	Token *word;

	if (next != NULL) {
		word = createToken(IDENTIFIER, lexer, true);
		while (!feof(lexer->file) && (next = nextTrie(next, lexer->current)) != NULL) {
			concatenateValueToToken(word, lexer);
			word->type = next->type;
		}

		if (word->type == 0)
			word->type = IDENTIFIER;
	} else {
		word = createToken(IDENTIFIER, lexer, true);
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
		number = createToken(INTEGER, lexer, true);
	else
		number = createToken(DECIMAL, lexer, true);
	
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
	Token *delim1 = createToken(STR_DELIMITER, lexer, true);
	
	if (lexer->current == '\"') {
		Token *delim2 = createToken(STR_DELIMITER, lexer, true);
		return;
	}
	
	Token *string = createToken(STRING, lexer, true);
	while (lexer->current != '\"') {
		concatenateValueToToken(string, lexer);

		if (lexer->current == '\n' || feof(lexer->file)) {
			STRING_ERROR(lexer->cursor.row);
		}
		if (lexer->current == '\\') {
			concatenateValueToToken(string, lexer);
			continue;
		}
	}
	
	Token *delim2 = createToken(STR_DELIMITER, lexer, true);
}

void handleComment(Lexer *lexer) {
	Token *comment = createToken(COMMENT, lexer, true);

	while (lexer->current != '~' && lexer->current != '\n' && !feof(lexer->file))
		concatenateValueToToken(comment, lexer);

	if (lexer->current == '~')
		concatenateValueToToken(comment, lexer);
}

void handleCommentOrString(Lexer *lexer) {
	if (lexer->current == '\"')
		handleString(lexer);
	else if (lexer->current == '~')
		handleComment(lexer);

	if (!isspace(lexer->current) && !feof(lexer->file))
		SPACE_REQUIRED_ERROR(lexer->cursor.row);
}

void handleNewline(Lexer *lexer) {
	Token *newline_token = createToken(SENTENCE_BREAK, lexer, true);
	lexer->indent = true;
	return;
}

void handleIndention(Lexer *lexer) {
	int indents = 0;
	
	while(lexer->current == ' ' || lexer->current == '\t')
	{
		indents++;
		moveCursor(lexer, false);
	}

	DEBUG_MSG("%d - %d", indentPeek(&lexer->indent_stack), indents);
	if (indentPeek(&lexer->indent_stack) < indents)
	{
		indentPush(&lexer->indent_stack, indents);
		Token *indent_token = createToken(INDENT, lexer, false);
		indent_token->length = 0;

	}
	else if (indentPeek(&lexer->indent_stack) > indents)
	{
		while (indentPeek(&lexer->indent_stack) > indents)
		{
			indentPop(&lexer->indent_stack);
			Token *indent_token = createToken(DEDENT, lexer, false);
			indent_token->length = 0;
			if (indentPeek(&lexer->indent_stack) < indents)
			{
				INDENTATION_ERROR(lexer->cursor.row);
			}
		}
	}


	lexer->indent = false;
}

void handleCharacter(Lexer *lexer) {
	if (lexer->indent)
		handleIndention(lexer);

	if (iscntrl(lexer->current) && !isspace(lexer->current))
		UNKNOWN_CHARACTER_ERROR(lexer->current, lexer->cursor.row);

	if (lexer->current == '\n') {
		handleNewline(lexer);
		return;
	}

	if (lexer->current == ' ' || lexer->current == '\t')
		moveCursor(lexer, false);	//Ignoring whitespace
	
	lexer->indent = false;
	if (lexer->current == '\"' || lexer->current == '~')
		handleCommentOrString(lexer);

	else if (isdigit(lexer->current) || lexer->current == '.')
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

	printf("ROW\tCOL\tLEN\t%-16s VALUE\n", "TYPE");
	fprintf(debug_file, "ROW\tCOL\tLEN\t%-16s VALUE\n", "TYPE");
	for (Token *current = lexer->symtable; current != NULL; current = current->next) {
		fseek(lexer->file, current->location.offset, SEEK_SET);
		
		char value[current->length+1];
		fread(value, current->length, 1, lexer->file);
		value[current->length] = '\0';

		Position location = current->location;
		char *token_type = TokenTypeStr[current->type];
		size_t length = current->length;

		if (*value == '\n' || current->next == NULL) {
			printf("%d\t%d\t%ld\t%-16s %s\n",
					location.row, location.column, length, token_type, "\\n");
			fprintf(debug_file, "%d\t%d\t%ld\t%-16s %s\n",
					location.row, location.column, length, token_type, "\\n");
		} else {
			printf("%d\t%d\t%ld\t%-16s %s\n",
					location.row, location.column, length, token_type, value);
			fprintf(debug_file, "%d\t%d\t%ld\t%-16s %s\n",
					location.row, location.column, length, token_type, value);
		}
	}

	DEBUG_MSG("See \".lexer_debug.txt\" for token symbol table.");
	fclose(debug_file);
}

void initialize_IndentStack(IndentStack *stack)
{
	stack->top = -1;
}

int indentPeek(IndentStack *stack)
{
	if (stack->top == -1) return 0;
	return stack->indent[stack->top];
}

int indentPop(IndentStack *stack)
{
	return stack->indent[stack->top--];
}

void indentPush(IndentStack *stack, int indent)
{
	stack->indent[++stack->top] = indent;
}

Token *tokenize(FILE *input_file) {
	Lexer lexer;
	lexer.file = input_file;
	lexer.current = fgetc(lexer.file);
	lexer.symtable = lexer.last = NULL;
	lexer.cursor = (Position){1, 1, 0}; // row & col: 1-based; offset: 0-based
	lexer.trie = generateTrie();
	initialize_IndentStack(&lexer.indent_stack);

	while (!feof(lexer.file)) {
		handleCharacter(&lexer);
	}
	if (lexer.symtable == NULL) return;
	else if (lexer.last->type != SENTENCE_BREAK) createToken(SENTENCE_BREAK, &lexer, true);

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
