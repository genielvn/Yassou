#include "global.h"
#include "lexer.h"

#define EXT ".yass"

Interpreter *interpreter;

void freeInterpreter(Interpreter *interpreter) {
	DEBUG_MSG("Freeing interpreter memory...");
	free(interpreter->file_name);
	fclose(interpreter->file);
	freeSymTable(interpreter->symtable);
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
				//Help
				printf("Help dialog\n");
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
	if (argc == 1)
		NO_FILE_ERROR;

	interpreter->debugging = false;

	for (unsigned int i = 1; i < argc; ++i) {
		if (argv[i][0] != '-') {
			parseFile(interpreter, argv[i]);
			continue;
		}

		parseParameter(interpreter, argv[i]);
	}

	if (interpreter->file_name == NULL)
		NO_FILE_ERROR;

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
	freeInterpreter(interpreter);

	return EXIT_SUCCESS;
}
