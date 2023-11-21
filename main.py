#!/usr/bin/python3

import argparse
import logging
from lexer import Lexer
from error import InvalidFileTypeError

class Interpreter():
    def __init__(self):
        self.arguments  = self.getAttributes()
        self.debugging  = self.debug()
        self.input_file = self.getFile()
        self.lexer      = Lexer(self.debugging, self.input_file)
        self.symtable   = self.lexer.getSymTable()

    def getAttributes(self):
        parser = argparse.ArgumentParser(
                description='PseudoFile language interpreter.')

        parser.add_argument('filename',
                            metavar='FILE',
                            type=argparse.FileType('r',encoding='UTF-8'),
                            help='.ppf file to process')

        parser.add_argument('-d', '--debug',
                            action='store_true',
                            help='enable debugging')

        return parser.parse_args()

    def debug(self):
        if self.arguments.debug:
            logging.basicConfig(level=logging.DEBUG,
                                format='%(levelname)s: %(message)s')
            logging.debug('Debugging is enabled.')
        else:
            logging.basicConfig(format='%(levelname)s: %(message)s')
        return self.arguments.debug

    def getFile(self):
        try:
            file = self.arguments.filename

            # file detection
            if not file.name.endswith('.ppf'):
                raise InvalidFileTypeError(file.name)

        except InvalidFileTypeError as error:
            error.invoke(__file__)

        else:
            return file

if __name__ == "__main__":
    interpreter = Interpreter()

