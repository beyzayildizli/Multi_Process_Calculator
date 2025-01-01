#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Global variables to store the PIDs of child processes for different operations
pid_t pidAddition, pidSubtraction, pidMultiplication, pidDivision;

// Function to create child processes for each operation (addition, subtraction, multiplication, division)
void create_child_process(const char *process_name, int pipefd[]);

int main()
{
    int operationType;
    
    double num1, num2;
    double numbers[2]; // Array to hold two numbers for calculation

    int pipes[4][2]; // Array of pipes for communication between parent and child processes

    // Initialize pipes for each operation
    for (int i = 0; i < 4; i++)
    {
        if (pipe(pipes[i]) == -1) // Create a pipe, return error if failed
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Create child processes for each operation
    create_child_process("./addition", pipes[0]);
    create_child_process("./subtraction", pipes[1]);
    create_child_process("./multiplication", pipes[2]);
    create_child_process("./division", pipes[3]);

    // Main loop for the calculator interface
    while (1)
    {
        printf("Calculator:\n");
        printf("1- Addition\n2- Subtraction\n3- Multiplication\n4- Division\n5- Exit\nYour operation: ");
        scanf("%d", &operationType); // Take user input for operation type

        // Exit condition, terminate all child processes and clean up
        if (operationType == 5)
        {
            // Terminate all child processes using SIGTERM signal
            kill(pidAddition, SIGTERM); 
            kill(pidSubtraction, SIGTERM);
            kill(pidMultiplication, SIGTERM);
            kill(pidDivision, SIGTERM);

            // Close all pipes after use
            for (int i = 0; i < 4; i++)
            {
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            // Wait for all child processes to terminate and check their exit status
            int status;
            while (wait(&status) > 0) 
            {
                if (WIFEXITED(status)) {
                    printf("Child process exited with status: %d\n", WEXITSTATUS(status));
                }
                else if (WIFSIGNALED(status)) {
                    printf("Child process was terminated by signal: %d\n", WTERMSIG(status));
                }
            }

            printf("Exit\n");
            exit(0); // Exit the program
        }
        // Handle valid operations (addition, subtraction, multiplication, division)
        else if (operationType >= 1 && operationType <= 4)
        {
            printf("Number 1: ");
            scanf("%lf", &num1); 
            printf("Number 2: ");
            scanf("%lf", &num2); 

            numbers[0] = num1; // Store first number
            numbers[1] = num2; // Store second number

            double result;

            // Write the numbers to the corresponding pipe for the chosen operation
            write(pipes[operationType - 1][1], numbers, sizeof(numbers));
            sleep(1); // Allow time for child processes to perform operation

            // Read the result of the operation from the corresponding pipe
            read(pipes[operationType - 1][0], &result, sizeof(result));

            // Display the result based on the chosen operation
            switch (operationType)
            {
            case 1:
                printf("Result: %.2f + %.2f = %.2f\n", num1, num2, result);
                break;
            case 2:
                printf("Result: %.2f - %.2f = %.2f\n", num1, num2, result);
                break;
            case 3:
                printf("Result: %.2f * %.2f = %.2f\n", num1, num2, result);
                break;
            case 4:
                printf("Result: %.2f / %.2f = %.2f\n", num1, num2, result);
                break;
            }
        }
        // Handle invalid operation choice
        else
        {
            printf("You must choose between 1 and 5!\n");
        }
    }

    return 0;
}

// Function to create a child process for a given operation and execute the corresponding program
void create_child_process(const char *process_name, int pipefd[])
{
    pid_t pid = fork(); // Create a child process
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE); // Exit if fork fails
    }
    else if (pid == 0) // Child process
    {
        char pipe_read_str[20], pipe_write_str[20];
        snprintf(pipe_read_str, sizeof(pipe_read_str), "%d", pipefd[0]); // Convert pipe read end to string
        snprintf(pipe_write_str, sizeof(pipe_write_str), "%d", pipefd[1]); // Convert pipe write end to string

        char *const args[] = {pipe_read_str, pipe_write_str, NULL}; // Prepare arguments for execv

        // Execute the operation program (addition, subtraction, multiplication, or division)
        execv(process_name, args);
        perror("execv");
        exit(EXIT_FAILURE); // Exit if execv fails
    }

    // Store the child process PID for the corresponding operation
    if (strcmp(process_name, "./addition") == 0)
        pidAddition = pid;
    else if (strcmp(process_name, "./subtraction") == 0)
        pidSubtraction = pid;
    else if (strcmp(process_name, "./multiplication") == 0)
        pidMultiplication = pid;
    else if (strcmp(process_name, "./division") == 0)
        pidDivision = pid;
}
