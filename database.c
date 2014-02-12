#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int id;
    char *firstname;
    char *lastname;
    char *birthday;
} client;

void createClient(char *firstname, char *lastname, char *birthday)
{
    FILE* f_client = fopen("database/clients.dat", "r+");

    if (f_client != NULL)
    {
        // On peut lire et écrire dans le fichier
        printf("Ok 6");

        client new_client;
        new_client.firstname = firstname;
        new_client.lastname = lastname;
        new_client.birthday = birthday;

        char buffer[200];
        // go to end file
        //fseek(f_client, 0, SEEK_END);
        int nb_clients = 1;
        while(fgets(buffer, 200, f_client)) {
            nb_clients ++;
        }
        new_client.id = nb_clients;
        fprintf(f_client, "%d %s %s %s \n", new_client.id, new_client.firstname, new_client.lastname, new_client.birthday);

        fflush(f_client);
        //rewind(f_client);
        //fclose(f_client);
    }
    else
    {
        // On affiche un message d'erreur si on veut
        printf("Impossible d'ouvrir le fichier des clients");
    }
}

void save_card(char* number, unsigned int date_expir, unsigned int code)
{
    FILE* f_card = fopen("database/cards.dat", "r+");

    if (f_card != NULL)
    {
       char buffer[200];
        // go to end file
        while(fgets(buffer, 200, f_card)) {}

        int solde = 0.0; // Solde de depart
        fprintf(f_card, "%s %d %d %.2f \n", number, date_expir, code, solde);

        fflush(f_card);
        fclose(f_card);
    }
    else
    {
        printf("Impossible d'ouvrir le fichier des clients");
    }
}

int is_primary(int number){
    int i;
    for(i=2; i<number; i++){
        if(number%i == 0) return 0;
    }
    return 1;
}

int pow_modulo(unsigned int a, unsigned int b, unsigned int p){
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

int pow_modulo_long(unsigned long a, unsigned int b, unsigned int p){
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

int pow_modulo_long_long(long long a, unsigned int b, unsigned int p){
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


long long transforme_m_to_ASCII(char *msg)
{
    long long msg_ascii;
    int i;

    for(i=0; i<6; ++i){
        char c = msg[i];
        int ascii_i = (int)c;
        if(i==0)
        {
            msg_ascii = (long long)ascii_i;
        }
        else
        {
            msg_ascii = 100*msg_ascii + ascii_i;
        }
    }

    return msg_ascii;
}

int pgcd(int a, int b){
    if(b==0)return a;
// D'aprés Euclide, pgcd(a, b) = pgcd(b, a%b) si a > b.
    return pgcd(b,a%b);
}

int phi(int n){
    int fi = 0;	// Déclare une entier qui va contenir la valeur de phi et l'initialise à 0.
    /*
     * Le phi d'un nombre est déterminé par le nombre de nombres premiers avec lui et
     * inférieur à lui
     */
     int i;
     for(i=1; i < n; i++){
         if(pgcd(i,n) == 1)	fi +=1; // A chaque fois que le pcgd est 1, on incrémente l
     }
    return fi; // Retourne la valeur de phi.
}

int* msg_signature(char *msg, int *key)
{
    int a = key[0];
    int g = key[1];
    int p = key[2];

    //Calcule de H(m) = m_to_ascii*m_to_ascii%p
    long long m_to_ascii = transforme_m_to_ASCII(msg);
    int h_m = pow_modulo_long_long(m_to_ascii, 2, p);
    //printf("h_m: %d \n", h_m);
    srand((unsigned)time(NULL));
    int k = rand()%(p-1);
    // Génére et vérifie jusqu'à avoir k entre 1 et p-1.
    while(pgcd(k, p-1)!=1 || k <= 1){
        k = rand()%(p-1);
    }

    int r = pow_modulo_long((long)g, k, p);
    //printf("r: %d\n", r);

    // Calcule l'inverse de k
    int fi = phi(p-1);						// Calcule phi(p-1)
    //printf("fi: %d\n", fi);

    int inv_k = pow_modulo((long)k, fi-1,p-1);	// calcule l'inverse de k.
    //printf("inv_k: %d\n", inv_k);

    // Calcule la valeur de s
    long s = a*r%(p-1);
    s = h_m - s; //(inv_k*(h_m-a*r)%(p-1) + (p-1))%(p-1)
    s = inv_k*s%(p-1);
   // s = s%(p-1);
    s = s +(p-1);	// On y ajoute p-1 au cas où s serait négatif.
    s = s%(p-1);
    int *signature=(int*)malloc(2*sizeof(int));
    signature[0] = (int)s;
    signature[1] = r;
    //printf("s: %ld\n", s);
    return signature;
}

char* get_expir_date()
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

    int month_expir = (p_datetime->tm_mon+1)%12;
    int date_expir_int = month_expir*10000 + (1900+p_datetime->tm_year+1);

    char *date_expir = malloc(6*sizeof(char));
    sprintf(date_expir, "%06d", date_expir_int);

    return date_expir;
}
void generate_card(){
    int bin = 1470;
    int a = bin;

    // generation des cles
   /* int p = 0;
    do{
        srand((unsigned)time(NULL));
        p = rand()%9999;
    }
    while(p<=a || !is_primary(p));*/
    int p = 6599; // Fixation de la valeur de p
    int q = (p-1)/2;

    // Creation du gemerateur
    int g = 0;
    do{
        srand((unsigned)time(NULL));
        g = rand()%999;
    }
    while(p<=g || g<=0 || pow_modulo_long((long)g, 2, p)==1 || pow_modulo_long((long)g, q, p)==1);

    int g_pow_a = pow_modulo_long((long)g, a, p);

    int *key = malloc(3*sizeof(int));
    key[0] = a;
    key[1] = g;
    key[2] = p;

    char *expir_date = get_expir_date();
    int *signature = (int*)malloc(2*sizeof(int));
    signature = msg_signature(expir_date, key);

    printf("card_number: %d %8.4d %8.4d %d code:%d date: %s\n", a, signature[0], signature[1], p, g, expir_date);
}
int main(int argc, char *argv[])
{
    //printf("ascii: %d\n", pow_modulo_long(99999999, 2, 1000));
    generate_card();
    //transforme_to_ASCII2("022015", 6599);
    //char a = '9';
   // int c = atoi(a);
   // printf("code:%d \n", (int) 9);
    //save_card("1111222233334444", 1015, 469);
    return 0;
}
