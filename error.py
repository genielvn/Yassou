#!/usr/bin/python3

import logging
import sys
import os

class Error(Exception):
    def __init__(self):
        self.error_name = ''
        self.details    = ''

    def invoke(self, component_file):
        logging.error(f'{os.path.basename(component_file)}: {self.details} [{self.error_name}]')
        sys.exit(1)

class InvalidFileTypeError(Error):
    def __init__(self, file):
        super().__init__()
        self.error_name = 'InvalidFileType'
        self.details    = f'File \"{file}\" is an invalid PPF file.'

class InvalidCharacterError(Error):
    def __init__(self, line_num, char_num, char):
        super().__init__()
        self.error_name = 'InvalidCharacter'
        self.details    = f'Invalid character detected at line {line_num+1}, character {char_num+1}: \"{char}\"'
