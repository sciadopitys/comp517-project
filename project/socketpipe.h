#ifndef socketpipe_h
#define socketpipe_h

#include "ourpipe.h"

void socketpipefn(char** left, char** right, int length, int pipenum);
pipe_info* socketpipe_get();

#endif // socketpipe_h