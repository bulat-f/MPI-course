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
struct pair min(double *x, const int size, const int shift);

int main (int argc, char* argv[])
{
    const int M = 2;
    int errCode, size, currRank, N;
    double x_map[M], *x;
    struct pair m;

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
    {
        x[i] = (double)(rand() % 100 - 40);
    }

    MPI_Scatter(x, M, MPI_DOUBLE, x_map, M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    struct pair res = min(x_map, M, M * currRank);
    MPI_Reduce(&res, &m, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if (currRank == 0)
    {
        printVector(x, N);

        printf("min.value == %f;\nmin.index == %d;\n", m.value, m.index);
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

struct pair min(double *x, const int size, const int shift)
{
    struct pair m;
    m.value = x[0];
    m.index = 0;
    for (int i = 1; i < size; i++)
    {
        if (x[i] < m.value)
        {
            m.value = x[i];
            m.index = i;
        }
    }
    m.index += shift;
    return m;
}
