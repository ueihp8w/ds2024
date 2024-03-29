#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <asm-generic/socket.h>
#define PORT 8080
int main(int argc, char const* argv[])
{
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char filename[1024] = {0};
    char* hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, filename, 1024 - 1); 
    printf("Recieved the filename: %s\n", filename);

    char path[] = "/home/p8w/ds2024/Labwork 1/";
    char fullpath[1000];
    strcpy(fullpath, path);
    strcat(fullpath, filename);
    FILE* fp = fopen(fullpath, "r");
    if (fp == NULL) {
        perror("File open error");
        exit(EXIT_FAILURE);
    }
    char data[1024] = {0};
    while (fgets(data, 1024, fp) != NULL) {
        send(new_socket, data, strlen(data), 0);
    }
    printf("File sent\n");
    fclose(fp);
    
    close(new_socket);
    close(server_fd);
    return 0;
}