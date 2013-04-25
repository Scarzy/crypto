#include <stdint.h>
#include <stdio.h>

#define FILE_LEN 556
#define LFSR_LEN 5

char taps[] = {1,0,0,1,1};

void bintoarr(uint8_t bin[], int len, uint8_t out[])
{
	int i;
	for(i = 0; i < len; i++)
	{
		int j;
		for(j = 0; j < 8; j++)
		{
			out[(8*i)+j] = (((bin[i] >> (7-j)) & 0x01) != 0) ? 1 : 0;
		}
	}
}

void filetoarr(char file[], uint8_t out[])
{
	uint8_t bin[FILE_LEN];
	
	FILE *f;
	f = fopen(file,"rb");
	fread(bin, FILE_LEN, 1, f);
	fclose(f);
	
	bintoarr(bin, FILE_LEN, out);
}

void printbytes(uint8_t out[])
{
	int i;
	for(i = 0; i < FILE_LEN; i++)
	{
		int j;
		for(j = 0; j < 8; j++)
		{
			printf("%d",out[(8*i)+j]);
		}
		printf("\n");
	}
}

int updatelfsr()
{
	static uint8_t buffer[] = {1,0,1,1,1};
	static uint8_t taps[] = {1,1,0,0,1};
	
	int i;
	uint8_t sum = 0;
	for(i = 0; i < LFSR_LEN; i++)
	{
		sum +=  ((buffer[i] == 1) & (taps[i] == 1)) ? 1 : 0;//((sum == 1) || ((buffer[i] == 1) & (taps[i] == 1))) ? 1 : 0;
	}
	sum = sum % 2;
	
	for(i = LFSR_LEN - 1; i > 0; i--)
	{
		buffer[i] = buffer[i-1];
	}
	buffer[0] = sum;
	
	for(i = 0; i < LFSR_LEN; i++)
	{
		printf("%d",buffer[i]);
	}
	printf("  %d\n",sum);
	
	return sum;
}

void main()
{
	uint8_t buffer = 0x00;
	uint8_t out[FILE_LEN * 8];
	
	filetoarr("bin",out);
	
	int i = 0;
	for(i = 0; i < 32; i++)
	{
		updatelfsr();
	}
}

