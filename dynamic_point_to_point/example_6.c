#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_NUMBERS 100

int main(int arc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if(world_rank > 1) {
    goto done;
  }

  int number_amount;
  if(world_rank == 0) {
    int numbers[MAX_NUMBERS];

    srand(time(NULL));
    number_amount = (rand() / (float)RAND_MAX * MAX_NUMBERS);
    MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 sent %d numbers to 1\n", number_amount);
  } else {
    MPI_Status status;

    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &number_amount);

    int* numbers = (int*)malloc(sizeof(int) * number_amount);

    MPI_Recv(numbers, number_amount, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Rank %d dynamically received %d | Source rank = %d | tag = %d\n", 
            world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);
  }

  done:
    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
