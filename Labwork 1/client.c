#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status
		= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	
    send(client_fd, "file.txt", strlen("file.txt"), 0);

    char file_content[1024] = {0};
    valread = read(client_fd, file_content, 1024 - 1);
    
    if (valread < 0) {
        perror("Error reading from socket");
        return -1;
    } else if (valread == 0) {
        printf("No data received from server\n");
    } else {
        file_content[valread] = '\0'; 
        printf("%s\n", file_content);
    }

	valread = read(client_fd, buffer, 1024 - 1);
	printf("%s\n", buffer);

	close(client_fd);
	return 0;
}
