#ifndef CARDS_MANAGER_H_INCLUDED
#define CARDS_MANAGER_H_INCLUDED

long long transforme_m_to_ASCII(char *msg);
int pgcd(int a, int b);
int phi(int n);
int* msg_signature(char *msg, int *key);
char* get_expir_date(int nb_year);
char* generate_card(void);
int check_card_number(char *number, char *expir_date, char *code);
#endif // CARDS_MANAGER_H_INCLUDED
