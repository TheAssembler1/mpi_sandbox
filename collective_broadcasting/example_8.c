#include <mpi.h>
#include <stdio.h>

void basic_cast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator);
void my_ring_cast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator);

int main(int arc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int num = 60;
  my_ring_cast(&num, 1, MPI_INT, 0, MPI_COMM_WORLD);
  num = 40;
  basic_cast(&num, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // Finalize the MPI environment
  MPI_Finalize();

  return 0;
}

void basic_cast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
  MPI_Barrier(communicator);

  int world_rank;
  MPI_Comm_rank(communicator, &world_rank);
  int world_size;
  MPI_Comm_size(communicator, &world_size);

  if(world_rank == root) {
    printf("starting basic_cast------------\n");
    printf("Rank %d initiating sending of number\n", world_rank);
  }

  if(world_rank == root) {
    for(int i = 1; i < world_size; i++) {
      MPI_Send(data, count, datatype, i, 0, communicator);
    }
  } else {
    MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
    int num = *((int*) data);
    printf("Rank %d received number %d\n", world_rank, num);
  }

  fflush(stdout);
  MPI_Barrier(communicator);
}

void my_ring_cast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
  MPI_Barrier(communicator);

  int world_rank;
  MPI_Comm_rank(communicator, &world_rank);
  int world_size;
  MPI_Comm_size(communicator, &world_size);

  if(world_rank == root) {
    printf("starting my_ring_cast------------\n");
    printf("Rank %d initiating sending of number\n", world_rank);
  }

  if(world_rank != root) {
    MPI_Recv(data, count, datatype, world_rank - 1, 0, communicator, MPI_STATUS_IGNORE);

    int num = *((int*) data);
    printf("Rank %d received number %d\n", world_rank, num);
  }
  if(world_rank != world_size - 1) {
    MPI_Send(data, count, datatype, world_rank + 1, 0, communicator);
  }

  fflush(stdout);
  MPI_Barrier(communicator);
}
