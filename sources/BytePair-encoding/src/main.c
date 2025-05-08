#include "bpe.h"
#include "bpe_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Check if we should train or use existing vocabulary
    if (argc < 3) {
        printf("Usage: %s [train|encode|decode] [options...]\n", argv[0]);
        printf("\nCommands:\n");
        printf("  train <text_file> <vocab_file> <num_merges>\n");
        printf("  encode <text_file> <vocab_file> <output_file>\n");
        printf("  decode <encoded_file> <vocab_file> <output_file>\n");
        return 1;
    }

    // Parse command
    const char *command = argv[1];
    
    if (strcmp(command, "train") == 0) {
        // Training mode
        if (argc < 5) {
            printf("Usage: %s train <text_file> <vocab_file> <num_merges>\n", argv[0]);
            return 1;
        }
        
        const char *text_file = argv[2];
        const char *vocab_file = argv[3];
        int num_merges = atoi(argv[4]);
        
        // Read input text file
        FILE *fp = fopen(text_file, "rb");
        if (!fp) {
            fprintf(stderr, "Error opening text file: %s\n", text_file);
            return 1;
        }
        
        // Get file size
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        // Allocate memory for text
        unsigned char *text = (unsigned char *)malloc(file_size + 1);
        if (!text) {
            fprintf(stderr, "Memory allocation failed for text\n");
            fclose(fp);
            return 1;
        }
        
        // Read file content
        size_t bytes_read = fread(text, 1, file_size, fp);
        fclose(fp);
        text[bytes_read] = '\0';  // Null-terminate
        
        printf("Training BPE model with %d merges...\n", num_merges);
        
        // Train BPE model
        int vocab_size;
        VocabularyEntry *vocab = train_bpe(text, bytes_read, num_merges, &vocab_size);
        if (!vocab) {
            fprintf(stderr, "BPE training failed\n");
            free(text);
            return 1;
        }
        
        // Save vocabulary to file
        if (!save_bpe_vocab(vocab_file, vocab, vocab_size)) {
            fprintf(stderr, "Failed to save vocabulary\n");
            free_vocabulary(vocab, vocab_size);
            free(text);
            return 1;
        }
        
        // Clean up
        free_vocabulary(vocab, vocab_size);
        free(text);
        printf("Training complete. Vocabulary saved to %s\n", vocab_file);
        
    } else if (strcmp(command, "encode") == 0) {
        // Encoding mode
        if (argc < 5) {
            printf("Usage: %s encode <text_file> <vocab_file> <output_file>\n", argv[0]);
            return 1;
        }
        
        const char *text_file = argv[2];
        const char *vocab_file = argv[3];
        const char *output_file = argv[4];
        
        // Read input text file
        FILE *fp = fopen(text_file, "rb");
        if (!fp) {
            fprintf(stderr, "Error opening text file: %s\n", text_file);
            return 1;
        }
        
        // Get file size
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        // Allocate memory for text
        unsigned char *text = (unsigned char *)malloc(file_size + 1);
        if (!text) {
            fprintf(stderr, "Memory allocation failed for text\n");
            fclose(fp);
            return 1;
        }
        
        // Read file content
        size_t bytes_read = fread(text, 1, file_size, fp);
        fclose(fp);
        text[bytes_read] = '\0';  // Null-terminate
        
        // Load vocabulary
        int vocab_size;
        VocabularyEntry *vocab = load_bpe_vocab(vocab_file, &vocab_size);
        if (!vocab) {
            fprintf(stderr, "Failed to load vocabulary\n");
            free(text);
            return 1;
        }
        
        // Encode text
        int encoded_len;
        int *encoded = encode(text, bytes_read, vocab, vocab_size, &encoded_len);
        if (!encoded) {
            fprintf(stderr, "Encoding failed\n");
            free_vocabulary(vocab, vocab_size);
            free(text);
            return 1;
        }
        
        // Save encoded data
        FILE *out_fp = fopen(output_file, "wb");
        if (!out_fp) {
            fprintf(stderr, "Error opening output file: %s\n", output_file);
            free(encoded);
            free_vocabulary(vocab, vocab_size);
            free(text);
            return 1;
        }
        
        // Write encoded length and data
        fwrite(&encoded_len, sizeof(int), 1, out_fp);
        fwrite(encoded, sizeof(int), encoded_len, out_fp);
        fclose(out_fp);
        
        // Clean up
        free(encoded);
        free_vocabulary(vocab, vocab_size);
        free(text);
        printf("Encoding complete. Output saved to %s\n", output_file);
        
    } else if (strcmp(command, "decode") == 0) {
        // Decoding mode
        if (argc < 5) {
            printf("Usage: %s decode <encoded_file> <vocab_file> <output_file>\n", argv[0]);
            return 1;
        }
        
        const char *encoded_file = argv[2];
        const char *vocab_file = argv[3];
        const char *output_file = argv[4];
        
        // Load vocabulary
        int vocab_size;
        VocabularyEntry *vocab = load_bpe_vocab(vocab_file, &vocab_size);
        if (!vocab) {
            fprintf(stderr, "Failed to load vocabulary\n");
            return 1;
        }
        
        // Read encoded data
        FILE *fp = fopen(encoded_file, "rb");
        if (!fp) {
            fprintf(stderr, "Error opening encoded file: %s\n", encoded_file);
            free_vocabulary(vocab, vocab_size);
            return 1;
        }
        
        // Read encoded length
        int encoded_len;
        if (fread(&encoded_len, sizeof(int), 1, fp) != 1) {
            fprintf(stderr, "Error reading encoded length\n");
            fclose(fp);
            free_vocabulary(vocab, vocab_size);
            return 1;
        }
        
        // Allocate memory for encoded data
        int *encoded = (int *)malloc(encoded_len * sizeof(int));
        if (!encoded) {
            fprintf(stderr, "Memory allocation failed for encoded data\n");
            fclose(fp);
            free_vocabulary(vocab, vocab_size);
            return 1;
        }
        
        // Read encoded data
        if (fread(encoded, sizeof(int), encoded_len, fp) != encoded_len) {
            fprintf(stderr, "Error reading encoded data\n");
            free(encoded);
            fclose(fp);
            free_vocabulary(vocab, vocab_size);
            return 1;
        }
        fclose(fp);
        
        // Decode data
        int result_len;
        unsigned char *result = decode(encoded, encoded_len, vocab, vocab_size, &result_len);
        if (!result) {
            fprintf(stderr, "Decoding failed\n");
            free(encoded);
            free_vocabulary(vocab, vocab_size);
            return 1;
        }
        
        // Save decoded data
        FILE *out_fp = fopen(output_file, "wb");
        if (!out_fp) {
            fprintf(stderr, "Error opening output file: %s\n", output_file);
            free(result);
            free(encoded);
            free_vocabulary(vocab, vocab_size);
            return 1;
        }
        
        fwrite(result, sizeof(unsigned char), result_len, out_fp);
        fclose(out_fp);
        
        // Clean up
        free(result);
        free(encoded);
        free_vocabulary(vocab, vocab_size);
        printf("Decoding complete. Output saved to %s\n", output_file);
        
    } else {
        printf("Unknown command: %s\n", command);
        printf("Available commands: train, encode, decode\n");
        return 1;
    }
    
    return 0;
}
