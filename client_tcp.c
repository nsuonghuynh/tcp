
/****************Client program********************************/

/* client_tcp.c is on zeus.cs.txstate.edu
   open a window on zeus.
   compile:
   $gcc -o c client_tcp.c
   $./c eros.cs.txstate.edu 8000
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

int main(int argc, char **argv){
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  unsigned short port;
  struct hostent *hostnm;

  uint32_t num, cnum;
  char msg[30];

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;

  /* set port number*/ 
  port = (unsigned short)atoi(argv[2]);

  /* htons() stands for "host to network short" and is used to convert the port number stored in the "port" variable from host byte order to network byte order, ensuring that data is not misinterpreted when sent over the network. */
  serverAddr.sin_port = htons(port);

  /* Set IP address to localhost */
  hostnm = gethostbyname(argv[1]);

/* This sets the serverAddr structure's sin_addr member to the host address provided by the hostnm->h_addr variable. The s_addr field contains the IP address of the host in network byte order, and the *((unsigned long *)hostnm->h_addr) expression casts the data to an unsigned long type. This ensures that the IP address taken from hostnm is formatted correctly for network communication. */
  serverAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr); 

/* Set all bits of the padding field to 0. It is used to ensure that the serverAddr structure is correctly zero initialized before use, which is necessary for certain network operations. */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) == 0)
	  printf("Connected to server successfully.\n");
  else printf("Couldn't connect to server.\n");


// communication starts from here

   // send an integer to the server
  printf("enter an integer:");
  scanf("%d", &num);

/* htonl stands for "host to network long" and is a function used in networking applications to convert values from host byte order to network byte order.*/
  cnum = htonl(num);
  printf("Attempting to send integer: %d \n", num);
  if (send(clientSocket, &cnum, sizeof(cnum), 0) != -1)
	  printf("Sent integer to server successfullly.\n");
  else printf("Error sending integer.\n");

  // receive a reply message from the server
  recv(clientSocket, msg, sizeof(msg), 0);
  printf("%s\n", msg);

  close(clientSocket);

  return 0;
}

