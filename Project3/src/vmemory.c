//Implement the API modeling the translation of virtual page address to a
//physical frame address. We assume a 32 bit virtual memory and physical memory.
//Access to the page table is only via the CR3 register.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vmemory.h"
#include <unistd.h>

#define OUT_TLB "../bin/tlb_out.txt"

bool FIFO_policy = true;
int **cr3;


//
// You can declare global variables necessary for your TLB implementation here
//

float HIT = 0;
float MISS = 0;
float TOTAL = 0;

//20 bit virtual address, physical frame base address
int myTLB[8][2] = {
    {-1,-1}, {-1,-1}, {-1,-1}, {-1,-1}, {-1,-1}, {-1,-1}, {-1,-1}, {-1,-1}
};

//
// More static helper functions can be implemented here
//



// The implementation of get_vpage_cr3 is provided in 
// an object file, so no need to re-implement it
void initialize_vmanager(int policy) 
{
	// Set LFU policy when passed as a parameter
	if (policy)
		FIFO_policy = false;
	// Set base pointer to page table
	cr3 = get_vpage_cr3();
	//printf("cr3: %p\n", cr3);
	
	// You can add other initialization here as necessary
}

//
// The implementation of the following functions is required
//
int translate_virtual_address(unsigned int v_addr)
{
	//TODO

	int ind1 = 0;
	int ind2 = 0;
	int offset =0;
	int physframe;
	int index;

	//grabbing the indices and offset
    ind1 = v_addr / (4194304);
    ind2 = (v_addr % (4194304)) / (4096);
	offset = v_addr % (4096);
	index = v_addr / (4096);

	//printf("v_addr is: %x ", v_addr);
	//printf("index is: %x", index);
	//printf("one, %x two, %x offset: %x\n", ind1, ind2, offset);
	
	//increase total for evey virtual address checked
	TOTAL++;

	if (get_tlb_entry(index) != -1) {
		physframe = get_tlb_entry(index);
		//printf("got the tlb entry\n");
		//printf("printing: ");
		print_physical_address(physframe, offset);
		int p_add = physframe * 0x1000 + offset;
		//populate_tlb();
		HIT += 1;
        return p_add;
	}
	
	//logic for getting the physical frame address
	if(cr3[ind1] == NULL) {
		//printf("the first frame was NULL\n");
		//printf("pagefault\n");	
		print_physical_address(-1, offset);
		return(-1);
	}
	else {
		physframe = cr3[ind1][ind2];
	}
	
	//logic for printing the physical address
	if (physframe == -1 ) {
		//printf("pagefault\n");
		print_physical_address(-1, offset);
		return(-1);
	}
	else {
		//printf("printing: ");
		print_physical_address(physframe, offset);
		int p_add = physframe * 0x1000 + offset;
		populate_tlb(index, physframe);
        return p_add;
		//return(0);
	}
	//TOTAL++;

	return 0;
}

void print_physical_address(int frame, int offset)
{
	//TODO
	if (frame == -1) {
		//invalid frame pagefault
		printf("-1\n");
	}
	else {
		//valid frame
		/*
		int physaddr = frame * 0x1000;
		physaddr += offset;
		printf("0x%x \n", frame);*/
		printf("0x%x%03x \n", frame, offset);
	}
	return;
}

int get_tlb_entry(int n)
{
	//n is first 20 bits
	//TODO
	for(int i = 0; i < 8; i++){
        /*if(myTLB[i][0] == -1){
			//printf("the mytlb[i][0] is empty");
            //break;
        }*/
		//int index = myTLB[i][0] / (4096);
        if(myTLB[i][0] == n){
            if(myTLB[i][1] == -1){
                MISS += 1;
                return -1;
            }
            else{
				//printf("hit\n");
				//HIT += 1;
                return myTLB[i][1];

            }
        }
    }
	//printf("miss\n");
    MISS += 1;
	return -1;
}

void populate_tlb(int v_addr, int p_addr) 
{
	//TODO
	//Check to see if there is an empty entry. If so, fill it with the new addresses and return.
    for(int i = 0; i < 8; i++){
        if(myTLB[i][0] == -1){
            myTLB[i][0] = v_addr;
            myTLB[i][1] = p_addr;
			//printf("new tlb entry v_addr: %x, p_addr: %x\n", v_addr, p_addr);
			//printf("the tlb entry [%d]: %x, [1]: %x\n",i, myTLB[i][0],myTLB[i][1]);
            return;
        }
    }
    //There is no empty entry. Move all existing entries over and fill in the new entry last.
    for(int j = 0; j < 8; j++){
        //myTLB[j] = myTLB[j+1];
		myTLB[j][0] = myTLB[j+1][0];
        myTLB[j][1] = myTLB[j+1][1];
    }
    myTLB[7][0] = v_addr;
    myTLB[7][1] = p_addr;
    
	return;
}

float get_hit_ratio()
{
	//TODO
	if (TOTAL != 0) {
		return (HIT / TOTAL);
	}
	else {
		printf("division by 0");
		return 0;
	}
}

// Write to the file denoted by OUT_TLB
void print_tlb()
{
	//TODO

	//printf("called printtlb()\n");

	FILE *output;

	output = fopen(OUT_TLB, "a");

	if(!output) {
		perror("no output file\n");
		exit(1);
	}

	printf("tlb:\n");

    for(int i = 0; i < 8; i++) {
        if(myTLB[i][0] == -1){
            printf("-1 -1\n");
            fprintf(output, "-1 -1\n");
        }
        else{
            printf("%x %x\n", myTLB[i][0], myTLB[i][1]);
            fprintf(output, "%x %x\n", myTLB[i][0], myTLB[i][1]);
        }
    }

    //fputs("\n", output);
	printf("\n");
    fclose(output);

	return;
}
