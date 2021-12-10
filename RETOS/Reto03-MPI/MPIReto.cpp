#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <functional>
#include <time.h>
#include <mpi.h>

using namespace std;

class MatrixMultiplicationMPI {

    void writeTime(double elapsed, int len, int npro)
    {
        /*
			Wite the result on output.txt file
			M -> Matrix, Mrow -> Matrix rows, Mcol -> Matrix columns
		*/
        FILE *f = fopen("timescmpiMatrices.txt", "a+"); //write at end of file and set result, append
        //float value=;
        fprintf(f, "%i;%i;%.3lf\n", npro, len, elapsed);
        fclose(f);
    }

    void exec(int argc, char *argv[])
    {
        //int n = atoi(argv[1]); //matrixes are n x n
        int n = 2000;
        //Allocating Memory and Assigning Values

        //Start MPI
        //Finding Matrix Product  and Printing
        int numranks, rank, len;
        char hostname[MPI_MAX_PROCESSOR_NAME];

        double *mat1 = (double *)malloc(n * n * sizeof(double));
        double *mat2 = (double *)malloc(n * n * sizeof(double));

        double startTime;
        double endTime;
        double tiempo;
        srand(time(NULL));
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numranks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Get_processor_name(hostname, &len);

        double *scatterMat = (double *)malloc((n + 1) * sizeof(double));
        double *gatherMat = (double *)malloc((n + 1) * sizeof(double));
        double *result = (double *)malloc(n * n * sizeof(double));

        if (rank == 0 && n % numranks != 0)
        {
            //printf("N: %d, numranks: %d\n", n, numranks);
            exit(1);
        }

        if (rank == 0)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    mat1[i * n + j] = rand() % 11;
                    mat2[i * n + j] = rand() % 11;
                }
            }
            //printf("Num tasks %d\nN: %d\n", numranks, n);
        }
        //Matrix B is copied to every processor
        MPI_Bcast(mat2, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        /*Print matricies
        if(rank == 0)
        {
        printf("Matrix A\n");
        printMat(mat1, n);
        printf("Matrix B\n");
        printMat(mat2, n);
        }*/

        //Matrix A is divided into blocks along the rows and distributed
        //among processors.
        int root = 0;
        startTime = MPI_Wtime();

        for (int x = 0; x < n * n; x = x + numranks * n)
        {
            MPI_Scatter(&mat1[x], n, MPI_DOUBLE, scatterMat, n, MPI_DOUBLE, root, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);

            double sum = 0.0;

            //Multiply
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    sum = sum + mat2[j * n + i] * scatterMat[j];
                }
                gatherMat[i] = sum;
                sum = 0.0;
            }

            MPI_Gather(gatherMat, n, MPI_DOUBLE, &result[x], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
        }

        endTime = MPI_Wtime();
        if (rank == 0)
        {
            tiempo = endTime - startTime;
            writeTime(tiempo, n, numranks);
        }
        /* printf("Time to complete %f\n", endTime - startTime);
        
        if(rank == 0)
        {
            printf("Result\n");
            printMat(result, n);
        }*/

        MPI_Finalize();
    }

    //Print singular matrix
    void printMat(double *mat, int n)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%.0f\t", mat[i * n + j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

class MatrixMultiplicationOpenMP {
    /**
    * Initialize a matrix.
    *
    * @param dimensions Number of dimensions.
    * @return Matrix initialized
    */
    int** initializeMatrix(int dimensions)
    {
        int** a = new int*[dimensions];
        for (size_t i = 0; i < dimensions; i++)
        {
            a[i] = new int[dimensions];
            for (size_t j = 0; j < dimensions; j++)
            {
                a[i][j] = rand() % 100;
            }
        }
        return a;
    }

    /**
    * Visualize a determinate matrix.
    *
    * @param a Matrix instance
    * @param dimensions Matrix dimensions number
    */
    void visualizeArray(int** a, int dimensions)
    {
        for (size_t i = 0; i < dimensions; i++)
        {
            for (size_t j = 0; j < dimensions; j++)
            {
                cout << a[i][j] << "|";
                if (j == dimensions - 1)
                {
                    cout << endl;
                }
            }
        }
    }

    /**
    * Compute the  result of a cell of matrix multiplication.
    *
    * @param a Matrix A
    * @param b Matrix B
    * @param i Column index
    * @param dimensions Matrix dimensions number
    * @param sum Result of a multiplication cell
    */
    void computeCell(int** &a, int** &b, int i, int dimensions, int* &sum)
    {
        for (size_t j = 0; j < dimensions; j++)
        {
            sum[j] = 0;
            for (size_t k = 0; k < dimensions; k++)
            {
                sum[j] += a[i][k] * b[k][j];
            }
        }
    }

    /**
    * Assignment the matrix multiplication at threads.
    *
    * @param a Matrix A.
    * @param b Matrix B.
    * @param dimensions Matrix dimension number
    * @return Matrix multiplication between A and B
    */
    int** assingMult(int** a, int** b, int dimensions)
    {
        int** mult = new int*[dimensions];
        #pragma omp parallel for num_threads(dimensions)
        for (size_t i = 0; i < dimensions; i++)
        {
            //cout << "Hilo id: " << omp_get_thread_num() << endl;
            mult[i] = new int[dimensions];
            int* &result = mult[i];
            computeCell(ref(a), ref(b), i, dimensions, ref(result));
        }
        return mult;
    }

    /**
    * Main function.
    * 
    * @return State of the process
    */
    void exec()
    {
        int** a;
        int** b;
        int** mult;
        int dimensions;
        cout << "Ingrese el número de dimensiones: ";
        cin >> dimensions;
        a = initializeMatrix(dimensions);
        b = initializeMatrix(dimensions);
        Timer t;
        mult = assingMult(a, b, dimensions);
        /* cout << "Elementos de la matriz A: " << endl;
        visualizeArray(a, dimensions);
        cout << "Elementos de la matriz B: " << endl;
        visualizeArray(b, dimensions);
        cout << "Resultado: " << endl;
        visualizeArray(mult, dimensions); */

        cout << "Tiempo de ejecución: " << t.elapsed() << "ms" << endl;

        return 0;
    }
}

int main(int argc, char *argv[]) {
    MatrixMultiplicationMPI mpi().exec(int argc, char *argv[]);
    MatrixMultiplicationOpenMP opemMp().exec();
}