#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    // Creating the Client Socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    //Defining Server Address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //Connecting to the Server
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    //Sending Data to the Server
    const char* message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);

    //Closing the Client Socket
    close(clientSocket);
}