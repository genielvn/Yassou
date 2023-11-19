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
- [X] Check file extension, reject file if not .pfl
- [X] Integer and Decimal Checking
- [ ] Identifier Checking
- [ ] Keyword Checking
  - [ ] Conditional Keywords
    - [ ] IF
    - [ ] ELSE
    - [ ] THEN
  - [ ] Iterative Keywords
    - [ ] FOR
    - [ ] TO
    - [ ] BY
    - [ ] DO
    - [ ] WHILE
  - [ ] I/O Keywords
    - [ ] INPUT
    - [ ] OUTPUT
  - [ ] Data Type Keywords
    - [ ] INTEGER
    - [ ] DECIMAL
    - [ ] STRING
    - [ ] BOOLEAN
  - [ ] Declaration/Assignment Keywords
    - [ ] SET
    - [ ] AS
- [ ] Reserved Words
  - [ ] Boolean Values
    - [ ] TRUE
    - [ ] FALSE
- [ ] Operators
  - [ ] Arithmetic Operators
    - [ ] +
    - [ ] -
    - [ ] *
    - [ ] /
    - [ ] %
    - [ ] ^
    - [ ] ONE MORE OPERATOR PLEASE
  - [ ] Boolean Operators
    - [ ] <
    - [ ] >
    - [ ] <=
    - [ ] >=
    - [ ] =
    - [ ] =/=
- [ ] Delimiters and Brackets
- [ ] Error Checking
- [ ] Output: text file containing the symbol table

### Syntactic Analyzer

...

### Semantic Analyzer

...
