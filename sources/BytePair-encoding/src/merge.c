/*
  merge.c - Implementation of merge-related functions for BPE
*/

#include "bpe.h"
#include <stdlib.h>
#include <string.h>

// Count occurrences of adjacent byte sequence pairs
Merge
find_most_frequent_pair (ByteSequences seqs)
{
  Merge best_merge = { NULL, 0, NULL, 0, NULL, 0, 0 };

  for (int i = 0; i < seqs.count - 1; i++)
    {
      ByteSequence first = seqs.sequences[i];
      ByteSequence second = seqs.sequences[i + 1];

      int count = 0;
      for (int j = 0; j < seqs.count - 1; j++)
        {
          if (byte_sequence_equals (seqs.sequences[j], first)
              && byte_sequence_equals (seqs.sequences[j + 1], second))
            {
              count++;
            }
        }

      if (count > best_merge.count)
        {
          if (best_merge.first != NULL)
            free (best_merge.first);
          if (best_merge.second != NULL)
            free (best_merge.second);
          if (best_merge.replacement != NULL)
            free (best_merge.replacement);

          best_merge.first = malloc (first.length);
          best_merge.first_len = first.length;
          memcpy (best_merge.first, first.bytes, first.length);

          best_merge.second = malloc (second.length);
          best_merge.second_len = second.length;
          memcpy (best_merge.second, second.bytes, second.length);

          best_merge.count = count;

          // Create the replacement (concatenation of first and second)
          best_merge.replacement_len = first.length + second.length;
          best_merge.replacement = malloc (best_merge.replacement_len);
          memcpy (best_merge.replacement, first.bytes, first.length);
          memcpy (best_merge.replacement + first.length, second.bytes, second.length);
        }
    }

  return best_merge;
}

// Apply a merge operation to the byte sequences
ByteSequences
apply_merge (ByteSequences seqs, Merge merge)
{
  ByteSequences new_seqs;
  new_seqs.sequences = malloc (sizeof (ByteSequence) * seqs.count);
  new_seqs.count = 0;

  for (int i = 0; i < seqs.count; i++)
    {
      // Check if this sequence and the next one form the pair to be merged
      if (i < seqs.count - 1 && seqs.sequences[i].length == merge.first_len
          && seqs.sequences[i + 1].length == merge.second_len
          && memcmp (seqs.sequences[i].bytes, merge.first, merge.first_len) == 0
          && memcmp (seqs.sequences[i + 1].bytes, merge.second, merge.second_len) == 0)
        {
          new_seqs.sequences[new_seqs.count]
              = create_byte_sequence (merge.replacement, merge.replacement_len);
          new_seqs.count++;
          i++; // Slip the next sequence as it's part of the merge; our `i+1`
        }
      else
        {
          // Keep the original sequence
          new_seqs.sequences[new_seqs.count]
              = create_byte_sequence (seqs.sequences[i].bytes, seqs.sequences[i].length);
          new_seqs.count++;
        }
    }

  return new_seqs;
}

void
free_merge (Merge merge)
{
  if (merge.first)
    free (merge.first);
  if (merge.second)
    free (merge.second);
  if (merge.replacement)
    free (merge.replacement);
}
