/*
 * server2.c
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
    int connected_sockfd;
    struct addrinfo clientinfo;
    socklen_t clientinfo_size;
    char send_string[140];
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

    /* getaddrinfo populates servinfo structure with local address info */
    if (getaddrinfo(NULL, PORT_NUM, &hints, &servinfo) != 0)
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

    /* bind the socket to the port we're interested in */
    if (bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        printf("Error binding to port\n");
        close(sockfd);
        return 1;
    }
    //}

    /* listen for connections on the socket */
    if (listen(sockfd, 10) == -1)
    {
        printf("Error in listen()\n");
        close(sockfd);
        return 1;
    }
    printf("Server is waiting for connections on port %s...\n", PORT_NUM);
    
    /* at this point we have received a connection
     * so, we accept it
     */
    clientinfo_size = sizeof(clientinfo);
    connected_sockfd = accept(sockfd, (struct sockaddr*)&clientinfo, &clientinfo_size);
    if (connected_sockfd == -1)
    {
        printf("Error accepting connection\n");
        return 1;
    }
    printf("Received connection from client\n");

    /* receive a message from the socket */
   while( bytes = recv(connected_sockfd, &i, sizeof(int), 0)){
    if (bytes == -1)
    {
        printf("Error in recv()\n");
        close(connected_sockfd);
        close(sockfd);
        return 1;
    }

    printf("The number received from the client is: %d \n", i);
    if(i%2 == 0)						//Compute is the number is odd or even(The remainder is 0 or no)
	{
 	printf("The number recieved is Even\n");		
 	char send_string[140] = "EVEN";	
	bytes1 = send(connected_sockfd, send_string, sizeof(send_string), 0); //send the message to the client
    		if (bytes == -1)
    			{
        		 printf("Error sending message\n");
    			}
		printf("*[INFO] Sending the message \"EVEN\" to the client*\n");
    		printf("Sent %d bytes to localhost port %s\n", bytes, PORT_NUM);
	}
    else{
	printf("The number recieved is Odd\n");
	char send_string[140] = "ODD";
	bytes1 = send(connected_sockfd, send_string, sizeof(send_string), 0);
    		if (bytes == -1)
    			{
        		printf("Error sending message\n");
    			}
		printf("*[INFO] Sending the message \"ODD\" to the client*\n");
    		printf("Sent %d bytes to localhost port %s\n", bytes, PORT_NUM);
	}
}

    /* close the sockets */
    close(connected_sockfd);
    close(sockfd);

    return 0;
}
