#include "parser.h"

ParseNode *generateParseNode(Parser *parser, Grammar grammar, bool next) {
	ParseNode *node = (ParseNode *)calloc(1, sizeof(ParseNode));
	MEMCHECK;
	node->grammar = grammar;

	if (next) {
		node->length = parser->current->length;
		node->location = parser->current->location;
		parser->current = parser->current->next;
		free(parser->current->previous);	//linked list element, no longer needed
	}
	
	DEBUG_MSG("Created %s grammar.", GrammarStr[grammar]);
	return node;
}

bool isCurrentToken(Parser *parser, TokenType type) {
	if (parser->current == NULL)
		return false;

	DEBUG_MSG("Current token: %s", TokenTypeStr[parser->current->type]);
	return parser->current->type == type;
}

void addChild(ParseNode *parent, ParseNode *token) {
	if (parent->first_child == NULL) {
		parent->first_child = token;
		return;
	}

	ParseNode *last_child = parent->first_child;
	for (; last_child->next_sibling != NULL; last_child = last_child->next_sibling);

	last_child->next_sibling = token;
}

//ParseNode *generateInvalidSyntax(Parser *parser) {
//	ParseNode *invalid = generateParseNode(parser, INVALID_SYNTAX, true);
//	for (; !iscurrentToken(parser, SENTENCE_BREAK);)
//}

ParseNode *generateVariableList(Parser *parser) {
	// Takes one variable for now.
	ParseNode *variables = generateParseNode(parser, VARIABLES, true);

	return variables;
}

ParseNode *generateVarType(Parser *parser) {
	if (isCurrentToken(parser, RESERVED_INTEGER))
		return generateParseNode(parser, INTEGER_RES, true);
	else if (isCurrentToken(parser, RESERVED_STRING))
		return generateParseNode(parser, STRING_RES, true);
	else if (isCurrentToken(parser, RESERVED_DECIMAL))
		return generateParseNode(parser, DECIMAL_RES, true);
	else if (isCurrentToken(parser, RESERVED_BOOLEAN))
		return generateParseNode(parser, BOOLEAN_RES, true);
	else
		return generateParseNode(parser, INVALID_SYNTAX, true);
}

ParseNode *generateDeclarationOrAssignment(Parser *parser) {
	ParseNode *statement = generateParseNode(parser, DECLARATION_STATEMENT, false);
	
	addChild(statement, generateParseNode(parser, SET_RES, true));

	if (isCurrentToken(parser, IDENTIFIER)) {
		addChild(statement, generateVariableList(parser));
	} else {
		DEBUG_MSG("Syntax error!");
		addChild(statement, generateParseNode(parser, INVALID_SYNTAX, true));
	}

	if (isCurrentToken(parser, RESERVED_AS)) {
		addChild(statement, generateParseNode(parser, AS_RES, true));
	} else if (isCurrentToken(parser, ASSIGNMENT)) {
		addChild(statement, generateParseNode(parser, ASSIGNMENT, true));
		statement->grammar = ASSIGN_STATEMENT;
	} else {
		addChild(statement, generateParseNode(parser, INVALID_SYNTAX, true));
	}

	if (statement->grammar == DECLARATION_STATEMENT) {
		addChild(statement, generateVarType(parser));
	} else if (statement->grammar == ASSIGN_STATEMENT) {
		DEBUG_MSG("Assignment");
	} else  {
		DEBUG_MSG("Syntax error!");
	}

	if (isCurrentToken(parser, SENTENCE_BREAK))
		addChild(statement, generateParseNode(parser, SENTENCE_END, true));
	else
		addChild(statement, generateParseNode(parser, INVALID_SYNTAX, true));

	return statement;
}

ParseNode *generateStatement(Parser *parser) {
	ParseNode *statement;

	if (isCurrentToken(parser, SENTENCE_BREAK)) {
		statement = generateParseNode(parser, BLANK_STATEMENT, true);
	} else if (isCurrentToken(parser, RESERVED_SET)) {
		statement = generateDeclarationOrAssignment(parser);

	//} else if (isCurrentToken(parser, RESERVED_INPUT)) {
		//statement = generateInputStatement(parser);
	//} else if (isCurrentToken(parser, RESERVED_OUTPUT)) {
		//statement = generateOutputStatement(parser);
	//} else if (isCurrentToken(parser, RESERVED_IF)) {
		//statement = generateConditionalStatement(parser);
	//} else if (isCurrentToken(parser, RESERVED_FOR)) {
		//statement = generateIterativeStatement(parser);
	} else {
		return NULL;
		DEBUG_MSG("Syntax Error!");
	}

	return statement;
}

ParseNode *parseTokens(Token *first) {
	DEBUG_MSG("PASSED HERE!");
	Parser parser;
	parser.current = first;
	parser.root = generateParseNode(&parser, STATEMENTS, false);

	while (parser.current != NULL) {
		addChild(parser.root, generateStatement(&parser));
	}

	return parser.root;
}
