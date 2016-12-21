#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int* receiveMessage(int *size, int tag, int source);
int* prepare(int *x, int *y, const int size);
int* join(int *a, const int a_size, int *b, const int b_size);
void printMessage(int *msg, const int size);

int main (int argc, char* argv[])
{
    const int N = 12;
    int errCode, size, currRank;
    int x[N], y[N], *z = NULL;

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
        {
            x[i] = rand() % 100;
            y[i] = rand() % 100;
        }

        int block_size = N / (size - 1);
        int begin;

        printMessage(x, N);
        printMessage(y, N);

        for (int i = 1; i < size; i++)
        {
            begin = (i - 1) * block_size;
            MPI_Send(&x[begin], block_size, MPI_INT, i, 88, MPI_COMM_WORLD);
            MPI_Send(&y[begin], block_size, MPI_INT, i, 99, MPI_COMM_WORLD);
        }


        for (int i = 1; i < size; i++)
        {
            begin = (i - 1) * block_size;

            z = join(z, begin, receiveMessage(&block_size, 77, i), block_size);
        }

        printMessage(z, N);
    }
    else
    {
        int msgSize, *x, *y;

        x = receiveMessage(&msgSize, 88, 0);
        y = receiveMessage(&msgSize, 99, 0);

        z = prepare(x, y, msgSize);

        MPI_Send(z, msgSize, MPI_INT, 0, 77, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

int* receiveMessage(int *size, int tag, int source)
{
    MPI_Status status;
    int *msg;

    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, size);

    msg = (int *) calloc(*size, sizeof(int));

    MPI_Recv(msg, *size, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

    return msg;
}

int* prepare(int *x, int *y, const int size)
{
    int *result = (int *) calloc(size, sizeof(int));
    for (int i = 0; i < size; i++)
        result[i] = x[i] + y[i];
    return result;
}

int* join(int *a, const int a_size, int *b, const int b_size)
{
    int *result = (int *) realloc(a, (a_size + b_size) * sizeof(int));
    for (int i = a_size; i < a_size + b_size; i++)
        result[i] = b[i - a_size];

    return result;
}

void printMessage(int *msg, const int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", msg[i]);

    printf("\n");
}
