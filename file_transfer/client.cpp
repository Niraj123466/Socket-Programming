#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fstream>

using namespace std;

#define BUFFER_SIZE 1024
#define PORT 5000

int main() {
    int socket_client;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};

    // Create the socket
    if((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Unable to create a socket at client");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if(connect(socket_client, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    // Open a file to send
    ifstream infile("niraj.txt", ios::binary);  // Specify your file here

    if (!infile.is_open()) {
        perror("Failed to open file to send");
        close(socket_client);
        return -1;
    }

    // Read from the file and send to the server
    while (!infile.eof()) {
        infile.read(buffer, BUFFER_SIZE);
        int bytesToSend = infile.gcount();  // Get the number of bytes read
        if (send(socket_client, buffer, bytesToSend, 0) < 0) {
            perror("Failed to send the file data");
            break;
        }
    }

    cout << "File sent successfully" << endl;

    infile.close();
    close(socket_client);

    return 0;
}
