#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define FILE_LEN 556
#define LFSR_LEN 5

uint8_t buffer[] = {1,0,1,1,1};
uint8_t taps[] = {0,0,1,0,1};

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

void arrtobin(uint8_t arr[], int len, uint8_t out[])
{
	int i;
	for(i = 0; i < len/8; i++)
	{
		uint8_t val = 0x00;
		int j;
		for(j = 0; j < 8; j++)
		{
			if(arr[(8*i)+j] == 1)
			{
				val |= (0x01 << (7-j));
			}
		}
		out[i] = val;
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

void arrtofile(char file[], uint8_t in[])
{
	uint8_t bin[FILE_LEN];
	
	arrtobin(in,FILE_LEN*8,bin);
	
	FILE *f;
	f = fopen(file,"wb");
	fwrite(bin, FILE_LEN, 1, f);
	fclose(f);
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

void initlfsr(char val)
{
	buffer[0] = (val & (0x01 << 0)) ? 1 : 0;
	buffer[1] = (val & (0x01 << 1)) ? 1 : 0;
	buffer[2] = (val & (0x01 << 2)) ? 1 : 0;
	buffer[3] = (val & (0x01 << 3)) ? 1 : 0;
	buffer[4] = (val & (0x01 << 4)) ? 1 : 0;
	
//	int i;
//	for(i = 0; i < LFSR_LEN; i++)
//	{
//		printf("%d",buffer[i]);
//	}
//	printf("\n");
}

void settaps(char val)
{
	taps[0] = (val & (0x01 << 0)) ? 1 : 0;
	taps[1] = (val & (0x01 << 1)) ? 1 : 0;
	taps[2] = (val & (0x01 << 2)) ? 1 : 0;
	taps[3] = (val & (0x01 << 3)) ? 1 : 0;
	taps[4] = (val & (0x01 << 4)) ? 1 : 0;
}

int updatelfsr()
{	
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
	
	return sum;
}

void buildkey(uint8_t key[], int keylen)
{
	int i;
	for(i = 0; i < keylen - 1; i++)
	{
		key[i] = updatelfsr();
	}
	key[keylen-1] = (updatelfsr() == 1) ? 0 : 1;	//6 was unlikely to be the correct value, so let's try the opposite.
}

int getkeybit(uint8_t key[], int keylen)
{
	static int position = 0;
	return key[(position++) % keylen];
}

int checkmatch(uint8_t res[])
{
	static uint8_t desired[] = {0,1,0,1,0,1,0,1,0,1,1,1,0,0,1,0};
	int i;
	for(i = 0; i < 16; i++)
	{
		if(desired[i] != res[i])
		{
			return 0;
		}
	}
	return 1;
}

void autocorrellate(uint8_t vals[], int vallen, int rets[])
{
	int i, j;
	for(i = 0; i < vallen; i++)	//The shift applied
	{
		rets[i] = 0;
		for(j = i; j < vallen; j++)	//The position in the array
		{
			rets[i] += ((vals[i] + vals[j]) % 2);
		}
	}
}

void main()
{
	uint8_t out[FILE_LEN * 8];
	uint8_t res[FILE_LEN * 8];
	
	filetoarr("bin",out);
	
	int autores[FILE_LEN * 8];
	autocorrellate(out, FILE_LEN * 8, autores);
	int i,j,k;
	for(i = 0; i < FILE_LEN * 8; i++)
	{
		printf("%d: %d\n", i, autores[i]);
	}
	return;
	for(k = 0; k < 32; k++)
	{
		settaps(k);
		for(i = 0; i < 32; i++)
		{
			initlfsr(i);
			uint8_t key[32];
			buildkey(key,32);
			for(j = 0; j < FILE_LEN*8; j++)
			{
				int rnd = getkeybit(key,pow(2,LFSR_LEN));
				res[j] = (out[j] + rnd) % 2;
			}
			if(checkmatch(res) == 1)
			{
				printf("Match - taps: %d, start: %d\n", k, i);
				char filename[10];
				sprintf(filename,"result%d-%d.bin",k,i);
				arrtofile(filename,res);
			}
		}
	}
}

