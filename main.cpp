#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning(disable:4996)

struct Resource
{
	char ResourceType;
	int TypeNumber, ResourceArea, ResourceLayer, NewResourceLayer;
	int linknum, *link, count;
};
Resource *resource;
struct Layer
{
	int LayerArea;
};
Layer *layer;

char InputFileName[20] = {};
int ResourceNumber = 0;
int LayerNumber = 0;
int MINTSV = 0;
double originDEP = 1.0;
double dep[6] = { 1.0,0.99,0.95,0.8,0.6,0.3 };
int odep[6] = { 0,0,0,0,0,0 };

void getResourceNumber();
void getData();
void getDependency();
void getMINTSV();
void Fileprint();
void buildLingoFile();
void startLingo();
void getoutput();
void getNewResourceLayer();
void printNewResourceLayer();

int main()
{
	int i, j;
	printf("請輸入要開啟的檔案名稱:");
	scanf("%s", &InputFileName);
	getResourceNumber();
	printf("ResourceNumber : %d\n", ResourceNumber);
	printf("LayerNumber : %d\n", LayerNumber);
	resource = (Resource*)malloc(sizeof(Resource)*ResourceNumber);
	layer = (Layer*)malloc(sizeof(Layer)*LayerNumber);
	for (i = 0; i < ResourceNumber; i++)
	{
		resource[i].linknum = 0;
		resource[i].count = 0;
	}
	getData();
	/*for (i = 0; i < ResourceNumber; i++)
	{
	printf("%c%d %d %d %d\t", resource[i].ResourceType, resource[i].TypeNumber, resource[i].ResourceLayer, resource[i].ResourceArea, resource[i].linknum);
	}
	printf("\n");
	for (i = 0; i < LayerNumber; i++)
	{
	printf("L%d(%d)\t", i + 1, layer[i].LayerArea);
	}
	printf("\n");*/
	for (i = 0; i < ResourceNumber; i++)
	{
		resource[i].link = (int*)malloc(sizeof(int)*resource[i].linknum);
	}
	getDependency();
	getMINTSV();
	Fileprint();
	buildLingoFile();
	startLingo();
	getoutput();
	system("PAUSE");
	return 0;
}
void getResourceNumber()
{
	char buf3[99] = {}, buf = NULL;
	int resourcenum = 0, buf2 = 0;
	FILE *fptr;
	fptr = fopen(InputFileName, "r");
	fscanf(fptr, "%s", &buf3);
	while (!feof(fptr))
	{
		fscanf(fptr, "%c", &buf);
		if (buf == 'E')
		{
			fscanf(fptr, "%c", &buf);
			if (buf == 'N')
			{
				fscanf(fptr, "%c", &buf);
				if (buf == 'D')
					break;
			}
		}
		else
		{
			while (buf != ':')
			{
				fscanf(fptr, "%c", &buf);
			}
			while (buf != ';')
			{
				buf2 = 0;
				fscanf(fptr, "%c", &buf);
				fscanf(fptr, "%d", &buf2);
				if (buf2 != 0)
					resourcenum++;
				fscanf(fptr, "%c", &buf);
			}
			LayerNumber++;
			fscanf(fptr, "%c", &buf);
		}
	}
	ResourceNumber = resourcenum;
	fclose(fptr);
}
void getData()
{
	int i = 0, j = 0, buf2 = 0, larea = 0, rtypearea = 0, rnum = 0, lnum = 0;
	char buf = NULL, buf3[99] = {}, rtype = NULL;
	FILE *fptr;
	fptr = fopen(InputFileName, "r");
	fscanf(fptr, "%s", &buf3);
	while (!feof(fptr))
	{
		fscanf(fptr, "%c", &buf);
		if (buf == 'E')
		{
			fscanf(fptr, "%c", &buf);
			if (buf == 'N')
			{
				fscanf(fptr, "%c", &buf);
				if (buf == 'D')
					break;
			}
		}
		if (buf == 'L')
		{
			fscanf(fptr, "%d", &lnum);
			fscanf(fptr, "%c", &buf);
			fscanf(fptr, "%d", &larea);
			layer[lnum - 1].LayerArea = larea;
			fscanf(fptr, "%c", &buf);
			fscanf(fptr, "%c", &buf);
			while (buf != ';')
			{
				fscanf(fptr, "%c", &buf);
				fscanf(fptr, "%d", &buf2);
				resource[i].ResourceType = buf;
				resource[i].TypeNumber = buf2;
				resource[i].ResourceLayer = lnum;
				i++;
				fscanf(fptr, "%c", &buf);
			}
			fscanf(fptr, "%c", &buf);
		}
	}
	while (!feof(fptr))
	{
		fscanf(fptr, "%c", &buf);
		if (buf == 'A')
		{
			fscanf(fptr, "%c", &buf);
			if (buf == 'r')
			{
				fscanf(fptr, "%c", &buf);
				if (buf == 'e')
				{
					fscanf(fptr, "%c", &buf);
					if (buf == 'a')
					{
						fscanf(fptr, "%c", &buf);
						if (buf == ':')
						{
							fscanf(fptr, "%c", &buf);
							break;
						}
					}
				}
			}
		}
	}
	while (!feof(fptr))
	{
		fscanf(fptr, "%c", &rtype);
		if (rtype == 'E')
		{
			fscanf(fptr, "%c", &buf);
			if (buf == 'N')
			{
				fscanf(fptr, "%c", &buf);
				if (buf == 'D')
					break;
			}
		}
		fscanf(fptr, "%c", &buf);
		fscanf(fptr, "%d", &rtypearea);
		for (i = 0; i < ResourceNumber; i++)
		{
			if (resource[i].ResourceType == rtype)
				resource[i].ResourceArea = rtypearea;
		}
		fscanf(fptr, "%c", &buf);
		fscanf(fptr, "%c", &buf);
	}
	while (!feof(fptr))
	{
		fscanf(fptr, "%c", &buf);
		if (buf == 'e')
		{
			fscanf(fptr, "%c", &buf);
			if (buf == 'n')
			{
				fscanf(fptr, "%c", &buf);
				if (buf == 'c')
				{
					fscanf(fptr, "%c", &buf);
					if (buf == 'y')
					{
						fscanf(fptr, "%c", &buf);
						if (buf == ':')
						{
							fscanf(fptr, "%c", &buf);
							break;
						}
					}
				}
			}
		}
	}
	while (!feof(fptr))
	{
		fscanf(fptr, "%c", &rtype);
		fscanf(fptr, "%d", &rnum);
		for (i = 0; i < ResourceNumber; i++)
		{
			if (resource[i].ResourceType == rtype && resource[i].TypeNumber == rnum)
				resource[i].linknum++;
		}
		fscanf(fptr, "%c", &buf);
		while (buf != ';')
			fscanf(fptr, "%c", &buf);
		fscanf(fptr, "%c", &buf);
	}
	fclose(fptr);
}
void getDependency()
{
	int i = 0, j = 0, buf2 = 0, rnum = 0;
	char buf = NULL, rtype = NULL;
	FILE *fptr;
	fptr = fopen(InputFileName, "r");
	while (!feof(fptr))
	{
		fscanf(fptr, "%c", &buf);
		if (buf == 'e')
		{
			fscanf(fptr, "%c", &buf);
			if (buf == 'n')
			{
				fscanf(fptr, "%c", &buf);
				if (buf == 'c')
				{
					fscanf(fptr, "%c", &buf);
					if (buf == 'y')
					{
						fscanf(fptr, "%c", &buf);
						if (buf == ':')
						{
							fscanf(fptr, "%c", &buf);
							break;
						}
					}
				}
			}
		}
	}
	while (!feof(fptr))
	{
		fscanf(fptr, "%c", &rtype);
		fscanf(fptr, "%d", &rnum);
		for (i = 0; i < ResourceNumber; i++)
		{
			if (resource[i].ResourceType == rtype && resource[i].TypeNumber == rnum)
			{
				fscanf(fptr, "%c", &buf);
				fscanf(fptr, "%c", &buf);
				fscanf(fptr, "%c", &buf);
				fscanf(fptr, "%c", &buf);
				fscanf(fptr, "%c", &rtype);
				fscanf(fptr, "%d", &rnum);
				for (j = 0; j < ResourceNumber; j++)
				{
					if (resource[j].ResourceType == rtype && resource[j].TypeNumber == rnum)
					{
						resource[i].link[resource[i].count] = j;
						resource[i].count++;
						break;
					}
				}
				break;
			}
		}
		fscanf(fptr, "%c", &buf);
		fscanf(fptr, "%c", &buf);
	}
	fclose(fptr);
}
void getMINTSV()
{
	int i, j;
	for (i = 0; i < ResourceNumber; i++)
	{
		for (j = 0; j < resource[i].linknum; j++)
		{
			MINTSV += abs(resource[i].ResourceLayer - resource[resource[i].link[j]].ResourceLayer);
			switch (abs(resource[i].ResourceLayer - resource[resource[i].link[j]].ResourceLayer))
			{
			case 0:
				odep[0]++;
				break;
			case 1:
				odep[1]++;
				break;
			case 2:
				odep[2]++;
				break;
			case 3:
				odep[3]++;
				break;
			case 4:
				odep[4]++;
				break;
			case 5:
				odep[5]++;
				break;
			}
		}
	}
	for (j = 0; j < LayerNumber; j++)
	{
		for (i = 1; i <= odep[j]; i++)
		{
			originDEP *= dep[j];
		}
	}
}
void Fileprint()
{
	int i, j;
	printf("檔案資料:\nLayer:\n");
	for (j = 0; j < LayerNumber; j++)
	{
		printf("L%d(%d):", j + 1, layer[j].LayerArea);
		for (i = 0; i < ResourceNumber; i++)
		{
			if (resource[i].ResourceLayer == j + 1)
				printf("%c%d(%d) ", resource[i].ResourceType, resource[i].TypeNumber, resource[i].ResourceArea);
		}
		printf(";\n");
	}
	printf("END;\n\nDependency:\n");
	for (i = 0; i < ResourceNumber; i++)
	{
		for (j = 0; j < resource[i].linknum; j++)
		{
			printf("%c%d -> %c%d\n", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[j]].ResourceType, resource[resource[i].link[j]].TypeNumber);
		}
	}
	printf("\nMINTSV:%d\n原始可靠度:%lf\nEND;\n\n", MINTSV, originDEP);
}
void buildLingoFile()
{
	int i, j, k;
	FILE  *fout;
	fout = fopen("TEST.LTF", "w");
	if (fout == NULL)
	{
		printf("Fail To Open File TEST.LTF");
		fclose(fout);
	}
	fprintf(fout, "MODEL:\n");

	/* Part 1 */
	for (i = 0; i < ResourceNumber; i++)
	{
		for (j = 0; j < LayerNumber; j++)
			fprintf(fout, "@BIN(r%c%d_%d);\n", resource[i].ResourceType, resource[i].TypeNumber, j + 1);
		fprintf(fout, "\n");
	}
	for (i = 0; i < ResourceNumber; i++)
	{
		for (k = 0; k < resource[i].linknum; k++)
		{
			fprintf(fout, "@GIN(v%c%d_%c%d);\n", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			for (j = 0; j < LayerNumber; j++)
				fprintf(fout, "@BIN(r%c%d_%c%d_%d);\n", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, j);
			fprintf(fout, "@GIN(low%c%d_%c%d);\n", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			fprintf(fout, "@GIN(high%c%d_%c%d);\n", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
		}
		fprintf(fout, "\n");
	}

	/* Part 2 */
	fprintf(fout, "!== Formula 1 =====================================;\n");
	for (i = 0; i < ResourceNumber; i++)
	{
		for (j = 0; j < LayerNumber; j++)
		{
			fprintf(fout, "r%c%d_%d", resource[i].ResourceType, resource[i].TypeNumber, j + 1);
			if (j + 1 == LayerNumber)
				fprintf(fout, "=1;\n");
			else
				fprintf(fout, "+");
		}
	}
	fprintf(fout, "\n");

	/* Part 3 */
	fprintf(fout, "!== Formula 2 =====================================;\n");
	for (j = 0; j < LayerNumber; j++)
	{
		for (i = 0; i < ResourceNumber; i++)
		{
			fprintf(fout, "r%c%d_%d*%d", resource[i].ResourceType, resource[i].TypeNumber, j + 1, resource[i].ResourceArea);
			if (i + 1 == ResourceNumber)
				fprintf(fout, "<=%d;\n", layer[j].LayerArea);
			else
				fprintf(fout, "+");
		}
	}
	fprintf(fout, "\n");

	/* Part 4 */
	fprintf(fout, "!== Formula 3 4 5 6 ===============================;\n");
	for (i = 0; i < ResourceNumber; i++)
		for (k = 0; k < resource[i].linknum; k++)
		{
			fprintf(fout, "low%c%d_%c%d<=", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			for (j = 0; j < LayerNumber; j++)
			{
				fprintf(fout, "r%c%d_%d*%d", resource[i].ResourceType, resource[i].TypeNumber, j + 1, j + 1);
				if (j + 1 == LayerNumber)
					fprintf(fout, ";\n");
				else
					fprintf(fout, "+");
			}
			fprintf(fout, "high%c%d_%c%d>=", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			for (j = 0; j < LayerNumber; j++)
			{
				fprintf(fout, "r%c%d_%d*%d", resource[i].ResourceType, resource[i].TypeNumber, j + 1, j + 1);
				if (j + 1 == LayerNumber)
					fprintf(fout, ";\n");
				else
					fprintf(fout, "+");
			}
			fprintf(fout, "low%c%d_%c%d<=", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			for (j = 0; j < LayerNumber; j++)
			{
				fprintf(fout, "r%c%d_%d*%d", resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, j + 1, j + 1);
				if (j + 1 == LayerNumber)
					fprintf(fout, ";\n");
				else
					fprintf(fout, "+");
			}
			fprintf(fout, "high%c%d_%c%d>=", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			for (j = 0; j < LayerNumber; j++)
			{
				fprintf(fout, "r%c%d_%d*%d", resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, j + 1, j + 1);
				if (j + 1 == LayerNumber)
					fprintf(fout, ";\n");
				else
					fprintf(fout, "+");
			}
		}
	fprintf(fout, "\n");

	/* Part 5 */
	fprintf(fout, "!== Formula 7 =====================================;\n");
	for (i = 0; i < ResourceNumber; i++)
		for (k = 0; k < resource[i].linknum; k++)
		{
			for (j = 0; j < LayerNumber; j++)
				fprintf(fout, "(1-r%c%d_%d)*%d+", resource[i].ResourceType, resource[i].TypeNumber, j + 1, j + 1);
			for (j = 0; j < LayerNumber; j++)
				fprintf(fout, "(1-r%c%d_%d)*%d+", resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, j + 1, j + 1);
			fprintf(fout, "low%c%d_%c%d+", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			fprintf(fout, "high%c%d_%c%d=", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			fprintf(fout, "2*(");
			for (j = 0; j < LayerNumber; j++)
			{
				fprintf(fout, "%d", j + 1);
				if (j + 1 == LayerNumber)
					fprintf(fout, ");\n");
				else
					fprintf(fout, "+");
			}
		}
	fprintf(fout, "\n");

	/* Part 6 */
	fprintf(fout, "!==================================================;\n");
	for (i = 0; i < ResourceNumber; i++)
		for (k = 0; k < resource[i].linknum; k++)
		{
			fprintf(fout, "v%c%d_%c%d=high%c%d_%c%d-low%c%d_%c%d;\n", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
		}
	fprintf(fout, "\n");

	/* Part 7 */
	int l = 0;
	fprintf(fout, "!==================================================;\n");
	for (i = 0; i < ResourceNumber; i++)
		for (k = 0; k < resource[i].linknum; k++)
		{
			for (j = 0; j < LayerNumber; j++)
			{
				fprintf(fout, "r%c%d_%d+r%c%d_%d<=r%c%d_%c%d_0+1;\n", resource[i].ResourceType, resource[i].TypeNumber, j + 1, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, j + 1, resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			}
			for (l = 1; l < LayerNumber; l++)
			{
				for (j = 0; j < LayerNumber - l; j++)
				{
					fprintf(fout, "r%c%d_%d+r%c%d_%d<=r%c%d_%c%d_%d+1;\n", resource[i].ResourceType, resource[i].TypeNumber, j + 1, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, j + 1 + l, resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, l);
				}
				for (j = LayerNumber; j >l; j--)
				{
					fprintf(fout, "r%c%d_%d+r%c%d_%d<=r%c%d_%c%d_%d+1;\n", resource[i].ResourceType, resource[i].TypeNumber, j, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, j - l, resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, l);
				}
			}
			fprintf(fout, "\n");
		}
	fprintf(fout, "\n");

	/* Part 8 */
	int bufcount = 0, buf = 0;
	fprintf(fout, "!==================================================;\n");
	for (i = 0; i < ResourceNumber; i++)
		for (k = 0; k < resource[i].linknum; k++)
		{
			for (j = 0; j < LayerNumber; j++)
			{
				fprintf(fout, "r%c%d_%c%d_%d", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, j);
				if (j + 1 == LayerNumber)
				{
					fprintf(fout, "=1;\n");
					bufcount++;
				}
				else
					fprintf(fout, "+");
			}
		}
	fprintf(fout, "\n");

	/* TEST */
	buf = 0;
	fprintf(fout, "!== TEST ==========================================;\n");
	for (i = 0; i < ResourceNumber; i++)
		for (k = 0; k < resource[i].linknum; k++)
		{
			fprintf(fout, "v%c%d_%c%d", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			buf++;
			if (buf != bufcount)
				fprintf(fout, "+");
		}
	fprintf(fout, "<=%d;\n\n", MINTSV);

	/* Part 9 */
/*	buf = 0;
	fprintf(fout, "min=");
	for (i = 0; i < ResourceNumber; i++)
		for (k = 0; k < resource[i].linknum; k++)
		{
			fprintf(fout, "v%c%d_%c%d", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
			buf++;
			if (buf != bufcount)
				fprintf(fout, "+");
		}
	fprintf(fout, ";");*/

	buf = 0;
	fprintf(fout, "!== Objective Function ===========================;\n");
	fprintf(fout, "MAX=");
	for (j = 0; j < LayerNumber; j++)
	{
		fprintf(fout, "@log(%lf)*(", dep[j]);		
		for (i = 0; i < ResourceNumber; i++)
			for (k = 0; k < resource[i].linknum; k++)
			{
				//fprintf(fout, "v%c%d_%c%d", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber);
				fprintf(fout, "r%c%d_%c%d_%d", resource[i].ResourceType, resource[i].TypeNumber, resource[resource[i].link[k]].ResourceType, resource[resource[i].link[k]].TypeNumber, j);
				buf++;
				if (buf != bufcount)
					fprintf(fout, "+");
			}
		if (j + 1 == LayerNumber)
			fprintf(fout, ");\n");
		else
			fprintf(fout, ")+\n");
		buf = 0;
	}
	fprintf(fout, "\n");

	fprintf(fout, "END\n");
	fprintf(fout, "SET TERSEO 1\n");
	fprintf(fout, "GO\n");
	fprintf(fout, "DIVERT SOLU.TXT\n");
	fprintf(fout, "SOLUTION\n");
	fprintf(fout, "RVRT\n");
	fprintf(fout, "QUIT\n");
	fclose(fout);
}
void startLingo()
{
	char command[50] = {};
	strcpy(command, "D:\\Downloads\\LINGO13\\LINGO13\\Lingo13.exe -tTEST.LTF");
	system(command);
}
void getoutput()
{
	FILE  *fp;
	double ans = NULL;
	char test[1000] = {};
	fp = fopen("SOLU.TXT", "r");
	while (!feof(fp))
	{
		fscanf(fp, "%s", &test);
		if (strcmp(test, "Objective") == 0)
		{
			fscanf(fp, "%s", &test);
			if (strcmp(test, "value:") == 0)
			{
				fscanf(fp, "%lf", &ans);
				if (ans != NULL)
				{
					if (exp(ans) > originDEP)
					{
						printf("此階層圖的可靠度提升至%f\n", exp(ans));
						getNewResourceLayer();
						printNewResourceLayer();
					}
					else
						printf("此階層圖已達最高可靠度\n");
				}
			}
		}
	}
	fclose(fp);
}
void getNewResourceLayer()
{
	FILE  *fp;
	int i, j;
	double check = NULL;
	int typenumbuf = NULL, layerbuf = NULL;
	char test[1000] = {}, c = NULL, typebuf = NULL;
	fp = fopen("SOLU.TXT", "r");
	while (!feof(fp))
	{
		fscanf(fp, "%s", &test);
		if (strcmp(test, "Reduced") == 0)
		{
			fscanf(fp, "%s", &test);
			if (strcmp(test, "Cost") == 0)
			{
				fscanf(fp, "%c", &c);
				for (j = 0; j < ResourceNumber*LayerNumber; j++)
				{
					do {
						fscanf(fp, "%c", &c);
					} while (c == ' ');
					if (c == 'R')
					{
						fscanf(fp, "%c", &typebuf);
						fscanf(fp, "%d", &typenumbuf);
						fscanf(fp, "%c", &c);
						fscanf(fp, "%d", &layerbuf);
						fscanf(fp, "%lf", &check);
						if ((int)check == 1)
						{
							for (i = 0; i < ResourceNumber; i++)
							{
								if (resource[i].ResourceType == typebuf && resource[i].TypeNumber == typenumbuf)
								{
									resource[i].NewResourceLayer = layerbuf;
									break;
								}
							}
						}
						fscanf(fp, "%s", &test);
						fscanf(fp, "%c", &c);
						check = NULL;
					}
				}
			}
		}
	}
	fclose(fp);
}
void printNewResourceLayer()
{
	int i, j;
	printf("\n改良後的階層圖如下\n");
	for (j = 0; j < LayerNumber; j++)
	{
		printf("L%d:", j + 1);
		for (i = 0; i < ResourceNumber; i++)
		{
			if (resource[i].NewResourceLayer == j + 1)
			{
				printf(" %c%d", resource[i].ResourceType, resource[i].TypeNumber);
			}
		}
		printf("\n");
	}
}