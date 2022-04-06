#include "../headers/faction.h"
#include <stdlib.h>
#include <stdio.h>

#define SIZE_NAME 150

struct impl_faction {
    char f_name[SIZE_NAME]; // Name of the faction
    int f_ddrsPoints; // Number of DDRS points of the team
    hand f_hand; // Hand of the faction
    deck f_deck; // Deck of the faction
    int hasBeenReshuffled; // 1 if the deck has been reshuffled, 0 if not
    int nbRoundWin; // Number of rounds won by the faction 
};

int hasTheDeckBeenShuffled(faction faction){
    return faction->hasBeenReshuffled; 
};

void reshuffleDeck(faction faction){
    shuffle_deck(&(faction->f_deck));
    faction->hasBeenReshuffled = 1;
};

void discardHand(faction faction){
    for(int j=0; j<NB_CARDS_IN_HAND; j++){
        push_deck(pop_hand(&(faction->f_hand)), &(faction->f_deck));
    }
};

void shuffleDeck(faction faction){
    shuffle_deck(&(faction->f_deck));
};

void drawCards(faction faction){
    for (int i=0;i<NB_CARDS_IN_HAND;i++){
        push_hand(pop_deck(&(faction->f_deck)),&(faction->f_hand));
    }
}

/*--------------------------------------------------------------------------*/

//GETTERS

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

/*--------------------------------------------------------------------------*/

//SETTERS

void setFactionName(faction faction, char* name){
    if (strlen(name) < strlen(faction->f_name)){    
        strcpy(faction->f_name, name);
    }
}

void setFactionDdrsPoints(faction faction, int DDRS_Points){
    faction->f_ddrsPoints=DDRS_Points;
}

void addFactionDdrsPoints(faction faction, int DDRS_PointsAdded){
    faction->f_ddrsPoints += DDRS_PointsAdded;
}

void setHand(faction faction, hand hand){
    faction->f_hand = hand; 
}

void setDeck(faction faction, deck deck){
    faction->f_deck = deck;
}

void setNbRoundWin(faction faction, int nbRoundWin){
    faction->nbRoundWin = nbRoundWin;
}

void addNbRoundWin(faction faction, int nbRoundWinAdded){
    faction->nbRoundWin += nbRoundWinAdded;
}

/*--------------------------------------------------------------------------*/