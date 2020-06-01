#include <stdio.h>
#include <stdlib.h>

char arr1[30000];
char arr2[30000];

int Checksum(int *data, int length)
 {
    int count;
    char j;
    int Sum = 0;
    int Sum1 = 0; 
    
    count = 0;
    do {
        Sum1 = Sum1 + data[count];
        count++;
    } while (--length);
    
    Sum = -Sum1;
     
    j = Sum & 0xFF;
     
    return (Sum & 0xFF);
 }

int ReplaceChecksum(int line, char ch)//, char arr[])
{
    int nums[20];
    int i = 0, j=0;
    int temp = 0;
    char arr[40] = {'1','0','0','0','0','0','0','0','B','8','0','7','0','0','2','0','6','5','0','1','0','0','0','8','8','B','0','1','0','0','0','8','8','D','0','1','0','0','0','8'};
    int checksum = 0;
    char cs[2] = {0,0};
    for(i=0;i<20;i++) nums[i]=0;

    for(i=0;i<20;i++)
    {
        if(arr[j] < 'A') temp = ((arr[j]-48)<<4); 
        else temp = ((arr[j]-55)<<4); 
        j++;
        if(arr[j] < 'A') temp += (arr[j]-48);
        else temp += (arr[j]-55);
        nums[i] = temp;
        //printf("\ntemp[%d] = %d", i, nums[i]);
        j++;
        temp = 0;
    }

    checksum = Checksum (nums, 20);
    //printf("\nChecksum = %d", checksum);

    cs[0] = ((checksum)>>4);
    if(cs[0] < 10) cs[0] += 48;
    else cs[0] += 55;

    cs[1] = (checksum&0x0F);
    if(cs[1] < 10) cs[1] += 48;
    else cs[1] += 55;

    //printf("\nChecksum = %c%c", cs[0], cs[1]);
}

int readFile(char *name, char arr[])
{
    char ch;
    
    FILE *fp;
    int byteCount = 0;

    fp = fopen(name, "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
    }

    while((ch = fgetc(fp)) != EOF)
    {
        arr[byteCount] = ch;
        byteCount++;        
    }
    fclose(fp);

    return byteCount;
}

int checkArrays(int count)
{
    int i = 0;
    int line= 0;
    char n = 0;

    for(i = 0; i < count; i++)
    {
        if((arr1[i] != arr2[i])) printf("\nChar differs! line = 0x%x char = 0x%x pos = %d %c %c", line, n, i, arr1[i], arr2[i]);
        if(arr1[i] == ':') 
        {
            line++;
            n = 1;
        }
        else n++;

    }
}

int main(void)
{
    int count = 0;
    //eplaceChecksum(0,0);
    count = readFile("AAA_VA_FF.hex", arr1);
    printf("\nbyte Count = %d", count);
    printf("\nbyte Count = %d", readFile("AAA_VA_00.hex", arr2));

    checkArrays(count);

    return 0;
}