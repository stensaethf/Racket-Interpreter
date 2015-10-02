/* interpreter.c
 * An impletementation of an interpreter for racket code.
 * Josh Pitkofsky, Sabastian Mugazambi, Frederik Stensaeth 
 * 05.17.2015
 */

#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"
#include <string.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "parser.h"
#include <math.h>

// Method prints an error message and exits the program.
void evaluationError() {
    printf("Evaluation error\n");
    texit(1);
}

// primtiveEqual() takes two numbers and checks whether they are equal to
// eachother or not. Returns #t/ #f depending on the outcome of this comparison.
Value *primitiveEqual(Value *args) {
    // make sure that we are given two elements in the arguments list.
    // no more, no less.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(cdr(args))).type != NULL_TYPE) {
        evaluationError();
    }
    // args is now verified to only contain two elements.
    
    Value *first = car(args);
    Value *second = car(cdr(args));
    double first_d;
    double second_d;
    
    // get the numbers from the arguemnts, make sure that they are of proper
    // type.
    // first argument.
    if ((*first).type == INT_TYPE) {
        first_d = (double)(*first).i;
    } else if ((*first).type == DOUBLE_TYPE) {
        first_d = (*first).d;
    } else {
        evaluationError();
    }
    // second argument.
    if ((*second).type == INT_TYPE) {
        second_d = (double)(*second).i;
    } else if ((*second).type == DOUBLE_TYPE) {
        second_d = (*second).d;
    } else {
        evaluationError();
    }
    
    int result;
    if ((first_d - second_d) == 0) {
        result = 1;
    } else {
        result = 0;
    }
    
    // result now holds the result of applying = to the arguments.
    Value *result_value = talloc(sizeof(Value));
    (*result_value).type = BOOL_TYPE;
    (*result_value).i = result;
    return result_value;
}

// primtiveGreaterThan() takes to numbers and checks whether the first is
// larger than the second or not. Returns #t/ #f depending on the outcome of
// this comparison.
Value *primitiveGreaterThan(Value *args) {
    // make sure that we are given two elements in the arguments list.
    // no more, no less.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(cdr(args))).type != NULL_TYPE) {
        evaluationError();
    }
    // args is now verified to only contain two elements.
    
    Value *first = car(args);
    Value *second = car(cdr(args));
    double first_d;
    double second_d;
    
    // get the numbers from the arguemnts, make sure that they are of proper
    // type.
    // first argument.
    if ((*first).type == INT_TYPE) {
        first_d = (double)(*first).i;
    } else if ((*first).type == DOUBLE_TYPE) {
        first_d = (*first).d;
    } else {
        evaluationError();
    }
    // second argument.
    if ((*second).type == INT_TYPE) {
        second_d = (double)(*second).i;
    } else if ((*second).type == DOUBLE_TYPE) {
        second_d = (*second).d;
    } else {
        evaluationError();
    }
    
    int result;
    // >
    if ((first_d - second_d) > 0) {
        result = 1;
    } else {
        result = 0;
    }
    
    // result now holds the result of applying > to the arguments.
    Value *result_value = talloc(sizeof(Value));
    (*result_value).type = BOOL_TYPE;
    (*result_value).i = result;
    return result_value;
}

// primtiveLessThan() takes to numbers and checks whether the first is smaller
// than the second or not. Returns #t/ #f depending on the outcome of this
// comparison.
Value *primitiveLessThan(Value *args) {
    // make sure that we are given two elements in the arguments list.
    // no more, no less.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(cdr(args))).type != NULL_TYPE) {
        evaluationError();
    }
    // args is now verified to only contain two elements.
    
    Value *first = car(args);
    Value *second = car(cdr(args));
    double first_d;
    double second_d;
    
    // get the numbers from the arguemnts, make sure that they are of proper
    // type.
    // first argument.
    if ((*first).type == INT_TYPE) {
        first_d = (double)(*first).i;
    } else if ((*first).type == DOUBLE_TYPE) {
        first_d = (*first).d;
    } else {
        evaluationError();
    }
    // second argument.
    if ((*second).type == INT_TYPE) {
        second_d = (double)(*second).i;
    } else if ((*second).type == DOUBLE_TYPE) {
        second_d = (*second).d;
    } else {
        evaluationError();
    }
    
    int result;
    // <
    if ((first_d - second_d) < 0) {
        result = 1;
    } else {
        result = 0;
    }
    
    // result now holds the result of applying < to the arguments.
    Value *result_value = talloc(sizeof(Value));
    (*result_value).type = BOOL_TYPE;
    (*result_value).i = result;
    return result_value;
}

// primitiveModulo() takes two arguments and finds the modular remainder between
// them. Returns this number.
Value *primitiveModulo(Value *args) {
    // make sure that we are given two elements in the arguments list.
    // no more, no less.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(cdr(args))).type != NULL_TYPE) {
        evaluationError();
    }
    // args is now verified to only contain two elements.
    
    Value *first = car(args);
    Value *second = car(cdr(args));
    int first_i;
    int second_i;
    
    // get the integers from the arguments, make sure that they are ints.
    if ((*first).type == INT_TYPE) {
        first_i = (*first).i;
    } else {
        evaluationError();
    }
    if ((*second).type == INT_TYPE) {
        second_i = (*second).i;
    } else {
        evaluationError();
    }
    
    // x mod 0 is undefined.
    if (second_i == 0) {
        evaluationError();
    }
    
    int result;
    // 0 mod x is 0.
    if (first_i == 0) {
        result = 0;
    // if first is not 0 we need to perform some math.
    } else {
        result = first_i % second_i;
        
        // if either of the numbers were negative, we need to add the second
        // number to the result.
        if (first_i < 0) {
            result = result + second_i;
        } else if (second_i < 0) {
            result = result + second_i;
        }
    }
    
    // result now holds the result of applying mod to the arguments.
    Value *result_value = talloc(sizeof(Value));
    (*result_value).type = INT_TYPE;
    (*result_value).i = result;
    return result_value;
}

