## lab10

Given the skeleton of some code to compute matrix addition
○ There are both multithreaded and single threaded versions
○ Both are timed so you can contrast their performance
○ Results are compared to ensure multithreaded variant is correct
○ Complete both versions by adding appropriate pthread function calls, etc.
○ You will need to set the proper arguments in an argument struct
○ The single threaded version should operate on the entire matrix
○ The individual multithreading threads should each operate on a quadrant
○ The struct should be cast to a void* and passed into each pthread_create
○ Remember to call pthread_join for each thread created
○ Also complete the partial_matrix_add function to add the entries in the range of its arg

