#include <stdio.h>
#include <stdlib.h>

#define STACK_BLOCK_SIZE 10
#define EMPTY 0

// struct for pole stacks
typedef struct Stack{
    
    int *array;
    int currentsize;
    int maxSize;
} Stack;

// returns 1 if initialization is succesful
int init(Stack *s)
{
    if(s != NULL)
        return 1;

    else
        return 0;
}

// initializes an empty stack
Stack* init_return(void)
{
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    
    if(init(stack) == 1)
        stack->array = (int*)calloc(STACK_BLOCK_SIZE,sizeof(int));
    
    return stack;
}

// declaring functions
int   pop(Stack* stack);
int   push(Stack *stack, int item);
int   size(Stack *pole);
int   power(int base, int exp);
void  pop_and_push(Stack *poleA, Stack *poleB, int pole_A_num, int pole_B_num, int move);
void  checkPush(int push_return);


int main()
{
    // taking number of disks from user
    int numOfDisks;
    printf("Enter Tower Size: ");
    scanf("%d", &numOfDisks);

    // calculating the min no. of moves to solve puzzle
    int moves;
    moves = ( (power(2, numOfDisks) - 1) );
    
    // creating stacks and initializing them to act as poles to hold the disks
    Stack *pole1, *pole2, *pole3;
    
    pole1 = init_return();
    pole2 = init_return();
    pole3 = init_return();
   
    pole1->currentsize = 0;
    pole2->currentsize = 0;
    pole3->currentsize = 0;
   
    pole1->maxSize = numOfDisks;
    pole2->maxSize = numOfDisks;
    pole3->maxSize = numOfDisks;
    
    
    // stacking disks to pole 1
    int i=numOfDisks, pushReturn=1;
    while(i >= 1)
    {
        pushReturn = push(pole1, i);
        checkPush(pushReturn);
        i--;
    }
    
    
    // solving the puzzle
    for(int j=1; j<=moves; j++)
    {
        if(numOfDisks % 2 == 1) // solving puzzle for odd number of disks
        {
            if(j % 3 == 1) // legal moves between poles 1 and 3
            {
                // if pole 3 if empty
                if(pole3->currentsize == EMPTY)
                    pop_and_push(pole3, pole1, 3, 1, j);
                
                // if pole 1 is empty
                else if(pole1->currentsize == EMPTY)
                    pop_and_push(pole1, pole3, 1, 3,j);
                
                // if disk on pole 1 is larger than disk on pole 3
                else if(size(pole1) > size(pole3))
                    pop_and_push(pole1, pole3, 1, 3,j);
                
                // if disk on pole 3 is lasrger than disk on pole 1
                else if(size(pole3) > size(pole1))
                    pop_and_push(pole3, pole1, 3, 1,j);
            }
            
            else if(j % 3 == 2) // legal moves between poles 1 and 2
            {
                if(pole2->currentsize == EMPTY)
                    pop_and_push(pole2, pole1, 2, 1,j);
                
                else if(pole1->currentsize == EMPTY)
                    pop_and_push(pole1, pole2, 1, 2,j);
                
                else if(size(pole2) > size(pole1))
                    pop_and_push(pole2, pole1, 2, 1,j);
                
                else if(size(pole1) > size(pole2))
                    pop_and_push(pole1, pole2, 1, 2,j);
            }
            
            else if(j % 3 == 0)// legal moves between poles 2 and 3
            {
                if(pole2->currentsize == EMPTY)
                    pop_and_push(pole2, pole3, 2, 3,j);
                
                else if(pole3->currentsize == EMPTY)
                    pop_and_push(pole3, pole2, 3, 2,j);
                    
                else if(size(pole2) > size(pole3))
                    pop_and_push(pole2, pole3, 2, 3,j);
                    
                else if(size(pole3) > size(pole2))
                    pop_and_push(pole3, pole2, 3, 2,j);
            }
        } // end of odd numbered disks statements
        
        
        if(numOfDisks % 2 == 0) // if the number of disks are even
        {
            if(j % 3 == 1) // legal moves between poles 1 and 2
            {
                if(pole2->currentsize == EMPTY)
                    pop_and_push(pole2, pole1, 2, 1,j);
                
                else if(pole1->currentsize == EMPTY)
                    pop_and_push(pole1, pole2, 1, 2,j);
                
                else if(size(pole1) > size(pole2))
                    pop_and_push(pole1, pole2, 1, 2,j);
                
                else if(size(pole2) > size(pole1))
                    pop_and_push(pole2, pole1, 2, 1,j);
            }
            
            else if(j % 3 == 2) // legal moves between poles 1 and 3
            {
                if(pole3->currentsize == EMPTY)
                    pop_and_push(pole3, pole1, 3, 1,j);
                
                else if(pole1->currentsize == EMPTY)
                    pop_and_push(pole1, pole3, 1, 3,j);
                
                else if(size(pole3) > size(pole1))
                    pop_and_push(pole3, pole1, 3, 1,j);
                
                else if(size(pole1) > size(pole3))
                    pop_and_push(pole1, pole3, 1, 3,j);
            }
            
            else if(j % 3 == 0) // legal moves between poles 2 and 3
            {
                if(pole2->currentsize == EMPTY)
                    pop_and_push(pole2, pole3, 2, 3,j);
                
                else if(pole3->currentsize == EMPTY)
                    pop_and_push(pole3, pole2, 3, 2,j);
                
                else if(size(pole2) > size(pole3))
                    pop_and_push(pole2, pole3, 2, 3,j);
                
                else if(size(pole3) > size(pole2))
                    pop_and_push(pole3, pole2, 3, 2,j);
            }
        } // end of even numbered disks statements
    }
    return 0;
} /* END OF MAIN FUNCTION */


