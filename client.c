/*-----------------------------------------------------------
Client a lancer apres le serveur avec la commande :
client <adresse-serveur> <message-a-transmettre>
------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "functions.h"

typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;

int main(int argc, char **argv) {

    int 	socket_descriptor, 	/* descripteur de socket */
		longueur; 		/* longueur d'un buffer utilisé */
    sockaddr_in adresse_locale; 	/* adresse de socket local */
    hostent *	ptr_host; 		/* info sur une machine hote */
    servent *	ptr_service; 		/* info sur service */
    char 	buffer[256];
    char *	prog; 			/* nom du programme */
    char *	host; 			/* nom de la machine distante */
    char *	mesg; 			/* message envoyé */

    if (argc != 2) {
	perror("usage : client <adresse-serveur> <message-a-transmettre>");
	exit(1);
    }

    prog = argv[0];
    host = argv[1];
    //mesg = argv[2];

    printf("nom de l'executable : %s \n", prog);
    printf("adresse du serveur  : %s \n", host);
    //printf("message envoye      : %s \n", mesg);

    if ((ptr_host = gethostbyname(host)) == NULL) {
	perror("erreur : impossible de trouver le serveur a partir de son adresse.");
	exit(1);
    }

    /* copie caractere par caractere des infos de ptr_host vers adresse_locale */
    bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
    adresse_locale.sin_family = AF_INET; /* ou ptr_host->h_addrtype; */

    /* 2 facons de definir le service que l'on va utiliser a distance */
    /* (commenter l'une ou l'autre des solutions) */

    /*-----------------------------------------------------------*/
    /* SOLUTION 1 : utiliser un service existant, par ex. "irc" */
    /*
    if ((ptr_service = getservbyname("irc","tcp")) == NULL) {
	perror("erreur : impossible de recuperer le numero de port du service desire.");
	exit(1);
    }
    adresse_locale.sin_port = htons(ptr_service->s_port);
    */
    /*-----------------------------------------------------------*/

    /*-----------------------------------------------------------*/
    /* SOLUTION 2 : utiliser un nouveau numero de port */
    adresse_locale.sin_port = htons(5000);
    /*-----------------------------------------------------------*/

    printf("numero de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));

    /* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("erreur : impossible de creer la socket de connexion avec le serveur.");
	exit(1);
    }

    /* tentative de connexion au serveur dont les infos sont dans adresse_locale */
    if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
	perror("erreur : impossible de se connecter au serveur.");
	exit(1);
    }

    printf("connexion etablie avec le serveur. \n");

    printf("envoi d'un message au serveur. \n");

    /* envoi du message vers le serveur
    if ((write(socket_descriptor, mesg, strlen(mesg))) < 0) {
        perror("erreur : impossible d'ecrire le message destine au serveur.");
        exit(1);
    }*/

    /* mise en attente du prgramme pour simuler un delai de transmission */
    sleep(3);

    printf("message envoye au serveur. \n");
    int sz_msg_client = 100;
    char *msg_client = malloc(sz_msg_client*sizeof(char));
    //char msg_client[30];

    /* lecture de la reponse en provenance du serveur */
    int sz_buffer = 200;
    char *msg_server = calloc(sz_buffer, sizeof(char));
    //while((longueur = read(socket_descriptor, msg_server, 200)) > 0) {
    while((longueur = my_read_socket(socket_descriptor, &msg_server, sz_buffer)) > 0) {
        //printf("reponse du serveur : \n");
        //write(1, str_sub(buffer, 3, sizeof(buffer)), sizeof(buffer)-3);
        printf("%s", str_sub(msg_server, 3, strlen(msg_server)));
        fflush(stdout);
        if(atoi(str_sub(msg_server, 0, 2))==100){// scanf a values
            printf("CLIENT>>");

            scanf("%s", msg_client);
            fflush(stdin);
            //write(socket_descriptor, msg_client, strlen(msg_client));
            my_write_socket(socket_descriptor, msg_client, strlen(msg_client));
        }
        msg_client = malloc(sz_msg_client*sizeof(char));
        msg_server = malloc(sz_buffer*sizeof(char));
    }

    printf("\nfin de la reception.\n");

    close(socket_descriptor);

    printf("connexion avec le serveur fermee, fin du programme.\n");

    //free(msg_server);
    exit(0);

}
