#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include "global.h"

#define INVOKE_ERROR(str, ...) \
	do { \
		printf("ERROR: " __FILE__ ": " str "\n", ##__VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} while (0);


#define MEMCHECK \
	do { \
		if (errno == ENOMEM) \
			INVOKE_ERROR("Cannot allocate memory. [MEM_ALLOC_ERROR]"); \
	} while (0);

#define FILE_EXIST_CHECK(file) \
	do { \
		if (errno == ENOENT)  \
			INVOKE_ERROR("No \"%s\" file. [NONEXISTENT_FILE_ERROR]", file); \
	} while (0);

#define DEBUG_FILE_CHECK \
	do { \
		if (errno) \
			INVOKE_ERROR("Cannot create debug file. [DEBUG_CREATE_ERROR]"); \
	} while (0);

#define WRONG_FILE_EXT_ERROR(file) \
	do { \
		INVOKE_ERROR("File \"%s\" is not a valid Yassou file. [WRONG_FILE_EXT_ERROR]", file); \
	} while (0);

#define MULTI_FILE_ERROR \
	do { \
		INVOKE_ERROR("Multiple files supplied. [MULTI_FILE_ERROR]"); \
	} while (0);

#define NO_FILE_ERROR \
	do { \
		INVOKE_ERROR("No file supplied. [NO_FILE_ERROR]"); \
	} while (0);

#define PARAMETER_ERROR \
	do { \
		INVOKE_ERROR("Unknown parameter supplied. [PARAMETER_ERROR]"); \
	} while (0);

#define NUMBER_PLUS_LETTER_ERROR(ch, line) \
	do { \
		INVOKE_ERROR("Attempted to concatenate \'%c\' to a number. (line %d) [NUMBER_PLUS_LETTER_ERROR]", ch, line); \
	} while (0);

#define UNKNOWN_CHARACTER_ERROR(ch, line) \
	do { \
		INVOKE_ERROR("Supplied unknown character \'%c\'. (line %d) [INVALID_CHARACTER_ERROR]", ch, line); \
	} while (0);

#define MULTIPLE_PERIOD_ERROR(line) \
	do { \
		INVOKE_ERROR("Multiple decimal point in a decimal token. (line %d) [MULTIPLE_PERIOD_ERROR]", line); \
	} while (0);

#define PERIOD_ONLY_NUMBER_ERROR(line) \
	do { \
		INVOKE_ERROR("Encountered a decimal-point-only token. (line %d) [PERIOD_ONLY_NUMBER_ERROR]", line); \
	} while (0);

#define SPACE_REQUIRED_ERROR(line) \
	do { \
		INVOKE_ERROR("Space delimiter required. (line %d) [SPACE_REQUIRED_ERROR]", line) \
	} while (0);

#define STRING_ERROR(line) \
	do { \
		INVOKE_ERROR("String token is terminated unexpectedly. (line %d)", line); \
	} while (0);

#endif
