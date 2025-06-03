#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    const char *message = "Hello from parent!";

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child Process
        close(pipefd[1]);  // Close write end
        ssize_t bytes = read(pipefd[0], buffer, BUFFER_SIZE);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("Child received: %s\n", buffer);
        }

	// Keep the child alive for inspection
	sleep(10);
        close(pipefd[0]);
    } else {
        // Parent Process
        close(pipefd[0]);  // Close read end
        write(pipefd[1], message, strlen(message));
        printf("Parent sent: %s\n", message);
        close(pipefd[1]);
        wait(NULL);  // Wait for child to finish
    }

    return 0;
}
