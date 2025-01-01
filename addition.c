#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // Infinite loop to continuously read data, perform operations, and save results
    while (1)
    {
        double data[2];
        
        // Read data (two numbers) from the pipe (pipe file descriptor passed as arguments)
        // argv[0] is the read pipe, argv[1] is the write pipe
        read(atoi(argv[0]), data, sizeof(data)); 

        // Perform addition operation on the received numbers
        double result = data[0] + data[1]; 

        // Write the result back to the corresponding pipe
        write(atoi(argv[1]), &result, sizeof(result)); 

        // Convert the numbers and result into strings with two decimal places
        char num1_str[20], num2_str[20], result_str[20];
        snprintf(num1_str, sizeof(num1_str), "%.2f", data[0]);
        snprintf(num2_str, sizeof(num2_str), "%.2f", data[1]);
        snprintf(result_str, sizeof(result_str), "%.2f", result);

        // Prepare arguments for calling the "saver" program to save the result
        char *const args[] = {
            "./saver", // Executable to save the result
            num1_str, // First number as string
            num2_str, // Second number as string
            "+",       // Operation symbol
            result_str, // Result as string
            NULL       // End of arguments
        };

        // Create a child process to execute the "saver" program
        if (fork() == 0) 
        {
            execv("./saver", args);  // Execute the saver program with the arguments
            perror("execv failed"); // If execv fails, display an error message
            exit(EXIT_FAILURE);     // Exit the child process
        }

        // Sleep for 2 seconds to allow time for the saver process to run
        sleep(2); 
    }
    return 0;
}
