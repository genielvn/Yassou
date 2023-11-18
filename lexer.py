from tokens import Tokens
from tokens import *
from error import InvalidCharacter, InvalidFileType

class Position:
    def __init__(self, index, ln, col, fn, ftxt):
        self.index = index
        self.line_number = ln
        self.column = col
        self.fn = fn
        self.ftxt = ftxt

    def advance(self, current_char):
        self.index += 1
        self.column += 1

        if current_char == '\n':
            self.line_number += 1
            self.column = 0

        return self

    def copy(self):
        return Position(self.index, self.line_number, self.column, self.fn, self.ftxt)

class Lexer():
    def __init__(self, fn, text):
        self.fn = fn
        self.text = text
        self.pos = Position(-1, 0, -1, fn, text)
        self.current_char = None
        self.advance()

    def advance(self):
        self.pos.advance(self.current_char)
        self.current_char = self.text[self.pos.index] if self.pos.index < len(self.text) else None

    def make_tokens(self):
        tokens = []

        while self.current_char != None:
            if self.current_char in ' \t':
                self.advance()
            elif self.current_char in DIGITS:
                tokens.append(self.make_number())
            elif self.current_char == '+':
                tokens.append(Tokens(TTYPE_PLUS))
                self.advance()
            elif self.current_char == '-':
                tokens.append(Tokens(TTYPE_MINUS))
                self.advance()
            elif self.current_char == '*':
                tokens.append(Tokens(TTYPE_MULTIPLY))
                self.advance()
            elif self.current_char == '/':
                tokens.append(Tokens(TTYPE_DIVIDE))
                self.advance()
            elif self.current_char == '(':
                tokens.append(Tokens(TTYPE_OPEN_PARENTHESIS))
                self.advance()
            elif self.current_char == ')':
                tokens.append(Tokens(TTYPE_CLOSE_PARENTHESIS))
                self.advance()
            else:
                pos_start = self.pos.copy()
                char = self.current_char
                self.advance()
                return [], InvalidCharacter(pos_start, self.pos, "'" + char + "'")

        return tokens, None

    def make_number(self):
        num_str = ''
        dot_count = 0

        while self.current_char != None and self.current_char in DIGITS + '.':
            if self.current_char == '.':
                if dot_count == 1: break
                dot_count += 1
                num_str += '.'
            else:
                num_str += self.current_char
            self.advance()

        if dot_count == 0:
            return Tokens(TTYPE_INT, int(num_str))
        else:
            return Tokens(TTYPE_DECIMAL, float(num_str))
        
def run(file):
    if not file.name.endswith('.ppf'):
        return [], InvalidFileType(None, None, "File must end with extension .ppf.")

    text = ''.join(file.readlines())
    lexer = Lexer(file.name, text)
    tokens, error = lexer.make_tokens()

    return tokens, error