#!/usr/bin/python3

from tokens import *
from error import InvalidCharacterError
import logging

class Lexer():
    def __init__(self, debugging):
        self.symtable = []
        self.token_queue = []
        
        if debugging:
            logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')
        
        logging.debug('Lexer object instantiated.')

    def addTokenToSymTable(self, token):
        for t in token:
            self.symtable.append(t)

    def createSymTable(self, file):
        for line_num, line in enumerate(file):
            # temporary queue to store token values
            self.token_queue = []

            for char_num, char in enumerate(line):
                if char == ' ':
                    self.token_queue.append(Token("SPACE", None, line_num, char_num))
                elif char == '\t':
                    self.token_queue.append(Token("INDENT", None, line_num, char_num))
                elif char in DELIMITER_DICT:
                    self.token_queue.append(DelimiterToken(DELIMITER_DICT[char], line_num, char_num))
                    continue
                elif char in SYM_DICT:
                    if char == '.' and self.previousTokenType() == 'INTEGER':
                        token = self.token_queue[-1]
                        self.replacePreviousToken(DecimalToken(str(token.getValue()) + char, token.location['line_num'], token.location['char_num']))
                    else:
                        self.token_queue.append(Token(SYM_DICT[char], char, line_num, char_num))
                    continue
                elif char in DIGITS:
                    if self.previousTokenType() == 'INTEGER' or self.previousTokenType() == 'DECIMAL':
                        self.token_queue[-1].concatValue(char)
                    else:
                        self.token_queue.append(IntegerToken(char, line_num, char_num))
                    continue
                elif char in KEYWORD_CHARACTERS:
                    self.make_keyword(char, line_num, char_num)
                else:
                    raise InvalidCharacterError(line_num, char_num, char)
                
            self.addTokenToSymTable(self.token_queue)

    def previousTokenType(self):
        if not self.token_queue: 
            return None
        return self.token_queue[-1].getType()
    
    def replacePreviousToken(self, new_token: Token):
        self.token_queue.pop()
        self.token_queue.append(new_token)

    def make_keyword(self, char, line_num, char_num):
        if self.previousTokenType() == 'IDENTIFIER':
            current_name = self.token_queue[-1].getValue() + char
            if current_name in KEYWORDS:
                token = self.token_queue[-1]
                self.replacePreviousToken(KeywordToken(current_name, token.location['line_num'], token.location['char_num']))
            else:
                self.token_queue[-1].concatValue(char)
        elif self.previousTokenType() == 'KEYWORD':
            current_name = self.token_queue[-1].getValue() + char
            token = self.token_queue[-1]
            self.replacePreviousToken(IdentifierToken(current_name, token.location['line_num'], token.location['char_num']))
        else:
            self.token_queue.append(IdentifierToken(char, line_num, char_num))
    
    def getSymTable(self, file):
        if not self.symtable:
            # symtable is empty
            self.createSymTable(file)

        return self.symtable

    def printSymTable(self):
        for token in self.symtable:
            print("{:<25}{:}".format(token.type, token.value))