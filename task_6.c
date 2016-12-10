#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    const int N = 10, M = 320000;
    int errCode, currRank;
    char x[N];
    MPI_Status status;

    if ((errCode = MPI_Init(&argc, &argv)) != 0)
    {
        return errCode;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &currRank);

    srand(time(NULL));

    if (!currRank)
    {
        srand(time(NULL));

        for (int i = 0; i < N; i++)
            x[i] = rand() % 100;

        printf("Massive X:\n");
        for (int i = 0; i < N; i++)
            printf("%c ", x[i]);

        printf("\nSend/receive count is %d ", M);

        double time = MPI_Wtime();

        for (int i = 0; i < M; i++)
        {
            MPI_Send(x, N, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(x, N, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        }

        time = MPI_Wtime() - time;
        printf("\nPingPong time is %f seconds\n", time);

        time = MPI_Wtime();

        for (int i = 0; i < M; i++)
        {
            MPI_Send(x, N, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(x, N, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        }

        time = MPI_Wtime() - time;
        printf("PingPing time is %f seconds\n", time);
    }
    else if (currRank == 1)
    {
        for (int i = 0; i < M; i++)
        {
            MPI_Recv(x, N, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(x, N, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }

        for (int i = 0; i < M; i++)
        {
            MPI_Send(x, N, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(x, N, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        }
    }


    MPI_Finalize();
    return 0;
}