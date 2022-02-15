#ifndef MATHOPS_H
#define MATHOPS_H 1

#define ISDECIMAL(chr) (chr >= ZERO && chr <= NINE)

enum {
    ADDITION        = '+',
    SUBTRACTION     = '-',
    MULTIPLICATION  = '*',
    DIVISION        = '/',
    MODULO          = '%',
    EQUALTO         = '=',
    NOT             = '!',
    LESSER          = '<',
    GREATER         = '>'
} OPERATION;

/* decimal definition */
enum {
    ZERO            = '0', 
    ONE             = '1', 
    TWO             = '2',
    THREE           = '3', 
    FOUR            = '4', 
    FIVE            = '5',
    SIX             = '6', 
    SEVEN           = '7', 
    EIGHT           = '8',
    NINE            = '9'
} DECIMAL;

#endif
