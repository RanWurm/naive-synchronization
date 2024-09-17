# Project: Synchronization of File Writes in a Multi-process Environment

## Overview

This project explores the challenges and implements a solution for synchronizing file writes between multiple processes (specifically a parent and its two child processes) using naive synchronization techniques like `wait` and `sleep`. This is intended to demonstrate the complexities and potential pitfalls of coordinating access to shared resources (a file) without advanced synchronization tools.

## Project Description

### Objective

The goal is to ensure orderly writing to a shared file by the parent and child processes without their outputs interleaving, using basic synchronization methods.

### Implementation Details

- **Process Creation**: The program initiates by forking two child processes from the parent.
- **Writing Mechanism**: Each process writes a specific message to the same output file multiple times.
- **Synchronization Approach**: Utilizes `wait` and `sleep` to manage the writing order, ensuring that child processes complete their writes before the parent begins.
- **Parameterization**: The program accepts input parameters specifying the messages and the number of repetitions for each process.

### Key Functionalities

1. **File Handling**:
   - Automatically creates an `output.txt` file to store the output from all processes.

2. **Controlled Execution**:
   - Child processes are created simultaneously and write to the file without waiting for each other to finish.
   - The parent process begins writing only after both child processes have finished their output.

3. **Naive Synchronization**:
   - Implements basic synchronization to prevent interleaving of outputs in the file.
   - Relies on timing mechanisms rather than locks or advanced concurrency controls.

4. **Error Management**:
   - Includes robust error handling to manage failures in process creation or file operations.

5. **User Input**:
   - Accepts command-line arguments to customize the output messages and the number of write operations for each process.

## Usage

Compile the program using a C compiler like GCC:

```bash
gcc -o syncwrites part1.c
Run the program with command-line arguments:

bash
Copy code
./syncwrites "Parent message" "Child1 message" "Child2 message" 10
Example
This will write each message 10 times to output.txt in the specified order:

Child1's messages are written first, followed by Child2's, and finally the Parent's messages, ensuring no interleaving occurs.
Error Handling
The program checks for sufficient command-line arguments and handles runtime errors gracefully, printing useful error messages if system calls fail.

Conclusion
This project not only demonstrates handling basic synchronization in multi-process applications but also underscores the limitations and considerations when using naive methods in place of more robust concurrency control mechanisms.

License
This code is provided under the MIT License.

Contact
For more information, suggestions, or contributions, please contact me at [RanWurembrand@gmail.com].
