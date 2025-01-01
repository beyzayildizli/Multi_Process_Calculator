#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "math.h"

int main(int argc, char *argv[])
{
    // Infinite loop to continuously read input, perform subtraction, and save the result
    while (1)
    {
        double data[2];  // Array to store the two numbers for subtraction
        
        // Read two numbers from the pipe (pipe read descriptor is passed in argv[0])
        // The data is stored in the 'data' array
        read(atoi(argv[0]), data, sizeof(data)); 

        // Perform the subtraction of the two numbers
        double result = data[0] - data[1];

        // Write the result of the subtraction to the pipe (write descriptor passed in argv[1])
        write(atoi(argv[1]), &result, sizeof(result)); 

        // Convert the numbers and result to strings for passing to the "saver" program
        char num1_str[20], num2_str[20], result_str[20];
        snprintf(num1_str, sizeof(num1_str), "%.2f", data[0]);  // Convert num1 to string with 2 decimal places
        snprintf(num2_str, sizeof(num2_str), "%.2f", data[1]);  // Convert num2 to string with 2 decimal places
        snprintf(result_str, sizeof(result_str), "%.2f", result);  // Convert result to string with 2 decimal places

        // Prepare the arguments for the "saver" program
        // Arguments include num1, num2, subtraction symbol, and result
        char *const args[] = {
            "./saver",   // The executable to save the result
            num1_str,    // First number as string
            num2_str,    // Second number as string
            "-",          // Subtraction operation symbol
            result_str,   // The result of the operation as a string
            NULL          // End of arguments
        };

        // Fork a child process to run the "saver" program
        if (fork() == 0)  // Child process
        {
            // Execute the "saver" program with the prepared arguments
            execv("./saver", args);  

            // If execv fails (e.g., "saver" program is not found or fails to execute), print error message
            perror("execv failed");  
            exit(EXIT_FAILURE);      // Exit the child process if execv fails
        }

        // Sleep for 2 seconds to give time for the "saver" process to execute
        sleep(2);
    }

    return 0;  // End of the main function
}
