#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib") // Winsock Library

#define BUFLEN 512  // Max length of the buffer
#define PORT 8888   // The port on which to listen for incoming data

int main()
{
    SOCKET s; // socket descriptor
    struct sockaddr_in server, si_other; // holds address information for the server and client
    int slen, recv_len; // size of the client's address structure and length of received data
    char buf[BUFLEN]; // buffer to hold incoming data
    WSADATA wsa; // hold informations about the windows sockets implementation

    slen = sizeof(si_other);

    // Initialise Winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) // Winsock version
    {
        printf("Failed. Error Code : %d", WSAGetLastError()); // error message
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");

    // Creates a socket
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError()); // error message
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure for the server
    server.sin_family = AF_INET; // Uses IPv4
    server.sin_addr.s_addr = INADDR_ANY; // Not sure
    server.sin_port = htons(PORT); // Converts the port number to network byte order

    // Binds the socket to the address and port
    if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError()); // error message
        exit(EXIT_FAILURE);
    }
    puts("Bind done");

    // Keep listening for data
    while (1)
    {
        printf("Waiting for data...");
        fflush(stdout); // Not sure

        // clear the buffer by filling null, it might have previously received data
        memset(buf, '\0', BUFLEN);

        // try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        char client_ip[INET_ADDRSTRLEN]; // Buffer to hold the client IP address as a string

        // convert the IP address to a string using inet_ntop
        inet_ntop(AF_INET, &(si_other.sin_addr), client_ip, INET_ADDRSTRLEN);

        // print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", client_ip, ntohs(si_other.sin_port));

        printf("Data: %s\n", buf);

        // now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
    }

    // Close the socket and clean up Winsock
    closesocket(s);
    WSACleanup();

    return 0;
}