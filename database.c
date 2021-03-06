#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "database.h"
#include "functions.h"
#include "cards_manager.h"

typedef struct
{
    int id;
    char *firstname;
    char *lastname;
    char *birthday;
} client;

int save_client(FILE* f_client, char *firstname, char *lastname, char *birthday)
{
    //FILE* f_client = fopen("database/clients.dat", "r+");
    int max_id=0, usr_id;
    printf("saving client: %s-%s-%s\n", firstname, lastname, birthday);
    if (f_client != NULL)
    {
        // On peut lire et �crire dans le fichier
        char buffer[200];
        // go to end file
        //fseek(f_client, 0, SEEK_END);
        while(fgets(buffer, 200, f_client)) {
            sscanf (buffer,"%d", &usr_id);
            if(usr_id>max_id)
                max_id = usr_id;
        }
        max_id++;
        //id prenom nom date-naissance
        fprintf(f_client, "%d %s %s %s\n", max_id, firstname, lastname, birthday);

        //fflush(f_client);
        //rewind(f_client);
        //fclose(f_client);
    }
    else
    {
        // On affiche un message d'erreur si on veut
        perror("ERROR : Can not access clients file!");
    }
    return max_id;
}

void save_card(FILE* f_card, char *card_infos, int client_id)
{
    //FILE* f_card = fopen("database/cards.dat", "r+");
    printf("***** Saving card infos \n");
    if (f_card != NULL)
    {

        char buffer[200];
        // go to end file
        while(fgets(buffer, 200, f_card)) {}

        double solde = 0.0; // Solde de depart
        //numero date-expiration code etat user_id solde
        fprintf(f_card, "%s %s %s %d %d %.2f\n", str_sub(card_infos, 0,15), str_sub(card_infos, 21, 26), str_sub(card_infos, 17, 19), -1, client_id, solde);

        fflush(f_card);
        //fclose(f_card);
    }
    else
    {
        perror("ERROR: Can not access cards file!");
    }
}

int activate_card(FILE* f_card, char *number, char *expir_date, char *code)
{
    printf("***** ACTIVATE STARTED *****\n");

    int response = 0;
    if (f_card != NULL)
    {
        char buffer[200];
        // go to end file
        int sz_line = 50;
        int line = 0;
        char **content;
        content = malloc(1*sizeof(*content));

        char *num_f = malloc(16*sizeof(char));
        char *date_f = malloc(16*sizeof(char));
        char *code_f = malloc(3*sizeof(char));
        int state;

        rewind(f_card);
        while(fgets(buffer, 200, f_card)) {
            int usrId;
            float solde;
            sscanf (buffer,"%s %s %s %d %d %f", num_f, date_f, code_f, &state, &usrId, &solde);
            //printf("Values line: %s %s %s %d %d %.2f\n", num_f, date_f, code_f, state, usrId, solde);
            if(line>0){
                content = realloc(content, (line+1)*sizeof(*content) );
            }
            content[line] = malloc(sz_line*sizeof(content[line]));
            if(content[line]==NULL){
                perror("ERROR: Can not allocate memory\n");
                exit(1);
            }

            if(atol(num_f)==atol(number) && atoi(date_f)==atoi(expir_date) && atoi(code_f)==atoi(code)){
                printf("found---\n");
                response = 1;
                //double val = (double)solde;
                sprintf(content[line], "%s %s %s %d %d %.2f\n", number, expir_date, code, 1, usrId, solde);
            }
            else{
                //content[line] = buffer;//1470001738016599
                sprintf(content[line], "%s", buffer);
            }
            printf("line boucle: %s",  content[line]);
            line ++;

        }

        if(response==1){// Modify file content
            rewind(f_card);
            int l;
            for(l=0; l<line-1; l++){
                //printf("l:%d cont:%s", l, content[l]);
                fprintf(f_card, "%s", content[l]);
            }
            //printf("l:%d cont: %s", l, content[l]);
            (content[l])[strlen(content[l])-1]= ' ';
            fprintf(f_card, "%s", content[l]);
            fflush(f_card);
        }
    }
    else
    {
        perror("ERROR: Can not access cards file!");
    }

    printf("***** ACTIVATE ENDED *****\n");

    return response; // Card unknown
}

