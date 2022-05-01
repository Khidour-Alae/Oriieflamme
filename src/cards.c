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

void const_card(card c, char* nameCard, char* descCard, cardName enumCard, int numberOccurence){
    setCardName(c, nameCard);
    setCardEffectDescription(c, descCard);
    setNumberOfOccurrence(c, numberOccurence);
    setCardStatus(c, 0);
    c->c_enumName = enumCard;
}

card copy(card c1) {
    card c2;
    setCardName(c2, c1->c_name);
    setCardEffectDescription(c2, c1->c_effectDescription);
    setNumberOfOccurrence(c2, c1->numberOfOccurrence);
    setCardStatus(c2, c1->cardStatus);
    c2->c_enumName = c1->c_enumName;
    return c2;
}
/*--------------------------------------------------------------------------*/

//GETTERS

char* getCardName(card card){
    return card->c_name;
}

cardName getCardEnumName(card card) {
    return card->c_enumName;
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
