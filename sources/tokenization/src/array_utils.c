#include <stdio.h>
#include <stdlib.h>
#include "array_utils.h"

// Function to initialize the array
void array_init(Array *array, size_t initial_capacity) {
    array->data = malloc(initial_capacity * sizeof(int));
    array->size = 0;
    array->capacity = initial_capacity;
}

// Function to add an element at the end of the array
void array_add(Array *array, int value) {
    if (array->size == array->capacity) {
        array->capacity *= 2;  // Double the capacity
        array->data = realloc(array->data, array->capacity * sizeof(int));
    }
    array->data[array->size++] = value;
}

// Function to get an element at a given position
int array_get(Array *array, size_t index) {
    if (index < array->size) {
        return array->data[index];
    } else {
        printf("Index out of bounds!\n");
        exit(EXIT_FAILURE);  // Exit if the index is out of bounds
    }
}

// Function to insert an element at a given position
void array_insert(Array *array, size_t index, int value) {
    if (index > array->size) {
        printf("Index out of bounds!\n");
        return;
    }

    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = realloc(array->data, array->capacity * sizeof(int));
    }

    // Shift elements to the right
    for (size_t i = array->size; i > index; i--) {
        array->data[i] = array->data[i - 1];
    }

    // Insert the new element
    array->data[index] = value;
    array->size++;
}

// Function to delete an element at a given index
void array_delete(Array *array, size_t index) {
    if (index >= array->size) {
        printf("Index out of bounds!\n");
        return;
    }

    // Shift elements to the left
    for (size_t i = index; i < array->size - 1; i++) {
        array->data[i] = array->data[i + 1];
    }

    array->size--;
}

// Function to print the array
void array_print(Array *array) {
    for (size_t i = 0; i < array->size; i++) {
        printf("%d ", array->data[i]);
    }
    printf("\n");
}

// Function to free the array memory
void array_free(Array *array) {
    free(array->data);
}
