#include "trie.h"

Trie *createTrieNode(void) {
	Trie *node = (Trie*)calloc(1, sizeof(Trie));
	MEMCHECK;

	return node;
}

void insertWord(Trie *root, const char *word, TokenType type) {
	Trie *current = root;
	
	for (unsigned int i = 0; i < strlen(word); ++i) {
		if (!current->children[word[i]])
			current->children[word[i]] = createTrieNode();

		current = current->children[word[i]];
	}

	current->type = type;
}

TokenType retrieveTokenType(Trie *root, const char *word) {
	Trie *current = root;
	
	for (unsigned int i = 0; i < strlen(word); ++i) {
		//Note: (type = 0) != SENTENCE_BREAK for this specific context!
		if (!current->children[word[i]])
			 return 0;

		current = current->children[word[i]];
	}

	return current->type;
}

Trie *generateTrie(void) {
	Trie *root = createTrieNode();

	for (unsigned int i = 0; i < 18; ++i)
		insertWord(root, RESERVED_WORDS[i], RESERVED_WORD);

	for (unsigned int i = 0; i < 20; ++i)
		insertWord(root, SYMBOLS[i],  i+2);

	return root;
}

void freeTrie(Trie *token) {
	if (!token) return;
	
	for (unsigned int i = 0; i < CHAR_MAX+1; ++i)
		freeTrie(token->children[i]);

	free(token);
}