// primitiveDivision() takes two arguments and divides them. Returns the result
// of this division.
Value *primitiveDivision(Value *args) {
    // make sure that we are given two elements in the arguments list.
    // no more, no less.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(cdr(args))).type != NULL_TYPE) {
        evaluationError();
    }
    // args is now verified to only contain two elements.
    
    Value *first = car(args);
    Value *second = car(cdr(args));
    double first_d;
    double second_d;
    
    // get the numbers from the arguemnts, make sure that they are of proper
    // type.
    // first argument.
    if ((*first).type == INT_TYPE) {
        first_d = (double)(*first).i;
    } else if ((*first).type == DOUBLE_TYPE) {
        first_d = (*first).d;
    } else {
        evaluationError();
    }
    // second argument.
    if ((*second).type == INT_TYPE) {
        second_d = (double)(*second).i;
    } else if ((*second).type == DOUBLE_TYPE) {
        second_d = (*second).d;
    } else {
        evaluationError();
    }
    
    // cannot divide by 0.
    if (second_d == 0.0) {
        evaluationError();
    }
    
    double result = first_d / second_d;
    int check;
    Value *result_value = talloc(sizeof(Value));
    // check whether the result is an integer or a double.
    if (result == (double)(int)result) {
        // integer
        (*result_value).type = INT_TYPE;
        (*result_value).i = (int)result;
    } else {
        // float
        (*result_value).type = DOUBLE_TYPE;
        (*result_value).d = result;
    }
    return result_value;
}

// primitiveMultiplication() takes a list of arguments and multiplies all of
// them together. Returns the result of this multiplication.
Value *primitiveMultiplication(Value *args) {
    // need to have at least 2 arguments.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != CONS_TYPE) {
        evaluationError();
    }
    // now we know that args consists of at least two elements.
    
    double result = 0;
    // take first element and add to result.
    if ((*car(args)).type == INT_TYPE) {
        result = (double)(*car(args)).i;
    } else if ((*car(args)).type == DOUBLE_TYPE) {
        result = (*car(args)).d;
    } else {
        evaluationError();
    }
    
    Value *args_ptr = cdr(args);
    // loop over remaining arguments and multiply with the first one.
    while ((*args_ptr).type != NULL_TYPE) {
        Value *car_args_ptr = car(args_ptr);
        if ((*car_args_ptr).type == INT_TYPE) {
            result = result * (double)(*car_args_ptr).i;
        } else if ((*car_args_ptr).type == DOUBLE_TYPE) {
            result = result * (*car_args_ptr).d;
        } else {
            evaluationError();
        }

        args_ptr = cdr(args_ptr);
    }
    
    // result now holds the result of multiplying the arguments.
    Value *result_value = talloc(sizeof(Value));
    (*result_value).type = DOUBLE_TYPE;
    (*result_value).d = result;
    return result_value;
}

// PrimitiveSubtraction() takes a list of arguments and 
// subctracts them from eachother.
// Returns a DOUBLE_TYPE value struct with the result.
Value *primitiveSubtraction(Value *args) {
    // make sure that we are given two elements in the arguments list.
    // no more, no less.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(cdr(args))).type != NULL_TYPE) {
        evaluationError();
    }
    
    Value *first = car(args);
    Value *second = car(cdr(args));
    double first_d = 0;
    double second_d = 0;
    
    if ((*first).type == INT_TYPE) {
        first_d = (double)(*first).i;
    } else if ((*first).type == DOUBLE_TYPE) {
        first_d = (*first).d;
    } else {
        evaluationError();
    }
    
    if ((*second).type == INT_TYPE) {
        second_d = (double)(*second).i;
    } else if ((*second).type == DOUBLE_TYPE) {
        second_d = (*second).d;
    } else {
        evaluationError();
    }
    
    double result = first_d - second_d;
    
    // result now holds the result of applygin subtraction on the arguments.
    Value *result_value = talloc(sizeof(Value));
    (*result_value).type = DOUBLE_TYPE;
    (*result_value).d = result;
    return result_value;
}

// printTreeEdit() works just like the previous way we used to print our trees,
// except for that it has added flexibility to be able to print lists returned
// from 'cons' correctly (the dots).
void printTreeEdit(Value *tree) {    
    Value *list_pointer = tree;
    int beginning = 1;
    
    char *dot_str = (char *)talloc(sizeof(char)*3);
    dot_str[0] = '.';
    dot_str[1] = ' ';
    dot_str[2] = '\0';
    // handle printing dependent on what type the item in the list is.
    while ((*list_pointer).type != NULL_TYPE) {
        // not a cons type? This means that it is part of the 'dot list'.
        // because of this we need to print the dot before printing the
        // value itself.
        Value * car_of_list_pointer = NULL;
        if ((*list_pointer).type != CONS_TYPE) {
            printf("%s", dot_str);
            car_of_list_pointer = list_pointer;
        // it is a cons type, which means that it requires "normal" printing.
        } else {
            car_of_list_pointer = car(list_pointer);
        }
        
        switch ((*car_of_list_pointer).type) {
            case STR_TYPE:
                printf("\"%s\"", (*car_of_list_pointer).s);
                break;
            case DOUBLE_TYPE:
                printf("%f", (*car_of_list_pointer).d);
                break;
            case INT_TYPE:
                printf("%i", (*car_of_list_pointer).i);
                break;
            case CONS_TYPE:
                printf("(");
                printTreeEdit(car_of_list_pointer);
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
                // do nothing.
                break;
            case CLOSURE_TYPE:
                // do nothing.
                break;
            case VOID_TYPE:
                // do nothing.
                break;
            case PRIMITIVE_TYPE:
                // do nothing.
                break;
        }
        
        // because we now allow for 'dot lists' we need to check if we
        // printed a dot, as this means that the end of the list has been
        // reached, but no NULL_TYPE was reached - so we create this.
        if ((*list_pointer).type == CONS_TYPE) {
            if ((*cdr(list_pointer)).type != NULL_TYPE) {
                printf(" ");
            }
            list_pointer = cdr(list_pointer);
        } else {
            list_pointer = talloc(sizeof(Value));
            (*list_pointer).type = NULL_TYPE;
        }
    }
}

