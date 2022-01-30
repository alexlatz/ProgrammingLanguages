
# AlphaLang (version Zero Zero One)

## The programming language with only letters

This language was developed by Alex Latz in two thousand twenty-two
in Honors Programming Languages at The Westminster Schools.

However, instead of some *lesser* non-numerical languages,
Alpha focuses on pure WPM.
Alpha's goal is to allow even faster programming with a
modified and (IMO) improved C-style syntax.

Due to this focus, Alpha uses a large number of reserved symbols and
almost all uppercase letters. We call this style **platypuscase**.

![Flat Platypus](https://www.nhm.ac.uk/content/dam/nhmwww/discover/platypus-puzzle/platypus-full-width.jpg.thumb.1160.1160.jpg)

## Variables

### Initialization

Alpha is a strongly-typed language with soft initialization.
Although type is not indicated before naming the variable, the interpreter
will detect and set the type internally after a variable's first initialization.

This helps cut down on boilerplate code, while still 
preventing hell from freezing over.

All variables are declared with the keyword `let` and optionally initialized with the keyword `be` (where `be` is equal to `=` in C-style).

```
let tmp be true  
let name be five
let var
```

### Modification

Variables can be modified later through the `be` keyword, although they must remain typed.


