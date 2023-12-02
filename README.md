# PseudoFile Language
PseudoFile Language (PFL) is a general-purpose programming language named after the terms "pseudocode" and "compile", which means to compile pseudocode.

## Developing
To run a (.ppf) file:
```
python3 main.py [FILE]
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
    - [X] %
    - [X] ^
    - [ ] ONE MORE OPERATOR PLEASE
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