// PrimitiveCons takes two lists and returns the lists combined in appropriate
// form.
Value *primitiveCons(Value *args) {
    // make sure that we are given two elements in the arguments list.
    // no more, no less.
    //printf("yes\n");
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(cdr(args))).type != NULL_TYPE) {
        evaluationError();
    }
    //printf("no\n");
    // args is now verified to contain 2 elements, and 2 elements only.
    
    // neither of the two elements in args can be CONS_TYPE
    Value *first_element = car(args);
    Value *second_element = car(cdr(args));
    if ((*first_element).type == CONS_TYPE) {
        evaluationError();
    } else if ((*second_element).type == CONS_TYPE) {
        evaluationError();
    }
    
    // we now know that first and second are not cons types.
    // first and second can not be NULL_TYPE's either.
    if ((*first_element).type == NULL_TYPE) {
        evaluationError();
    } else if ((*second_element).type == NULL_TYPE) {
        evaluationError();
    }
    
    // we now know that first and second argument is not CONS_TYPE and not
    // NULL_TYPE, so they have to be PTR_TYPE or some other type.
    // should be no dots in the thing we return, as those are handled in
    // printing, as shown in the racket documentation. 
    
    Value *new_list;
    // second is PTR_TYPE
    if ((*second_element).type == PTR_TYPE) {
        // since second is PTR we need to get the body of it.
        Value *second_body = (*second_element).p;
        
        // need to check if first is PTR_TYPE or not;
        if ((*first_element).type == PTR_TYPE) {
            // since first is PTR we need to get the body of it.
            Value *first_body = (*first_element).p;
            
            // need to check if second element is '(). If it is we do
            // cons(fist, null);
            Value *car_second = car(second_body);
            if ((*car_second).type == NULL_TYPE) {
                // second element is '().
                new_list = cons(first_body, makeNull());
            } else {
                // second element is not '().
                Value *car_first = car(first_body);
                if ((*car_first).type == NULL_TYPE) {
                    new_list = cons(makeNull(), second_body);
                } else {
                    new_list = cons(first_body, second_body);
                }
            }
        } else {
            Value *first_body = first_element;
            
            // need to check if second element is '(). If it is we do
            // cons(first, null);
            Value *car_second = car(second_body);
            if ((*car_second).type == NULL_TYPE) {
                // second element is '().
                new_list = cons(first_body, makeNull());
            } else {
                // second element is not '().
                new_list = cons(first_body, second_body);
            }
        }
    // second is not PTR_TYPE
    } else {
        // second cannot be '() here.
        Value *second_body = second_element;
        
        // need to check if first is PTR_TYPE or not.
        if ((*first_element).type == PTR_TYPE) {
            // since first is PTR we need to get the body of it.
            Value *first_body = (*first_element).p;
            // this cons type is special as the cdr is not a cons cell,
            // but rather an int, double, etc.
            new_list = cons(first_body, second_body);
        } else {
            Value *first_body = first_element;
            // this cons type is special as the cdr is not a cons cell,
            // but rather an int, double, etc.
            new_list = cons(first_body, second_body);
        }
    }
    
    // now add the new list created to the PTR_TYPE that we return.
    Value *ptr_list = talloc(sizeof(Value));
    (*ptr_list).type = PTR_TYPE;
    (*ptr_list).p = new_list;
    
    return ptr_list;
}

// PrimitiveCdr takes in a list and returns the cdr of the list. The cdr is
// a value struct of type CONS_TYPE.
Value *primitiveCdr(Value *args) {
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != NULL_TYPE) {
        evaluationError();
    }
    // now we now that args is of length 1.
    
    Value *first_element = car(args);
    
    // check to see if it is a PTR_TYPE ~ comes from a 'quote'
    if ((*first_element).type != PTR_TYPE) {
        evaluationError();
    }
    
    // get the actual body of the quote.
    Value *quote_body = (*first_element).p;
    if ((*quote_body).type != CONS_TYPE) {
        evaluationError();
    }
    // (2 3),null
    
    // if empty list '(), we want to throw an error.
    if ((*quote_body).type != CONS_TYPE) {
        evaluationError();
    }
    
    // now we now that the list is not empty. So, we want to take all but
    // the first element and return them in a new PTR_TYPE.
    Value *cdr_quote_body = cdr(quote_body);
    Value *return_value = makeNull();
    
    // was it a 1 element list? If so, we need to make a new valid list.
    if ((*cdr_quote_body).type == NULL_TYPE) {
        return_value = cons(return_value, makeNull());
    } else {
        return_value = cdr_quote_body;
    }
    
    Value *ptr_value = talloc(sizeof(Value));
    (*ptr_value).type = PTR_TYPE;
    (*ptr_value).p = return_value;
    
    return ptr_value;
}

// PrimitiveCar takes in a list and returns the car of the list.
Value *primitiveCar(Value *args) {
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != NULL_TYPE) {
        evaluationError();
    }
    // now we now that args is of length 1.
    
    Value *first_element = car(args);
    
    // check to see if it is a PTR_TYPE ~ comes from a 'quote'
    if ((*first_element).type != PTR_TYPE) {
        evaluationError();
    }
    
    // get the actual body of the quote.
    Value *quote_body = (*first_element).p;
    if ((*quote_body).type != CONS_TYPE) {
        evaluationError();
    }
    
    // if empty list '(), we want to throw an error.
    if ((*car(quote_body)).type == NULL_TYPE) {
        evaluationError();
    }
    
    // now we now that the list is not empty. So, we want to take the
    // first element and return them it in a new PTR_TYPE.
    Value *car_quote_body = car(quote_body);
    Value *return_value = makeNull();
    
    // (1 2),null
    
    // we need to make a new valid list.
    if ((*car_quote_body).type == CONS_TYPE) {
        return_value = car_quote_body;
        Value *ptr_value = talloc(sizeof(Value));
        (*ptr_value).type = PTR_TYPE;
        (*ptr_value).p = return_value;
    
        return ptr_value;
    } else {
        return car_quote_body;
    }
    
    Value *ptr_value = talloc(sizeof(Value));
    (*ptr_value).type = PTR_TYPE;
    (*ptr_value).p = return_value;
    
    return ptr_value;
}

