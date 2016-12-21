#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int* receiveMessage(int *size, int tag, int source);
int* prepare(int *B, const int size, const int N, const int currRank);
int* join(int *a, const int a_size, int *b, const int b_size);
void printMessage(int *msg, const int size);
void printMatrix(int A[][12], const int size);

int main (int argc, char* argv[])
{
    const int N = 12;
    int errCode, size, currRank;
    int A[N][N], *x = NULL;

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
            for (int j = 0; j < N; j++)
                A[i][j] = rand() % 100;

        int block_len = N * N / (size - 1);
        int block = N / (size - 1), begin;

        printMatrix(A, N);
        printf("\n");

        for (int i = 1; i < size; i++)
        {
            begin = (i - 1) * block;
            MPI_Send(A[begin], block_len, MPI_INT, i, 88, MPI_COMM_WORLD);
        }


        for (int i = 1; i < size; i++)
        {
            begin = (i - 1) * block;

            x = join(x, begin, receiveMessage(&block, 77, i), block);
        }

        printMessage(x, N);
    }
    else
    {
        int msgSize, *B, *z;

        B = receiveMessage(&msgSize, 88, 0);

        z = prepare(B, msgSize, N, currRank);

        MPI_Send(z, msgSize / N, MPI_INT, 0, 77, MPI_COMM_WORLD);
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

int* prepare(int *B, const int size, const int N, const int currRank)
{
    int M = size / N;
    int *result = (int *) calloc(M, sizeof(int));
    for (int i = 0; i < M; i++)
        result[i] = B[(currRank - 1) * M + (N + 1) * i];
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

void printMatrix(int A[][12], const int size)
{
    for (int i = 0; i < size; i++)
        printMessage(A[i], size);
}
