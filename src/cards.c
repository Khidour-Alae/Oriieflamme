#include "../headers/cards.h"
#include <stdlib.h>
#include <stdio.h>

#define SIZE_NAME 150
#define EFFECT_DESCRIPTION 1000

typedef enum cardName cardName;
enum cardName {
    Julien_Forest,
    Alae_Khigour
}


struct impl_card{
    char name[SIZE_NAME]; 
    cardName cnale;
    char effectDescription[EFFECT_DESCRIPTION];
    int numberOfOccurrence;
    int cardStatus; // 0 if Face Down, 1 if Face Up
}

