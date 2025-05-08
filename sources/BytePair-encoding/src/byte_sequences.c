/*
  byte_sequences.c - Implentation of ByteSequence related functions
*/

#include "bpe.h"
#include <stdlib.h>
#include <string.h>

// create a new byte sequence
ByteSequence
create_byte_sequence (unsigned char *bytes, int length)
{
  ByteSequence seq;
  seq.bytes = malloc (length);
  seq.length = length;
  memcpy (seq.bytes, bytes, length);
  return seq;
}

// Free a ByteSequence
void
free_byte_sequence (ByteSequence seq)
{
  free (seq.bytes);
}

// Compare two byte sequences
bool
byte_sequence_equals (ByteSequence bs1, ByteSequence bs2)
{
  if (bs1.length != bs2.length)
    return false;
  return memcmp (bs1.bytes, bs2.bytes, bs1.length) == 0;
}

// Tokenize text into individual bytes
ByteSequences
tokinize_to_bytes (const unsigned char *text, int text_len)
{
  ByteSequences sequences;
  sequences.sequences = malloc (sizeof (ByteSequence) * text_len);
  sequences.count = 0;

  for (int i = 0; i < text_len; i++)
    {
      sequences.sequences[sequences.count] = create_byte_sequence ((unsigned char *) &text[i], 1);
      sequences.count++;
    }

  return sequences;
}

// Free memory allocated for byte sequences
void
free_byte_sequences (ByteSequences seqs)
{
  for (int i = 0; i < seqs.count; i++)
    {
      free_byte_sequence (seqs.sequences[i]);
    }
  free (seqs.sequences);
}

// Print byte sequence as hex values for debugging purposes
void
print_byte_sequence (ByteSequence seq)
{
  for (int i = 0; i < seq.length; i++)
    {
      DEBUG_PRINT ("%02X ", seq.bytes[i]);
    }
}

// Print the current state of byte sequences
void
print_byte_sequences (ByteSequences seqs)
{
  DEBUG_PRINT ("Current byte sequences (%d): ", seqs.count);
  for (int i = 0; i < seqs.count; i++)
    {
      DEBUG_PRINT ("[");
      print_byte_sequence (seqs.sequences[i]);
      DEBUG_PRINT ("] ");
    }

  DEBUG_PRINT ("\n");
}

// Print readable version of byte sequences (for ASCII content only)
void
print_readable (ByteSequences seqs)
{
  DEBUG_PRINT ("Readable: '");
  for (int i = 0; i < seqs.count; i++)
    {
      for (int j = 0; j < seqs.sequences[i].length; j++)
        {
          if (seqs.sequences[i].bytes[j] >= 0x20 && seqs.sequences[i].bytes[j] <= 0x7e)
            {
              DEBUG_PRINT ("%c", seqs.sequences[i].bytes[j]);
            }
          else
            {
              DEBUG_PRINT ("\\x%02X", seqs.sequences[i].bytes[j]);
            }
        }
    }
  DEBUG_PRINT ("'\n");
}
