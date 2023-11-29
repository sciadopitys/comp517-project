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
#include "socketpipe.h"
#include <sys/socket.h>

void socketpipefn(char** left, char** right, int length, int pipenum) {
    int    sockfd[2];
    pid_t  cpid;

    if (socketpair(PF_LOCAL, SOCK_STREAM, 0, sockfd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {    // Writes to pipe 
        close(sockfd[0]);          // Close unused read end
        dup2(sockfd[1], STDOUT_FILENO); // Redirect stdin to pipe
        close(sockfd[1]); // Close write end after dup

        execvp(left[0], left);

        // ececvp only returns on error
        close(sockfd[0]);
        exit(EXIT_FAILURE);

    } else {        // Reads from pipe
        int cpid2 = fork();
        if (cpid2 == 0) {
            close(sockfd[1]);          // Close unused write end
            dup2(sockfd[0], STDIN_FILENO); // Redirect stdin to pipe
            close(sockfd[0]); // Close read end after dup

            execvp(right[0], right);

            // ececvp only returns on error
            close(sockfd[0]);
            exit(EXIT_FAILURE);
        }
        waitpid(cpid, NULL, 0);
        shutdown(sockfd[0], SHUT_WR);
        shutdown(sockfd[1], SHUT_WR);
        waitpid(cpid2, NULL, 0);
    }
}    


pipe_info* socketpipe_get(){
    pipe_info *pipe = (pipe_info *)malloc(sizeof(pipe_info));
    pipe->pipefn = &socketpipefn;
    pipe->name = strdup("Socket Pipe");
    pipe->desc = strdup("A linux socket pair");
    return pipe;
}