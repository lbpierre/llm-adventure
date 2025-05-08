// bpe_io.c - Functions for saving and loading BPE vocabulary
#include "bpe.h"

// Saves the trained BPE vocabulary to a file
// Returns true on success, false on failure
bool save_bpe_vocab(const char *filename, VocabularyEntry *vocab, int vocab_size) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
        return false;
    }
    
    // Write vocabulary size
    if (fwrite(&vocab_size, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "Error writing vocabulary size\n");
        fclose(fp);
        return false;
    }
    
    // Write each vocabulary entry
    for (int i = 0; i < vocab_size; i++) {
        // Write length of entry
        if (fwrite(&vocab[i].length, sizeof(int), 1, fp) != 1) {
            fprintf(stderr, "Error writing entry length for index %d\n", i);
            fclose(fp);
            return false;
        }
        
        // Write bytes of entry
        if (fwrite(vocab[i].bytes, sizeof(unsigned char), vocab[i].length, fp) != vocab[i].length) {
            fprintf(stderr, "Error writing entry bytes for index %d\n", i);
            fclose(fp);
            return false;
        }
    }
    
    fclose(fp);
    printf("Vocabulary saved successfully to %s (%d entries)\n", filename, vocab_size);
    return true;
}

// Loads a BPE vocabulary from a file
// Returns the vocabulary and sets vocab_size, or NULL on failure
VocabularyEntry *load_bpe_vocab(const char *filename, int *vocab_size) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error opening file for reading: %s\n", filename);
        return NULL;
    }
    
    // Read vocabulary size
    if (fread(vocab_size, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "Error reading vocabulary size\n");
        fclose(fp);
        return NULL;
    }
    
    // Validate vocab size
    if (*vocab_size <= 0 || *vocab_size > MAX_VOCAB_SIZE) {
        fprintf(stderr, "Invalid vocabulary size: %d\n", *vocab_size);
        fclose(fp);
        return NULL;
    }
    
    // Allocate memory for vocabulary
    VocabularyEntry *vocab = (VocabularyEntry *)malloc(*vocab_size * sizeof(VocabularyEntry));
    if (!vocab) {
        fprintf(stderr, "Memory allocation failed for vocabulary\n");
        fclose(fp);
        return NULL;
    }
    
    // Read each vocabulary entry
    for (int i = 0; i < *vocab_size; i++) {
        // Read length of entry
        if (fread(&vocab[i].length, sizeof(int), 1, fp) != 1) {
            fprintf(stderr, "Error reading entry length for index %d\n", i);
            free_vocabulary(vocab, i); // Free what we've loaded so far
            fclose(fp);
            return NULL;
        }
        
        // Validate entry length
        if (vocab[i].length <= 0 || vocab[i].length > MAX_BYTES_LEN) {
            fprintf(stderr, "Invalid entry length: %d for index %d\n", vocab[i].length, i);
            free_vocabulary(vocab, i); // Free what we've loaded so far
            fclose(fp);
            return NULL;
        }
        
        // Allocate memory for entry bytes
        vocab[i].bytes = (unsigned char *)malloc(vocab[i].length * sizeof(unsigned char));
        if (!vocab[i].bytes) {
            fprintf(stderr, "Memory allocation failed for entry bytes at index %d\n", i);
            free_vocabulary(vocab, i); // Free what we've loaded so far
            fclose(fp);
            return NULL;
        }
        
        // Read bytes of entry
        if (fread(vocab[i].bytes, sizeof(unsigned char), vocab[i].length, fp) != vocab[i].length) {
            fprintf(stderr, "Error reading entry bytes for index %d\n", i);
            free(vocab[i].bytes); // Free the current entry's bytes
            free_vocabulary(vocab, i); // Free what we've loaded so far
            fclose(fp);
            return NULL;
        }
    }
    
    fclose(fp);
    printf("Vocabulary loaded successfully from %s (%d entries)\n", filename, *vocab_size);
    return vocab;
}
