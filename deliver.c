/* A simple UDP client which measures round trip delay */ 

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <math.h>
#define SERVER_UDP_PORT 5000 
#define MAX_FRAGMENT_SIZE 1000 	/* maximum data length */
#define DEFLEN 64		/* default length */

typedef struct packet {
	unsigned int total_frag;
	unsigned int frag_no;
	unsigned int size;
	char filename[100];
	char filedata[1000];
}packet;
 
long delay(struct timeval t1, struct timeval t2);

int main(int argc, char **argv)
{

	char packetString[MAX_FRAGMENT_SIZE+500];
	int data_size = DEFLEN, port = SERVER_UDP_PORT;
	int i, j, sd, server_len;
	char *pname, *host, rbuf[MAX_FRAGMENT_SIZE], sbuf[MAX_FRAGMENT_SIZE];
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
	FILE *file = fopen(argv[4], "r");

	if(file == NULL)
	{
		fprintf(stderr, "Couldn't open file\n");
		exit(1);
	}

	int fileSize, read_size, stat, packet_index, total_frag;
	char send_buffer[MAX_FRAGMENT_SIZE], read_buffer[256];
	
	packet_index = 1;

	if(file == NULL) 
	{
		printf("Error Opening File\n"); 
	} 

	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	printf("Total image size: %i\n",fileSize);

	packet sendPack;

	total_frag = ceil((float)fileSize/MAX_FRAGMENT_SIZE);

	sendPack.total_frag = total_frag;
	strcpy(sendPack.filename, argv[4]);

	printf("Total number of fragments = %i\n", total_frag);


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

	

	while(!feof(file)) 
	{
		//while(packet_index = 1){
		//Read from the file into our send buffer
		read_size = fread(sendPack.filename, 1, sizeof(send_buffer)-1, file);

		
		sendPack.frag_no = packet_index;
		sendPack.size = read_size;

		// printf("Packet Number: %i\n",packet_index);
		// printf("Packet Size Sent: %i\n",read_size);     
		// printf(" \n");
		// printf(" \n");

		//Send data through our socket 
		
		sprintf(packetString, "%d:%d:%d:%s:%s", sendPack.total_frag,sendPack.frag_no, sendPack.size, sendPack.filename, sendPack.filedata);

		/* transmit data */
		server_len = sizeof(server);
		
		if (sendto(sd, packetString, sizeof(packetString), 0, (struct sockaddr *) &server, server_len) == -1) 
		{ 
			fprintf(stderr, "sendto error\n");
			exit(1);
		}

		/* receive data */
		if (recvfrom(sd, rbuf, MAX_FRAGMENT_SIZE, 0, (struct sockaddr *) &server, &server_len) < 0) 
		{ 
			fprintf(stderr, "recvfrom error\n"); 
			exit(1);
		}  

		if (strncmp(send_buffer, rbuf, data_size) != 0) 
		{
			printf("Data is corrupted\n");
		}
		else
		{
			printf("ACK received\n");
		}

		packet_index++;  

		//Zero out our send buffer
		bzero(send_buffer, sizeof(send_buffer));
	}
	


	gettimeofday(&start, NULL); /* start delay measure */

	gettimeofday(&end, NULL); /* end delay measure */

	printf ("Round-trip delay = %ld ms.\n", delay(start, end));

	

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


