/*
vocab.c - Implementation of vocabulary-related functions for BPE
*/

#include "bpe.h"

// Print the current vocabulary
void
print_vocabulary (VocabularyEntry *vocab, int vocab_size)
{
  DEBUG_PRINT ("Vocabulary (size: %d):\n", vocab_size);
  for (int i = 0; i < vocab_size; i++)
    {
      DEBUG_PRINT ("%d: [", i);
      for (int j = 0; j < vocab[i].length; j++)
        {
          DEBUG_PRINT ("%02X ", vocab[i].bytes[j]);
        }
      DEBUG_PRINT ("] ");

      // Print readable form for ASCII content
      DEBUG_PRINT ("'");
      for (int j = 0; j < vocab[i].length; j++)
        {
          if (vocab[i].bytes[j] >= 32 && vocab[i].bytes[j] <= 126)
            {
              DEBUG_PRINT ("%c", vocab[i].bytes[j]);
            }
          else
            {
              DEBUG_PRINT ("\\x%02X", vocab[i].bytes[j]);
            }
        }
      DEBUG_PRINT ("'\n");
    }
  DEBUG_PRINT ("\n");
}

int
find_in_vocabulary (VocabularyEntry *vocab, int vocab_size, ByteSequence seq)
{
  for (int i = 0; i < vocab_size; i++)
    {
      if (vocab[i].length == seq.length && memcmp (vocab[i].bytes, seq.bytes, seq.length) == 0)
        {
          return i;
        }
    }
  return -1;
}

void
free_vocabulary (VocabularyEntry *vocab, int vocab_size)
{
  for (int i = 0; i < vocab_size; i++)
    {
      free (vocab[i].bytes);
    }
}
