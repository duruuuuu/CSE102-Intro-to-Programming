#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define  MAX 100 

double func(char string[MAX]);

int main()
{
   double totalCost;
   char object[MAX]; //declares variable for the input object
   printf("Define the object:\n");
   scanf("%s", object); //takes string input from user 
//(i tried to use gets() but compiler kept giving an error)
    
   totalCost = func(object);

   printf("Total cost of %s is %.2lf\n", object, totalCost); //prints final result
}

double func(char string[MAX])
{
   double val; //variable for price inputs
   char str1[MAX], str2[MAX], swap1[MAX], swap2[MAX], *star, *plus;
   int i;

   star = strstr(string, "*"); //looks for the first star in the statement

   if(star == NULL && (val = atof(string)))//if the input is a price
   {
     return val; //converts into double and returns the value
   }


   else if(star != NULL) //if '*' is found
   {
    i = (int) (star - string);
    strncpy(swap1, string, i); // takes string until it reaches '*'
    val= atof(swap1);	 // converts value into double
    strcpy(swap1, star+1);// takes string after the '*'

    plus = strstr(swap1, "+"); //looks for '+'
    if(plus != NULL)//if plus is found
    {			
      i = (int) (plus - swap1); 
      strncpy(swap2, swap1, i); // takes only the name of hte object

      printf("What is %s?:\n", swap2);
      scanf("%s", str1);	 // gets input of the object part

      strcpy(str2, plus+1);

      return val*func(str1)+func(str2);
    }
    return val*func(swap1);
   }

   else //if the input is only a word
   {
     printf("What is %s?\n", string);
     scanf("%s", str1);
     return func(str1);
   }

}
