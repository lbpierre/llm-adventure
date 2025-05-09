#ifndef SET_UTILS_H
#define SET_UTILS_H

#include "uthash.h"

typedef struct {
  char *key;
  int tokenID;
  UT_hash_handle hh;
} StringSet, Tokens;


void add_unique_string(StringSet **set, const char *string);
int get_tokenID(StringSet *set, const char *string);
char* get_word(StringSet *set, int tokenID);
void print_set(StringSet *set);
void free_set(StringSet *set);

#endif // SET_UTILS_H
