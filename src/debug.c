#include "../lib/global.h"
#include "../lib/error.h"
#include "../lib/lexer.h"
#include "../lib/parser.h"

void printTokens(Lexer *lexer) {
	if (!interpreter->debugging) return;
	
	FILE *debug_file = fopen(".lexer_debug.txt", "w+");
	DEBUG_FILE_CHECK(debug_file);

	printf("########################################\n%-20s VALUE\n########################################\n", "TYPE");
	fprintf(debug_file, "########################################\n%-20s VALUE\n########################################\n", "TYPE");
	for (Token *current = lexer->symtable; current != NULL; current = current->next) {
		fseek(lexer->file, current->location.offset, SEEK_SET);
		
		char value[current->length+1];
		fread(value, current->length, 1, lexer->file);
		value[current->length] = '\0';

		Position location = current->location;
		char *token_type = TokenTypeStr[current->type];
		size_t length = current->length;

		if (*value == '\n' || current->next == NULL) {
			printf("%-20s %s\n",
					token_type, "\\n");
			fprintf(debug_file, "%-20s %s\n",
					token_type, "\\n");
		} else {
			printf("%-20s %s\n",
					token_type, value);
			fprintf(debug_file, "%-20s %s\n",
					token_type, value);
		}
	}

	DEBUG_MSG("See \".lexer_debug.txt\" for token symbol table.");
	fclose(debug_file);
}

void recursivePrint(ParseNode *node, size_t level, FILE *file) {
	if (node == NULL) return;

	for (size_t i = 0; i < level; ++i) {
		fprintf(file, "   │");
		printf("   │");
	}

	fprintf(file, "-%s\n", GrammarStr[node->grammar]);
	printf("-%s\n", GrammarStr[node->grammar]);

	//if (node->grammar == IDENTIFIER || node->grammar == INTEGER ||
	//	node->grammar == DECIMAL || node->grammar == STRING_LITERAL) {
	//to print variable
	//}

	for (ParseNode *current = node->first_child; current != NULL;
		 current=current->next_sibling) {
		recursivePrint(current, level+1, file);
	}
}

void printParseTree(ParseNode *node) {
	if (!interpreter->debugging) return;

	FILE *debug_file = fopen(".parser_debug.txt", "w+");
	DEBUG_FILE_CHECK(debug_file);

	recursivePrint(node, 0, debug_file);

	DEBUG_MSG("See \".parser_debug.txt\" for syntax parse tree.");
	fclose(debug_file);
}

void test()
{
	printf("fafdsfdsafdsa");
}