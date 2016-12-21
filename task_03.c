#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int* receiveMessage(int *size);
void printMessage(int *msg, const int size);

int main (int argc, char* argv[])
{
    const int N = 10;
    int errCode, size, currRank;
    MPI_Status status;
    int array[N];

    if ((errCode = MPI_Init(&argc, &argv)) != 0)
    {
        return errCode;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &currRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL));

    if (currRank == 0)
    {
        for (int i = 0; i < N; i++)
            array[i] = rand() % 100;

        for (int i = 1; i < size; i++)
        {
            MPI_Send(array, N, MPI_INT, i, 99, MPI_COMM_WORLD);
        }
    }
    else
    {
        int msgSize, *msg;

        msg = receiveMessage(&msgSize);
        printMessage(msg, msgSize);
    }

    MPI_Finalize();
    return 0;
}

int* receiveMessage(int *size)
{
    MPI_Status status;
    int *msg;

    MPI_Probe(0, 99, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, size);

    msg = (int *) calloc(*size, sizeof(int));

    MPI_Recv(msg, *size, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);

    return msg;
}

void printMessage(int *msg, const int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", msg[i]);

    printf("\n");
}
