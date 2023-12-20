// Michael Sirna
// 1094947
// 2023-11-24
// CIS3150 Assignment 4

/* Parse tree using ASCII graphics - original NCurses code from "Game Programming in C with the Ncurses Library"
    https://www.viget.com/articles/game-programming-in-c-with-the-ncurses-library/
    and from "NCURSES Programming HOWTO" http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef NOGRAPHICS
#include <ncurses.h>
#include <unistd.h>
#endif

#define SCREENSIZE 200

#ifndef NOGRAPHICS
// curses output
// row indicates in which row the output will start - larger numbers move down
// col indicates in which column the output will start - larger numbers move to the right
// when row,col == 0,0 it is the upper left hand corner of the window
void print(int row, int col, char *str) {
    mvprintw(row, col, str);
}
#endif

#define MAX_NAME_LENGTH 11
#define MAX_ARG_LENGTH 101
#define MAX_LIST_LENGTH 1024

// Struct for all operations / lines in the file
struct operation {
    int index;
    int lineNumber;
    char command[MAX_NAME_LENGTH];
    char arg1[MAX_ARG_LENGTH];
    char arg2[MAX_ARG_LENGTH];
    char arg3[MAX_ARG_LENGTH];
} Operation;

// Struct for variables
struct variable {
    int index;
    char name[MAX_NAME_LENGTH];
    int value;
} Variable;

// Closes curses if it's open
void checkNoGraphics() {
#ifndef NOGRAPHICS
    endwin();
#endif
}

// Searches the operation list for a given line number. Returns the index if found. Exits on fail
int getOpIndex(struct operation opList[MAX_LIST_LENGTH], int listLen, int lnToFind) {
    for (int i = 0; i < listLen; i++) {
        if (opList[i].lineNumber == lnToFind)
            return i;
    }
    checkNoGraphics();
    printf("\nERROR: Could not find operation with line number %d.\nPlease check your input file.\nExiting Program....\n\n", lnToFind);
    exit(-1);
}

// Searches the variables list for a given variable name. Returns the index if found. Exits on fail
int getVarIndex(struct variable varList[MAX_LIST_LENGTH], int listLen, char nameToFind[MAX_NAME_LENGTH]) {
    for (int i = 0; i < listLen; i++) {
        if (strcmp(varList[i].name, nameToFind) == 0)
            return i;
    }
    checkNoGraphics();
    printf("\nERROR: Could not find variable with the name '%s'.\nPlease check your input file.\nExiting Program....\n\n", nameToFind);
    exit(-1);
}

// Calculates the new value of the value integet based on the type of operation we're using.
void calcVar(int *value, char type[5], int newValue, int lineNumber) {
    if (strcmp(type, "add") == 0)
        *value += newValue;
    if (strcmp(type, "sub") == 0)
        *value -= newValue;
    if (strcmp(type, "mult") == 0)
        *value *= newValue;
    if (strcmp(type, "div") == 0) {
        if (newValue == 0) {
            checkNoGraphics();
            printf("\nERROR: Attempted to divide by zero on line %d.\nPlease check your input file.\nExiting Program....\n\n", lineNumber);
            exit(-1);
        }
        *value /= newValue;
    }
}

// Compares two variables based on the type of comparison we're doing. Returns true if the comparison is true. False otherwise.
int evaluateVars(struct variable varList[MAX_LIST_LENGTH], int varListLen, char var1[MAX_NAME_LENGTH], char var2[MAX_NAME_LENGTH], char type[5]) {
    int var1Value = varList[getVarIndex(varList, varListLen, var1)].value;
    int var2Value = varList[getVarIndex(varList, varListLen, var2)].value;
    if (strcmp(type, "eq") == 0)
        return var1Value == var2Value;
    if (strcmp(type, "ne") == 0)
        return var1Value != var2Value;
    if (strcmp(type, "gt") == 0)
        return var1Value > var2Value;
    if (strcmp(type, "gte") == 0)
        return var1Value >= var2Value;
    if (strcmp(type, "lt") == 0)
        return var1Value < var2Value;
    if (strcmp(type, "lte") == 0)
        return var1Value <= var2Value;
    return 0;
}

int main(int argc, char *argv[]) {
    // Error checking on the file name argument
    if (argc != 2) {
        printf("\nERROR: Incorrect Syntax.\nSyntax:\n- Graphics Version:\t./a4 <file name>\n- No Graphics Version:\t./a4ng <file name>\n\n");
        return -1;
    }

#ifndef NOGRAPHICS
    // initialize ncurses
    initscr();
    noecho();
    cbreak();
    timeout(0);
    curs_set(FALSE);
#endif

    // Open the file and check if it's valid.
    char *fileName = argv[1];
    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        checkNoGraphics();
        printf("\nERROR: Failed to open the file \'%s\'.\nExiting program...\n\n", fileName);
        return -1;
    }

    // Init variables
    char temp[1024];
    struct operation opList[MAX_LIST_LENGTH];
    struct variable varList[MAX_LIST_LENGTH];

    int lineIndex = 0;

    // Loop through the entire file
    while (fgets(temp, sizeof(temp), fp) != NULL) {
        // Set the end of the current line to NULL so that this doesn't break anything
        temp[strlen(temp) - 1] = 0;

        // Split the string
        char *token = strtok(temp, " ");

        // Create a new struct an initialize its values (strings become NULL)
        struct operation newOp;
        newOp.index = lineIndex;
        newOp.arg1[0] = 0;
        newOp.arg2[0] = 0;
        newOp.arg3[0] = 0;

        // For every argument in this line, assign that value to the struct based on its position.
        for (int i = 0; token != NULL; i++) {
            if (i == 0)
                newOp.lineNumber = atoi(token);
            if (i == 1)
                strncpy(newOp.command, token, MAX_NAME_LENGTH);
            if (i == 2)
                strncpy(newOp.arg1, token, MAX_ARG_LENGTH);
            if (i == 3)
                strncpy(newOp.arg2, token, MAX_ARG_LENGTH);
            if (i == 4)
                strncpy(newOp.arg3, token, MAX_ARG_LENGTH);

            token = strtok(NULL, " ");
        }
        // Add the new struct to the list and go to the next line
        opList[lineIndex] = newOp;
        lineIndex++;
    }

    // Now that everything is saved to the list, we can loop through each operation in the list
    int varCount = 0;
    int beginLineNumber = -1;
    for (int pc = 0; pc < lineIndex; pc++) {
        // If the command is begin, we store the number to check gotos.
        if (strcmp(opList[pc].command, "begin") == 0)
            beginLineNumber = opList[pc].lineNumber;

        // If the command is end, break and exit the program
        if (strcmp(opList[pc].command, "end") == 0)
            break;

        // If the command is int, create a new variable and add it to the list. Increment the variable count too.
        if (strcmp(opList[pc].command, "int") == 0) {
            struct variable newVar;
            newVar.index = varCount;
            strcpy(newVar.name, opList[pc].arg1);
            newVar.value = 0;
            varList[varCount] = newVar;
            varCount++;
        }

        // If the command is set, get the variable's index and set its value to the argument
        if (strcmp(opList[pc].command, "set") == 0) {
            int index = getVarIndex(varList, varCount, opList[pc].arg1);
            varList[index].value = atoi(opList[pc].arg2);
        }

        // If we command is a mathimatical operation, get the index, then send it to the calcVar funciton to calculate the new value
        if (strcmp(opList[pc].command, "add") == 0 || strcmp(opList[pc].command, "sub") == 0 || strcmp(opList[pc].command, "mult") == 0 || strcmp(opList[pc].command, "div") == 0) {
            int index = getVarIndex(varList, varCount, opList[pc].arg1);
            calcVar(&varList[index].value, opList[pc].command, atoi(opList[pc].arg2), opList[pc].lineNumber);
        }

        // If the command is print, get the values of each argument, then print based on the version of this program
        if (strcmp(opList[pc].command, "print") == 0) {
            int x = varList[getVarIndex(varList, varCount, opList[pc].arg1)].value;
            int y = varList[getVarIndex(varList, varCount, opList[pc].arg2)].value;
#ifndef NOGRAPHICS
            print(x, y, opList[pc].arg3);
#else
            printf("%d %d %s\n", x, y, opList[pc].arg3);
#endif
        }

        // If the command is goto, get the operation's index in the operation list, and set the program counter to that value
        // It needs to be -1 'cause when we use continue, pc is incremented by 1 in the loop
        if (strcmp(opList[pc].command, "goto") == 0) {
            int opIndex = getOpIndex(opList, lineIndex, atoi(opList[pc].arg1));

            // If the goto goes to an operation with a line number less than the begin, then we exit the program.
            if (opList[opIndex].lineNumber < beginLineNumber) {
                checkNoGraphics();
                printf("\nERROR: Attempted to goto a line before begin on line %d.\nPlease check your input file.\nExiting Program....\n\n", opList[pc].lineNumber);
                exit(-1);
            }
            pc = opIndex - 1;
            continue;
        }

        // If the command is an if operation, check if evaluate vars FAILS.
        // If it fails, we increment the pc to skip two lines (remember, the end of the loop increments by 1, so we're essentially incrementing twice)
        if (strcmp(opList[pc].command, "if") == 0) {
            if (!evaluateVars(varList, varCount, opList[pc].arg1, opList[pc].arg3, opList[pc].arg2))
                pc++;
        }
    }

    int c;
#ifndef NOGRAPHICS
    /* loop until the 'q' key is pressed */
    while (1) {
        c = getch();
        if (c == 'q') break;
    }

    // shut down ncurses
    endwin();
#endif
}
