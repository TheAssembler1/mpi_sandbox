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

  int token;
  if(world_rank != 0) {
    MPI_Recv(&token, 1, MPI_INT, world_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Rank %d recieved token %d\n", world_rank, token);
    token++;
  } else {
    token =  0;
  }

  MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);

  if(world_rank == 0) {
    MPI_Recv(&token, 1, MPI_INT, world_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Rank %d recieved token %d\n", world_rank, token);
  }

  // Finalize the MPI environment
  done:
    MPI_Finalize();
    return 0;
}
