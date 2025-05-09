#include "reader.h"

#define BUFFER_SIZE 1024

typedef void (*AddTokenFunc)(void *token_set, const char *token);

typedef struct {
    char ***tokens;
    size_t *count;
} TokenArrayContext;

typedef struct {
    StringSet **set;
} StringSetContext;


void add_string_wrapper(void *ctx, const char *token) {
    TokenArrayContext *context = (TokenArrayContext *)ctx;
    add_string(context->tokens, context->count, token);
}

void add_unique_string_wrapper(void *ctx, const char *token) {
    StringSetContext *context = (StringSetContext *)ctx;
    add_unique_string(context->set, token);
}

void* regex_split_generic(const char *text, PCRE2_SPTR pattern, AddTokenFunc add_token, void *token_set) {
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

    size_t len = strlen(text);
    int start = 0;

    while ((size_t)start < len) {
        // Match regex
        int rc = pcre2_match(re, (PCRE2_UCHAR8 *)text + start, len - start, 0, 0, match_data, NULL);
        if (rc < 0) break;

        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
        size_t match_start = ovector[0];
        size_t match_end = ovector[1];
        size_t token_len = match_end - match_start;

        if (match_end > match_start) {
            char *prefix_token = strndup(text + start, match_start);
            strip(prefix_token);
            if (strlen(prefix_token) > 0) {
                add_token(token_set, prefix_token);
            }
            free(prefix_token);
        }

        if (token_len > 0) {
            char *match_token = strndup(text + start + match_start, token_len);
            add_token(token_set, match_token);
            free(match_token);
        }

        start += match_end;
    }

    pcre2_code_free(re);
    pcre2_match_data_free(match_data);

    return token_set;
}

StringSet* regex_split_stream(FILE *file, PCRE2_SPTR pattern) {
    StringSet *tokens = NULL;
    StringSetContext ctx = { .set = &tokens };

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file)) {
        regex_split_generic(buffer, pattern, add_unique_string_wrapper, &ctx);
    }

    /* tokens-> */

    return tokens;
}

char **regexp_split_text(const char *text, PCRE2_SPTR pattern, size_t* num_hits) {
    char **tokens = NULL;
    *num_hits = 0;
    TokenArrayContext ctx = { .tokens = &tokens, .count = num_hits };
    regex_split_generic(text, pattern, add_string_wrapper, &ctx);

    return tokens;
}

// Function to add a string to an array (no uniqueness check)
void add_string(char ***tokens, size_t *count, const char *token) {
    // Simply add the new token to the array
    *tokens = realloc(*tokens, (*count + 1) * sizeof(char*));
    (*tokens)[*count] = strdup(token);
    (*count)++;
}


