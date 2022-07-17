#include<stdio.h>
#include<math.h>

int main()
{
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
    char tempC[100], tempP[100];
    double coeff, power, coefficients[100], powers[100];
    int countParse=0, pow_index, x_index;

    for(int i=polyIndex; i >= 0; i--) //going through whole expression
    {
        if(poly[i] == '+' || poly[i] == '-' || i == 0)
        {
            int subIndex = i;
           
            for(int j=0; j<=countParse; j++, subIndex++)
            {

                if(poly[subIndex] == 'x')
                    x_index = subIndex;

                if(poly[subIndex] == '^')
                    pow_index = subIndex;
            }
            int lastIndex = subIndex;
            subIndex = i;
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
                        z++;
                    }
                }
            }
            z=0;
            sscanf(tempC, "%lf", &coeff);
            coefficients[count2] = coeff;
            
            for(subIndex=i; subIndex<lastIndex; subIndex++)//going through powers
            {
                if(subIndex > pow_index)
                {
                    if(poly[x_index+1] == '^')
                        tempP[z] = poly[subIndex];
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
            
            count2--; //index for coefficient and power
            countParse = 0; //resetting

            for(int r=0; r<100; r++)//emptying temp arrays
            {
                tempP[r] = 0;
                tempC[r] = 0;
            }
        }
        countParse++;
    }

    double ans=0, value;
    printf("\n");
    for(int h=0; h<polyIndex;h++)
        printf("%c", poly[h]);
    printf("\n\n");
    while(fscanf(val, "%lf", &value) != EOF)
    {
        for(int r=0; r<count; r++)
            ans += (pow(value, powers[r]))*(coefficients[r]);
        printf("x = %lf [ans = %.2lf]\n", value, ans);
        fprintf(eval, "%.2lf\n", ans);
        ans = 0;
    }

    fclose(val);
    fclose(polynomial);
    fclose(eval);
    return 0;
}
