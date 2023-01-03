/*
    implementing/ simulating a logic circuit

    - circuit.txt starts either with a connection or gate name
    - you can assume that the gates already exist if they are being connected

    **Read the circuit.txt file only once. Do not keep going back/ repeatedly parsing
    ** there wont be any loop connections or connections to itself
    - Represent the connections as a tree
    - Every gate has only one output but can have many inputs

    - Have structs for the definitions of the logic gates and use the structs to call the gates?
    - You have to use dynamic allocation :(
    - Use the connection information and use the pointers to connect them
    * must have pointer connection between structs

    How to solve:
    1. Create a structure for the gate
    * for inputs you can have an array of inputs

    eval(struct *gate)
    - repeatedly call evaluate function for each gate
    - start calling the evaluation func from the output and end it until the inputs
    - use recursion in this function

    - If you need to evaluate a gate twice, store a flag inside the struct saying it was laready evaluated.
    - Have a component storing the output of the gate.
    - If the gate has laalreadyreadt been evaluated just get the output.

    - THis function finds the output of the gate by checking its inputs


    Topics to study:
    - Struct
    - Dynamic Memory Allocation
    - Trees
    - Linked Lists


    2. Count the gates and then store them in a partially filled array?
*/

/* questions: is it sure that each row will contain only 3 words */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 20
#define INIT_SIZE 10
#define INIT_INPUT 2
#define UNCHECKED_GATE_VAL -999

typedef enum
{
    INPUT,
    OUTPUT,
    AND,
    OR,
    NOT,
    FLIPFLOP
} gate_enum;

struct gate_s
{
    gate_enum type;         /* type of the gate */
    char name[MAX_LEN];     /* name of the gate */
    struct gate_s **inputs; /* points to a dynamically created array of pointers which store the addresses of input gate structs */
    int numInputs;          /* stores the number of inputs a gate has */
    int output;             /* stores the current output  */
    int former_out;         /* stores former output for flipflop gate */
};
typedef struct gate_s gate_t;

void eval(gate_t *gate);
int evalOr(gate_t *gate);
int evalAnd(gate_t *gate);
int evalNot(gate_t *gate);
int evalFlipFlop(gate_t *gate);

gate_enum convert_gate_type(char *str)
{
    if (strcmp(str, "INPUT") == 0)
        return INPUT;

    else if (strcmp(str, "OUTPUT") == 0)
        return OUTPUT;

    else if (strcmp(str, "AND") == 0)
        return AND;

    if (strcmp(str, "OR") == 0)
        return OR;

    else if (strcmp(str, "NOT") == 0)
        return NOT;

    else if (strcmp(str, "FLIPFLOP") == 0)
        return FLIPFLOP;

    return 0;
}

gate_t fill_struct(gate_enum type, char name[], struct gate_s **inputs, int output, int former_out)
{
    /* This function is used to assign the struct element variables */
    gate_t s;
    s.type = type;
    strcpy(s.name, name);
    s.inputs = NULL;
    s.output = output;
    s.former_out = 0;
    s.numInputs = 0;

    return s;
}

