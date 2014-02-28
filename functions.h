#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

char *base_convert(char *str, int b_from, int b_to);

char* calculate_bitmap(char *gb_data[], int MAX_BIT);
char *str_sub (const char *s, unsigned int start, unsigned int last);
char* get_date_time();
int my_read_socket(int my_socket, char **buffer, int buf_sz);
int my_write_socket(int my_socket, char *buffer, int buf_sz);
#endif // FUNCTIONS_H_INCLUDED