// PrimitiveNullCheck chechs whether an element is NULL and returns #t/#f in
// a Value struct of BOOL_TYPE.
Value *primitiveNullCheck(Value *args) {
    // make sure that we are given one element in the arguments list.
    // no more, no less.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != NULL_TYPE) {
        evaluationError();
    }
    
    Value *return_value = talloc(sizeof(Value));
    (*return_value).type = BOOL_TYPE;
    
    // make sure that the first element is 'quote' by the type of args being
    // PTR_TYPE. (*args).p holds the list itself.
    Value *car_args = car(args);
    if ((*car_args).type != PTR_TYPE) {
        // #f only if int, double, str, or bool. Else error.
        if ((*car_args).type == INT_TYPE) {
            (*return_value).i = 0;
        } else if ((*car_args).type == DOUBLE_TYPE) {
            (*return_value).i = 0;
        } else if ((*car_args).type == STR_TYPE) {
            (*return_value).i = 0;
        } else if ((*car_args).type == BOOL_TYPE) {
            (*return_value).i = 0;
        } else {
            evaluationError();
        }
    } else {
        // get the actual list that was quoted.
        Value *quote_value = (*car_args).p;
        if ((*quote_value).type != CONS_TYPE) {
            // #f
            (*return_value).i = 0;
        } else if ((*car(quote_value)).type == NULL_TYPE) {
            // #f
            (*return_value).i = 1;
        } else {
            // #t
            (*return_value).i = 0;
        }
    }
        
    return return_value;
}

// PrimitiveAdd() takes a list of arguments and adds them all together.
// Returns a DOUBLE_TYPE value struct with the sum.
Value *primitiveAdd(Value *args) {
    double sum = 0;
    Value *args_ptr = args;
    // loop over the arguments and add their value to sum.
    while ((*args_ptr).type != NULL_TYPE) {
        //printTree(args_ptr);
        //printf("\n");
        
        Value *car_args_ptr = car(args_ptr);
        if ((*car_args_ptr).type == INT_TYPE) {
            sum = sum + (double)(*car_args_ptr).i;
        } else if ((*car_args_ptr).type == DOUBLE_TYPE) {
            sum = sum + (*car_args_ptr).d;
        } else {
            evaluationError();
        }
        
        args_ptr = cdr(args_ptr);
    }
    
    // sum now holds the sum of the arguments.
    Value *sum_value = talloc(sizeof(Value));
    (*sum_value).type = DOUBLE_TYPE;
    (*sum_value).d = sum;
    return sum_value;
}

// bind() takes a primitive function and binds it in the top level frame for
// later use.
void bind(char *name, Value *(*function)(struct Value *), Frame *frame) {
    // Add primitive functions to top-level bindings list.
    Value *value = talloc(sizeof(Value));
    (*value).type = PRIMITIVE_TYPE;
    (*value).pf = function;
    
    Value *func_name = talloc(sizeof(Value));
    (*func_name).type = SYMBOL_TYPE;
    (*func_name).s = name;
    
    Value *new_binding = cons(func_name, value);
    
    (*frame).bindings = cons(new_binding, (*frame).bindings);
}

// evalCond() essentially works just like an if-statement, except that 'cond'
// allows for multiple statements after eachother - similar to else-if.
// Can end in an 'else' or not depending on the need of the user.
Value *evalCond(Value *args, Frame *frame) {
    Value *args_ptr = args;
    // loop over all the different cases in the cond.
    while ((*args_ptr).type != NULL_TYPE) {
        Value *clause = car(args_ptr);
        if ((*clause).type != CONS_TYPE) {
            evaluationError();
        }
        
        Value *condition = car(clause);
        Value *body = cdr(clause);
        
        Value *eval_condition;
        // check to see if the condition is 'else'.
        if ((*condition).type == SYMBOL_TYPE) {
            if (!strcmp((*condition).s, "else")) {
                // else has to be the last clause.
                if ((*cdr(args_ptr)).type != NULL_TYPE) {
                    evaluationError();
                }
                
                Value *true_bool = talloc(sizeof(Value));
                (*true_bool).type = BOOL_TYPE;
                (*true_bool).i = 1;
                eval_condition = true_bool;
            } else {
                eval_condition = eval(condition, frame);
            }
        } else {
            eval_condition = eval(condition, frame);
        }
        
        // make sure that the condition evaluates to a boolean.
        if ((*eval_condition).type != BOOL_TYPE) {
            evaluationError();
        }
        
        // if true
        if ((*eval_condition).i == 1) {
            // make sure that a body exists.
            if ((*body).type == NULL_TYPE) {
                evaluationError();
            // make sure that body is only one element.
            } else if ((*body).type == CONS_TYPE) {
                if ((*cdr(body)).type != NULL_TYPE) {
                    evaluationError();
                }
            }
            // now we are sure that the body is of proper format. I.e. one
            // element.
            
            
            return eval(car(body), frame);
        }
        
        args_ptr = cdr(args_ptr);
    }
    
    // if we havent returned already, that means that there were no clauses
    // that evaluated to true. So we return a VOID_TYPE.
    
    Value *void_cell = talloc(sizeof(Value));
    (*void_cell).type = VOID_TYPE;
    return void_cell;
}

