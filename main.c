#include "src/global.h"
#include "src/lexer.h"
#include "src/parser.h"

#define EXT ".yass"

Interpreter *interpreter;

void printHelp(void) {
	printf("usage: ./yassou [-h] [-d] FILE\n\n");
	printf("Yassou language interpreter.\n\n");
	printf("positional argument:\n  %-6s .yass file to process\n\n", "FILE");
	printf("options:\n  %-6s show this help message and exit\n", "-h");
	printf("  %-6s enable debugging\n", "-d");
}

void freeInterpreter(Interpreter *interpreter) {
	DEBUG_MSG("Freeing interpreter memory...");
	free(interpreter->file_name);
	fclose(interpreter->file);
	free(interpreter);
}

void parseFile(Interpreter *interpreter, char *file) {
	if (interpreter->file_name != NULL) MULTI_FILE_ERROR;
	
	interpreter->file_name = (char*)malloc(strlen(file)*sizeof(char));
	strcpy(interpreter->file_name, file);
}

void parseParameter(Interpreter *interpreter, char *string) {
	for (unsigned int i = 1; string[i] != '\0'; ++i) {
		switch (string[i]) {
			case 'd':
				interpreter->debugging = true;
				DEBUG_MSG("Debugging started...");
				break;
			case 'h':
				printHelp();
				exit(EXIT_SUCCESS);
				break;

			default:
				PARAMETER_ERROR;
		}
	}
}

bool isSameFileExt(const char *file, const char *target_ext) {
	char *file_ext = strrchr(file, '.');

	if (file_ext == NULL) return false;

	return strcmp(file_ext, target_ext);
}

void parseArguments(Interpreter *interpreter, int argc, char **argv) {
	if (argc == 1) {
		printHelp();
		exit(0);
	}

	interpreter->debugging = false;

	for (unsigned int i = 1; i < argc; ++i) {
		if (argv[i][0] != '-') {
			parseFile(interpreter, argv[i]);
			continue;
		}

		parseParameter(interpreter, argv[i]);
	}

	if (interpreter->file_name == NULL)
	{
		printHelp();
		NO_FILE_ERROR;
	}

	if (isSameFileExt(interpreter->file_name, EXT))
		WRONG_FILE_EXT_ERROR(interpreter->file_name);

	interpreter->file = fopen(interpreter->file_name, "r");
	FILE_EXIST_CHECK(interpreter->file_name);
}

int main(int argc, char **argv) {
	interpreter = (Interpreter*)calloc(1, sizeof(Interpreter));

	parseArguments(interpreter, argc, argv);
	DEBUG_MSG("Tokenizing...");
	interpreter->symtable = tokenize(interpreter->file);

	DEBUG_MSG("Parsing...");
	interpreter->parse_tree = parseTokens(interpreter->symtable);
	freeInterpreter(interpreter);

	return EXIT_SUCCESS;
}
