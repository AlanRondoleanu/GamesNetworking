#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link the Winsock Library

#define SERVER "127.0.0.1" // ip address of udp server
#define BUFLEN 512         // Max length of buffer
#define PORT 8888          //The port on which to listen for incoming data
int main(void)
{
    struct sockaddr_in si_other; // server's address
    int s, slen = sizeof(si_other); // socket descriptor and the size of the server address structure
    char buf[BUFLEN]; // Buffer to store the received data
    char message[BUFLEN]; // Buffer to store the message to be sent
    WSADATA wsa; // Holds information about the windows sockets implementation

    // Initialise Winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");

    // Create socket
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        printf("socket() failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Setup address structure
    memset((char*)&si_other, 0, sizeof(si_other)); // Clear the structure to avoid any garbage values
    si_other.sin_family = AF_INET; // IPv4
    si_other.sin_port = htons(PORT); // Convert port number to network byte order
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER); // Convert the IP address to network byte order

    // Start communication
    while (1)
    {
        printf("Enter message : ");
        gets(message); // Get the message from the user (unsafe, but used for simplicity)

        // Send the message to the server
        if (sendto(s, message, strlen(message), 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf, '\0', BUFLEN); // Clear the buffer to ensure no old data remains

        // try to receive some data, this is a blocking call        
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        // Print the server's response
        puts(buf);
    }

    // Close the socket and clean up Winsock
    closesocket(s);
    WSACleanup();

    return 0;
}