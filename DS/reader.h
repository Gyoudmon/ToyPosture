#ifndef _READER_H
#define _READER_H

int has_reached_end_of_word(FILE* src);

unsigned long long int read_natural(FILE* src, int* size);
long long int read_integer(FILE* src, int* size);

void discard_word(FILE* src);
void discard_space(FILE* src);
void discard_newline(FILE* src);
void discard_this_line(FILE* src);

#endif /* _READER_H */

