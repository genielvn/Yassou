#!/usr/bin/python3

from enum import Enum
import logging
from tokens import *
from constants import *
from error import Error, InvalidCharacterError, StringNotClosedError

class Mode(Enum):
    NORMAL      = 1 # Allows concatenation
    INDENT      = 2 # Lexer handles indention level in indent mode
    STRING      = 3 # Lexer concatenates all characters until ", priority
    COMMENT     = 4 # Lexer skips character except ~ in comment mode, priority
    NOCONCAT    = 5 # Lexer forces no concatenation
    ESC_SEQ     = 6 # Escape sequence in string mode

class Lexer():
    def __init__(self, debugging, file):
        self.debugging  = debugging
        self.file       = file
        self.tstack     = []                # temporary stack to store token values
        self.mode       = Mode.INDENT       # a line starts with indention.
        
        if self.debugging:
            logging.basicConfig(level=logging.DEBUG,
                                format='%(levelname)s: %(message)s')
        else:
            logging.basicConfig(format='%(levelname)s: %(message)s')
        
        logging.debug('Lexer object instantiated.')
        
        self.symtable   = []
        self.createSymTable()
    
    def getSymTable(self):
        return self.symtable

    def createSymTable(self):
        for line_num, line in enumerate(self.file):
            self.mode = Mode.INDENT
            self.handleLine(line_num, line)

        if self.debugging: self.createDebugFile()

    def handleLine(self, line_num, line):
        try:
            for char_num, char in enumerate(line):
                self.handleCharacter(line_num, line, char_num, char)

            if self.mode is Mode.STRING:
                raise StringNotClosedError(char, line_num, char_num) 

            if len(self.tstack) > 1: self.symtable.extend(self.tstack)
            self.tstack = []

        except Error as error:
            error.invoke(__file__)

    def handleCharacter(self, line_num, line, char_num, char):
        try:
            if self.mode is Mode.COMMENT: self.handleComment(char)
            elif self.mode is Mode.STRING: self.handleString(char, line_num, char_num)
            elif char in DELIMITERS: self.handleDelimiter(char, line_num, char_num)
            elif char.isspace(): self.handleWhitespace(char, line_num)
            elif char in SYMBOLS: self.handleSymbol(char, line_num, char_num)
            elif char in DIGITS: self.handleDigit(char, line_num, char_num)
            elif char in LETTERS: self.handleLetter(char, line_num, char_num)
            else: raise InvalidCharacterError(char, line_num, char_num)

        except Error as error:
            error.invoke(__file__)

    def handleComment(self, char):
        if char == '~': self.mode = Mode.NOCONCAT

    def handleString(self, char, line_num, char_num):
        if not self.getPrevTokenType() == 'STRING':
            self.tstack.append(StringToken(line_num, char_num))

        if char == '\"':
            self.mode = Mode.NOCONCAT
            self.tstack.append(
                    DelimiterToken(DELIMITER_DICT[char], char, line_num, char_num))
            return

        self.tstack[-1].concatValue(char)

    def handleWhitespace(self, char, line_num):
        if self.mode is Mode.INDENT:
            try: self.tstack[-1].addIndentLevel()
            except: self.tstack.append(IndentToken(line_num))
            return
        
        # self.mode is Mode.NORMAL or Mode.NOCONCAT, forces no concatenation
        self.mode = Mode.NOCONCAT

    def handleDelimiter(self, char, line_num, char_num):
        try:
            self.tstack.append(
                    DelimiterToken(DELIMITER_DICT[char], char, line_num, char_num))
            if char == '\"': self.mode = Mode.STRING
            else: self.mode = Mode.NOCONCAT

        except KeyError:
            self.tstack.append(
                    DelimiterToken('SENTENCE_BREAK', '\\n', line_num, char_num))

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
            self.tstack[-1].concatValue(char)
        
        elif self.getPrevTokenType() == 'DIVIDE' and char == '/':
            self.tstack[-1].concatValue(char)
        
        else:
            self.tstack.append(SymbolToken(SYM_DICT[char], char, line_num, char_num))
                        
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

        # a decimal without no integer part
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
        
    def getPrevTokenType(self):
        try:
            return self.tstack[-1].getType()
        except IndexError:
            return None

    def createDebugFile(self):
        with open('.lexer_debug.txt', 'wt') as debug_file:
            logging.debug((f"Symbol table of {self.file.name}:\n"
                          f"Line\tChar\t{'Type':20}Value"))
            self.printTokenLine(debug_file) 
            logging.debug("The symbol table has been saved to .lexer_debug.txt.")

    def printTokenLine(self, file):
        for token in self.symtable:
            string = (f"{token.location['line_num']}\t"
                      f"{token.location['char_num']}\t"
                      f"{token.type:20}"
                      f"{token.value}"
                     )
            print(string)
            file.write(string + '\n')
