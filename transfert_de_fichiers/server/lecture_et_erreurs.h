#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Affiche message d'erreur et exit

void fatal(char *message) {
   char error_message[100];

   strcpy(error_message, "[!!] Fatal Error ");
   strncat(error_message, message, 83);
   perror(error_message);
   exit(-1);
}

        
        


