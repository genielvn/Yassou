#include "../lib/lexer.h"
#include "../lib/debug.h"

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
	State *next = nextState(lexer->trie, lexer->current);
	
	if (next == NULL)
		UNKNOWN_CHARACTER_ERROR(lexer->current, lexer->cursor.row);

	Token *symbol = createToken(next->type, lexer, true);
	
	if (!feof(lexer->file) && (next = nextState(next, lexer->current)) != NULL) {
		concatenateValueToToken(symbol, lexer);
		symbol->type = next->type;
	}
}

void handleWord(Lexer *lexer) {
	State *next = nextState(lexer->trie, lexer->current);
	Token *word;

	if (next != NULL) {
		word = createToken(IDENTIFIER, lexer, true);
		while (!feof(lexer->file) && (next = nextState(next, lexer->current)) != NULL) {
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

	int last_indent = indentPeek(&lexer->indent_stack);
	if (last_indent < indents)
	{
		indentPush(&lexer->indent_stack, indents);
		Token *indent_token = createToken(INDENT, lexer, false);
		indent_token->length = 0;
	}
	else if (last_indent > indents)
	{
		while (last_indent > indents)
		{
			if (last_indent < indents)
			{
				INDENTATION_ERROR(lexer->cursor.row);
			}
			indentPop(&lexer->indent_stack);
			Token *indent_token = createToken(DEDENT, lexer, false);
			indent_token->length = 0;
			last_indent = indentPeek(&lexer->indent_stack);
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

int indentPeek(IndentNode **stack)
{
	if (*stack == NULL) 
	{	
		return 0;
	}
	// DEBUG_MSG("IndentStack has peeked: %d", stack->data);
	return (*stack)->data;
}

int indentPop(IndentNode **stack)
{
	IndentNode *temp = *stack;
	if ((*stack) == NULL)
	{
		return 0;
	}
	else
	{
		*stack = (*stack)->next;
	}
	int num = temp->data;

	free(temp);
	return num;
}

void indentPush(IndentNode **stack, int indent)
{
	IndentNode *next_node = (IndentNode*)malloc(sizeof(IndentNode));
	MEMCHECK;
	next_node->data = indent;
	next_node->next = *stack;	
	*stack = next_node;
}

Token *tokenize(FILE *input_file) {
	Lexer lexer;
	lexer.file = input_file;
	lexer.current = fgetc(lexer.file);
	lexer.symtable = lexer.last = NULL;
	lexer.cursor = (Position){1, 1, 0}; // row & col: 1-based; offset: 0-based
	lexer.trie = generateAutomata();
	lexer.indent_stack = NULL;
	// initializeIndentStack(&lexer.indent_stack);

	while (!feof(lexer.file)) {
		handleCharacter(&lexer);
	}
	if (lexer.symtable == NULL) return;
	else if (lexer.last->type != SENTENCE_BREAK) createToken(SENTENCE_BREAK, &lexer, true);

	DEBUG_MSG("Successfully created symtable.");
	
	printTokens(&lexer);
	freeAutomata(lexer.trie);
	DEBUG_MSG("%d", lexer.symtable->type);
	return lexer.symtable;
}

void freeSymTable(Token *token) {
	if (token == NULL) return;

	freeSymTable(token->next);
	free(token);
}
