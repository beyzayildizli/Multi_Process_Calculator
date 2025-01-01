# Multi-Process Calculator

This project implements a multi-process calculator using C. It leverages inter-process communication (IPC) via pipes to perform basic arithmetic operations such as addition, subtraction, multiplication, and division. Each operation is handled by a separate process, and results are stored in a file for future reference.


## Features

- **Addition**
- **Subtraction**
- **Multiplication**
- **Division**
- Results are saved to a `results.txt` file in the format:
  
  ```
  num1 operator num2 = result
  ```
- Graceful termination of all child processes.


## File Structure

- `Makefile`: Used for compiling the project components.
- `calculator.c`: Main program that provides a CLI interface for users.
- `addition.c`, `subtraction.c`, `multiplication.c`, `division.c`: Individual programs for each arithmetic operation.
- `saver.c`: Handles saving the results of calculations to `results.txt`.
- `results.txt`: Output file to store calculation results.


## Requirements

- GCC compiler


## Compilation

Run the following command to compile the project:

```bash
make
```

This will generate the following executable files:

- `calculator`
- `addition`
- `subtraction`
- `multiplication`
- `division`
- `saver`

## Usage

Run the `calculator` program:

```bash
./calculator
```

Follow the prompts to select an operation and input numbers. The program supports:

1. Addition
2. Subtraction
3. Multiplication
4. Division
5. Exit

Example:

```
Calculator:
1- Addition
2- Subtraction
3- Multiplication
4- Division
5- Exit
Your operation: 1
Number 1: 10
Number 2: 20
Result: 10.00 + 20.00 = 30.00
```

### Saving Results

Each operation result is automatically saved to `results.txt`. Example content:

```
10.00 + 20.00 = 30.00
50.00 - 25.00 = 25.00
```

## Cleanup

To remove compiled files and the results file, run:

```bash
make clean
```


## Implementation Details

### Communication via Pipes

- Parent and child processes communicate using pipes.
- Pipes are initialized for each operation (addition, subtraction, multiplication, division).

### Process Management

- Each arithmetic operation runs in a separate child process.
- Parent process sends numbers to child processes via pipes and reads the result.

### Error Handling

- Handles invalid inputs and division by zero gracefully.
- Ensures proper termination of all child processes when exiting.



## Credits

This project was developed by Beyza Yıldızlı. You can find me on [LinkedIn](https://www.linkedin.com/in/beyzayildizli/) or [GitHub](https://github.com/beyzayildizli).


