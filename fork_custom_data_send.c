#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[]) {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    char *message;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if ( argc == 2 ){
    	message = strdup(argv[1]);
	if ( !message ){
		perror("strdup");
		exit(EXIT_FAILURE);
	}
    } else {
	message = "Hello";
    }

    if (pid == 0) {
        // Child Process
        close(pipefd[1]);  // Close write end
	printf("Receiving data from the parent\r\n");
        ssize_t bytes = read(pipefd[0], buffer, BUFFER_SIZE);
        if (bytes > 0) {
            buffer[bytes] = '\0';

	    // Checking what was received
	    if ( strcmp(buffer, "0xFF") == 0 ) {
		    printf("Turning on the lights\r\n");
	    } else if ( strcmp(buffer, "0xFE") == 0 ) {
		    printf("Turning on the AC\r\n");
	    } else {
		    printf("Invalid option\r\n");
	    }
            //printf("Child received: %s\n", buffer);
        }

	// Keep the child alive for inspection
	//sleep(20);
        close(pipefd[0]);
    } else {
        // Parent Process
        close(pipefd[0]);  // Close read end
	printf("Sending data to the child\r\n");
	//sleep(20);
        write(pipefd[1], message, strlen(message));
        printf("Parent sent: %s\n", message);
        close(pipefd[1]);
        wait(NULL);  // Wait for child to finish
    }

    return 0;
}
