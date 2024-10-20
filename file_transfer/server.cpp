#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <fstream>

using namespace std;

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int socket_fd, new_socket;
    struct sockaddr_in address;
    int addresslen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create the socket
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Unable to create socket at server");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if(bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed at server");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if(listen(socket_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server is listening on port: " << PORT << endl;

    // Accept a client connection
    if((new_socket = accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&addresslen)) < 0) {
        perror("Unable to accept the request");
        exit(EXIT_FAILURE);
    }

    // Open a file to save the received data
    ofstream outfile("received_file.txt", ios::binary);  // Save as a binary file

    if (!outfile.is_open()) {
        perror("Failed to open the file to save data");
        close(new_socket);
        close(socket_fd);
        return -1;
    }

    // Receive the file in chunks
    int bytesReceived;
    while((bytesReceived = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        outfile.write(buffer, bytesReceived);
    }

    cout << "File received and saved as 'received_file.txt'" << endl;

    outfile.close();
    close(new_socket);
    close(socket_fd);

    return 0;
}
