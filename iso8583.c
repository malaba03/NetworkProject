#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"

#define A 1;
#define B 2;
#define AN 3;
#define N 4;
#define ANS 5;
#define Z 6;
#define NB_BITS 128;
#define SZ_MAX_ELM 99;

int [][] gb_data_element ={{B, 64, 0},       //bon
                        {AN, 19, 1},      //bon
                        {N, 6, 0},        //bon
                        {N, 12, 0},       //bon
                        {N, 12, 0},       //bon
                        {N, 12, 0},       //bon
                        {AN, 10, 0},      //bon
                        {N, 8, 0},
                        {N, 8, 0},
                        {N, 8, 0},
                        {N, 6, 0},        //bon
                        {N, 6, 0},
                        {N, 4, 0},
                        {N, 4, 0},        //bon
                        {N, 4, 0},
                        {N, 4, 0},
                        {N, 4, 0},
                        {N, 4, 0},
                        {N, 3, 0},
                        {N, 3, 0},
                        {N, 3, 0},
                        {N, 3, 0},
                        {N, 3, 0},        //bon
                        {N, 3, 0},
                        {N, 2, 0},
                        {N, 2, 0},
                        {N, 1, 0},
                        {N, 8, 0},
                        {AN, 9, 0},
                        {N, 8, 0},
                        {AN, 9, 0},
                        {N, 11, 1},
                        {N, 11, 1},
                        {AN, 28, 1},
                        {Z, 37, 1},
                        {N, 104, 1},
                        {AN, 12, 0},
                        {AN, 6, 0},
                        {AN, 3, 0},
                        {AN, 3, 0},
                        {ANS, 8, 0},
                        {ANS, 19, 0},
                        {ANS, 40, 0},
                        {AN, 19, 1},
                        {AN, 76, 1},
                        {AN, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {AN, 3, 0},
                        {AN, 3, 0},
                        {A, 3, 0},
                        {AN, 4, 0},
                        {AN, 18, 0},
                        {AN, 120, 0},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 99, 1},
                        {ANS, 60, 1},
                        {ANS, 99, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {B, 16, 0},
                        {B, 16, 0},
                        {N, 1, 0},
                        {N, 2, 0},
                        {N, 3, 0},
                        {N, 3, 0},
                        {N, 3, 0},
                        {N, 4, 0},
                        {ANS, 999, 1},
                        {N, 6, 0},
                        {N, 10, 0},
                        {N, 10, 0},
                        {N, 10, 0},
                        {N, 10, 0},
                        {N, 10, 0},
                        {N, 10, 0},
                        {N, 10, 0},
                        {N, 10, 0},
                        {N, 12, 0},
                        {N, 12, 0},
                        {N, 12, 0},
                        {N, 12, 0},
                        {N, 15, 0},
                        {AN, 16, 0},
                        {N, 16, 0},
                        {N, 16, 0},
                        {AN, 42, 0},
                        {AN, 1, 0},
                        {N, 2, 0},
                        {N, 5, 0},
                        {AN, 7, 0},
                        {AN, 42, 0},
                        {AN, 8, 0},
                        {AN, 17, 0},
                        {ANS, 25, 0},
                        {N, 11, 1},
                        {N, 11, 1},
                        {ANS, 17, 0},
                        {ANS, 28, 1},
                        {ANS, 28, 1},
                        {AN, 99, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {N, 11, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 999, 1},
                        {ANS, 255, 1},
                        {ANS, 50, 1},
                        {ANS, 6, 1},
                        {ANS, 999, 1},
                        {B, 16, 0}};

char **gb_data;
int i;
gb_data = malloc(NB_BITS*sizeof(*gb_data));
if(gb_data==NULL){
    perror("Erreur allocation impossible\n");
    exit(1);
}
for(i=0 ; i < nb_elm ; i++){
   gb_data[i] = malloc(SZ_MAX_ELM * sizeof(gb_data[i]));
   if(gb_data[i]==NULL){
        perror("Erreur allocation de %d impossible\n");
        exit(1);
    }
}

char *gb_bitmap;
int gb_mti;
char *gb_iso;
char *gb_valid;

void add_mti(char *mti)
{
    if(strlen(mti)==4){
        gb_mti = atoi(mti);
    }
}

void add_data(int bit, char *data)
{
        if (bit>1 && bit<129) {
            gd_data[bit]	= pack_element(gb_data_element[bit], data);
            //MLB ksort($this->_data);
            $this->_calculateBitmap();
            gb_bitmap = calculate_bitmap(gb_data, NB_BITS);
        }
}

//return data element in correct format
private function _packElement($data_element, $data) {
    $result	= "";

    //numeric value
    if ($data_element[0]=='n' && is_numeric($data) && strlen($data)<=$data_element[1]) {
        $data	= str_replace(".", "", $data);

        //fix length
        if ($data_element[2]==0) {
            $result	= sprintf("%0". $data_element[1] ."s", $data);
        }
        //dinamic length
        else {
            if (strlen($data) <= $data_element[1]) {
                $result	= sprintf("%0". strlen($data_element[1])."d", strlen($data)). $data;
            }
        }
    }

    //alpha value
    if (($data_element[0]=='a' && ctype_alpha($data) && strlen($data)<=$data_element[1]) ||
        ($data_element[0]=='an' && ctype_alnum($data) && strlen($data)<=$data_element[1]) ||
        ($data_element[0]=='z' && strlen($data)<=$data_element[1]) ||
        ($data_element[0]=='ans' && strlen($data)<=$data_element[1])) {

        //fix length
        if ($data_element[2]==0) {
            $result	= sprintf("% ". $data_element[1] ."s", $data);
        }
        //dinamic length
        else {
            if (strlen($data) <= $data_element[1]) {
                $result	= sprintf("%0". strlen($data_element[1])."s", strlen($data)). $data;
            }
        }
    }

    //bit value
    if ($data_element[0]=='b' && strlen($data)<=$data_element[1]) {
        //fix length
        if ($data_element[2]==0) {
            $tmp	= sprintf("%0". $data_element[1] ."d", $data);

            while ($tmp!='') {
                $result	.= base_convert(substr($tmp, 0, 4), 2, 16);
                $tmp	= substr($tmp, 4, strlen($tmp)-4);
            }
        }
    }

    return $result;
}
