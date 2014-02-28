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

int my_read_socket(int my_socket, char **buffer, int buf_sz)
{
    int resp = read(my_socket, *buffer, buf_sz);
    // Treatment after read => decrypt
    // *buffer =
    return resp;
}

int my_write_socket(int my_socket, char *buffer, int buf_sz)
{
    // Treatment before write => encrypt
    // buffer=
    write(my_socket, buffer, buf_sz);
}
