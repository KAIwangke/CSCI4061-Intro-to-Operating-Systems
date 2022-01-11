/*test machine: csel-kh1250-04.cselabs.umn.edu
* date: 10/21/21
*/

### purpose of the program
    Mimic a unix shell, with rewritten ls, cd, and wc functions.

### how to compile
     Navigate to a working directory within a terminal, copy file contents into the directory. Then execute the makefile with `make all`. Then execute the program with `./shell'. From there, the user can execute given commands, pipe outputs, etc. Enter "exit" to exit the shell program and return to the regular system terminal shell. 

### what the program does
     Mimic a unix shell in which a user can input and execute commands and send results either to a text file, another command, or simply print to screen (default).
     
### functions
    SHELL:
        main
            Runs a while loop that continues until the exit command is called. For every other input, call basecommand on the input in order to execute it
        basecommand
            Handles the bulk of the work for shell. Determines whether an input is one command or multiple, then executes the commands and sends the outputs to the requested locations
        pid1func
            Helper function for basecommand. Writes the output of the first command's execution into the pipe to be sent to the second command.
        pid2func
            Helper function for basecommand. Sends the contents of the pipe to STDIN so the second function may call on it.
            
    LS:
        main
            Determines whether the call is recursive and if a path is given, then calls ls based on these parameters
        ls
            Prints out every file in a given directory or the current directory if none is given. If ls -R is called, recursively calls ls on every folder present in the directory.
    CD:
        main
            Prints an error if a path is not provided. Otherwise, calls cd on the given path.
        cd
            Changes the current working directory to the desired directory.
    WC:
        main
            Determines the mode and path specified by the input, then calls wc based on those parameters.
        wc
            Prints the number of lines, words, and characters present in the input depending on the mode.
            
### Test Cases: 
    ls 
    ls  -R
    wc cd.c (or any file in the directory really)
    wc -l cd.c
    wc -w cd.c
    wc  -c cd.c
    cd .
    cd ..
    cd test
    ls > out.txt
    ls >> out.txt
    ls | wc > out.txt
    ls | wc | wc > out.txt
    /bin/echo hello > out.txt
    /bin/echo some separated words >> out.txt
