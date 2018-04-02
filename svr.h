#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_TCP_PORT 7000	// Default port
#define BUFLEN	255		//Buffer length
#define TRUE	1
#define LISTENQ	5
#define MAXLINE 4096

// Function Prototypes
static void SystemFatal(const char* );

/*------------------------------------------------------------------------------------
-- FUNCTION: IdleListen()
--
-- DATE:  March 4, 2018
--
-- REVISIONS: March 4, 2018
--							Initial file set up
--
-- DESIGNER: Li-Yan Tong & John Tee
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: void IdleListen()
--
-- RETURNS: void
--
-- NOTES:
-- Listens for new client connections and new messages in mux socket.
---------------------------------------------------------------------------------------*/
void IdleListen();

/*------------------------------------------------------------------------------------
-- FUNCTION: NewClient()
--
-- DATE:  March 4, 2018
--
-- REVISIONS: March 4, 2018
--							Initial file set up
--
-- DESIGNER: Li-Yan Tong & John Tee
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: void NewClient()
--
-- RETURNS: void
--
-- NOTES:
-- Set up a new connection for a new client process joining multicast address.
---------------------------------------------------------------------------------------*/
void NewClient();

/*------------------------------------------------------------------------------------
-- FUNCTION: ReceiveMSG()
--
-- DATE:  March 4, 2018
--
-- REVISIONS: March 4, 2018
--							Initial file set up
--
-- DESIGNER: Li-Yan Tong & John Tee
--
-- PROGRAMMER: Li-Yan Tong
--
-- INTERFACE: void ReceiveMSG()
--
-- RETURNS: void
--
-- NOTES:
-- Checks all currently socketed clients and places messages into a queue.
---------------------------------------------------------------------------------------*/
void ReceiveMSG();

/*------------------------------------------------------------------------------------
-- FUNCTION: MUXCast()
--
-- DATE:  March 4, 2018
--
-- REVISIONS: March 4, 2018
--							Initial file set up
--
-- DESIGNER: Li-Yan Tong & John Tee
--
-- PROGRAMMER: John Tee
--
-- INTERFACE: void PrintMux()
--
-- RETURNS: void
--
-- NOTES:
-- Sends a multicasted text messages to connected clients.
---------------------------------------------------------------------------------------*/
void MUXCast();

/*------------------------------------------------------------------------------------
-- FUNCTION: Terminate()
--
-- DATE:  March 4, 2018
--
-- REVISIONS: March 4, 2018
--							Initial file set up
--
-- DESIGNER: Li-Yan Tong & John Tee
--
-- PROGRAMMER: Li-Yan Tong
--
-- INTERFACE: void Terminate()
--
-- RETURNS:
--
-- NOTES:
-- Gracefully terminate the program.
---------------------------------------------------------------------------------------*/
void terminate();
