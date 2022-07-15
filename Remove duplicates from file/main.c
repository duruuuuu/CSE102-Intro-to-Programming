#include<stdio.h>
int findDigitCount(int n);
int main()
{
    FILE *fptr;
    FILE *compfp;

    //file operations
    fptr = fopen("file.txt", "r+"); 
    if(fptr == NULL)
    {
        printf("Error Opening File.\n");
        return -1;
    }
    compfp = fopen("file.txt", "r");

    int numOfDigit, currNum;
    int pos1, pos2;
    while(fscanf(fptr, "%d", &currNum) != EOF)
    {
        numOfDigit = findDigitCount(currNum);
        pos1 = ftell(fptr);
        
        int compNum;
        while(fscanf(compfp, "%d", &compNum) != EOF)
        {
            pos2 = ftell(compfp);
            if(compNum == currNum && pos2 < pos1)
            {
                if(currNum > 0)
                {
                    fseek(fptr, -numOfDigit, SEEK_CUR);
                    for(int i=0; i<numOfDigit; i++)
                        fputs(" ", fptr);
                }

                else if(currNum < 0)
                {
                    numOfDigit += 1;
                    fseek(fptr, -numOfDigit, SEEK_CUR);
                    for(int i=0; i<numOfDigit; i++)
                        fputs(" ", fptr); 
                }
            }
        }
        rewind(compfp);
    }
    
    
    fclose(fptr);
    fclose(compfp);
  
    return 0;
}

//function to get how many digits is in a number
int findDigitCount(int n)
{
    int i=0; 
    while(n!=0){  
        n=n/10;  
        i++;  
    } 
    return i; 
}
