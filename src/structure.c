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


//board2D structure

//defined later but used some functions
int getPositionFromCoordinates_board2D(board2D *b2D, int x, int y);
int getXFromPosition_board2D(board2D *b2D, int p);
int getYFromPosition_board2D(board2D *b2D, int p);

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
    
    b2D->box.xmin = -1;
    b2D->box.ymin = -1;
    b2D->box.xmax = 1;
    b2D->box.ymax = 1;
}

int isEmpty_board2D(board2D *b2D) {
    for (int i = 0; i < b2D->sizeBoard2D; i++)
    {
        if (b2D->c[i] != NULL)
        {
            return 1;
        }
    }
    return 0;
}

int getCenter_board2D(board2D *b2D) {
    return getPositionFromCoordinates_board2D(b2D,0,0);
}

card getCard_board2D(board2D *b2D, int x, int y) {
    return b2D->c[getPositionFromCoordinates_board2D(b2D,x,y)];
}

faction getFaction_board2D(board2D *b2D, int x, int y) {
    return b2D->f[getPositionFromCoordinates_board2D(b2D,x,y)];
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
void addCard_board2D(board2D *b2D, card c, faction f, int x, int y) {
    
    //check if we need to resize the board
    if (x == 0 || x == b2D->sideLength - 1 || y == 0 || y == b2D->sideLength - 1)
    {
        resize_board2D(b2D);
    }

    //resize the boundingBox if needed
    b2D->box.xmin = min(x,b2D->box.xmin);
    b2D->box.ymin = min(y,b2D->box.ymin);
    b2D->box.xmax = max(x,b2D->box.xmax);
    b2D->box.ymax = max(y,b2D->box.ymax);

    //check if we can place the card in the board
    if (getCard_board2D(b2D,x,y) != NULL)
    {
        //there already is a card there
        //raise error
    }
    else //we place the card
    {
        //place the card
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
    (*xmin) = b2D->box.xmin;
    (*ymin) = b2D->box.ymin;
    (*xmax) = b2D->box.xmax;
    (*ymax) = b2D->box.ymax;
}

/**
* \brief get the position (index) correstponding to the point ( \a x, \a y)
* \param b2D is the board2D, we need the board2D argument as the board can be dynamically extended
* \param x is the x coordinate of the point
* \param y is the y coordinate of the point
* \return \a p the position corresponding to the point ( \a x, \a y)
**/
int getPositionFromCoordinates_board2D(board2D *b2D, int x, int y) {
    int xcenter = b2D->sideLength / 2;
    int ycenter = b2D->sideLength / 2;
    int xshifted = xcenter + x;
    int yshifted = ycenter + y;

    return b2D->sideLength * yshifted + xshifted;
}

/**
* \brief get the x coordinate correstponding to the position \a p
* \param b2D is the board2D, we need the board2D argument as the board can be dynamically extended
* \param p is the position
* \return \a x the x coordinate correstponding to the position \a p
**/
int getXFromPosition_board2D(board2D *b2D, int p) {
    int xcenter = b2D->sideLength / 2;
    int xshifted = p % b2D->sideLength;
    return xshifted - xcenter;
}

/**
* \brief get the y coordinate correstponding to the position \a p
* \param b2D is the board2D, we need the board2D argument as the board can be dynamically extended
* \param p is the position
* \return \a y the y coordinate correstponding to the position \a p
**/
int getYFromPosition_board2D(board2D *b2D, int p) {
    int ycenter = b2D->sideLength / 2;
    int yshifted = (p - (p % b2D->sideLength))/b2D->sideLength;
    return yshifted - ycenter;
}