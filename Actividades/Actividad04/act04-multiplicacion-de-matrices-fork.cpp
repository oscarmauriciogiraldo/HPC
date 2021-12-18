/*
  ============================================================================
  Name        : matrices-fork.c
  Author      : Oscar Mauricio Giraldo Herrera
  ID          : 1 088 322 329
  Version     : 0.5
  Description : Caso de estudio multiplicacion de matrices utilizando procesos Fork()
  ============================================================================
*/

//Librerias:
// para mostrar o imprimir mensajes por pantalla 
#include <stdio.h>
//libreria para exit()
#include <stdlib.h>
#include <unistd.h>
// libreria para el relog
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include<sys/wait.h>
#include <string.h>


#define MAX_RANDOM 5

//Definicion de variables 

int *matrixA, *matrixResult;

void fillMatrix(int* matrix, int SIZE);
void* create_shared_memory(size_t size);
void printMatrix(int* matrix, int SIZE);

void multiplyMatrix(int* matrix, int* result, int SIZE);
void multiplyMatrixByRows(int* matrix, int* result, int SIZE, int begin, int end);
void multiplyMatrixByColumns(int* matrix, int* result, int SIZE, int begin, int end);

double runSecuential(int SIZE);
double measureProcessesOperation(int nprocesses, void(*operation)(int*, int*, int, int, int), int* matrix, int* result, int SIZE);
void createProcesses(int nprocesses, void(*operation)(int*, int*, int, int, int), int* matrix, int* result, int SIZE);

void generateTimes(char name[20], int* matrix, int* result, int SIZE);

// Fuhncion principal 
int main(){
    srand(time(NULL));
    char nombres[6][20] = {"100.csv", "200.csv", "400.csv", "800.csv", "1600.csv", "3200.csv"};
    int SIZE = 100; // TAMAÑO
    for(int i = 0; i < 6; i++){
        matrixA = (int*) malloc(SIZE * SIZE * sizeof(int)); // Memory allocation
        matrixResult = (int*) malloc(SIZE * SIZE * sizeof(int));
        fillMatrix(matrixA, SIZE);
        void* shmem_matrixA = create_shared_memory(SIZE * SIZE * sizeof(int)); // shared memory = memoria compartida
        void* shmem_result = create_shared_memory(SIZE * SIZE * sizeof(int));
        memcpy(shmem_matrixA, matrixA, SIZE * SIZE * sizeof(int));
        generateTimes(nombres[i], (int *)shmem_matrixA, (int *)shmem_result, SIZE);
        free(matrixA);
        free(matrixResult);
        munmap(shmem_matrixA, SIZE * SIZE * sizeof(int));
        munmap(shmem_result, SIZE * SIZE * sizeof(int));
        SIZE = SIZE * 2;
    }
    return 0;
}


// funcion para generar memoria 
void* create_shared_memory(size_t size){
    // Buffer de memoria puede ser leído y escrito
    int protection = PROT_READ | PROT_WRITE;
    /* 
        El buffer será compartido, lo que significa que otros
        procesos pueden acceder a él, pero también anónimo, lo
        que significa que procesos externos al programa no pueden
        obtener una dirección de este. Solo el proceso actual y sus
        hijos pueden usarlo
    */
   int visibility = MAP_SHARED | MAP_ANONYMOUS;
    /*
        mmap(address, size, protection, flags, fd, offset)
        Cuando la bandera "MAP_ANONYMOUS" está activada, el "fd" (file descriptor)
        es ignorado. Sin embargo, algunas implementaciones requieren que sea -1.
        El offset debe ser 0 cuando se usa la bandera "MAP_ANONYMOUS"
    */
   return mmap(NULL, size, protection, visibility, -1, 0);
}

// funcion para crear procesos
void createProcesses(int nprocesses, void(*operation)(int*, int*, int, int, int), int* matrix, int* result, int SIZE){
    pid_t pid; // Process ID
    int fragmented_size = SIZE/nprocesses;
    for(int i = 0; i < nprocesses; i++){
        pid = fork();
        if (pid < 0){
            perror("fork");
            abort();
        } else if (pid == 0){
            // Hijo
            int begin, end;
            begin = fragmented_size * i;
            if ( i == (nprocesses - 1)) end = SIZE;
            else end = fragmented_size * (i + 1);
            operation(matrix, result, SIZE, begin, end);
            exit(0);
        }
    }
}

