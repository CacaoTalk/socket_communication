#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 6667

int main(void) {
    int socketNum = -1;
    int newSocket = -1;
    int valread = 0;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

    // Creating socket file descriptor
    if ((socketNum = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "socket Number: " << socketNum << std::endl;
 
    // Forcefully attaching socket to the port 6667
    if (setsockopt(socketNum, SOL_SOCKET,
                   SO_REUSEADDR, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Forcefully attaching socket to the port 6667
    if (bind(socketNum, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // listen
    if (listen(socketNum, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // accept
    if ((newSocket
         = accept(socketNum, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("after accept");
    valread = read(newSocket, buffer, 1024);
    printf("%d %s\n", valread, buffer);
    send(newSocket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
 
    // closing the connected socket
    close(newSocket);
    // closing the listening socket
    shutdown(socketNum, SHUT_RDWR);
    return 0;
}
