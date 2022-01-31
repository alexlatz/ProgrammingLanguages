
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

## Basic Syntax

### Line Endings

Most symbols have a one-to-one replacement in Alpha. One notable exception is the semicolon, as statements in Alpha are instead finished with a newline character.
However, statements in Alpha can be multi-line with the use of an indent.

ex. 
```
let name be one X
    two
let nametwo be two times one
```

### Creating Scope

All scope in Alpha is separated by the `L` and `J` characters, instead of `{}` in C. 
Additionally, all functions and statements that usually require the `()` parentheses are replaced by `C` and `D`.
ex. 
```
if Cone is twoS L
    rtrn false
J
```

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

## Functions

Functions can be declared using the `fxn` keyword. To provide arguments to a function, enclose a space-separated list of variable names with `CD`.

```
fxn squareCxD L
    rtrn x X x
J

squareCfiveD
```

Alpha intentionally does not support functional programming (due to a high number of symbols used in functional languages).

## Operators

Alpha's operators are fairly different from most languages.

| Operator |         Behavior        |                  Usage                 |
|:--------:|:-----------------------:|:--------------------------------------:|
|   `be`   |        Assignment       |            `varname be two`            |
|   `add`  |         Addition        |              `one add two`             |
|   `sub`  |       Subtraction       |              `two sub one`             |
|    `X`   |      Multiplication     |               `one X two`              |
|   `div`  |         Division        |              `two div one`             |
|   `mod`  |          Modulo         |              `two mod one`             |
|  `addbe` |       Plus-equals       |             `var addbe one`            |
|  `subbe` |       Minus-equals      |             `var subbe one`            |
|   `Xbe`  |       Times-equals      |              `var Xbe one`             |
|  `divbe` |      Divide-equals      |             `var divbe one`            |
|  `modbe` |      Modulo-equals      |             `var modbe one`            |
|   `inc`  |        Increment        |                `var inc`               |
|   `dec`  |        Decrement        |                `var dec`               |
|  `less`  |       Lesser than       |            `var less vartwo`           |
|  `more`  |        More than        |            `var more vartwo`           |
|   `is`   |         Equal to        |             `var is vartwo`            |
| `lessis` | Lesser than or equal to |           `var lessis vartwo`          |
| `moreis` |  More than or equal to  |           `var moreis vartwo`          |
|   `not`  |       Logical NOT       |          `not Cvar is vartwoD`         |
|   `and`  |       Logical AND       | `var is vartwo and vartwo is varthree` |
|   `or`   |        Logical OR       |  `var is vartwo or vartwo is varthree` |