// evalLetrec() works more or less just like evalLet(), except for that it
// allows for circular assignments and recursive assignments.
Value *evalLetrec(Value *args, Frame *e) {
    if ((*car(args)).type != CONS_TYPE) {
        evaluationError();
        return NULL;
    } else {
        // let e be a pointer to the current frame. create a new frame
        // f whose parent is e.
        Frame *f = talloc(sizeof(Frame));
        (*f).parent = e;
        (*f).bindings = makeNull();
                 
        // we want to add the variables names to f with an invalid boolean.
        Value *args_ptr = car(args);
        while ((*args_ptr).type != NULL_TYPE) {
            // ((x 5)) --> (x 5)
            Value *pair = car(args_ptr);

            // error checking for it pair is not a pair.
            if ((*pair).type != CONS_TYPE) {
                evaluationError();
            } else if ((*cdr(pair)).type != CONS_TYPE) {
                evaluationError();
            }

            // (x 5) --> x
            Value *variable_name = car(pair);

            // make sure that the variable is a SYMBOL_TYPE.
            if ((*variable_name).type != SYMBOL_TYPE) {
                evaluationError();
            }

            Value *false_value = talloc(sizeof(Value));
            (*false_value).type = BOOL_TYPE;
            // set the number of the bool to an undefined integer so that we
            // know later on how to handle the output.
            (*false_value).i = -1;

            Value *symbol_and_false = cons(variable_name, false_value);

            (*f).bindings = cons(symbol_and_false, (*f).bindings);

        
            args_ptr = cdr(args_ptr);
        }
        // f now contains all the variable names. The variables have the -1
        // boolean associated with them.
        
        // now we want to eval all the proper values.
        Value *evaled_variables = makeNull();
        Value *args_ptr_value = car(args);

        // lastly we want to associate the variables with their proper values.
        // reverse the bindings list as it is now in the opposite order of how
        // the user gave it to us.
        Value *f_ptr = reverse((*f).bindings);
        //Value *value_ptr = evaled_variables;
        while ((*f_ptr).type != NULL_TYPE) {
            Value *bindings_pair = car(f_ptr);
            //Value *correct_value = car(value_ptr);
            
            // the car(cdr(car(xx))) is to access the value itself of the pair.
            Value *correct_value = eval(car(cdr(car(args_ptr_value))), f);
            
            ((*bindings_pair).c).cdr = correct_value;
            
            //value_ptr = cdr(value_ptr);
            args_ptr_value = cdr(args_ptr_value);
            f_ptr = cdr(f_ptr);
        }
        
        // evaluate body using frame f and return the result.
        // cdr(args) --> the list with 'x' in it.
        // car(cdr(args) --> only 'x'.
        Value *result = eval(car(cdr(args)), f);
        if ((*result).type == BOOL_TYPE) {
            if ((*result).i == -1) {
                evaluationError();
            }
        }
        
        return result;
    }
}

// evalSetband() takes a symbol and a value and changes the current value of the 
// symbol to the value given by the user. Will traverse frames until it finds
// symbol and change the value of it in that frame. If the symbol is not
// defined yet an error will be thrown.
Value *evalSetBang(Value *args, Frame *frame) {
    // make sure that args only contains two elements.
    // no more, no less.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(args)).type != CONS_TYPE) {
        evaluationError();
    } else if ((*cdr(cdr(args))).type != NULL_TYPE) {
        evaluationError();
    }
    
    Value *var = car(args);
    Value *new_value = car(cdr(args));
    new_value = eval(new_value, frame);
    
    if ((*var).type != SYMBOL_TYPE) {
        evaluationError();
    }
    
    // loop over the frames to find out whether the value has already been
    // defined or not. If it had been defined already, change the value of it
    // and return a VOID_TYPE. An error is thrown if the value has not been
    // defined yet.
    Frame *frame_ptr = frame;
    while (frame_ptr != NULL) {
        Value *bindings_ptr = (*frame_ptr).bindings;
        
        while ((*bindings_ptr).type != NULL_TYPE) {
            // need to check if the car is the same string.
            // ((x 5) (y 6) (z 7)) --> (x 5)
            Value *car_bindings = car(bindings_ptr);
            // (x 5) --> 'x'.
            Value *symbol_in_frame = car(car_bindings);
            if (strcmp((*var).s, (*symbol_in_frame).s) == 0) {
                // we have now found the symbol we were looking to change the
                // associated value of.
                // set the cdr of the bindings pair to be the new value.
                ((*car_bindings).c).cdr = new_value;
                
                // create a void cell that is to be returned to avoid printing.
                Value *void_cell = talloc(sizeof(Value));
                (*void_cell).type = VOID_TYPE;
                return void_cell;
            }
            
            // go to the next element.
            bindings_ptr = cdr(bindings_ptr);
        }
        
        // go to the next frame.
        frame_ptr = (*frame_ptr).parent;
    }
    // we did not find the symbol we were looking for, so we throw an error.
    evaluationError();
    
    // this will never actually be reached.
    return NULL;
}

// begin() evaluates all the elements in a list before returning the evaluated
// version of the last element.
Value *evalBegin(Value *args, Frame *frame) {
    // return VOID_TYPE if args is empty.
    if ((*args).type == NULL_TYPE) {
        Value *void_cell = talloc(sizeof(Value));
        (*void_cell).type = VOID_TYPE;
        return void_cell;
    // if there are arguments, they need to be formatted in a list.
    } else if ((*args).type != CONS_TYPE) {
        evaluationError();
    }
    
    // loop over all the elements and evaluate them.
    Value *args_ptr = args;
    Value *temp;
    while ((*args_ptr).type != NULL_TYPE) {
        Value *car_args = car(args_ptr);
        temp = eval(car_args, frame);
        
        args_ptr = cdr(args_ptr);
    }
    
    // return the last element.
    return temp;
}

