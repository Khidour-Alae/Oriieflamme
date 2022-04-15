    #include "../headers/cards.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE_NAME 150
#define EFFECT_DESCRIPTION 1000


struct impl_card{
    char c_name[SIZE_NAME]; // Name of the card
    cardName c_enumName; //Attribute that allows to find the card by an identifier (useful for the program that codes the effects of the cards, a switch...case will be used)
    char c_effectDescription[EFFECT_DESCRIPTION]; // Description of the card's effect
    int numberOfOccurrence; // Number of occurence of the card in the deck
    int cardStatus; // 0 if Face Down, 1 if Face Up
};

card const_card(cardName cName, int numberOccurence, int cardStatus){
    card->c_enumName = cName;
    
}

/*--------------------------------------------------------------------------*/

//GETTERS

char* getCardName(card card){
    return card->c_name;
}

char* getCardEffectDescription(card card){
    return card->c_effectDescription;
}

int getNumberOfOccurrence(card card){
    return card->numberOfOccurrence;
}

int getCardStatus(card card){
    return card->cardStatus;
}

/*--------------------------------------------------------------------------*/

//SETTERS

void setCardName(card card, char* name){
    if (strlen(name) < strlen(card->c_name)){    
        strcpy(card->c_name, name);
    }
}

void setCardEffectDescription(card card, char* effectDescription){
    if (strlen(effectDescription) < strlen(card->c_effectDescription)){    
        strcpy(card->c_effectDescription, effectDescription);
    }
}

void setNumberOfOccurrence(card card, int numberOfOccurrence){
    card->numberOfOccurrence=numberOfOccurrence;
}

void setCardStatus(card card, int status){
    card->cardStatus=status; 
}

/*--------------------------------------------------------------------------*/
