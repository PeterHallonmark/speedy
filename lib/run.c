#include "run.h"

#include <stdio.h>
#include <sys/types.h>

#ifdef SIMULATE
#include "simulate.h"
#else
#include <sys/wait.h>
#include <unistd.h>
#endif

void run(const char *filename, char *const argv[])
{
    pid_t pid = fork();
    int exec_return;
    int status;
    
    /* Check if it is the child. */
    if (pid == 0) {
        exec_return = execv(filename, argv);
        /* If execve fails. */
        printf("Failure! execv error code = %d\n", exec_return);
        _exit(0); 
    } else if (pid < 0) {
        printf("Failed to fork\n");
    } else {
        /* Since it successfully ws able to fork, wait until the child
           process has run completly. */
        wait(&status);
    }
}
