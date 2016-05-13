#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h> 
#include "lecture_et_erreurs.h"

#define PORT 7890

int main(int argc , char *argv[])
{


 int socket_desc , client_sock , c , read_size;

 struct sockaddr_in server , client;
 bzero((char*)&server,sizeof(server));
 char client_message[100],msg[100];
 


 if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        fatal("erreur dans la creation du socket\n");

 puts("Socket cree");
 

server.sin_family = AF_INET;          // ordre octet hote
server.sin_port = htons(PORT);        // ordre octets reseau
server.sin_addr.s_addr = INADDR_ANY; // remplir avec mon ip
memset(&(server.sin_zero), '\0', 8); // remplir avec des zeros

 
 
 if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
                fatal("erreur dans bind()\n");

if (listen(socket_desc, 5) == -1)  // maximum 5 tentatives dans la file
        fatal("erreur dans listen()\n");


 

 puts("\n\n en attente de connections...");
 c = sizeof(struct sockaddr_in);
 
 //accepter connection
 client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
 if (client_sock < 0)
	 fatal("erreur dans accept");
 puts("Connection acceptee");
 
 // recevoir un message du client
 bzero(&client_message, sizeof(client_message));
 while( (read_size = recv(client_sock , client_message , 100 , 0)) > 0 )

 {
  puts("\nClient message: ");
  puts(client_message);
  printf("\saisissez message a envoyer au client: ");
  bzero(&msg, sizeof(msg));
  gets(msg);

  if(write(client_sock , msg , sizeof(msg)) < 0)
  	fatal("client deconnecte");
  
  bzero(&client_message, sizeof(client_message));
 }
 
 if(read_size == 0)
 {
  puts("client deconnecte");
  fflush(stdout);
 }
 else if(read_size == -1)
 {
  perror("erreur de reception");
 }

 return 0;
}
