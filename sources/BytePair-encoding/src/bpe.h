/*
 bpe.h - Header file for Byte Pair Encoding implementation

 Contains declarations for data structures and functions needed for BPE
*/

#ifndef BPE_H
#define BPE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf (stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void) 0)
#endif

#define MAX_VOCAB_SIZE 1000
#define MAX_BYTES_LEN 2048
#define MAX_TEXT_LEN 10000

// Struture for the byte sequence
typedef struct
{
  unsigned char *bytes;
  int length;
} ByteSequence;

// Structure to track sequence of bytes
typedef struct
{
  ByteSequence *sequences;
  int count;
} ByteSequences;

// Structure for a pair of byte sequences
typedef struct
{
  unsigned char *first;
  int first_len;
  unsigned char *second;
  int second_len;
  unsigned char *replacement;
  int replacement_len;
  int count;
} Merge;

// Structure for BPE vocabulary entry
typedef struct
{
  unsigned char *bytes;
  int length;
} VocabularyEntry;

// ByteSequence functions
ByteSequence create_byte_sequence (unsigned char *bytes, int length);
void free_byte_sequence (ByteSequence seq);
bool byte_sequence_equals (ByteSequence bs1, ByteSequence bs2);

// ByteSequences functions
ByteSequences tokinize_to_bytes (const unsigned char *text, int text_len);
void free_byte_sequences (ByteSequences seqs);
void print_byte_sequences (ByteSequences seqs);
void print_readable (ByteSequences seqs);

// Merge functions
Merge find_most_frequent_pair (ByteSequences seqs);
ByteSequences apply_merge (ByteSequences seqs, Merge merge);
void free_merge (Merge merge);

// Vocabulary functions
void print_vocabulary (VocabularyEntry *vocab, int vocab_size);
int find_in_vocabulary (VocabularyEntry *vocab, int vocab_size, ByteSequence seq);
void free_vocabulary (VocabularyEntry *vocab, int vocab_size);

// Core BPE functions
VocabularyEntry *train_bpe (const unsigned char *text, int text_len, int num_merges,
                            int *final_vocab_size);
int *encode (const unsigned char *text, int text_len, VocabularyEntry *vocab, int vocab_size,
             int *encoded_len);
unsigned char *decode (int *encoded, int encoded_len, VocabularyEntry *vocab, int vocab_size,
                       int *result_len);

// Utility functions
void print_bytes_hex (unsigned char *bytes, int len);
char *read_file_to_buffer (const char *filename, size_t *text_len);
void print_byte_sequence (ByteSequence seq);

#endif // BPE_H
