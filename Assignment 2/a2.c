// Michael Sirna
// 1094947
// 2023-12-19
// CIS3150 Assignment 2 Version 2

#include "header.h"

int isFound = 0, currentIndex = 0, skipCount = 0, matchList[128] = {0}, mlPopulated = 0;

int main(int argc, char *argv[]) {
    // Error checking on the file name argument
    if (argc < 2) {
        printf("\nERROR: No file name argument found\nSyntax: ./a2 <file name>\n\n");
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
    char regex[1024], text[1024];
    fscanf(fp, "%s", regex);
    fscanf(fp, "%s", text);
    fclose(fp);
    fp = NULL;

    printf("\nREGEX: %s\n", regex);
    printf(" TEXT: %s\n", text);

    // match() prints all the indexes when they're found. Returns 0 if nothing is found.
    if (!match(regex, text))
        printf("\nNot found");
    printf("\n\n");

    return 0;
}


int match(char *regexp, char *text) {
    // If the regex starts with a ^ and matchhere return 1, then we just match 0 cause there's no other place the text would appear.
    if ((regexp[0] == '^') && (matchhere(regexp + 1, text))) {
        printf("\nMatch: 0");
        return 1;
    }

    while (*text != '\0') {
        skipCount = 0;

        // If there's a match, then print the current index
        if (matchhere(regexp, text) && skipCount > 0) {
            // If this is the first match, print "Match: "
            if (!isFound) {
                printf("\nMatch: ");
                isFound = 1;
            }

            // Increment the text and currentIndex to move on in the regex and text. Reset matchList if it's populated
            printf("%d ", currentIndex);
            text += skipCount;
            currentIndex += skipCount;
            if (mlPopulated){
                memset(matchList, 0, sizeof(matchList));
                mlPopulated = 0;    
            }
        }
        // If nothing is found, just go to the next character
        else {
            text++;
            currentIndex++;
        }
    }
    return isFound;
}


int matchhere(char *regexp, char *text) {
    
    // Depending on the current characters scanned in the regex, it goes to the function that would match that special character
    // If we've scanned to the end of the regex without being reset using "return 0", then we have a match.
    if (regexp[0] == '\0')
        return 1;

    if (regexp[0] == '\\')
        return matchhere(regexp + 1, text);

    if (regexp[0] == '[')
        return matchset(regexp + 1, text);

    if (regexp[1] == '?')
        return matchquestion(regexp[0], regexp + 2, text);

    if (regexp[1] == '+')
        return matchplus(regexp[0], regexp + 2, text);

    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp + 2, text);

    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';

    if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text || matchList[(int)*text])) {
        skipCount++;
        return matchhere(regexp + 1, text + 1);
    }

    return 0;
}


int matchquestion(int c, char *regexp, char *text){

    // If the character before the ? matches the text and the rest of the text matches, match
    if (*text != '\0' && (*text == c || (c == '.' && *text != '\0'))) {
        if (matchhere(regexp, text + 1)) {
            skipCount++;
            return 1;
        }
    }

    // Otherwise just check if the text matches in general
    return (matchhere(regexp, text));
}


int matchstar(int c, char *regexp, char *text) {
    char *temp;
    skipCount++;

    // For every char that matches, keep going through the text
    // This helps with something like "aaaaa" with regex "a", it only matches once.
    for (temp = text; *temp != '\0' && (c == *temp || matchList[(int)*temp] || c == '.'); temp++) {
        skipCount++;
    }

    // Now loop backwards through temp until we get to the original text. The sooner it matches, the more the text is incremented
    do {
        if (matchhere(regexp, temp)) {
            if (skipCount > 1)
                skipCount--;
            return 1;
        }
    } while (temp-- > text);

    return 0;
}


int matchplus(int c, char *regexp, char *text) {
    char *temp = text;

    // For every char that matches, keep going through the text
    while (*text != '\0' && (c == *text || matchList[(int)*text] || c == '.')) {
        text++;
        skipCount++;
    }

    // Now loop backwards through temp until we get to the original text. The sooner it matches, the more the text is incremented
    while (text > temp) {
        if (matchhere(regexp, text--))
            return 1;
        skipCount--;
    }

    return 0;
}


int matchset(char *regexp, char *text) {

    // Check negation. If true, move past, then check the next character. Return 0 if it matches.
    if (*regexp == '^' && *(regexp + 1) == *text)
        return 0;

    else {
        // Add all the ranges in the [] in the matchList array
        while (*regexp != ']' && *regexp != '\0') {
            if (*regexp < 0 || *regexp > 128) {
                printf("\nERROR: Invalid Regex Character: %c\nExiting Program....\n\n", *regexp);
                exit(-1);
            }
            // If there's a '-', loop through the range from the first char to the next. Increment by 3 to move past the range
            if (regexp[1] == '-' && regexp[2] != ']' && regexp[2] != '\0') {
                for (int i = regexp[0]; i <= regexp[2]; i++) {
                    matchList[i] = 1;
                }
                regexp += 3;
            } 
            // Otherwise just add the character to the matchlist and increment by 1 to get the next
            else {
                matchList[(int)*regexp] = 1;
                regexp++;
            }
        }
        mlPopulated = 1;

        if (regexp[1] == '+' && regexp[2] == '?')
            return matchhere(regexp + 2, text);

        if (regexp[1] == '?' || (regexp[1] == '*' && regexp[2] == '?')) 
            return matchhere(regexp, text);

        // If the next character in the text isn't the end of the string, loop through the text until we reach the end, then check for matches
        if (regexp[1] == '$') {
            while (text[1] != '\0')
                return 0;

            return matchhere(regexp, text);
        }

        // If there's a * or + after the ], check using the matchstar and matchplus functions respectively
        else if ((regexp[1] == '*' && matchstar(regexp[0], regexp, text)) || (regexp[1] == '+' && matchplus(regexp[0], regexp, text))){

            // If there's stuff after the + or *, continue matching.
            if (regexp[2] != '\0' && regexp[2] != '?' && regexp[2] != '$'){
                memset(matchList, 0, sizeof(matchList));
                mlPopulated = 0;    
                return matchhere(regexp + 2, text + skipCount - 1);
            }      
            return 1;
        }

        // If it's not the end of the regex yet and the current character is a match, keep scanning
        else if (regexp[1] != '\0' && matchList[(int)*text]) {
            memset(matchList, 0, sizeof(matchList));
            mlPopulated = 0;    
            return matchhere(regexp + 1, text + 1);
        }

        // If there's no char after the set, check if the current char is in the range.
        else return matchhere(regexp, text);
    }
    return 0;
}