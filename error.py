class Error:
    def __init__(self, pos_start, pos_end, error_name, details):
        self.pos_start = pos_start
        self.pos_end = pos_end
        self.error_name = error_name
        self.details = details
    
    def description(self):
        result  = f'{self.error_name}: {self.details}\n'
        if self.pos_start == None or self.pos_end == None:
            return result
        
        result += f'File {self.pos_start.fn}, line {self.pos_start.line_number + 1}'
        return result

class InvalidCharacter(Error):
    def __init__(self, pos_start, pos_end, details):
        super().__init__(pos_start, pos_end, 'Invalid Character', details)

class InvalidFileType(Error):
    def __init__(self, pos_start, pos_end, details):
        super().__init__(pos_start, pos_end, 'Invalid File Type', details)
