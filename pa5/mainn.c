#include <stdio.h>

int main()
{
    FILE *fp;
    fp = fopen("bop.txt", "r");

    int ch, ch2;

    while (1)
    {

        ch = fgetc(fp);
        ch2 = fgetc(fp);
        if (feof(fp))
            break;

        if (ch == 10 || ch2 == 10)
            continue;
        printf("%d\n", ch);

        fseek(fp, -1, SEEK_CUR);
    }
}