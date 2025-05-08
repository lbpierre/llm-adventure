/*
 utils.c - Utility functions for BPE implementation
 */

#include "bpe.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void
print_bytes_hex (unsigned char *bytes, int len)
{
  for (int i = 0; i < len; i++)
    {
      DEBUG_PRINT ("%02X ", bytes[i]);
    }
  DEBUG_PRINT ("\n");
}

char *
read_file_to_buffer (const char *filename, size_t *text_len)
{
  FILE* file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open file");
    return NULL;
  }

  // Seek to the end to find the file size
  if (fseek(file, 0, SEEK_END) != 0) {
    perror("Cannot seek at the end of the file");
    fclose(file);
    return NULL;
  }

  long size = ftell(file);
  if (size < 0) {
    perror("file empty");
    fclose(file);
    return NULL;
  }

  rewind(file); // Reset to beginning

  // Allocate bufffer (+1 for null-terminator)
  char* buffer = malloc(size + 1);
  if (!buffer) {
    perror("failed to allocate memory for the buffer");
    fclose(file);
    return NULL;
  }

  // Read file content
  size_t len = fread(buffer, 1, size, file);
  if (len != (size_t)size) {
    perror("Failed to read completly the file");
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[len] = '\0';
  fclose(file);

  if (text_len) *text_len = size;
  return buffer;
}
