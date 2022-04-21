/**
* \file cards.h
*
* This file describes the functions we need to manipulate the cards. 
* There are 8 fonctions, 1 type and 1 enum :
* - the type \a *card define the structure of a card.
* - the enum \a cardName that contains all the name of available cards.
* - getCardName allow to get the name of the card.
* - getCardEffectDescription allow to get the descripion of a card's effect.
* - getNumberOfOccurrence allow to get the number of occurrence of a card.
* - getCardStatus allow to get the state of the card (flipped or not).
* - setCardName allow to set the name of the card.
* - setCardEffectDescription allow to set the descripion of a card's effect.
* - setNumberOfOccurrence allow to set the number of occurrence of a card.
* - setCardStatus allow to set the state of the card (flipped or not).
*/


/**
* To avoid multiple inclusions 
*/
#ifndef _CARDS_H  
#define _CARDS_H

typedef struct impl_card *card;
typedef enum cardName cardName;
enum cardName {
    FISE,
    FISA,
    FC,
    EcologIIE,
    lIIEns,
    Soiree_sans_alcool,
    Alcool,
    Cafe,
    The,
    Ecocup,
    Reprographie,
    Isolation_du_batiment,
    Parcours_sobriete_numerique,
    Heures_supplementaires,
    Kahina_Bouchama,
    Kevin_Goilard,
    Massinissa_Merabet,
    Vitera_Y,
    Jonas_Senizergues,
    Fetia_Bannour,
    Catherine_Dubois,
    Anne_Laure_Ligozat,
    Guillaume_Burel,
    Christophe_Mouilleron,
    Thomas_Lim,
    Julien_Forest,
    Dimitri_Watel,
    Djibril_Aurelien_Dembele_Cabot,
    Eric_Lejeune,
    Lucienne_Pacave,
    Katrin_Salhab,
    Laurent_Prevel
};

/**
* \brief initialize the different cards of the deck
* \param nameCard the name of the card
* \param descCard the description of the card
* \param enumCard the name of the card in the enum
* \param numberOccurence the number of occurence of the card
* \return the descripion of the card's effect
*/
card const_card(char* nameCard, char* descCard, cardName enumCard, int numberOccurence);

//GETTERS

/**
* \brief get the name of the card
* \param card a card
* \return the name of the card
*/
char* getCardName(card card); //DONE


/**
* \brief get the descripion of a card's effect
* \param card a card
* \return the descripion of the card's effect
*/
char* getCardEffectDescription(card card); //DONE

/**
* \brief get the number of occurrence of a card
* \param card a card
* \return the number of occurrence of a card
*/
int getNumberOfOccurrence(card card); //DONE


/**
* \brief get the state of the card (flipped or not)
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
* \brief set the state of the card (flipped or not)
* \param card a card
* \param status state of the card (flipped or not)
*/
void setCardStatus(card card, int status);//DONE

/*--------------------------------------------------------------------------*/









#endif