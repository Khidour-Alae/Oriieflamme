#include "../headers/structure.h"
#include <stdlib.h>
#include <time.h>

struct deckBase
{
    card *c;
    int top;
};


//deck structure
void init_deck(deck d) {
    d->c = malloc(sizeof(card)*NB_CARDS_IN_DECK);
    d->top = -1;
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
    }
}

card pop_deck(deck d) {
    d->top -= 1;
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
    init_deck(d);
}

void delete_deck(deck d) {
    free(d->c);
}



//hand structure
struct handBase
{
    card c[NB_CARDS_IN_HAND];
};

void init_hand(hand h) {
    for (int i = 0; i < NB_CARDS_IN_HAND; i++)
    {
        h->c[i] = NULL;
    }
}
int isEmpty_hand(hand h) {
    for (int i = 0; i < NB_CARDS_IN_HAND; i++)
    {
        if (h->c[i] != NULL) return 0;
    }
    return 1;
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
    init_hand(h);
}