// evalLetStar() works just like evalLet() except for that the assignments can
// depend on each other, as they are evaluated from left to right.
Value *evalLetStar(Value *args, Frame *e) {
    if ((*car(args)).type != CONS_TYPE) {
        evaluationError();
        return NULL;
    } else {
        // let e be a pointer to the current frame. create a new frame
        // f whose parent is e.
        Frame *f = talloc(sizeof(Frame));
        (*f).parent = e;
        (*f).bindings = makeNull();
        
        // for i = 1 through n.
        // // let vali be the result of evaluating expri in frame e
        // // add a binding from vari to vali to f.
        // pointer_args points to the list of lists of args
        Value *pointer_args = car(args);
        while ((*pointer_args).type != NULL_TYPE) {
            Frame *temp = talloc(sizeof(Frame));
            (*temp).parent = f;
            (*temp).bindings = makeNull();
            
            // ((x 5)) --> (x 5)
            Value *vali = car(pointer_args);
            // (x 5) --> 5
            vali = car(cdr(vali));
            vali = eval(vali, f);
            
            Value *symbol_and_value = talloc(sizeof(Value));
            (*symbol_and_value).type = CONS_TYPE;
            // car is now equal to 'x'.
            if ((*car(car(pointer_args))).type != SYMBOL_TYPE) {
                evaluationError();
            }
            
            ((*symbol_and_value).c).car = car(car(pointer_args));
            // cdr is not equal to '5'.
            ((*symbol_and_value).c).cdr = vali;
            
            (*temp).bindings = cons(symbol_and_value, (*temp).bindings);
            f = temp;
            pointer_args = cdr(pointer_args);
        }
        
        // evaluate body using frame f and return the result.
        // cdr(args) --> the list with 'x' in it.
        // car(cdr(args) --> only 'x'.
        Value *result = eval(car(cdr(args)), f);
        return result;
    }
}

// evalAnd() takes a list of arguments and checks whether there is a #f in it.
// if a non-bool type is found it is evaluated and skipped.
Value *evalAnd(Value *args, Frame *frame) {
    // make sure that params are specified.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    }
    
    // loop over args and eval each one as you go. If you see a #f, return
    // #f. If no #f is found, then #t is returned.
        
    Value *result = talloc(sizeof(Value));
    (*result).type = BOOL_TYPE;
    
    // loop over the arguments and check for #f.
    Value *args_ptr = args;
    while ((*args_ptr).type != NULL_TYPE) {
        Value *car_args_ptr = car(args_ptr);
        Value *evaled_car = eval(car_args_ptr, frame);
        // we require booleans.
        if ((*evaled_car).type == BOOL_TYPE) {
            if ((*evaled_car).i == 0) {
                (*result).i = 0;
                return result;
            }
        }
        
        args_ptr = cdr(args_ptr);
    }
    // no #f seen, so we return true.
    (*result).i = 1;
    
    return result;
}

// evalOr() takes a list of arguments and checks to see whether there one of
// the argumnets evaluates to true. If a non bool_type is found that value is
// returned.
Value *evalOr(Value *args, Frame *frame) {
    // make sure that params are specified.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    }
    
    // loop over args and eval each one as you go. If you see a #t, return
    // #t. If no #t is found, then #f is returned.
    
    Value *result = talloc(sizeof(Value));
    (*result).type = BOOL_TYPE;
    
    // loop over the arguments and checks for #t.
    Value *args_ptr = args;
    while ((*args_ptr).type != NULL_TYPE) {
        Value *car_args_ptr = car(args_ptr);
        Value *evaled_car = eval(car_args_ptr, frame);
        // we require booleans.
        if ((*evaled_car).type != BOOL_TYPE) {
            return evaled_car;
        } else {
            // if true, return true.
            if ((*evaled_car).i == 1) {
                (*result).i = 1;
                return result;
            }
        }
        
        args_ptr = cdr(args_ptr);
    }
    // no #t seen, so we return false.
    (*result).i = 0;
    
    return result;
}

// EvalLambda takes the arguments of a lambda expression and a frame.
// Uses this information to evaluate the lambda and return a closure.
Value *evalLambda(Value *args, Frame *frame) {
    // make sure that params are specified.
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    }
    
    // get parameter list and function body from lambda expression.
    Value *params = car(args);
    Value *body = cdr(args);
    if ((*params).type != CONS_TYPE) {
        params = makeNull();
    } else if ((*car(params)).type == NULL_TYPE) {
        params = makeNull();
    }
    
    // make sure that the body exists
    if ((*body).type != CONS_TYPE) {
        evaluationError();
    }
    
    // verify that all parameters are of symbol type.
    Value *param_ptr = params;
    while ((*param_ptr).type == CONS_TYPE) {
        if ((*car(param_ptr)).type != SYMBOL_TYPE) {
            evaluationError();
        }
        
        param_ptr = cdr(param_ptr);
    }
    
    // create the closure that is to be returned.
    Value *new_closure = talloc(sizeof(Value));
    (*new_closure).type = CLOSURE_TYPE;
    ((*new_closure).cl).paramNames = params;
    ((*new_closure).cl).functionCode = body;
    ((*new_closure).cl).frame = frame;
    
    return new_closure;
}

// Apply takes a function (closure or primitive) and a list of arguments.
// Evaluates the function with the given arguments and returns the result.
Value *apply(Value *function, Value *args) {
    // function is a closure
    // args is either an empty list or a list where each element is
    // a symbol

    // make sure that function is a closure or primitive
    if (((*function).type != CLOSURE_TYPE) && ((*function).type != PRIMITIVE_TYPE)) {
        evaluationError();
    }
    
    // deal with primitives here. If not primitive (~ closure) deal with
    // the rest of the code below the if-statement.
    if ((*function).type == PRIMITIVE_TYPE) {
        return (*function).pf(args);
    }
    
    // create new frame with parent being frame stored in the closure.
    Frame *new_frame = talloc(sizeof(Frame));
    (*new_frame).parent = ((*function).cl).frame;
    
    // make sure that the amount of params in the function call is the same
    // as in the lambda.
    int lambda_args_length = length(((*function).cl).paramNames);
    int given_args_length = length(args);
    if (given_args_length != lambda_args_length) {
        evaluationError();
    }
    // map bindings and add to the new frame.
    Value *new_bindings = makeNull();
    Value *args_ptr = args;
    Value *func_param_ptr = ((*function).cl).paramNames;
    while ((*args_ptr).type == CONS_TYPE) {
        // combine the elements from both lists.
        // car(func_param_ptr) holds the symbol.
        // car(args_ptr) holds the value.
        Value *combined = cons(car(func_param_ptr), car(args_ptr));
        new_bindings = cons(combined, new_bindings);
        
        // get the next item in each list.
        func_param_ptr = cdr(func_param_ptr);
        args_ptr = cdr(args_ptr);
    }
    
    Value *body = ((*function).cl).functionCode;
    if ((*cdr(body)).type == NULL_TYPE) {
        body = car(body);
    }
    (*new_frame).bindings = new_bindings;
    Value *function_evaled = eval(body, new_frame);    
    return function_evaled;
}

