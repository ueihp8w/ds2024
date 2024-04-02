#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  MPI_Init(&argc, &argv);

  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  int server_rank = 0; 

  char filename[1024] = "file.txt";

  MPI_Send(filename, strlen(filename)+1, MPI_CHAR, server_rank, 0, MPI_COMM_WORLD);

  MPI_Finalize();

  return 0;
}
