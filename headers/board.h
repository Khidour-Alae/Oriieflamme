#ifndef _BOARD_H
#define _BOARD_H
#include "faction.h"
#include "cards.h"

typedef struct board_base * board;

/**
* \brief Create a board and the two factions
* \return a struct board with the elements initialized
**/
board createBoard();

/**
* \brief Free the memory allocated to the board and the two factions
* \param b A pointer of type board
* \attention the pointer must be valid
**/
void freeBoard(board b);

/**
* \brief Create a new round, or indicate if the game is finished
* \param counterRoundNumber is the number of round played
* \param f1 is the faction 1
* \param f2 is the faction 2
* \return 0 if the game is finished, 1 otherwise
**/
int newRound(int counterRoundNumber, faction f1, faction f2);

/**
* \brief show the list of the factions
* \param b is the board created at the beginning of the game
* \return the list of the factions
**/
faction* listFactions(board b);

/**
* \brief It allows to put a card on the board
* \param b is the board created at the beginning of the game
* \param c is the card the faction wants to put on the board
* \param f is the faction which wants to put the card on the board
* \param p is the position of the card oon the board
**/
void putDownCard(board b, card c, faction f, int p);

/**
* \brief return the card selected and activate its effect
* \param c is a pointer to the card to return
* \return 1 as long as the board contains a flipable card, 0 otherwise
* \attention the pointer must be valid
**/
int flipCard(board b, card *c);

/**
* \brief Get the center of the board
* \param b the board of which we wants the center
* \return the index of the center of the board
**/
int getCenterOfBoard(board b);

#endif 
