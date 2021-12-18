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
#include <time.h>
#include <sys/time.h>
#include <omp.h>

#define MAX_RANDOM 5;

double runOMP(int SIZE);
double runSequential(int SIZE);
void writeFile(int SIZE, char name[20]);
int main();

double runOMP(int SIZE)
{
    int i,j,k;
    struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed; 
    int A[SIZE][SIZE];
    int B[SIZE][SIZE];
    int C[SIZE][SIZE];
    omp_set_num_threads(omp_get_num_procs());

    for (i= 0; i< SIZE; i++)
        for (j= 0; j< SIZE; j++)
	{
            A[i][j] = rand() % MAX_RANDOM;;
            B[i][j] = rand() % MAX_RANDOM;;

	}

    gettimeofday(&tv1, &tz);

    #pragma omp parallel for private(i,j,k) shared(A,B,C)

    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            for (k = 0; k < SIZE; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    elapsed = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    printf("elapsed time for OMP run = %f seconds.\n", elapsed);

    // for (i= 0; i< N; i++)
    // {
    //     for (j= 0; j< N; j++)
    //     {
    //         printf("%d\t",C[i][j]);
    //     }
    //     printf("\n");
    // }

    return elapsed;
}

double runSequential(int SIZE) 
{
    int i,j,k;
    struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed; 

    int A[SIZE][SIZE];
    int B[SIZE][SIZE];
    int C[SIZE][SIZE];

    for (i= 0; i< SIZE; i++)
        for (j= 0; j< SIZE; j++)
	{
            A[i][j] = rand() % MAX_RANDOM;;
            B[i][j] = rand() % MAX_RANDOM;;

	}

    gettimeofday(&tv1, &tz);

    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            for (k = 0; k < SIZE; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&tv2, &tz);
    elapsed = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    printf("elapsed time for sequential run = %f seconds.\n", elapsed);

    // for (i= 0; i< N; i++)
    // {
    //     for (j= 0; j< N; j++)
    //     {
    //         printf("%d\t",C[i][j]);
    //     }
    //     printf("\n");
    // }

    return elapsed;
}

void writeFile(int SIZE, char name[20]){
    FILE *fp;
    fp = fopen(name, "w+");
    fprintf(fp, "Secuencial, OMP");
    for (int i = 0; i<10; i++){
        fprintf(fp, "\n%.5f, ", runSequential(SIZE));
        fprintf(fp, "%.5f, ", runOMP(SIZE));

    }
    fclose(fp);
}

int main()
{
    srand(time(NULL)); // SEMILLA
    int SIZE = 100; // TAMAÑO
    char nombres[6][20] = {"100.csv", "200.csv", "400.csv", "800.csv", "1600.csv", "3200.csv"};
    for (int i = 0; i < 4; i++){
        writeFile(SIZE, nombres[i]);
        SIZE = SIZE * 2;

    }
    return 0;
}
