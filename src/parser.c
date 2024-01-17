#include "parser.h"

ParseNode *generateExpression(Parser *parser, bool is_sub_expr);
ParseNode *generateStatement(Parser *parser);
void printParseTree(ParseNode *node, size_t level);

ParseNode *generateParseNode(Parser *parser, Grammar grammar, bool next) {
	ParseNode *node = (ParseNode *)calloc(1, sizeof(ParseNode));
	MEMCHECK;
	node->grammar = grammar;

	if (next) {
		node->length = parser->current->length;
		node->location = parser->current->location;

		Token *next = parser->current->next;
		free(parser->current);	//linked list element, no longer needed
		parser->current = next;
	}
	
	DEBUG_MSG("Created %s grammar.", GrammarStr[grammar]);
	return node;
}

bool isCurrentToken(Parser *parser, TokenType type) {
	if (parser->current == NULL)
		return false;
	else if (parser->current->type == COMMENT) {
		Token *next = parser->current->next;
		free(parser->current);
		parser->current = next;
	}

	//DEBUG_MSG("Current token: %s", TokenTypeStr[parser->current->type]);
	return parser->current->type == type;
}

void addChild(ParseNode *parent, ParseNode *node) {
	if (parent->first_child == NULL) {
		parent->first_child = node;
		return;
	}

	ParseNode *last_child = parent->first_child;
	for (; last_child->next_sibling != NULL; last_child = last_child->next_sibling);

	last_child->next_sibling = node;
}

ParseNode *generateVariableList(Parser *parser) {
	ParseNode *variables = generateParseNode(parser, VARIABLES, false);

	addChild(variables, generateParseNode(parser, IDENTIFIER_VAR, true));

	while (isCurrentToken(parser, COMMA)) {
		addChild(variables, generateParseNode(parser, COMMA_DEL, true));
		addChild(variables, generateParseNode(parser, IDENTIFIER_VAR, true));
	}

	return variables;
}

ParseNode *generateVariable(Parser *parser) {
	if (isCurrentToken(parser, IDENTIFIER))
		return generateParseNode(parser, IDENTIFIER_VAR, true);
	DEBUG_MSG("Syntax error: Not identifier!");
	return generateParseNode(parser, INVALID_SYNTAX, true);
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
	else {
		DEBUG_MSG("Syntax error: Not a variable type!");
		return generateParseNode(parser, INVALID_SYNTAX, true);
	}
}

ParseNode *generateOperand(Parser *parser) {
	ParseNode *operand = generateParseNode(parser, OPERAND, false);

	if (isCurrentToken(parser, RESERVED_TRUE)) {
		addChild(operand, generateParseNode(parser, TRUE_LIT, true));
		return operand;
	} else if (isCurrentToken(parser, RESERVED_FALSE)) {
		addChild(operand, generateParseNode(parser, FALSE_LIT, true));
		return operand;
	}

	//optional MINUS
	if (isCurrentToken(parser, MINUS)) {
		addChild(operand, generateParseNode(parser, MINUS_OP, true));
	}

	if (isCurrentToken(parser, INTEGER)) {
		addChild(operand, generateParseNode(parser, INTEGER_LITERAL, true));
	} else if (isCurrentToken(parser, DECIMAL)) {
		addChild(operand, generateParseNode(parser, DECIMAL_LITERAL, true));
	} else if (isCurrentToken(parser, IDENTIFIER)) {
		addChild(operand, generateParseNode(parser, IDENTIFIER, true));
	} else if (isCurrentToken(parser, EXPR_BEGIN)) {
		addChild(operand, generateExpression(parser, false));
	} else {
		DEBUG_MSG("Syntax error: Expects integer, decimal, identifier, or `(`");
		addChild(operand, generateParseNode(parser, INVALID_SYNTAX, true));
	}

	return operand;
}

ParseNode *generateExpOp(Parser *parser) {
	return generateParseNode(parser, RAISE_OP, true);
}

ParseNode *generateFactor(Parser *parser) {
	ParseNode *factor = generateParseNode(parser, FACTOR, false);

	addChild(factor, generateOperand(parser));

	if (isCurrentToken(parser, RAISE)) {
		addChild(factor, generateExpOp(parser));
	} else {
		return factor;
	}

	addChild(factor, generateOperand(parser));

	while (isCurrentToken(parser, RAISE)) {
		addChild(factor, generateExpOp(parser));
		addChild(factor, generateOperand(parser));
	}

	return factor;
}

