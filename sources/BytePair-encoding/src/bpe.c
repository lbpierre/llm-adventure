/*
bpe.c - Core BPE algorithm implementation
 */

#include "bpe.h"
#include <stdlib.h>
#include <string.h>

VocabularyEntry *
train_bpe (const unsigned char *text, int text_len, int num_merges, int *final_vocab_size)
{
  VocabularyEntry *vocab = malloc (sizeof (VocabularyEntry) * MAX_VOCAB_SIZE);
  int vocab_size = 0;

  // Start with byte-level tokens
  ByteSequences seqs = tokinize_to_bytes (text, text_len);

  // Add initial byte tokens to vocabulary
  for (int i = 0; i < seqs.count; i++)
    {
      ByteSequence seq = seqs.sequences[i];
      if (find_in_vocabulary (vocab, vocab_size, seq) == -1)
        {
          vocab[vocab_size].bytes = malloc (seq.length);
          vocab[vocab_size].length = seq.length;
          memcpy (vocab[vocab_size].bytes, seq.bytes, seq.length);
          vocab_size++;
        }
    }

  // Perform BPE merge operations
  for (int i = 0; i < num_merges; i++)
    {
      printf ("\nMerge iteration %d:\n", i + 1);
      print_byte_sequences (seqs);
      print_readable (seqs);

      // Find the most frequent pair
      Merge best_merge = find_most_frequent_pair (seqs);

      // If no pairs found, then break
      if (best_merge.count == 0)
        {
          DEBUG_PRINT ("No more pairs to merge. Stopping early\n");
          printf ("No more pairs to merge. Stopping early, at round: %d\n", i);
          break;
        }

      // Add the new merged token to vocabulary
      vocab[vocab_size].bytes = malloc (best_merge.replacement_len);
      vocab[vocab_size].length = best_merge.replacement_len;
      memcpy (vocab[vocab_size].bytes, best_merge.replacement, best_merge.replacement_len);
      vocab_size++;

      DEBUG_PRINT ("Merging [");
      for (int j = 0; j < best_merge.first_len; j++)
        {
          DEBUG_PRINT ("%02X ", best_merge.first[j]);
        }
      DEBUG_PRINT ("] + [");
      for (int j = 0; j < best_merge.second_len; j++)
        {
          DEBUG_PRINT ("%02X ", best_merge.second[j]);
        }
      DEBUG_PRINT ("] -> [");
      for (int j = 0; j < best_merge.replacement_len; j++)
        {
          DEBUG_PRINT ("%02X ", best_merge.replacement[j]);
        }
      DEBUG_PRINT ("] (count: %d)\n", best_merge.count);

      // Apply the merge operation
      ByteSequences new_seqs = apply_merge (seqs, best_merge);

      // Free the old sequences and replace with the new ones
      free_byte_sequences (seqs);
      seqs = new_seqs;

      // Free the merge data
      free_merge (best_merge);
    }

  /* print_vocabulary (vocab, vocab_size); */

  // Clean up
  free_byte_sequences (seqs);

  *final_vocab_size = vocab_size;
  return vocab;
}

// Function to encode text using the learned vocabulary
int *
encode (const unsigned char *text, int text_len, VocabularyEntry *vocab, int vocab_size,
        int *encoded_len)
{
  ByteSequences seqs = tokinize_to_bytes (text, text_len);

  // Apply merges until no more can be applied
  bool made_changes;
  do
    {
      made_changes = false;

      // Try each merge in vocabulary (in reverse order of learning)
      for (int v = vocab_size - 1; v >= 0; v--)
        {
          VocabularyEntry merge_result = vocab[v];

          // Skpip single-byte token in vocabulary
          if (merge_result.length <= 1)
            continue;

          // Find all possible parts of this merge
          for (int i = 0; i < seqs.count - 1; i++)
            {
              ByteSequence first = seqs.sequences[i];
              ByteSequence second = seqs.sequences[i + 1];

              // Check if first + second equals the merge result
              if (first.length + second.length == merge_result.length)
                {
                  unsigned char combined[MAX_BYTES_LEN];
                  memcpy (combined, first.bytes, first.length);
                  memcpy (combined + first.length, second.bytes, second.length);

                  if (memcmp (combined, merge_result.bytes, merge_result.length) == 0)
                    {
                      // Create new sequences array
                      ByteSequences new_seqs;
                      new_seqs.sequences = malloc (sizeof (ByteSequence) * seqs.count);
                      new_seqs.count = 0;

                      // Copy sequences before the merge
                      for (int j = 0; j < i; j++)
                        {
                          new_seqs.sequences[new_seqs.count] = create_byte_sequence (
                              seqs.sequences[j].bytes, seqs.sequences[j].length);
                          new_seqs.count++;
                        }

                      // Add the merged sequence
                      new_seqs.sequences[new_seqs.count]
                          = create_byte_sequence (merge_result.bytes, merge_result.length);
                      new_seqs.count++;

                      // Copy sequences after the merge
                      for (int j = i + 2; j < seqs.count; j++)
                        {
                          new_seqs.sequences[new_seqs.count] = create_byte_sequence (
                              seqs.sequences[j].bytes, seqs.sequences[j].length);
                          new_seqs.count++;
                        }

                      // Free old sequences
                      free_byte_sequences (seqs);

                      // Upadte sequences
                      seqs = new_seqs;

                      made_changes = true;
                      break; // Start over with new sequence list
                    }
                }
            }

          if (made_changes)
            break;
        }
    }
  while (made_changes);

  int *encoded = malloc (sizeof (int) * seqs.count);
  *encoded_len = seqs.count;

  for (int i = 0; i < seqs.count; i++)
    {
      encoded[i] = find_in_vocabulary (vocab, vocab_size, seqs.sequences[i]);

      if (encoded[i] == -1)
        {
          DEBUG_PRINT (
              "Warning: Byte sequence not found in vocabulary at position %d. Using index -1\n", i);
        }
    }

  free_byte_sequences (seqs);
  return encoded;
}

// Function to decode encoded tokens back to bytes
unsigned char *
decode (int *encoded, int encoded_len, VocabularyEntry *vocab, int vocab_size, int *result_len)
{
  unsigned char *result = malloc (MAX_TEXT_LEN);
  *result_len = 0;

  for (int i = 0; i < encoded_len; i++)
    {
      if (encoded[i] >= 0 && encoded[i] < vocab_size)
        {
          VocabularyEntry entry = vocab[encoded[i]];
          memcpy (result + *result_len, entry.bytes, entry.length);
          *result_len += entry.length;
        }
    }

  return result;
}
