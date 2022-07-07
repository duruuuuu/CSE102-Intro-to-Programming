#include <stdio.h>
#include <stdlib.h>
#include "util.h"
//	xs 		= first element
//	*seq	= array of sequence
//	*ls		=
//	*le		=
void generate_sequence(int xs, int currentlen, int seqlen, int *seq)
{
		seq[currentlen] = xs; //fills the sequence array with the numbers
		
		if(xs%2 == 1) //if element is odd
		{
			xs = (xs*3) + 1;
			
			if(currentlen+1 < seqlen)
				generate_sequence(xs, currentlen+1, seqlen, seq);
		}
		
		else //if element is even
		{
			xs = xs/2;
			
			if(currentlen+1 < seqlen)
				generate_sequence(xs, currentlen+1, seqlen, seq);
		}
} /* END OF GENERATE SEQUENCE FUNCTION */

void check_loop_iterative(void (*generate_sequence)(int , int currentlen, int , int *seq), int xs, int seqlen, int *loop, int *looplen)
{
	int currentLen = 0, seq[seqlen+1], ls, le;
	
	
	//generating sequence
	generate_sequence(xs, currentLen, seqlen, seq);
	
	//checking for loops
	if(*looplen >= 2) //loop length must be greater than 1 in order to be valid
	{
		printf("Checking if there is a loop of length %d...\n", *looplen);
		
		if(has_loop(seq, seqlen, *looplen, &ls, &le) == 0)
		//if there is no loop, recursively call the check loop function
		{
			*looplen -= 1;
			check_loop_iterative(generate_sequence, xs, seqlen, loop, looplen);
		}
		
		else
		{
			printf("\n\nLoop detected with a length of %d.\n", *looplen);
		    
		    //fill and print loop array with loop elements
			for(int i=0; i < *looplen; i++)
				loop[i] = seq[ls+i];
			
			printf("The indexes of the loop's first occurence: %d (first digit) %d (last digit) \n" , ls,le);
         	printf("\nLoop: {" );
          	for(int i =0 ;i<*looplen;i++)
           	 	printf("%d, ",loop[i]);
			printf("\b\b}\n");
			return;
		}
	}
	
	else //if there is no loop or loop value is invalid
	{
		*looplen = 0;
		printf("No loop found.\n");
	}
		
	
	return;
}/* END OF CHECK LOOP ITERATIVE FUNCTION */

int has_loop(int *arr, int n, int looplen, int *ls, int *le)
{
	int index,
		count = 0;
	
	for(int x=0; x < n; x++)
	{
		for(int y=0; y <= x; y++)
		{
			if(arr[y+looplen] == arr[y])
			{
				count++;
				index = y;
			}
		}
		
		if(count == looplen)//if a loop is found
		{
			*ls = (index  - looplen )+1;
			*le = index;
			
			return 1;
		}
		count = 0;//initializing count back to 0
	}
	//if there is no loop
	return 0;
}/* END OF HAS LOOP FUNCTION */

void hist_of_firstdigits(void (*generate_sequence)(int , int , int , int *), int xs, int seqlen, int *h, int digit)
{
	int *seq = (int*)calloc(seqlen,sizeof(int));
	int i = digit;

	generate_sequence(xs, 0, seqlen, seq);
		
	if(digit >= seqlen)
		return;
		
	else
	{
		while(seq[i] >= 10 )
			seq[i] /= 10;
		
		if(seq[i] == 1)
			h[0]++;
		
		if(seq[i] == 2)
			h[1]++;
		
		if(seq[i] == 3)
			h[2]++;
		
		if(seq[i] == 4)
			h[3]++;
		
		if(seq[i] == 5)
			h[4]++;
		
		if(seq[i] == 6)
			h[5]++;
		
		if(seq[i] == 7)
			h[6]++;
		
		if(seq[i] == 8)
			h[7]++;
		
		if(seq[i] == 9)
			h[8]++;   

		hist_of_firstdigits(generate_sequence, xs, seqlen, h, digit+1); 
		}
} /* END OF HISTOGRAM OF FIRST DIGITS FUNCTION */

void print_sequence( void (*generate_sequence)(int xs, int currentlen, int , int *), int seqlen, int *seq )
{
	//seq = (int*)malloc(sizeof(int)*seqlen);
	printf("\nSequence: {");
	for(int i=0; i<seqlen; i++)
	{
		printf("%d, ", seq[i]);
	}
	printf("\b\b}\n\n");
} /* END OF PRINT SEQUENCE FUNCTION */
