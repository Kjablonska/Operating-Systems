# Operating-Systems

## Task 1
The aim of this project was to implement a Shell script modifying filenames.
It contains two files:
- modify.sh - implementing the task.
- modify_exmaples.sh - script for testing purposes.

## Task 1 - functionalities
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
