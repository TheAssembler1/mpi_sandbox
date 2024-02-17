#include <mpi.h>
#include <stdio.h>

#define PING_PONG_LIMIT 10

int main(int arc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int ping_pong_count = 0;
  int partner_rank = (world_rank + 1) % 2;

  if (world_rank > 1) {
    goto done;
  }

  while(ping_pong_count < PING_PONG_LIMIT) {
    if(world_rank == ping_pong_count % 2) {
      ping_pong_count++;
      MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
      printf("Rank %d sent pong message #%d\n", world_rank, ping_pong_count);
    } else {
      MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Rank %d received pong message #%d\n", world_rank, ping_pong_count);
    }
  }

  // Finalize the MPI environment
  done:
    MPI_Finalize();
    return 0;
}
