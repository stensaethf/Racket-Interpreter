/* A linkedlist implementation using cons cell holding a value
 * and a pointer to the next cons cell in the list. 
 * 04.28.2015
 * Sabastian Mugazambi, Josh Pitkofsky, and Frederik Roenn Stensaeth
 */

// DISCLAIMER:
// Dave answered questions on piazza the morning of the 30th,
// no necessary changes were done then. These were questions that
// had not been specified in the assignment.

#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


// Create a new NULL_TYPE value node.
Value *makeNull() {
    // Create new Value struct and set type to NULL_TYPE.
    Value *val = malloc(sizeof(Value));
    (*val).type = NULL_TYPE;
    
    return val;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *car_value, Value *cdr_value) {
//    // Invalid car? Do a check.
//    assert((*car_value).type != NULL_TYPE);
//    // Invalid cdr? Do a check.
//    assert(((*cdr_value).type != INT_TYPE));
//    assert(((*cdr_value).type != DOUBLE_TYPE));
//    assert(((*cdr_value).type != STR_TYPE));
//
//    // If the cdr is a cons cell, loop over the list and
//    // verify that is correctly formatted.
//    if ((*cdr_value).type == CONS_TYPE) {
//        Value *check = cdr_value;
//        
//        // Loop over the cdr and assert that it is correctly
//        // formatted.
//        while ((*check).type == CONS_TYPE) {
//            if ((*car(check)).type == NULL_TYPE) {
//                assert((*car(check)).type != NULL_TYPE);
//            }
//
//            check = cdr(check);
//        }
//        
//        // Should end in a NULL_TYPE, so check that.
//        if ((*check).type != NULL_TYPE) {
//            assert((*check).type == NULL_TYPE);
//        }
//    }
    
    // Create a new cons cell with the appropriate car and cdr.
    Value *return_val = malloc(sizeof(Value));
    (*return_val).type = CONS_TYPE;
    ((*return_val).c).car = car_value;
    ((*return_val).c).cdr = cdr_value;
    
    return return_val;
}

// Display the contents of the linked list to the screen in some kind of
// readable format.
void display(Value *list) {
    // Make sure input is valid.
    //assert((*list).type == CONS_TYPE);

    printf("[ ");
    
    // Create a new pointer to the list that will be used to
    // loop over the list.
    Value *list_copy = list;
    
    // Loop over the list and print elements along the way.
    while ((*list_copy).type == CONS_TYPE) {
        Value *car_of_listcopy = car(list_copy);
        
        // Find the type of the car and print appropriately.
        switch ((*car_of_listcopy).type) {
            case INT_TYPE:
                printf ("%i", (*car_of_listcopy).i);
                break;
            case DOUBLE_TYPE:
                printf ("%f", (*car_of_listcopy).d);
                break;
            case STR_TYPE:
                printf ("%s", (*car_of_listcopy).s);
                break;
            case CONS_TYPE:
                // list as item? call display on the list.
                display(car_of_listcopy);
                break;
            case NULL_TYPE:
                printf("NULL");
                break;
        }
        //free(car_of_listcopy);

        Value *cdr_of_listcopy = cdr(list_copy);
        
        // Only print a comma if the end is not reached yet.
        if ((*cdr_of_listcopy).type != NULL_TYPE) {
            printf(" , ");
        }
        
        // Go to the next element in the list.
        list_copy = cdr_of_listcopy;
    }
    
    // Make sure that the list ends in a NULL_TYPE.
//    if ((*list_copy).type != NULL_TYPE) {
//        assert((*list_copy).type == NULL_TYPE); // exit.
//    }

    //free(list_copy);
        
    printf(" ]\n");
}

