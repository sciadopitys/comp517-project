#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

#include "ourpipe.h"

// our pipes
#include "truepipe.h"
#include "filepipe.h"

char **split_args(char *str){
    int ntok = 0;
    char *delimiter = " ";
    // Step 1, count number of tokens
    char *tempStr = strdup(str); // Duplicate the string for counting

    char *token = strtok(tempStr, delimiter);
    while (token != NULL) {
        ntok++;
        token = strtok(NULL, delimiter);
    }

    // Step 2, create the array
    char **tokens = malloc(ntok * sizeof(char *) + 1);
    tokens[ntok] = NULL; // Null terminate
    
    tempStr = strdup(str);
    token = strtok(tempStr, delimiter);
    int i = 0;
    while (token != NULL) {
        // printf("%s\n", token);
        tokens[i] = strdup(token);
        token = strtok(NULL, delimiter);
        i++;
    }
    return tokens;
}

enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

int count_args(char** args){
    int i;
    for(i = 0; args[i] != NULL; i++);
    return i+1;
}


void test_pipe(pipefunc pipefn, char *pipeline, int pipenum){


    char* plstr = strdup(pipeline);
    char *left_str = plstr;
    char *right_str;

    char* first_pipe = strchr(plstr, '|'); 
    if (first_pipe == NULL) {
        
        printf("\nNot a pipeline.\n");
        return;
        /*
        char **args = split_args(plstr);
        execvp(args[0], args); 
        */
    }
    *first_pipe = '\0'; // Add null terminator to make it two strings

    right_str = first_pipe + 1;

    char **left = split_args(left_str);
    char **right = NULL;

    char* second_pipe = strchr(right_str, '|'); 
    if (second_pipe != NULL) {
        // chain of pipes, call recursively

        char pnumstr[5];
        snprintf(pnumstr, 5, "%d", pipenum);
        right = malloc(6 * sizeof(char *));

        right[0] = strdup("./ourpipe");
        right[1] = strdup("--pipe");
        right[2] = strdup(pnumstr);
        right[3] = strdup("--pipeline");
        right[4] = strdup(right_str);
        right[5] = NULL;
    } else {
        // just one pipe
        right = split_args(right_str);
    }

    // int i;
    // for(i = 0; left[i] != NULL; i++){
    //     printf("%s\n", left[i]);
    // }

    // printf("left above, right below\n");

    // for(i = 0; right[i] != NULL; i++){
    //     printf("%s\n", right[i]);
    // }

    // printf("left args: %d, right args: %d\n", count_args(left), count_args(right));

    // pipefn = &filepipefn;

    (*pipefn)(left, right, count_args(left) + count_args(right), pipenum);
}


int main(int argc, char *argv[]) {
    int opt;
    int option_index = 0;
    char *inbuffer = NULL;
    size_t inbufsize = 0;
    
    int selected_pipe = 0;
    int selected_preset_pipe = -1;
    int eval_time = 0;
    int max_count = 1;

    char *pipeline = "";
    
    char *pipedir = "pipes";
    mkdir(pipedir, 0777);

    pipe_info *pipes[] = {
        truepipe_get(),
        filepipe0_get(),
        filepipe1_get(),
        filepipe2_get(),
    };
    
    int n_pipes = sizeof(pipes) / sizeof(pipes[0]);

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"pipe", required_argument, 0, 'p'},
        {"pipeline", required_argument, 0, 'l'},
        {"pipelinenum", required_argument, 0, 'n'},
        {0, 0, 0, 0}
    };


    int n_preset_pipelines = 3;
    char *preset_pipelines[] = {"ls | wc", "ls -l | grep ourpipe", "ls -l | grep ourpipe | wc"};

    while ((opt = getopt_long(argc, argv, "rthp:l:n:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'r':
                printf("User can specify different pipelines in sequence (only works with file pipes).\n");
                max_count = 50;
                break;
            case 't':
                printf("Evaluating pipeline performance (only works with file pipes).\n");
                eval_time = 1;
                break;
            case 'h':
                printf("This is the master program to test all of our different pipes.\n");
                break;
            case 'p':
                selected_pipe = atoi(optarg);
                break;
            case 'l':
                pipeline = strdup(optarg);
                break;
            case 'n':
                selected_preset_pipe = atoi(optarg);
                break; 
            default:
                break;
        }
    }

    int count = 0;
    while (count < max_count) {
        if (count > 0) {
            selected_pipe = 0;
            selected_preset_pipe = -1;
            pipeline = "";
        }
        while (selected_pipe <= 0 || selected_pipe > (n_pipes + 1)) {
            printf("\nPlease select a pipe to test, or press 5 to exit:\n");

            int i;
            for(i=0; i<n_pipes; i++) {
                printf("(%d) %s: %s\n", i + 1, pipes[i]->name, pipes[i]->desc);
            }

            if (getline(&inbuffer, &inbufsize, stdin) != -1) {
                // Do something with the input
                selected_pipe = atoi(inbuffer);
            } else {
                printf("Error reading input.\n");
                return -1;
            }
        }
        
        if (selected_pipe == 5) {
            count = 50;
            break;
        }

        if (pipeline[0] == '\0') {
            while (selected_preset_pipe < 0 || selected_preset_pipe > n_preset_pipelines) {
                printf("\nPlease select a pipeline to run. Default is custom pipeline:\n");

                printf("(0) Enter custom string\n");

                int i;
                for(i=0; i<n_preset_pipelines; i++) {
                    printf("(%d) %s\n", i+1, preset_pipelines[i]);
                }

                if (getline(&inbuffer, &inbufsize, stdin) != -1) {
                    // Do something with the input
                    selected_preset_pipe = atoi(inbuffer);
                } else {
                    printf("Error reading input.\n");
                    return -1;
                }
            }
        }

        if (selected_preset_pipe > 0){
            pipeline = preset_pipelines[selected_preset_pipe - 1];
        }
        

        while (pipeline[0] == '\0') {
            printf("\nPlease enter a pipeline to test:\n");

            if (getline(&inbuffer, &inbufsize, stdin) != -1) {
                // Do something with the input
                pipeline = strdup(inbuffer);
                pipeline[strcspn(pipeline, "\r\n")] = 0; // strip newline
            } else {
                printf("Error reading input.\n");
                return -1;
            }
        }

        // printf("running pipe %d\n", selected_pipe);
        if (eval_time == 1) {
            struct timespec start, end, delta;

            clock_gettime( CLOCK_REALTIME, &start );
            
            test_pipe(pipes[selected_pipe-1]->pipefn, pipeline, selected_pipe);

            clock_gettime( CLOCK_REALTIME, &end );
            sub_timespec(start, end, &delta);
            printf("%d.%.9ld seconds taken\n", (int)delta.tv_sec, delta.tv_nsec);
        } else {
            test_pipe(pipes[selected_pipe-1]->pipefn, pipeline, selected_pipe);
        }
    
    count++;
    }    

    return 0;
}
