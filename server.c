/*----------------------------------------------
Serveur à lancer avant le client
------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h> 	/* pour les sockets */
#include <sys/socket.h>
#include <netdb.h> 		/* pour hostent, servent */
#include <string.h> 		/* pour bcopy, ... */
#include "cards_manager.h"
#include "functions.h"
#include "database.h"
#define TAILLE_MAX_NOM 256

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

/* ------------------ */
/*--- Files defined as database ---*/
FILE* f_card;
FILE* f_client;
FILE*f_transactions;
FILE*f_logs;
/*------------------------------------------------------*/
void renvoi (int sock) {
    //printf("new connection \n");
    char buffer[256];
   // char *question;
    int longueur;

    int buf_sz = 200;
    int qt_sz = 50;
    char *msg_client = malloc(buf_sz*sizeof(char));
    char *question = malloc(buf_sz*sizeof(char));
    start:
        //msg_client = malloc(buf_sz*sizeof(char));
        //printf("before scan:%s\n", question);
        question = "100BANK >> Do you need a card ?\n";
        //printf("question scan:%s\n", question);
        fflush(stdout);
        my_write_socket(sock, question, strlen(question));
        printf("scan sent:%s\n", question);

        if ((longueur = my_read_socket(sock, &msg_client, buf_sz)) <= 0)
            return;

        printf("message lu 1 : %s \n", msg_client);
        //if(msg_client[0]=='y' || strncasecmp(msg_client, "yes", 3)==0){// Generating a new card
        if(strcasecmp(msg_client,"y")==0 || strcasecmp(msg_client, "yes")==0){
            char firstname[30];
            char *lastname = malloc(30*sizeof(char));
            char *birthday = malloc(15*sizeof(char));

            //first name
            question = "100BANK >> Type your first name please:\n";
            my_write_socket(sock, question,strlen(question));
            //
            if ((longueur = my_read_socket(sock, &msg_client, buf_sz)) <= 0)
                return;
            sprintf(firstname, "%s", msg_client);

            //last name
            msg_client = malloc(buf_sz*sizeof(char));
            question = "100BANK >> Type your last name please:\n";
            my_write_socket(sock, question,strlen(question));
            //fflush(stdout);
            if ((longueur = my_read_socket(sock, &msg_client, buf_sz)) <= 0)
                return;
            sprintf(lastname, "%s", msg_client);

            msg_client = malloc(buf_sz*sizeof(char));
            //birthday
            question = "100BANK >> Type your birthday (ex 04/02/1956) please:\n";
            my_write_socket(sock, question,strlen(question));
            //fflush(stdout);
            if ((longueur = my_read_socket(sock, &msg_client, buf_sz)) <= 0)
                return;
            sprintf(birthday, "%s", msg_client);

            int client_id = save_client(f_client, firstname, lastname, birthday);

            if(client_id>0){
                char *card_infos = malloc(27*sizeof(char));
                card_infos = generate_card();
                printf("***** CLIENT OK: %s \n", card_infos);
                char *new_question = malloc(buf_sz*sizeof(char));

                sprintf(new_question, "200BANK >> Your new card:|Number:%s |Expiration date:%s |Code:%s\n", str_sub(card_infos, 0, 15), str_sub(card_infos, 21, 26), str_sub(card_infos, 17, 19));
                //Save card and user

                save_card(f_card, card_infos, client_id);
                //printf("***** AFTER \n");

                my_write_socket(sock, new_question,strlen(new_question));
                //fflush(stdout);

                free(card_infos);
                free(new_question);
            }
        }
        else{ // Other treatment
            //last name
            typing_card_infos:
            msg_client = malloc(buf_sz*sizeof(char));
            char *card_infos = malloc(28*sizeof(char));
            question = "100BANK >> Type card infos [number-date-code]:\n";
            my_write_socket(sock, question,strlen(question));

            if ((longueur = my_read_socket(sock, &msg_client, buf_sz)) <= 0)
                return;
            sprintf(card_infos, "%s", msg_client);

            int resp = 0;
            if(strlen(card_infos)==27){
                resp = check_card_number(str_sub(card_infos, 0, 15), str_sub(card_infos, 17, 22), str_sub(card_infos, 24, 26));
            }

            if(!resp){
                question = "300BANK >> Error: Card unknown, try again:\n";
                my_write_socket(sock, question,strlen(question));

                goto typing_card_infos;
            }
            else{
                choose_option:
                msg_client = malloc(buf_sz*sizeof(char));
                char *choice = malloc(1*sizeof(char));

                question = "100BANK >> Please choose an operation to perform:\n    1:Activate card\n    2:Debit card\n    3:Credit card\n    4:See you balance\n    5:See transactions list\n";
                my_write_socket(sock, question,strlen(question));

                if ((longueur = my_read_socket(sock, &msg_client, buf_sz)) <= 0)
                    return;
                sprintf(choice, "%s", msg_client);

                printf("choice: %s\n", choice);
                char *number = malloc(16*sizeof(char));
                sprintf(number, "%s", str_sub(card_infos, 0, 15));
                char *exp_date = malloc(6*sizeof(char));
                sprintf(exp_date, "%s", str_sub(card_infos, 17, 22));
                char *code = malloc(3*sizeof(char));
                sprintf(code, "%s", str_sub(card_infos, 24, 26));

                int resp_f;
                double amount;
                switch(atoi(choice)){
                    case 0:
                        perror("Error : Unknown option, please choose option !");
                    break;

                    case 1: // activate card
                        resp_f = activate_card(f_card, number, exp_date, code);
                        if(resp_f){
                            question = "200BANK >> SUCCESS: Card activated,You can perform operations.\n";
                        }
                        else{
                            question = "300BANK >> Error: Sorry card can not be activated!\n";
                        }

                        my_write_socket(sock, question,strlen(question));
                        goto choose_option;
                    break;

                    case 2: //debit(double amount);
                        resp = check_card(f_card, number, exp_date, code);

                        if(resp==-1){
                            question = "300BANK >> Error: Card not activated, Please active you card first!\n";
                            my_write_socket(sock, question,strlen(question));

                            goto choose_option;
                        }
                        // ask amount
                        typing_amount_debit:
                        msg_client = malloc(buf_sz*sizeof(char));
                        //char *mnt = malloc(28*sizeof(char));
                        question = "100BANK >> Type amount to debit [1000.25]:\n";
                        my_write_socket(sock, question,strlen(question));

                        if ((longueur = my_read_socket(sock, &msg_client, buf_sz)) <= 0)
                            return;
                        printf("amount: %s\n", msg_client);
                        amount = atof(msg_client);
                        if(amount<=0.00){
                            question = "300BANK >> Error: Amount is wrong, try again:\n";
                            my_write_socket(sock, question,strlen(question));

                            goto typing_amount_debit;
                        }
                        else{
                            int deb = debit_card(f_card, number, exp_date, code, amount);
                            if(deb==-1){
                                question = "300BANK >> Error: You don't have enough funds. Please credit your card!\n";
                                my_write_socket(sock, question,strlen(question));

                                goto typing_amount_debit;
                            }
                            else if(deb==1){
                                // saving transaction
                                save_transaction(f_transactions, "D", number, "", amount);
                                question = malloc(buf_sz*sizeof(char));
                                sprintf(question, "200BANK >> SUCCESS: [%.2f] debited from your card!\n", amount);
                            }
                            else
                                question = "300BANK >> Error: Can not debited card!\n";
                        }
                    break;

                    case 3: //credit(double amount);
                        // ask amount
                        typing_amount_credit:
                        msg_client = malloc(buf_sz*sizeof(char));
                        //char *mnt = malloc(28*sizeof(char));
                        question = "100BANK >> Type amount to credit [ex 1000.25]:\n";
                        my_write_socket(sock, question,strlen(question));

                        if ((longueur = my_read_socket(sock, &msg_client, buf_sz)) <= 0)
                            return;
                        printf("amount cred: %s\n", msg_client);
                        amount = atof(msg_client);
                        if(amount<=0.00){
                            question = "300BANK >> Error: Amount is wrong, try again:\n";
                            my_write_socket(sock, question,strlen(question));

                            goto typing_amount_credit;
                        }
                        else{
                            int cred = credit_card(f_card, number, exp_date, code, amount);
                            if(cred==1){
                                //question = "";
                                save_transaction(f_transactions, "C", number, "", amount);
                                question = malloc(buf_sz*sizeof(char));
                                sprintf(question, "200BANK >> SUCCESS: [%.2f] added to your card!\n", amount);
                            }
                            else
                                question = "300BANK >> Error: Can not credited card!\n";
                        }
                    break;

                    case 4: //get balance
                        //
                        question = malloc(buf_sz*sizeof(char));
                        sprintf(question, "200BANK >> Your balance at this time is: |%.2f|\n", get_balance(f_card, number, exp_date, code));
                    break;

                    case 5: //releve
                        question = malloc(buf_sz*sizeof(char));
                        sprintf(question, "200BANK >> Your transactions list:%s\n", list_transactions(f_transactions, number));
                    break;

                    case 6: //transfert(char *num_dest, double amount);
                    break;

                    default:
                        //perror("Error : Unknown option, please choose option !");
                        //response = "Error : Unknown option, please choose option !";
                    break;
                }
                my_write_socket(sock, question,strlen(question));
            }
        }

        //free(question);
        //msg_client = NULL;
        printf("**************** End Treatments *****************.\n");
        fflush(stdout);
        fflush(stdin);
        question = malloc(buf_sz*sizeof(char));
        //printf("after trea:%s\n", question);
    goto start;
    //Decrypt message

    //printf("RESP : %s \n", response);



   // write(sock,response,strlen(response)+1);
    //free(response);

    printf("message envoye. \n");

    return;

}

