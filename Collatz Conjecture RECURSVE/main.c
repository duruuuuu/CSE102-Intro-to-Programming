#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int main()
{
	int seqLen, firstElem;
	int *histogram = (int*)calloc(9,sizeof(int));
	
	//getting user input
	printf("Please enter the sequence length: ");
	scanf("%d", &seqLen);
	printf("Please enter the first element: ");
	scanf("%d", &firstElem);
	int currentLen = 0;
	
	//allocating space for sequence pointer
	int *seq = (int*)calloc(seqLen,sizeof(int));
	
	//generating sequence
	generate_sequence(firstElem, currentLen, seqLen, seq);
	
	//printing sequence
	print_sequence(generate_sequence, seqLen, seq);
	
	//checking loop in sequence
	int loopLen = seqLen/2, *loop;
	loop = (int*)malloc(sizeof(int)*loopLen); //allocating space for loop
	check_loop_iterative(generate_sequence, firstElem, seqLen, loop, &loopLen);
	
	//get and print histogram
	hist_of_firstdigits(generate_sequence, firstElem, seqLen, histogram, 0);
	printf("\nHistogram of the sequence: {");
	for(int i = 0; i < 9; i++)
		printf("%d, ", histogram[i]);
	printf("\b\b}\n");
	
	return 0;
}
