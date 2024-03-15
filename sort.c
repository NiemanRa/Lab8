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
	// printf("Extra memory allocated, size: %ld\n", sz); <---- UNCOMMENT LATER
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	// printf("Extra memory deallocated, size: %ld\n", *pSz); // <---- UNCOMMENT LATER
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

void printArray(int pData[], int dataSz);

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	// LEFT IS 0!  AND RIGHT IS DATA SIZE - 1!

	if (l < r) // If Left is Less than Right
	{
		int m = (l + r) / 2; // Middle

		mergeSort(pData, l, m); // Sorts Left
		mergeSort(pData, m + 1, r); // Sorts Right - Floored Uneven Number Needs +1

		// printf("Testing l = %d r = %d m = %d\n", l, r, m);

		/* Merge! */
		int i; // Counter for Left
		int j; // Counter for Right
		int n2 = m + 1; // Right-Middle Value

		// printf("%d and %d\n", n1, n2);

		// Temps
		int* tempLeft = (int*) Alloc(sizeof(int) * (m + 1));
		int* tempRight = (int*) Alloc(sizeof(int) * (r - m + 1));

		printf("Do things work past Alloc?\n");

		// Copy Data to Temps
		for (i = 0; i <= m; i++)
			tempLeft[i] = pData[i];
		for (j = i; j <= n2; j++)
			tempRight[j - n2] = pData[j];

		printf("Do things work past copy temps?\n");
		printArray(tempLeft, m + 1);
		printArray(tempRight, r - m + 1);

		// Merge Temps
		i = 0; // Reset
		j = 0; // Reset
		int k = 0;

		while (i <= m && j <= (r - n2))
		{
			printf("%d, %d, %d\n", i, j, m);
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

		// printf("Do things work past merging?\n");
		// printf("i = %d, j = %d, k = %d\n", i, j, k);

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

		// printf("Do things work past copying remaining elements?\n");
		// printf("%p\n", tempLeft);

		DeAlloc(tempLeft);
		// printf("Free Left?\n");
		DeAlloc(tempRight);

		// printf("Do things work past DeAlloc?\n");
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
