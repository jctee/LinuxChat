/*---------------------------------------------------------------------------------------
--	SOURCE FILE:		svr_chat.c -   A simple multiplexed echo server using TCP
--
--	PROGRAM:   svr.exe
--
--	FUNCTIONS: Berkeley Socket API
--
--	DATE:      March 28, 2018
--
--	REVISIONS:		(Date and Description)
--				March 28, 2018
--				Initialize project
--
--	DESIGNERS:		Based on Richard Stevens Example, p165-166
--				Modified & redesigned: Aman Abdulla: February 20, 2008
--        Modified & redesigned: LYT & JCT: March 28, 2018
--
--	PROGRAMMER:  Li-Yan Tong
--
--	NOTES:
--	The program will accept TCP connections from multiple client machines.
-- 	The program will read data from each client socket and simply echo it back.
---------------------------------------------------------------------------------------*/
#include "svr.h"

int main (int argc, char **argv)
{
	int i, maxi, nready, bytes_to_read, arg;
	int listen_sd, new_sd, sockfd, port, maxfd, client[FD_SETSIZE];
	struct in_addr clientName[FD_SETSIZE];
	unsigned int client_len;
	struct sockaddr_in server, client_addr;
	char *bp, buf[BUFLEN];
	ssize_t n;
	fd_set rset, allset;

	switch(argc)
	{
		case 1:
		port = SERVER_TCP_PORT;	// Use the default port
		break;
		case 2:
		port = atoi(argv[1]);	// Get user specified port
		break;
		default:
		fprintf(stderr, "Usage: %s [port]\n", argv[0]);
		exit(1);
	}

	// Create a stream socket
	if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	SystemFatal("Cannot Create Socket!");

	// set SO_REUSEADDR so port can be resused imemediately after exit, i.e., after CTRL-c
	arg = 1;
	if (setsockopt (listen_sd, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
	SystemFatal("setsockopt");

	// Bind an address to the socket
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

	if (bind(listen_sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	SystemFatal("bind error");

	// Listen for connections
	// queue up to LISTENQ connect requests
	listen(listen_sd, LISTENQ);

	maxfd	= listen_sd;	// initialize
	maxi	= -1;		// index into client[] array

	for (i = 0; i < FD_SETSIZE; i++)
	client[i] = -1;             // -1 indicates available entry
	FD_ZERO(&allset);
	FD_SET(listen_sd, &allset);


	while (TRUE)
	{
		rset = allset;               // structure assignment
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listen_sd, &rset)) // new client connection
		{
			printf("connected to client");

			client_len = sizeof(client_addr);
			if ((new_sd = accept(listen_sd, (struct sockaddr *) &client_addr, &client_len)) == -1)
			SystemFatal("accept error");

			printf("Remote Address:  %s\n", inet_ntoa(client_addr.sin_addr));

			for (i = 0; i < FD_SETSIZE; i++)
			if (client[i] < 0)
			{
				client[i] = new_sd;	// save descriptor
				clientName[i] = client_addr.sin_addr;
				break;
			}
			if (i == FD_SETSIZE)
			{
				printf ("Too many clients\n");
				exit(1);
			}
			FD_SET (new_sd, &allset);     // add new descriptor to set
			if (new_sd > maxfd)
			maxfd = new_sd;	// for select

			if (i > maxi)
			maxi = i;	// new max index in client[] array

			if (--nready <= 0)
			continue;	// no more readable descriptors
		}

		for (i = 0; i <= maxi; i++)	// check all clients for data
		{
			if ((sockfd = client[i]) < 0)
			continue;
			//memset(buf, '\0', BUFLEN);

			if (FD_ISSET(sockfd, &rset))
			{
				memset(buf, '\0', BUFLEN);
				n = read(sockfd, buf, 255);
				for (int j = 0; j <= maxi; j++)
				{
					if(client[j] != -1)
					write(client[j], buf, BUFLEN);
				}
				// echo to client

				if (n == 0) // connection closed by client
				{
					printf("Remote Address:  %s closed connection\n", inet_ntoa(clientName[i]));
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
			}
		}
	}
	return(0);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SystemFatal
--
-- DATE: March 28 2018
--
-- REVISIONS: None
--
-- DESIGNER: Aman Abdulla
--
-- PROGRAMMER: Aman Abdulla
--
-- INTERFACE: void SystemFatal(const char* message)
--
-- RETURNS: void.
--
-- NOTES:
-- Call this function to print the error message and gracefully end the program.
----------------------------------------------------------------------------------------------------------------------*/
static void SystemFatal(const char* message)
{
	perror (message);
	exit (EXIT_FAILURE);
}
