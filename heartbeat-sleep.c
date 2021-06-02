#include<mpi.h>
#include<stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define CONVERGENCE_COEF 100
#define NOT_RECEIVED -1

int num_neigh;
int *neigh;

void read_neighbours(int rank) {
    FILE *fp;
    char file_name[15];
    sprintf(file_name, "./files/%d.in", rank);

    fp = fopen(file_name, "r");
	fscanf(fp, "%d", &num_neigh);

	neigh = malloc(sizeof(int) * num_neigh);

	for (size_t i = 0; i < num_neigh; i++)
		fscanf(fp, "%d", &neigh[i]);
}

int leader_chosing(int rank, int nProcesses) {
	int leader = rank;
	int q = -1;
	MPI_Request send_request, recv_request;
	
	for (int k = 0; k < CONVERGENCE_COEF; k++) {
		for (int i = 0; i < num_neigh; i++) {
			MPI_Isend(&leader, 1, MPI_INT, neigh[i], 1, MPI_COMM_WORLD, &send_request);
			sleep(5);
			MPI_Irecv(&q, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &recv_request);

			// se asteapta un cvorum - macar de la un copil
			// daca am primit de la cel putin un copil sau ceva rang minimal

			// am primit de la un copil, stau un timp -> e ok
			// daca nu am primit de la niciunul, mai astept
			// daca nu si nu -> abort, ca nu e ceva ok

			sleep(5);

			printf("[%d]: leader = %d | recv_value = %d\n", rank, leader, q);

			if (q == -1) {
				sleep(5);
			}

			if (q == -1) {
				sleep(5);
			}

			if (q != -1) {
				printf("[%d]: leader = %d | recv_value = %d\n | success\n", rank, leader, q);
				if (q > leader) {
					leader = q;
				}
			} else {
				printf("[%d]: leader = %d | recv_value = %d\n | now aborting...\n", rank, leader, q);
				MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
			}
		}
	}

	printf("%i/%i: leader is %i\n", rank, nProcesses, leader);

	return leader;
}

int main(int argc, char *argv[]) {
	int rank, nProcesses, leader;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

	if (nProcesses != 12) {
		printf("please run with: mpirun --oversubscribe -np 12 %s\n", argv[0]);
		MPI_Finalize();	
		exit(0);
	}
	 
	read_neighbours(rank);
	leader = leader_chosing(rank, nProcesses);
	
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();
	return 0;
}