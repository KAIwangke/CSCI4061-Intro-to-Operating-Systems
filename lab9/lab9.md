## lab9

### translate virtual addresses to physical addresses

translate virtual address to 
○ Given an implementation of a 3-level page table as an array of pointers (to other arrays)
○ Write a function to translate virtual addresses to physical addresses using the page table
○ Important points about the tables/memory for this lab:
○ Physical addresses use the 30 least significant bits in an int
○ Pages are 256 bytes
○ How many bits should the offset be?
○ The first level page table has 1024 entries
○ The second and third level page tables have 64 entries each
○ How many bits should be used for each index?
○ Idea: Decompose the input address, index into the tables, then return the PPN + offset
○ Important: Remember to concatenate the PPN and offset, NOT just add them!