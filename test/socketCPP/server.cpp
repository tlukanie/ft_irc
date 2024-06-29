#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main()
{
    /*
        socketfd: It is the file descriptor for the socket.
        AF_INET: It specifies the IPv4 protocol family.
        SOCK_STREAM: It defines that the TCP type socket.
    */

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //returns sort of file descriptor for the socket

    //defining server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //Binding the Server Socket (makes the socket public)
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); // assigning socket file descriptor with the ip address

    //Listening for Connections
    listen(serverSocket, 5);

    //Accepting a Client Connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);

    //Receiving Data from the Client
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;

    //Closing the Server Socket
    close(serverSocket);
}