/**
*To avoid multiple inclusions 
*/
#ifndef _FACTIONS_H  
#define _FACTIONS_H

typedef struct impl_faction *faction;

/**
* \brief Allows to know if the shuffle option was used
* \param faction Valid faction
* \return Returns 1 if the shuffle option was used, else returns 0
*/
int hasTheDeckBeenShuffled(faction faction);



/**
* \brief Shuffles the deck and remove the right of the faction to use the shuffle option
* \param faction Valid faction
*/
void reshuffleDeck(faction *faction);



/**
* \brief Discards all the cards from the hand to the deck
* \param faction Valid faction
*/
void discardHand(faction *faction);



/**
* \brief Shuffles the deck 
* \param faction Valid faction
*/
void shuffleDeck(faction *faction);



/**
* \brief Draws 8 cards from the deck to the hand
* \param faction Valid faction
*/
void drawCard(faction *faction);


/**
* \brief 
* \param 
* \return 
*/

/**
* \brief 
* \param 
* \return 
*/
char getFactionName(faction f);

/**
* \brief 
* \param 
* \return 
*/
int getFactionDdrsPoints(faction f);

/**
* \brief 
* \param 
* \return 
*/






















#endif










