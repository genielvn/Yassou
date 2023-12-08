#!/usr/bin/python3

import logging
import sys
import os

class Error(Exception):
    def __init__(self):
        self.details    = None

    def invoke(self, component_file):
        logging.error(f'{os.path.basename(component_file)}: {self.details} [{type(self).__name__}]')
        sys.exit(1)

class InvalidFileTypeError(Error):
    def __init__(self, file):
        super().__init__()
        self.details    = f'File \"{file}\" is an invalid YASS file.'

class InvalidCharacterError(Error):
    def __init__(self, char, line_num, char_num):
        super().__init__()
        self.details    = f'Invalid character detected at line {line_num+1}, character {char_num+1}: \"{char}\"'

class StringNotClosedError(Error):
    def __init__(self, char, line_num, char_num):
        super().__init__()
        self.details    = f'Expected \" at line {line_num+1}, character {char_num+1}.'