// Eval each takes a list of arguments and evaluates each of these
// values. This is done by the means of a loop. Returns the a list
// with the result from evaluating each one of the values.
Value *evalEach(Value *args, Frame *frame) {
    Value *evaled_list = makeNull();
    
    // loop over the args and eval all of them.
    Value *args_ptr = args;
    while ((*args_ptr).type != NULL_TYPE) {
        Value *checked_var = eval(car(args_ptr), frame);
        
        evaled_list = cons(checked_var, evaled_list);
        
        args_ptr = cdr(args_ptr);
    }
    // empty argument list.
    if ((*evaled_list).type == NULL_TYPE) {
        return evaled_list;
    }
    
    evaled_list = reverse(evaled_list);
    
    return evaled_list;
}

// EvalDefine takes a list of arguments and a frame and evaluates the
// arguments given the frame. The evaluated result is stored in the
// frame. The function returns a void_cell to avoid priting anything.
Value *evalDefine(Value *args, Frame *frame) {
    if ((*args).type != CONS_TYPE) {
        evaluationError();
    } 
    
    Value *var = car(args);
    Value *expr = cdr(args);
    
    // make sure input is valid.
    Value *binding = talloc(sizeof(Value));
    (*binding).type = CONS_TYPE;
    if ((*var).type != SYMBOL_TYPE) {
        evaluationError();
    } else if ((*expr).type != CONS_TYPE) {
        evaluationError();
    }
    // create the new binding.
    ((*binding).c).car = var;
    ((*binding).c).cdr = eval(car(expr), frame);
    (*frame).bindings = cons(binding, (*frame).bindings);
    
    // create a void cell that is to be returned to avoid printing.
    Value *void_cell = talloc(sizeof(Value));
    (*void_cell).type = VOID_TYPE;
    return void_cell;
}

// Method evaluates the arguments given for an if statement
// and returns the appropriate value.
Value *evalIf(Value *args, Frame *frame) {
    Value *condition = eval(car(args), frame);
    // should not allow for argument list to be less than 3 long.
    if (length(args) < 3) {
        evaluationError();
    // make sure the condition evaluates to either true or false.
    } else if (((*condition).i != 0) && ((*condition).i != 1)) {
        evaluationError();
    }
    
    // if true
    if ((*condition).i == 1) {
        return eval(car(cdr(args)), frame);
    // if false
    } else {
        Value *cdr_args = cdr(args);
        Value *cdr_cdr_args = cdr(cdr_args);
        return eval(car(cdr_cdr_args), frame);
    }
}

// Method evaluates the arguments given for a let statement
// and returns the appropriate value.
Value *evalLet(Value *args, Frame *e) {  
    if ((*car(args)).type != CONS_TYPE) {
        evaluationError();
        return NULL;
    } else {
        // let e be a pointer to the current frame. create a new frame
        // f whose parent is e.
        Frame *f = talloc(sizeof(Frame));
        (*f).parent = e;
        (*f).bindings = makeNull();
        
        // for i = 1 through n.
        // // let vali be the result of evaluating expri in frame e
        // // add a binding from vari to vali to f.
        // pointer_args points to the list of lists of args
        Value *pointer_args = car(args);
        while ((*pointer_args).type != NULL_TYPE) {
            // ((x 5)) --> (x 5)
            Value *vali = car(pointer_args);
            // (x 5) --> 5
            vali = car(cdr(vali));
            vali = eval(vali, e);
            
            Value *symbol_and_value = talloc(sizeof(Value));
            (*symbol_and_value).type = CONS_TYPE;
            // car is now equal to 'x'.
            if ((*car(car(pointer_args))).type != SYMBOL_TYPE) {
                evaluationError();
            }
            
            ((*symbol_and_value).c).car = car(car(pointer_args));
            // cdr is now equal to '5'.
            ((*symbol_and_value).c).cdr = vali;
            
            (*f).bindings = cons(symbol_and_value, (*f).bindings);
                
            pointer_args = cdr(pointer_args);
        }
        
        // evaluate body using frame f and return the result.
        // cdr(args) --> the list with 'x' in it.
        // car(cdr(args) --> only 'x'.
        Value *result = eval(car(cdr(args)), f);
        return result;
    }   
}

// Method looks up a symbol and returns the value of it.
Value *lookUpSymbol(Value *symbol, Frame *frame) {
    if ((*frame).parent != NULL) {
        // bindings is a list of lists, where car is the symbol
        // and the cdr is the value for each item in bindings.
        //  bindings ---> ((x 5) (y 6) (z 7))
        Value *bindings_ptr = (*frame).bindings;
        while ((*bindings_ptr).type != NULL_TYPE) {
            // ((x 5) (y 6) (z 7)) --> (x 5)
            Value *car_bindings = car(bindings_ptr);
            // (x 5) --> 'x'.
            Value *symbol_in_frame = car(car_bindings);
            if (strcmp((*symbol).s, (*symbol_in_frame).s) == 0) {
                Value *cdr_car_bindings = cdr(car_bindings);
                return cdr_car_bindings;
            }
            bindings_ptr = cdr(bindings_ptr);
        }
        // look up the symbol in the parent frame.
        return lookUpSymbol(symbol, (*frame).parent);
    } else {
        // check the global frame
        //  bindings ---> ((x 5) (y 6) (z 7))
        Value *bindings_ptr = (*frame).bindings;
        while ((*bindings_ptr).type != NULL_TYPE) {
            // ((x 5) (y 6) (z 7)) --> (x 5)
            Value *car_bindings = car(bindings_ptr);
            // (x 5) --> 'x'.
            Value *symbol_in_frame = car(car_bindings);
            if (strcmp((*symbol).s, (*symbol_in_frame).s) == 0) {
                return cdr(car_bindings);
                //return symbol;
            }
            bindings_ptr = cdr(bindings_ptr);
        }
        evaluationError();
        return NULL;
    }
}

