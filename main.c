#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
//#include "cards_manager.h"
//#include "database.h"
#include "functions.h"
//#include "elgamal.h"

void ma_f(char **chaine)
{
    printf("before= %s\n", *chaine);
    *chaine= "malaba";
    printf("after= %s\n", *chaine);
}

int main()
{
    /*FILE* f_card = fopen("database/cards.dat", "r+");
    credit_card(f_card, "1470338160926599", "022015", "078", (float)1500);
    //fseek(f_card, 0, SEEK_SET);
    debit_card(f_card, "1470338160926599", "022015", "078", (float)1800);
    //fseek(f_card, 0, SEEK_SET);
    fclose(f_card);*/
    //printf("now: %s\n", get_date_time());
    //FILE* f_transactions = fopen("database/transactions.dat", "r+");
    //save_transaction(f_transactions, "C", "1470338160926599", "", 2000);
    /*char *ch = malloc(6*sizeof(char));
    ch = "lamine";
    ma_f(&ch);*/
   // printf("end= %s\n", transforme_str_to_ASCII("MALABA"));
    /*char **blocs_msg;
    cut_msg2("776576656665", 5, &blocs_msg);
    printf("end= %s\n", blocs_msg[0]);*/

    encrypt_msg("100BANK >> Please choose an operation to perform:\n    1:Activate card\n    2:Debit card\n    3:Credit card\n    4:See you balance\n    5:See transactions list\n");
    //decrypt_msg("010011010100000101001100010000010100001001000001");
    //decimal_to_binary(77);
    return 0;
}
