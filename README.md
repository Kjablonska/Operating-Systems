# Operating Systems assignments

## Script for filenames modification
The aim of this project was to implement a Shell script modifying filenames.
It contains two files:
- modify.sh - implementing the task.
- modify_exmaples.sh - script for testing purposes.

```sh
./modify [-r] [-l|-u] <dir/file names...>
./modify [-r] <sed pattern> <dir/file names...>
./modify [-h]
```

Flags functions:
- ```-r``` 
Recursive change of filenames in the provided directory and subdirectories.
- ```-u```
Change filenames to upercase.
- ```-l``` 
Change filenames to lowercase.

The script is capable of handling "problematic" dictionaries and filenames, such as: starting from minus sign, space-separated, containing symbols and numbers.

  
## Processes and signals

The aim of the laboratory was to create a program in C language for testing the synchronization mechanisms and signals.  
It contains two parts:  
1. Without signal handlers:  
- creates child processes.   
- verify correcteness of creation process.  
- prints corresponsind messages.  
2. With signals handlers.  
- handles the keyboard interrupt CTRL+C  
- on interrupt kills all created children and stops creation of the children.  
      
To build&run:
```sh
gcc tsig.c
./a.out
```
  
## Dining Philosophers
The Dining Philosophers problem is a multi-process synchronization problem.  
The problem consists of five philosophers sitting at a table who do nothing but think and eat. Between each philosopher, there is a single fork. In order to eat, a philosopher must have both forks. A problem can arise if each philosopher grabs the fork on the right, then waits for the fork on the left. In this case a deadlock has occurred, and all philosophers will starve. Also, the philosophers should be fair. Each philosopher should be able to eat as much as the rest.

There are two versions of the Dining Philosopher problem implemented:
1. Using semaphores.
2. Using threads.  
     
work in progres...
