#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

// Simulate handling an RPC function call (replace with actual RPC server logic)
int handle_transfer_file(int sockfd, char* filename) {
    char path[] = "/home/p8w/ds2024/Labwork 2/";
    char fullpath[1000];
    strcpy(fullpath, path);
    strcat(fullpath, filename);

    FILE* fp = fopen(fullpath, "r");
    if (fp == NULL) {
        perror("File open error");
        send(sockfd, "ERROR: File not found", strlen("ERROR: File not found") + 1, 0);
        return -1;
    }

    char data[1024] = {0};
    while (fgets(data, 1024, fp) != NULL) {
        send(sockfd, data, strlen(data), 0);
    }
    fclose(fp);
    return 0;
}

int main(int argc, char const* argv[]) {
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char filename[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read(new_socket, filename, 1024);
    if (valread < 0) {
        perror("Error reading filename");
        return -1;
    }

    if (handle_transfer_file(new_socket, filename) < 0) {
        printf("File transfer failed\n");
    } else {
        printf("File transfer successful\n");
    }

    close(new_socket);
    close(server_fd);
    return 0;
}  
