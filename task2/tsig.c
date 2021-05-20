#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------------------------------------------------------

    The aim of the laboratory was to create a program in C language for testing the synchronization mechanisms and signals.
    It contains two parts:
    1. Without signal handlers:
        * creates child processes.
        * verify correcteness of creation process.
        * prints corresponsind messages.
    2. With signals handlers.
        * handles the keyboard interrupt CTRL+C
        * on interrupt kills all created children and stops creation of the children.

    To build&run:
    gcc tsig.c
    ./a.out

 ------------------------------------------------------------------------------------------------------------------------ */

#define NUM_CHILD   8                   // Defines number of children to be created.
#define WITH_SIGNALS

int interrupt = 0;                      // Flag for interrupt. On keyboard interrupt it is set to 1.
pid_t children_pids[NUM_CHILD];         // Array for children.


#ifdef WITH_SIGNALS
    void trigger_interrupt() {
        printf("parent[%d] Keyboard interrupt triggered.\n", getpid());
        interrupt = 1;
    }

    void interrupt_message() {
        printf("child[%d]: received SIGTERM signal, terminating\n", getpid());
    }
#endif

/*
    Terminates all crated children - sending SIGTERM signal.
*/
void terminate(int fail_number) {
    for (int i = 0; i <= fail_number; i++) {
        printf("parent[%d]: sending SIGTERM signal to child[%d].\n", getpid(), children_pids[i]);
        kill(children_pids[i], SIGTERM);
    }
}

/*
    Waits for all processes to end.
    Counts number of processes ended with code 0 and 1.
    Prints information about the program execution.

    WIFEXITED(status)   -   returns true if child terminated normally.
    WEXITSTATUS(status) -   returns the exit status of the child.
*/
void getSystemInfo() {
    pid_t pid;
    int status, code_zero_count = 0, code_one_count = 0;

    for(;;) {
        /*
            wait() blocks the calling process until one of its child processes exits or a signal is received.
        */
        pid = wait(&status);

        if (pid == -1)
            break;

        if (WIFEXITED(status))
            printf("child[%d]: Exit status: %d\n", pid, WEXITSTATUS(status));


        if(WEXITSTATUS(status) == 1)
            code_one_count++;
        else if (WEXITSTATUS(status) == 0)
            code_zero_count++;

    }

    printf("\nparent[%d]: End of children creation process.\n", getpid());
    printf("Number of children processes: %d\n", code_zero_count + code_one_count);
    printf("Children processes exited with code 0: %d\n", code_zero_count);
    printf("Children processes exited with code 1: %d\n", code_one_count);
}

void childProcess() {

    #ifdef WITH_SIGNALS
            signal(SIGINT, SIG_IGN);                    // Signal interrupt.
            signal(SIGTERM, interrupt_message);         // Signal terminate.
    #endif

    printf("child[%d]: Created by parent[%d]\n", getpid(), getpid());
    sleep(10);                                      // 10 sec delay.
    printf("child[%d]: Execution completed\n", getpid());
}

void create_child(int i) {
    /*
        fork() creates a new process - child, by duplicating the calling process.
        It returns:
        * 0   if child creation was created successfully
        * -1  if child creation failed
        * >0  if we are in parent process, returned value is child's PID.
    */
    pid_t pid = fork();

    if (pid == 0) {                                     // Child process successful
        childProcess();
        exit(0);
    }
    else if (pid == -1) {                                // Child process failure.
        printf("child[pid]: Error. Can not create new child.");
        terminate(i);
        exit(1);
    }
    else {                                              // Parent process; pid = child's pid.
        children_pids[i] = pid;
        printf("parent[%d]: Created child[%d]\n", getpid(), pid);
    }
}

int main() {
    /*
        SIG_IGN -   Signal ignored.
        SIGCHLD -   When a child process stops or terminates, SIGCHLD is sent to the parent process.
        SIG_DFL -   Default signal handling.
        SIGINT  -   Signal interrupt.
        _NSIG   -   Total number of signals.
    */

   // Force ignoring all signals and then restore the default handler for SIGCHLD signal.
    #ifdef WITH_SIGNALS
        for(int i = 0; i < _NSIG; i++)
            signal(i, SIG_IGN);

        signal(SIGCHLD, SIG_DFL);
        signal(SIGINT, trigger_interrupt);              // Interrupt handler.
    #endif


    /*
        Loop for creating children.
        Bewteen two consecutive children there is a one second delay and check for interrupt.
        If interrupt (CTRL+C) is triggered, process of children creation is stopped and all already children are killed.
    */

    for (int i = 0; i < NUM_CHILD; i++) {
        create_child(i);
        sleep(1);                                       // 1sec delay.

        // Checking if keyboard interrupt.
        #ifdef WITH_SIGNALS
            if (interrupt != 0) {
                terminate(i);
                break;                                  // End process of children creation.
            }
        #endif

    }

    getSystemInfo();

    // Setting all signals to default.
    #ifdef WITH_SIGNALS
        for(int i = 0; i < _NSIG; i++)
            signal(i, SIG_DFL);                         // SIG_DFL	default signal handling
    #endif

    return 0;

}

