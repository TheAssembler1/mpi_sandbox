#include <mpi.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>

#define DOMAIN_SIZE 100
#define NUM_WALKERS_PER_PROC 2
#define MAX_WALK_SIZE 2

using namespace std;

typedef struct Walker_Struct Walker;

void decompose_domain(int domain_size, int world_rank, int world_size, int* subdomain_start, int* subdomain_size);
void initialize_walkers(int num_walkers_per_proc, int max_walk_size, int subdomain_start, int subdomain_size,
                        vector<Walker>* incoming_walkers);
void walk(int world_rank, Walker* walker, int subdomain_start, int subdomain_size, int domain_size, vector<Walker>* outgoing_walkers);
void send_outgoing_walkers(vector<Walker>* outgoing_walkers, int world_rank, int world_size);
void receive_incoming_walkers(vector<Walker>* incoming_walkers, int world_rank, int world_size);

int main(int arc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int subdomain_start;
  int subdomain_size;
  vector<Walker> incoming_walkers;
  vector<Walker> outgoing_walkers;

  decompose_domain(DOMAIN_SIZE, world_rank, world_size, &subdomain_start, &subdomain_size);
  initialize_walkers(NUM_WALKERS_PER_PROC, MAX_WALK_SIZE, subdomain_start, subdomain_size, &incoming_walkers);

  int maximum_sends_recv = MAX_WALK_SIZE / (DOMAIN_SIZE / world_size) + 1;
  for(int m = 0; m < maximum_sends_recv; m++) {
    for(int i = 0; i < incoming_walkers.size(); i++) {
      walk(world_rank, &incoming_walkers[i], subdomain_start, subdomain_size, DOMAIN_SIZE, &outgoing_walkers);
    }

    if(world_rank % 2 == 0) {
      send_outgoing_walkers(&outgoing_walkers, world_rank, world_size);
      receive_incoming_walkers(&incoming_walkers, world_rank, world_size);
    } else {
      receive_incoming_walkers(&incoming_walkers, world_rank, world_size);
      send_outgoing_walkers(&outgoing_walkers, world_rank, world_size);
    }
  }

  // Finalize the MPI environment
  MPI_Finalize();

  return 0;
}

struct Walker_Struct{
  int location;
  int num_steps_left_in_walk;
};

void decompose_domain(int domain_size, int world_rank, int world_size, int* subdomain_start, int* subdomain_size) {
  if (world_size > domain_size) {
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  *subdomain_start = domain_size / world_size * world_rank;
  *subdomain_size = domain_size / world_size;

  if(world_rank == world_size - 1) {
    *subdomain_size += domain_size % world_size;
  }
}

void initialize_walkers(int num_walkers_per_proc, int max_walk_size, int subdomain_start, int subdomain_size,
                        vector<Walker>* incoming_walkers) {
  Walker walker;

  for(int i = 0; i < num_walkers_per_proc; i++) {
    walker.location = subdomain_start;
    walker.num_steps_left_in_walk = (rand() / (float)RAND_MAX) * max_walk_size;
    incoming_walkers->push_back(walker);
  }
}

void walk(int world_rank, Walker* walker, int subdomain_start, int subdomain_size, int domain_size, vector<Walker>* outgoing_walkers) {
  while(walker->num_steps_left_in_walk > 0) {
    if(walker->location == subdomain_start + subdomain_size) {
      walker->location = 0;
      outgoing_walkers->push_back(*walker);
      break;
    } else {
      printf("Rank %d advancing walker | %d steps remaining\n", world_rank, walker->num_steps_left_in_walk);
      walker->num_steps_left_in_walk--;
      walker->location++;
    }
  }
}

void send_outgoing_walkers(vector<Walker>* outgoing_walkers, int world_rank, int world_size) {
  MPI_Send((void*)outgoing_walkers->data(), outgoing_walkers->size() * sizeof(Walker), MPI_BYTE,
           (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);
  outgoing_walkers->clear();
}

void receive_incoming_walkers(vector<Walker>* incoming_walkers, int world_rank, int world_size) {
  MPI_Status status;
  int incoming_rank = (world_rank == 0) ? world_size - 1 : world_rank - 1;
  MPI_Probe(incoming_rank, 0, MPI_COMM_WORLD, &status);

  int incoming_walkers_size;
  MPI_Get_count(&status, MPI_BYTE, &incoming_walkers_size);

  incoming_walkers->resize(incoming_walkers_size / sizeof(Walker));

  MPI_Recv((void*)incoming_walkers->data(), incoming_walkers_size, MPI_BYTE, incoming_rank, 0, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
}