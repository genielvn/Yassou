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
                # insert character operation here


    def getSymTable(self, file):
        if not self.symtable:
            # symtable is empty
            self.createSymTable(file)

        return self.symtable
