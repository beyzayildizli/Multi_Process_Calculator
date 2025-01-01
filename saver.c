#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is provided
    if (argc != 5) 
    {
        // If not, print an error message and show the correct usage
        fprintf(stderr, "Usage: %s <num1> <num2> <operator> <result>\n", argv[0]);
        return EXIT_FAILURE;  
    }

    // Open the file "results.txt" in append mode to store the result
    FILE *file = fopen("results.txt", "a"); 
    if (file == NULL)
    {
        // If the file cannot be opened, print an error message
        perror("Failed to open results.txt");
        return EXIT_FAILURE; 
    }

    // Write the calculation result to the file in the format: num1 operator num2 = result
    fprintf(file, "%s %s %s = %s\n", argv[1], argv[3], argv[2], argv[4]); 

    // Close the file after writing
    fclose(file); 

    return 0;  
}
