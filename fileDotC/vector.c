/* A program that implements an ArrayList in C. */
// Frederik Roenn Stensaeth
// 04.21.2015

#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
// You might need other includes for printing or whatnot


/* Take an already existing Vector as a parameter. (Do not create a Vector
 inside this function. Set its internal memory size to match the value given in
 the parameter, allocate an appropriately-sized C array to hold the actual data,
 and initialize the size variable to 0. */

void init(Vector *vector, int memorySize)
{
    // Your init code goes in here
    // Set size to 0;
    (*vector).size = 0;
    
    // Create array for the vector.
    (*vector).array = malloc(memorySize * sizeof(int));
    
    // Set memorySize to appropriate value.
    (*vector).memorySize = memorySize;
}


// Now do this again for all of the other functions in vector.h

/* Removes the array associated with the Vector. */

void cleanup(Vector *vector) {
    // Free the array and set size and memorySize to 0.
    free((*vector).array);
    (*vector).size = 0;
    (*vector).memorySize = 0;
}

/* Print out the Vector for debugging purposes. */

void print(Vector *vector) {
    if ((*vector).size == 0) {
        printf("[]\n");
    } else if ((*vector).size == 1) {
        printf("[ %i ]\n", (*vector).array[0]);
    // If size is greater than 1, printing will depend on location
    // in the array of the element.
    } else {
        int i = 0;
        for (i = 0 ; i < (*vector).size ; i++) {
            if (i == 0) {
                printf("[ %i, ", (*vector).array[i]);
            } else if (i == ((*vector).size - 1)) {
                printf("%i ]\n", (*vector).array[i]);
            } else {
                printf("%i, ", (*vector).array[i]);
            }
        }
    }
}

/* Insert value at location inside the Vector. If the list has items in
 positions 0 through n-1, you are only permitted to insert new values at
 locations 0 through n. (This is identical to Java's ArrayList behavior.) If
 there is already data at the location you are inserting into, insert slides the
 rest of the data down one position to make room for the new value. Returns 1 if
 success, and 0 if the location is invalid (less than 0, or greater than the
 size). Inserting at the very end of the current data (at position equal to
 size) is fine. All inserts should increase size by 1.

 If the internal array is too small to fit a new value, a new array of twice the
 size is created with malloc. The values are copied, the Vector is appropriately
 updated, and the old array is freed.) */

int insert(Vector *vector, int location, int value) {
    // Invalid location.
    if (((*vector).size < location) | (location < 0)) {
        return 0;
    }
    // Array is empty. Create array of size 1 and update memorySize.
    if (((*vector).size == 0) && ((*vector).memorySize == 0)) {
        (*vector).array = malloc(sizeof(int));
        (*vector).memorySize = 1;
        (*vector).array[0] = value;
        (*vector).size = 1;
        return 1;
    }
    // Array is full, so need to double the size of the memory allocated and transfer entries.
    else if ((*vector).size == (*vector).memorySize) {
        int *tempArray = malloc(((*vector).memorySize * 2) * sizeof(int));
        int i;
        for (i = 0 ; i < ((*vector).memorySize * 2) ; i++) {
            tempArray[i] = 0;
        }
        // Transfer entries over to the new array.
        for (i = 0 ; i < (*vector).size ; i++) {
            tempArray[i] = (*vector).array[i];
        }
        // Free the old array.
        free((*vector).array);
        // Set tempArray to be the array of the vector.
        (*vector).array = tempArray;
        // Update memorySize.
        (*vector).memorySize = (*vector).memorySize * 2;
    }
        
    // Enter value at location n
    if ((*vector).size == location) {
        (*vector).array[location] = value;
    // Location is between 0 and n - 1, so need to shift entries. 
    } else {
        int i;
        // Shift all values after location to the right.
        for (i = (*vector).size - 1; i > location ; i--) {
            (*vector).array[i] = (*vector).array[i - 1];
        }
        // Add value at location.
        (*vector).array[location] = value;
    }
    
    // Increment size by 1.
    (*vector).size = (*vector).size + 1;
    return 1;
}


/* Obtains value at location inside the Vector. Returns 1 if success, and 0 if
 the location is invalid (less than 0, or greater than or equal to the
 size). The value itself is returned through the parameter value. */

int get(Vector *vector, int location, int *value) {
    // Invalid location.
    if (((*vector).size <= location) | (location < 0)) {
        return 0;
    }
    // Set value to point to the value at array[location].
    *value = (*vector).array[location];
    
    return 1;
}


/* Deletes value at location inside the Vector.  Returns 1 if success, and 0 if
 the location is invalid (less than 0, or greater than or equal to the
 size). When an item is deleted, everything else past it in the array should be
 slid down. The internal array itself does not need to be compressed, i.e., you
 don't need to halve it in size if it becomes half full, or anything like
 that. */

int delete(Vector *vector, int location) {
    // Invalid location.
    if (((*vector).size <= location) | (location < 0)) {
        return 0;
    // If only one element in the vector, free the vector.
    } else if ((*vector).size == 1) {
        free((*vector).array);
    // If location is valid and size is greater than 1,
    // shift elements to the left, but only elements that are
    // at higher indexes than the location index.
    } else {
        int i;
        for (i = location ; i < (*vector).size ; i++) {
            (*vector).array[i] = (*vector).array[i + 1];
        }
        (*vector).array[(*vector).size - 1] = 0;
    }
    (*vector).size = (*vector).size - 1;
    
    return 1;
}

// Main.
//int main() {
//    return 0;
//}