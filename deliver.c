/* A simple UDP client which measures round trip delay */ 

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#define SERVER_UDP_PORT 5000 
#define MAXLEN 4096 	/* maximum data length */
#define DEFLEN 64		/* default length */

 
long delay(struct timeval t1, struct timeval t2);

int main(int argc, char **argv)
{

	int data_size = DEFLEN, port = SERVER_UDP_PORT;
	int i, j, sd, server_len;
	char *pname, *host, rbuf[MAXLEN], sbuf[MAXLEN];
	struct hostent *hp;
	struct sockaddr_in server, client;
	struct timeval start, end;

	int port_client;

	/*
	deliver <server address> <server port number> <client listen port> <file name> 
	*/

	if(argc != 5)
	{
		fprintf(stderr, "Usage: ./deliver <server address> <server port number> <client listen port> <file name>\n" );
		exit(1);
	}


	host = argv[1];
	port = atoi(argv[2]);
	port_client = atoi(argv[3]);


	/* Opening the file to be sent */
	FILE *file = fopen("hello.txt", "r");

	if(file == NULL)
	{
		fprintf(stderr, "Couldn't open file\n");
		exit(1);
	}


	/* Create a datagram socket */
	if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
		fprintf(stderr, "Can't create a socket\n");
		exit(1); 
	}



	/* Store server's information */ 
	bzero((char *)&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if ((hp = gethostbyname(host)) == NULL) 
	{
		fprintf(stderr, "Can't get server's IP address\n");
		exit(1);
	}

	bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);



	/* Bind local address to the socket */
	bzero((char *)&client, sizeof(client)); 
	client.sin_family = AF_INET;
	client.sin_port = htons(port_client);
	client.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sd, (struct sockaddr *)&client, sizeof(client)) == -1) 
	{
		fprintf(stderr, "Can't bind name to socket\n");
		exit(1); 
	}

	if (data_size > MAXLEN) 
	{
		fprintf(stderr, "Data is too big\n");
		exit(1); 
	}

	/* data is a, b, c, ..., z, a, b, ... */ 
	for (i = 0; i < data_size; i++) 
	{
		j = (i < 26) ? i : i % 26;
		sbuf[i] = "a" + j; 
	}



	gettimeofday(&start, NULL); /* start delay measure */
	
	/* transmit data */
	server_len = sizeof(server);
	
	if (sendto(sd, sbuf, data_size, 0, (struct sockaddr *) &server, server_len) == -1) 
	{ 
		fprintf(stderr, "sendto error\n");
		exit(1);
	}

	/* receive data */
	if (recvfrom(sd, rbuf, MAXLEN, 0, (struct sockaddr *) &server, &server_len) < 0) 
	{ 
		fprintf(stderr, "recvfrom error\n"); 
		exit(1);
	}

	gettimeofday(&end, NULL); /* end delay measure */

	printf ("Round-trip delay = %ld ms.\n", delay(start, end));

	if (strncmp(sbuf, rbuf, data_size) != 0) 
	{
		printf("Data is corrupted\n");
	}
	else
	{
		printf("ACK received\n");
	}

	// printf("client: buffer received = %s\n", rbuf);

	close(sd);
	return(0); 
}


/*
* Compute the delay between t1 and t2 in milliseconds */
long delay (struct timeval t1, struct timeval t2)
{
	long d;
	d = (t2.tv_sec - t1.tv_sec) * 1000;
	d += ((t2.tv_usec - t1.tv_usec + 500) / 1000); 
	return(d);
}


