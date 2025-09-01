#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>  
#include <stdlib.h>    
#include <string.h>

int main() {
    printf("PREFORK: This message will be duplicated by all processes");

    pid_t pid1 = fork();

    if (pid1 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid1 == 0) {
        fflush(stdout);

        pid_t curr_pid = getpid();
        pid_t parent_pid = getppid();

        for (int i = 1; i <= 3; i++) {
            printf("\nMessage %d from child 1 | PID: %d | PPID: %d\n", i, curr_pid, parent_pid);
            sleep(1);
        }
        exit(0);  
    }

    pid_t pid2 = fork();

    if (pid2 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid2 == 0) {
        fflush(stdout);

        pid_t curr_pid = getpid();
        pid_t parent_pid = getppid();

        for (int i = 1; i <= 3; i++) {
            printf("\nMessage %d from child 2 | PID: %d | PPID: %d\n", i, curr_pid, parent_pid);
            sleep(1);
        }
        exit(0); 
    }

    fflush(stdout);

    pid_t curr_pid = getpid();
    pid_t parent_pid = getppid();

    for (int i = 1; i <= 3; i++) {
        printf("\nMessage %d from parent | PID: %d | PPID: %d\n", i, curr_pid, parent_pid);
        sleep(1);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("\nBoth children finished execution\n");

    return 0;
}

