#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    while (1)  // Infinite loop to continuously read input, perform operation, and save result
    {
        double data[2];  // Array to store two numbers for multiplication
        
        // Read two numbers from the pipe. Pipe read descriptor passed as argv[0]
        read(atoi(argv[0]), data, sizeof(data)); 

        // Perform the multiplication of the two numbers
        double result = data[0] * data[1];

        // Write the result of multiplication to the write end of the pipe (argv[1])
        write(atoi(argv[1]), &result, sizeof(result)); 

        // Convert the numbers and result to strings for passing to the "saver" program
        char num1_str[20], num2_str[20], result_str[20];
        snprintf(num1_str, sizeof(num1_str), "%.2f", data[0]);  // Convert num1 to string with 2 decimals
        snprintf(num2_str, sizeof(num2_str), "%.2f", data[1]);  // Convert num2 to string with 2 decimals
        snprintf(result_str, sizeof(result_str), "%.2f", result);  // Convert result to string with 2 decimals

        // Prepare the arguments for the "saver" program
        // Arguments include num1, num2, multiplication symbol, and result
        char *const args[] = {
            "./saver",   // The executable to save the result
            num1_str,    // First number as string
            num2_str,    // Second number as string
            "*",          // Multiplication operation symbol
            result_str,   // The result of the operation as a string
            NULL          // End of arguments
        };

        // Fork a child process to run the "saver" program
        if (fork() == 0)  // Child process
        {
            execv("./saver", args);  // Execute the saver program with the prepared arguments
            perror("execv failed");  // If execv fails, print error message
            exit(EXIT_FAILURE);      // Exit the child process on failure
        }

        // Sleep for 2 seconds to give time for the saver process to execute
        sleep(2);
    }

    return 0;  // End of the main function
}
