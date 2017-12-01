/*UDP Echo Client*/
#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFF_SIZE 1024

int main(int argc, char const *argv[])
{
	if (argc != 3) exit(1);
	int SERV_PORT;
	char SERV_IP[100];
	int client_sock;
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr;
	int bytes_sent,bytes_received, sin_size, total;
	
	SERV_PORT = atoi(argv[1]);
	strcpy(SERV_IP, argv[2]);
	//Construct a UDP socket
	if ((client_sock=socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){  /* calls socket() */
		perror("\nError: ");
		exit(1);
	}

	//Define the address of the server
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERV_IP);
	
	while(1){
		//Communicate with server
		printf("\nInsert string to send:");
		// memset(buff,'\0',(strlen(buff)+1));
		fgets(buff, BUFF_SIZE, stdin);
		if ((buff[0] == 'q' || buff[0] == 'Q') && buff[1] == '\n') break;
		sin_size = sizeof(struct sockaddr);
	
		bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
		if(bytes_sent < 0){
			perror("Error: ");
			close(client_sock);
			return 0;
		}

		bytes_received = recvfrom(client_sock, &total, BUFF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &sin_size);
		if(bytes_received < 0){
			perror("Error: ");
			close(client_sock);
			return 0;
		}
		
		if (total != -1)
		{
			printf("Reply from server: %d\n", total);
		} else printf("An error occurred");
	}
	close(client_sock);
	return 0;
}
