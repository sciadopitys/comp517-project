#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "filepipe.h"


void truepipefn(char** left, char** right, int length, int pipenum) {
    int    pipefd[2];
    pid_t  cpid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {    // Writes to pipe 
        close(pipefd[0]);          // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdin to pipe
        close(pipefd[1]); // Close write end after dup

        execvp(left[0], left);

        // ececvp only returns on error
        close(pipefd[0]);
        exit(EXIT_FAILURE);

    } else {        // Reads from pipe
        close(pipefd[1]);          // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe
        close(pipefd[0]); // Close read end after dup

        execvp(right[0], right);

        // ececvp only returns on error
        close(pipefd[0]);
        exit(EXIT_FAILURE);
    }
}    


pipe_info* truepipe_get(){
    pipe_info *pipe = (pipe_info *)malloc(sizeof(pipe_info));
    pipe->pipefn = &truepipefn;
    pipe->name = strdup("True Pipe");
    pipe->desc = strdup("A real linux pipe");
    return pipe;
}