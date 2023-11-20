#!/usr/bin/python3

from decimal import Decimal

# accepted input characters
UPPERCASE_LETTERS       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
LOWERCASE_LETTERS       = UPPERCASE_LETTERS.lower()
DIGITS                  = '0123456789' 
KEYWORD_CHARACTERS      = UPPERCASE_LETTERS + LOWERCASE_LETTERS + '_'
IDENTIFIER_CHARACTERS   = KEYWORD_CHARACTERS + DIGITS
SYMBOLS                 = './*+-~!|&=\\%^<>'
DELIMITERS              = ',()'
WHITESPACE              = ' \t'

# tokens
SYM_DICT = {
        '+'  : 'PLUS',
        '-'  : 'MINUS',
        '*'  : 'MULTIPLY',
        '/'  : 'DIVIDE',
        '%'  : 'MODULO',
        '^'  : 'RAISE',
        '!!' : 'NOT',
        '||' : 'OR',
        '&&' : 'AND',
        '='  : 'ASSIGNMENT',
        '==' : 'EQUALS',
        '<'  : 'LESS_THAN',
        '>'  : 'GREATER_THAN',
        '<=' : 'LT_EQUAL',
        '>=' : 'GT_EQUAL',
        '=/=': 'NOT_EQUAL',
        '.'  : 'DOT'
        }

DELIMITER_DICT = {
        '\n' : 'SENTENCE',
        '('  : 'EXPR_BEGIN',
        ')'  : 'EXPR_TERMINATE',
        '~'  : 'COMMENT',
        '\"' : 'STR_QUOTE'
        }

# keywords/reserved words
KEYWORDS       = ['FOR', 'TO', 'BY', 'DO', 'WHILE',
                  'IF', 'ELSE', 'THEN', 'INPUT', 'OUTPUT',
                  'INTEGER', 'STRING', 'DECIMAL', 'BOOLEAN',
                  'TRUE', 'FALSE', 'SET', 'AS']

class Token():
    def __init__(self, token_type, value, line_num, char_num):
        self.type       = token_type
        self.value      = value
        self.location   = {'line_num' : line_num, 'char_num' : char_num}

    def __repr__(self):
        return f'({self.type}, {self.value}, {self.location})'

    # This only gives the string equivalent (necessary for concatValue())
    def getValue(self):
        return self.value

    def getType(self):
        return self.type

    def getLocation(self):
        return self.location

    def concatValue(self, char):
        self.value = self.value + char

class IntegerToken(Token):
    def __init__(self, value, line_num, char_num):
        super().__init__('INTEGER', value, line_num, char_num)
    
    def getValue(self):
        return int(self.value)

# Decimal is 28 significant figures by default.
class DecimalToken(Token):
    def __init__(self, value, line_num, char_num):
        super().__init__('DECIMAL', value, line_num, char_num)

    def getValue(self):
        return Decimal(self.value)

class IdentifierToken(Token):
    def __init__(self, value, line_num, char_num):
        super().__init__('IDENTIFIER', value, line_num, char_num)

# In PPF, keywords are also reserved words.
class KeywordToken(Token):
    def __init__(self, value, line_num, char_num):
        super().__init__('KEYWORD', value, line_num, char_num)

class BooleanToken(Token):
    def __init__(self, value, line_num, char_num):
        super().__init__('BOOLEAN', value, line_num, char_num)

# String literals include ""
class StringToken(Token):
    def __init__(self, value, line_num, char_num):
        super().__init__('STRING', value, line_num, char_num)

# Refer to SYM_DICT for SymbolToken values
class SymbolToken(Token):
    def __init__(self, value, line_num, char_num):
        super().__init__('SYMBOL', value, line_num, char_num)

# length of whitespace == tab level
class TabToken(Token):
    def __init__(self, value, line_num, char_num):
        super().__init__('TAB', value, line_num, char_num)

    def getValue(self):
        return len(self.value)

# Refer to DELIMITER_DICT for DelimiterToken values
class DelimiterToken(Token):
    def __init__(self, value, line_num, char_num):
        super().__init__('DELIMITER', value, line_num, char_num)
