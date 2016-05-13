#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lecture_et_erreurs.h"

#define PORT 7890	

int main(void) {
	int sockfd, new_sockfd;  // listen on sockfd, new connection on new_sockfd
	struct sockaddr_in host_addr, client_addr;	// my address information
	socklen_t sin_size;
	int recv_length=1, yes=1;
	char buffer[1024];

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("erreur dans la creation du socket\n");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		fatal("erreur en changeant l option du socket a SO_REUSEADDR\n");
	
	host_addr.sin_family = AF_INET;		 // ordre octet hote
	host_addr.sin_port = htons(PORT);	 // ordre octets reseau
	host_addr.sin_addr.s_addr = INADDR_ANY; // remplir avec mon ip
	memset(&(host_addr.sin_zero), '\0', 8); // remplir avec des zeros

	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		fatal("erreur dans bind()\n");

	if (listen(sockfd, 5) == -1)  // maximum 5 tentatives dans la file
		fatal("erreur dans listen()\n");

	while(1) {    // accepter connection
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size); // la fonction accept retourne un nouveau socket 
											// de cette facon, le premier socket peut etre utilisé pr 
											// accepter de nouvelles connections alors que le 2eme
											// est utilisé pr communiquer avec le client
		if(new_sockfd == -1)
			fatal("erreur dans le socket de communication (2eme)\n");
		printf("le serveur recoit la connecxion dans le %s port %d ...\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		send(new_sockfd, "Bienvenu dans le programme serveur!\n", 50, 0);
		recv_length = recv(new_sockfd, &buffer, 1024, 0);
		while(recv_length > 0) {
			printf("RECV: %d bytes\n", recv_length);
    			printf("les donnees recus: %s",buffer);
			recv_length = recv(new_sockfd, &buffer, 1024, 0);
		}
		close(new_sockfd);
	}
	return 0;
}
