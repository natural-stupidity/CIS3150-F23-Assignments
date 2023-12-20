// Michael Sirna
// 1094947
// 2023-12-19
// CIS3150 Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** The kick start function for the recursion. All recursive functions below return to this one. This function also prints off the outputs.
 *
 *  @param regexp The regex to test.
 *  @param text The text it cycles through to test the regex.
 */
int match(char *regexp, char *text);


/** The main recursive function. Directs / starts another function depending on the current regex character.
 *
 *  @param regexp The regex to test.
 *  @param text The text it cycles through to test the regex.
 */
int matchhere(char *regexp, char *text);


/** Handles all ? cases. Matches optional characters
 *
 *  @param c - The character specified before the ?
 *  @param regexp The regex to test.
 *  @param text The text it cycles through to test the regex.
 */
int matchquestion(int c, char *regexp, char *text);


/** Handles all * cases. Matches 0 or more of character c.
 *
 *  @param c - The character specified before the *
 *  @param regexp The regex to test.
 *  @param text The text it cycles through to test the regex.
 */
int matchstar(int c, char *regexp, char *text);


/** Handles all + cases. Matches 1 or more of character c.
 *
 *  @param c - The character specified before the +
 *  @param regexp The regex to test.
 *  @param text The text it cycles through to test the regex.
 */
int matchplus(int c, char *regexp, char *text);


/** Handles all [] cases. Matches characters in a range of values, or a set of characters.
 *
 *  @param regexp The regex to test.
 *  @param text The text it cycles through to test the regex.
 */
int matchset(char *regexp, char *text);