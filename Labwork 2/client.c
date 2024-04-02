#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 8080
#define SERVER_ADDRESS "127.0.0.1"

// Simulate an RPC function call (replace with actual RPC library calls)
int transfer_file_rpc(int sockfd, char* filename) {
  	int bytes_sent = send(sockfd, filename, strlen(filename) + 1, 0);
  	if (bytes_sent < 0) {
    	perror("Error sending filename");
    	return -1;
  	}
  // Simulate receiving a success/failure response
  	char response[100];
  	int bytes_received = recv(sockfd, response, sizeof(response), 0);
	if (bytes_received < 0) {
		perror("Error receiving response");
		return -1;
	}
	printf("File content: %s\n", response);
	return 0;
}

int main(int argc, char const* argv[]) {
	int sockfd;
	struct sockaddr_in serv_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_ADDRESS, &serv_addr.sin_addr);

	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connection failed");
		exit(EXIT_FAILURE);
	}

	if (transfer_file_rpc(sockfd, "file.txt") < 0) {
		printf("File transfer failed\n");
	} else {
		printf("File transfer successful\n");
	}

	close(sockfd);
	return 0;
}
