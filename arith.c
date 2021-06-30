#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SYMBOLS 25
#define ALPHABET 26

int counter = 0;

typedef struct Letter
{
    char symb;
    int num;
} Letter;

int read(char *fl, char *s1, char *s2, char *s3, char *func) 
{
    FILE *fp = fopen(fl, "r");
    
    if(fgets(s1, SYMBOLS, fp) == NULL)
        return 0;
    
    char *func1 = (char*) calloc(SYMBOLS, sizeof(char));
    if(fgets(func1, SYMBOLS, fp) == NULL)
        return 0;
    *func = *func1;
    if(*func!='+' && *func!='-' && *func!='*' && *func!='/')
        return 0;
    
    if(fgets(s2, SYMBOLS, fp) == NULL)
        return 0;
    if(fgets(s3, SYMBOLS, fp) == NULL)
        return 0;
    
    s1[strlen(s1)-1] = '\0';
    s2[strlen(s2)-1] = '\0';
    s3[strlen(s3)-1] = '\0';
    
    fclose(fp);
    return 1;
}

void convert_to_num(Letter *letters, int count, 
                    char *s1, char *s2, char *s3,
                    int *num1, int *num2, int *num3)
{
    int tier, j, i;
    *num1 = 0; 
    *num2 = 0; 
    *num3 = 0,
  
    tier = 1;
    for (i = strlen(s1) - 1; i >= 0; i--) 
    {
        for (j = 0; j < count; j++) 
            if (letters[j].symb == s1[i]) 
                break; 
  
        *num1 += tier * letters[j].num; 
        tier *= 10; 
    }
  
    tier = 1;
    for (i = strlen(s2) - 1; i >= 0; i--) 
    {
        for (j = 0; j < count; j++) 
            if (letters[j].symb == s2[i]) 
                break; 
  
        *num2 += tier * letters[j].num; 
        tier *= 10; 
    }
  
    tier = 1;
    for (i = strlen(s3) - 1; i >= 0; i--) 
    {
        for (j = 0; j < count; j++) 
            if (letters[j].symb == s3[i]) 
                break; 
  
        *num3 += tier * letters[j].num; 
        tier *= 10; 
    } 
}

void find_letters(Letter *letters, int *n, char *s1, char *s2, char *s3)
{
    *n = 0;
    char *symbols = (char*) calloc(ALPHABET, sizeof(char));
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < strlen(s1); j++)
        {
            if(strchr(symbols, s1[j]) == NULL)
            {
                letters[*n].symb = s1[j];
                symbols[*n] = s1[j];
                *n = *n +1;
            }
        }
        for(int j = 0; j < strlen(s2); j++)
        {
            if(strchr(symbols, s2[j]) == NULL)
            {
                letters[*n].symb = s2[j];
                symbols[*n] = s2[j];
                *n = *n +1;
            }
        }
        for(int j = 0; j < strlen(s3); j++)
        {
            if(strchr(symbols, s3[j]) == NULL)
            {
                letters[*n].symb = s3[j];
                symbols[*n] = s3[j];
                *n = *n +1;
            }
        }
    }
}

int check_comb(Letter *letters, int count, 
          char *s1, char *s2, char *s3, char func) 
{
    counter++;
    for(int i = 0; i < count; i++)
    {
        //if long number starts with 0
        if(((strlen(s1) > 1 &&
            letters[i].symb == s1[0]) ||
           (strlen(s2) > 1 &&
            letters[i].symb == s2[0]) ||
           (strlen(s3) > 1 &&
            letters[i].symb == s3[0])) &&
            letters[i].num == 0)
            return 0;
        //if division from 0
        if(func == '/' && strlen(s2) == 1 && 
           letters[i].symb == s2[0] &&
           letters[i].num == 0)
            return 0;
    }
    
    int num1, num2, num3;
    convert_to_num(letters, count, s1, s2, s3,
                   &num1, &num2, &num3);
  
    //func
    if(func == '+')
    {
        if((num1 + num2) == num3)
            return 1;
    }
    else if(func == '-')
    {
        if(num1 - num2 == num3)
            return 1;
    }
    else if(func == '*')
    {
        if(num1 * num2 == num3)
            return 1;
    }
    else if(func == '/')
    {
        if(num1 % num2 == 0 && num1 / num2 == num3)
            return 1;
    }

    return 0; 
} 

int combinations(Letter *letters, int count, int n, 
                 char *s1, char *s2, char *s3, char func, int *use) 
{
    if(n == count) 
    { 
        if (check_comb(letters, count, s1, s2, s3, func) == 1) 
            return 1;
        return 0; 
    }
   
    for (int i = 0; i < 10; i++) 
    { 
        // if int not used yet 
        if (use[i] == 0) 
        { 
            // assign char at index n integer i 
            letters[n].num = i;
            use[i] = 1; 
            if (combinations(letters, count, n + 1, s1, s2, s3, func, use)) 
                return 1; 
  
            // backtrack for other comb
            use[i] = 0; 
        } 
    } 
    return 0; 
}

void print(Letter *letters, int count, char *s1, char *s2, char *s3, char func)
{
    printf("%s\n%c\n%s\n%s\n\n", s1, func, s2, s3);
    int num1, num2, num3;
    convert_to_num(letters, count, s1, s2, s3, &num1, &num2, &num3);
    printf("%d\n%c\n%d\n%d\n", num1, func, num2, num3);
    printf("counter = %d\n", counter);
}

int check_if_file_correct(char *fl)
{
    if(fopen(fl, "r") == NULL)
    {
        printf("No file.\n");
        return 0;
    }
    FILE *fp = fopen(fl, "r");
    
    fseek(fp, 0, SEEK_END);
    if(ftell(fp) == 0)//check_comb if file not empty
    {
        fclose(fp);
        printf("File is empty.\n");
        return 0;
    }
    fclose(fp);
    return 1;
}

int main()
{
    char *fl = "num1.txt";
    
    if(check_if_file_correct(fl) == 0)
        return 0;
    
    char *s1, *s2, *s3;
    s1 = (char*) calloc(SYMBOLS, sizeof(char));
    s2 = (char*) calloc(SYMBOLS, sizeof(char));
    s3 = (char*) calloc(SYMBOLS, sizeof(char));
    char func;
    if(read(fl, s1, s2, s3, &func) == 0)
    {
        printf("Incorrect data.\n");
        return 0;
    }
    
    Letter *letters = (Letter*) calloc(ALPHABET, sizeof(Letter));
    int count;
    find_letters(letters, &count, s1, s2, s3);
    
    int *use = (int*) calloc(10, sizeof(int));
    if (combinations(letters, count, 0, 
        s1, s2, s3, func, use)) 
        print(letters, count, s1, s2, s3, func);
    else
        printf("No solution found.\n");
    
    free(s1);
    free(s2);
    free(s3);
    free(letters);
    return 0;
}