// Return a new list that is the reverse of the one that is passed in. All
// content within the list should be duplicated; there should be no shared
// memory between the original list and the new one.
//
// FAQ: What if there are nested lists inside that list?
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
Value *reverse(Value *list) {
    // Invalid input? Check it.
    //assert((*list).type == CONS_TYPE);
    
    // Create a NULL_TYPE that will be the end of the reversed
    // list.
    Value *val = makeNull();

    Value *list_copy = list;
    
    // Loop over the given list and copy elements along the way.
    while ((*list_copy).type == CONS_TYPE) {
        Value *copy_of_element = malloc(sizeof(Value));

        // Find the type of the element and create a new appropriate
        // element containing the same information.
        switch ((*car(list_copy)).type) {
            case INT_TYPE:
                (*copy_of_element).type = INT_TYPE;
                (*copy_of_element).i = (*car(list_copy)).i;
                break;
            case DOUBLE_TYPE:
                (*copy_of_element).type = DOUBLE_TYPE;
                (*copy_of_element).d = (*car(list_copy)).d;
                break;
            case STR_TYPE:
                (*copy_of_element).type = STR_TYPE;
                // Malloc enough space for the string.
                (*copy_of_element).s = malloc(strlen((*car(list_copy)).s) + 1);
                // Copy the string over.
                strcpy((*copy_of_element).s, (*car(list_copy)).s);
                break;
            case CONS_TYPE:
                // nested list? call reverse on it.
                free(copy_of_element);
                Value *copy_of_element_tmp = reverse(car(list_copy));
                copy_of_element = reverse(copy_of_element_tmp);
                cleanup(copy_of_element_tmp);
                break;
            case NULL_TYPE:
                (*copy_of_element).type = NULL_TYPE;
                break;
        }
        
        // Put the copied element on the reversed list and move on
        // to the next item in the list.
        val = cons(copy_of_element, val);
        list_copy = cdr(list_copy);
    }
    
    //free(list_copy);
    
    return val;
}

// Frees up all memory directly or indirectly referred to by list. Note that
// this linked list might consist of linked lists as items, so you'll need to
// clean them up as well.
// FAQ: What if there are nested lists inside that list?
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
void cleanup(Value *list) {
    // Make sure that the input is valid.
    //assert((*list).type == CONS_TYPE);

    Value *list_copy = list;
    
    // Loop over the list and free items along the way.
    while ((*list_copy).type == CONS_TYPE) {
        Value *car_listcopy = car(list_copy);
        
        // If the item is a string, make sure to free the
        // actual string itself from the heap.
        if ((*car_listcopy).type == STR_TYPE) {
            free((*car_listcopy).s);
        }
        if ((*car_listcopy).type == CONS_TYPE) {
            cleanup(car_listcopy);
        } else {
            // Free the car Value struct.
            free(car_listcopy);
        }
        // Copy the pointer to the cdr.
        Value *list_copy_tmp = cdr(list_copy);
        // Free the cons cell.
        free(list_copy);
        // Move on to the cdr of the cons cell that way just freed.
        list_copy = list_copy_tmp;
    }
    // Free the last item in the list. Should be of NULL_TYPE.
    free(list_copy);

}

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list) {
    // make sure input is valid.   
    assert ((*list).type == CONS_TYPE);
    
    return ((*list).c).car;
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list) {
    // make sure input is valid.
    assert((*list).type == CONS_TYPE);
    
    return ((*list).c).cdr;
}

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make 
// sure that this is a legitimate operation.
bool isNull(Value *value) {
    // make sure input is valid.
    assert(value != NULL);
    
    if ((*value).type != NULL_TYPE) {
        return 0;
    }
 
    return 1;
}

// Measure length of list. Use assertions to make sure that this is a 
// legitimate operation.
int length(Value *list) {
    // make sure input is valid.
    assert((*list).type == CONS_TYPE);
    
    int count = 0;
    Value *val = list;
    
    // loop over the list and count the number of elements.
    while ((*val).type == CONS_TYPE) {
        count++;
        
        // Move on to the cdr of the cons cell - next item in the list.
        val = cdr(val);
    }
    
    assert((*val).type == NULL_TYPE);
    
    return count;
}





