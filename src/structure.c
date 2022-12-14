#include "../headers/structure.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

struct deckBase
{
    card *c;
    int top;
    int nbOfCards; //utile pour les tests
};


//deck structure
deck init_deck() {
    deck d = malloc(sizeof(struct deckBase));
    d->c = malloc(sizeof(card)*NB_CARDS_IN_DECK);
    d->top = -1;
    d->nbOfCards = 0;
    return d;
}

int isEmpty_deck(deck d) {
    return d->top == -1;
}

void push_deck(card c, deck d) {
    if (d->top == NB_CARDS_IN_DECK - 1)
    {
        //raise error
    }
    else
    {
        d->top += 1;
        d->c[d->top] = c;  
        d->nbOfCards += 1;   
    }

}

card pop_deck(deck d) {
    d->top -= 1;
    d->nbOfCards -= 1; 
    return d->c[d->top + 1];
}

//using Fisher–Yates shuffle algorithm
void shuffle_deck(deck d) {
    srand(time(NULL));
    int j;
    for (int i = 0; i < d->top - 2; i++)
    {
        j = rand()%((d->top)-i) + i;
        card tmp = d->c[i];
        d->c[i] = d->c[j];
        d->c[j] = tmp;
    }
}

void reset_deck(deck d) {
    free(d->c);
    d->c = malloc(sizeof(card)*NB_CARDS_IN_DECK);
    d->top = -1;
    d->nbOfCards = 0;
}

void delete_deck(deck d) {
    free(d->c);
}



//hand structure
struct handBase
{
    card c[NB_CARDS_IN_HAND];
};

hand init_hand() {
    hand h = malloc(sizeof(struct handBase));
    for (int i = 0; i < NB_CARDS_IN_HAND; i++)
    {
        h->c[i] = NULL;
    }
    return h;
}
int isEmpty_hand(hand h) {
    for (int i = 0; i < NB_CARDS_IN_HAND; i++)
    {
        if (h->c[i] != NULL) return 0;
    }
    return 1;
}



int numberOfCards(hand h) {//Used for tests
    int n = 0;
    for (int i = 0; i < NB_CARDS_IN_HAND; i++)
    {
        if (h->c[i] != NULL) n+=1;
    }
    return n;
}

void setCard_hand(hand h, card c, int index) {
    h->c[index] = c;
}

card getCard_hand(hand h, int index) {
    return h->c[index];
}

//the hand being numbered from 1 to X
void discardCard_hand(hand h, int index) {
    h->c[index] = NULL;
}

void reset_hand(hand h) {
    for (int i = 0; i < NB_CARDS_IN_HAND; i++)
    {
        h->c[i] = NULL;
    }
}
