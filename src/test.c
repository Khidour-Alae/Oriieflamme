#include <CUnit/CUnit.h>
#include "board.c"
#include "structure.c"
#include "cards.c"
#include "faction.c"
#include "interface.c"


/* 
Functions to test:
    In cards.c:
        1) Create a card 
        2) Set the card 
        3) Get all its information
    In structure.c:
        I) Deck:
            1) init_deck
            2) Check that it's empty
            3) Push some cards (10 cards) in the deck to test push_deck
            4) Check that it's not empty
            5) Draw some cards to test pop_deck
            6) Shuffle the deck
            7) Drawn again
            8) Reset the deck
            9) Delete the deck
        II) Hand:
            1) init_hand
            2) Check that it's empty
            3) Set 8 cards in the hand
            4) Check that it's not empty
            5) Get them all
            6) Discard some cards 
            7) Reset the hand
    
*/





























void testFactionCreation(void){
    CU_ASSERT_PTR_NOT_NULL(createFaction());
}

void testBoardCreation(void){
    CU_ASSERT_PTR_NOT_NULL(createBoard());
}