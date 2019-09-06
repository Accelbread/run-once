# run-once

A small shell utility to run a program if it is not already running.

The utility will execute the given exeuctable if run-once has not already
been used to run that executable. It works by taking an advisory lock on the
executable file, and if sucessful, execing the program. It takes one parameter:
a path to the executable to run.

# ToDo

- Support running binaries on $PATH
- Lock files directory
- Configuration file
- Commadn parameters