ParseNode *generateMulDivOp(Parser *parser) {
	if (isCurrentToken(parser, MULTIPLY))
		return generateParseNode(parser, MULTIPLY_OP, true);
	else if (isCurrentToken(parser, DIVIDE))
		return generateParseNode(parser, DIVIDE_OP, true);
	else if (isCurrentToken(parser, FLOOR_DIVIDE))
		return generateParseNode(parser, FLOOR_DIVIDE, true);
	else
		return generateParseNode(parser, MODULO, true);
}

ParseNode *generateTerm(Parser *parser) {
	ParseNode *term = generateParseNode(parser, TERM, false);

	addChild(term, generateFactor(parser));

	if (isCurrentToken(parser, MULTIPLY) || isCurrentToken(parser, DIVIDE) ||
		isCurrentToken(parser, FLOOR_DIVIDE) || isCurrentToken(parser, MODULO)) {
		addChild(term, generateMulDivOp(parser));
	} else {
		return term;
	}

	addChild(term, generateFactor(parser));

	while (isCurrentToken(parser, MULTIPLY) || isCurrentToken(parser, DIVIDE) ||
		   isCurrentToken(parser, FLOOR_DIVIDE) || isCurrentToken(parser, MODULO)) {
		addChild(term, generateMulDivOp(parser));
		addChild(term, generateOperand(parser));
	}

	return term;
}

ParseNode *generateAddSubOp(Parser *parser) {
	if (isCurrentToken(parser, PLUS))
		return generateParseNode(parser, PLUS_OP, true);
	return generateParseNode(parser, MINUS_OP, true);
}

ParseNode *generateComparand(Parser *parser) {
	ParseNode *comparand = generateParseNode(parser, COMPARAND, false);

	addChild(comparand, generateTerm(parser));

	if (isCurrentToken(parser, PLUS) || isCurrentToken(parser, MINUS)) {
		addChild(comparand, generateAddSubOp(parser));
	} else {
		return comparand;
	}

	addChild(comparand, generateTerm(parser));

	while (isCurrentToken(parser, PLUS) || isCurrentToken(parser, MINUS)) {
		addChild(comparand, generateAddSubOp(parser));
		addChild(comparand, generateTerm(parser));
	}

	return comparand;
}

ParseNode *generateComparison(Parser *parser) {
	if (isCurrentToken(parser, OR))
		return generateParseNode(parser, OR_OP, true);
	else if (isCurrentToken(parser, AND))
		return generateParseNode(parser, AND_OP, true);
	else if (isCurrentToken(parser, EQUALITY))
		return generateParseNode(parser, EQUALITY_OP, true);
	else if (isCurrentToken(parser, GREATER_THAN))
		return generateParseNode(parser, GREATER_THAN_OP, true);
	else if (isCurrentToken(parser, LESS_THAN))
		return generateParseNode(parser, LESS_THAN_OP, true);
	else if (isCurrentToken(parser, LT_EQUAL))
		return generateParseNode(parser, LT_EQUAL_OP, true);
	else if (isCurrentToken(parser, GT_EQUAL))
		return generateParseNode(parser, GT_EQUAL_OP, true);
	
	return generateParseNode(parser, NOT_EQUAL_OP, true);
}

