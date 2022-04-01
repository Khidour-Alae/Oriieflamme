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

//the hand being numbered from 1 to X
card popNthCard_hand(hand *h, int n) {
    hand tmp;
    for (int i = 0; i < n - 1; i++)
    {
        if (!isEmpty_hand(*h))
        {
            push_hand(pop_hand(h),&tmp);
        }
        else
        {
            //raise exception
        }
    }
    if (!isEmpty_hand(*h))
    {
        card nthCard = pop_hand(h);
        while (!isEmpty_hand(tmp))
        {
            push_hand(pop_hand(&tmp),h);
        }
        return nthCard;
    }
    else
    {
        //raise exception
    }
}

card getTopCard_hand(hand *h) {
    return h->c[h->top];
}

card getNthCard_hand(hand *h, int n) {
    hand tmp;
    for (int i = 0; i < n - 1; i++)
    {
        if (!isEmpty_hand(*h))
        {
            push_hand(pop_hand(h),&tmp);
        }
        else
        {
            //raise exception
        }
    }
    if (!isEmpty_hand(*h))
    {
        card nthCard = pop_hand(h);
        push_hand(nthCard,h);
        while (!isEmpty_hand(tmp))
        {
            push_hand(pop_hand(&tmp),h);
        }
        return nthCard;
    }
    else
    {
        //raise exception
    }
}

void reset_hand(hand *h) {
    free(h->c);
    init_hand(h);
}
void delete_hand(hand *h) {
    free(h->c);
}


//board2D structure
void init_board2D(board2D *b2D) {
    b2D->sizeBoard2D = SIZE_2DBOARD;
    b2D->c = malloc(sizeof(card)*b2D->sizeBoard2D);
    b2D->f = malloc(sizeof(faction)*b2D->sizeBoard2D);
    for (int i = 0; i < b2D->sizeBoard2D; i++)
    {
        b2D->c[i] = NULL;
        b2D->f[i] = NULL;
    }
    b2D->sideLength = 4*NB_CARDS_IN_HAND + 1;
    
    //we would like to do
    //b2D->box.pmax = getPositionFromCoordinates_board2D(xmax = 1, ymax = 1);
    //but we can't use getPositionFromCoordinates_board2D() yet as it requires a board as parameter
    int xmax = 1, ymax = 1;
    int xcenter = b2D->sideLength / 2;
    int ycenter = b2D->sideLength / 2;
    int xshiftedmax = xcenter + xmax;
    int yshiftedmax = ycenter + ymax;

    b2D->box.pmax = b2D->sideLength * yshiftedmax + xshiftedmax;
    //same goes for b2D->box.pmin
    int xmin = -1, ymin = -1;
    int xshiftedmin = xcenter + xmin;
    int yshiftedmin = ycenter + ymin;

    b2D->box.pmin = b2D->sideLength * yshiftedmin + xshiftedmin;
}

int isEmpty_board2D(board2D b2D) {
    for (int i = 0; i < b2D.sizeBoard2D; i++)
    {
        if (b2D.c[i] != NULL)
        {
            return 1;
        }
    }
    return 0;
}

int getCenter_board2D(board2D *b2D) {
    return getPositionFromCoordinates_board2D(b2D,0,0);
}

card getCard_board2D(board2D *b2D, int p) {
    return b2D->c[p];
}

faction getFaction_board2D(board2D *b2D, int p) {
    return b2D->f[p];
}

void resize_board2D(board2D *b2D) {
    b2D->sideLength += 2;
    b2D->sizeBoard2D = b2D->sideLength * b2D->sideLength;
    card *newc = malloc(sizeof(card)*b2D->sizeBoard2D);
    faction *newf = malloc(sizeof(card)*b2D->sizeBoard2D);
    
    for (int i = 0; i < b2D->sideLength-2; i++)
    {
        for (int j = 0; j < b2D->sideLength-2; j++)
        {
            newc[(i+1)*b2D->sideLength + (j+1)] = b2D->c[i*b2D->sideLength + j];
            newc[(i+1)*b2D->sideLength + (j+1)] = b2D->f[i*b2D->sideLength + j];
        }
    }

    free(b2D->c); free(b2D->f);
    b2D->c = newc; b2D->f = newf;
}

int min(int a, int b) {return a < b ? a : b;}
int max(int a, int b) {return a < b ? b : a;}
void addCard_board2D(board2D *b2D, card c, faction f, int pos) {
    
    //check if we can place the card in the board
    int x = getXFromPosition_board2D(b2D,pos);
    int y = getYFromPosition_board2D(b2D,pos);

    if (getCard_board2D(b2D,pos) != NULL)
    {
        //there already is a card there
        //raise error
    }
    else //we place the card
    {
        //check if we need to resize the board
        if (x == 0 || x == b2D->sideLength - 1 || y == 0 || y == b2D->sideLength - 1)
        {
            resize_board2D(b2D);
        }

        //resize the boundingBox if needed
        int x_min = getXFromPosition_board2D(b2D, b2D->box.pmin);
        int y_min = getYFromPosition_board2D(b2D, b2D->box.pmin);
        int x_max = getXFromPosition_board2D(b2D, b2D->box.pmax);
        int y_max = getYFromPosition_board2D(b2D, b2D->box.pmax);
        
        b2D->box.pmin = getPositionFromCoordinates_board2D(b2D,min(x,x_min),min(y,y_min));
        b2D->box.pmax = getPositionFromCoordinates_board2D(b2D,max(x,x_max),max(y,y_max));

        //place the card
        //we need to update the position as the board might have been resized
        int p = getPositionFromCoordinates_board2D(b2D,x,y);
        b2D->c[p] = c; b2D->f[p] = f;
    }
}

void reset_board2D(board2D *b2D) {
    free(b2D->c);
    free(b2D->f);
    init_board2D(b2D);
}

void delete_board2D(board2D *b2D) {
    free(b2D->c);
    free(b2D->f);
}

void getBoundingBox(board2D *b2D, int *xmin, int *ymin, int *xmax, int *ymax) {
    (*xmin) = getXFromPosition_board2D(b2D, b2D->box.pmin);
    (*ymin) = getXFromPosition_board2D(b2D, b2D->box.pmin);
    (*xmax) = getXFromPosition_board2D(b2D, b2D->box.pmax);
    (*ymax) = getYFromPosition_board2D(b2D, b2D->box.pmax);
}

int getPositionFromCoordinates_board2D(board2D *b2D, int x, int y) {
    int xcenter = b2D->sideLength / 2;
    int ycenter = b2D->sideLength / 2;
    int xshifted = xcenter + x;
    int yshifted = ycenter + y;

    return b2D->sideLength * yshifted + xshifted;
}

int getXFromPosition_board2D(board2D *b2D, int p) {
    int xcenter = b2D->sideLength / 2;
    int xshifted = p % b2D->sideLength;
    return xshifted - xcenter;
}

int getYFromPosition_board2D(board2D *b2D, int p) {
    int ycenter = b2D->sideLength / 2;
    int yshifted = (p - (p % b2D->sideLength))/b2D->sideLength;
    return yshifted - ycenter;
}