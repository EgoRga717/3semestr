#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 3
#define M 4
#define L 5
//First - NxM, second - MxL, result - NxL
typedef struct{
	int strk;
	int stlb;
	int** array1;
	int** array2;
	int** resArr;
} pthrData;

void* threadFunc(void* thread_data)
{
    int i;
	pthrData *data = (pthrData*) thread_data;
	for(i = 0; i < M; i++)
		data->resArr[strk][stlb] += data->array1[strk][i] * data->array2[i][stlb];
    printf("%d ", data->resArr[strk][stlb]);
	return NULL;
}

int main()
{
	int** matrix1 = (int**) malloc(N * sizeof(int*));
	int** matrix2 = (int**) malloc(M * sizeof(int*));
	int** resultMatrix = (int**) malloc(N * sizeof(int*));
    int i, j, k;
	for(i = 0; i < M; i++)
    {
		matrix1[i] = (int*) malloc(M * sizeof(int));
		matrix2[i] = (int*) malloc(L * sizeof(int));
		resultMatrix[i] = (int*) malloc(L * sizeof(int));
	}

	for(i = 0; i < N; i++)
		for(j = 0; j < M; j++)
			matrix1[i][j] = 2;
    for(i = 0; i < M; i++)
		for(j = 0; j < L; j++)
			matrix1[i][j] = 3;
    for(i = 0; i < N; i++)
		for(j = 0; j < L; j++)
			matrix1[i][j] = 0;

	pthread_t* threads = (pthread_t*) malloc(N * L * sizeof(pthread_t));
	pthrData* threadData = (pthrData*) malloc(N * L * sizeof(pthrData));

	for(i = 0; i < N; i++)
    {
        for(j = 0; j < L; j++)
        {
            threadData[i].strk = i;
            threadData[i].stlb = j;
            threadData[i].array1 = matrix1;
            threadData[i].array2 = matrix2;
            threadData[i].resArr = resultMatrix;
            pthread_create(&(threads[k]), NULL, threadFunc, &threadData[k]);
            ++k;
        }
        printf("\n");
    }

	for(int i = 0; i < N * L; i++)
		pthread_join(threads[i], NULL);

	free(threads);
	free(threadData);
	for(i = 0; i < N; i++)
		free(matrix1[i]);
    for(i = 0; i < M; i++)
		free(matrix2[i]);
    for(i = 0; i < N; i++)
		free(resultMatrix[i]);
	free(matrix1);
	free(matrix2);
	free(resultMatrix);
	return 0;
}
