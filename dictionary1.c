/**
 * dictionary.c
 *
 * Implements a dictionary's functionality using hash table.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <math.h>
#include <ctype.h>

#define NUMBER_OF_KEYS 20000

typedef struct Word {
    char str[LENGTH + 1];
    struct Word *next;
} Word;


int wordsLoaded = -1;
Word *hashTable[NUMBER_OF_KEYS] = {NULL};
char filename[100];

/**
 * Returns hash value of a string.
 */
int hash(const char *str) {
    int hashValue = 5381;
    int i;
    while ((i = *str++))
        hashValue = ((hashValue << 5) + hashValue) + i;
    return abs(hashValue % NUMBER_OF_KEYS);
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word) {
    // Finds out the lowercase of a word and stores it in lowerCase variable.
    char lowerCase[LENGTH + 1];
    int i;
    for (i = 0; word[i] != '\0'; i++) {
        lowerCase[i] = tolower(word[i]);
    }
    lowerCase[i] = '\0';

    int hashValue = hash(lowerCase);
    if (hashTable[hashValue] == NULL) {
        return false;
    } else {
        Word *trav = hashTable[hashValue];
        while (trav != NULL) {
            if (strcmp(trav->str, lowerCase) == 0)
                return true;
            trav = trav->next;
        }
    }
    return false;
}

Word* getNewWord(char* word) {
    Word *newWord = (Word*) malloc(sizeof(Word));
    strcpy(newWord->str, word);
    newWord->next = NULL;
    return newWord;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char *dictionary) {
    // Copies filename for unload.
    strcpy(filename, dictionary);

    FILE *dict = fopen(dictionary, "r");

    // If unable to read the dictionary file then return false.
    if (dict == NULL) return false;

    while (!feof(dict)) {
        char word[LENGTH + 1];
        fscanf(dict, "%s", word);
        wordsLoaded++;

        int hashValue = hash(word);
        if (hashTable[hashValue] == NULL) {
            hashTable[hashValue] = getNewWord(word);
            if (hashTable[hashValue] == NULL) return false;
        } else {
            Word *oldFirst = hashTable[hashValue];
            hashTable[hashValue] = getNewWord(word);
            if (hashTable[hashValue] == NULL) return false;
            hashTable[hashValue]->next = oldFirst;
        }
    }
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void) {
    return (unsigned int) wordsLoaded;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void) {
    FILE *dict = fopen(filename, "r");
    if (dict == NULL) return false;

    while (!feof(dict)) {
        char word[LENGTH + 1];
        fscanf(dict, "%s", word);

        int hashValue = hash(word);
        if (hashTable[hashValue] != NULL) {
            Word *trav = hashTable[hashValue];
            while (trav != NULL) {
                Word *toDelete = trav;
                trav = trav->next;
                free(toDelete);
            }
            hashTable[hashValue] = NULL;
        }
    }
    return true;
}
