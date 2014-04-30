#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2], pipe5[2];
    struct timeval start, end;
    int seconds = 0;
    int microseconds = 0;
    int i = 1;
    
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
                    open(pipe1[0]);
                    open(pipe2[0]);
                    open(pipe3[0]);
                    
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