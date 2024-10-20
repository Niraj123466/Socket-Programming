#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
using namespace std;

#define BUFFER_SIZE 1024
#define PORT 5000

int main() {
    // declare the required variables
    int socket_client;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};

    // create a socket at client
    if((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Unable to create a socket at client");
        exit(EXIT_FAILURE);
    }

    // add info to struct address
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    // convert ipv4 and ipv6 address from text to binary
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        perror("Unable to convert IP address to binary");
        exit(EXIT_FAILURE);
    }

    // connect to server
    if(connect(socket_client, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Unable to connect to server");
        exit(EXIT_FAILURE);
    }

    // Real-time chat loop
    while(true) {
        // send message to the server
        cout << "Client: ";
        string clientMessage;
        getline(cin, clientMessage);

        if(send(socket_client, clientMessage.c_str(), clientMessage.size(), 0) <= 0) {
            perror("Unable to send message to server");
            break;
        }

        if(clientMessage == "exit") {
            cout << "Closing connection..." << endl;
            break;
        }

        // clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // receive message from the server
        int valread = read(socket_client, buffer, BUFFER_SIZE);
        if(valread <= 0 || strcmp(buffer, "exit") == 0) {
            cout << "Server disconnected" << endl;
            break;
        }

        cout << "Server: " << buffer << endl;
    }

    close(socket_client);

    return 0;
}
