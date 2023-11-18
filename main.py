#!/usr/bin/python3

import argparse
import logging

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
        logging.info('Debugging is enabled.')

    # lexer(cmd_args.filename)
