#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Main function to continuously read numbers, perform division, and save the result
int main(int argc, char *argv[])
{
    while (1)
    {
        double data[2]; // Array to hold two numbers for division
        // Read the two numbers from the pipe (pipe file descriptors passed as arguments)
        read(atoi(argv[0]), data, sizeof(data)); 

        // Perform the division operation
        // If data[1] is zero, division by zero will occur, and this may cause undefined behavior.
        double result = data[0] / data[1]; 

        // Write the result back to the corresponding pipe
        write(atoi(argv[1]), &result, sizeof(result)); 

        // Prepare the numbers and result for saving
        char num1_str[20], num2_str[20], result_str[20];
        snprintf(num1_str, sizeof(num1_str), "%.2f", data[0]);
        snprintf(num2_str, sizeof(num2_str), "%.2f", data[1]);
        snprintf(result_str, sizeof(result_str), "%.2f", result);

        // Prepare arguments for calling the "saver" program to save the operation result
        char *const args[] = {
            "./saver", // Executable to save the result
            num1_str, // First number as string
            num2_str, // Second number as string
            "/",       // Division operation symbol
            result_str, // Result as string
            NULL       // End of arguments
        };

        // Fork a new child process to execute the "saver" program
        if (fork() == 0) 
        {
            execv("./saver", args);  // Execute the saver program with the arguments
            perror("execv failed"); // If execv fails, print error message
            exit(EXIT_FAILURE);     // Exit child process if execv fails
        }

        // Sleep for 2 seconds to allow the saver process to execute and finish
        sleep(2);
    }
    return 0;
}
