#include <stdio.h>
#include <stdlib.h>

/*PROTOTYPES*/
int Checksum(int *data, int length);
void ReplaceChecksum(int ch, char arr[]);
int readFile(char arr[]);
int WriteFile(char arr[], int byteCount);
void ReplaceSerial(char arr[], char HexNum[]);
int ReadCurrentSerialNum(char HexNum[]);
int UpdateSerialNumber(int currentSerial);
void swapNums(char arr[]);
void DecimelToHex(int serial, char HexNum[]);
int getLineNum(char arr[]);
/*END OF PROTOTYPES*/

#define SERIAL_POSITION  0x3151//12625  //0x30D5; //12501

char arr1[30000];

int Checksum(int *data, int length)
 {
    int count;

    int Sum = 0;
    int Sum1 = 0; 
    
    count = 0;
    do {
        Sum1 = Sum1 + data[count];
        count++;
    } while (--length);
    
    Sum = -Sum1;
     
    return (Sum & 0xFF);
 }

void ReplaceChecksum(int ch, char arr[])
{
    int nums[20];
    int i = 0, j=0;
    int temp = 0;
    int checksum = 0;
    char cs[2] = {0,0};

    for(i=0;i<20;i++) nums[i]=0;

    for(i=0;i<20;i++)
    {
        if(arr[j+ch] < 'A') temp = ((arr[j+ch]-48)<<4); 
        else temp = ((arr[j+ch]-55)<<4); 
        j++;
        if(arr[j+ch] < 'A') temp += (arr[j+ch]-48);
        else temp += (arr[j+ch]-55);
        nums[i] = temp;
        //printf("\ntemp[%d] = %x", i, nums[i]);
        j++;
        temp = 0;
    }

    checksum = Checksum (nums, 20);
    //printf("\nChecksum = %x", checksum);

    cs[0] = ((checksum)>>4);
    if(cs[0] < 10) cs[0] += 48;
    else cs[0] += 55;

    cs[1] = (checksum&0x0F);
    if(cs[1] < 10) cs[1] += 48;
    else cs[1] += 55;

    //printf("\nChecksum = %c%c", cs[0], cs[1]);
    arr1[ch+40] = cs[0];
    arr1[ch+41] = cs[1];
}

int readFile(char arr[])
{
    char ch;
    FILE *fp;
    int byteCount = 0;

    fp = fopen("AAA_VA.hex", "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file wake_me.\n");
    }

    while((ch = fgetc(fp)) != EOF)
    {
        arr[byteCount] = ch;
        byteCount++;        
    }
    fclose(fp);

    return byteCount;
}

int WriteFile(char arr[], int byteCount)
{
    FILE * fp;
   int i;

   /* open the file for writing*/
   fp = fopen ("AAA_VA.hex","w");

   if (fp == NULL)
    {
        perror("\nError while opening the file wake_me.\n");
        return 1;
    }
 
   //fprintf (fp, "%d",currentSerial+1);
    for(i = 0; i < byteCount; i++) fprintf (fp, "%c",arr[i]);

   /* close the file*/  
   fclose (fp);
   
   return 0; 
}

void ReplaceSerial(char arr[], char HexNum[])
{
    int i = 0;
    int temp = 0;
    temp = SERIAL_POSITION;

    for(i = temp ; i < temp+4; i++) arr[i] = HexNum[i - temp];
} 


int ReadCurrentSerialNum(char HexNum[])
{
    char ch;
    char num[5] = {0,0,0,0,0};
    int i = 0;
    int serialNum = 0;

    FILE *fp;

    fp = fopen("Serials.txt", "r"); // read mode

    if (fp == NULL)
    {
        perror("\nError while opening the file.\n");
        return 0;
    }

    while((ch = fgetc(fp)) != EOF)
    {
        num[i] = ch;
        i++;
    }
    fclose(fp);
    
    serialNum = atoi(num);

    if(serialNum > 65535) serialNum = 1;
    else if(serialNum == 0) serialNum = 1;
    
    DecimelToHex(serialNum, HexNum);

    return serialNum;
}

int UpdateSerialNumber(int currentSerial)
{
   FILE * fp;

   /* open the file for writing*/
   fp = fopen ("Serials.txt","w");

   if (fp == NULL)
    {
        perror("\nError while opening the file.\n");
        return 1;
    }
 
   fprintf (fp, "%d",currentSerial+1);

   /* close the file*/  
   fclose (fp);
   
   return 0; 
}

void swapNums(char arr[])
{
    char tempArr[4];
    int i = 0;

    for(i=0; i < 4; i++) { tempArr[i] = arr[i]; }
    //ABCD becomes CDAB
    arr[3] = tempArr[2];
    arr[2] = tempArr[3];
    arr[1] = tempArr[0];
    arr[0] = tempArr[1];
}

void DecimelToHex(int serial, char HexNum[])
{
    int i = 0;
    int quotient = 0;
    int temp = 0;

    quotient = serial;

    while(quotient != 0)
    {
        temp = quotient% 16;
        if(temp < 10) temp += 48;
        else temp += 55;

        HexNum[i] = temp;
        i++;
        quotient /= 16;
    }
    
    //for(i = 0; i < 4; i++) printf("\nHexNum[%d] = %c", i, HexNum[i]);
    printf("\n");
    swapNums(HexNum);
    //for(i = 0; i < 4; i++) printf("\nHexNum[%d] = %c", i, HexNum[i]);
}

int getLineNum(char arr[])
{
    int i = 0;
    int lineNum = 0;
    int temp = SERIAL_POSITION;
    int lineStart = 0;

    for(i = 0; i < temp; i++) 
    {
        if(arr[i] == ':') 
        {
            lineNum++;    
            lineStart = i;
        }
    }
    lineStart++;
    //printf("\nLine = %d", lineNum);
    //printf("\nLinestart = %d", lineStart);
    
    if(lineNum != 0) ReplaceChecksum(lineStart, arr);
    else return 0;

    return 1;    
}

int main(void)
{
    int currentSerial = 0;
    int byteCount = 0;
    char HexNum[4] = {48,48,48,48}; //start at '0'

    currentSerial = ReadCurrentSerialNum(HexNum);

    byteCount = readFile(arr1);
    
    ReplaceSerial(arr1, HexNum);
    getLineNum(arr1);
    
    if(currentSerial > 0) UpdateSerialNumber(currentSerial);
    else printf("ERROR, serial = 0");

    WriteFile(arr1, byteCount);

    return 0;
}