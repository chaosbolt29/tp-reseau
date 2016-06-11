#include<stdio.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<arpa/inet.h>   
#include<netdb.h>      
#include "lecture_et_erreurs.h"
#define MAXBUFLEN 10000

int main(int argc, char *argv[])
{
	if(argc!=2)
		fatal("usage ./server 'port sur lequel ecouter'\n");
	
	int sockfd, sock_client, c, read_size;
	
	struct sockaddr_in server, client;
	bzero((char*)&server, sizeof(server));

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		fatal("creation de socket\n");
	printf("Socket sockfd cree\n");

	server.sin_family = AF_INET;//IPV4
	server.sin_port = htons(atoi(argv[1])); // port sur lequel ecouter, 1er argument
	server.sin_addr.s_addr = INADDR_ANY; //remplir avec mon IP
	memset(&(server.sin_zero), '\0', 8);// remplir avec des zeros
	
    int true = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
    
	if(bind(sockfd,(struct sockaddr *)&server, sizeof(server)) < 0)
		fatal("Bind\n");
	if(listen(sockfd, 5)==-1) // max 5 tentatives de connexion dans la file
		fatal("listen\n");
	printf("\n En attente de connexions...\n\n");
	c = sizeof(struct sockaddr_in);

	//accepter connexion
	sock_client = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&c);
	if (sock_client < 0)
		fatal("dans accept\n");
	printf("Connexion acceptee\n\n");

	//recevoir le path du fichier a envoyer et envoi du ficher
	char fichier_a_envoyer[100], fichier_demande[MAXBUFLEN];
	bzero(&fichier_a_envoyer, sizeof(fichier_a_envoyer));
    
	if((read_size = recv(sock_client, fichier_a_envoyer, 100, 0)) < 0)
        fatal("dans la reception du nom du fichier\n");
	
    puts("\n Le client demande le fichier: ");
    puts(fichier_a_envoyer);
    
    puts("\nCopie du fichier demande dans le buffer\n");
    bzero(&fichier_demande, sizeof(fichier_demande));
    
    FILE *input_file;
    input_file = fopen( fichier_a_envoyer, "r" );
    if (!input_file) 
        fatal("ouverture du fichier demande");
    

// **************************copie du contenue du fichier_a_envoyer dans le buffer fichier_demande*******************************
    size_t newLen = fread(fichier_demande, sizeof(char), MAXBUFLEN, input_file);
    if (newLen == 0) {
        fatal("dans la lecture du fichier_demande");
    } else {
        fichier_demande[newLen++] = '\0'; 
    }
    fclose(input_file);
    
 //**********************************************************************************************

// ecriture du buffer fichier_damande dans le socket client
    if(write(sock_client , fichier_demande , sizeof(fichier_demande)) < 0)
  	fatal("client deconnecte");


    
	
    return 0;
    
}
