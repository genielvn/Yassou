#!/usr/bin/python3

# accepted input characters
UPPERCASE_LETTERS       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
LOWERCASE_LETTERS       = UPPERCASE_LETTERS.lower()
LETTERS                 = UPPERCASE_LETTERS + LOWERCASE_LETTERS
SYMBOLS                 = '_./*+-()~!|&=\\%^'
WHITESPACE              = ' \t'
DIGITS                  = '0123456789'

# tokens
TOKEN_DICT = {
        '+'  : 'PLUS',
        '-'  : 'MINUS',
        '*'  : 'MULTIPLY',
        '/'  : 'DIVIDE',
        '%'  : 'MODULO',
        '^'  : 'RAISE',
        '('  : 'PARENTHESIS_OPEN',
        ')'  : 'PARENTHESIS_CLOSE',
        '~'  : 'COMMENT',
        '!!' : 'NOT',
        '||' : 'OR',
        '&&' : 'AND',
        '='  : 'ASSIGNMENT',
        '==' : 'EQUALS'
        }

# keywords/reserved words
RESERVED_WORDS = ['FOR', 'TO', 'BY', 'DO', 'WHILE',
                  'IF', 'ELSE', 'THEN', 'INPUT', 'OUTPUT',
                  'INTEGER', 'STRING', 'DECIMAL', 'BOOLEAN',
                  'TRUE', 'FALSE', 'SET', 'AS']

# to be deleted
TTYPE_INT               = 'INTEGER'
TTYPE_DECIMAL           = 'DECIMAL'
TTYPE_PLUS              = 'PLUS'
TTYPE_MINUS             = 'MINUS'
TTYPE_MULTIPLY          = 'MULTIPLY'
TTYPE_DIVIDE            = 'DIVIDE'
TTYPE_MODULO            = 'MODULO'
TTYPE_RAISE             = 'RAISE'
TTYPE_OPEN_PARENTHESIS  = 'PARENTHESIS_OPEN'
TTYPE_CLOSE_PARENTHESIS = 'PARENTHESIS_CLOSE'

class Tokens():
    def __init__(self, type_, value=None, pos_start=None, pos_end=None):
        self.type = type_
        self.value = value
    
    def __repr__(self):
         if self.value:
             return f'{self.type}: {self.value}'
         return f'{self.type}'
