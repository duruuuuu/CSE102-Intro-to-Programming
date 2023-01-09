#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CARB_CAL_1G 4    /* Number of calories in 1g carbohydrate */
#define PROTEIN_CAL_1G 4 /* Number of calories in 1g protein */
#define FAT_CAL_1G 9     /* Number of calories in 1g fat */
#define WATER_CAL_1G 0   /* Number of calories in 1g water */

#define MAX_CHAR 30  /* Max length for input */
#define MAX_INPUT 20 /* Max input amount */

#define CARBS_INDEX 0   /* Index for carbs value in an array */
#define PROTEIN_INDEX 1 /* Index for protein value in an array */
#define FATS_INDEX 2    /* Index for fats value in an array */

typedef struct
{
    /* This structure is used to hold the nutrient information of components */
    float carbs;
    float protein;
    float fat;
    float totalCals;
    float totalGrams;
} MacroNutrients;

void delete_newline(char *str)
{
    /*
      This function deletes the newline character from the end of the string that was taken using the function fgets() to avoid
      any confusion while sending the string to other functions as well as to delete an unnecessary newline when printing the string.
    */

    int i = 0;
    while (*str != '\n')
    {
        str++; // Using pointer arithmetic to find the newline character
        i++;   // Incrementing a counter so that we can reset the pointer back to the beggining of the string at the end.
    }

    *str = '\0';   // Setting a new end to the string
    str = str - i; // Resetting the pointer back to the beginning of the string.
}

void delete_whitespace(char str[])
{
    /* This function deletes any leading spaces in the strings to fix the output format */
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t')
        i++;

    strcpy(str, str + i); // copying the string from the first occurrence of a character omitting spaces
}

int tokenize(char b[][MAX_CHAR], char a[], char *delim)
{
    /* This function is used to parse and separate the inpit strings to be able
    to get the substrings that are needed to calculate stuff for the program */

    delete_newline(a); // deleting newline at the end of the string to avoid confusion later on
    char *token;
    /* Separating phrase into individual components and putting them into the array*/
    token = strtok(a, delim); // getting first token

    /* Getting all the other tokens in the phrase */
    int i = 0;
    while (token != NULL)
    {
        strcpy(b[i], token); // Storing the token in the array
        i++;
        token = strtok(NULL, delim);
    }

    return i; // returns the index so that it can be used in a loop
}

void get_substring(char substring[], char string[][MAX_CHAR], char *startToken, char *endToken)
{
    char *start;
    start = strchr(*string, *startToken); // Finding the Position of the firsat element of the substring
    size_t len = strlen(start + 1);       // calculating the length of the substring to get the last element of the substring
    strncpy(substring, start + 1, len);   // Copying the substring
    substring[len] = '\0';                // terminating string with null character
}

MacroNutrients get_macronutrients(char components[MAX_INPUT][MAX_CHAR])
{
    char macroInput[MAX_CHAR * MAX_INPUT], gramsChar[MAX_CHAR];
    MacroNutrients mcrs;

    get_substring(gramsChar, components, "(", "g"); // getting the substring of the gram value;
    mcrs.totalGrams = atof(gramsChar);              // Converting string of gram value to float type

    /* Getting value information of macronutrients */
    printf("List the amount amount of macro nutrients in %s:\n", *components);
    fgets(macroInput, sizeof(macroInput), stdin);

    char macrosArr[FATS_INDEX + 1][MAX_CHAR], carbsChar[MAX_CHAR], fatsChar[MAX_CHAR], proteinChar[MAX_CHAR], macrosChar[FATS_INDEX + 1][MAX_CHAR];
    tokenize(macrosArr, macroInput, ","); // Separating the string by commas

    /* Separating the macros array to get only the percent signs and values */
    get_substring(macrosChar[CARBS_INDEX], (macrosArr + CARBS_INDEX), "C", "\0");
    get_substring(macrosChar[PROTEIN_INDEX], (macrosArr + PROTEIN_INDEX), "P", "\0");
    get_substring(macrosChar[FATS_INDEX], (macrosArr + FATS_INDEX), "F", "\0");

    /* separating the strings further to get only the percent values and no
       signs or symbols to ensure correct conversion from string to float */
    get_substring(carbsChar, (macrosChar + CARBS_INDEX), "%", "\0");
    get_substring(proteinChar, (macrosChar + PROTEIN_INDEX), "%", "\0");
    get_substring(fatsChar, (macrosChar + FATS_INDEX), "%", "\0");

    /* Converting macronutrient values from string to float type */
    mcrs.carbs = atof(carbsChar);
    mcrs.protein = atof(proteinChar);
    mcrs.fat = atof(fatsChar);

    return mcrs;
}

