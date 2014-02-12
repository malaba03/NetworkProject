#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "cards_manager.h"
#include "functions.h"


int main()
{
    int nb_elm = 32, sz_elm =4;
    int i;
    char **gb_data;
    gb_data = malloc(nb_elm*sizeof(*gb_data));
    if(gb_data==NULL){
        perror("Erreur allocation impossible\n");
        exit(1);
    }
    for(i=0 ; i < nb_elm ; i++){
       gb_data[i] = malloc(sz_elm * sizeof(gb_data[i]));
       if(gb_data[i]==NULL){
            perror("Erreur allocation de %d impossible\n");
            exit(1);
        }
    }

    gb_data[0] = "0200";
    //printf("test:%s\n", gb_data[0]);

    for(i=1; i<nb_elm; i++){
        if(i<nb_elm/2){
            gb_data[i] = "1234";
        }
        else{
            gb_data[i] = "    ";
        }

        if(i==27){
            gb_data[i] = "1483";
        }

    }
    /*for(i=0; i<nb_elm; i++){
        printf("tab:%d contenu:%s\n", i, gb_data[i]);
    }*/

    calculate_bitmap(gb_data, nb_elm);
    //printf("test:%s\n", base_convert("11100100110010001011", 2, 16));
    return 0;
}
