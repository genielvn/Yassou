#!/usr/bin/python3

import argparse
import logging
from lexer import Lexer

from error import Error, InvalidFileTypeError

if __name__ == "__main__":
    cmd_parser = argparse.ArgumentParser(
            description="PseudoFile language interpreter.")
    
    cmd_parser.add_argument('filename', metavar="FILE",
                            type=argparse.FileType('r', encoding='UTF-8'),
                            help='.ppf file to process')
    cmd_parser.add_argument('-d', '--debug',
                            action='store_true',
                            help='enable debugging')

    cmd_args = cmd_parser.parse_args()

    # Enable debug mode
    if cmd_args.debug:
        logging.basicConfig(level=logging.DEBUG,
                            format='%(levelname)s: %(message)s')
        logging.debug('Debugging is enabled.')
    
    try:
        file = cmd_args.filename
        
        # Detect file type
        if not file.name.endswith('.ppf'):
            raise InvalidFileTypeError(file.name)

        lexer = Lexer(cmd_args.debug)
        symtable = lexer.getSymTable(file)

    except Error as error:
        error.invoke(__file__)

    # symtable is the list of tokens
    #print(result)

    
