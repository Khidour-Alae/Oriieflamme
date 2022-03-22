#include "../headers/faction.h"
#include "../headers/structure.h"
#include <stdlib.h>
#include <stdio.h>

#define SIZE_NAME

struct impl_faction{
    char f_name[SIZE_NAME];
    int f_ddrsPoints;
    hand f_hand;
    deck f_deck;
    int hasBeenReshuffled; // 1 if the deck has been reshuffled, 0 if not
};

int hasTheDeckBeenShuffled(faction faction){
    return faction->hasBeenReshuffled; 
};

void reshuffleDeck(faction *faction){

};

void discardHand(faction *faction){

};

void shuffleDeck(faction *faction){

};

void drawCard(faction *faction){

};

