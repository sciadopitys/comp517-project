#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

#include "ourpipe.h"

// our pipes
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
        // not a pipeline, just run
        char **args = split_args(plstr);
        execvp(args[0], args);
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

    (*pipefn)(left, right, count_args(left) + count_args(right));
}


int main(int argc, char *argv[]) {
    int opt;
    int option_index = 0;
    char *inbuffer = NULL;
    size_t inbufsize = 0;
    
    int selected_pipe = 0;
    int selected_preset_pipe = -1;

    char *pipeline = "";

    int n_pipes = 1;
    pipe_info *pipes[n_pipes];

    pipes[0] = filepipe_get();

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"pipe", required_argument, 0, 'p'},
        {"pipeline", required_argument, 0, 'l'},
        {"pipelinenum", required_argument, 0, 'n'},
        {0, 0, 0, 0}
    };
    
    int n_preset_pipelines = 3;
    char *preset_pipelines[] = {"ls | wc", "ls -l | grep ourpipe", "ls -l | grep ourpipe | wc"};

    while ((opt = getopt_long(argc, argv, "hpli:", long_options, &option_index)) != -1) {
        switch (opt) {
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

    while (selected_pipe <= 0 || selected_pipe > n_pipes) {
        printf("Please select a pipe to test:\n");

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

    while (pipeline[0] == '\0' && (selected_preset_pipe < 0 || selected_preset_pipe > n_preset_pipelines)) {
        printf("Please select a pipeline to run:\n");

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

    if (selected_preset_pipe > 0){
        pipeline = preset_pipelines[selected_preset_pipe - 1];
    }
    

    while (pipeline[0] == '\0') {
        printf("Please enter a pipeline to test:\n");

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
    test_pipe(pipes[selected_pipe-1]->pipefn, pipeline, selected_pipe);

    return 0;
}
