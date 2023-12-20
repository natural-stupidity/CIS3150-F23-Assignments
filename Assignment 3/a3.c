// Michael Sirna
// 1094947
// 2023-11-03
// CIS3150 Assignment 3

/* Parse tree using ASCII graphics
        -original NCurses code from "Game Programming in C with the Ncurses Library"
         https://www.viget.com/articles/game-programming-in-c-with-the-ncurses-library/
         and from "NCURSES Programming HOWTO"
         http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SCREENSIZE 200

// current position in string
int position;
// indentation
int width;

int regexp(char *, int);

// draws text on screen at location (depth, width) using ncurses
void print(int depth, char *str) {
    // offset in depth, used to break line and move it down the screen
    // when the max line length is reached
    static int offset = 0;

    // if the output reaches the max width of the window then
    // move down and back to the left edge (carriage return and newline)
    if (width > 150) {
        width = 0;
        offset += 15;
    }
    // ncurses command to draw textstr
    mvprintw(depth + offset, width, str);
}

int drawTree(char *regex) {
    char c;
    int depth = 0;

    // start in leftmost position
    width = 0;
    position = 0;

    // ncurses clear screen
    clear();

    // parse tree
    // parsing functions calls print to draw output
    // -regex is the string containing the regular expression to be parsed
    // -depth contains the current depth in the parse tree, it is
    //    incremented with each recursive call
    regexp(regex, depth);

    refresh();

    // read keyboard and exit if 'q' pressed
    while (1) {
        c = getch();
        if (c == 'q') return (1);
    }
}

// Prints the nodes for the <eoln> branch. Return true if the current character is the end of line marker \0
int endofline(char *regex, int depth) {
    print(depth, "eoln");

    if (*regex == '\0') {
        print(depth + 1, "match");
        return 1;
    }

    print(depth + 1, "fail");
    return 0;
}

// Return true if the current character is a tab, vertical tab, or new line
int white(char *regex, int depth) {
    return (*regex == '\t' || *regex == '\v' || *regex == '\n');
}

// Return true if the current character is \\ or |
int metachar(char *regex, int depth) {
    return (*regex == '\\' || *regex == '|' || white(regex, depth + 1));
}

// Return true if the current character is any of these symbols in this long if statement. Note, this includes the <sp> case
int symbol(char *regex, int depth) {
    return (*regex == '!' || *regex == '"' || *regex == '#' || *regex == '$' || *regex == '%' || *regex == '&' || *regex == '\'' || *regex == '+' || *regex == ',' || *regex == '-' ||
            *regex == '.' || *regex == '/' || *regex == ':' || *regex == ';' || *regex == '<' || *regex == '=' || *regex == '>' || *regex == '?' || *regex == '@' || *regex == '[' ||
            *regex == ']' || *regex == '^' || *regex == '_' || *regex == '`' || *regex == '{' || *regex == '}' || *regex == '~' || *regex == ' ' || metachar(regex, depth + 1));
}

// Returns true if the current character is alphanumeric.
int alphanum(char *regex, int depth) {
    return ((*regex >= 'A' && *regex <= 'Z') || (*regex >= 'a' && *regex <= 'z') || (*regex >= '0' && *regex <= '9'));
}

// Prints the nodes for the <char> branch. Returns true if the current character is alphanumeric, a symbol, or whitespace.
int charCheck(char *regex, int depth) {
    print(depth, "char");

    if (alphanum(regex, depth + 1) || symbol(regex, depth + 1) || white(regex, depth + 1)) {
        print(depth + 1, "match");
        return 1;
    }

    print(depth + 1, "fail");
    return 0;
}

// Prints the nodes for the <group> branch.
// Returns true if the the current string in the regex starts with a open bracket, reads a regex, then reads an end bracket - ( <regexp )
int group(char *regex, int depth) {
    print(depth, "group");

    if (*regex == '(') {

        // Check for an end bracket before starting so it doesn't seg fault
        int tempPosition = 0;
        for (; *regex != ')'; regex++){
            tempPosition++;
            if (*regex == '\0'){
                print(depth + 1, "fail");
                return 0;
            }
        }
        regex -= tempPosition;


        regex++;
        if (regexp(regex, depth + 1)){
            for (; *regex != ')'; regex++)
                position += 1;
            position++;
            return 1;
        }
    }
    print(depth + 1, "fail");
    return 0;
}

// Prints the nodes for the <element> branch. Returns true if the current character is a group of char.
int element(char *regex, int depth) {
    print(depth, "element");

    if (group(regex, depth + 1))
        return 1;

    width += 10;

    if (charCheck(regex, depth + 1))
        return 1;

    return 0;
}

// Prints the nodes for the star branch. Returns true if the current character is an element, and is followed by the * symbol.
int star(char *regex, int depth) {
    print(depth, "star");
    if (element(regex, depth + 1)) {

        // If the position is greater than 0 (i.e., we just got out of the group function), increment the regex so we can check for *
        if (position > 0)
            regex += position;
        
        // If the current char is an element, and the next element is a star,
        // we increment position to start reading everything after when we return, then return true.
        if (regex[0] == '*' || regex[1] == '*') {
            
            // Some printing stuff for the original examples we were given.
            // width += 10;
            // print(depth, "*match");
            position += 1;
            return 1;
        }

        // If the check fails, we go back in the regex so that we can match the rest of the regex.
        if (position > 0){
            regex -= position;
            position = 0;
        }
    }
    return 0;
}

// Prints the nodes for the <term> branch. Returns true if the current character is a star or element.
int term(char *regex, int depth) {
    print(depth, "term");

    if (star(regex, depth + 1))
        return 1;

    width += 10;

    if (element(regex, depth + 1))
        return 1;

    return 0;
}

// Prints the nodes for the <concat> branch. Returns true on <term><concat>, <term>, and <endofline>
int concat(char *regex, int depth) {
    print(depth, "concat");

    // Increment the regex if position > 0. Reset position after for the next round.
    if (position > 0) {
        regex += position;
        position = 0;
    }

    // <term><concat>
    if (term(regex, depth + 1)) {
        width += 10;
        if (concat(regex + 1, depth + 1))
            return 1;

    }

    width += 10;

    // <term>
    if (term(regex, depth + 1))
        return 1;

    width += 10;

    // <eoln>
    if (endofline(regex, depth + 1))
        return 1;

    return 0;
}

int regexp(char *regex, int depth) {
   
    // Some printing stuff for debugging. Prints the regex at the top of the tree before calling concat.
    // print(depth, regex);
    // depth++;
    print(depth, "regexp");
    return concat(regex, depth + 1);
}

int main(int argc, char *argv[]) {
    
    // Error checking on the file name argument
    if (argc != 2) {
        printf("\nERROR: Incorrect Syntax.\nSyntax: ./a3 <file name>\n\n");
        return -1;
    }

    // Open the file and check if it's valid.
    char *fileName = argv[1];
    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        printf("\nERROR: Failed to open the file \'%s\'\nExiting program...\n\n", fileName);
        return -1;
    }

    // Set up regex and text variables and read the file.
    char regex[1024], temp[1024];
    while (fgets(temp, sizeof(temp), fp) != NULL) {
        strcat(regex, temp);
    }

    // initialize ncurses
    initscr();
    noecho();
    cbreak();
    timeout(0);
    curs_set(FALSE);

    // traverse and draw the parse tree
    drawTree(regex);

    // shut down ncurses
    endwin();
    fclose(fp);
    fp = NULL;
}
