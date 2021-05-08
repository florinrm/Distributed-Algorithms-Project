#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int max(int *list, int size) {
    int res = 0;
    for (int i = 0; i < size; i++) {
        if (res <= list[i]) {
            res = list[i];
        }
    }

    return res;
}

int main (int argc, char *argv[]) {
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int list[procs];
    list[0] = rank;
    for (int k = 1; k < procs; k++) {
        list[k] = 0;
    }

    int i = 1;

    int recv_val, value = rank;
    int prec = rank == 0 ? procs - 1 : rank - 1;

    MPI_Send(&value, 1, MPI_INT, (rank + 1) % procs, 0, MPI_COMM_WORLD);

    MPI_Recv(&recv_val, 1, MPI_INT, prec, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    list[i++] = recv_val;
    value = recv_val;

    while (rank != recv_val) {
        MPI_Send(&value, 1, MPI_INT, (rank + 1) % procs, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_val, 1, MPI_INT, prec, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value = recv_val;
        list[i++] = recv_val;
    }

    if (value == max(list, procs)) {
        printf("Process [%d] won!\n", rank);
    } else {
        printf("Process [%d] lost!\n", rank);
    }

    MPI_Finalize();
    return 0;
}