int credit_card(FILE* f_card, char *number, char *expir_date, char *code, double amount)
{
    printf("***** CREDIT BALANCE STARTED *****\n");
    int response = 0;
    if (f_card != NULL)
    {
        char buffer[200];
        // go to end file
        int sz_line = 50;
        int line = 0;
        char **content;
        content = malloc(1*sizeof(*content));

        char *num_f = malloc(16*sizeof(char));
        char *date_f = malloc(16*sizeof(char));
        char *code_f = malloc(3*sizeof(char));
        int state;

        rewind(f_card);
        while(fgets(buffer, 200, f_card)) {
            int usrId;
            float solde;
            sscanf (buffer,"%s %s %s %d %d %f", num_f, date_f, code_f, &state, &usrId, &solde);
            //printf("Values line: %s %s %s %d %d %.2f\n", num_f, date_f, code_f, state, usrId, solde);
            if(line>0){
                content = realloc(content, (line+1)*sizeof(*content) );
            }
            content[line] = malloc(sz_line*sizeof(content[line]));
            if(content[line]==NULL){
                perror("ERROR: Can not allocate memory\n");
                exit(1);
            }

            if(atol(num_f)==atol(number) && atoi(date_f)==atoi(expir_date) && atoi(code_f)==atoi(code)){
                response = 1;
                amount += solde;
                sprintf(content[line], "%s %s %s %d %d %.2f\n", number, expir_date, code, 1, usrId, amount);
            }
            else{
                //content[line] = buffer;
                sprintf(content[line], "%s", buffer);
            }
            //printf("BUFF-line: %s\n", content[line]);
            line ++;

        }
        if(response==1){// Modify file content
            rewind(f_card);
            int l;
            for(l=0; l<line-1; l++){
               fprintf(f_card, "%s", content[l]);
            }
            (content[l])[strlen(content[l])-1]= ' ';
            fprintf(f_card, "%s", content[l]);
            fflush(f_card);
        }
    }
    else
    {
        perror("ERROR: Can not access cards file!");
    }

    printf("***** CREDIT BALANCE ENDED *****\n");
    return response; // Card unknown
}

int debit_card(FILE* f_card, char *number, char *expir_date, char *code, double amount)
{
    printf("***** DEBIT BALANCE STARTED *****\n");
    int response = 0;
    if (f_card != NULL)
    {
        char buffer[200];
        // go to end file
        int sz_line = 50;
        int line = 0;
        char **content;
        content = malloc(1*sizeof(*content));

        char *num_f = malloc(16*sizeof(char));
        char *date_f = malloc(16*sizeof(char));
        char *code_f = malloc(3*sizeof(char));
        int state;

        rewind(f_card);
        while(fgets(buffer, 200, f_card)) {
            int usrId;
            float solde;
            sscanf (buffer,"%s %s %s %d %d %f", num_f, date_f, code_f, &state, &usrId, &solde);
            //printf("Values line: %s %s %s %d %d %.2f\n", num_f, date_f, code_f, state, usrId, solde);
            if(line>0){
                content = realloc(content, (line+1)*sizeof(*content) );
            }
            content[line] = malloc(sz_line*sizeof(content[line]));
            if(content[line]==NULL){
                perror("ERROR: Can not allocate memory\n");
                exit(1);
            }
            if(atol(num_f)==atol(number) && atoi(date_f)==atoi(expir_date) && atoi(code_f)==atoi(code)){
                response = -1;
                if(solde>amount){
                    response = 1;
                    solde -= amount;
                    sprintf(content[line], "%s %s %s %d %d %.2f\n", number, expir_date, code, 1, usrId, solde);
                }
                else{
                    //content[line] = buffer;
                    sprintf(content[line], "%s", buffer);
                }
            }
            else{
                content[line] = buffer;
            }
            //printf("BUFF-line: %s\n", content[line]);
            line ++;

        }

        if(response==1){// Modify file content
            rewind(f_card);
            int l;
            for(l=0; l<line-1; l++){
               fprintf(f_card, "%s", content[l]);
            }
            (content[l])[strlen(content[l])-1]= ' ';
            fprintf(f_card, "%s", content[l]);

            fflush(f_card);
        }
    }
    else
    {
        perror("ERROR: Can not access cards file!");
    }

    printf("***** DEBIT BALANCE ENDED *****\n");
    return response; // Card unknown
}



