/* fread example: read an entire file */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#define MAX_FRAGMENT_SIZE 4096

int main () {

	FILE *image;
	int imageSize, read_size, stat, packet_index, total_frag;
	char send_buffer[MAX_FRAGMENT_SIZE], read_buffer[256];
	
	packet_index = 1;

	image = fopen("church.jpg", "r");

	if(image == NULL) 
	{
		printf("Error Opening Image File"); 
	} 

	fseek(image, 0, SEEK_END);
	imageSize = ftell(image);
	fseek(image, 0, SEEK_SET);

	printf("Total image size: %i\n",imageSize);

	total_frag = ceil((float)(imageSize/MAX_FRAGMENT_SIZE));

	// printf("Total number of fragments = %i\n", total_frag);



	while(!feof(image)) 
	{
		//while(packet_index = 1){
		//Read from the file into our send buffer
		read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, image);

		//Send data through our socket 
		
		// do
		// {
		// 	stat = write(socket, send_buffer, read_size);  
		
		// }while (stat < 0);

		printf("Packet Number: %i\n",packet_index);
		printf("Packet Size Sent: %i\n",read_size);     
		printf(" \n");
		printf(" \n");


		packet_index++;  

		//Zero out our send buffer
		bzero(send_buffer, sizeof(send_buffer));
	}


	return 0;
}