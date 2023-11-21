#define _GNU_SOURCE

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
#include <sys/socket.h>
#include <linux/in.h>
#include "filepipe.h"


void filepipefn(char** left, char** right, int length) {
    /*
    int fdr = open("/tmp", O_RDWR|O_TMPFILE, S_IRUSR|S_IWUSR);
       
      
    srand(time(NULL));
    int r = rand();
    */

    char buf[12];
    snprintf(buf, 12, "pipe_%d", length);
    /*int fdr = open(buf, O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);*/
    int fdr = socket(AF_LOCAL, SOCK_DGRAM, 0);
    int dummy = socket(AF_LOCAL, SOCK_DGRAM, 0);
    /*int fdr = open("/tmp", O_RDWR|O_TMPFILE, S_IRUSR|S_IWUSR);*/
    /*int fdr = open(buf, O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);*/
    
    if (fdr < 0)
    {
        printf("something went wrong: %s", strerror(errno));
    }
    
    struct sockaddr_in address;
    address.sin_family = AF_LOCAL; 
    address.sin_port = 0;  
    address.sin_addr.s_addr = INADDR_ANY;

    int bind_sock = bind(fdr, (struct sockaddr *)&address, sizeof(address));
    if (bind_sock < 0)
    {
        printf("something went wrong: %s", strerror(errno));
        perror("forking failed");
    }
    
    int connect_sock = connect(dummy, (struct sockaddr *)&address, sizeof(address));
    if (connect_sock < 0)
    {
        printf("something went wrong: %s", strerror(errno));
        perror("forking failed");
    }     
    
    
    int fdw = dup(fdr);
    
    int rcl = fork();
    if (rcl < 0)
    {
        printf("something went wrong: %s", strerror(errno));
        perror("forking failed");
    } else if (rcl == 0) {
        dup2(fdw, fileno(stdout));
        close(fdr);
        execvp(left[0], left);
    } else {
        wait(NULL);
        int rcr = fork();
        
        if (rcr < 0)
        {
            printf("something went wrong: %s", strerror(errno));
            perror("forking failed");
        } else if (rcr == 0) {
            
            lseek(fdr, 0, SEEK_SET);
            dup2(fdr, fileno(stdin));
            
            close(fdw);
            execvp(right[0], right);
        } else {
            wait(NULL);
            close(fdr);
            close(fdw);
        }
    }    
}    


pipe_info* filepipe_get(){
    pipe_info *pipe = (pipe_info *)malloc(sizeof(pipe_info));
    pipe->pipefn = &filepipefn;
    pipe->name = strdup("File Pipe");
    pipe->desc = strdup("Pipe which uses routing of stdout and stdin and a file for communication");
    return pipe;
}


int main1( int argc, char *argv[] ) {  
    int length = argc - 1;
    char* argv1[length];
    char* argv2[length];
    memset(argv1, 0, length*sizeof(char*));
    memset(argv2, 0, length*sizeof(char*));
    int j = 0;
    bool left = true;
    int i;
    for (i=1; i<argc; i++) {
        
        if (left) {
            char* elem = argv[i];
            if (strcmp(elem, "NULL") == 0) {
                j = 0;
                left = false;
            } else {
                argv1[j] = elem;
                ++j;
            }
        } else {
            argv2[j] = argv[i];
            ++j;
        }
    }
    
    /*
    char *const argv1[] = {"ls", "-l", NULL};  
    char *const argv2[] = {"wc", "-l", NULL};  
    */
    
    filepipefn(argv1, argv2, length); 
    
      
    return 1;  
}  