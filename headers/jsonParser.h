#ifndef _JSONPARSER_H  
#define _JSONPARSER_H

typedef struct jsonParser_base *jsParser;

/**
* \brief initialize the parser
* \param filePath the path to the json file to parse
* \return the parser
*/
jsParser initParser(char *filePath);

/**
* \brief get the id of the card of index \a index
* \param jsP the parser
* \param index the index (we can see the parser as an array of cards)
* \return the card id
*/
int getCardId_json(jsParser jsP, int index);
/**
* \brief get the name of the card of index \a index
* \param jsP the parser
* \param index the index (we can see the parser as an array of cards)
* \return the card name
*/
char * getCardName_json(jsParser jsP, int index);
/**
* \brief get number of occurences of the card of index \a index
* \param jsP the parser
* \param index the index (we can see the parser as an array of cards)
* \return the number of occurences of the card
*/
int getCardOccurences_json(jsParser jsP, int index);
/**
* \brief get the description of the card of index \a index
* \param jsP the parser
* \param index the index (we can see the parser as an array of cards)
* \return the card's description
*/
char * getCardDescription_json(jsParser jsP, int index);
/**
* \brief get the number of cards in the json file
* \param jsP the parser
* \param index the index (we can see the parser as an array of cards)
* \return the number of cards in the json file
*/
int getSize_json(jsParser jsP);

/**
* \brief free the allocated memory
* \param jsP the parser
*/
void freeParser(jsParser jsP);

#endif