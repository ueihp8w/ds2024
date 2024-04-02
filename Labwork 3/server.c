#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <asm-generic/socket.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
  MPI_Init(&argc, &argv);

  int num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0) {
    char filename[1024];
    int worker_rank = 1;
    while (1) {
      MPI_Recv(filename, 1024, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      
      MPI_Send(filename, strlen(filename)+1, MPI_CHAR, worker_rank, 0, MPI_COMM_WORLD);
      
      worker_rank = (worker_rank + 1) % (num_processes - 1);
    }
  } else {
    char filename[1024];
    char path[] = "/home/p8w/ds2024/Labwork 3/";
    char fullpath[1000];

    while (1) {
      MPI_Recv(filename, 1024, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }

  MPI_Finalize();

  return 0;
}
