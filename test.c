#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

char **names = NULL;
int counter = 0;

void changeArraySize(int length)
{
	names = (char**)realloc(names, (counter+1)*sizeof(char*));
	names[counter] = (char*)calloc(length, sizeof(char));
}

int arrLen(char dir[])
{
	int x;
	for(x = 0; dir[x] != '\0'; x++);
	return x+1;	
}

void copyToArr(char tmp[])
{
	int len = arrLen(tmp);
	changeArraySize(len);
	memcpy(names[counter], tmp, len*sizeof(char));
	printf("%s\n", names[counter]);
	counter++;
}

void qs(int first, int last)
{
    int i = first, j = last;
	char* x = names[(first + last) / 2];
    do 
	{
        while(strcmp(names[i], x) < 0)
		{
			i++;
		}
        while(strcmp(names[j], x) > 0)
		{
			j--;
		}
			
  
        if(i <= j) 
		{
            if(strcmp(names[i], names[j]) > 0)
			{
				char* tp = names[i];
				names[i] = names[j];
				names[j] = tp;
			}
            i++;
            j--;
        }
    }while(i <= j);
  
    if(i < last)
        qs(i, last);
    if(first < j)
        qs(first, j);
}

void OutputInfo(char workDir[],  bool paramL, bool paramF, bool paramD, bool paramS, bool ignoreUnknown)
{
	DIR *dp;
	struct dirent *ep;
	char tmp[350];
	dp = opendir(workDir);
	if(dp != NULL)
	{
		while(ep = readdir(dp))
			if(ep->d_type == DT_DIR && ep->d_name[0] != '.')
			{
				if(workDir[1] != '\0')
				{
					snprintf(tmp, sizeof tmp, "%s/%s", workDir, ep->d_name);
					if(paramD == true)
						copyToArr(tmp);
				}
				else
				{
					snprintf(tmp, sizeof tmp, "%s%s", workDir, ep->d_name);
					if(paramD == true)
						copyToArr(tmp);
				}
				OutputInfo(tmp, paramL, paramF, paramD, paramS, ignoreUnknown);
			}
			else 
			{
				if(paramD && paramF && paramL && true)
				{
					if(workDir[1] != '\0')
					snprintf(tmp, sizeof tmp, "%s/%s", workDir, ep->d_name);
					else snprintf(tmp, sizeof tmp, "%s%s", workDir, ep->d_name);
					copyToArr(tmp);
				}
				else
				{
					if(ep->d_type == DT_DIR && paramD)
					{
						if(workDir[1] != '\0')
						snprintf(tmp, sizeof tmp, "%s/%s", workDir, ep->d_name);
						else snprintf(tmp, sizeof tmp, "%s%s", workDir, ep->d_name);
						copyToArr(tmp);
					}
					else
					{
						if(ep->d_type == DT_REG && paramF)
						{
							if(workDir[1] != '\0')
							snprintf(tmp, sizeof tmp, "%s/%s", workDir, ep->d_name);
							else snprintf(tmp, sizeof tmp, "%s%s", workDir, ep->d_name);
							copyToArr(tmp);
						}
						else
						{
							if(ep->d_type == DT_LNK && paramL)
							{
								if(workDir[1] != '\0')
								snprintf(tmp, sizeof tmp, "%s/%s", workDir, ep->d_name);
								else snprintf(tmp, sizeof tmp, "%s%s", workDir, ep->d_name);
								copyToArr(tmp);
							}
						}
					}
				}
			}
		(void)closedir(dp);
	}
	else if(errno != EACCES) 
		puts("Couldn't open the directory.\n");
	return;
}

int main(int argc, char *argv[])
{
	char workName[255];
	if(argc==1)
	{
		getcwd(workName, 255);
		OutputInfo(workName, true, true, true, false, false);
	}
	else
	{
		if(argv[1][0] != '-')
			memcpy(workName, argv[1], 255);
		else getcwd(workName, 255);
		int opt;
		bool l = false,f = false,d = false,s = false;
		while((opt = getopt(argc, argv, "ldfs")) != -1)
		{
			switch (opt)
			{
				case 'l':
					l = true;
					break;
				case 'f':
					f = true;
					break;
				case 'd':
					d = true;
					break;
				case 's':
					s = true;
					break;
				case '?':
				default:
					printf("%s: option '-%c' is invalid\n", argv[0], opt);
					return 0;
					break;
			}
		}
		names = (char**)calloc(1, sizeof(char*));
		if(l == f == d == false)
			OutputInfo(workName, true, true, true, s, false);
		else OutputInfo(workName, l, f ,d ,s, true);
		if(s)
			qs(0, counter - 1);
		for(int z = 0; z < counter;z++)
			printf("%s\n", names[z]);
		for(int y=0; y < counter;y++)
			free(names[y]);
		free(names);
		printf("%d - counter\n", counter);
	}
	return 0;
}
