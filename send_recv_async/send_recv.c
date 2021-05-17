#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
 
int main (int argc, char *argv[])
{
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // The current process ID / Rank.
    MPI_Get_processor_name(hostname, &len);

    MPI_Request req;
 
    srand(42);
    int random_num = rand();
    printf("Before send: process with rank %d has the number %d.\n", rank,
            random_num);
 
    if (rank == 0) {
        MPI_Isend(&random_num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);
    } else {
        MPI_Status status;
        int flag;
        MPI_Irecv(&random_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, &status);
        
        printf("Process with rank %d, received %d.\n",
                rank, random_num);
    }
 
    printf("After send: process with rank %d has the number %d.\n", rank,
            random_num);
 
    MPI_Finalize();
 
}