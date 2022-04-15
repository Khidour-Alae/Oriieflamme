/**
* To avoid multiple inclusions 
*/
#ifndef _CARDS_H  
#define _CARDS_H

typedef struct impl_card *card;


//GETTERS

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


/*--------------------------------------------------------------------------*/

//SETTERS

/**
* \brief set the name of the card
* \param card a card
* \param name name of the card
*/
void setCardName(card card, char* name); //DONE


/**
* \brief set the descripion of a card's effect
* \param card a card
* \param effectDescription description of the card's effect
*/
void setCardEffectDescription(card card, char* effectDescription); //DONE


/**
* \brief set the number of occurrence of a card
* \param card a card
* \param numberOfOccurrence number of occurrence of the card in the deck
*/
void setNumberOfOccurrence(card card, int numberOfOccurrence); //DONE


/**
* \brief set the state of the card (fliped or not)
* \param card a card
* \param status state of the card (fliped or not)
*/
void setCardStatus(card card, int status);//DONE

/*--------------------------------------------------------------------------*/









#endif