// reader.c
#include "set_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_utils.h"
#include "reader.h"

#define BUFFER_SIZE 1024

// Function to add a string to an array (no uniqueness check)
void add_string(char ***tokens, size_t *count, const char *token) {
    // Simply add the new token to the array
    *tokens = realloc(*tokens, (*count + 1) * sizeof(char*));
    (*tokens)[*count] = strdup(token);
    (*count)++;
}

// Function to split text using regular expression and return an array of strings
char **regexp_split_text(const char *text, PCRE2_SPTR pattern, size_t* num_hits) {
    pcre2_code *re;
    pcre2_match_data *match_data;
    int errorcode;
    PCRE2_SIZE error_offset;

    // Compile the regex pattern
    re = pcre2_compile((PCRE2_UCHAR8 *)pattern, strlen((const char *)pattern), 0, &errorcode, &error_offset, NULL);
    if (re == NULL) {
        printf("Regex compilation failed!\n");
        return NULL;
    }

    // Allocate match data
    match_data = pcre2_match_data_create_from_pattern(re, NULL);

    // Prepare the array for storing tokens
    char **tokens = NULL;
    size_t count = 0;

    size_t len = strlen(text);
    int start = 0;

    while ((size_t)start < len) {

        // Try to find a match in the remaining string
        int rc = pcre2_match(re, (PCRE2_UCHAR8 *)text + start, len - start, 0, 0, match_data, NULL);

        if (rc < 0) {
            // No match found, break the loop
            break;
        }

        // Extract the matched substring
        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
        size_t match_start = ovector[0];
        size_t match_end = ovector[1];
	size_t token_len = match_end - match_start;

	
	if (match_end > match_start) {
	  // Null-terminate and store the token
	  char *token = strndup(text + start, match_start);
	  strip(token);
	  size_t str_length_striped = strlen(token);
	  if (str_length_striped > 0){
	    add_string(&tokens, &count, token);
	  }
	  free(token);
	}

	if (token_len > 0) {
	  char * token = strndup(text + start + match_start, token_len);
	  add_string(&tokens, &count, token);
	  free(token);
	}

        // Move the start position past the match
        start += match_end;
    }

    // Clean up the regex and match data
    pcre2_code_free(re);
    pcre2_match_data_free(match_data);
    *num_hits = count;

    return tokens;  // Return the array of matched tokens
}

// Function to process the file and split using PCRE2 (UTF-8)
StringSet* regex_split_stream(FILE *file, PCRE2_SPTR pattern) {
    pcre2_code *re;
    pcre2_match_data *match_data;
    int errorcode;
    PCRE2_SIZE error_offset;
    StringSet *tokens = NULL;  

    // Compile the regex pattern
    re = pcre2_compile((PCRE2_UCHAR8 *)pattern, strlen((const char *)pattern), 0, &errorcode, &error_offset, NULL);
    if (re == NULL) {
        printf("Regex compilation failed!\n");
        return NULL;
    }

    // Allocate match data
    match_data = pcre2_match_data_create_from_pattern(re, NULL);

    char buffer[BUFFER_SIZE];  // Buffer for reading the file in chunks

    while (fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        int start = 0;

        while ((size_t)start < len) {
            // Try to find a match in the remaining string
            int rc = pcre2_match(re, (PCRE2_UCHAR8 *)buffer + start, len - start, 0, 0, match_data, NULL);

            if (rc < 0) {
                // No match found
                break;
            }

            // Extract the matched substring
            PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
            size_t match_start = ovector[0];
            size_t match_end = ovector[1];

	    size_t token_len = match_end - match_start;

	    if (match_end > match_start) {
	      // Null-terminate and store the token
	      char *token = strndup(buffer + start, match_start);
	      strip(token);
	      size_t str_length_striped = strlen(token);
	      if (str_length_striped > 0){
		add_unique_string(&tokens, token);
	      }
	      free(token);
	    }
	    
	    if (token_len > 0) {
	      char * token = strndup(buffer + start + match_start, token_len);
	      add_unique_string(&tokens, token);
	      free(token);
	    }

            // Move start position past the match
            start += match_end;
        }
    }

    // Clean up the regex and match data
    pcre2_code_free(re);
    pcre2_match_data_free(match_data);

    return tokens;
}
