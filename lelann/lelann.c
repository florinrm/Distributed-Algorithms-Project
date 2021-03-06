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

    int recv_val = -1, value = rank;
    int prec = rank == 0 ? procs - 1 : rank - 1;
    MPI_Request send_request, recv_request;

    // de sters Wait dupa Isend

    MPI_Isend(&value, 1, MPI_INT, (rank + 1) % procs, 0, MPI_COMM_WORLD, &send_request);

    MPI_Irecv(&recv_val, 1, MPI_INT, prec, 0, MPI_COMM_WORLD, &recv_request);
    MPI_Wait(&recv_request, NULL);

    if (recv_val == -1) {
        MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
    }

    list[i++] = recv_val;
    value = recv_val;

    while (rank != recv_val) {
        MPI_Isend(&value, 1, MPI_INT, (rank + 1) % procs, 0, MPI_COMM_WORLD, &send_request);

        MPI_Irecv(&recv_val, 1, MPI_INT, prec, 0, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, NULL);

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