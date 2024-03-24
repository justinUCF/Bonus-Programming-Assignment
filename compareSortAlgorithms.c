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
	//printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	//printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int pData[], int n)
{
	int max, rightchild, temp1, temp2, temp3, i, j;
	for (i = n / 2 - 1; i >= 0; i--) {
		for (j = i; (j * 2) + 1 < n; j = max) {
			max = j * 2 + 1;
			rightchild = max + 1;
			if (pData[max] < pData[rightchild] && n > rightchild)
				max++;
			if (pData[j] < pData[max]) {
				//Swap 1
				temp1 = pData[j];
				pData[j] = pData[max];
				pData[max] = temp1;

			}
			else
				break;
		}
	}

	for (i = n - 1; i > 0; i--) {
		//Swap 2
		temp2 = pData[0];
		pData[0] = pData[i];
		pData[i] = temp2;

		for (j = 0; j * 2 + 1 < i; j = max) {
			max = j * 2 + 1;
			if (max + 1 < i && pData[max] < pData[max + 1])
				max++;
			if (pData[j] < pData[max]) {
				//Swap 3
				temp3 = pData[j];
				pData[j] = pData[max];
				pData[max] = temp3;
			}
			else
				break;
		}
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r){
		int middle  = (l+r)/2;
		
		mergeSort(pData, l, middle);
		mergeSort(pData, middle+1, r);

		//Merge start
		int i, j, k;
		int leftSize = middle - l + 1;
		int rightSize = r - middle;

		int *L = (int*)Alloc(leftSize*sizeof(int));
		int *R = (int*)Alloc(rightSize*sizeof(int));

		for (i = 0; i < leftSize; i++){
			L[i] = pData[l + i];
		}
		for (j = 0; j < rightSize; j++){
			R[j] = pData[middle + 1 + j];
		}
		i=0; 
		j=0; 
		k=l;
		while (i < leftSize && j < rightSize){
			if (L[i] <= R[j])
				pData[k++] = L[i++];
			else
				pData[k++] = R[j++];
		}
		while (i < leftSize){
			pData[k++] = L[i++];
		}
		while (j < rightSize){
			pData[k++] = R[j++];
		}
		DeAlloc(L);
		DeAlloc(R);
		//Merge end
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, item, j;
	for (i = 1; i < n; i++){
		item = pData[i];
		for (j = i-1; j >= 0; j--){
			if(pData[j] > item)
				pData[j+1] = pData[j];
			else
				break;
		}
		pData[j+1] = item;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	int i, j, temp;
	for (i = 0; i < n-1; i++){
		for (j = 0; j < n-i-1; j++){
			if (pData[j] > pData[j+1]){
				temp = pData[j];
				pData[j] = pData[j+1];
				pData[j+1] = temp;
			}
		}
	}

}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i,j, min_index, temp;
	for (i = 0; i < n-1; i++){
		min_index = i;
		for (j = i+1; j < n; j++){
			if (pData[j] < pData[min_index]){
				temp = pData[j];
				pData[j] = pData[min_index];
				pData[min_index] = temp;
			}
		}
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
		*ppData = (int*)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL){
			printf("ERROR: parseData() failed\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i){
			fscanf (inFile, "%d", &n);
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
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
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
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
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

        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
