#include "../headers/board.h"
#include "../headers/structure.h"
#include <stdio.h>
#include <stdlib.h>

// #######################################################
// ### The board2D structure
// #######################################################

typedef struct board2DBase * board2D;

/**
* It's recommended to read the comments of board2D before.
* board2DBoundingBox defines the boundingBox of the board2D.
* It just represent a rectangle with two points :
* one point is the lowest left point
* one point is the highest right point
*
**/
typedef struct {
    int xmin; int ymin; int xmax; int ymax;
} board2DBoundingBox;

/**
* board2D is a complex structure. It represents the board,
* that is to say a 2D plane on which you can place cards 
* belonging to factions. The cards can only be placed on 
* integer coordinates (for example : (0,0), (3,2), (-1,-2)).
* 
* To do this, we make two 2D-arrays, one that store the cards
* and one that stores from which faction the card was placed.
* 
* The 2D-arrays are implemented as a 1D-array with the following
* transformation : 1DArray[sizeOfLine * y + x] <=> 2DArray[x][y]
* 
* To represent an infinite 2D plane, the arrays can be
* dynamically resized. For example, if the array covers the
* 2D plane for :
*   # -5 <= x <= 5
*   # -5 <= y <= 5
* and we need to place a card at (5,6), then the array will
* grow in order to cover enough of the 2D plane to place the card
*
* As the array could be covering unecessary parts of the 2D plane
* we are using a bounding box. For example :
* if the array is covering -50 <= x <= 50 and -50 <= y <= 50
* but there are cards only in -10 <= x <= 3 and -7 <= y <= 4
* then the bounding box is used to get this information
* This is usefull mostly for printing the board or iterating
* through the board.
*
**/
struct board2DBase {
    card *c;
    faction *f;
    board2DBoundingBox box;
    int sideLength; //the board is a square
    int sizeBoard2D; //the 1D-array length
};


//defined later but used some functions
int getPositionFromCoordinates_board2D(board2D b2D, int x, int y);
int getXFromPosition_board2D(board2D b2D, int p);
int getYFromPosition_board2D(board2D b2D, int p);

/**
* \brief Initializes the board (mostly memory allocation)
* \param b2D is the board2D
**/
void init_board2D(board2D b2D) {
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

/**
* \brief Check if the board is empty
* \param b2D is the board2D
* \return 0 if there is at least one card placed on the board, 1 otherwise
**/
int isEmpty_board2D(board2D b2D) {
    for (int i = 0; i < b2D->sizeBoard2D; i++)
    {
        if (b2D->c[i] != NULL)
        {
            return 1;
        }
    }
    return 0;
}


/**
* \brief get the card placed at coordinates ( \a x, \a y)
* \param b2D is the board2D
* \param x is the x-coordinate
* \param y is the y-coordinate
* \return NULL if there are no card placed at at coordinates ( \a x, \a y); return the card otherwise
**/
card getCard_board2D(board2D b2D, int x, int y) {
    return b2D->c[getPositionFromCoordinates_board2D(b2D,x,y)];
}

/**
* \brief get the faction that played the card placed at coordinates ( \a x, \a y)
* \param b2D is the board2D
* \param x is the x-coordinate
* \param y is the y-coordinate
* \return NULL if there are no card placed at coordinates ( \a x, \a y); return the faction that played the card otherwise
**/
faction getFaction_board2D(board2D b2D, int x, int y) {
    return b2D->f[getPositionFromCoordinates_board2D(b2D,x,y)];
}

void resize_board2D(board2D b2D) {
    b2D->sideLength += 2;
    b2D->sizeBoard2D = b2D->sideLength * b2D->sideLength;
    card *newc = malloc(sizeof(card)*b2D->sizeBoard2D);
    faction *newf = malloc(sizeof(card)*b2D->sizeBoard2D);
    
    for (int i = 0; i < b2D->sideLength-2; i++)
    {
        for (int j = 0; j < b2D->sideLength-2; j++)
        {
            newc[(i+1)*b2D->sideLength + (j+1)] = (b2D->c[i*b2D->sideLength + j]);
            newc[(i+1)*b2D->sideLength + (j+1)] = (b2D->f[i*b2D->sideLength + j]);
        }
    }

    free(b2D->c); free(b2D->f);
    b2D->c = newc; b2D->f = newf;
}

int min(int a, int b) {return a < b ? a : b;}
int max(int a, int b) {return a < b ? b : a;}

/**
* \brief place a card \a c at coordinates ( \a x, \a y)
* \param b2D is the board2D
* \param c is the card
* \param f is the faction that played the card
* \param x is the x-coordinate at which you want to place the card
* \param y is the y-coordinate at which you want to place the card
**/
void addCard_board2D(board2D b2D, card c, faction f, int x, int y) {
    
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

/**
* \brief puts the board back to its original state (that is to say the empty board)
* \param b2D is the board2D
**/
void reset_board2D(board2D b2D) {
    free(b2D->c);
    free(b2D->f);
    init_board2D(b2D);
}

/**
* \brief Deletes the board2D (free allocated memory)
* \param b2D is the board2D
**/
void delete_board2D(board2D b2D) {
    free(b2D->c);
    free(b2D->f);
}

/**
* \brief get the bounding box of the board2D
* \param b2D is the board2D
* \param xmin is the x coordinate of the lowest left point of the bounding box (=rectangle)
* \param ymin is the y coordinate of the lowest left point of the bounding box (=rectangle)
* \param xmax is the x coordinate of the highest right point of the bounding box (=rectangle)
* \param ymax is the y coordinate of the highest right point of the bounding box (=rectangle)
**/
void getBoundingBox(board2D b2D, int *xmin, int *ymin, int *xmax, int *ymax) {
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
int getPositionFromCoordinates_board2D(board2D b2D, int x, int y) {
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
int getXFromPosition_board2D(board2D b2D, int p) {
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
int getYFromPosition_board2D(board2D b2D, int p) {
    int ycenter = b2D->sideLength / 2;
    int yshifted = (p - (p % b2D->sideLength))/b2D->sideLength;
    return yshifted - ycenter;
}

// #######################################################
// ### The board structure
// #######################################################

struct board_base
{
    board2D b2D; // Board of the game
    faction listFactions[2]; // List of factions playing on the board 
};



board createBoard(){
    board b;
    init_board2D(b->b2D);
    faction f1;
    faction f2;
    initFaction(f1);
    initFaction(f2);
    b->listFactions[0] = f1;
    b->listFactions[1] = f2;
    return b;
}

void freeBoard(board b){
    delete_board2D(b->b2D);
}

int newRound(int counterRoundNumber, faction f1, faction f2){
    if (counterRoundNumber == 3) return 0;
    if (getNbRoundWin(f1) == 2 || getNbRoundWin(f2) == 2) return 0;
    return 1;
}

faction* listFactions(board b){
    return b->listFactions;
}

void putDownCard(board b, card c, faction f, int x, int y){
    addCard_board2D(b->b2D, c, f, x, y);
}

int flipCard(board b, card *c){
    /* Récupérer le bounding box de b avec la fonction getBoundingBox
    Il faudra ensuite parcourir le tableau de en haut à gauche jusqu'à en bas à droite, et utiliser la fonction getCard_board2D. 
    S'il n'y a aucune carte, cette fonction revoie NULL. 
    S'il y a une carte, il faudra retourner la carte. */
}
