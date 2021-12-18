/*
  ============================================================================
  Name        : Calculo PI Monte Carlo
  Author      : Oscar Mauricio Giraldo Herrera
  Version     : 5
Description : RETO 08 OPTIMIZACIÓN OMP EN C++
  ============================================================================
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>


#define N 1000
#define MAX_RANDOM 5

int A[N][N];
int B[N][N];
int C[N][N];

int main() 
{
    int i,j,k;
    struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed; 

    omp_set_num_threads(omp_get_num_procs());

    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
	{
            A[i][j] = rand() % MAX_RANDOM;;
            B[i][j] = rand() % MAX_RANDOM;;

	}

    gettimeofday(&tv1, &tz);

    #pragma omp parallel for private(i,j,k) shared(A,B,C)

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    elapsed = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    printf("elapsed time = %f seconds.\n", elapsed);

    for (i= 0; i< N; i++)
    {
        for (j= 0; j< N; j++)
        {
            printf("%d\t",C[i][j]);
        }
        printf("\n");
    }
}
