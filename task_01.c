#include <stdio.h>
#include <mpi.h>

int main (int argc, char* argv[])
{
	int errCode;
    int currRank, size;
	if ((errCode = MPI_Init(&argc, &argv)) != 0)
	{
		return errCode;
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &currRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Rank = %d; Size = %d\n", currRank, size);

	MPI_Finalize();
	return 0;
}

