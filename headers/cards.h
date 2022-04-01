/**
* To avoid multiple inclusions 
*/
#ifndef _CARDS_H  
#define _CARDS_H

typedef struct impl_card *card;



/**
* \brief get the name of the card
* \param c a card
* \return the name of the card
*/
char getCardName(card c);


/**
* \brief get the descripion of a card's effect
* \param c a card
* \return the descripion of the card's effect
*/
char getCardEffectDescription(card c);

/**
* \brief get the number of occurrence of a card
* \param c a card
* \return the number of occurrence of a card
*/
int getNumberOfOccurrence(card c);

/**
* \brief get the state of the card (fliped or not)
* \param c a card
* \return the state of the card
*/
int getCardStatus(card c);














#endif