/*------------------------------------------------------*/

/*------------------------------------------------------*/
main(int argc, char **argv) {
    //
    f_card = fopen("database/cards.dat", "r+");
    f_client = fopen("database/clients.dat", "r+");
    f_transactions = fopen("database/transactions.dat", "r+");
    f_logs = fopen("database/logs.log", "r+");

    int 		socket_descriptor, 		/* descripteur de socket */
			nouv_socket_descriptor, 	/* [nouveau] descripteur de socket */
			longueur_adresse_courante; 	/* longueur d'adresse courante d'un client */
    sockaddr_in 	adresse_locale, 		/* structure d'adresse locale*/
			adresse_client_courant; 	/* adresse client courant */
    hostent*		ptr_hote; 			/* les infos recuperees sur la machine hote */
    servent*		ptr_service; 			/* les infos recuperees sur le service de la machine */
    char 		machine[TAILLE_MAX_NOM+1]; 	/* nom de la machine locale */

    gethostname(machine,TAILLE_MAX_NOM);		/* recuperation du nom de la machine */

    /* recuperation de la structure d'adresse en utilisant le nom */
    if ((ptr_hote = gethostbyname(machine)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son nom.");
		exit(1);
    }

    /* initialisation de la structure adresse_locale avec les infos recuperees */

    /* copie de ptr_hote vers adresse_locale */
    bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
    adresse_locale.sin_family		= ptr_hote->h_addrtype; 	/* ou AF_INET */
    adresse_locale.sin_addr.s_addr	= INADDR_ANY; 			/* ou AF_INET */

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
    /* SOLUTION 2 : utiliser un nouveau numero de port */
    adresse_locale.sin_port = htons(5000);
    /*-----------------------------------------------------------*/

    printf("numero de port pour la connexion au serveur : %d \n",
		   ntohs(adresse_locale.sin_port) /*ntohs(ptr_service->s_port)*/);

    /* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("erreur : impossible de creer la socket de connexion avec le client.");
		exit(1);
    }

    /* association du socket socket_descriptor à la structure d'adresse adresse_locale */
    if ((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("erreur : impossible de lier la socket a l'adresse de connexion.");
		exit(1);
    }

    //log
    /*char host[16];
    char serv[16];
    if (getnameinfo((const struct sockaddr *) &adresse_locale, adresse_locale.sin_len, host, sizeof(host), serv, sizeof(serv), 0) != 0)
    {
        perror("Failed to get server details\n");
        exit(1);
    }*/
    char *msg_log = malloc(100*sizeof(char));
    if(msg_log==NULL){
        perror("Failed to allocate memory to msg\n");
        exit(1);
    }
    char addr_svr[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &(adresse_locale.sin_addr.s_addr), addr_svr, INET_ADDRSTRLEN );
    sprintf(msg_log, "Server started on %s/%d", addr_svr, ntohs(adresse_locale.sin_port));
    logs(f_logs, "SERVER", addr_svr, msg_log);
    //free(msg_log);

    /* initialisation de la file d'ecoute */
    listen(socket_descriptor,5);

    /* attente des connexions et traitement des donnees recues */
    for(;;) {

		longueur_adresse_courante = sizeof(adresse_client_courant);

		/* adresse_client_courant sera renseigné par accept via les infos du connect */
		if ((nouv_socket_descriptor =
			accept(socket_descriptor,
			       (sockaddr*)(&adresse_client_courant),
			       &longueur_adresse_courante))
			 < 0) {
			perror("erreur : impossible d'accepter la connexion avec le client.");
			exit(1);
		}
		//log
		char addr_client[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &(adresse_client_courant.sin_addr.s_addr), addr_client, INET_ADDRSTRLEN );

        msg_log = malloc(100*sizeof(char));
        sprintf(msg_log, "Connection accepted from client: %s/%d", addr_client, ntohs(adresse_client_courant.sin_port));
        logs(f_logs, "CLIENT", addr_client, msg_log);
        //free(msg_log);

        // Add logs connections
		switch(fork()){
		    case 0:
                close(socket_descriptor); // cloture de la socket d'écoute des connexions
                    /* traitement du message */
                //printf("reception d'un message.\n");

                renvoi(nouv_socket_descriptor);

                close(nouv_socket_descriptor);
                //log

                msg_log = malloc(100*sizeof(char));
                sprintf(msg_log, "Connection close with client: %s/%d", addr_client, ntohs(adresse_client_courant.sin_port));
                logs(f_logs, "CLIENT", addr_client, msg_log);
            break;

            case -1: //problème sur le père
                perror(">> impossible de créer un fils ");
                exit(1);
            break;

            default : // cas du père
                //perror(">> nothing");
            //close(nouv_socket_descriptor);
            break;
		}



    }

}
