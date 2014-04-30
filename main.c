#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

<<<<<<< HEAD
int main() {
    pid_t pid;
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2], pipe5[2];
    struct timeval start, end;
    int seconds = 0;
    int microseconds = 0;
    int i = 1;
=======
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

>>>>>>> FETCH_HEAD
    
    fd_set pipes;
    FD_ZERO(&pipes);
    
    gettimeofday(&start, NULL);
    
    /* Create Pipes Start */
    if (pipe(pipe1) < 0) {
        printf("Pipe #1 failed\n");
        return -1;
    }
    if (pipe(pipe2) < 0) {
        printf("Pipe #2 failed\n");
        return -1;
    }
    if (pipe(pipe3) < 0) {
        printf("Pipe #3 failed\n");
        return -1;
    }
    /*pipe(pipe4);
     pipe(pipe5);
     /* Create Pipes End */
    
<<<<<<< HEAD
    FD_SET(pipe1[0], &pipes);
    FD_SET(pipe2[0], &pipes);
    FD_SET(pipe3[0], &pipes);
    int message1 = 1;
    int message2 = 1;
    int message3 = 1;
    
    pid = fork();
    
    while (seconds < 29) {
        
        gettimeofday(&end, NULL);
        seconds = end.tv_sec - start.tv_sec;
        microseconds = end.tv_usec - start.tv_usec;
        float messageTime = seconds + (microseconds / 100000.0);
        
        if (pid == 0) {
            close(pipe1[0]);
            
            gettimeofday(&end, NULL);
            seconds = end.tv_sec - start.tv_sec;
            microseconds = end.tv_usec - start.tv_usec;
            messageTime = seconds + (microseconds / 100000.0);
            
            char readBuffer[256];
            sprintf(readBuffer, "%1d:%2.3f: Child 1 message %d\n", 0, messageTime, message1++);
            write(pipe1[1], readBuffer, (strlen(readBuffer)+1));
            int rand = random() % 3;
            sleep(rand);
            open(pipe1[0]);
        }
        else {
            pid = fork();
=======
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
>>>>>>> FETCH_HEAD
            
            if (pid == 0) {
                close(pipe2[0]);
                
                gettimeofday(&end, NULL);
                seconds = end.tv_sec - start.tv_sec;
                microseconds = end.tv_usec - start.tv_usec;
                messageTime = seconds + (microseconds / 100000.0);
                
                char readBuffer[256];
                sprintf(readBuffer, "%1d:%2.3f: Child 2 message %d\n", 0, messageTime, message2++);
                write(pipe2[1], readBuffer, (strlen(readBuffer)+1));
                int rand = random() % 3;
                sleep(rand);
                open(pipe2[0]);
            }
            else {
                pid = fork();
                
                if (pid == 0) {
                    close(pipe3[0]);
                    
                    gettimeofday(&end, NULL);
                    seconds = end.tv_sec - start.tv_sec;
                    microseconds = end.tv_usec - start.tv_usec;
                    messageTime = seconds + (microseconds / 100000.0);
                    
                    char readBuffer[256];
                    sprintf(readBuffer, "%1d:%2.3f: Child 3 message %d\n", 0, messageTime, message3++);
                    write(pipe3[1], readBuffer, (strlen(readBuffer)+1));
                    int rand = random() % 3;
                    sleep(rand);
                    open(pipe3[0]);
                }
                else {/* Parent process does the reading */
                    
                    gettimeofday(&end, NULL);
                    seconds = end.tv_sec - start.tv_sec;
                    microseconds = end.tv_usec - start.tv_usec;
                    messageTime = seconds + (microseconds / 100000.0);
                    
                    close(pipe1[1]);
                    close(pipe2[1]);
                    close(pipe3[1]);
                    
                    int rand = random() % 3;
                    sleep(rand);
                    
                    if (select(FD_SETSIZE, &pipes, NULL, NULL, NULL) > 0) {
                        if (FD_ISSET(pipe1[0], &pipes)) {
                            char input[256];
                            read(pipe1[0], input, sizeof(input));
                            printf("%1d:%2.3f: %s", 0, messageTime, input);
                            
                        }
                        else if (FD_ISSET(pipe2[0], &pipes)) {
                            char input[256];
                            read(pipe2[0], input, sizeof(input));
                            printf("%1d:%2.3f: %s", 0, messageTime, input);
                        }
                        else if (FD_ISSET(pipe3[0], &pipes)) {
                            char input[256];
                            read(pipe3[0], input, sizeof(input));
                            printf("%1d:%2.3f: %s", 0, messageTime, input);
                        }
                        else {
                            
                        }
                        open(pipe1[1]);
                        open(pipe2[1]);
                        open(pipe3[1]);
                    }
                    
                }
            }
        }
        int rand = random() % 3;
        sleep(rand);
    }
    return 0;
}
