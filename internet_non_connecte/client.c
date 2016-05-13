#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lecture_et_erreurs.h"

#define PORT 7890	

void main(int argc, char *argv[]) {
	
	if (argc != 2){
		fatal("usage: ./client 'addresse ip du serveur'");
	}
	int sockfd;  // socket client
	struct sockaddr_in host_addr;	
	socklen_t sin_size;
	char buffer[1024];
    
        if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("erreur dans la creation du socket\n");

	host_addr.sin_family = AF_INET;		 // ordre octet hote
	host_addr.sin_port = htons(PORT);	 // ordre octets reseau
	host_addr.sin_addr.s_addr = inet_addr(argv[1]); // remplir avec l'ip du server fournit comme premier argument'
	memset(&(host_addr.sin_zero), '\0', 8); // remplir avec des zeros        
      
        if(connect(sockfd, (struct sockaddr *) &host_addr, sizeof host_addr) == -1)
        	fatal("erreur de connection au server\n");

        send(sockfd, "Bonjour, je suis le client\n", 50, 0);

    	recv(sockfd, buffer, 1024, 0);
    
    	printf("les donnees recus: %s",buffer);
    
    
   
}    

