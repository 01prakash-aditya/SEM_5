#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t curr_pid = getpid();
    printf("PID stored in the var curr_pid: %d\n", curr_pid);
    sleep(60);
    return 0;
}

