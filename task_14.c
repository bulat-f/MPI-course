#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void printA(const int *a, const int N, const double *b, const int M);

int main (int argc, char* argv[])
{
    const int N = 10, M = 8;
    const int buf_size = N * sizeof(int) + M * sizeof(double);
	int errCode, currRank, pos = 0;
    MPI_Status status;

	if ((errCode = MPI_Init(&argc, &argv)) != 0)
	{
		return errCode;
	}

    int a[N];
    double b[M];
	char buf[buf_size];

	MPI_Comm_rank(MPI_COMM_WORLD, &currRank);

    if (currRank == 0)
    {
        srand(time(NULL));

		for (int i = 0; i < N; i++)
        {
            a[i] = rand() % 100;
        }
		for (int i = 0; i < M; i++)
        {
            b[i] = (double)rand() / 1000 * 100;
        }
		MPI_Pack(a, N, MPI_INT, buf, buf_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(b, M, MPI_DOUBLE, buf, buf_size, &pos, MPI_COMM_WORLD);

        printA(a, N, b, M);

    }
    MPI_Bcast(buf, buf_size, MPI_PACKED, 0, MPI_COMM_WORLD);
    if (currRank == 1)
    {
	    pos = 0;
		MPI_Unpack(buf, buf_size, &pos, a, N, MPI_INT, MPI_COMM_WORLD);
		MPI_Unpack(buf, buf_size, &pos, b, M, MPI_DOUBLE, MPI_COMM_WORLD);

        printA(a, N, b, M);
    }

	MPI_Finalize();
	return 0;
}

void printA(const int *a, const int N, const double *b, const int M)
{
    for (int i = 0; i < N; i++)
        printf("%d ", a[i]);
    printf("\n");

    for (int i = 0; i < M; i++)
        printf("%f ", b[i]);
    printf("\n");
}
