#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

#define CANDIDATE 0
#define LOST 1
#define LEADER 2

#define REGULAR_MESSAGE_ID 0

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

    MPI_Request send_request, recv_request;

    int state = CANDIDATE;

    MPI_Isend(&rank, 1, MPI_INT, (rank + 1) % procs, 0, MPI_COMM_WORLD, &send_request);

    int stop_message_sent = 0;
    
    while (state != LEADER) {
        int prec = rank == 0 ? procs - 1 : rank - 1;
        int recv_val = -1;
        MPI_Irecv(&recv_val, 1, MPI_INT, prec, 0, MPI_COMM_WORLD, &recv_request);
        MPI_Wait(&recv_request, NULL);

        if (recv_val == rank) {
            state == LEADER;
            printf("Process [%d] win!\n", rank);
            MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
        } else if (recv_val > rank) {
            if (state == CANDIDATE) {
                state = LOST;
                printf("Process [%d] lost!\n", rank);
                MPI_Isend(&recv_val, 1, MPI_INT, (rank + 1) % procs, 0, MPI_COMM_WORLD, &send_request);
            }
        }
    }

    MPI_Finalize();
    return 0;
}