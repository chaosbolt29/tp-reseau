#include<stdio.h> 
#include<string.h> 
#include<sys/socket.h> 
#include<arpa/inet.h>   
#include<netdb.h>      
#include "lecture_et_erreurs.h"

#define PORT 7890

int main(int argc , char *argv[])
{
 
 if(argc!=2)
	 fatal("usage ./client 'addresse ip du serveur'");

 int sockfd;

 struct sockaddr_in server;
 struct hostent *host;

 char message[100] , server_reply[100];
 

 sockfd = socket(AF_INET , SOCK_STREAM , 0);

 if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
         fatal("erreur dans la creation du socket\n");

 
server.sin_family = AF_INET;          // ordre octet hote
server.sin_port = htons(PORT);        // ordre octets reseau
server.sin_addr.s_addr = inet_addr(argv[1]); // remplir avec l'ip du se$
memset(&(server.sin_zero), '\0', 8); // remplir avec des zeros        


if(connect(sockfd, (struct sockaddr *) &server, sizeof server) == -1)
        fatal("erreur de connection au server\n");


 
 puts("Connection reussie\n");
 
 puts("Saisissez un message: \n");
 bzero(&message, sizeof(message));
 gets(message);  		

 send(sockfd , message , strlen(message) , 0) ;
  
 bzero(&server_reply, sizeof(server_reply));

 while( recv(sockfd , server_reply , 100 , 0) > 0)
 {
  
  puts("\r Reponse serveur :\n");
  puts(server_reply);
  bzero(&server_reply, sizeof(server_reply));
  puts("Saisissez message (tapez bye pour quitter) ");
  bzero(&message, sizeof(message));
  gets(message);
  send(sockfd , message , strlen(message) , 0) ;
  if(strcmp(message,"bye")==0)
   break;
  
 }
}
