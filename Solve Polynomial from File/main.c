#include<stdio.h>
#include<math.h>

int main()
{
    printf("\n////////////////////////////\n");
    int x = 5;
    FILE *val;
    FILE *polynomial;
    FILE *eval;

    val = fopen("values.txt", "r");
    polynomial = fopen("polynomial.txt", "r");
    eval = fopen("evaluations.txt", "w");

    //getting entire polynomial to an array and getting amount of chars
    char poly[1000];
    char c=fgetc(polynomial); int polyIndex=0;
    while(c != EOF)
    {
        poly[polyIndex] = c;
        c=fgetc(polynomial);
        polyIndex++;
    }

    //gets the amount of expressions
    int count=0;
    for(int z=0; z<polyIndex; z++)
    {
        if(poly[z] == '+' || poly[z] == '-')
            count++;
    }
    if(poly[0] != '-')
        count++;
    int count2 = count-1; int z=0;
    //printf("expression count = %d ; count2 = %d\n", count, count2);
    char tempC[100], tempP[100];
    double coeff, power, coefficients[100], powers[100];
    int countParse=0, pow_index, x_index;

    for(int i=polyIndex; i >= 0; i--) //going through whole expression
    {
        if(poly[i] == '+' || poly[i] == '-' || i == 0)
        {
            int subIndex = i;
            //printf("\n\n %d = %c\n", subIndex, poly[subIndex]);
           
            for(int j=0; j<=countParse; j++, subIndex++)
            {
                //printf("hello%d: %c\n", subIndex, poly[subIndex]);

                if(poly[subIndex] == 'x')
                {
                    x_index = subIndex;
                    //printf("x: %d\n", x_index);
                }

                if(poly[subIndex] == '^')
                {
                    pow_index = subIndex;
                    //printf("pow: %d\n", pow_index);
                }
            }
            //printf("checkpoint\n");
            int lastIndex = subIndex;
            //printf("last index: %d\n", lastIndex);
            subIndex = i;
            //printf("sub index: %d\n", subIndex);
            for(subIndex=i; subIndex<lastIndex;subIndex++)//going through coefficients
            {
                if(poly[x_index-1] == '+' || poly[x_index-1] == '-')
                {
                    tempC[0] = poly[x_index-1];
                    tempC[1] = '1';
                }

                else if(x_index == 0)
                    tempC[0] = '1';

                else
                {
                    if(subIndex < x_index)
                    {
                        tempC[z] = poly[subIndex];
                        //printf("%d : tempC: %c\n", z, tempC[z]);
                        z++;
                    }
                }
            }
            z=0;
            //printf("\ncheckpoint\n");
            sscanf(tempC, "%lf", &coeff);
            coefficients[count2] = coeff;
            
            for(subIndex=i; subIndex<lastIndex; subIndex++)//going through powers
            {
                if(subIndex > pow_index)
                {
                    if(poly[x_index+1] == '^')
                        tempP[z] = poly[subIndex];
                        //printf("%d : tempP : %c\n", z, tempP[z]);
                        z++;
                }
            }
            z=0;
            if(poly[x_index+1] == '^')
            {
                sscanf(tempP, "%lf", &power);
                powers[count2] = power;
            }
            else if(poly[x_index+1] != '^')
                powers[count2] = 1;

            //printf("coeff : %d : %.2lf\n", count2, coefficients[count2]);
            //printf("pow : %d : %.1lf\n", count2, powers[count2]);
            
            
            count2--; //index for coefficient and power
            countParse = 0; //resetting
            //printf("\n");

            for(int r=0; r<100; r++)//emptying temp arrays
            {
                tempP[r] = 0;
                tempC[r] = 0;
            }
        }
        countParse++;
    }

    double ans=0, value;
    /*for(int r=0; r<count; r++)
    {
        printf("%d : %.1lf : %.1lf\n", r,coefficients[r], powers[r]);
    }*/
    printf("\n");
    for(int h=0; h<polyIndex;h++)
    {
        printf("%c", poly[h]);
    }
printf("\n\n");
    while(fscanf(val, "%lf", &value) != EOF)
    {
        //printf("value = %lf\n", value);
        for(int r=0; r<count; r++)
            ans += (pow(value, powers[r]))*(coefficients[r]);
        printf("x = %lf [ans = %.2lf]\n", value, ans);
        fprintf(eval, "%.2lf\n", ans);
        ans = 0;
    }
    

    fclose(val);
    fclose(polynomial);
    fclose(eval);
    
    printf("\n////////////////////////////\n");
    return 0;
}
/*
    if there is no ^ after the x, poewr is 1. if there is a ^ after the x, get the power
    if there is no coefficient before x, coefficient is 1. if there is, get coeffieciednt

    arrays:
    coefficient, power

    for(int i=0; i<power/coeffIndex; i++)
    {
        ans = pow(power[i], x);
        ans = ans*coefficient;
    }
    
    coefficients
    0: 5
    1: 23.5
    2: 1
    
    powers
    0: 1
    1: 3
    2: 2

    ans = pow(powers[i], x);
    ans = coefficients[i]*ans;
*/