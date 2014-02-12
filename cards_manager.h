#ifndef CARDS_MANAGER_H_INCLUDED
#define CARDS_MANAGER_H_INCLUDED

int is_prime(int number);
int pow_modulo(unsigned int a, unsigned int b, unsigned int p);
int pow_modulo_long(unsigned long a, unsigned int b, unsigned int p);
int pow_modulo_long_long(long long a, unsigned int b, unsigned int p);
long long transforme_m_to_ASCII(char *msg);
int pgcd(int a, int b);
int phi(int n);
int* msg_signature(char *msg, int *key);
char* get_expir_date(int nb_year);
void generate_card(void);
int check_card_number(char *number, char *expir_date, char *code);
#endif // CARDS_MANAGER_H_INCLUDED
