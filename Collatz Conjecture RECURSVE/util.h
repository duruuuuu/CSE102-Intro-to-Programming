#ifndef _UTIL_H_
#define _UTIL_H_

	void generate_sequence(int xs, int currentlen, int seqlen, int *seq);

	void check_loop_iterative(void (*generate_sequence)(int , int currentlen, int , int *seq), int xs, int seqlen, int *loop, int *looplen);

	int has_loop(int *arr, int n, int looplen, int *ls, int *le);

	void hist_of_firstdigits(void (*generate_sequence)(int , int , int , int *), int xs, int seqlen, int *h, int digit);
	
	void print_sequence( void (*generate_sequence)(int xs, int currentlen, int , int *), int seqlen, int *seq );
	
	void print_histogram(void (*hist_of_firstdigits)( void (*generate_sequence)(int xs, int currentlen, int seqlen, int *seq), int xs, int seqlen, int , int digit), int *h );

#endif











































