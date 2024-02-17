#include <mpi.h>
#include <stdio.h>

int main(int arc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int number;
  if(world_rank == 0) {
    number = 69;
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("Sent number %d\n", number);
  }
  if(world_rank == 1) {
    MPI_Recv(&number, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Received number %d\n", number);
  }

  // Finalize the MPI environment
  MPI_Finalize();

  return 0;
}
