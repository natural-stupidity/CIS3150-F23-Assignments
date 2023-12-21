# CIS 3150 Assignment 4

## How to run:

1. Open the folder / files in an IDE and run:
```
make
```
This command will create two programs: a4 and a4ng.
  - **a4**: The graphical version of the program.
  - **a4ng**: The non-graphical version of the program.

2. To run the program, use one of the following commands:
  - To run the GRAPHICS version, run:
    ```
    ./a4 <file name>
    ```
  - To run the NON GRAPHICS version, run:
    ```
    ./a4ng <file name>
    ```
**Note: For the graphical version, type "q" or "Ctrl-C" to exit the program.**
    
## Assignment Description:

Write an interpreter which reads a program file and then executes the code stored in the file. 

### 1. The Language
The program file has the format:
```
<lineNumber> <command> <arg1> <arg2> <arg3>
```
The number of arguments depends on the command. There can be 0, 1, 2, or 3 args included on each line.

There are no lines without a <lineNumber> and a <command> with the associated arguments. 

The language uses the following keywords:
```
int <var>      -declare a variable as an int with the name <var>
set <var> #    -set the variable <var> equal to #
begin          -start program, after all declarations, there is one begin command
end            -last line of the program, there is one end command

add <var> #    -these functions perform arithmetic operations using <var> and #,  for example: add <var> 3 means <var> = <var> + 3
div <var> #

print <var1> <var2> string     -if the program is using the graphics mode it prints the string at row == <var1> and  column == <var2> 
                               -if not in graphics mode it prints the contents of <var1>, then <var2>, then the string to stdout.
                               -the string cannot have any spaces within it

goto <lineNumber>        -jump to the <lineNumber> and begin executing there
                         -you cannot jump before the begin line
                         -goto can jump anywhere between the begin and end commands, including both of those commands

if <var> <op> <var>      -<op> can be eq, ne, gt, gte, lt, lte
                         -evaluates the expression
                         -if true then execute the next line
                         -if false then jump over next line
```

The begin and end commands perform no operations. Think of them as brackets which define a block of code.

```
Sample Program - pgm1
1 int a 
3 int b
5 begin
10 set a 5
12 add a 2
15 set b 7
40 if a eq b
50   print a b a==b
100 end
```
Without curses the output will be:
```
7 7 a==b
```

Sample Program - pgm3
```
1 int a 
2 int b
5 begin
8 set a 5
9 set b 0
50 add b 1
55 print  b b *
60 if a ne b
70   goto 50
100 end
 ```

Without curses the output will be:
```
1 1 *
2 2 *
3 3 *
4 4 *
5 5 *
```
