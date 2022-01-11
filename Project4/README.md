/*test machine: csel-kh1250-04.cselabs.umn.edu
* date: 11/07/21
* name: Michael Allrich, Lee Knutson, Ke Wang
*/

### Purpose of the Program

Create a linked list based on the contents of a file using concurrently running threads. The threads create nodes featuring the line number and character content of a given line before inserting it into the list and assigning it a sequence number based on when it was added. Afterward, the list is traversed, printing the sequence number, line number, and content of each node.

### how to compile

Navigate to a working directory within a terminal, copy file contents into the directory. Then execute the makefile with `make`. Execute the program with `./tlist <text_file> <#_threads>`, where “text file” is the name of a valid txt file and “# threads” is the number of threads (between 1 and 16) that the user wishes to work on the file. The output will be displayed to the terminal upon completion.

### what the program does
	
Read_line: Takes in a filename and reads a line from it.

Traversal: Traverses the linked list and prints the sequence number, line number, and content of each node.

Insert: Inserts a node into the linked list based on the line number of the node. Nodes are inserted in ascending order. Assigns the node’s sequence number and adds it to the sequence counter.

Create_node: Allocates the memory for and creates a new node with the given line number and content.

Process_file: Locks the line before calling readline, unlocks the line, locks the list, and adds the line to the linked list.
   
### assumptions  

Each line was at most 256 chars long.
    

### test cases
./tlist test.txt 1
./tlist test.txt 3
./tlist test.txt 10
//empty file testing
./tlist test2.txt 1
./tlist test2.txt 3
In order to verify that the threads were running concurrently, a combination of rand and sleep 
was used to check for different sequence and line numbers. This is placed in between the pthread_mutex_unlock(&line_lock),
and pthread_mutex_lock(&list_lock), in lines 52-57. this section will need to be commented or recommented to function.


