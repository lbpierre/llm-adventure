#ifndef READER_H
#define READER_H

#include <stdio.h>
#include "set_utils.h"

// Define PCRE2_CODE_UNIT_WIDTH as 8 for UTF-8 (8-bit code units)
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>


StringSet* regex_split_stream(FILE *file, PCRE2_SPTR pattern);

char **regexp_split_text(const char *text, PCRE2_SPTR pattern, size_t *num_hits);

void add_string(char ***tokens, size_t *count, const char *token);

#endif // READER_H
