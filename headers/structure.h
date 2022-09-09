#ifndef _STRUCTURE_H  
#define _STRUCTURE_H

#include "cards.h"

#define NB_CARDS_IN_DECK 44
#define NB_CARDS_IN_HAND 8
#define SIZE_2DBOARD 1089 // (4*NB_CARDS_IN_HAND + 1)^2

// #######################################################
// ### The deck structure
// #######################################################

/**
* \brief The deck is a classic FILO structure
*  with the particularity of providing an optimized shuffle function
**/
typedef struct deckBase * deck;

/**
* \brief Initializes the deck (mostly memory allocation)
**/
deck init_deck();

/**
* \brief Check if there are cards in the deck
* \param d is the deck
* \return 0 if there is at least one card in the deck, 1 otherwise
**/
int isEmpty_deck(deck d);

/**
* \brief Puts a card on top of the deck
* \param d is the deck
* \param c is the card
**/
void push_deck(card c, deck d);

/**
* \brief Draws the card on top of the deck
* \param d is the deck
* \return the card that was on top of the deck
**/
card pop_deck(deck d);

/**
* \brief Shuffles the deck using Fisher–Yates shuffle algorithm
* \param d is the deck
**/
void shuffle_deck(deck d);

/**
* \brief Empty the deck
* \param d is the deck
**/
void reset_deck(deck d);

/**
* \brief Deletes the deck (free allocated memory)
* \param d is the deck
**/
void delete_deck(deck d);


// #######################################################
// ### The hand structure
// #######################################################

/**
* \brief The hand is an array of cards of size NB_CARDS_IN_HAND
**/
typedef struct handBase * hand;

/**
* \brief Initializes the hand to the empty hand
**/
hand init_hand();

/**
* \brief Check if there are cards in the hand
* \param h is the hand
* \return 0 if there is at least one card in the hand, 1 otherwise
**/
int isEmpty_hand(hand h);

/**
* \brief Put a card in the hand at index \a index
* \param h is the hand
* \param c is the card
* \param index is the index at which you want to put the card, it is between 0 and NB_CARDS_IN_HAND
**/
void setCard_hand(hand h, card c, int index);

/**
* \brief Get the card at index \a index
* \param h is the hand
* \param index is the index at which you want to put the card, it is between 0 and NB_CARDS_IN_HAND
* \return the the card at index \a index
**/
card getCard_hand(hand h, int index);

/**
* \brief discard the nth card (the 1-th being the last card added to the hand)
* \param h is the hand
* \param index is the index at which you want to put the card, it is between 0 and NB_CARDS_IN_HAND
**/
void discardCard_hand(hand h, int index);

/**
* \brief Empty the hand
* \param h is the hand
**/
void reset_hand(hand h);

#endif