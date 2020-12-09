#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * int main() {
    calc_expr(&string_convert, &result_as_string);
    return 0;
}
 */

long long split(char *expr, int start, int end);

void call_split(char *expr, int len);

long long convert_leaf(char* expr, int start, int end);

long long convert_non_para(char* expr, int start, int i);

int determine_operator(char op);

long long string_convert(char *num) {
    return strtol(num, NULL, 10);
}

int main() {
    // in the assembler function we will read the string length from STDIN, and send the full length to the first call to split function
    call_split("(1)", 3);
    call_split("(-1)", 4);
    call_split("(1--1)", 5);
    call_split("(1-(-1))", 8);
    call_split("((2+3)*2)", 9);
    call_split("((1*2)/(3-5))", 13);
    call_split("(1+(1+(1+(1+1))))", 17);
    call_split("((((1+1)+1)+1)+1)", 17);
    call_split("(((1*2)+(3*4))*(5+6))", 21);
    return 0;
}

void call_split(char *expr, int len) {
    long long result = split(expr, 0, len);
    printf("calculation result for string %s is: %lld\n", expr, result);
}

long long split(char *expr, int start, int end) {
    int numOpen = 0;
    int numClose = 0;
    int i = start;
    if (expr[start] == '(') {
        // this is not a leaf
        numOpen++;
        i++;
    } else {
        return convert_leaf(expr, start, end);
    }
    // if we reach a leaf node, this is a number that4 needs to be converted from string form
    // we should perform return string_convert(expr);
    if (expr[i] == '(' && expr[i + 1] == '-') {
        numOpen++;
        i += 2;
    }
    if (expr[i] == '-') {
        // this is a node with a negative number at it's start
        i++;
    }
    while (i < end) {
        if (determine_operator(expr[i]) != 4 && numOpen - numClose == 1) {
            break;
        }
        if (expr[i] == '(') {
            numOpen++;
        }
        if (expr[i] == ')') {
            numClose++;
        }
        i++;
    }

//    if (i == end) {
//        return convert_non_para(expr, start, i);
//    }
    // if we reached this point, it means we met a character that divides the arithmetic expression (could also be ')')
    int op = determine_operator(expr[i]);

    if (op == 0) {
        return split(expr, start + 1, i - 1) + split(expr, i + 1, end - 1);
    } else if (op == 1) {
        return split(expr, start + 1, i - 1) - split(expr, i + 1, end - 1);
    } else if (op == 2) {
        return split(expr, start + 1, i - 1) * split(expr, i + 1, end - 1);
    } else if (op == 3) {
        return split(expr, start + 1, i - 1) / split(expr, i + 1, end - 1);
    } else {
        // we encountered a closing parentheses, meaning we reached the end of this node
        return split(expr, start + 1, end - 1);
    }
}

int determine_operator(char op) {
    if (op == '+') {
        return 0;
    } else if (op == '-') {
        return 1;
    } else if (op == '*') {
        return 2;
    } else if (op == '/') {
        return 3;
    } else {
        // this should be a closing parentheses
        return 4;
    }
}

long long convert_leaf(char *expr, int start, int end) {
    int len = end - start + 2;
    char *num = malloc(len * sizeof(char));
    int j = start;
    for (int pos = 0; pos < len - 1; pos++) {
        num[pos] = expr[j];
        j++;
    }
    num[len - 1] = 0;
    return string_convert(num);
}

long long convert_non_para(char *expr, int start, int i) {
    char *num = malloc((i - 1) * sizeof(char));
    int pos = 0;
    for (int j = start + 1; j < (i - 1); j++) {
        num[pos] = expr[j];
        pos++;
    }
    num[i - 2] = 0;
    return string_convert(num);
}

/**
calc_expression will receive 2 pointers to functions (one is string_convert and the other is result_as_string)
calc_expression should read from STDIN until it receives a null terminator character
 after that it should try to split the arithmetic expression into chunks (denoted by OPERATOR (+,-,*,/) or parentheses'(' ')')
 after splitting into chunks, calc_expression should send every numeric expression into the function string_convert, and invoke the result with the
 operator found next to it, it should do so recursively from the innermost parentheses all the way to the outermost, and in the end call result_as_string
 with the result of the calculation, and print it to stdout.

 PART 1:
 split into numeric chunks (1+1) -> 1 + 1, (1+(2*3)) -> 1 + 2*3 -> 1 + 6
 PART 2:
 determine which mathematical operator needs to be used between every separated numeric chunks (figure out A EXP B, is A + B/A - B)
 PART 3:
 send the numerics to string_convert before every calculation
 PART 4:
 perform the arithmetic calculation based on parts 1,2 and 3
 PART 5:
 send the complete result to result_as_string
 PART 6:
 print to stdout based on the return value of result_as_string

 EDGE CASES:
 received an empty string (without even parentheses)
 received only a number as input
 received only a negative number as input
*/