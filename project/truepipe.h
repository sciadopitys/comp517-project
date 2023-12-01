#ifndef truepipe_h
#define truepipe_h

#include "ourpipe.h"

void truepipefn(char** left, char** right, int length, int pipenum);
pipe_info* truepipe_get();

#endif // truepipe_h