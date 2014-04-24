#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

/* Not used for now
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
 */


int main () {
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
    
    char readBuffer[80];
    char input[256];
    //pid = fork();
    if ((pid = fork()) == 0) { //Needs a loop to ask for user input.
        //This process should terminate when time runs out.
        /* Child 1 close the read end of pipe */
        close(pipe_1[0]);
        printf("Child 1: Process started.\n");
        printf("Child 1: Waiting for user input\n");
        //gets user input write to pipe

        while (scanf("%[^\n]*c", input) > 0) {
            //printf("%s\n", input);
            
            write(pipe_1[1], input, (strlen(input))+1);
        }

        //write_to_pipe(pipe_1[1]);
        //write(pipe_1[1], string, (strlen(string)+1));
        printf("Child 1: Terminating.\n");
        //exit(0);
    } else if (pid > 0) {
        
        if ((pid = fork()) == 0) {
            /* Child 2 process close its read end of pipe */
            close(pipe_2[0]);
            printf("Child 2: Starting child 2\n");
            
            
            char string[] = "Child 2 message 1\n";
            write(pipe_2[1], string, (strlen(string))+1);
            printf("Child 2: Terminating.\n");
        } else if (pid > 0) {
            
            if ((pid = fork()) == 0) {
                /* Child 3 process close its read end of pipe */
                close(pipe_3[0]);
                printf("Child 3: Starting child 3\n");
                char string2[] = "Child 3 message 1\n";
                write(pipe_3[1], string2, (strlen(string2))+1);
                printf("Child 3: Terminating.\n");
            } else if (pid > 0) {
                
                if ((pid = fork()) == 0) {
                    /* Child 4 process close its read end of pipe */
                    close(pipe_4[0]);
                    printf("Child 4: Starting child 4\n");
                    char string3[] = "Child 4 message 1\n";
                    write(pipe_4[1], string3, (strlen(string3))+1);
                    printf("Child 4: Terminating.\n");
                } else if (pid > 0) {
                    
                    if ((pid = fork()) == 0) {
                        /* Child 5 process close its read end of pipe */
                        close(pipe_5[0]);
                        printf("Child 5: Starting child 5\n");
                        char string4[] = "Child 5 message 1\n";
                        write(pipe_5[1], string4, (strlen(string4))+1);
                        printf("Child 5: Terminating.\n");
                    } else if (pid > 0) {
                        /* This is the parent process. Close other ends first */
                        close(pipe_1[1]);
                        close(pipe_2[1]);
                        close(pipe_3[1]);
                        close(pipe_4[1]);
                        close(pipe_5[1]);
                        
                        printf("Parent: Waiting for input on pipe #1\n");
                        waitpid(-1, NULL, 0);
                        
                        //need to use select() here instead of these while loops to identify
                        //which pipe is ready to be read from.
                        while (read(pipe_1[0], readBuffer, sizeof(readBuffer)) > 0) {
                            printf("Reading from child 1: ");
                            printf("%s\n", readBuffer);
                            //write(STDOUT_FILENO, readBuffer, (strlen(readBuffer))+1);
                        }
                        
                        
                        while (read(pipe_2[0], readBuffer, sizeof(readBuffer)) > 0) {
                            printf("Reading from child 2: ");
                            printf("%s", readBuffer);
                            //write(STDOUT_FILENO, readBuffer, (strlen(readBuffer))+1);
                        }
                        
                        while (read(pipe_3[0], readBuffer, sizeof(readBuffer)) > 0) {
                            printf("Reading from child 3: ");
                            printf("%s", readBuffer);
                            //write(STDOUT_FILENO, readBuffer, (strlen(readBuffer))+1);
                        }
                        
                        while (read(pipe_4[0], readBuffer, sizeof(readBuffer)) > 0) {
                            printf("Reading from child 4: ");
                            printf("%s", readBuffer);
                            //write(STDOUT_FILENO, readBuffer, (strlen(readBuffer))+1);
                        }
                        
                        while (read(pipe_5[0], readBuffer, sizeof(readBuffer)) > 0) {
                            printf("Reading from child 5: ");
                            printf("%s", readBuffer);
                            //write(STDOUT_FILENO, readBuffer, (strlen(readBuffer))+1);
                        }
                        //printf("Parent: Read '%s' from child pipe #1\n", readBuffer);
                        printf("Parent: Terminating.\n");
                    } else {
                        /* The fork failed */
                        fprintf(stderr, "Fork failed\n");
                        exit(1);
                    }
                } else {
                    /* The fork failed */
                    fprintf(stderr, "Fork failed\n");
                    exit(1);
                }
            } else {
                /* The fork failed */
                fprintf(stderr, "Fork failed\n");
                exit(1);
            }
        } else {
            /* The fork failed */
            fprintf(stderr, "Fork failed\n");
            exit(1);
        }
    } else {
        /* The fork failed */
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }
    return 0;
}
