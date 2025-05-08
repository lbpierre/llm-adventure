#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_utils.h"
#include "set_utils.h"

// Define PCRE2_CODE_UNIT_WIDTH as 8 for UTF-8 (8-bit code units)
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>


// Generic function pointer type for adding tokens
typedef void (*AddTokenFunc)(void *token_set, const char *token);

// Generic regex-based text splitter
void* regex_split_generic(const char *text, PCRE2_SPTR pattern, AddTokenFunc add_token, void *token_set);

// Wrapper to split a string and return an array of strings
char **regexp_split_text(const char *text, PCRE2_SPTR pattern, size_t *num_hits);

// Wrapper to split a stream and return a StringSet pointer
// typedef struct StringSet StringSet;  // Forward declaration
StringSet* regex_split_stream(FILE *file, PCRE2_SPTR pattern);


/* StringSet* regex_split_stream(FILE *file, PCRE2_SPTR pattern); */

/* char **regexp_split_text(const char *text, PCRE2_SPTR pattern, size_t *num_hits); */

void add_string(char ***tokens, size_t *count, const char *token);

#endif // READER_H
