#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void read_from_pipe (int file) {
    FILE *stream;
    int c;
    stream = fdopen (file, "r");
    while ((c = fgetc (stream)) != EOF)
        putchar(c);
    fclose (stream);
}

void write_to_pipe (int file) {
    FILE *stream;
    stream = fdopen (file, "w");
    fprintf (stream, "Child 1 message 1\n");
    fclose (stream);
}

int main (void) {
    /* The five processess */
    pid_t pid;
    /* The five pipes */
    int pipe_1[2], pipe_2[2], pipe_3[2], pipe_4[2], pipe_5[2];
    
    
    /*Create the pipes*/
    if (pipe(pipe_1)) { //Use this pipe for keyboard input
        fprintf (stderr, "Pipe #1 failed\n");
    }
    //The rest should be used for files.
    if (pipe(pipe_2)) {
        fprintf (stderr, "Pipe #2 failed\n");
    }
    if (pipe(pipe_3)) {
        fprintf (stderr, "Pipe #3 failed\n");
    }
    if (pipe(pipe_4)) {
        fprintf (stderr, "Pipe #4 failed\n");
    }
    if (pipe(pipe_5)) {
        fprintf (stderr, "Pipe #5 failed\n");
    }
    
    /* Create the child process */
    int nbytes;
    //char string[] = "Child 1 message 2";
    char readBuffer[80];
    char input[256];
    //pid = fork();
    if ((pid = fork()) == (pid_t) 0) { //Needs a loop to ask for user input.
        //This process should terminate when time runs out.
        /* This is the child process. Close other end first */
        close(pipe_1[0]);
        printf("Child 1: Process started.\n");
        printf("Child 1: Waiting for user input\n");
        //gets user input write to pipe
        scanf("%[^\n]s", input);
        printf("%s\n", input);
        write(pipe_1[1], input, (strlen(input)+1));
        //write_to_pipe(pipe_1[1]);
        //write(pipe_1[1], string, (strlen(string)+1));
        printf("Child 1: Finished writing\n");
        printf("Child 1: Terminating.\n");
        //exit(0);
    }
    else if (pid > (pid_t) 0) {
        /* This is the parent process. Close other end first */
        int status;
        close(pipe_1[1]);
        printf("Parent: Waiting for input on pipe #1\n");
        waitpid(-1, &status, 0);
        //printf("Parent: Reading from child pipe #1.\n");
        //read_from_pipe(pipe_1[0]);
        nbytes = read(pipe_1[0], readBuffer, sizeof(readBuffer));
        printf("Parent: Read %s from child pipe #1\n", readBuffer);
        printf("Parent: Terminating.\n");
        //exit(0);
    }
    else {
        /* The fork failed */
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }
    
    return 0;
}
