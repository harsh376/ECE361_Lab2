/* Echo server using UDP */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_UDP_PORT 5000	// well-known port
// #define MAXLEN 1106				// maximum data length
// -- #define MAXLEN 1000				// maximum data length
#define MAXLEN 1200				// maximum data length

typedef struct packet {
	unsigned int total_frag;
	unsigned int frag_no;
	unsigned int size;
	char filename[100];
	char filedata[1000];
}packet;

int main(int argc, char **argv)
{
	int sd, client_len, port, n, firstPacket;
	char buf[MAXLEN];
	char** fileInfo;
	struct sockaddr_in server, client;

	firstPacket = 1;

	char buf_ACK[MAXLEN] = "ACK\n";

	switch(argc)
	{
		case 1:
			port = SERVER_UDP_PORT;
			break;

		case 2:
			port = atoi(argv[1]);
			break;

		default:
			fprintf(stderr, "Usage: %s [port]\n", argv[0]);
			exit(1);
	}

	// Create a Datagram Socket
	if((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		fprintf(stderr, "Can't create a socket\n");
		exit(1);
	}

	// Bind an address to the socket
	bzero((char *)&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		fprintf(stderr, "Can't bind name to socket\n");
		exit(1);
	}

	packet sendPack;


	while(1)
	{
		client_len = sizeof(client);
		if((n = recvfrom(sd, buf, MAXLEN, 0, (struct sockaddr *)&client, &client_len)) < 0)
		{
			fprintf(stderr, "Can't receive datagram\n");
			exit(1);
		}
		else
		{

			FILE *fp;

			fp = fopen( "file.jpg" , "a" );
			fwrite(&(buf[200]) , 1 , 1000 , fp); 
			
			fclose(fp);
	

			// printf("%s\n", b);

		}

		char bytes1[20];
	    // itoa(n, bytes1, 10);

		snprintf(bytes1, 20,"%d",n);

		if(sendto(sd, bytes1, sizeof(bytes1), 0, (struct sockaddr *)&client, client_len) != sizeof(bytes1))
		{
			fprintf(stderr, "Can't send datagram\n");
			exit(1);
		}
	}


	close(sd);
	return 0;

}

