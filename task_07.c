#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

void printVector(double *x, const int size);
double norm(double *x, const int size);

int main (int argc, char* argv[])
{
    const int M = 10;
    int errCode, size, currRank, N;
    double r[M], *x, n;

    if ((errCode = MPI_Init(&argc, &argv)) != 0)
    {
        return errCode;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &currRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL));

    N = M * size;
    x = calloc(N, sizeof(double));

    for (int i = 0; i < N; i++)
        x[i] = (double)(rand() % 1000 - 500) / 10;

    MPI_Scatter(x, M, MPI_DOUBLE, r, M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double res = norm(x, M);
    MPI_Reduce(&res, &n, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (currRank == 0)
    {
        printVector(x, N);
        printf("norm = %f\n", n);
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

double norm(double *x, const int size)
{
    double result = 0;
    for (int i = 0; i < size; i++)
        result += fabs(x[i]);

    return result;
}
