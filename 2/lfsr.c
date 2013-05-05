#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define LFSR_LEN 6

uint8_t buffer[] = {1,0,1,1,1,1};
uint8_t taps[] = {0,0,1,0,1,1};
uint8_t states[64][6];

void printstate()
{
	int i;
	for(i = 0; i < LFSR_LEN; i++)
	{
		printf("%d",buffer[i]);
	}
	printf("\n");
}

void storestate(int pos)
{
	int i;
	for(i = 0; i < LFSR_LEN; i++)
	{
		states[pos][i] = buffer[i];
	}
}

void initlfsr(char val)
{
	buffer[0] = (val & (0x01 << 0)) ? 1 : 0;
	buffer[1] = (val & (0x01 << 1)) ? 1 : 0;
	buffer[2] = (val & (0x01 << 2)) ? 1 : 0;
	buffer[3] = (val & (0x01 << 3)) ? 1 : 0;
	buffer[4] = (val & (0x01 << 4)) ? 1 : 0;
	buffer[5] = (val & (0x01 << 5)) ? 1 : 0;
}

void settaps(char val)
{
	taps[0] = (val & (0x01 << 0)) ? 1 : 0;
	taps[1] = (val & (0x01 << 1)) ? 1 : 0;
	taps[2] = (val & (0x01 << 2)) ? 1 : 0;
	taps[3] = (val & (0x01 << 3)) ? 1 : 0;
	taps[4] = (val & (0x01 << 4)) ? 1 : 0;
	taps[5] = (val & (0x01 << 5)) ? 1 : 0;
}

int updatelfsr()
{	
	int i;
	uint8_t sum = 0;
	for(i = 0; i < LFSR_LEN; i++)
	{
		sum +=  ((buffer[i] == 1) & (taps[i] == 1)) ? 1 : 0;
	}
	sum = sum % 2;
	
	for(i = LFSR_LEN - 1; i > 0; i--)
	{
		buffer[i] = buffer[i-1];
	}
	buffer[0] = sum;
	
	return sum;
}

void writeham()
{
	FILE *f;
	fopen("ham_mat.tex","w");
	int i,j;
//	fwrite("\\begingroup \\fontsize{8pt}{12pt} \\selectfont\n\\[ \\left(\\begin{array}{ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc}\n",140,1,f);
	for(i = 0; i < LFSR_LEN; i++)
	{
		for(j = 0; j < 57; j++)
		{
			char string[4];
			sprintf(string,"%d & ",states[j][i]);
			fwrite(string,4,1,f);
		}
		j = 0;
		while(j < i)
		{
			fwrite("0 & ",4,1,f);
			j++;
		}
		fwrite("1 ",2,1,f);
		if(++j == LFSR_LEN)
		{
			fwrite("\\\\\n",3,1,f);
			continue;
		}
		else
		{
			fwrite("& ",2,1,f);
		}
		while(j++ <= LFSR_LEN)
		{
			fwrite("0 ",2,1,f);
			if(j == LFSR_LEN)
			{
				fwrite("\\\\\n",3,1,f);
				break;
			}
			else
			{
				fwrite("& ",2,1,f);
			}
		}
	}
//	fwrite("\\end{array} \\right)\\]\n\\endgroup",30,1,f);
	fclose(f);
}

void writegen()
{
	FILE *f;
	f = fopen("gen_mat.tex","w");
//	fwrite("\\begingroup \\fontsize{8pt}{12pt} \\selectfont\n\\[ \\left(\\begin{array}{ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc}\n",140,1,f);
	int i,j,k;
	for(i = 0; i < 57; i++)
	{
		j = 0;
		while(j++ < i)
		{
			fwrite("0 & ",4,1,f);
		}
		fwrite("1 & ",4,1,f);
		while(++j < 58)
		{
			fwrite("0 & ",4,1,f);
		}
		for(j = 0; j < LFSR_LEN; j++)
		{
			char string[4];
			sprintf(string,"%d ",states[i][j]);
			fwrite(string,2,1,f);
			if(j == LFSR_LEN - 1)
			{
				fwrite("\\\\\n",3,1,f);
			}
			else
			{
				fwrite("& ",2,1,f);
			}
		}
	}
//	fwrite("\\end{array} \\right)\\]\n\\endgroup",31,1,f);
	fclose(f);
}

int sumstate()
{
	int i, sum = 0;
	for(i = 0; i < LFSR_LEN; i++)
	{
		sum += buffer[i];
	}
	return (sum > 1) ? 1 : 0;
}

void main()
{
	int i,j,k;
	uint8_t l = 0;
	uint8_t tapopts[] = {0x39};//{0x27,0x3C,0x31,0x39,0x0F,0x23};
	for(i = 0; i < 1; i++)
	{
		settaps(tapopts[i]);
		initlfsr(0xFF);
		k = 0;
		for(j = 0; j < 63; j++)
		{
			if(sumstate() == 1)
			{
				storestate(k);
				k++;
			}
			updatelfsr();
		}
		writegen();
		writeham();
	}
}