// function to select and remove element from stack, decrements size by ONE
int pop(Stack *stack)
{
    int popped=0;
    if (stack->currentsize > 0)
    {
        popped = stack->array[stack->currentsize-1];
        
        // allocating memory for and copying stack to temp array
        int *temp = (int*)malloc(sizeof(int)*stack->currentsize);
        for(int i=0; i < stack->currentsize; i++)
            temp[i] = stack->array[i];
        
        // decreasing array size, allocating memory of new size, transferring values from temp array to stack array
        stack->currentsize--;
        stack->array = (int*)malloc(sizeof(int)*stack->currentsize);
        
        for(int i=0; i < stack->currentsize; i++)
            stack->array[i] = temp[i];
        
        free(temp);
    }
    return popped;
} /* END OF POP FUNCTION */


// function to add element to stack, increments size by ONE
int push(Stack *stack, int item)
{
    // allocating memory and copying elements to temp array
    int *temp = (int*)malloc(sizeof(int)*stack->currentsize);
    for(int i=0; i < stack->currentsize; i++)
        temp[i] = stack->array[i];
    
    // incrementing array and allocating memory of new size, transferring temp elements
    stack->currentsize++;
    stack->array = (int*)malloc(sizeof(int)*stack->currentsize);
    for(int i=0; i < stack->currentsize-1; i++)
        stack->array[i] = temp[i];
    
    // adding pushed element onto top if stack
    stack->array[stack->currentsize-1] = item;
    
    free(temp);
    return 1;
} /* END OF PUSH FUNCTION */


// function to check if push function is succesful
void checkPush(int push_return)
{
    if(push_return == 0) 
    	printf("\nerror\n");
} /* END OF CHECK PUSH FUNCTION */


// function for popping and pushing elements between legal moves
void pop_and_push(Stack *toPole, Stack *fromPole, int toPoleNum, int fromPoleNum, int move)
{
    int disk, pushReturn;
    
    disk = pop(fromPole);
    pushReturn = push(toPole, disk);
    checkPush(pushReturn);
    
    printf("%d. Move the disk %d from '%d' to '%d'\n", move, disk, fromPoleNum, toPoleNum);
} /* END OF POP AND PUSH FUNCTION */


// returns current size of pole
int size(Stack *pole)
{
    return pole->array[pole->currentsize-1];
} /* END OF SIZE FUNCTION */


// power function used to calculate minimum num of moves
// ( i didnt want to use math.h just in case )
int power(int base, int exp)
{
    if (exp == 0)
        return 1;
    else if (exp%2 == 0)
        return power(base, exp/2)*power(base, exp/2);
    else
        return base*power(base, exp/2)*power(base, exp/2);
} /* END OF POWER FUNCTION*/