void fillMatrix(int* matrix, int SIZE){
    for(int row = 0; row < SIZE; row++){
        for (int column = 0; column < SIZE; column++){
            *(matrix + (row * SIZE) + column) = rand() % MAX_RANDOM;
        }
    }
}

void printMatrix(int* matrix, int SIZE){
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            printf("%d\t", *(matrix + (i * SIZE) + j));
        }
        printf("\n");
    }
}

void multiplyMatrix(int* matrix, int* result, int SIZE){
    for(int row_A = 0; row_A < SIZE; row_A++){
        for (int column_A = 0; column_A < SIZE; column_A++){
            int sum = 0;
            for (int row_B = 0; row_B < SIZE; row_B++){
                sum += *(matrix + (row_A * SIZE) + row_B) * *(matrix + (row_B * SIZE) + column_A); 
            }
            *(result + (row_A * SIZE) + column_A) = sum;
        }
    }
}

void multiplyMatrixByRows(int* matrix, int* result, int SIZE, int begin, int end){
    for(int row_A = begin; row_A < end; row_A++){
        for (int column_A = 0; column_A < SIZE; column_A++){
            int sum = 0;
            for (int row_B = 0; row_B < SIZE; row_B++){
                sum += *(matrix + (row_A * SIZE) + row_B) * *(matrix + (row_B * SIZE) + column_A); 
            }
            *(result + (row_A * SIZE) + column_A) = sum;
        }
    }
}

void multiplyMatrixByColumns(int* matrix, int* result, int SIZE, int begin, int end){
    for(int row_A = 0; row_A < SIZE; row_A++){
        for (int column_A = begin; column_A < end; column_A++){
            int sum = 0;
            for (int row_B = 0; row_B < SIZE; row_B++){
                sum += *(matrix + (row_A * SIZE) + row_B) * *(matrix + (row_B * SIZE) + column_A); 
            }
            *(result + (row_A * SIZE) + column_A) = sum;
        }
    }
}

double runSecuential(int SIZE){
    // START RECORDING TIME ELAPSED
    struct timeval begin, end;
    gettimeofday(&begin, 0);

    multiplyMatrix(matrixA, matrixResult, SIZE);
    
    // STOP RECORDING TIME ELAPSED
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    ///////////////////////
    // printf("Tiempo secuencial: %.3f segundos\n", elapsed);
    return elapsed;
}

double measureProcessesOperation(int nprocesses, void(*operation)(int*, int*, int, int, int), int* matrix, int* result, int SIZE){
    // START RECORDING TIME ELAPSED
    struct timeval begin, end;
    int n = nprocesses, status;
    pid_t pid;
    gettimeofday(&begin, 0);
    createProcesses(nprocesses, operation, matrix, result, SIZE);
    // STOP RECORDING TIME ELAPSED
    while (n > 0){
            pid = wait(&status);
            --n;
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    ///////////////////////
    return elapsed;
}


// generar tiempos
void generateTimes(char name[20], int* matrix, int* result, int SIZE){
    double times[10][7];
    for (int i = 0; i<10; i++){
        times[i][0] = runSecuential(SIZE);
        times[i][1] = measureProcessesOperation(2, multiplyMatrixByRows, matrix, result, SIZE);
        times[i][2] = measureProcessesOperation(2, multiplyMatrixByColumns, matrix, result, SIZE);
        times[i][3] = measureProcessesOperation(3, multiplyMatrixByRows, matrix, result, SIZE);
        times[i][4] = measureProcessesOperation(3, multiplyMatrixByColumns, matrix, result, SIZE);
        times[i][5] = measureProcessesOperation(4, multiplyMatrixByRows, matrix, result, SIZE);
        times[i][6] = measureProcessesOperation(4, multiplyMatrixByColumns, matrix, result, SIZE);
    }
    FILE *fp;
    fp = fopen(name, "w+");
    fprintf(fp, "\nSecuencial, Filas, Columnas, Filas, Columnas, Filas, Columnas\n");
    for (int i = 0; i < 10; i ++){
        for (int j = 0; j < 7; j++){
            fprintf(fp, "%.5f, ", times[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
