#include "../headers/cards.h"
#include <stdlib.h>
#include <stdio.h>

#define SIZE_NAME 150
#define EFFECT_DESCRIPTION 1000


struct impl_card{
    char c_name[SIZE_NAME]; // Name of the card
    cardName c_enumName; //Attribute that allows to find the card by an identifier (useful for the program that codes the effects of the cards, a switch...case will be used)
    char c_effectDescription[EFFECT_DESCRIPTION]; // Description of the card's effect
    int numberOfOccurrence; // Number of occurence of the card in the deck
    int cardStatus; // 0 if Face Down, 1 if Face Up
};

card const_card(char* nameCard, char* descCard, cardName enumCard, int numberOccurence){
    card c = malloc(sizeof(struct impl_card));
    setCardName(c, nameCard);
    setCardEffectDescription(c, descCard);
    setNumberOfOccurrence(c, numberOccurence);
    setCardStatus(c, 0);
    c->c_enumName = enumCard;
    return c;
}

card copy(card c1) {
    card c2 = malloc(sizeof(struct impl_card));
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
    int i = 0;
    while (i < SIZE_NAME - 1 && name[i] != '\0')
    {

        card->c_name[i] = name[i];
        i++;
    }
    card->c_name[i] = '\0';
}

void setCardEffectDescription(card card, char* effectDescription){
    int i = 0;
    while (i < EFFECT_DESCRIPTION - 1 && effectDescription[i] != '\0')
    {

        card->c_effectDescription[i] = effectDescription[i];
        i++;
    }
    card->c_effectDescription[i] = '\0';
}

void setNumberOfOccurrence(card card, int numberOfOccurrence){
    card->numberOfOccurrence=numberOfOccurrence;
}

void setCardStatus(card card, int status){
    card->cardStatus=status; 
}

/*--------------------------------------------------------------------------*/
