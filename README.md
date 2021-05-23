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
