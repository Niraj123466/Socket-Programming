#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
using namespace std;

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    // declare all the variables
    int socket_fd, new_socket; // socket descriptor
    struct sockaddr_in address; // address of the server
    int addresslen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // create the socket at server
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Unable to create socket at server");
        exit(EXIT_FAILURE);
    }

    // provide all the information to socket struct
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    // BIND SERVER INFO TO SOCKET
    if(bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed at server");
        exit(EXIT_FAILURE);
    }

    // Listen to all the requests from client
    if(listen(socket_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server is listening on port: " << PORT << endl;

    // accept the request from client
    if((new_socket = accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&addresslen)) < 0) {
        perror("Unable to accept the request");
        exit(EXIT_FAILURE);
    }

    // Real-time chat loop
    while(true) {
        // clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // read the message from client
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if(valread <= 0 || strcmp(buffer, "exit") == 0) {
            cout << "Client disconnected" << endl;
            break;
        }
        cout << "Client: " << buffer << endl;

        // send a message to the client
        cout << "Server: ";
        string serverMessage;
        getline(cin, serverMessage);

        if(send(new_socket, serverMessage.c_str(), serverMessage.size(), 0) <= 0) {
            perror("Unable to send message to client");
            break;
        }

        if(serverMessage == "exit") {
            cout << "Closing connection..." << endl;
            break;
        }
    }

    close(new_socket);
    close(socket_fd);

    return 0;
}