int main()
{
    /* file opening operation adn error handling on files */
    FILE *circuit, *input;
    circuit = fopen("circuit.txt", "r");
    if (circuit == NULL)
    {
        // File could not be opened
        printf("Error: Could not open file\n");
        return -1;
    }

    input = fopen("input.txt", "r");
    if (input == NULL)
    {
        // File could not be opened
        printf("Error: Could not open file\n");
        return -1;
    }

    /* SCANNING CIRCUIT TO STORE GATES AND MAKE CONNECTIONS  */
    char keyword[MAX_LEN], input1[MAX_LEN], input2[MAX_LEN];

    gate_t *gatesArr = (gate_t *)malloc(sizeof(gate_t) * INIT_SIZE); // allocating memory to array for storing gates structs
    if (gatesArr == NULL)
    {
        // Memory allocation failed
        printf("Error: Memory allocation failed\n");
        return -1;
    }

    gate_t currGate;
    gate_enum gateType; // used to get and convert the type of the gate
    int count = 0, currentSize = INIT_SIZE, inputGateCount = 0, inputCurrentSize = INIT_INPUT, inputCount = 0;
    while (fscanf(circuit, "%s%s%s", keyword, input1, input2) != EOF)
    {
        fflush(circuit); // flushing file to make sure there is no problem while scanning the words

        /* if it is a new gate */
        if (strcmp(keyword, "GATE") == 0)
        {
            if (count == currentSize) // reallocating memory in case we reach the maximum allocated input
            {
                currentSize *= 2;
                gatesArr = (gate_t *)realloc(gatesArr, currentSize * sizeof(gate_t));
            }

            /* Storing the current gate in an array */
            gateType = convert_gate_type(input1);                                  // converting the string of the gate type to enum type
            currGate = fill_struct(gateType, input2, NULL, UNCHECKED_GATE_VAL, 0); // assigning struct elements
            gatesArr[count] = currGate;                                            // assigning struct to struct array elements
            count++;

            if (gateType == 0)
                inputGateCount++; // counting the number of INPUT gates
        }

        /* if it is a new connection */
        else if (strcmp(keyword, "CONNECTION") == 0)
        {
            int from, to;
            for (from = 0; from < count; from++) // getting the source gate
            {
                if (strcmp(gatesArr[from].name, input1) == 0)
                    break;
            }

            for (to = 0; to < count; to++) // getting the destination gate
            {
                if (strcmp(gatesArr[to].name, input2) == 0)
                    break;
            }

            gatesArr[to].inputs = malloc(sizeof(gate_t) * inputCurrentSize); // allocating memory enough for 2 inputs in the pointer
            if (gatesArr[to].inputs == NULL)
            {
                // Memory allocation failed
                printf("Error: Memory allocation failed\n");
                return -1;
            }

            *gatesArr[to].inputs = &gatesArr[from]; // asiging the address of the input to the inputs pointer
            gatesArr[to].numInputs += 1;            // keepig track of tbhe number of inputs in the gate
            inputCount++;

            if (inputCount == inputCurrentSize) // reallocating memory
            {
                inputCurrentSize *= 2;
                gatesArr[to].inputs = realloc(gatesArr[to].inputs, inputCurrentSize * sizeof(gate_t));
            }
        }
    }

    /* GETTING THE INPUT VALUES FOR THE INPUT GATES */
    int i = 0, j = 0, num;
    char *n = malloc(1); // allocating memory for character pointer
    if (n == NULL)
    {
        // Memory allocation failed
        printf("Error: Memory allocation failed\n");
        return -1;
    }

    int rows = INIT_SIZE, cols = inputGateCount;
    gate_t **inputsArr = (gate_t **)malloc(rows * sizeof(gate_t **)); // allocating memory to inputs array for input gates
    if (inputsArr == NULL)
    {
        // Memory allocation failed
        printf("Error: Memory allocation failed\n");
        return -1;
    }

    for (int k = 0; k < rows; k++) // allocating memory to arrays
    {
        inputsArr[k] = (gate_t *)malloc(cols * sizeof(gate_t *));
    }

    /* GOING THROUGH THE INPUT FILE */
    while (((*(n)) = fgetc(input)))
    {
        if (i == rows) // realocating memory in struct nad int arrays in case there is no space left
        {
            rows *= 2;
            inputsArr = (gate_t **)realloc(inputsArr, rows * sizeof(gate_t **));
        }

        if (*n == '\n') // jump to next row if file pointer reaches newline
        {
            i++;      // increasing row count
            j = 0;    // resetting coloumn count
            continue; // going to the next number in the file
        }

        if (feof(input)) // stop the program if it readhes end of file
            break;

        num = atoi(n);                // converting character to integer
        inputsArr[i][j].output = num; // storing values in int array before stroing them in struct array
        j++;
    }
    i++;

    /* assigning the int binary values ot the input gate structure as well as the other values */
    printf("gate: %s, numInputs: %d\n", gatesArr[2].name, gatesArr[2].numInputs);
    /* ASSIGNING INPUTS FROM FILE TO INPUT GATES AND CALLING EVALUATION FUNCTION FOR SIMULATIONS */
    int flag = 0;
    gate_t *outputGate = NULL;
    for (int k = 0; k < i; k++) // going through simulations
    {
        flag = 0;
        for (int m = 0; m < inputGateCount; m++) // going through inputs
        {
            for (int n = flag; n < i; n++)
            {
                if (gatesArr[n].type == INPUT)
                {
                    /* connecting pointers from simulation inputs to input struct pointers */
                    gatesArr[n].inputs = malloc(sizeof(int *)); // allocating memory to inputs pointer
                    if (gatesArr[n].inputs == NULL)
                    {
                        // Memory allocation failed
                        printf("Error: Memory allocation failed\n");
                        return -1;
                    }
                    *gatesArr[n].inputs = &inputsArr[k][m];      // assigning input address to inputs pointer
                    gatesArr[n].output = inputsArr[k][m].output; // assigning the output to the gate aswell
                    printf("%s:%d ", gatesArr[n].name, gatesArr[n].output);
                    flag = n + 1; // marking where we left off on the input gates array
                    break;
                }
            }
        }

        int g = 0;
        for (g = 0; g < count; g++) // looking for the output gate aka root node so we can call the eval function
        {
            if (gatesArr[g].type == OUTPUT)
            {
                // outputGate = &gatesArr[n];
                break;
            }
        }
        // printf("output index: %d\n", g);
        printf("output: %d\n", gatesArr[g].output);
        eval(&gatesArr[g]);
    }

    /* freeing allocated memory */
    free(gatesArr);
    free(inputsArr);
    /* closing files */
    fclose(circuit);
    fclose(input);

    return 0;
}

int evalAnd(gate_t *gate)
{
    int result = gate->inputs[0]->output;
    for (int i = 1; i < gate->numInputs; i++)
    {
        result = result && gate->inputs[i]->output;
    }
    return result;
}

int evalOr(gate_t *gate)
{
    int result = gate->inputs[0]->output;
    int numInputs = sizeof(gate->inputs) / sizeof(gate->inputs[0]);
    for (int i = 1; i < gate->numInputs; i++)
    {
        result = result || gate->inputs[i]->output;
    }
    return result;
}

int evalNot(gate_t *gate)
{
    return !gate->inputs[0]->output;
}

void eval(gate_t *gate)
{
    //  If this gate has already been evaluated, return its output
    if (gate->output != UNCHECKED_GATE_VAL)
        return;

    // Evaluate all input gates first
    if (gate->inputs != NULL)
    {
        for (int i = 0; i < gate->numInputs; i++)
        {
            eval(gate->inputs[i]);
        }
    }

    // Calculate the output of this gate based on its type and inputs
    switch (gate->type)
    {
    case INPUT:
        // Input gates don't have any inputs, so their output is already set
        break;

    case OUTPUT:
        // Output gates don't have any outputs, so their output is the same as their input
        eval(gate->inputs[0]);
        gate->output = gate->inputs[0]->output;
        break;

    case AND:
        gate->output = evalAnd(gate);
        break;

    case OR:
        gate->output = evalOr(gate);
        break;

    case NOT:
        gate->output = evalNot(gate);
        break;

    case FLIPFLOP:
        // Implement the logic for a flip-flop gate here
        break;
    }
}