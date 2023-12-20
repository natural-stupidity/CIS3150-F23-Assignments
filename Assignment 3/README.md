# CIS 3150 Assignment 3

## Assignment Description:

Given the context free grammar for regular expressions write a recursive descent parser. 

### 1. The Context Free Grammar
Implement the parser using the following grammar. Each rule will be replaced by a recursive function call.

The grammar is written in Backus–Naur form (BNF). The production rules are written differently from the CFG rules. Variables appear inside pairs of greater > and less than signs <, terminals can appear within double quotes "..." when needed for clarity, the arrow operation is replaced with the ::= symbol. 

The CFG grammar for regular expressions is:
```
<regexp> ::= <concat>
<concat> ::= <term><concat> | <term> | <endofline>
<term> ::= <star> | <element> 
<star> ::= <element>*
<element> ::= <group> | <char>
<group> ::= (<regexp>)
<char> ::= <alphanum> | <symbol> | <white> 
<alphanum> ::= A | B | C | ... | Z |
               a | b | c | ... | z |
               0 | 1 | 2 | ... | 9
<symbol> ::= ! | " | # | $ | % | & | ' | + | , | - | . | 
             / | : | ; | < | = | > | ? | @ | [ | ] | ^ | 
             _ | ` | { | } | ~ | <sp> | <metachars>
<sp> ::= " "
<metachar> ::= \ | "|" |  <white>
<white> ::= <tab> | <vtab> | <nline>
<tab> ::= '\t'
<vtab> ::= '\v'
<nline> ::= '\n'
<endofline> ::= '\0'
```
### 2. Input
The input for the program will be a file containing the regular expression. The regular expression can contain any of the terminals listed in the grammar and the brackets ().

Escape characters will appear as their single ASCII character equivalents. For example, the newline is represented as \n in the grammar but it will appear as the single character '\n' in the file. Similarly, the tab appears as \t in the grammar but it will be a tab in the input file.

There can be newlines within the regular expression. 

Assume the regular expression will not be more than 1000 characters in length.

Read the filename from the command line. There are no other command line arguments.

### 3. Output

Add your parser to the starting code and use the print() function to
display the parse tree. 

There are several global variables in the code that may be useful to
draw the tree. You can use these variable if you wish. You cannot add global
more variables.

Print out the names of the variables for the rules that do not directly
represent terminals. These are regexp, concat, term, star, element,
group, and char. These are the names that should appear in the output.
If a terminal matches a character in the regular expression then print
match to the screen. If the terminal does not match the current character
in the regex then print fail.

Calling a function that represents a grammar replacement rule increases the
depth of the tree. Moving to each replacement rule after the first in an AND
(e.g. in <term><concat> it would be <concat>) or and OR 
(e.g. in <star> | <element> it would be <element>) increases the width of
the tree. Normally, depth can increase by 1 per replacement and with can
increase by 10 per replacement.

The output when a terminal matches a character in the regex string
should be "match" or "fail" (without the double quotes).  Only one of
these strings should be printed for each character in the regular expression. 
This means the bottom most element of each branch will end in either the
string "pass" or "fail". These will always appear at the bottom of a column
(this represents an end of a branch).

Each time you match a character in the regex to a terminal you should advance the position marker in the regex. Do not do this for the end of line character. It needs to be the last character in the string so it can be matched when at the end of the tree traversal.

The starting code will display the first variable <regex> on the screen
when it is run. Add recursive functions for the other rules.  

### 4. Sample Output
![pic0_20231028002158418](https://github.com/msirna/CIS3150-F23-Assignments/assets/91761269/2783a73d-1e41-4ca6-be3e-f5654ea0e78e)
<br/>

![pic1_20231028002158485](https://github.com/msirna/CIS3150-F23-Assignments/assets/91761269/6457351b-603a-47de-a72f-f4b0388408e6)
<br/>

![pic0_20231101214753548](https://github.com/msirna/CIS3150-F23-Assignments/assets/91761269/7f911ab2-acea-478f-89e1-0f922c86805f)

## How to run:

1. Open the folder / files in an IDE and run:
```
make
```
2. To run the program, use the following command:
```
./a3 <file name>
```
