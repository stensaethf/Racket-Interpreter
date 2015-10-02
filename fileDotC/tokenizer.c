/* tokenizer.c
 * A program that takes a file with racket code and tokenizes it.
 * Sabastian Mugazambi, Josh Pitkofsky, Frederik Roenn Stensaeth.
 * 05.06.2015
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "linkedlist.h"
#include "talloc.h"

// Read all of the input from stdin, and return a linked list consisting of
// the tokens.
Value *tokenize() {
    char charRead;
    Value *list = makeNull();
    charRead = fgetc(stdin);
    
    while (charRead != EOF) {
        
        // open
        if (charRead == '(') {
            // add OPEN_TYPE to the list.
            Value *new_token = talloc(sizeof(Value));
            (*new_token).type = OPEN_TYPE;
            list = cons(new_token, list);
            
            charRead = fgetc(stdin);
        }
        
        // close
        else if (charRead == ')') {
            // add CLOSE_TYPE to the list.
            Value *new_token = talloc(sizeof(Value));
            (*new_token).type = CLOSE_TYPE;
            list = cons(new_token, list);
            
            charRead = fgetc(stdin);
        }
                 
        // comments
        else if (charRead == ';') {
            // loop over the comments.
            while (charRead != '\n' && charRead != EOF) {
                charRead = fgetc(stdin);
            }
        }
        
        // bools
        else if (charRead == '#') {
            charRead = fgetc(stdin);
            
            // make sure that the char after the '#' is valid.
            if (charRead == 'f' || charRead == 't' || charRead == 'F' || 
                charRead == 'T') {
                // Add the appropriate bool to the list.
                Value *new_token = talloc(sizeof(Value));
                (*new_token).type = BOOL_TYPE;
                if (charRead == 'f' || charRead == 'F') {
                    (*new_token).i = 0;
                } else {
                    (*new_token).i = 1;
                }
                list = cons(new_token, list);
            } else {
                printf("Syntax error: invalid bool.\n");
                texit(1);
            }
            charRead = fgetc(stdin);
        }
        
        // ints / floats (no sign)
        else if (charRead >= '0' && charRead <= '9') {
            char *number_string = (char *)talloc(sizeof(char)*300);
            int i;
                for (i = 0; i < 300; i++) {
                    *(number_string+i) = '0';
                }

            int count = 0;
            // period_check = 0, means that no '.' has been seen.
            // period_check = 1, means that '.' has been seen.
            int period_check = 0;
            while (charRead != ' ' && charRead != ')') {                
                // if digit, add it to the string.
                if (charRead >= '0' && charRead <= '9') {
                    *(number_string+count) = charRead;
                // if . add it to the list if '.' has not been seen before.
                // if '.' seen before, cast an error.
                } else if (charRead == '.') {
                    if (period_check == 0) {
                        *(number_string+count) = '.';
                        period_check = 1;
                    } else {
                        printf("Invalid syntax: invalid float with multiple '.'\n");
                        texit(1);
                    }
                // if not digit, not '.', not space, and not ) cast an error.
                } else {
                    printf("Invalid syntax: invalid float.\n");
                    texit(1);
                }
                count = count + 1;
                charRead = fgetc(stdin);
            }
            // add string terminator.
            *(number_string+count) = '\0';
            
            Value *new_token = talloc(sizeof(Value));
            // token is an integer.
            if (period_check == 0) {
                (*new_token).type = INT_TYPE;
                (*new_token).i = atoi(number_string);
            // token is a float.
            } else {
                (*new_token).type = DOUBLE_TYPE;
                (*new_token).d = atof(number_string);
            }
            
            list = cons(new_token, list);
        }
        
        // ints / floats / addition (positive)
        else if (charRead == '+') {
            charRead = fgetc(stdin);
            
            // character after + is a space
            if (charRead == ' ') {
                char *symbol_string = (char *)talloc(sizeof(char) * 2);
                *(symbol_string+0) = '+';
                *(symbol_string+1) = '\0';
                Value *new_token = talloc(sizeof(Value));
                (*new_token).type = SYMBOL_TYPE;
                (*new_token).s = symbol_string;
                
                list = cons(new_token, list);
            // character after + is a digit
            } else if (charRead > '0' && charRead < '9') {
                char *number_string = (char *)talloc(sizeof(char)*300);
                int i;
                for (i = 0; i < 300; i++) {
                    *(number_string+i) = '0';
                }

                int count = 0;
                // period_check = 0, means that no '.' has been seen.
                // period_check = 1, means that '.' has been seen.
                int period_check = 0;
                while (charRead != ' ' && charRead != ')') {                
                    // if digit, add it to the string.
                    if (charRead >= '0' && charRead <= '9') {
                        *(number_string+count) = charRead;
                    // if . add it to the list if '.' has not been seen before.
                    // if '.' seen before, cast an error.
                    } else if (charRead == '.') {
                        if (period_check == 0) {
                            *(number_string+count) = '.';
                            period_check = 1;
                        } else {
                            printf("Invalid syntax: invalid float with multiple '.'\n");
                            texit(1);
                        }
                    // if not digit, not '.', not space, and not ) cast an error.
                    } else {
                        printf("Invalid syntax: invalid float.\n");
                        texit(1);
                    }
                    count = count + 1;
                    charRead = fgetc(stdin);
                }
                // add string terminator.
                *(number_string+count) = '\0';

                Value *new_token = talloc(sizeof(Value));
                // token is an integer.
                if (period_check == 0) {
                    (*new_token).type = INT_TYPE;
                    (*new_token).i = atoi(number_string);
                // token is a float.
                } else {
                    (*new_token).type = DOUBLE_TYPE;
                    (*new_token).d = atof(number_string);
                }

                list = cons(new_token, list);
            // character after + is a '.'
            } else if (charRead == '.') {
                char *float_string = (char *)talloc(sizeof(char)*300);
                int i;
                for (i = 0; i < 300; i++) {
                    *(float_string+i) = '0';
                }
                
                *(float_string+1) = '0';
                *(float_string+2) = '.';

                charRead = fgetc(stdin);

                // invalid character after '.'.
                if (charRead < '0' || charRead > '9') {
                    printf("Syntax error: invalid character after '+.'.\n");
                    texit(1);
                }

                int count = 2;
                while (charRead >= '0' && charRead <= '9') {
                    *(float_string+count) = charRead;
                    count = count + 1;
                    charRead = fgetc(stdin);
                }

                // add string terminator to string.
                *(float_string+count) = '\0';

                if (charRead != ' ' && charRead != ')') {
                    printf("Syntax error: invalid character after float.\n");
                    texit(1);
                }

                // create new value to hold the float.
                Value *new_token = talloc(sizeof(Value));
                (*new_token).type = DOUBLE_TYPE;
                // convert the string to float using atof and then store
                // it in the double slot in the value created.
                (*new_token).d = atof(float_string);
                // add token to the list.
                list = cons(new_token, list);
            }
            // character after + is not a number or space
            else {
                printf("Syntax error: invalid character after '+'.\n");
                texit(1);
            }
        }
        
        // ints / float / subtraction (negative)
        else if (charRead == '-') {
            charRead = fgetc(stdin);
            
            // character after - is a space
            if (charRead == ' ') {
                char *symbol_string = (char *)talloc(sizeof(char) * 2);
                *(symbol_string+0) = '-';
                *(symbol_string+1) = '\0';
                Value *new_token = talloc(sizeof(Value));
                (*new_token).type = SYMBOL_TYPE;
                (*new_token).s = symbol_string;
                
                list = cons(new_token, list);
            // character after - is a digit
            } else if (charRead > '0' && charRead < '9') {    
                char *number_string = (char *)talloc(sizeof(char)*300);
                int i;
                for (i = 0; i < 300; i++) {
                    *(number_string+i) = '0';
                }
                *(number_string+0) = '-';

                int count = 1;
                // period_check = 0, means that no '.' has been seen.
                // period_check = 1, means that '.' has been seen.
                int period_check = 0;
                while (charRead != ' ' && charRead != ')') {                
                    // if digit, add it to the string.
                    if (charRead >= '0' && charRead <= '9') {
                        *(number_string+count) = charRead;
                    // if . add it to the list if '.' has not been seen before.
                    // if '.' seen before, cast an error.
                    } else if (charRead == '.') {
                        if (period_check == 0) {
                            *(number_string+count) = '.';
                            period_check = 1;
                        } else {
                            printf("Invalid syntax: invalid float with multiple '.'\n");
                            texit(1);
                        }
                    // if not digit, not '.', not space, and not ) cast an error.
                    } else {
                        printf("Invalid syntax: invalid float.\n");
                        texit(1);
                    }
                    count = count + 1;
                    charRead = fgetc(stdin);
                }
                // add string terminator.
                *(number_string+count) = '\0';

                Value *new_token = talloc(sizeof(Value));
                // token is an integer.
                if (period_check == 0) {
                    (*new_token).type = INT_TYPE;
                    (*new_token).i = atoi(number_string);
                // token is a float.
                } else {
                    (*new_token).type = DOUBLE_TYPE;
                    (*new_token).d = atof(number_string);
                }

                list = cons(new_token, list);
            // character after - is a '.'
            } else if (charRead == '.') {
                char *float_string = (char *)talloc(sizeof(char) * 300);
                int i;
                for (i = 0; i < 300; i++) {
                    *(float_string+i) = '0';
                }
                *(float_string+0) = '-';
                *(float_string+1) = '0';
                *(float_string+2) = '.';

                charRead = fgetc(stdin);

                // invalid character after '.'.
                if (charRead < '0' || charRead > '9') {
                    printf("Syntax error: invalid character after '-.'.\n");
                    texit(1);
                }

                int count = 3;
                while (charRead >= '0' && charRead <= '9') {
                    *(float_string+count) = charRead;
                    count = count + 1;
                    charRead = fgetc(stdin);
                }

                // add string terminator to string.
                *(float_string+count) = '\0';

                if (charRead != ' ' && charRead != ')') {
                    printf("Syntax error: invalid character after float.\n");
                    texit(1);
                }

                // create new value to hold the float.
                Value *new_token = talloc(sizeof(Value));
                (*new_token).type = DOUBLE_TYPE;
                // convert the string to float using atof and then store
                // it in the double slot in the value created.
                (*new_token).d = atof(float_string);
                // add token to the list.
                list = cons(new_token, list);
            // character after - is not a number or space
            } else {
                printf("Syntax error: invalid character after '-'.\n");
                texit(1);
            }
        }
                 
        // floats
        else if (charRead == '.') {
            char *float_string = (char *)talloc(sizeof(char) * 300);
            int i;
                for (i = 0; i < 300; i++) {
                    *(float_string+i) = '0';
                }
            *(float_string+0) = '0';
            *(float_string+1) = '.';
            
            charRead = fgetc(stdin);
            
            // invalid character after '.'.
            if (charRead < '0' || charRead > '9') {
                printf("Syntax error: invalid character after '.'.\n");
                texit(1);
            }
            
            int count = 2;
            while (charRead >= '0' && charRead <= '9') {
                *(float_string+count) = charRead;
                count = count + 1;
                charRead = fgetc(stdin);
            }
            
            // add string terminator to string.
            *(float_string+count) = '\0';
            
            if (charRead != ' ' && charRead != ')') {
                printf("Syntax error: invalid character after float.\n");
                texit(1);
            }
            
            // create new value to hold the float.
            Value *new_token = talloc(sizeof(Value));
            (*new_token).type = DOUBLE_TYPE;
            // convert the string to float using atof and then store
            // it in the double slot in the value created.
            (*new_token).d = atof(float_string);
            // add token to the list.
            list = cons(new_token, list);
        }
                 
        // strings -- look out for escaped quotes
        else if (charRead == '"') {
            char *string_token = (char *)talloc(sizeof(char)*300);
            *(string_token+0) = '"';
            
            int count = 0;
            // escape_check = 0, means that the previous char was not \.
            // escape_check = 1, means that the previous char was \.
            // escape_check = 2, means that string is complete.
            int escape_check = 0;
            while (escape_check != 2) {
                count = count + 1;
                charRead = fgetc(stdin);
                
                if (charRead == EOF) {
                    printf("Syntax error: Untokenizable. unterminated string.\n");
                    texit(1);
                }
                
                // if char is " we need to check if it was escaped or not.
                if (charRead == '"') {
                    // was the previous character a \? No = 0.
                    if (escape_check == 0) {
                        escape_check = 2;
                        *(string_token+count) = '"';
                        // add the string terminator.
                        count = count + 1;
                        *(string_token+count) = '\0';
                    } else {
                        escape_check = 0;
                        *(string_token+count) = '"';
                    }
                // if char is \ we need to update string_check
                } else if (charRead == '\\') {
                    if (escape_check == 0) {
                        escape_check = 1;
                        *(string_token+count) = '\\';
                    } else {
                        escape_check = 0;
                        *(string_token+count) = '\\';
                    }
                // char is any other character, so add it to the string.
                } else {
                    escape_check = 0;
                    *(string_token+count) = charRead;
                }
            }
            
            Value *new_token = talloc(sizeof(Value));
            (*new_token).type = STR_TYPE;
            (*new_token).s = string_token;
            
            list = cons(new_token, list);
            
            charRead = fgetc(stdin);
        }
        
        // whitespace
        else if (charRead == ' ') {
            // do nothing
            charRead = fgetc(stdin);
        }
                 
        // newline
        else if (charRead == '\n') {
            // do nothing
            charRead = fgetc(stdin);
        }
                 
        else if (charRead == '\t') {
            // do nothing
            charRead = fgetc(stdin);
        }
                 
        
        // symbols. make sure the symbol is valid.
        else if ((charRead >= 'a' && charRead <= 'z') || (charRead >= 'A'
                    && charRead <= 'Z') || charRead == '!' || charRead == '$'
                 || charRead == '%' || charRead == '&' || charRead == '*' || 
                 charRead == '/' || charRead == ':' || charRead == '<' || 
                 charRead == '>' || charRead == '=' || charRead == '?' || 
                 charRead == '~' || charRead == '_' || charRead == '^') {
            char *symbol_string = (char *)talloc(sizeof(char)*300);
            *(symbol_string+0) = charRead;
            charRead = fgetc(stdin);
            
            int count = 1;
            // while the char is a legal one, keep adding it to the string and
            // check the next one.
            while ((charRead >= 'a' && charRead <= 'z') || (charRead >= 'A' && 
                    charRead <= 'Z') || charRead == '!' || charRead == '$' || 
                   charRead == '%' || charRead == '&' || charRead == '*' || 
                   charRead == '/' || charRead == ':' || charRead == '<' ||
                   charRead == '>' || charRead == '=' || charRead == '?' || 
                   charRead == '~' || charRead == '_' || charRead == '^' || 
                   (charRead >= '0' && charRead <= '9') || charRead == '.' || 
                   charRead == '+' || charRead == '-') {
                *(symbol_string+count) = charRead;
                count = count + 1;
                charRead = fgetc(stdin);
            }
            // add string terminator.
            *(symbol_string+count) = '\0';
            
            // make sure the next char is valid.
            if (charRead != ' ' && charRead != '(' && charRead != ')') {
                printf("Syntax error: illegal symbol.\n");
                texit(1);
            }
            
            // add the symbol to the list.
            Value *new_token = talloc(sizeof(Value));
            (*new_token).type = SYMBOL_TYPE;
            (*new_token).s = symbol_string;
            
            list = cons(new_token, list);
        }
        
        // if char was not covered till now, it is illegal.
        else {
            printf("Syntax error: illegal character.\n");
            texit(1);
        }          
    }
    
    Value *revList = reverse(list);
    return revList;
}

                  
// Displays the contents of the linked list as tokens, with type information
void displayTokens(Value *list) {
    Value *list_pointer = list;
    // loop until the end of the list.
    while ((*list_pointer).type != NULL_TYPE) {
        // create string with newline char in it.
        char *newline_string = (char *)talloc(sizeof(char)*2);
        *(newline_string+0) = '\n';
        *(newline_string+1) = '\0';
        // if the end of the list is reached, do not add newline.
        if ((*((*list_pointer).c).cdr).type == NULL_TYPE) {
            *(newline_string+0) = '\0';
        }
        
        // check what type the car is and print accordingly.
        if ((*((*list_pointer).c).car).type == BOOL_TYPE) {
            if ((*((*list_pointer).c).car).i == 0) {
                printf("#f:bool%s", newline_string);
            } else {
                printf("#t:bool%s", newline_string);
            }
        } else if ((*((*list_pointer).c).car).type == INT_TYPE) {
            printf("%i:integer%s", (*((*list_pointer).c).car).i, 
                                    newline_string);
        } else if ((*((*list_pointer).c).car).type == DOUBLE_TYPE) {
            printf("%f:float%s", (*((*list_pointer).c).car).d, 
                                  newline_string);
        } else if ((*((*list_pointer).c).car).type == STR_TYPE) {
            printf("%s:string%s", (*((*list_pointer).c).car).s, 
                                newline_string);
        } else if ((*((*list_pointer).c).car).type == OPEN_TYPE) {
            printf("(:open%s", newline_string);
        } else if ((*((*list_pointer).c).car).type == CLOSE_TYPE) {
            printf("):close%s", newline_string);
        } else if ((*((*list_pointer).c).car).type == SYMBOL_TYPE) {
            printf("%s:symbol%s", (*((*list_pointer).c).car).s, 
                                   newline_string);
        }
        
        // move on to the next item in the list.
        list_pointer = ((*list_pointer).c).cdr;
    }
}