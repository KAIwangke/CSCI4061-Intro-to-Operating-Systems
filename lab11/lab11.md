## lab11

Exercise:
○ Given some code containing 100 threads and race conditions
○ Add condition variables and mutexes to the code so that it is correct
○ Two critical sections to worry about, marked by TODO
○ 1. a producer function run by 50 threads to add items to a buffer
○ 2. a consumer function run by 50 threads to remove elements from the buffer
○ What might go wrong here without any synchronization?
○ Note that the buffer has a built in mutex/condition variable for you to use
○ See the definition of struct condBuffer for details of how they are stored
○ Expected result if correctly synchronized: Total of buffer = 1275

