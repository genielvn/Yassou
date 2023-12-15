#ifndef ERROR_H
#define ERROR_H

#include <errno.h>

#define MSG_MEMORY_FAIL	__FILE__ ": Cannot allocate memory."
#define MSG_DEBUG_FAIL_CREATE __FILE__ ": Unable to create debug file."
#define MSG_INVALID_CHAR(char) __FILE__ ": Invalid input character: " #char
#define MSG_INVALID_FILE(str) __FILE__ ": File" \
                                   str ": is an invalid file."
#define MSG_NOT_MAIN_FILE __FILE__ ": Not main file."

enum ErrorName {
	MEMORY_ALLOCATION_ERROR,
	INVALID_CHARACTER_ERROR,
	DEBUG_FILE_ERROR,
	INVALID_FILE_ERROR,
	WRONG_PROGRAM_ERROR
};

void invoke_error(const char *error_msg, enum ErrorName error_name);

#define MEMCHECK			\
	if (errno == ENOMEM)	\
		invoke_error(MSG_MEMORY_FAIL, "MEMORY_ALLOCATION_ERROR");

#define FILEEXISTCHECK		\
	if (errno == ENOENT)	\
		invoke_error(MSG_MEMORY_FAIL, "FILE_NONEXISTENT_ERROR");

#define DEBUGFILECHECK		\
	if (errno)				\
		invoke_error(MSG_DEBUG_FAIL_CREATE, "DEBUG_CREATE_FAIL_ERROR");

#endif
