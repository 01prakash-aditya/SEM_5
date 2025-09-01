#include <stdio.h>
#include <stdlib.h>

typedef enum {NEW, READY, RUNNING, BLOCKED, FINISHED} State;

typedef struct Process {
    int pid;
    int cpu_cycles;
    State state;
    int eventId;
    struct Process* next;
} Process;

Process* ready_head = NULL;
Process* ready_tail = NULL;

Process* blocked_head = NULL;

Process* running = NULL;

int pid_counter = 1;

void enqueue_ready(Process* p) {
    p->state = READY;
    p->next = NULL;
    if (ready_tail == NULL) {
        ready_head = ready_tail = p;
    } else {
        ready_tail->next = p;
        ready_tail = p;
    }
}

Process* dequeue_ready() {
    if (ready_head == NULL) return NULL;
    Process* p = ready_head;
    ready_head = ready_head->next;
    if (ready_head == NULL) ready_tail = NULL;
    p->next = NULL;
    return p;
}

void add_blocked(Process* p) {
    p->state = BLOCKED;
    p->next = blocked_head;
    blocked_head = p;
}

Process* remove_blocked_by_event(int eventId) {
    Process *curr = blocked_head, *prev = NULL;
    while (curr != NULL) {
        if (curr->eventId == eventId) {
            if (prev == NULL) {
                blocked_head = curr->next;
            } else {
                prev->next = curr->next;
            }
            curr->next = NULL;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

void newProcess() {
    Process* p = (Process*) malloc(sizeof(Process));
    p->pid = pid_counter++;
    p->cpu_cycles = 0;
    p->state = NEW;
    p->eventId = 0;
    p->next = NULL;
    enqueue_ready(p);
    printf("New: Process %d created and added to READY queue\n", p->pid);
}

void cpuEvent() {
    if (running == NULL) {
        running = dequeue_ready();
        if (running == NULL) {
            printf("CPU: No READY process to dispatch\n");
            return;
        }
        running->state = RUNNING;
        printf("Dispatch: Process %d is now RUNNING\n", running->pid);
    }
    running->cpu_cycles++;
    printf("CPU: Process %d ran for 1 cycle (total: %d)\n", running->pid, running->cpu_cycles);
}

void blockEvent(int eventId) {
    if (running == NULL) {
        printf("Block: No RUNNING process to block\n");
        return;
    }
    running->eventId = eventId;
    printf("Block: Process %d is BLOCKED on event %d\n", running->pid, eventId);
    add_blocked(running);
    running = NULL;
}

void unblockEvent(int eventId) {
    Process* p = remove_blocked_by_event(eventId);
    if (p == NULL) {
        printf("Unblock: No process found blocked on event %d\n", eventId);
    } else {
        enqueue_ready(p);
        printf("Unblock: Process %d moved to READY queue\n", p->pid);
    }
}

void doneEvent() {
    if (running == NULL) {
        printf("Done: No RUNNING process to finish\n");
        return;
    }
    printf("Done: Process %d has finished execution\n", running->pid);
    running->state = FINISHED;
    free(running);
    running = NULL;
}

int main() {
    newProcess();
    cpuEvent();
    cpuEvent();
    blockEvent(101);
    newProcess();
    cpuEvent();
    cpuEvent();
    doneEvent();
    newProcess();
    cpuEvent();
    unblockEvent(101);
    cpuEvent();
    doneEvent();
    cpuEvent();
    doneEvent();
    return 0;
}

