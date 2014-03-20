#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include "functions.h"


int is_prime(int number)
{
    int i;
    for(i=2; i<number; i++){
        if(number%i == 0) return 0;
    }
    return 1;
}

int pow_modulo(unsigned int a, unsigned int b, unsigned int p)
{
   if (b==0) return 1;
    if (b==1) return a%p;

    if ( (b & 0x01 ) == 0){
        int c = pow_modulo(a, b >> 1, p);
        return (c * c)%p;
    }
    else{
        return (a * pow_modulo(a, b - 1, p))%p;
    }
}

int pow_modulo_long(unsigned long a, unsigned int b, unsigned int p)
{
    if (b==0) return 1;
    if (b==1) return a%p;

    if ( (b & 0x01 ) == 0){
        int c = pow_modulo_long(a, b >> 1, p);
        return (c * c)%p;
    }
    else{
        return (a * pow_modulo_long(a, b - 1, p))%p;
    }
}

int pow_long_long_modulo(unsigned long a, unsigned long b, unsigned int p)
{
    if (b==0) return 1;
    if (b==1) return a%p;

    if ( (b & 0x01 ) == 0){
        int c = pow_long_long_modulo(a, b >> 1, p);
        return (c * c)%p;
    }
    else{
        return (a * pow_long_long_modulo(a, b - 1, p))%p;
    }
}

int pow_modulo_long_long(long long a, unsigned int b, unsigned int p)
{
    if (b==0) return 1;
    if (b==1) return a%p;

    if ( (b & 0x01 ) == 0){
        int c = pow_modulo_long_long(a, b >> 1, p);
        return (c * c)%p;
    }
    else{
        return (a * pow_modulo_long_long(a, b - 1, p))%p;
    }
}

char *str_sub (const char *s, unsigned int start, unsigned int last)
{
   char *new_s = NULL;

   if (s != NULL && start < last)
   {
/* (1)*/
      new_s = malloc (sizeof (*new_s) * (last - start + 2));
      if (new_s != NULL)
      {
         int i;

/* (2) */
         for (i = start; i <= last; i++)
         {
/* (3) */
            new_s[i-start] = s[i];
         }
         new_s[i-start] = '\0';
      }
      else
      {
         fprintf (stderr, "Memoire insuffisante\n");
         exit (EXIT_FAILURE);
      }
   }
   return new_s;
}

char* get_date_time()
{
    struct tm
    {
        int tm_sec;       /* secondes (0,59) */
        int tm_min;       /* minutes (0,59) */
        int tm_hour;      /* heures depuis minuit (0,23) */
        int tm_mday;      /* jour du mois (0,31) */
        int tm_mon;       /* mois depuis janvier (0,11) */
        int tm_year;      /* années écoulées depuis 1900 */
        int tm_wday;      /* jour depuis dimanche (0,6) */
        int tm_tm_yday;   /* jour depuis le 1er janvier (0,365) */
        int tm_isdst;
    };

    time_t intps;
    struct tm *p_datetime;
    intps = time(NULL);
    p_datetime = (struct tm*)localtime(&intps);

    int year = 1900+p_datetime->tm_year;
    //yyyy-mm-dd hh:mm:ss
    char *date_time = malloc(19*sizeof(char));
    sprintf(date_time, "%d-%02d-%02d %02d:%02d:%02d", year, p_datetime->tm_mon+1, p_datetime->tm_mday, p_datetime->tm_hour,p_datetime->tm_min, p_datetime->tm_sec);

    return date_time;
}

int binary_to_decimal(char *str_binary)
{
    int i, idx=0;
    int decimal = 0;
    char a_car[1];

    for(i=strlen(str_binary)-1; i>=0; i--){
        a_car[0] = str_binary[i];
        decimal += atoi(a_car)*pow(2, idx);
        idx++;
    }
    return decimal;
}

char* decimal_to_binary(int decimal)
{

    char *str_binary = malloc(8*sizeof(char));
    if(str_binary==NULL){
        perror("Can not allocate memory"); exit(1);
    }
    sprintf(str_binary, "%08d", 0);
    int idx_bin = 7;

    while(decimal!=0){
        if(decimal%2 != 0)
            str_binary[idx_bin] = '1';

        decimal = (int)decimal/2;
        idx_bin--;
    }
    //printf("fin str_binary=%s\n", str_binary);
    //free(str_binary);
    return str_binary;
}

char* encrypt_msg(char *msg)
{
    char *msg_ascii;
    msg_ascii = calloc((strlen(msg)*8), sizeof(char));

    int i;
    for(i=0; i<strlen(msg); ++i){
        char c = msg[i];
        int ascii_i = (int)c;

        sprintf(msg_ascii, "%s%s", msg_ascii, decimal_to_binary(ascii_i));
    }
    //printf("taille:%d -- MSG:%s\n", strlen(msg_ascii), msg_ascii);

    return msg_ascii;
}

char* decrypt_msg(char *msg)
{
    char *msg_clear;
    msg_clear = calloc((strlen(msg)/8), sizeof(char));

    int i=0;
    while(i<strlen(msg)){
        char c = (char)(binary_to_decimal(str_sub(msg, i, i+7)));

        i += 8;
        sprintf(msg_clear, "%s%c", msg_clear, c);
    }
    //printf("msg_clear:%d -- MSG:%s\n", strlen(msg_clear), msg_clear);

    return msg_clear;
}


int my_read_socket(int my_socket, char **buffer, int buf_sz)
{
    char *msg_dec = calloc(1500, sizeof(char));
    int resp = read(my_socket, msg_dec, 1500);

    *buffer = calloc(buf_sz, sizeof(char));
    sprintf(*buffer, "%s", decrypt_msg(msg_dec));
    free(msg_dec);
    // Treatment after read => decrypt
    // *buffer =
    return resp;
}

int my_write_socket(int my_socket, char *buffer, int buf_sz)
{
    // Treatment before write => encrypt
    // buffer=
    //write(my_socket, buffer, buf_sz);
    write(my_socket, encrypt_msg(buffer), 1500);
}
