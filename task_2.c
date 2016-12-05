#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main (int argc, char* argv[])
{
    const int N = 10;
	int errCode, currRank;
    MPI_Status status;
    int array[N];

	if ((errCode = MPI_Init(&argc, &argv)) != 0)
	{
		return errCode;
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &currRank);

    srand(time(NULL));

    if (currRank == 0)
    {
        for (int i = 0; i < N; i++)
            array[i] = rand() % 100;

        MPI_Send(array, N, MPI_INT, 1, 99, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(array, N, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);

        for (int i = 0; i < N; i++)
            printf("%d ", array[i]);

        printf("\n");
    }

	MPI_Finalize();
	return 0;
}

