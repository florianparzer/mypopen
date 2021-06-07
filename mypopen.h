
#ifndef MYPOPEN_H_
#define MYPOPEN_H_
#include <stdio.h>
#include <stdlib.h>


#endif

FILE* mypopen(const char* cmd, const char *mode);

int mypclose(FILE* stream);

void reset();
