/*
 * client2.c
 * CPE-555 Real-Time and Embedded Systems
 * Hetansh Madhani
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PORT_NUM "5550"

int main()
{
    struct addrinfo hints;
    struct addrinfo *servinfo;
    int sockfd;
    char rcv_string[140];
    int send_integer;
    int bytes;
	int bytes1;
    int i;

    /* clear hints structure */
    memset(&hints, 0, sizeof(hints));

    /* don't care IPv4 or IPv6
     * TCP stream socket
     * fill in local IP address automatically
     */
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    /* getaddrinfo populates servinfo structure with server's address info */
    if (getaddrinfo("localhost", PORT_NUM, &hints, &servinfo) != 0)
    {
        printf("Error in getaddrinfo\n");
        return 1;
    }

    /* get a file descriptor for the new socket */
    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd == -1)
    {
        printf("Error creating socket\n");
        return 1;
    }

    /* connect the socket */
    if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        printf("Error connecting the socket\n");
        return 1;
    }
    
    /*prompt the user to enter the number*/
    printf("Please enter a number: ");
    scanf("%d",&i);
    /* send the message on the socket */
    bytes = send(sockfd, &i, sizeof(int), 0);
    

    if (bytes == -1)
    {
        printf("Error sending message\n");
    }
    printf("*[INFO]Sent the number to the server*\n");
    printf("Sent %d bytes to localhost port %s\n", bytes, PORT_NUM);

    /*Recieve the message from the server whether the number is ODD or EVEN*/
    bytes1 = recv(sockfd, rcv_string, 140, 0);
    if (bytes1 == -1)
    {
        puts("Error in recv()\n");
        //close(connected_sockfd);
        close(sockfd);
        return 1;
    }
    rcv_string[bytes] = '\0';
    printf("Received reply from server that the no was : %s\n", rcv_string);

    /* close the socket */
    close(sockfd);

    return 0;
}
