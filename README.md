# Mini Linux Shell (C++)

A simple command-line shell built in C++ that can execute Linux commands, support background execution, and basic piping.

## Features

- Run standard Linux commands like ls, pwd, etc.
- Background execution using &
- Basic piping using |
- Process handling using fork(), execvp(), and wait()

## How it Works

The program takes input from the user, splits it into command and arguments, and runs it as a new process. It can also run commands in the background and connect two commands using a pipe.

## Example Commands

ls  
pwd  
ls &  
ls | grep txt  

## How to Compile

g++ main.cpp -o shell

## How to Run

./shell

Type exit to quit.

## Concepts Used

- Process creation and management  
- Inter-process communication  
- Command parsing  
- Basic Linux system calls  

Built as a systems project to understand Operating Systems concepts.