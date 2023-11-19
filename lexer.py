#!/usr/bin/python3

from tokens import *
from error import InvalidCharacterError
import logging

class Lexer():
    def __init__(self, debugging):
        self.symtable = []
        
        if debugging:
            logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')
        
        logging.debug('Lexer object instantiated.')

    def addTokenToSymTable(self, token):
        self.symtable.append(token)

    def createSymTable(self, file):
        for line_num, line in enumerate(file):
            # temporary queue to store token values
            token_queue = []

            for char_num, char in enumerate(line):
                if char in WHITESPACE:
                    continue
                elif char in SYM_DICT:
                    token_queue.append(Token(SYM_DICT[char], char, line_num, char_num))
                    continue
                elif char in DELIMITER_DICT:
                    token_queue.append(Token(DELIMITER_DICT[char], char, line_num, char_num))
                    continue
                elif char in DIGITS:
                    token_queue.append(self.make_number(line_num, char_num))
                    continue
                elif char == '\n':
                    continue
                else:
                    raise InvalidCharacterError(line_num, char_num, char)
            
            self.addTokenToSymTable(token_queue)

    def getSymTable(self, file):
        if not self.symtable:
            # symtable is empty
            self.createSymTable(file)

        return self.symtable
