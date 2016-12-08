#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

void printVector(double *x, const int size);
double dotProduce(double *x, double *y, const int size);

int main (int argc, char* argv[])
{
    const int M = 1;
	int errCode, size, currRank, N;
    double x_map[M], y_map[M], *x, *y, d;

	if ((errCode = MPI_Init(&argc, &argv)) != 0)
	{
		return errCode;
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &currRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL));

    N = M * size;
    x = calloc(N, sizeof(double));
    y = calloc(N, sizeof(double));

    for (int i = 0; i < N; i++)
    {
        x[i] = (double)(rand() % 10 - 5);
        y[i] = (double)(rand() % 10 - 5);
    }

    MPI_Scatter(x, M, MPI_DOUBLE, x_map, M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(y, M, MPI_DOUBLE, y_map, M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double res = dotProduce(x_map, y_map, M);
    MPI_Reduce(&res, &d, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (currRank == 0)
    {
        printVector(x, N);
        printVector(y, N);

        printf("dot produce = %f\n", d);
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

double dotProduce(double *x, double *y, const int size)
{
    double result = 0;
    for (int i = 0; i < size; i++)
        result += x[i] * y[i];

    return result;
}
