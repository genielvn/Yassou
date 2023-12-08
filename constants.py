#!/usr/bin/python3

# accepted input characters
UPPERCASE               = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
LOWERCASE               = UPPERCASE.lower()
DIGITS                  = '0123456789'
LETTERS                 = UPPERCASE + LOWERCASE + '_'
SYMBOLS                 = './*+-~!|&=%^<>'
DELIMITERS              = ',()\n\"'
WHITESPACE              = ' \t'

# tokens
SYM_DICT = {
        '+'  : 'PLUS',
        '-'  : 'MINUS',
        '*'  : 'MULTIPLY',
        '/'  : 'DIVIDE',
        '//' : 'FLOOR_DIVIDE',
        '%'  : 'MODULO',
        '^'  : 'RAISE',
        '!' : 'NOT',
        '|' : 'OR',
        '&' : 'AND',
        '='  : 'ASSIGNMENT',
        '==' : 'EQUALITY',
        '<'  : 'LESS_THAN',
        '>'  : 'GREATER_THAN',
        '<=' : 'LT_EQUAL',
        '>=' : 'GT_EQUAL',
        '!=': 'NOT_EQUAL',
        '.'  : 'PERIOD'
        }

DELIMITER_DICT = {
        '('  : 'EXPR_BEGIN',
        ')'  : 'EXPR_TERMINATE',
        ','  : 'COMMA',
        '\"' : 'STR_DELIMITER'
        }

# reserved words
RESERVED_WORDS  = ['FOR', 'TO', 'BY', 'DO', 'WHILE',
                  'IF', 'ELSE', 'THEN', 'INPUT', 'OUTPUT',
                  'INTEGER', 'STRING', 'DECIMAL', 'BOOLEAN',
                  'TRUE', 'FALSE', 'SET', 'AS']
