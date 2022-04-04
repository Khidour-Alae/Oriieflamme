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


/*--------------------------------------------------------------------------*/

//GETTERS

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

/*--------------------------------------------------------------------------*/

//SETTERS

/**
* \brief set the name of the faction
* \param faction a faction
* \param name name of the faction
*/
void setFactionName(faction faction, char* name); //DONE


/**
* \brief set number of DDRS' points
* \param faction a faction
* \param points number of DDRS' points
*/
void setFactionDdrsPoints(faction faction, int DDRS_Points);


/**
* \brief increase(or decrease if negatif) the DDRS' points of the team by the int "DDRS_PointsAdded"
* \param faction a faction
* \param DDRS_Points number of DDRS' points to add (can be negatif to remove)
*/
void addFactionDdrsPoints(faction faction, int DDRS_PointsAdded);


/**
* \brief set the hand of a faction
* \param faction a  faction
* \param hand a hand
*/
void setHand(faction faction, hand hand);


/**
* \brief set the deck of a faction
* \param faction a  faction
* \param deck a deck
*/
void setDeck(faction faction, deck deck);


/**
* \brief set the number of rounds won by the faction
* \param faction a  faction
* \param NbRoundWin points to set
*/
void setNbRoundWin(faction faction, int NbRoundWin);


/**
* \brief increase (or decrease if negatif) the number of rounds won by the faction by "NbRoundWinAdded"
* \param faction a faction 
* \param NbRoundWinAdded points to add (or remove)
*/
void addNbRoundWin(faction faction, int NbRoundWinAdded);




/*--------------------------------------------------------------------------*/



















#endif










