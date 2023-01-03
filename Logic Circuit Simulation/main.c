#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 20              /* Maximum length of lines in files */
#define INIT_SIZE 10            /* Initial size of arrays used for allocateing memory */
#define UNCHECKED_GATE_VAL -999 /* Flag value for un-evaluated gates */

typedef enum gate_enum
{
    /* enumerated type for assigning gates */
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

void reset_outputs(gate_t *arr, int size);
int eval(gate_t *gate);

gate_enum convert_gate_type(char *str)
{
    /* this function is used to convert the string of the gate type
       from the file to a corresponding enumerated type */

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
    /* This function is used to assign the struct element variables according to the
       information taken from the file */
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
    if (gatesArr == NULL)                                            // error checking memory allocation
    {
        // Memory allocation failed
        printf("Error: Memory allocation failed\n");
        return -1;
    }

    gate_t currGate;    // used to store the current gate in the file pointer
    gate_enum gateType; // used to get and convert the type of the gate
    int count = 0, currentSize = INIT_SIZE, inputGateCount = 0, inputCurrentSize = INIT_SIZE, inputCount = 0;
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

            if (gatesArr[to].inputs == NULL) // allocating memory only if there are no inputs in the gate yet
            {
                gatesArr[to].inputs = malloc(sizeof(gate_t) * inputCurrentSize); // allocating memory enough for 2 inputs in the pointer
                if (gatesArr[to].inputs == NULL)
                {
                    // Memory allocation failed
                    printf("Error: Memory allocation failed\n");
                    return -1;
                }
            }
            gatesArr[to].inputs[gatesArr[to].numInputs] = &gatesArr[from]; // asiging the address of the input to the inputs pointer
            gatesArr[to].numInputs += 1;                                   // keepig track of the number of inputs in the gate
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
        /* realocating memory in struct nad int arrays in case there is no space left */
        if (i == rows)
        {
            rows *= 2;
            inputsArr = (gate_t **)realloc(inputsArr, rows * sizeof(gate_t **));
        }

        /* jump to next row if file pointer reaches newline */
        if (*n == '\n')
        {
            i++;      // increasing row count
            j = 0;    // resetting coloumn count
            continue; // going to the next number in the file
        }

        /* stop the program if it readhes end of file */
        if (feof(input))
            break;

        num = atoi(n);                 // converting character to integer
        inputsArr[i][j].output = num;  // storing values in int array before stroing them in struct array
        inputsArr[i][j].inputs = NULL; // assigning inputs to null so we can finds the base case of the eval function
        j++;
    }

    /* ASSIGNING INPUTS FROM FILE TO INPUT GATES AND CALLING EVALUATION FUNCTION FOR SIMULATIONS */
    int flag = 0, simulationCount = i + 1;
    gate_t *outputGate = NULL;
    for (i = 0; i < simulationCount; i++) // going through simulations (i is also teh simulation count)
    {
        flag = 0;
        for (j = 0; j < inputGateCount; j++) // going through inputs
        {
            for (int n = flag; n < count; n++)
            {
                if (gatesArr[n].type == INPUT)
                {
                    /* connecting pointers from simulation inputs to input struct pointers */
                    gatesArr[n].inputs = malloc(sizeof(gate_t *)); // allocating memory to inputs pointer
                    if (gatesArr[n].inputs == NULL)
                    {
                        // Memory allocation failed
                        printf("Error: Memory allocation failed\n");
                        return -1;
                    }

                    *gatesArr[n].inputs = &inputsArr[i][j]; // assigning input address to inputs pointer
                    gatesArr[n].numInputs = 1;              // assigning number of inputs in gate
                    flag = n + 1;                           // marking where we left off on the input gates array
                    break;
                }
            }
        }

        /* looking for the root node aka the output gate so we can call the eval function with it */
        int g = 0;
        for (g = 0; g < count; g++)
        {
            if (gatesArr[g].type == OUTPUT)
                break;
        }
        int output = eval(&gatesArr[g]);
        printf("final output: %d\n", output);
        reset_outputs(gatesArr, count);
    }

    /* freeing allocated memory */
    free(gatesArr);
    free(inputsArr);

    /* closing files */
    fclose(circuit);
    fclose(input);

    return 0;
}

void reset_outputs(gate_t *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i].type == INPUT)
            continue;
        arr[i].output = UNCHECKED_GATE_VAL;
    }
}

int eval(gate_t *gate)
{
    int output;
    switch (gate->type)
    {

    case INPUT:
        gate->output = gate->inputs[0]->output;
        return gate->output;

    case AND:
        output = eval(gate->inputs[0]);
        for (int i = 1; i < gate->numInputs; i++)
            output = output && eval(gate->inputs[i]);

        return output;

    case OR:
        output = 0;
        for (int i = 0; i < gate->numInputs; i++)
            output = output || eval(gate->inputs[i]);

        return output;

    case NOT:
        return !eval(gate->inputs[0]);

    case FLIPFLOP:
        if (gate->inputs[0]->output == 1 && gate->inputs[0]->former_out == 0)
        {
            gate->former_out = 1;
            return 1;
        }

        else if (gate->inputs[0]->output == 0 && gate->inputs[0]->former_out == 1)
        {
            gate->former_out = 0;
            return 0;
        }

        else
        {
            return gate->former_out;
        }

    case OUTPUT:
        return eval(gate->inputs[0]);

    default:
        // Invalid gate type
        return -1;
    }
}
