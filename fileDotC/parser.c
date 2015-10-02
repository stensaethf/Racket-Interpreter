/* parser.c
 * An implementation of rackter parsing in c.
 * Josh Pitkofsky, Sabastian Mugazambi, Frederik Roenn Stensaeth
 * 05.10.2015
 */


#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Takes a tree, a depth constant, and a token. Adds the token to the tree
// if it is not a close parentheses. Pops appropriate amount of tokens
// of until a open parentheses is reached. Adds that new list to the stack.
Value *addToParseTree(Value *tree, int *depth, Value *token) {
    // add the token to the stack and increment depth if it is an open
    // parentheses.
    if ((*token).type == OPEN_TYPE) {
        (*depth) = (*depth) + 1;
        tree = cons(token, tree);
    // pop tokens off until an open parentheses is reached. Add the new
    // list to the stack.
    } else if ((*token).type == CLOSE_TYPE) {
        // make sure there are enough parentheses.
        if ((*depth) == 0) {
            printf("Syntax error: too many close parentheses.\n");
            texit(1);
        } else {
            Value *list_new = makeNull();
            Value *pointer_to_tree = tree;
            while ((*car(pointer_to_tree)).type != OPEN_TYPE) {
                list_new = cons(car(pointer_to_tree), list_new);
                // point to the next value in the stack.
                pointer_to_tree = cdr(pointer_to_tree);
                // set the first item in the stack to be the item after
                // the open parentheses.
                tree = cdr(pointer_to_tree);
            }
            // hack to make sure () is handled correctly.
            if ((*list_new).type == NULL_TYPE) {
                tree = cons(list_new, cdr(tree));
            } else {
                tree = cons(list_new, tree);
            }
            (*depth) = (*depth) - 1;
        }
    // add the token to the stack if it is not a close parentheses.
    } else {
        tree = cons(token, tree);
    }
    return tree;
}

// Takes a list of tokens from a Racket program, and returns a pointer to a
// parse tree representing that program.
Value *parse(Value *tokens) {
   Value *tree = makeNull();
    int depth = 0;
    
    // loop over the tokens and pass them to addToParseTree which handles
    // them correctly depending on what they are.
    Value *current = tokens;
    assert(current != NULL && "Error (parse): null pointer");
    while (current->type != NULL_TYPE) {
        Value *token = car(current);
        tree = addToParseTree(tree, &depth, token);
        current = cdr(current);
    }
    // make sure there were enough parentheses.
    if (depth != 0) {
        printf("Syntax error: not enough close parentheses.\n");
        texit(1);
    }
    
    tree = reverse(tree);
    
    return tree;
}

// Prints the tree to the screen in a readable fashion. It should look just like
// Racket code; use parentheses to indicate subtrees.
void printTree(Value *tree) {    
    Value *list_pointer = tree;
    
    // handle printing dependent on what type the item in the list is.
    while ((*list_pointer).type == CONS_TYPE) {
        Value *car_of_list_pointer = car(list_pointer);
        //printf("%u", (*car_of_list_pointer).type);
        
        switch ((*car_of_list_pointer).type) {
            case STR_TYPE:
                printf("%s", (*car_of_list_pointer).s);
                break;
            case DOUBLE_TYPE:
                printf("%f", (*car_of_list_pointer).d);
                break;
            case INT_TYPE:
                printf("%i", (*car_of_list_pointer).i);
                break;
            case CONS_TYPE:
                printf("(");
                printTree(car_of_list_pointer);
                printf(")");
                break;
            case NULL_TYPE:
                printf("()");
                break;
            case OPEN_TYPE:
                // do nothing.
                break;
            case CLOSE_TYPE:
                // do nothing.
                break;
            case BOOL_TYPE:
                if ((*car_of_list_pointer).i == 1) {
                    printf("#t");
                } else {
                    printf("#f");
                }
                break;
            case SYMBOL_TYPE:
                printf("%s", (*car_of_list_pointer).s);
                break;
            case PTR_TYPE:
                break;
        }
        
        if ((*((*list_pointer).c).cdr).type != NULL_TYPE) {
            printf(" ");
        }
        
        
        list_pointer = ((*list_pointer).c).cdr;
    }
}