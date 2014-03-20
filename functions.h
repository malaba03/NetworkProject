#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

int is_prime(int number);
int pow_modulo(unsigned int a, unsigned int b, unsigned int p);
int pow_modulo_long(unsigned long a, unsigned int b, unsigned int p);
int pow_modulo_long_long(long long a, unsigned int b, unsigned int p);

char *base_convert(char *str, int b_from, int b_to);

char* calculate_bitmap(char *gb_data[], int MAX_BIT);
char *str_sub (const char *s, unsigned int start, unsigned int last);
char* get_date_time();
int my_read_socket(int my_socket, char **buffer, int buf_sz);
int my_write_socket(int my_socket, char *buffer, int buf_sz);
#endif // FUNCTIONS_H_INCLUDED
