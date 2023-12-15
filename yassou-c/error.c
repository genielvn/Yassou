#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "error.h"

void invoke_error(const char *error_msg, char *error_name) {
	printf("ERROR: %s [%s]\n", error_msg, error_name);
	exit(EXIT_FAILURE);
}
