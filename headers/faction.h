/**
*To avoid multiple inclusions 
*/
#include "../headers/structure.h"
#ifndef _FACTIONS_H  
#define _FACTIONS_H


typedef struct impl_faction *faction;

/**
* \brief Allows to know if the shuffle option was used
* \param faction Valid faction
* \return 1 if the shuffle option was used, else returns 0
*/
int hasTheDeckBeenShuffled(faction faction); //DONE



/**
* \brief Shuffles the deck and remove the right of the faction to use the shuffle option
* \param faction Valid faction
*/
void reshuffleDeck(faction faction); //DONE



/**
* \brief Discards all the cards from the hand to the deck
* \param faction Valid faction
*/
void discardHand(faction faction); //DONE



/**
* \brief Shuffles the deck 
* \param faction Valid faction
*/
void shuffleDeck(faction faction); //DONE



/**
* \brief Draws 8 cards from the deck to the hand
* \param faction Valid faction
*/
void drawCards(faction faction); //DONE


/**
* \brief 
* \param 
* \return 
*/

/**
* \brief get the name of a faction
* \param faction a faction
* \return the name of the faction
*/
char getFactionName(faction faction); //DONE


/**
* \brief get the number of DDRS points of a faction
* \param faction a faction
* \return the number of DDRS points of the faction
*/
int getFactionDdrsPoints(faction faction); //DONE


/**
* \brief get the hand of a faction
* \param faction a faction
* \return the hand of the faction
*/
hand getHand(faction faction); //DONE


/**
* \brief get the deck of a faction
* \param faction a faction
* \return the deck of the faction
*/
deck getDeck(faction faction); //DONE


/**
* \brief get the number of rounds won by a faction
* \param faction a faction
* \return the number of rounds won by the faction
*/
int getNbRoundWin(faction faction); //DONE





















#endif










