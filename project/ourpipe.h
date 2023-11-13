#ifndef ourpipe_h
#define ourpipe_h

typedef void (*pipefunc)(char**, char**, int);

typedef struct pipe_information {
    pipefunc pipefn;
    char* name;
    char* desc;
} pipe_info;

#endif // ourpipe_h