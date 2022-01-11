//main.c program to translate the virtual addresses from the virtual.txt
//into the physical frame address. Project 3 - CSci 4061
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "vmemory.h"

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define INPUT_FILE "../bin/virtual.txt"

int main(int argc, char* argv[])
{
	if (argc > 2) {
		printf("Too many arguments, enter up to one argument\n");
		exit(-1);
	}

	int policy = (argc == 2) ? 1:0;
	initialize_vmanager(policy);

	//TODO: Translate all of the virtual addresses in INPUT_FILE
	//	Print the corresponding physical addresses line by line
	//	Make sure to populate the TLB as you go!
	//	Output the TLB at the start and end along with the hit rate
	
	FILE *input;

	input = fopen(INPUT_FILE, "r");
	int v_addr;
	
	/*
	while(fscanf(input, "%x", &v_addr) != EOF) {
		int p_addr = translate_virtual_address(v_addr);
		//populate_tlb(v_addr, p_addr);
	}*/

	print_tlb();
	
	while(fscanf(input, "%x", &v_addr) != EOF) {
        int p_addr = translate_virtual_address(v_addr);
		//printf("v_addr %x", v_addr);
        //populate_tlb(v_addr, p_addr);
    }
	
	printf("\n");
	print_tlb();

	printf("Hit ratio: ");
	float hit = get_hit_ratio();
	printf("%f\n", hit);

	//print("%f", HIT);
	//print("%f", TOTAL);

	
	//Free the page table
	free_resources();
	return 0;
}
