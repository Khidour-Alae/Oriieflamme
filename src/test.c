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
    In faction.c:
        1) Create a faction then initialize it
        2) Set the faction
        3) Get all its information
        4) Check that the deck has not been reshuffled and reshuffle it
        5) Discard the hand, shuffle the deck then draw cards
    In board.c:
        1) Create a board
        2) Check the list of factions 
        3) To test this part I decided to play a real game in real life with real cards and 
           then reproduce it with the function I have to see if I get the same result. 
           The game description will be in the report.
        4) Conclude and report the bugs
*/


void cardCreationTest(void){
    card c = const_card("test", 2, 3);
    CU_ASSERT_PTR_NOT_NULL(c);
    CU_ASSERT_STRING_EQUAL(getCardName(c),"test");
    CU_ASSERT_EQUAL(getCardEnum(c),2);
    CU_ASSERT_EQUAL(getNumberOfOccurrence(c),3);
}

void deckTest(void){
    deck d;
    
    init_deck(d);
    CU_ASSERT_PTR_NOT_NULL(d);
    CU_ASSERT(isEmpty_deck(d));

    for (int i = 1; i < 11; i++){ // Add 10 different cards
        card c = const_card((char) i,"useless",i,1); // Check this later
        push_deck(c,d);
        CU_ASSERT_FALSE(isEmpty_deck(d));
        CU_ASSERT_EQUAL(d->nbOfCards,i);
    }

    card c1 = pop_deck(d);
    CU_ASSERT_PTR_NOT_NULL(c);
    CU_ASSERT_STRING_EQUAL(getCardName(c),"10");
    CU_ASSERT_EQUAL(getCardEnum(c),10);
    CU_ASSERT_EQUAL(getNumberOfOccurrence(c),1);
    CU_ASSERT_EQUAL(d->nbOfCards,9);

    deck d2=d;
    shuffle_deck(d2);
    CU_ASSERT_PTR_NOT_EQUAL(d,d2);
    CU_ASSERT_PTR_NOT_NULL(d2);
    pop_deck(d);
    CU_ASSERT_EQUAL(d->nbOfCards,8);

    reset_deck(d);
    reset_deck(d2);
    CU_ASSERT(isEmpty_deck(d));
    CU_ASSERT(isEmpty_deck(d2));
    CU_ASSERT_EQUAL(d->nbOfCards,0);

    delete_deck(d);
    delete_deck(d2);
    CU_ASSERT_PTR_NULL(d);
    CU_ASSERT_PTR_NULL(d2);
}

void handTest(void){
    hand h;
    init_hand(h);
    CU_ASSERT_EQUAL(numberOfCards(h),0);
    CU_ASSERT_PTR_NOT_NULL(h);
    CU_ASSERT(isEmpty_hand(h));

    int i;
    for ( i = 0; i < NB_CARDS_IN_HAND; i++){ // Add 8 different cards
        card c = const_card((char) i,"useless",i,1); // Check this later
        setCard_hand(h,c,i)
        CU_ASSERT_FALSE(isEmpty_hand(h));
        CU_ASSERT_EQUAL(numberOfCards(h),(i+1));
    }
    
    card c1 = getCard_hand(h,1);
    CU_ASSERT_PTR_NOT_NULL(c1);
    CU_ASSERT_STRING_EQUAL(getCardName(c1),"1");
    CU_ASSERT_EQUAL(getCardEnum(c1),1);
    CU_ASSERT_EQUAL(getNumberOfOccurrence(c1),1);    

    card c2 = getCard_hand(h,2);
    CU_ASSERT_PTR_NOT_NULL(c2);
    CU_ASSERT_STRING_EQUAL(getCardName(c2),"2");
    CU_ASSERT_EQUAL(getCardEnum(c2),2);
    CU_ASSERT_EQUAL(getNumberOfOccurrence(c2),1);  

    discardCard_hand(h,4);
    CU_ASSERT_PTR_NULL(getCard_hand(h,4));

    reset_hand(h);
    CU_ASSERT_PTR_NOT_NULL(h);
    CU_ASSERT(isEmpty_hand(h))
}

void factionTest(void){
    faction f;
    initFaction(f);

    CU_ASSERT_EQUAL(hasTheDeckBeenShuffled(f),0);
    CU_ASSERT_EQUAL(getNbRoundWin(f),0);
    CU_ASSERT_EQUAL(getFactionDdrsPoints(f),0);
    
    for (int i = 1; i < 11; i++){ // Add 10 different cards
        card c = const_card((char) i,"useless",i,1);
        push_deck(c,getDeck(f));
    }

    drawCards(f);
    CU_ASSERT_FALSE(isEmpty_hand(f->f_hand));

    discardHand(f);
    CU_ASSERT(isEmpty_hand(f->f_hand));

    reshuffleDeck(f);
    CU_ASSERT_EQUAL(hasTheDeckBeenShuffled(f),1);

    
}




















