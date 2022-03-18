/**
* \file interface.h
*
* This file describes the functions we need to make the interface. 
* The interface (at least until Task C) is coded on the console.
* There are 7 fonctions:
* - showBoard prints the given board
* - showHand prints the given hand
* - asfkReshuffle asks the faction if they want to change their cards. They can only do it once during the game.
* - askWardWantToPlay asks the faction which card of their hand.
* - askWhereWantToPlaceCard aks the player where they want to put the card given as argument.
* - showCardEffect prints the card given as argument.
* - showWinner prints the faction given as argument to show the player they won the game.

*/

#ifdef _INTERFACE_H
#define _INTERFACE_H

#include <board.h>
#include <card.h>
#include <faction.h>


/**
* \brief prints the board \a b
* \param b A board
* \return nothing
*/ 
void showBoard(board b) {} 


/**
* \brief prints the hand of the faction \a f
* \param f A faction
* \return nothing
*/
void showHand(faction f) {}


/**
* \brief asks the faction \a f if they want change their hand.
* \param f A faction.
* \return The answer of the faction, a card.
*/
int askReshuffle(faction f) {}


/**
* \brief asks the faction \a f which of their cards they want to play.
* \param f A faction.
* \return The answer of the faction, a card.
*/
card askCardWantToPlay(faction f) {}


/**
* \brief asks a faction where to place the card \a c .
* \param c A card.
* \return The answer of the faction, an int.
*/
int askWhereWantToPlaceCard(card c) {}


/**
* \brief prints the effects of the card \a c .
* \param c A card.
* \return nothing.
*/
void showCardEffect(card c) {}


/**
* \brief prints the name of a faction, in order to show their are the winner.
* \param f A faction.
* \return nothing.
*/
void showWinner(faction f) {}

#endif