ParseNode *generateExpression(Parser *parser, bool is_sub_expr) {
	ParseNode *expression = generateParseNode(parser, EXPRESSION, false);

	if (is_sub_expr) {		//is_sub_expr == always starts with `(`
		addChild(expression, generateParseNode(parser, EXPR_BEGIN_DEL, true));
	}

	//optional NOT
	if (isCurrentToken(parser, NOT)) {
		addChild(expression, generateParseNode(parser, NOT_OP, true));
	}

	addChild(expression, generateComparand(parser));

	if (isCurrentToken(parser, OR) || isCurrentToken(parser, AND) ||
		isCurrentToken(parser, EQUALITY) || isCurrentToken(parser, LESS_THAN) ||
		isCurrentToken(parser, GREATER_THAN) || isCurrentToken(parser, LT_EQUAL) ||
		isCurrentToken(parser, GT_EQUAL) || isCurrentToken(parser, NOT_EQUAL_OP)) {
		addChild(expression, generateComparison(parser));
	} else if (!is_sub_expr) {
		return expression;
	} else if (isCurrentToken(parser, EXPR_TERMINATE)) {	//is_sub_expr is true
		addChild(expression, generateParseNode(parser, EXPR_TERMINATE_DEL, true));
		return expression;
	} else {
		DEBUG_MSG("Syntax Error: No `)`");
		addChild(expression, generateParseNode(parser, INVALID_SYNTAX, true));
		return expression;
	}

	addChild(expression, generateComparand(parser));
	
	while (isCurrentToken(parser, OR) || isCurrentToken(parser, AND) ||
		   isCurrentToken(parser, EQUALITY) || isCurrentToken(parser, LESS_THAN) ||
		   isCurrentToken(parser, GREATER_THAN) || isCurrentToken(parser, LT_EQUAL) ||
		   isCurrentToken(parser, GT_EQUAL) || isCurrentToken(parser, NOT_EQUAL_OP)) {
		addChild(expression, generateComparison(parser));
		addChild(expression, generateComparand(parser));
	}

	if (is_sub_expr && isCurrentToken(parser, EXPR_TERMINATE)) {
		addChild(expression, generateParseNode(parser, EXPR_TERMINATE_DEL, true));
	} else if (is_sub_expr) {
		DEBUG_MSG("Syntax Error: No `)`");
		addChild(expression, generateParseNode(parser, INVALID_SYNTAX, true));
	}

	DEBUG_MSG("Closing expression...");
	return expression;
}

ParseNode *generatePredicate(Parser *parser) {
	ParseNode *predicate = generateParseNode(parser, PREDICATE_COND, false);

	DEBUG_MSG("Starting predicate...");

	addChild(predicate, generateParseNode(parser, IF_RES, true));
	addChild(predicate, generateExpression(parser, false));
	
	DEBUG_MSG("Closing predicate...");
	return predicate;
}

ParseNode *generateMedialOrFinal(Parser *parser) {
	ParseNode *cond = generateParseNode(parser, FINAL_COND, false);
	addChild(cond, generateParseNode(parser, ELSE_RES, true));

	if (isCurrentToken(parser, RESERVED_IF)) {
		cond->grammar = MEDIAL_COND;
		addChild(cond, generatePredicate(parser));

		//optional THEN
		if (isCurrentToken(parser, RESERVED_THEN))
			addChild(cond, generateParseNode(parser, THEN_RES, true));

		if (isCurrentToken(parser, SENTENCE_BREAK)) {
			addChild(cond, generateParseNode(parser, SENTENCE_END, true));
		} else {
			DEBUG_MSG("Syntax error: No SENTENCE_BREAK!");
			addChild(cond, generateParseNode(parser, INVALID_SYNTAX, true));
		}

		//indent
		if (isCurrentToken(parser, INDENT)) {
			addChild(cond, generateParseNode(parser, INDENT_DEL, true));
		} else {
			DEBUG_MSG("Syntax error: No INDENT!");
			addChild(cond, generateParseNode(parser, INVALID_SYNTAX, true));
		}
	
		//sub-statements
		addChild(cond, generateStatement(parser));

		if (isCurrentToken(parser, DEDENT)) {
			addChild(cond, generateParseNode(parser, DEDENT_DEL, true));
		} else {
			DEBUG_MSG("Syntax error: No DEDENT!");
			addChild(cond, generateParseNode(parser, INVALID_SYNTAX, true));
		}

		return cond;
	}
	
	//optional THEN
	if (isCurrentToken(parser, RESERVED_THEN))
		addChild(cond, generateParseNode(parser, THEN_RES, true));

	if (isCurrentToken(parser, SENTENCE_BREAK)) {
		addChild(cond, generateParseNode(parser, SENTENCE_END, true));
	} else {
		DEBUG_MSG("Syntax error: No SENTENCE_BREAK!");
		addChild(cond, generateParseNode(parser, INVALID_SYNTAX, true));
	}

	//indent
	if (isCurrentToken(parser, INDENT)) {
		addChild(cond, generateParseNode(parser, INDENT_DEL, true));
	} else {
		DEBUG_MSG("Syntax error: No INDENT!");
		addChild(cond, generateParseNode(parser, INVALID_SYNTAX, true));
	}
	
	//sub-statements
	addChild(cond, generateStatement(parser));

	if (isCurrentToken(parser, DEDENT)) {
		addChild(cond, generateParseNode(parser, DEDENT_DEL, true));
	} else {
		DEBUG_MSG("Syntax error: No DEDENT!");
		addChild(cond, generateParseNode(parser, INVALID_SYNTAX, true));
	}
	
	return cond;
}

