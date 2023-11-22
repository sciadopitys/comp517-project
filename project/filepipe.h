#ifndef filepipe_h
#define filepipe_h

#include "ourpipe.h"

void filepipefn(char** left, char** right, int length, int pipenum);
pipe_info* filepipe0_get();
pipe_info* filepipe1_get();
pipe_info* filepipe2_get();
#endif // filepipe_h