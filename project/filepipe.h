#ifndef filepipe_h
#define filepipe_h

#include "ourpipe.h"

void filepipefn(char** left, char** right, int length);
pipe_info* filepipe_get();
#endif // filepipe_h