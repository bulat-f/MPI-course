#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void printMatrix(int A[][8], const int N, const int M);

int main (int argc, char* argv[])
{
    const int N = 8;
    const int M = 2;
	int errCode, size, currRank;
    MPI_Status status;
    int a[N][N], d[M][N];

	if ((errCode = MPI_Init(&argc, &argv)) != 0)
	{
		return errCode;
	}

    MPI_Datatype send_type, recv_type;
    int lengths[2] = { N, N };
	long int send_indexes[2] = { 0, 4 * N * sizeof(int) };
	long int recv_indexes[2] = { 0, N * sizeof(int) };
	MPI_Datatype types[2] = { MPI_INT, MPI_INT };

    MPI_Type_create_struct(2, lengths, send_indexes, types, &send_type);
	MPI_Type_commit(&send_type);

	MPI_Type_create_struct(2, lengths, recv_indexes, types, &recv_type);
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

        MPI_Comm_size(MPI_COMM_WORLD, &size);
        int pieces = size < 5 ? size - 1 : 4;
        for (int i = 0; i < pieces; i++)
	    {
            MPI_Send(a[i], 1, send_type, i + 1, 88, MPI_COMM_WORLD);
	    }
    }
    else
    {
        if (currRank <= 4)
        {
            MPI_Recv(*d, 1, recv_type, 0, 88, MPI_COMM_WORLD, &status);

            printf("\n======== %d ========\n", currRank);
            printMatrix(d, M, N);
            printf("===================\n");
        }
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
