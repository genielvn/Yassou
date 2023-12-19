#!/usr/bin/python3

from decimal import Decimal
from constants import *

class Token():
    def __init__(self, token_type, value, line_num, char_num):
        self.type       = token_type
        self.value      = value
        self.location   = {'line_num' : line_num+1, 'char_num' : char_num+1}

    def __repr__(self):
        return f'({self.type}, {self.value}, {self.location})'

    # This only gives the string equivalent (necessary for concatValue())
    def getValue(self):
        return self.value

    def getType(self):
        return self.type

    def getLocation(self):
        return self.location

    # concatenate if value is character, addition if integer
    def concatValue(self, value):
        self.value = self.value + value

# NumberToken includes integer and decimal (28 sig figs)
class NumberToken(Token):
    def __init__(self, token_type, value, line_num, char_num):
        super().__init__(token_type, value, line_num, char_num)
    
    def getValue(self):
        if self.type == 'INTEGER':
            return int(self.value)

        return Decimal(self.value)

    def concatValue(self, char):
        self.value = self.value + char

        if char == '.':
            self.type = 'DECIMAL'

# WordToken includes reserved words and identifier words
class WordToken(Token):
    def __init__(self, value, line_num, char_num):
        # WordToken starts as an identifier since there's no
        # reserved word that is one character long.
        super().__init__('IDENTIFIER', value, line_num, char_num)

    def concatValue(self, char):
        self.value += char

        if self.value in RESERVED_WORDS:
            self.type = 'RESERVED_WORD'
        else:
            self.type = 'IDENTIFIER'

# String literals include ""
class StringToken(Token):
    def __init__(self, line_num, char_num):
        super().__init__('STRING', '', line_num, char_num)
        self.value = str(self.value)

# Refer to SYM_DICT for SymbolToken token_type and value
class SymbolToken(Token):
    def __init__(self, token_type, value, line_num, char_num):
        super().__init__(token_type, value, line_num, char_num)

    # concat must only be equal
    def concatValue(self, char):
        self.value += char
        self.type = SYM_DICT[self.value]

# length of whitespace == tab level
class IndentToken(Token):
    def __init__(self, line_num):
        super().__init__('INDENT', 1, line_num, 0)

    def addIndentLevel(self):
        self.value += 1

# Refer to DELIMITER_DICT for DelimiterToken token_type and value
class DelimiterToken(Token):
    def __init__(self, token_type, value, line_num, char_num):
        super().__init__(token_type, value, line_num, char_num)
