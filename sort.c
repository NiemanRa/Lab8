#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) // If Left is Less than Right
	{
		int m = (l + r) / 2; // Middle

		/* Merge! */
		int i = 0; // Counter for Left
		int j = 0; // Counter for Right
		int n2 = m + 1; // Right-Middle Value

		// Temps
		int* tempLeft = (int*) Alloc(sizeof(int) * (m + 1));
		int* tempRight = (int*) Alloc(sizeof(int) * (r - m));

		// Copy Data to Temps
		for (i = 0; i <= m; i++)
			tempLeft[i] = pData[i];
		for (j = i; j <= r; j++)
			tempRight[j - n2] = pData[j];

        mergeSort(tempLeft, 0, m);
        mergeSort(tempRight, 0, r - m - 1);

		// Merge Temps
		i = 0; // Reset
		j = 0; // Reset
		int k = 0;

		while (i <= m && j < (r - m))
		{
			if (tempLeft[i] <= tempRight[j])
			{
				pData[k] = tempLeft[i];
				i++;
			}

			else
			{
				pData[k] = tempRight[j];
				j++;
			}

			k++;
		}

		// Copy the Any Remaining Elements
		while (i <= m)
		{
			pData[k] = tempLeft[i];
			i++;
			k++;
		}

		while (j <= (r - n2))
		{
			pData[k] = tempRight[j];
			j++;
			k++;
		}
		DeAlloc(tempLeft);
		DeAlloc(tempRight);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
	int firstHundred = (dataSz < 100 ? dataSz : 100);
	printf("\tData:\n\t");

	for (i = 0; i < firstHundred; ++i)
	{
		printf("%d ",pData[i]);
	}

	printf("\n\t");

	for (i = sz ; i < dataSz; ++i)
	{
		printf("%d ",pData[i]);
	}

	printf("\n\n");
}


int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

	for (i = 0; i < 4; ++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int *)malloc(sizeof(int)*dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		free(pDataCopy);
		free(pDataSrc);
	}

}
