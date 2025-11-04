# Posix Command Line Interface

A simple Unix-style command line interface written in C.  
Implements basic shell functionality using POSIX system calls for process creation, execution, and inter-process communication.

## Features

- Supports core shell commands (`cd`, `ls`, `clear`, `exit`)
- Executes external binaries from the system PATH
- Handles process creation and termination using `fork()`, `execvp()`, and `wait()`
- Implements inter-process communication through Unix pipes (`|`)
- Supports command chaining (e.g., `ls | grep txt | wc -l`)
- Demonstrates low-level concepts in process management, IPC, and file descriptor redirection

## Commands

- **exit**  
  Exit the shell.  
  Args: None

- **cd**  
  Change the current working directory.  
  Args: `cd <path>`

- **ls**  
  List directory contents.  
  Args: `ls`

- **clear**  
  Clear the terminal screen.  
  Args: None

- **piping**  
  Command chaining using the pipe operator (`|`).  
  Example: `ls | grep .c | wc -l`

## Technical Notes

- Built from scratch with POSIX APIs: `fork()`, `execvp()`, `pipe()`, `dup2()`, and `wait()`
- Uses file descriptors for inter-process communication
- Demonstrates how a Unix shell handles process control, redirection, and IPC at the system call level
- Serves as a minimal reference for understanding shell-level process orchestration
- Understanding of userspace communication and how the kernel manages IPC related resources with process handling as well
- Minimal string handling and edge cases are handled, the project goal was to implement process related features

## Project Timeline

- **Started:** 26/10/24  
- **Completed:** 03/11/25  
- **Edited:** 03/11/25

## Screenshots

See the screenshots folder for demonstrations.

`Copyright (c) Mustafa Malik. See LICENSE for more.`
