#include<stdio.h>
#include<Windows.h>
#include<iostream>
#include<stdio.h>
#include<openssl/md5.h>
#include<fstream>
#include<math.h>
#include<time.h>
#define pssize 6
#define num 100000

//unsigned char PASSCHAR[] = "0123456789abcdefghijklmnopqrstuvwxyz_@!$ABCDEFGHIJKLMNOPQRSTUVWXYZ~#%^&*()+[]\\{}|;':\",./<>?";
unsigned char PASSCHAR[] = "0123456789";
int len = 10;
int length = 10;
unsigned char result[pssize] = { 0 };
unsigned char heads[num][pssize + 1];
unsigned char tails[num][pssize + 1];
unsigned char plen[pssize + 1];


void R7(unsigned char * pwd,const unsigned char * hash, int step)
{
	
	unsigned int idx[4];
	idx[0] = (*(unsigned int*)hash) ^ -1;
	idx[1] = (*(unsigned int*)(hash + 4));
	idx[2] = (*(unsigned int*)(hash + 8));
	idx[3] = (*(unsigned int*)(hash + 12));
	idx[0] %= len * len;
	idx[1] %= len * len;
	idx[2] %= len * len;
	idx[3] %= len * len;
	pwd[0] = PASSCHAR[idx[0] / len];
	pwd[1] = PASSCHAR[idx[1] % len];
	pwd[2] = PASSCHAR[idx[1] / len];
	pwd[3] = PASSCHAR[idx[2] % len];
	pwd[4] = PASSCHAR[idx[2] / len];
	pwd[5] = PASSCHAR[idx[3] % len];
	pwd[6] = 0;
}

void table_gen(const char * filepath)
{
	unsigned char p0[pssize + 1];
	unsigned char h[17];
	unsigned char plen[pssize + 1];
	p0[pssize] = 0;
	h[16] = 0;
	plen[pssize] = 0;
	FILE *fp = fopen(filepath, "wt");
	srand(time(NULL));
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < pssize-1; j++)
		{
			p0[5] = '0';
			p0[j] = i/ (int)pow(10,j) % 10+'0';
		}

		memcpy(plen, p0, pssize);
		for (int j = 0; j < length; j++)
		{
			MD5(plen, pssize, h);
			R7(plen, h, 0);
		}
		fprintf(fp, "%s %s\n", (char*)p0, (char*)plen);
	}
	fclose(fp);
}
void table_load(const char * filepath)
{
	unsigned char psh[pssize + 1];
	unsigned char pst[pssize + 1];
	memset(psh, 0, pssize + 1);
	memset(pst, 0, pssize + 1);
	FILE * fp = fopen(filepath, "rt");
	int i = 0;
	if (fp)
	{
		while (fscanf(fp, "%s %s", psh, pst) != EOF)
		{
			memcpy(heads[i], psh, pssize);
			memcpy(tails[i], pst, pssize);
			i++;
		}
		fclose(fp);
	}
}

int search(unsigned char* a)
{
	int i, j;
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (a[j] != tails[i][j]) break;
		}
		if (j == 6) return i;
	}
	return -1;
}

int verify(const unsigned char * hash0, int nu, int i)
{
	unsigned char pi[pssize + 1];
	unsigned char hashi[17];
	pi[pssize] = 0;
	hashi[16] = 0;
	memcpy(pi,heads[nu], pssize);
	MD5(pi, 6, hashi);
	for (int j = 0; j < i; j++)
	{
		R7(pi, hashi, 0);
		MD5(pi, 6, hashi);
	}
	if (!strcmp((char *)hash0, (char *)hashi))
	{
		memcpy(result, pi,pssize);
		return 1;
	}
	else return 0;
};




int atk(const unsigned char * hash)
{
	unsigned char pi[pssize + 1];
	pi[pssize] = 0;
	unsigned char hashi[17];
	hashi[16] = 0;
	int sea;
	R7(pi, hash, 0);
	sea = search(pi);
	if (verify(hash, sea, length - 1) &&(sea!=-1)) return 1;
	for (int i = 1; i <= length-1; i++)
	{
		MD5(pi, pssize, hashi);
		R7(pi, hashi, 0);
		sea = search(pi);
		if (sea == -1) continue;
		if (verify(hash, sea, length - i - 1) == 1) return 1;
	}
	return 0;
}



int main()
{
	int i = 0;
	table_gen("D:\\rainbow.txt");
	table_load("D:\\rainbow.txt");
	unsigned char test[pssize + 1] = "000000";
	test[pssize] = 0;
	unsigned char hash0[17];
	hash0[16] = 0;


	//MD5(test, 6, hash0);
	//i = atk(hash0);



	//memcpy(plen, test, pssize);
	//unsigned char h[17];
	//h[16] = 0;
	//for (int j = 0; j < length; j++)
	//{
	//	MD5(plen, pssize, h);
	//	R7(plen, h, j);
	//}





	
	for (int k1 = 0; k1 <= 9; k1++)
	{
		for (int k2 = 0; k2 <= 9; k2++)
		{
			for (int k3 = 0; k3 <= 9; k3++)
			{
				for (int k4 = 0; k4 <= 9; k4++)
				{
					for (int k5 = 0; k5 <= 9; k5++)
					{
						for (int k6 = 0; k6 <= 9; k6++)
						{
							MD5(test, 6, hash0);
							i += atk(hash0);
							test[0] = k1 + '0';
							test[1] = k2 + '0';
							test[2] = k3 + '0';
							test[3] = k4 + '0';
							test[4] = k5 + '0';
							test[5] = k6 + '0';
						}
					}
				}
			}
		}
	}


	return 0;
}