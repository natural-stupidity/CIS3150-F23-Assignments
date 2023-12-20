// Michael Sirna
// 1094947
// 2023-09-29
// CIS3150 Assignment 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** Inititalizes a string array
 *.
 *  @param size the size of the list that we want to initialize.
 *  @param fp the file to scan through to get inputs.
 *  @param str the input from the file to store into the array.
 */
char **initList(int size, FILE *fp, char *str);




/** Gets the index of a state in a certain list. Returns -1 if not found.
 *.
 *  @param list the array to search.
 *  @param listSize the size of the array to search.
 *  @param state the state that we're looking for.
 */
int getStateIndex( char **list, int listSize, char *state);




/** The function used to check each transition in the list.
 *  It loops through the first state of every transition. If that state is active, 
 *  and it's activation input is equal to the current input,
 *  then get that transition's next state, and set the index of that state in the next array.
 *
 *  @param states the states array.
 *  @param stateSize the size of the states array.
 *  @param transitions the transitions array.
 *  @param transitionSize the size of the transitions array.
 *  @param currentInput the current input string being processed.
 *  @param arrayToCheck the current active, or next active arrays that we will check to see if the state is active.
 *  @param nextStates the next active states array.
 */
void checkTransition(char **states, int stateSize, char **transitions, int transitionSize, char *currentInput, int *arrayToCheck, int *nextStates);




/** This function goes through all the transitions multiple times and sets active states in a next array.
 *.
 *  @param states the states array.
 *  @param stateSize the size of the states array.
 *  @param inputs the inputs array.
 *  @param inputSize the size of the inputs array.
 *  @param transitions the transitions array.
 *  @param transitionSize the size of the transitions array.
 *  @param activeStates an array of currently active states. This is pretty much only used as a baseline for nextStates.
 *  @param nextStates an array that stores the next active states. All values are copied over to active states at the end.
 *  @param startIndex the index to the starting state.
 *  @param acceptIndex the index of the accept state.
 */
bool solveMachine(char **states, int stateSize, char **inputs, int inputSize, char **transitions, int transitionSize, int *activeStates, int *nextStates, int startIndex, int acceptIndex);




/** Function that frees a string array.
 *.
 *  @param list the array to free.
 *  @param listSize the size of the array to free.
 */
void freeList (char **list, int listSize);




// Some global variables I use to print inputs nicely.
int globInputLen = 0;
bool diffLengthInputs = false;