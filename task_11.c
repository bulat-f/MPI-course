#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void printMatrix(int A[][8], const int N, const int M);

int main (int argc, char* argv[])
{
    const int N = 8;
    const int M = N / 2;
	int errCode, size, currRank;
    MPI_Status status;
    int a[N][N], b[M][N], c[M][N];

	if ((errCode = MPI_Init(&argc, &argv)) != 0)
	{
		return errCode;
	}

    MPI_Datatype send_type, recv_type;

    MPI_Type_vector(M, N, 2*N, MPI_INT, &send_type);
    MPI_Type_vector(M, N, N, MPI_INT, &recv_type);
    MPI_Type_commit(&send_type);
    MPI_Type_commit(&recv_type);

    MPI_Comm_rank(MPI_COMM_WORLD, &currRank);

    if (currRank == 0)
    {
        srand(time(NULL));

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                a[i][j] = (rand() % 50 - 20);
        }

        printMatrix(a, N, N);
        MPI_Send(*a, 1, send_type, 1, 88, MPI_COMM_WORLD);
        MPI_Send(a[1], 1, send_type, 2, 99, MPI_COMM_WORLD);
    }
    else if (currRank == 1)
    {
        MPI_Recv(*b, 1, recv_type, 0, 88, MPI_COMM_WORLD, &status);

        printf("\n======== %d ========\n", currRank);
        printMatrix(b, M, N);
        printf("===================\n");
    }
    else if (currRank == 2)
    {
        MPI_Recv(*c, 1, recv_type, 0, 99, MPI_COMM_WORLD, &status);

        printf("\n======== %d ========\n", currRank);
        printMatrix(c, M, N);
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
