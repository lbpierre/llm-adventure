#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set_utils.h"
#include "uthash.h"
#include "reader.h"

void add_unique_string(StringSet **set, const char *string) {
  Tokens *entry;
  static int next_token_id = 0; // static to keep state accross funcion calls

  HASH_FIND_STR(*set, string, entry);

  // check if the string is not present in the set
  if (!entry) {
    entry = malloc(sizeof(StringSet));
    entry->key = strdup(string);
    entry->tokenID = next_token_id++;
    HASH_ADD_KEYPTR(hh, *set, entry->key, strlen(entry->key), entry);
  }
}

int get_tokenID(Tokens *set, const char *string) {

  Tokens *entry;
  HASH_FIND_STR(set, string, entry);

  if (!entry) {
    return -1;
  }
  else {
    return entry->tokenID;
  }
}

char *get_word(StringSet *set, int tokenID) {

  Tokens *entry;
  HASH_FIND_INT(set, &tokenID, entry);

  if (entry){
    printf("found tokenID %d using the build in\n", tokenID);
  }
  
  HASH_ITER(hh, set, entry, entry) {
    if (entry->tokenID == tokenID) {
      break;
    }
  }
  if (entry) {
    return entry->key;
  }
  else {
    printf("cannot find the token %d\n", tokenID);
  }
  return "";
}


// print all the string in the set <- debug only
void print_set(StringSet *set) {
  StringSet *entry;
  HASH_ITER(hh, set, entry, entry) {
    printf("(%s: %d), ", entry->key, entry->tokenID);
  }
  printf("\n");
}

// Cleanup and free memory
void free_set(StringSet *set) {
  StringSet *tmp, *entry;
  HASH_ITER(hh, set, entry, tmp) {
    HASH_DEL(set, entry);            // Remove from hash table
    free(entry->key);                // Free string memory
    free(entry);                     // Free struct memory
  }
}