ParseNode *generateInitialCond(Parser *parser) {
	ParseNode *initial = generateParseNode(parser, INITIAL_COND, false);

	addChild(initial, generatePredicate(parser));

	//optional THEN
	if (isCurrentToken(parser, RESERVED_THEN))
		addChild(initial, generateParseNode(parser, THEN_RES, true));

	if (isCurrentToken(parser, SENTENCE_BREAK)) {
		addChild(initial, generateParseNode(parser, SENTENCE_END, true));
	} else {
		DEBUG_MSG("Syntax error: No SENTENCE_BREAK!");
		addChild(initial, generateParseNode(parser, INVALID_SYNTAX, true));
	}

	//indent
	if (isCurrentToken(parser, INDENT)) {
		addChild(initial, generateParseNode(parser, INDENT_DEL, true));
	} else {
		DEBUG_MSG("Syntax error: No INDENT!");
		addChild(initial, generateParseNode(parser, INVALID_SYNTAX, true));
	}
	
	//sub-statements
	addChild(initial, generateStatement(parser));

	if (isCurrentToken(parser, DEDENT)) {
		addChild(initial, generateParseNode(parser, DEDENT_DEL, true));
	} else {
		DEBUG_MSG("Syntax error: No DEDENT!");
		addChild(initial, generateParseNode(parser, INVALID_SYNTAX, true));
	}

	return initial;
}

ParseNode *generateConditionalStatement(Parser *parser) {
	ParseNode *statement = generateParseNode(parser, CONDITIONAL_STATEMENT, false);

	addChild(statement, generateInitialCond(parser));

	while (isCurrentToken(parser, RESERVED_ELSE))
		addChild(statement, generateMedialOrFinal(parser));

	return statement;
}

ParseNode *generateString(ParseNode *parser) {
	//String syntax is handled in the lexer
	ParseNode *string_literal = generateParseNode(parser, STRING, false);

	DEBUG_MSG("Starting string...");
	
	addChild(string_literal, generateParseNode(parser, STR_DEL, true));
	addChild(string_literal, generateParseNode(parser, STRING_LITERAL, true));
	addChild(string_literal, generateParseNode(parser, STR_DEL, true));

	DEBUG_MSG("Closing string...");
	return string_literal;
}

ParseNode *generateValue(Parser *parser) {
	if (isCurrentToken(parser, STR_DELIMITER))
		return generateString(parser);
	else if (isCurrentToken(parser, INTEGER) || isCurrentToken(parser, DECIMAL) ||
			 isCurrentToken(parser, IDENTIFIER) || isCurrentToken(parser, MINUS) ||
			 isCurrentToken(parser, NOT) || isCurrentToken(parser, EXPR_BEGIN))
		return generateExpression(parser, false);
	else
		DEBUG_MSG("Syntax error: Expects a value...");
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
		addChild(statement, generateExpression(parser, false));
	}

	if (isCurrentToken(parser, SENTENCE_BREAK))
		addChild(statement, generateParseNode(parser, SENTENCE_END, true));
	else
		addChild(statement, generateParseNode(parser, INVALID_SYNTAX, true));

	return statement;
}

ParseNode *generateOutputStatement(Parser *parser) {
	ParseNode *statement = generateParseNode(parser, OUTPUT_STATEMENT, false);

	addChild(statement, generateParseNode(parser, OUTPUT_RES, true));
	addChild(statement, generateValue(parser));

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
	} else if (isCurrentToken(parser, RESERVED_OUTPUT)) {
		statement = generateOutputStatement(parser);
	} else if (isCurrentToken(parser, RESERVED_IF)) {
		statement = generateConditionalStatement(parser);
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

	printParseTree(parser.root, 0);
	return parser.root;
}

void printParseTree(ParseNode *node, size_t level) {
	if (node == NULL || !interpreter->debugging) return;

	//FILE *debug_file = fopen(".parser_debug.txt", "w+");
	//DEBUG_FILE_CHECK(debug_file);

	for (size_t i = 0; i < level; ++i)
		printf("   ");

	printf("%s\n", GrammarStr[node->grammar]);
	for (ParseNode *current = node->first_child; current != NULL;
		 current=current->next_sibling) {
		printParseTree(current, level+1);
	}
	
	if (level != 0) return;

	DEBUG_MSG("See \".parser_debug.txt\" for syntax parse tree.");
	//fclose(debug_file);
}
