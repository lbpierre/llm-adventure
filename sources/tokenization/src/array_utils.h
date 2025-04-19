#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <stddef.h>

// Define the Array structure
typedef struct {
    int *data;        // Pointer to the array of integers
    size_t size;      // Number of elements in the array
    size_t capacity;  // Total capacity (allocated space) for the array
} Array;

// Function to initialize the dynamic array with an initial capacity
void array_init(Array *array, size_t initial_capacity);

// Function to add an element to the dynamic array
void array_add(Array *array, int value);

// Function to get an element at a given index
int array_get(Array *array, size_t index);

// Function to insert an element at a specific position in the array
void array_insert(Array *array, size_t index, int value);

// Function to delete an element at a specific index
void array_delete(Array *array, size_t index);

// Function to print the array elements
void array_print(Array *array);

// Function to free the memory used by the array
void array_free(Array *array);

#endif
