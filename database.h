#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

void save_card(FILE* f_card, char* card_infos, int client_id);
int save_client(FILE* f_client, char *firstname, char *lastname, char *birthday);
int activate_card(FILE* f_card, char *number, char *expir_date, char *code);
int debit_card(FILE* f_card, char *number, char *expir_date, char *code, double amount);
int credit_card(FILE* f_card, char *number, char *expir_date, char *code, double amount);
void save_transaction(FILE* f_transactions, char *type, char *number, char *num_from_dest, double amount);
void logs(FILE* f_ogs, char *type, char *adress, char *request);
char* list_transactions(FILE* f_transactions, char *number);
float get_balance(FILE* f_card, char *number, char *expir_date, char *code);
//void logs(FILE* f_ogs, char *adress, char *request, char *date);
#endif // DATABASE_H_INCLUDED