char* list_transactions(FILE* f_transactions, char *number)
{
    printf("***** STATEMENTS STARTED *****\n");
    char *response;
    if (f_transactions != NULL)
    {
        char buffer[200];
        // go to end file

        char *num_f = malloc(16*sizeof(char));
        char *num_dest = malloc(16*sizeof(char));
        char *date_ = malloc(10*sizeof(char));
        char *_time = malloc(8*sizeof(char));
        char type;
        float amount, balance_after;

        rewind(f_transactions);
        char **list_trans;

        int max_trans = 100, sz_line=40, nb_trans=0;
        list_trans = calloc(max_trans, sizeof(*list_trans));

        while(fgets(buffer, 200, f_transactions)) {
            // 1470049930036599 2014-02-28 02:23:50 C  500.00
            sscanf (buffer,"%s %s %s %c %s %f", num_f, date_, _time, &type, num_dest, &amount);
        //printf("dest: %s\n", date_, _time, type, amount);

            if(atol(num_f)==atol(number)){
                if(type=='C')
                    printf("line: %s %s|CREDIT|%.2f\n", date_, _time, amount);
                else
                    printf("line: %s %s|DEBIT |%.2f\n", date_, _time, amount);

                list_trans[nb_trans] = calloc(sz_line, sizeof(list_trans[nb_trans]));
                sprintf(list_trans[nb_trans], " \t%s %s|%c|%s", date_, _time, type, num_dest);

                nb_trans++;
            }

        }

		response = calloc(sz_line*10, sizeof(char));
		int l, max_return=10, k=0;
		for(l=nb_trans-1; l>=0; l--){
            //k++;
            sprintf(response, "%s\n%s", response, list_trans[l]);
            printf("trans: %s\n", list_trans[l]);
		}

    }
    else
    {
        perror("ERROR: Can not access cards file!");
    }

    printf("resp: %s\n", response);
    printf("***** STATEMENTS ENDED *****\n");
    return response;
}

float get_balance(FILE* f_card, char *number, char *expir_date, char *code)
{
    printf("***** GET BALANCE STARTED *****\n");
    float response = 0.00;
    if (f_card != NULL)
    {
        char buffer[200];

        char *num_f = malloc(16*sizeof(char));
        char *date_f = malloc(16*sizeof(char));
        char *code_f = malloc(3*sizeof(char));
        int state;

        rewind(f_card);
        while(fgets(buffer, 200, f_card)) {
            int usrId;
            float solde;
            sscanf (buffer,"%s %s %s %d %d %f", num_f, date_f, code_f, &state, &usrId, &solde);

            if(atol(num_f)==atol(number) && atoi(date_f)==atoi(expir_date) && atoi(code_f)==atoi(code)){
                response = solde;
            }

        }

    }
    else
    {
        perror("ERROR: Can not access cards file!");
    }

    printf("resp: %.2f\n", response);
    printf("***** GET BALANCE ENDED *****\n");
    return response;
}

void save_transaction(FILE* f_transactions, char *type, char *number, char *num_from_dest, double amount)
{
    if (f_transactions != NULL)
    {
        //printf("infos to save:%s\n", card_infos);
        char buffer[200];
        // go to end file
        while(fgets(buffer, 200, f_transactions)) {}
        //numero date_heure type num from/dest amount
        fprintf(f_transactions, "%s %s %s %s %.2f\n", number, get_date_time(), type, num_from_dest, amount);

        fflush(f_transactions);
        printf("New transaction saved< CARD:%s DATE:%s TYPE:%s CARD-2%s AMOUNT:%.2f>\n", number, get_date_time(), type, num_from_dest, amount);
    }
    else
    {
        perror("ERROR: Can not access transactions file!");
    }
}

void logs(FILE* f_logs, char *type, char *adress, char *msg)
{
    if (f_logs != NULL)
    {
        //printf("infos to save:%s\n", card_infos);
        char buffer[200];
        // go to end file
        while(fgets(buffer, 200, f_logs)) {}
        //date-heure tye=req/resp adress, msg
        fprintf(f_logs, "%s %s %s %s\n", get_date_time(), type, adress, msg);
        fflush(f_logs);
        printf("New Log< DATE:%s TYPE:%s Address%s MSG:%s>\n", get_date_time(), type, adress, msg);
    }
    else
    {
        perror("ERROR: Can not access logs file!");
    }
}


int check_card(FILE* f_card, char *number, char *expir_date, char *code)
{
    printf("***** CHECK STARTED *****\n");
    printf("card to act: %s-%s-%s", number,expir_date, code);
    int response = check_card_number(number, expir_date, code);
    if (f_card != NULL)
    {
        char buffer[200];

        char *num_f = malloc(16*sizeof(char));
        char *date_f = malloc(16*sizeof(char));
        char *code_f = malloc(3*sizeof(char));
        int state;

        rewind(f_card);
        while(fgets(buffer, 200, f_card)) {
            int usrId;
            float solde;
            sscanf (buffer,"%s %s %s %d %d %f", num_f, date_f, code_f, &state, &usrId, &solde);

            if(atol(num_f)==atol(number) && atoi(date_f)==atoi(expir_date) && atoi(code_f)==atoi(code)){
                response = state;
            }
        }
    }
    else
    {
        perror("ERROR: Can not access cards file!");
    }

    printf("***** CHECK ENDED *****\n");

    return response; // Card unknown
}
