#ifndef ERROR_H
#define ERROR_H

#include <errno.h>

#define MSG_MEMORY_FAIL ": Cannot allocate memory."
#define MSG_DEBUG_FAIL_CREATE ": Unable to create debug file."
#define MSG_INVALID_CHAR ": Invalid input character: "
#define MSG_INVALID_FILE ": File is an invalid file."
#define MSG_NO_FILE_PROVIDED ": No file provided."
#define MSG_NOT_MAIN_FILE ": Not main file."

enum ErrorName {
	MEMORY_ALLOCATION_ERROR,
	INVALID_CHARACTER_ERROR,
	DEBUG_FILE_ERROR,
	INVALID_FILE_ERROR,
	WRONG_PROGRAM_ERROR,
	NO_FILE_PROVIDED,
};

void invoke_error(const char *error_msg, char *error_name);

#define MEMCHECK			\
	if (errno == ENOMEM)	\
		invoke_error(MSG_MEMORY_FAIL, "MEMORY_ALLOCATION_ERROR");

#define FILEEXISTCHECK		\
	if (errno == ENOENT)  \
		invoke_error(MSG_INVALID_FILE, "INVALID_FILE_ERROR");

#define DEBUGFILECHECK		\
	if (errno)				\
		invoke_error(MSG_DEBUG_FAIL_CREATE, "DEBUG_CREATE_FAIL_ERROR");

#define FILEPROVIDED		\
		invoke_error(MSG_NO_FILE_PROVIDED, "NO_FILE_PROVIDED");

#endif
