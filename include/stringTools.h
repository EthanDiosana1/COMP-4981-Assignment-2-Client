//
// Created by main on 28/01/24.
//

#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <stdlib.h>
#include <string.h>

/**
 * @brief Struct that holds both a token and the original string.
 *
 */
typedef struct
{
    /** @brief The token. */
    char *token;

    /** @brief The original string */
    char *originalStr;

} TokenAndStr;

/**
 * @brief Struct that holds an array of strings.
 * @author Ethan D.
 *
 * Helps with tokenization.
 */
typedef struct
{
    /** @brief Pointer to an array of strings. */
    char **strings;

    /** @brief Number of strings. */
    unsigned int numStrings;

    /** @brief Int array containing the lengths of the strings in the string
     * array. */
    unsigned int *stringLengths;

} StringArray;

/**
 * @brief Returns a struct StringArray from a string.
 * Tokenizes the string based on the delimiter and returns a struct.
 * @param string The string to tokenize.
 * @param delim The delimiter to tokenize the string with.
 * @return struct StringArray
 */
StringArray tokenizeString(const char *string, const char *delim);

/**
 * @brief Returns a struct containing the final token of a string to be
 * tokenized.
 * @param string The string to tokenize.
 * @param delim The delimiter.
 * @return last token
 */
TokenAndStr getLastToken(const char *string, const char *delim);

/**
 * @brief Returns a struct containing the first token of a string to be
 * tokenized.
 * @param string The string to tokenize.
 * @param delim The delimiter.
 * @return TokenAndStr struct
 */
TokenAndStr getFirstToken(const char *string, const char *delim);

/**
 * @brief Returns the number of tokens from the string.
 * @param string The string to tokenize.
 * @param delim The delimiter.
 * @return number of tokens
 */
unsigned int getNumberOfTokens(const char *string, const char *delim);

/**
 * @brief Adds a string to the start of a string.
 * @param original The original string.
 * @param toAdd The string to add.
 * @return char *
 */
char *addCharacterToStart(const char *original, const char *toAdd);

#endif    // STRINGTOOLS_H
