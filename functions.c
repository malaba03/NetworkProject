#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

long binary_to_decimal(char *str_binary)
{
    int i, sz, idx=0;
    long decimal = 0;
    char a_car[1];
    for(sz=0; str_binary[sz]!='\0'; ++sz);

    for(i=sz-1; i>=0; i--){
        a_car[0] = str_binary[i];
        decimal += atoi(a_car)*pow(2, idx);
        idx++;
    }
    return decimal;
}

long hexa_to_decimal(char *str_hexa)
{
    int i, sz, idx=0, val_hex;
    long decimal = 0;
    char a_car[1];
    for(sz=0; str_hexa[sz]!='\0'; ++sz);

    for(i=sz-1; i>=0; i--){
        a_car[0] = str_hexa[i];
        val_hex = atoi(a_car);
        if(val_hex==0 && a_car!="0")
        {
            //a_car = toupper(a_car);
            if(a_car=="A") val_hex = 10;
            else if(a_car=="B") val_hex = 11;
            else if(a_car=="C") val_hex = 12;
            else if(a_car=="D") val_hex = 13;
            else if(a_car=="E") val_hex = 14;
            else if(a_car=="F") val_hex = 15;
        }
        decimal += val_hex*pow(16, idx);
        idx++;
    }
    //printf("dec:%ld \n", decimal);
    return decimal;
}

char *base_convert(char *str, int b_from, int b_to)
{
    long decimal;
    char *result;
    if(b_from==2){
        decimal = binary_to_decimal(str);
    }
    else if(b_from==10){
        decimal = atol(str);
    }
    else if(b_from==16){
        decimal = hexa_to_decimal(str);
    }
    int str_sz, rs_sz;
    for(str_sz=0; str[str_sz]!='\0'; ++str_sz);

    if(b_to==2){
        rs_sz = 4*str_sz;
    }
    else if(b_to==16){
        while(str_sz%4!=0){
            str_sz++;
        }
        rs_sz = str_sz/4;
    }

    result = malloc(rs_sz*sizeof(char));

    ltoa(decimal, result, b_to);

    int idx_dec;
    for(idx_dec=0; result[idx_dec]!='\0'; ++idx_dec){
        result[idx_dec]=(char)toupper((int)result[idx_dec]);
    }
    //printf("=>%s\n", result);

    return result;
}

char* calculate_bitmap(char *gb_data[], int sz_data) {
    int i,sz_btm = sz_data/2;
    char *tmp = malloc(sz_btm*sizeof(int));
    char *tmp2 = malloc(sz_btm*sizeof(int));
    //tmp	= sprintf("%064d", 0); tmp2	= sprintf("%064d", 0);
    //sprintf(tmp, "%064d", 0);
    //sprintf(tmp2, "%064d", 0);
     for(i=0; i<sz_btm; i++){
        tmp[i] = '0';
        tmp2[i] = '0';
    }

    int key, idx_tmp = sz_btm+1;

    for(key=1; key<sz_data; key++) {
        char *val = gb_data[key];
        //printf("tab:%d contenu:%s  val:%s\n", key, gb_data[key], val);
        if (key<sz_btm && val[0]!=' '){
            tmp[key]	= '1';
        }
        else if (val[0]!=' '){
            tmp[0]	= '1';
            tmp2[key-sz_btm]	= '1';
        }
    }
    //printf("temp:%s\n", tmp);  printf("temp2:%s\n", tmp2);
    char *master;
    int sz_master = sz_btm/4;
    if(tmp[0]=='1') {
        //printf("ya slave\n");
        master = malloc(2*sz_master*sizeof(char));
        sprintf(master, "%s%s", base_convert(tmp, 2, 16), base_convert(tmp2, 2, 16));
    }
    else{
        //printf("ya pas slave\n");
        master = malloc(sz_master*sizeof(char));
        master = base_convert(tmp, 2, 16);
    }
    //printf("master:%s\n", master);
    return master;
}
