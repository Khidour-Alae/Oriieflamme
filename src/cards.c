#include "../headers/cards.h"
#include <stdlib.h>
#include <stdio.h>

#define SIZE_NAME 150
#define EFFECT_DESCRIPTION 1000

typedef enum cardName cardName;
enum cardName {
    Julien_Forest,
    Alae_Khigour
};


struct impl_card{
    char c_name[SIZE_NAME]; 
    cardName c_enumName;
    char c_effectDescription[EFFECT_DESCRIPTION];
    int numberOfOccurrence;
    int cardStatus; // 0 if Face Down, 1 if Face Up
};

char getCardName(card card){
    return card->c_name;
}

char getCardEffectDescription(card card){
    return card->c_effectDescription;
}

int getNumberOfOccurrence(card card){
    return card->numberOfOccurrence;
}

int getCardStatus(card card){
    return card->cardStatus;
}
