#include "../headers/structure.h"
#include <stdlib.h>
#include <time.h>

//deck structure
void init_deck(deck *d) {
    d->c = malloc(sizeof(card)*NB_CARDS_IN_DECK);
    d->top = -1;
}

int isEmpty_deck(deck d) {
    return d.top == -1;
}

void push_deck(card c, deck *d) {
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

card pop_deck(deck *d) {
    d->top -= 1;
    return d->c[d->top + 1];
}

//using Fisher–Yates shuffle algorithm
void shuffle_deck(deck *d) {
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

void reset_deck(deck *d) {
    free(d->c);
    init_deck(d);
}

void delete_deck(deck *d) {
    free(d->c);
}



//hand structure
void init_hand(hand *h) {
    h->c = malloc(sizeof(card)*NB_CARDS_IN_HAND);
    h->top = -1;
}
int isEmpty_hand(hand h) {
    return h.top == -1;
}

void push_hand(card c, hand *h) {
        if (h->top == NB_CARDS_IN_HAND - 1)
    {
        //raise error
    }
    else
    {
        h->top += 1;
        h->c[h->top] = c;        
    }
}

card pop_hand(hand *h) {
    h->top -= 1;
    return h->c[h->top + 1];
}

void reset_hand(hand *h) {
    free(h->c);
    init_hand(h);
}
void delete_hand(hand *h) {
    free(h->c);
}