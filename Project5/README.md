/*test machine: csel-kh4250-04.cselabs.umn.edu

date: Dec.14.2021
name: Michael Allrich, Lee Knutson, Ke Wang */
purpose of the program
Utilize a message queue to have a client and server program communicate with one another. A given directory has its files partitioned among the client processes. The processes then send the addresses of their files to their respective server threads via a message queue. From there, the server acknowledges the message and determines how many words start with a given letter. Once all processes have concluded, the server returns a string of 26 integers representing how many times a word started with that letter.

how to compile
Navigate to a working directory within a terminal, copy file contents into the directory. Compile the server and client files by calling “make clean; make” within both the client and server directories. Within a split terminal containing both the client and server directories as the cwd, execute the server with ./server <#clients> before executing the client with ./client <path_to_target> <#clients>where “path_to_target” is the name of a valid relative or absolute directory and “# clients” isa positive integer less than the number of files in the target directory that the user wishes to partition. The output for each client will be displayed in a text file called client/Output/client#.txt upon completion.

what the program does
Client.c:

Createlist: Given a file path, construct a linked list with each node containing the path to a file found within the original’s directory.

Insert: Given a node containing a new path, place the node at the end of the linked list for a different client, making sure a different client is used each time.

Create_node: Given a file path, create a new node whose contents are said file path.

Fileprintlist: For each client’s input file, print out the contents of that client’s linked list (the paths that client passes) to the file.

Main: Create a series of processes (one per client) and call createlist to partition the contents of the input directory. Within each client, the process sends the path names that the client oversees to a message queue to be removed by the server. Afterwards, the client receives a message of acknowledgement from the server. Once all of the client's messages have been sent, it sends an “END” message to the server and awaits final results.

Server.c:

The server will create(in main function) input ‘s’ threads to handle the requests from ‘c’ clients delivered via the message queue. Message queue is thread safe, so multiple threads can access the queue in a mutually exclusive manner. Thread ‘i’ will handle requests from client ‘i’. Once a request, which is a file path, is extracted, the thread opens(in Handle_multiple) the file and reads it line by line. The file consists of a single word per line and increments a global integer array (size 26) maintained to keep track of alphabet letter count, based on the first letter of the word.(in Parsing)

Handle_multiple: Handle_multiple will make the message queue and receive the message from the client then try to open the ‘path’ from the message queue and call the ‘Parsing’ function to get the integer array then and then reform to the character array then send the result back. Parsing: Parsing will make sure that each character is well counted to the integer array and do the statistics of the words and send the result to the global array created by the Server.c. Main: main function creates and joines all the threads that user input and multi-threading the ‘Handle_multiple’.

Testcases:
for client
./client <path from home dir to testcase folder>/Testcases/TestCase2<F1 or F2 or ... F10> 1
./client <path from home dir to testcase folder>/Testcases/TestCase2<F1 or F2 or ... F10> 3
./client <path from home dir to testcase folder>/Testcases/TestCase2<F1 or F2 or ... F10> 9

for server with the appropriate client
./server 1
./server 3
./server 9
assumptions
- The server program will always be run before the client program.
Both the server program and the client program will be run on the same machine.
The files in the input directory of the client will have a single word per line not exceeding 50 characters and the number of words per file can vary.
The number of clients is less than the number of files. (for the test case files, clients must be less than or equal to 20).
The argument for the server and the second argument for the client, which is the number of clients, will always be the same.
