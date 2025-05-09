#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include "uthash.h"
#include "set_utils.h"
#include "array_utils.h"
#include "reader.h"

// Define PCRE2_CODE_UNIT_WIDTH as 8 for UTF-8 (8-bit code units)
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

typedef struct Tokenizer Tokenizer;

typedef Array* (*EncodeFn)(Tokenizer *self, const char *input);
typedef char* (*DecodeFn)(Tokenizer *self, Array*);

struct Tokenizer {
  Tokens* vocabulary;
  PCRE2_SPTR pattern;
  EncodeFn encode;
  DecodeFn decode;
};

Tokenizer *create_tokenizer(const char* filepath, PCRE2_SPTR pattern);
void free_tokenizer(Tokenizer* tokenizer);

#endif // TOKENIZER_H
