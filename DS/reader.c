#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "reader.h"

/**************************************************************************************************/
static const int linefeed = 0x0A;
static const int carriage_return = 0x0D;
static const int space = 0x20;
static const int zero = 0x30;
static const int nine = 0x39;
static const int exponent = 0x45;
static const int Exponent = 0x65;
static const int plus = 0x2B;
static const int comma = 0x2C;
static const int minus = 0x2D;
static const int dot = 0x2E;

/**************************************************************************************************/
static inline int char_space(int ch) {
    return isspace(ch);
}

static inline int char_end_of_line(int ch) {
    return (ch == linefeed) || (ch == carriage_return);
}

static inline int char_blank(int ch) {
    return (char_space(ch) && !char_end_of_line(ch));
}

static inline int read_char(FILE* src) {
    return fgetc(src);
}

static inline int pushback_char(int ch, FILE* src) {
    return ungetc(ch, src);
}

/************************************************************************************************/
unsigned long long int read_natural(FILE* src, int* size) {
    unsigned long long n = 0;
    int count = 0;
    int ch;

    discard_space(src);

    while ((ch = read_char(src)) != EOF) {
        if ((ch < zero) || (ch > nine)) {
            pushback_char(ch, src);
            break;
        }

        count ++;
        n = n * 10 + (ch - zero);
    }

    if (size != NULL) {
        (*size) = count;
    }

    return n;
}

long long int read_integer(FILE* src, int* size) {
    long long n = 0;
    long long sign = 1;
    int count = 0;
    int ch;

    discard_space(src);

    ch = read_char(src);
    if (ch == minus) {
        sign = -1;
    } else if (ch == plus) {
        sign = 1;
    } else if (ch != EOF) {
        pushback_char(ch, src);
    }

    while ((ch = read_char(src)) != EOF) {
        if ((ch < zero) || (ch > nine)) {
            pushback_char(ch, src);
            break;
        }

        count ++;
        n = n * 10 + (ch - zero);
    }
    
    if (size != NULL) {
        (*size) = count;
    }

    return n * sign;
}

void discard_word(FILE* src) {
    int ch;

    while ((ch = read_char(src)) != EOF) {
        if (char_space(ch)) {
            pushback_char(ch, src);
            break;
        }
    }
}

void discard_blank(FILE* src) {
    int ch;

    while ((ch = read_char(src)) != EOF) {
        if (!char_blank(ch)) {
            pushback_char(ch, src);
            break;
        }
    }
}

void discard_space(FILE* src) {
    int ch;

    while ((ch = read_char(src)) != EOF) {
        if (!char_space(ch)) {
            pushback_char(ch, src);
            break;
        }
    }
}

void discard_newline(FILE* src) {
    int ch;

    while ((ch = read_char(src)) != EOF) {
        if (!char_end_of_line(ch)) {
            pushback_char(ch, src);
            break;
        }
    }
}

void discard_this_line(FILE* src) {
    char crlf[3];
    int ch;

    while ((ch = read_char(src)) != EOF) {
        if (char_end_of_line(ch)) {
            pushback_char(ch, src);
            fgets(crlf, sizeof(crlf)/sizeof(char), src);
            break;
        }
    }
}

/************************************************************************************************/
int has_reached_end_of_word(FILE* src) {
    int ch = read_char(src);
    int yes = (char_space(ch) || (ch == EOF));

    if (ch != EOF) {
        pushback_char(ch, src);
    }

    return yes;
}

