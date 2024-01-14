#include "trie.h"

State *createState(void) {
	State *state = (State*)calloc(1, sizeof(State));
	MEMCHECK;

	return state;
}

void insertWord(State *root, const char *word, TokenType type) {
	State *current = root;

	for (unsigned int i = 0; i < strlen(word); ++i) {
		if (!current->children[word[i]])
			current->children[word[i]] = createState();

		current = current->children[word[i]];
	}

	current->type = type;
}

State *nextState(State *state, const char ch) {
	return state->children[ch];
}

State *generateAutomata(void) {
	State *root = createState();

	for (unsigned int i = 0; i < 18; ++i)
		insertWord(root, RESERVED_WORDS[i], i+29 );

	for (unsigned int i = 0; i < 20; ++i)
		insertWord(root, SYMBOLS[i],  i+3);

	return root;
}

void freeAutomata(State *token) {
	if (!token) return;
	
	for (unsigned int i = 0; i < CHAR_MAX+1; ++i)
		freeAutomata(token->children[i]);

	free(token);
}
