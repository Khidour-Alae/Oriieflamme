/**
* \file board.h
*
* This file describes the functions we need to manage the board. 
* There are 6 fonctions and 1 type :
* - the type \a board define the structure of the board.
* - createBoard allow to create the board and the two factions.
* - freeBoard allow to free the memory allocated to the board and the two factions.
* - newRound it creates a new round, or indicates if the game is finished.
* - listFactions shows the list of the factions.
* - putDownCard it allows to put a card on the board.
* - flipCard return the card selected and activate its effect.
*/

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
* \param x is the position of the card on the x axe
* \param y is the position of the card on the y axe
**/
void putDownCard(board b, card c, faction f, int x, int y);

/**
* \brief return the card selected and activate its effect
* \param c is a pointer to the card to return
* \return 1 as long as the board contains a flipable card, 0 otherwise
* \attention the pointer must be valid
**/
int flipCard(board b, card *c);


#endif 
