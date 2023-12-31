/*
** EPITECH PROJECT, 2023
** tank.ioInC
** File description:
** strToWordArray
*/

//------------Explaination for this file------------------------------//
// thi file contains the functions to convert a string into a word array.
// This is used to parse the buffer received from the client.

#include "server.h"


//------------Check if the char is alphanumeric------------------------//
int is_alnum(char c)
{
    if (c <= ' ' || c >= 127)
        return 0;
    return 1;
}

//------------Count the number of words in the string-----------------//
int word_count(char *str)
{
    int i;
    int word;

    word = 0;
    i = 0;
    while (str != NULL && str[i]) {
        if (is_alnum(str[i]) == 1 && is_alnum(str[i + 1]) == 0)
            word = word + 1;
        i = i + 1;
    }
    return word;
}

//------------Convert the string into a word array-------------------//
char **strToWordArray(char *str)
{
    // init variables that will be used
    int j = 0;
    int i = 0;
    char **wordtab;
    int len = 0;

    // count the number of words in the string
    int wordCount = word_count(str);


    // if the string is empty, return NULL
    if (wordCount == 0) {
        wordtab = (char **)malloc(2 * sizeof(char *));
        if (wordtab == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire.\n");
            return NULL;
        }
        wordtab[0] = strdup(str);
        wordtab[1] = NULL;
        return wordtab;
    }

    // allocate memory for the word array
    wordtab = (char **)malloc((wordCount + 1) * sizeof(char *));
    if (wordtab == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        return NULL;
    }

    // loop through the string and add the words to the word array
    while (str != NULL && str[i]) {
        if (is_alnum(str[i])) {
            len = len + 1;
        }

        // if the char is not alphanumeric, add the word to the word array
        if (is_alnum(str[i]) == 1 && is_alnum(str[i + 1]) == 0) {
            wordtab[j] = (char *)malloc((len + 1) * sizeof(char));
            if (wordtab[j] == NULL) {
                fprintf(stderr, "Erreur d'allocation mémoire.\n");
                return NULL;
            }
            strncpy(wordtab[j], &str[i - len + 1], len);
            wordtab[j][len] = '\0';
            len = 0;
            j = j + 1;
        }
        i = i + 1;
    }
    wordtab[j] = NULL;
    return wordtab;
}