// Method interprets a parse tree, which results in the proper
// output being printed out to the screen.
void interpret(Value *tree) {
    Value *tree_pointer = tree;
    Frame *frame_pointer = talloc(sizeof(Frame));
    Value *bindings_list = makeNull();
    (*frame_pointer).bindings = bindings_list;
    (*frame_pointer).parent = NULL;
    
    bind("+", primitiveAdd, frame_pointer);
    bind("car", primitiveCar, frame_pointer);
    bind("cdr", primitiveCdr, frame_pointer);
    bind("null?", primitiveNullCheck, frame_pointer);
    bind("cons", primitiveCons, frame_pointer);
    bind("-", primitiveSubtraction, frame_pointer);
    bind("*", primitiveMultiplication, frame_pointer);
    bind("modulo", primitiveModulo, frame_pointer);
    bind("<", primitiveLessThan, frame_pointer);
    bind(">", primitiveGreaterThan, frame_pointer);
    bind("=", primitiveEqual, frame_pointer);
    bind("/", primitiveDivision, frame_pointer);
    
    // loop over the tree and evaluate each part.
    while ((*tree_pointer).type != NULL_TYPE) {
        // evaluate the the expressions given the frame.
        Value *returned_value = eval(car(tree_pointer), frame_pointer);
        
        if ((*returned_value).type == PTR_TYPE) {
            returned_value = (*returned_value).p;
        }
        
        // depeding on what type the result is we'll print out the result.
        switch ((*returned_value).type) {
            case INT_TYPE:
                printf("%i\n", (*returned_value).i);
                break;
            case DOUBLE_TYPE:
                printf("%f\n", (*returned_value).d);
                break;
            case STR_TYPE:
                printf("\"%s\"\n", (*returned_value).s);
                break;
            case SYMBOL_TYPE:
                printf("%s\n", (*returned_value).s);
                break;
            case BOOL_TYPE:
                if ((*returned_value).i == 0) {
                    printf("#f\n");
                } else if ((*returned_value).i == 1) {
                    printf("#t\n");
                }
                break;
            case PTR_TYPE:
                returned_value = (*returned_value).p;
                printTreeEdit(returned_value);
                //evaluationError();
                break;
            case OPEN_TYPE:
                evaluationError();
                break;
            case CLOSE_TYPE:
                evaluationError();
                break;
            case CONS_TYPE:
                printTreeEdit(returned_value);
                printf("\n");
                //evaluationError();
                break;
            case NULL_TYPE:
                evaluationError();
                break;
            case VOID_TYPE:
                break;
            case CLOSURE_TYPE:
                printf("#<procedure>\n");
                break;
            case PRIMITIVE_TYPE:
                evaluationError();
                break;
        }
        tree_pointer = cdr(tree_pointer);
    }
}

// Method takes an expression and a frame and evaluates the expressions
// accordingly.
Value *eval(Value *expr, Frame *frame) {    
    Value *result = talloc(sizeof(Value));
    Value *first;
    Value *args;
    // evaluation and how to go about it depends on the expression.
    // for symbols we need to look up the symbol in the frame and find
    // the value.
    switch ((*expr).type) {
        case INT_TYPE:
            result = expr;
            break;
        case DOUBLE_TYPE:
            result = expr;
            break;
        case BOOL_TYPE:
            result = expr;
            break;
        case STR_TYPE:
            result = expr;
            break;
        case SYMBOL_TYPE:
            return lookUpSymbol(expr, frame);
            break;
        case OPEN_TYPE:
            evaluationError();
            break;
        case CLOSE_TYPE:
            evaluationError();
            break;
        case NULL_TYPE:
            evaluationError();
            break;
        case PTR_TYPE:
            evaluationError();
            break;
        case VOID_TYPE:
            evaluationError();
            break;
        case CLOSURE_TYPE:
            evaluationError();
            break;
        case PRIMITIVE_TYPE:
            //result = (*expr).pf;
            evaluationError();
            break;
        case CONS_TYPE:
            //printTree(expr);
            //printf("\n");
            
            first = car(expr);
            args = cdr(expr);
            
            // Sanity and error checking on first....
            if (first == NULL) {
                evaluationError();
            }
            
            // call different functions depending on what string is seen.
            if (!strcmp((*first).s, "if")) {
                result = evalIf(args, frame);
            } else if (!strcmp((*first).s, "let")) {
                result = evalLet(args, frame);
            } else if (!strcmp((*first).s, "quote")) {                
                if ((*args).type != CONS_TYPE) {
                    evaluationError();
                } else if ((*car(args)).type == NULL_TYPE) {
                    args = args;
                } else if ((*car(args)).type != CONS_TYPE) {
                    args = car(args);
                } else {
                    args = car(args);
                }
                
                (*result).p = args;
                (*result).type = PTR_TYPE;
            } else if (!strcmp((*first).s, "define")) {
                result = evalDefine(args, frame);
            } else if (!strcmp((*first).s, "lambda")) {
                result = evalLambda(args, frame);
            } else if (!strcmp((*first).s, "and")) {
                result = evalAnd(args, frame);
            } else if (!strcmp((*first).s, "or")) {
                result = evalOr(args, frame);
            } else if (!strcmp((*first).s, "let*")) {
                result = evalLetStar(args, frame);
            } else if (!strcmp((*first).s, "set!")) {
                result = evalSetBang(args, frame);
            } else if (!strcmp((*first).s, "letrec")) {
                result = evalLetrec(args, frame);
            } else if (!strcmp((*first).s, "cond")) {
                result = evalCond(args, frame);
            } else if (!strcmp((*first).s, "begin")) {
                result = evalBegin(args, frame);
            }
                
            // ... other special forms here ....
            
            else {
                // If not a special form, evaluate the first, evaluate the
                // args, then apply the first to the args.
                Value *evaledOperator = eval(first, frame);
                Value *evaledArgs = evalEach(args, frame);
                return apply(evaledOperator, evaledArgs); 
            }
            break;
    }
    return result;
}