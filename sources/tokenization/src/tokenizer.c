#include "tokenizer.h"
#include "array_utils.h"
#include "set_utils.h"
#include "str_utils.h"

// Function to print an array of strings
void print_string_array(char **tokens, size_t num_tokens) {
    for (size_t i = 0; i < num_tokens; i++) {
        printf("Token %zu: %s\n", i + 1, tokens[i]);
    }
}

Array* encode_input(Tokenizer *self, const char *text) {
  /* equivalent of a str to int but with int present
   in a vocabulary table */

  Array *encoded = malloc(sizeof(Array));
  char ** string_array = NULL;
  size_t num_hits = 0;

  string_array = regexp_split_text(text, self->pattern, &num_hits);
  array_init(encoded, num_hits);

  for (size_t i = 0; i < num_hits; i++) {
    int tokenID = -1;
    tokenID = get_tokenID(self->vocabulary, string_array[i]);
    if (tokenID >= 0) {
      array_add(encoded, tokenID);
    }
  }

  return encoded;
}

char *decode_input(Tokenizer *self, Array* encoded) {

  char *data = strdup("");

  for (size_t n = 0; n < encoded->size; n++){
    char* word = get_word(self->vocabulary, encoded->data[n]);
    concat(&data, word);
  }
  
  return data;
}

Tokenizer* create_tokenizer(const char* filepath, PCRE2_SPTR pattern) {
  Tokenizer *tokenizer = malloc(sizeof(Tokenizer));
  tokenizer->vocabulary = NULL;
  tokenizer->pattern = pattern;
  tokenizer->encode = encode_input;
  tokenizer->decode = decode_input;

  FILE *file = fopen(filepath, "r");
  if (!file) {
    perror("Error opening file");
    return NULL;
  }
  
  tokenizer->vocabulary = regex_split_stream(file, pattern);
  printf("There are %u token in the set\n", HASH_COUNT(tokenizer->vocabulary));

  return tokenizer;
}

void free_tokenizer(Tokenizer *tokenizer) {
  if (tokenizer) {
    free_set(tokenizer->vocabulary);
    free(tokenizer);
  }
}
