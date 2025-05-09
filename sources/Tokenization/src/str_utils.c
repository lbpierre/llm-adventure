#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void strip(char *str) {
    char *start = str;
    char *end;

    // Skip leading whitespace
    while (isspace((unsigned char)*start)) start++;

    // If all spaces
    if (*start == 0) {
        str[0] = '\0';
        return;
    }

    // Find end of string
    end = start + strlen(start) - 1;

    // Move backward to skip trailing whitespace
    while (end > start && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';

    // Move trimmed string to the beginning
    memmove(str, start, end - start + 2); // +1 for '\0', +1 because end is inclusive
}

void concat(char **s1, const char *s2) {
  size_t len_s1 = strlen(*s1);
  size_t len_s2 = strlen(s2);

  *s1 = realloc(*s1, len_s1 + len_s2 + 1);

  if(*s1 == NULL) {
    perror("realloc failed");
    exit(EXIT_FAILURE);
  }

  strncat(*s1, s2, len_s2);

}
