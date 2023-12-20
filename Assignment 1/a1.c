// Michael Sirna
// 1094947
// 2023-09-29
// CIS3150 Assignment 1

#include "header.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("\nERROR: No file name argument found\nSyntax: ./a1 <file name>\n\n");
        return -1;
    }

    char *fileName = argv[1];
    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        printf("\nERROR: Failed to open the file \'%s\'\nExiting program...\n\n", fileName);
        return -1;
    }
    
    char fileStr[101];
    int alphabetSize = 0, stateSize = 0, inputSize = 0, transitionSize = 0, startIndex = -1, acceptIndex = -1;
    int *activeStates, *nextStates;
    char **alphabet, **states, **inputs, **transitions;

    // This entire for loop scans through the file and initializes all of the variables above.
    for (int i = 0; fscanf(fp, "%s", fileStr) != EOF; i++){
        
        // initialize the alphabet array.
        // This part isn't really needed for the assignment, but it's just here to read the alphabet lines without breaking the program.
        if (i == 0) {
            
            alphabetSize = atoi(fileStr);
            alphabet = initList(alphabetSize, fp, fileStr);
        } 

        // Initialize the states array along with active states list and next states list.
        if (i == 1) {
            stateSize = atoi(fileStr);
            states = initList(stateSize, fp, fileStr);
            activeStates = (int*) calloc(stateSize, sizeof(int));
            nextStates = (int*) calloc(stateSize, sizeof(int));
        }

        // Get the indecies of the start and accept states.
        if (i == 2) startIndex = getStateIndex(states, stateSize, fileStr);
        if (i == 3) acceptIndex = getStateIndex(states, stateSize, fileStr);

        // Just some basic error checking.
        if (i == 4 && (startIndex == -1 || acceptIndex == -1)){
            if (startIndex == -1) printf("\nERROR: Start state does not exist in the list of states!\nExiting program...\n\n");
            else printf("\nERROR: Accept state does not exist in the list of states!\nExiting program...\n\n");
            freeList(alphabet, alphabetSize);
            freeList(states, stateSize);
            free(activeStates);
            free(nextStates);
            fclose(fp);
            return -1;
        }

        // Initialize the list of inputs
        if (i == 4) {
            inputSize = atoi(fileStr);
            inputs = initList(inputSize, fp, fileStr);

            // Sets globInputLen and diffLenghtInputs for printing to be somewhat nicer.
            for (int j = 0; j < inputSize; j++) {
                if (globInputLen != 0 && (strlen(inputs[j]) > globInputLen || strlen(inputs[j]) < globInputLen)) diffLengthInputs = true;
                if (strlen(inputs[j]) > globInputLen) globInputLen = strlen(inputs[j]);
            }
        }

        // Get the transition size and initialize the transitions list.
        if (i == 5){
            transitionSize = atoi(fileStr) * 3;
            transitions = initList(transitionSize, fp, fileStr);
        }
    }

    printf("\n");

    bool isAccept = solveMachine(states, stateSize, inputs, inputSize, transitions, transitionSize, activeStates, nextStates, startIndex, acceptIndex);

    if (isAccept) printf("Accept\n\n");
    else printf("Reject\n\n");

    freeList(alphabet, alphabetSize);
    freeList(states, stateSize);
    free(activeStates);
    free(nextStates);
    freeList(inputs, inputSize);
    freeList(transitions, transitionSize);
    fclose(fp);
    return 0;
}


char **initList(int size,  FILE *fp, char *str){
    char **list = (char **)malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++){
        list[i] = (char *)malloc(11 * sizeof(char));
    }
    for (int j = 0; j < size; j++){
        fscanf(fp, "%s", str);
        strcpy(list[j], str);
    }  
    return list;
}


int getStateIndex( char **list, int listSize, char *state){
    for( int i = 0; i < listSize; i++) {
        if( strcmp(state, list[i]) == 0){
            return i;
        }
    }
    return -1;
}


void checkTransition(char **states, int stateSize, char **transitions, int transitionSize, char *currentInput, int *arrayToCheck, int *nextStates){
    for (int i = 0; i < transitionSize; i += 3){
        int currentState = getStateIndex(states, stateSize, transitions[i]);
        if (arrayToCheck[currentState] == 1 && strcmp(transitions[i + 1], currentInput) == 0){
            int stateIndexToChange = getStateIndex(states, stateSize, transitions[i + 2]);
            nextStates[stateIndexToChange] = 1;
        }
    }
}


bool solveMachine(char **states, int stateSize, char **inputs, int inputSize, char **transitions, int transitionSize, int *activeStates, int *nextStates, int startIndex, int acceptIndex){
    
    // Initialize the start index
    activeStates[startIndex] = 1;

    // Catch empty strings on the start state if it exists.
    // Loops through the first state of every transition. If that transition is the same as the start state and that state
    // has a transition that contains "e", then change the next state to active.
    for (int i = 0; i < transitionSize; i += 3){
        if (strcmp(transitions[i], states[startIndex]) == 0 && strcmp(transitions[i + 1], "e") == 0){
            int stateIndexToChange = getStateIndex(states, stateSize, transitions[i + 2]);
            activeStates[stateIndexToChange] = 1;
        }
    }

    for (int j = 0; j < inputSize; j++){

        // Print the current input (use a tab if the inputs are different lengths)
        if (diffLengthInputs) {
            int numOfSpaces = (int) globInputLen - strlen(inputs[j]);
            printf("%*s", numOfSpaces , "");
            printf("%s - ", inputs[j]);
        } 
        else printf("%s - ", inputs[j]);

        checkTransition(states, stateSize, transitions, transitionSize, inputs[j], activeStates, nextStates);

        // Do the same thing, but for the empty string transitions.
        // Instead of checking the active states, though, we must check the nextStates array to know what is currently active.
        checkTransition(states, stateSize, transitions, transitionSize, "e", nextStates, nextStates);

        // Copy over the next states array to the active states for the next round and print all contents.
        for (int l = 0; l < stateSize; l++){
            activeStates[l] = nextStates[l];
            printf("%d ", activeStates[l]);
            nextStates[l] = 0;
        }
        printf("\n");
    }

    // Return true of false depending if the accept state is active.
    return activeStates[acceptIndex] == 1;
}


void freeList (char **list, int listSize){
    for (int i = 0; i < listSize; i++){
        free(list[i]);
        list[i] = NULL;
    }
    free(list);
    list = NULL;
}
