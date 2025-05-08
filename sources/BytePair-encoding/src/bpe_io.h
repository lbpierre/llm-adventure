#ifndef BPE_IO_H
#define BPE_IO_H

#include "bpe.h"
#include <stdbool.h>

// Saves the trained BPE vocabulary to a file
// Returns true on success, false on failure
bool save_bpe_vocab(const char *filename, VocabularyEntry *vocab, int vocab_size);

// Loads a BPE vocabulary from a file
// Returns the vocabulary and sets vocab_size, or NULL on failure
VocabularyEntry *load_bpe_vocab(const char *filename, int *vocab_size);

#endif // BPE_IO_H
