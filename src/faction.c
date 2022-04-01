#include "../headers/faction.h"
#include <stdlib.h>
#include <stdio.h>

#define SIZE_NAME 150

struct impl_faction{
    char f_name[SIZE_NAME];
    int f_ddrsPoints;
    hand f_hand;
    deck f_deck;
    int hasBeenReshuffled; // 1 if the deck has been reshuffled, 0 if not
    int nbRoundWin;
};

int hasTheDeckBeenShuffled(faction faction){
    return faction->hasBeenReshuffled; 
};

void reshuffleDeck(faction *faction){
    shuffle_deck(&(*faction)->f_deck);
    (*faction)->hasBeenReshuffled = 1;
};

void discardHand(faction *faction){
    for(int j=0; j<NB_CARDS_IN_HAND; j++){
        push_deck(pop_hand(&((*faction)->f_hand)), &((*faction)->f_deck));
    }
};

void shuffleDeck(faction *faction){
    shuffle_deck(&(*faction)->f_deck);
};

void drawCards(faction *faction){
    for (int i=0;i<NB_CARDS_IN_HAND;i++){
        push_hand(pop_deck(&((*faction)->f_deck)),&((*faction)->f_hand));
    }
}

char getFactionName(faction faction){
    return faction->f_name;
}

int getFactionDdrsPoints(faction faction){
    return faction->f_ddrsPoints;
}

hand getHand(faction faction){
    return faction->f_hand;
}

deck getDeck(faction faction){
    return faction->f_deck;
}

int getNbRoundWin(faction faction){
    return faction->nbRoundWin;
}