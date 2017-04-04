/**
 * dictionary.c
 *
 * Implements a dictionary's functionality using trie.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <math.h>
#include <ctype.h>

#define TOTAL_CHARACTERS 27
/*
 * Node for Trie, containing pointers to its 27 children (a-z and ')
 * and one boolean variable specifying whether it is last character of some word.
 */
typedef struct node {
    struct node *children[TOTAL_CHARACTERS];
    bool isLastCharacter;
} node;


int wordsLoaded = -1;
node *root;

/*
 * Returns corresponding index of a character.
 */
int getIndex(char c) {
    if (c == '\'') return 26;
    else return tolower(c) - 'a';
}

/*
 * Returns an empty default node.
 */

node* getNewNode() {
    node* newNode = (node*) malloc(sizeof(node));
    if (newNode != NULL) {
        newNode->isLastCharacter = false;
        for (int i = 0; i < TOTAL_CHARACTERS; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word) {
    node *trav = root;
    for (int i = 0, n = (int) strlen(word); i < n; i++) {
        int index = getIndex(word[i]);
        if (trav->children[index] == NULL) {
            return false;
        } else {
            trav = trav->children[index];
            if (i == n - 1)
                return trav->isLastCharacter;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char *dictionary) {
    root = getNewNode();
    // Returns false if there is not enough memory to load new words.
    if (root == NULL)
        return false;

    // Opens the dictionary file.
    FILE *dict = fopen(dictionary, "r");
    
    // If unable to read the dictionary file then return false.
    if (dict == NULL) return false;

    // Reads one word at a time and loads it in memory.
    while (!feof(dict)) {
        char word[LENGTH + 1];
        fscanf(dict, "%s", word);
        wordsLoaded++;

        node *trav = root;
        int n = (int) strlen(word);
        for (int i = 0; i < n; i++) {
            int index = getIndex(word[i]);

            if (trav->children[index] == NULL) {
                
                trav->children[index] = getNewNode();

                // Returns false if there is not enough memory to load new words.
                if (trav->children[index] == NULL) {
                    return false;
                }

                if (i == n - 1)
                    trav->children[index]->isLastCharacter = true;
                else
                    trav = trav->children[index];

            } else {

                if (i == n - 1)
                    trav->isLastCharacter = true;
                else
                    trav = trav->children[index];
            }

        }
    }
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void) {
    return (unsigned int) wordsLoaded;
}

/*
 * Removes all the children of a node and frees memory.
 */
void removeChildren(node *node) {
    for (int i = 0; i < TOTAL_CHARACTERS; ++i) {
        if (node->children[i] != NULL) {
            removeChildren(node->children[i]);
            free(node->children[i]);
        }
        node->children[i] = NULL;
    }
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void) {
    removeChildren(root);
    free(root);
    return true;
}
