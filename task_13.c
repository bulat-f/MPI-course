#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void printMatrix(int A[][8], const int N, const int M);

int main (int argc, char* argv[])
{
    const int N = 8;
	int errCode, size, currRank;
    MPI_Status status;
    int a[N][N], d[N][N];

	if ((errCode = MPI_Init(&argc, &argv)) != 0)
	{
		return errCode;
	}

    MPI_Datatype type;
    int lengths[N], indexes[N];
	for (int i = 0; i < N; i++)
	{
		lengths[i] = N - i;
		indexes[i] = N * i + i;
	}
	MPI_Type_indexed(N, lengths, indexes, MPI_INT, &type);
	MPI_Type_commit(&type);

    MPI_Comm_rank(MPI_COMM_WORLD, &currRank);

    if (currRank == 0)
    {
        srand(time(NULL));

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                a[i][j] = j < i ? 0 : rand() % 50 - 20;
        }

        printMatrix(a, N, N);

        MPI_Send(*a, 1, type, 1, 88, MPI_COMM_WORLD);
    }
    else if (currRank == 1)
    {
        MPI_Recv(*d, 1, type, 0, 88, MPI_COMM_WORLD, &status);

        printf("\n======== %d ========\n", currRank);
        printMatrix(d, N, N);
        printf("===================\n");
    }

	MPI_Finalize();
	return 0;
}

void printMatrix(int A[][8], const int N, const int M)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            printf("%d ", A[i][j]);
        printf("\n");
    }
}
