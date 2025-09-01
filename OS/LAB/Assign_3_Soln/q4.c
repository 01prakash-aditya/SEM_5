#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/select.h>

#define MAX_PROCESSES 3

pid_t ready_queue[MAX_PROCESSES * 10]; 
int front = 0, rear = 0;

void enqueue(pid_t pid) {
    ready_queue[rear++] = pid;
}

pid_t dequeue() {
    return ready_queue[front++];
}

int is_queue_empty() {
    return front == rear;
}

void child_process(const char *type) {
    raise(SIGSTOP); 
    if (strcmp(type, "P1") == 0) {
        for (int i = 0; i <= 16; i += 2) {
            printf("[P1 - Even] %d\n", i);
            fflush(stdout);
            usleep(500000);
        }
    } else if (strcmp(type, "P2") == 0) {
        for (int i = 1; i <= 16; i += 2) {
            printf("[P2 - Odd] %d\n", i);
            fflush(stdout);
            usleep(500000);
        }
    } else if (strcmp(type, "P3") == 0) {
        for (char c = 'A'; c <= 'J'; c++) {
            printf("[P3 - Char] %c\n", c);
            fflush(stdout);
            usleep(500000);
        }
    }
    exit(0);
}

int main() {
    pid_t pids[MAX_PROCESSES];
    int finished = 0;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            if (i == 0) child_process("P1");
            else if (i == 1) child_process("P2");
            else child_process("P3");
        } else {
            pids[i] = pid;
            enqueue(pid);
        }
    }

    while (finished < MAX_PROCESSES) {
        if (is_queue_empty())
            break;

        pid_t pid = dequeue();
        kill(pid, SIGCONT);

        printf("=== Press Enter to BLOCK the process (or let it finish) ===\n");
        fflush(stdout);

        int status;
        int child_finished = 0;

        while (1) {
            pid_t retpid = waitpid(pid, &status, WNOHANG);
            if (retpid > 0) {
                if (WIFEXITED(status)) {
                    child_finished = 1;
                    break;
                }
            }

            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);
            struct timeval tv = {0, 100000}; 

            int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

            if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
                getchar();
                kill(pid, SIGSTOP);
                enqueue(pid);
                printf("Scheduler: P%d blocked and re-enqueued\n",
                       pid == pids[0] ? 1 : pid == pids[1] ? 2 : 3);
                break;
            }
        }

        if (child_finished) {
            printf("[P%d] Finished\n", pid == pids[0] ? 1 : pid == pids[1] ? 2 : 3);
            finished++;
        }
    }

    printf("All processes finished.\n");
    return 0;
}

