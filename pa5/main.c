#include <stdio.h>
#include <math.h>

#define MAX 27

void open_files(FILE **fp1, FILE **fp2, FILE **fp3, FILE **fp4, FILE **fp5, FILE **fpX)
{ /* Function opens all files in read mode */
    *fp1 = fopen("language_1.txt", "r");
    *fp2 = fopen("language_2.txt", "r");
    *fp3 = fopen("language_3.txt", "r");
    *fp4 = fopen("language_4.txt", "r");
    *fp5 = fopen("language_5.txt", "r");
    *fpX = fopen("language_x.txt", "r");
}

void clean_array(int a[MAX][MAX])
{ /* This function is used to make sure the array is reset whenever it is used for a new language */
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
            a[i][j] = 0;
    }
}

void close_files(FILE *fp1, FILE *fp2, FILE *fp3, FILE *fp4, FILE *fp5, FILE *fpX)
{ /* function closes all files at the end of the program */
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    fclose(fpX);
}

int get_bigram_count(int arr[MAX][MAX], FILE *fp)
{
    /* row is the first character, coloumn is the second character. 0 indices are whitespace.
       example: arr[1][2] = "ab"
                arr[0][3] = " c"
       Letter's indices are calculated through their ascii codes.
    */
    char ch, ch2;
    int char1, char2, totalCount = 0;

    clean_array(arr); // Cleaning array before using to make sure nothing is left over from previous input

    while (1)
    {
        ch = fgetc(fp);
        ch2 = fgetc(fp);

        if (feof(fp))
            break;

        char1 = ch;
        char2 = ch2;

        if (char1 == 10 || char2 == 10)
            continue;

        else if (char1 == 32) // first character is a space
            arr[0][char2 - 96] += 1;

        else if (char2 == 32) // second character is a space
            arr[char1 - 96][0] += 1;

        else // no space in the bigram
            arr[char1 - 96][char2 - 96] += 1;

        if (char1 != 32 || char2 != 32) // total count is increased as long as bigram is not a double whitespace
            totalCount += 1;

        fseek(fp, -1, SEEK_CUR); // going back one place in the file to get the next bigram
    }

    rewind(fp);

    return totalCount;
}

float calculate_frequency(int totalCount, int currentCount)
{
    /* Frequency of a bigram is calculated by getting the ratio of the current bigram to the total no. of bigrams in a language */
    float freq = 0.0;

    freq = ((float)currentCount) / ((float)(totalCount));

    return freq;
}

float calculate_score(int a[MAX][MAX], int b[MAX][MAX], int totalCountA, int totalCountB)
{
    /* Dissimilarity score of a bigram across two languages is calculated by subtracting the frequencies of the bigrams.
       The bigram arrays are traversed. Back to back whitespaces and non-existent bigrams in both languages are ignored.
       Scores are then printed.
    */
    int i, j;
    float freqA = 0.0, freqB = 0.0, score = 0.0, sum = 0.0;
    for (i = 0; i < MAX; i++)
    {
        for (j = 0; j < MAX; j++)
        {
            if (i == 0 && j == 0) // ignore if both elements are whitespaces
                continue;

            if (a[i][j] == 0 && b[i][j] == 0) // ignore if bigram does not exist in both languages
                continue;

            freqA = calculate_frequency(totalCountA, a[i][j]);
            freqB = calculate_frequency(totalCountB, b[i][j]);

            score = fabs(freqA - freqB);

            sum += score;
        }
    }
    return sum;
}

void dissimilarity(FILE *fp_A, FILE *fp_B)
{
    /* To get the dissimilarity scores between two languages, first we get the bigram counts and assign them to their arrays.
       Then we calculate the scores. We clean the array in the end to
    */
    int bigramCount_B[MAX][MAX], bigramCount_A[MAX][MAX], totalBigram_A = 0, totalBigram_B = 0;

    totalBigram_A = get_bigram_count(bigramCount_A, fp_A);
    totalBigram_B = get_bigram_count(bigramCount_B, fp_B);

    printf("%f\n", calculate_score(bigramCount_A, bigramCount_B, totalBigram_A, totalBigram_B));
}

void get_all_dissimilarity(FILE *fp1, FILE *fp2, FILE *fp3, FILE *fp4, FILE *fp5, FILE *fpX)
{
    dissimilarity(fpX, fp1);
    dissimilarity(fpX, fp2);
    dissimilarity(fpX, fp3);
    dissimilarity(fpX, fp4);
    dissimilarity(fpX, fp5);
}

int main()
{
    FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fpX;

    open_files(&fp1, &fp2, &fp3, &fp4, &fp5, &fpX);

    get_all_dissimilarity(fp1, fp2, fp3, fp4, fp5, fpX);

    close_files(fp1, fp2, fp3, fp4, fp5, fpX);

    return 0;
}