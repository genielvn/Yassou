# Yassou
Yassou is a beginner-friendly, general-purpose programming language coined after the Greek word meaning "Hello." This language is simple and easy to understand as it uses natural syntax for better readability and writability. It serves as the bridge between pseudocode and other general-purpose programming languages, which helps programmers and readers simulate the logic of the code to spot errors and inconsistencies before the actual programming process.  

## Developing
Compile the Yassou Interpreter using the Makefile first.
```
make
```

To run a (.yass) file:
```
./yassou [FILE] [-d]
```

## Checklist

### Lexical Analyzer
- [X] Open file
- [X] Check file extension, reject file if not .ppf
- [X] Integer and Decimal Checking
- [X] String Checking
- [X] Comment Checking
- [X] Identifier Checking
- [X] Keyword Checking
- [X] Indentation Checking
  - [X] Conditional Keywords
    - [X] IF
    - [X] ELSE
    - [X] THEN
  - [X] Iterative Keywords
    - [X] FOR
    - [X] TO
    - [X] BY
    - [X] DO
    - [X] WHILE
  - [X] I/O Keywords
    - [X] INPUT
    - [X] OUTPUT
  - [X] Data Type Keywords
    - [X] INTEGER
    - [X] DECIMAL
    - [X] STRING
    - [X] BOOLEAN
  - [X] Declaration/Assignment Keywords
    - [X] SET
    - [X] AS
- [X] Reserved Words
  - [X] Boolean Values
    - [X] TRUE
    - [X] FALSE
- [X] Operators
  - [X] Arithmetic Operators
    - [X] +
    - [X] -
    - [X] *
    - [X] /
    - [X] //
    - [X] %
    - [X] ^
  - [X] Boolean Operators
    - [X] <
    - [X] >
    - [X] <=
    - [X] >=
    - [X] =
    - [X] =/=
  - [X] Logical Operators
   - [X] !!
   - [X] &&
   - [X] ||
- [X] Delimiters and Brackets
- [X] Error Checking
- [X] Output: text file containing the symbol table

### Syntactic Analyzer

...

### Semantic Analyzer
- [ ] Conversion from INTEGER to DECIMAL when division results to a floating number
- [ ] Unique ID for IDENTIFIERs
...
