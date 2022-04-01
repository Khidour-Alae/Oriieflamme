/**
* To avoid multiple inclusions 
*/
#ifndef _CARDS_H  
#define _CARDS_H

typedef struct impl_card *card;



/**
* \brief get the name of the card
* \param card a card
* \return the name of the card
*/
char getCardName(card card); //DONE


/**
* \brief get the descripion of a card's effect
* \param card a card
* \return the descripion of the card's effect
*/
char getCardEffectDescription(card card); //DONE

/**
* \brief get the number of occurrence of a card
* \param card a card
* \return the number of occurrence of a card
*/
int getNumberOfOccurrence(card card); //DONE

/**
* \brief get the state of the card (fliped or not)
* \param card a card
* \return the state of the card
*/
int getCardStatus(card card); //DONE














#endif