MacroNutrients calculate_macronutrients(MacroNutrients macroCalories, MacroNutrients macroPercent)
{
    /* This functions calculates the amount of calories in a component based on the percentage of its macronutrient values */

    macroCalories.carbs += ((macroPercent.totalGrams * (macroPercent.carbs / 100)) * CARB_CAL_1G);
    macroCalories.protein += ((macroPercent.totalGrams * (macroPercent.protein / 100)) * PROTEIN_CAL_1G);
    macroCalories.fat += ((macroPercent.totalGrams * (macroPercent.fat / 100)) * FAT_CAL_1G);
    macroCalories.totalCals = (macroCalories.carbs + macroCalories.fat + macroCalories.protein);
    macroCalories.totalGrams += macroPercent.totalGrams;

    return macroCalories;
}

MacroNutrients question(char meal[], MacroNutrients macroCalories)
{
    /* GETTING USER INPUT FOR THE SECOND LINE OF QUESTIONING */
    char componentsInput[MAX_CHAR * MAX_INPUT], components[MAX_INPUT][MAX_CHAR];
    printf("What is %s?\n", meal);
    fgets(componentsInput, sizeof(componentsInput), stdin);

    int i = tokenize(components, componentsInput, ","); // Separating string by comma and getting number of inputs/substrings

    for (int k = 0; k < i; k++) // Deleting any leading whitespaces from strings for proper output format
        delete_whitespace(components[k]);

    /* Parse through the components to either calculate its macronutrient values or call the function again to get sub components */
    MacroNutrients macroPercent;
    int j;
    for (j = 0; j < i; j++)
    {
        if (strchr(components[j], '(') != NULL)
        {
            macroPercent = get_macronutrients(&components[j]);                     // getting percentages of the macronutrients
            macroCalories = calculate_macronutrients(macroCalories, macroPercent); // calculating the macronutrients based on their values
        }

        /* If the current component does not have a gram value, the function is called recursively to get its sub-objects */
        else
            macroCalories = question(components[j], macroCalories);
    }

    return macroCalories;
}

MacroNutrients calculate_percentage(MacroNutrients totalMacros)
{
    /* This function is used to calculate the percentage of macronutrients from calories of the meal */

    totalMacros.carbs = ((totalMacros.carbs / totalMacros.totalCals) * 100);
    totalMacros.protein = ((totalMacros.protein / totalMacros.totalCals) * 100);
    totalMacros.fat = ((totalMacros.fat / totalMacros.totalCals) * 100);

    return totalMacros;
}

int main()
{
    /* INITIALIZING STRUCT VALUES BEFORE USING */
    MacroNutrients totalMacros;
    totalMacros.carbs = 0.0;
    totalMacros.protein = 0.0;
    totalMacros.fat = 0.0;
    totalMacros.totalCals = 0.0;
    totalMacros.totalGrams = 0.0;

    /* GETTING USER INPUT */
    char meal[MAX_CHAR];
    printf("What is the meal?\n");
    fgets(meal, sizeof(meal), stdin); // getting input

    delete_newline(meal);    // Deleting newline at the end of the string to avoid confusion
    delete_whitespace(meal); // Deleting any leading spaces to fix output format

    totalMacros = question(meal, totalMacros);       // Asking questions about the components and getting the macronutrient values
    totalMacros = calculate_percentage(totalMacros); // Calculating percentage values to get final outputs

    printf("Total calories is %.1f in %.1fg %s. %c%.1f from carbs, %c%.1f from protein, %c%.1f from fats.\n",
           totalMacros.totalCals, totalMacros.totalGrams, meal, 37, totalMacros.carbs, 37, totalMacros.protein, 37, totalMacros.fat);

    return 0;
}
