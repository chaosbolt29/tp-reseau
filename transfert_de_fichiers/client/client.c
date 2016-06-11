#include<stdio.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<arpa/inet.h>   
#include<netdb.h>      
#include "lecture_et_erreurs.h"
#define MAXBUFLEN 10000

int main(int argc , char *argv[])
{

    if(argc!=3)
        fatal("usage ./client 'addresse ip du serveur' port_sur_lequel_le_server_ecoute");

    int sock_client;
    struct sockaddr_in server;
    
    char fichier_a_recevoir[100] , fichier_demande[MAXBUFLEN];

    if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            fatal("erreur dans la creation du socket\n");

	server.sin_family = AF_INET;//IPV4
	server.sin_port = htons(atoi(argv[2])); // port sur lequel le server ecoute, 2eme argument
	server.sin_addr.s_addr = inet_addr(argv[1]); //remplir avec l'IP du server, 1er argument
	memset(&(server.sin_zero), '\0', 8);// remplir avec des zeros

    if(connect(sock_client, (struct sockaddr *) &server, sizeof server) == -1)
        fatal("erreur de connection au server\n");
    
    puts("Connection reussie\n");
    puts("Saisissez nom du fichier a telecharger: \n");
    bzero(&fichier_a_recevoir, sizeof(fichier_a_recevoir));
    
    gets(fichier_a_recevoir);          
    
    if(send(sock_client , fichier_a_recevoir , strlen(fichier_a_recevoir) , 0) < 0)
        fatal("dans l envoi du nom de fichier\n");

    bzero(&fichier_demande, sizeof(fichier_demande));
    
    puts("\r Le serveur prepare l envoi du buffer:\n");
    
    if(recv(sock_client , fichier_demande , MAXBUFLEN , 0) < 0)
        fatal("dans la reception du buffer\n");
    
    puts("\nCopie du buffer dans un nouveau fichier de meme nom\n");
    
    FILE *output_file;
	
    output_file = fopen( fichier_a_recevoir, "w" );
    if (!output_file) 
        fatal("ouverture du nouveau fichier dans lequel ecrire\n");

// *********************************************************
    
    fwrite(fichier_demande, 1,MAXBUFLEN,output_file);

    
//**********************************************************
    fclose(output_file);
    
 }
