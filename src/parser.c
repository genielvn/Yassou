#include "parser.h"

ParseNode *createParseNode(Parser *parser, Grammar grammar, bool next) {
	ParseNode *node = (ParseNode *)calloc(1, sizeof(ParseNode));
	MEMCHECK;
	node->grammar = grammar;

	if (next) {
		node->length = parser->current->length;
		node->location = parser->current->location;
		parser->current = parser->current->next;
		free(parser->current);	//linked list element, no longer needed
	}

	return node;
}

bool isCurrentToken(Parser *parser, TokenType type) {
	if (parser->current == NULL)
		return false;

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

//ParseNode *createDeclarationOrAssignment(Parser *parser) {
//	ParseNode *statement = generateParseNode(parser, DECLARATION_STATEMENT, false);
//
//	addChild(statement, generateParseNode(parser, SET_RES, true));
//
//	
//}

ParseNode *generateStatement(Parser *parser) {
	ParseNode *statement;

	if (isCurrentToken(parser, SENTENCE_BREAK)) {
		//statement = generateParseNode(parser, BLANK_STATEMENT, true);
	} else if (isCurrentToken(parser, RESERVED_SET)) {
		//statement = generateDeclarationOrAssignment(parser);
	} else if (isCurrentToken(parser, RESERVED_INPUT)) {
		//statement = generateInputStatement(parser);
	} else if (isCurrentToken(parser, RESERVED_OUTPUT)) {
		//statement = generateOutputStatement(parser);
	} else if (isCurrentToken(parser, RESERVED_IF)) {
		//statement = generateConditionalStatement(parser);
	} else if (isCurrentToken(parser, RESERVED_FOR)) {
		//statement = generateIterativeStatement(parser);
	}

	return statement;
}

ParseNode *parseTokens(Token *first) {
	Parser parser;
	parser.current = first;
	parser.root = createParseNode(&parser, STATEMENTS, false);

	while (parser.current != NULL) {
		addChild(parser.root, generateStatement(&parser));
	}

	return parser.root;
}
