DIGITS                  = '0123456789'
TTYPE_INT               = 'INTEGER'
TTYPE_DECIMAL           = 'DECIMAL'
TTYPE_PLUS              = 'PLUS'
TTYPE_MINUS             = 'MINUS'
TTYPE_MULTIPLY          = 'MULTIPLY'
TTYPE_DIVIDE            = 'DIVIDE'
TTYPE_MODULO            = 'MODULO'
TTYPE_RAISE             = 'RAISE'
TTYPE_OPEN_PARENTHESIS  = 'OPEN_PARENTHESIS'
TTYPE_CLOSE_PARENTHESIS = 'CLOSE_PARENTHESIS'

class Tokens():
    def __init__(self, type_, value=None, pos_start=None, pos_end=None):
        self.type = type_
        self.value = value
    
    def __repr__(self):
         if self.value:
             return f'{self.type}: {self.value}'
         return f'{self.type}'