#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

struct pair
{
    double value;
    int index;
};

void printVector(double *x, const int size);
void printMatrix(double *x, const int N, const int M);
double* mult(double *A, double *x, const int N, const int M);

int main (int argc, char* argv[])
{
    const int M = 1;
    int errCode, size, currRank;

    if ((errCode = MPI_Init(&argc, &argv)) != 0)
    {
        return errCode;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &currRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = M * size;
    double A[N][N], x[N], A_map[M * N], *y = calloc(N, sizeof(double));
    int root = 0;

    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        x[i] = (double)(rand() % 50 - 20);
        for (int j = 0; j < N; j++)
            A[i][j] = (double)(rand() % 50 - 20);

    }

    MPI_Scatter(*A, M*N, MPI_DOUBLE, A_map, M*N, MPI_DOUBLE, root, MPI_COMM_WORLD);
    MPI_Bcast(x, N, MPI_DOUBLE, root, MPI_COMM_WORLD);

    double *res = mult(A_map, x, M, N);
    MPI_Gather(res, M, MPI_DOUBLE, y, M, MPI_DOUBLE, root, MPI_COMM_WORLD);

    if (currRank == 0)
    {
        printMatrix(*A, N, N);
        printf("=======================\n");
        printVector(x, N);

        printf("=======================\n");
        printVector(y, N);
    }

    MPI_Finalize();
    return 0;
}

void printVector(double *x, const int size)
{
    for (int i = 0; i < size; i++)
        printf("%f ", x[i]);

    printf("\n");
}

void printMatrix(double *A, const int N, const int M)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            printf("%f ", A[i * M + j]);
        printf("\n");
    }

    printf("\n");
}

double* mult(double *A, double *x, const int N, const int M)
{
    double *result = calloc(N, sizeof(double));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            result[i] += A[i * N + j] * x[j];
    }
    return result;
}
