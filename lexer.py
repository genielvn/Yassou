#!/usr/bin/python3

from enum import Enum
import logging
from tokens import *
from constants import *
from error import Error, InvalidCharacterError, CreateDebugFileError, StringNotClosedError

class Mode(Enum):
    NORMAL      = 1 # Allows concatenation
    INDENT      = 2 # Lexer handles indention level in indent mode
    STRING      = 3 # Lexer concatenates all characters until ", priority
    COMMENT     = 4 # Lexer skips character except ~ in comment mode, priority
    NOCONCAT    = 5 # Lexer forces no concatenation

class Lexer():
    def __init__(self, debugging, file):
        self.debugging  = debugging
        self.file       = file
        self.tstack     = []                # temporary stack to store token values
        self.mode       = Mode.INDENT       # a PPF line starts with indention.
        
        if self.debugging:
            logging.basicConfig(level=logging.DEBUG,
                                format='%(levelname)s: %(message)s')
        else:
            logging.basicConfig(format='%(levelname)s: %(message)s')
        
        logging.debug('Lexer object instantiated.')
        
        self.symtable   = []
        self.createSymTable()

    def createSymTable(self):
        for line_num, line in enumerate(self.file):
            self.mode = Mode.INDENT

            for char_num, char in enumerate(line):
                try:
                    if self.mode is Mode.COMMENT: self.handleComment(char)
                    elif self.mode is Mode.STRING: self.handleString(char, line_num, char_num)

                    elif char in DELIMITERS:
                        self.handleDelimiter(char, line_num, char_num)
                    elif char.isspace():
                        self.handleWhitespace(char, line_num)
                    elif char in SYMBOLS:
                        self.handleSymbol(char, line_num, char_num)
                    elif char in DIGITS:
                        self.handleDigit(char, line_num, char_num)
                    elif char in LETTERS:
                        self.handleLetter(char, line_num, char_num)
                    else: raise InvalidCharacterError(char, line_num, char_num)

                except Error as error:
                    error.invoke(__file__)

            if len(self.tstack) != 1:   # contains <1 tokens (besides "\n")
                self.symtable.extend(self.tstack)
            self.tstack = []
        
        if self.debugging: self.createDebugFile()
        
    def handleComment(self, char):
        if char == '~':
            self.mode = Mode.NOCONCAT

    def handleString(self, char, line_num, char_num):
        if not self.getPrevTokenType() == 'STRING':
            self.tstack.append(StringToken(line_num, char_num))
        if char == '\n' and self.mode == Mode.STRING:
            raise StringNotClosedError(char, line_num, char_num)
        if char == '\"':
            self.mode = Mode.NOCONCAT
            self.tstack.append(DelimiterToken(DELIMITER_DICT[char], char, line_num, char_num))
            return
        self.tstack[-1].concatValue(char)

    def handleWhitespace(self, char, line_num):
        if self.mode is Mode.INDENT:
            try: self.tstack[-1].addIndentLevel()
            except: self.tstack.append(IndentToken(line_num))
            return
        # self.mode is Mode.NORMAL or Mode.NOCONCAT
        self.mode = Mode.NOCONCAT

    def handleDelimiter(self, char, line_num, char_num):
        if char == '\n':
            self.tstack.append(
                    DelimiterToken('SENTENCE_BREAK', '\\n', line_num, char_num))
            return
        elif char == '\"':
            self.mode = Mode.STRING

            self.tstack.append(
                    DelimiterToken(DELIMITER_DICT[char], char, line_num, char_num))
        else:
            self.tstack.append(
                    DelimiterToken(DELIMITER_DICT[char], char, line_num, char_num))
            self.mode = Mode.NOCONCAT

        # try:
        # except KeyError:    # Using newline as a dictionary key is an exception

    
    def handleSymbol(self, char, line_num, char_num):
        if char == '~':
            self.mode = Mode.COMMENT
            return
        
        # integer + dot = integer-only decimal
        if self.mode in [Mode.NOCONCAT, Mode.INDENT]:
            self.tstack.append(SymbolToken(
                SYM_DICT[char], char, line_num, char_num))
            self.mode = Mode.NORMAL
            return
    
        if self.getPrevTokenType() == 'INTEGER' and char == '.':
            self.tstack[-1].concatValue(char)
        
        # for double-character symbols
        elif self.getPrevTokenType() in ['NOT', 'ASSIGNMENT',
                        'LESS_THAN', 'GREATER_THAN'] and char == '=':
            logging.debug("entered =")
            self.tstack[-1].concatValue(char)
        
        elif self.getPrevTokenType() == 'DIVIDE' and char == '/':
            logging.debug("entered /")
            self.tstack[-1].concatValue(char)
        else:
                        self.tstack.append(SymbolToken(
                SYM_DICT[char], char, line_num, char_num))
                        
    # Note: negative numbers handled in syntax analyzer
    def handleDigit(self, char, line_num, char_num):
        if self.mode in [Mode.NOCONCAT, Mode.INDENT]:
            self.tstack.append(NumberToken('INTEGER', char, line_num, char_num))
            self.mode = Mode.NORMAL
            return

        # self.mode is Mode.NORMAL
        if self.getPrevTokenType() in ['INTEGER', 'DECIMAL',
                                         'RESERVED_WORD', 'IDENTIFIER']:
            self.tstack[-1].concatValue(char)

        elif self.getPrevTokenType() == 'PERIOD':
            self.tstack.pop()
            self.tstack.append(
                    NumberToken('DECIMAL', f'.{char}', line_num, char_num))
        else:
            self.tstack.append(NumberToken('INTEGER', char, line_num, char_num))
            
        self.mode = Mode.NORMAL
        

    def handleLetter(self, char, line_num, char_num):
        if self.mode in [Mode.NOCONCAT, Mode.INDENT]:
            self.tstack.append(WordToken(char, line_num, char_num))
            # allow concatenation for words with >1 character names
            self.mode = Mode.NORMAL
            return

        # self.mode is Mode.NORMAL
        if self.getPrevTokenType() in ['RESERVED_WORD', 'IDENTIFIER']:
            self.tstack[-1].concatValue(char)

        # scientific notation handled in syntax analyzer
        elif self.getPrevTokenType() in ['INTEGER', 'DECIMAL'] and char == 'e':
            self.tstack.append(SymbolToken('EXPONENT', 'e', line_num, char_num))
            self.mode = Mode.NOCONCAT
            return
        
        # last stack element is a number, symbol, delimiter or string
        else:
            self.tstack.append(WordToken(char, line_num, char_num))
        self.mode = Mode.NORMAL
        
    def getSymTable(self):
        return self.symtable
    
    def getPrevTokenType(self):
        if len(self.tstack) == 0:
            return None
        return self.tstack[-1].getType()

    def createDebugFile(self):
        try:
            debug_file = open('.lexer_debug.txt', 'wt')

            logging.debug(f"Symbol table of {self.file.name}:")
            for token in self.symtable:
                string = f"Line {token.location['line_num']}, Char {token.location['char_num']:3}:\t{token.type:15}\t{token.value}"

                print(" " + string)
                debug_file.write(string + '\n')
        except CreateDebugFileError as error:
            error.invoke(__file__)
        
        else:
            debug_file.close()

