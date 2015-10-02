/* An implementation of talloc, an equivalent to malloc(), but the
 * pointer to the allocated memory is stored, which enables easy
 * garbage collection.
 * 04.30.2015
 * Sabastian Mugazambi, Josh Pitkofsky, and Frederik Roenn Stensaeth
 */

#include "talloc.h"

// Global Variables
Value *garbage = NULL;

// Create a new CONS_TYPE value node.
Value *cons_talloc(Value *car_value, Value *cdr_value) {
    // Create a new cons cell with the appropriate car and cdr.
    Value *return_val = malloc(sizeof(Value));
    (*return_val).type = CONS_TYPE;
    ((*return_val).c).car = car_value;
    ((*return_val).c).cdr = cdr_value;
    
    return return_val;
}

// Create a new NULL_TYPE value node.
Value *makeNull_talloc() {
    // Create new Value struct and set type to NULL_TYPE.
    Value *val = malloc(sizeof(Value));
    (*val).type = NULL_TYPE;
    
    return val;
}

// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in some kind of list; a linked list would do fine, but insert
// here whatever code you'll need to do so; don't call functions in the
// pre-existing linkedlist.h. Otherwise you'll end up with circular
// dependencies, since you're going to modify the linked list to use talloc.
void *talloc(size_t size) {
    size_t *allocated_pointer = malloc(size);
    
    if (garbage == NULL) {
        garbage = makeNull_talloc();
    }
    
    Value *value_pointer = malloc(sizeof(Value));
    (*value_pointer).type = PTR_TYPE;
    (*value_pointer).p = allocated_pointer;
    
    garbage = cons_talloc(value_pointer, garbage);
    
    return allocated_pointer;
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated in lists to hold those pointers.
void tfree() {
    // CODE
    
    if (garbage != NULL) {
        while ((*garbage).type == CONS_TYPE) {
            free((*((*garbage).c).car).p);
            
            free(((*garbage).c).car);

            Value *garbage_cdr_tmp = ((*garbage).c).cdr;
            free(garbage);
            garbage = garbage_cdr_tmp;
        }
        
        free(garbage);
        
        garbage = NULL;
    }    
}

// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status) {
    // CODE
    
    tfree();
    exit(status);
}