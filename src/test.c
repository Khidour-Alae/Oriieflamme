#include <CUnit/CUnit.h>
#include "CUnit/Basic.h"
#include "board.c"
#include "structure.c"
#include "cards.c"
#include "faction.c"
#include "interface.c"
#include <stdlib.h>
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
            4) Check that it's not emptys
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
    card c = const_card("test", "useless", 2, 3);
    CU_ASSERT_PTR_NOT_NULL(c);
    CU_ASSERT_STRING_EQUAL(getCardName(c),"test");
    CU_ASSERT_EQUAL(getCardEnumName(c),2);
    CU_ASSERT_EQUAL(getNumberOfOccurrence(c),3);
}

char string[20]; // Will be used for itoa function to transform an int to a char*

void deckTest(void){
    deck d;
    
    d=init_deck();
    CU_ASSERT_PTR_NOT_NULL(d);
    CU_ASSERT(isEmpty_deck(d));

    for (int i = 1; i < 11; i++){ // Add 10 different cards
        card c = const_card(itoa(i,string,10),"useless",i,1); // Check this later
        push_deck(c,d);
        CU_ASSERT_FALSE(isEmpty_deck(d));
        CU_ASSERT_EQUAL(d->nbOfCards,i);
    }

    card c1 = pop_deck(d);
    CU_ASSERT_PTR_NOT_NULL(c1);
    CU_ASSERT_STRING_EQUAL(getCardName(c1),"10");
    CU_ASSERT_EQUAL(getCardEnumName(c1),10);
    CU_ASSERT_EQUAL(getNumberOfOccurrence(c1),1);
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
    h=init_hand();
    CU_ASSERT_EQUAL(numberOfCards(h),0);
    CU_ASSERT_PTR_NOT_NULL(h);
    CU_ASSERT(isEmpty_hand(h));

    int i;
    for ( i = 0; i < NB_CARDS_IN_HAND; i++){ // Add 8 different cards
        card c = const_card(itoa(i,string,10),"useless",i,1); // Check this later
        setCard_hand(h,c,i);
        CU_ASSERT_FALSE(isEmpty_hand(h));
        CU_ASSERT_EQUAL(numberOfCards(h),(i+1));
    }
    
    card c1 = getCard_hand(h,1);
    CU_ASSERT_PTR_NOT_NULL(c1);
    CU_ASSERT_STRING_EQUAL(getCardName(c1),"1");
    CU_ASSERT_EQUAL(getCardEnumName(c1),1);
    CU_ASSERT_EQUAL(getNumberOfOccurrence(c1),1);    

    card c2 = getCard_hand(h,2);
    CU_ASSERT_PTR_NOT_NULL(c2);
    CU_ASSERT_STRING_EQUAL(getCardName(c2),"2");
    CU_ASSERT_EQUAL(getCardEnumName(c2),2);
    CU_ASSERT_EQUAL(getNumberOfOccurrence(c2),1);  

    discardCard_hand(h,4);
    CU_ASSERT_PTR_NULL(getCard_hand(h,4));

    reset_hand(h);
    CU_ASSERT_PTR_NOT_NULL(h);
    CU_ASSERT(isEmpty_hand(h));
}

void factionTest(void){
    faction f;
    f=initFaction("f");

    CU_ASSERT_EQUAL(hasTheDeckBeenShuffled(f),0);
    CU_ASSERT_EQUAL(getNbRoundWin(f),0);
    CU_ASSERT_EQUAL(getFactionDdrsPoints(f),0);
    
    for (int i = 1; i < 11; i++){ // Add 10 different cards
        card c = const_card(itoa(i,string,10),"useless",i,1);
        push_deck(c,getDeck(f));
    }

    drawCards(f);
    CU_ASSERT_FALSE(isEmpty_hand(f->f_hand));

    discardHand(f);
    CU_ASSERT(isEmpty_hand(f->f_hand));

    reshuffleDeck(f);
    CU_ASSERT_EQUAL(hasTheDeckBeenShuffled(f),1);

    
}

void gameTest(void){
    board b; faction* f;
    faction f1; faction f2;
    b = createBoard();
    f = listFactions(b);
    f1 = f[0]; f2 = f[1];

    int roundCounter = 0;
    
    while (1) 
    {
        clearBoard(b);//gets rid of any cards placed on the board if there are any
        if (!(newRound(roundCounter,b,f1,f2))) //if game finished we leave the wile loop
        {
            break;
        }
        else roundCounter++;

        switch (roundCounter)
        {
        case 1:

            CU_ASSERT_EQUAL(getNbRoundWin(f1),0);
            CU_ASSERT_EQUAL(getNbRoundWin(f2),0);

            //round 1: Communiste starting

            CU_ASSERT_EQUAL(getFactionName(f1),"Communiste");
            CU_ASSERT_EQUAL(getFactionName(f2),"Capitaliste");

            card fise = const_card("FISE", "La faction qui a posé cette carte gagne 1 point DDRS.", FISE, 4);
            card ecologiie = const_card("EcologIIE", "La faction qui a posé cette carte gagne 1 point DDRS par carte FISE/FISA/FC retournée.", EcologIIE, 2);
            card the = const_card("The", "Supprimez toutes les cartes Café et Alcool retournées sur le plateau. Si une carte Ecocup est retournée sur le plateau, la faction qui a posé cette carte gagne 1 point DDRS. Sinon elle perd 1 point DDRS.", The, 3);
            setCard_hand(f1->f_hand,fise,0);
            setCard_hand(f1->f_hand,the,1);
            setCard_hand(f1->f_hand,ecologiie,2);

            card fisa = const_card("FISA", "La faction qui a posé cette carte gagne 2 points DDRS si le nombre de cartes retournées sur le plateau (y compris celle-ci) est pair, et 0 sinon.", FISA, 4);
            card dimitri_watel = const_card("Dimitri_Watel", "La faction qui a posé cette carte gagne 3 points DDRS par carte FISA ou FC retournée sur le plateau si au moins une carte Thé est retournée sur le plateau.", Dimitri_Watel, 1);
            card ecocup = const_card("Ecocup", "Cette carte est sans effet.", Ecocup, 1);
            setCard_hand(f2->f_hand,fisa,0);
            setCard_hand(f2->f_hand,ecocup,1);
            setCard_hand(f2->f_hand,dimitri_watel,2);

                
            putDownCard(b,getCard_hand(f1->f_hand,0),f1,0,0); // fise put
            CU_ASSERT_TRUE(getCard_board2D(b->b2D,0,0)==fise); //Test putDownCard
            putDownCard(b,getCard_hand(f2->f_hand,0),f2,1,0); // fisa put

            putDownCard(b,getCard_hand(f1->f_hand,1),f1,2,0); // the put
            putDownCard(b,getCard_hand(f2->f_hand,1),f2,1,-1); // ecocup put

            putDownCard(b,getCard_hand(f1->f_hand,2),f1,1,-2); // ecologiie put
            putDownCard(b,getCard_hand(f2->f_hand,2),f2,2,-1); // dimitri_watel put

            //reveal round 1
            card cardFlipped;
            flipCard(b, &cardFlipped); // fise flipped
            CU_ASSERT_TRUE(f1->f_ddrsPoints==1);

            flipCard(b, &cardFlipped); // fisa flipped 
            CU_ASSERT_TRUE(f2->f_ddrsPoints==2);

            flipCard(b, &cardFlipped); // the flipped
            CU_ASSERT_TRUE(f1->f_ddrsPoints==0);

            flipCard(b, &cardFlipped); // ecocup flipped
            //Nothing to test
                
            flipCard(b, &cardFlipped); // dimitri_watel flipped
            CU_ASSERT_TRUE(f2->f_ddrsPoints==5);

            flipCard(b, &cardFlipped); // ecologiie flipped
            CU_ASSERT_TRUE(f1->f_ddrsPoints==2);
             
            //end round 1: Capitaliste Win
            break;
        case 3:

            CU_ASSERT_EQUAL(getNbRoundWin(f1),1);
            CU_ASSERT_EQUAL(getNbRoundWin(f2),1); // Check that Communiste won round 2

            //round 3: Capitaliste starting

            CU_ASSERT_EQUAL(getFactionName(f1),"Capitaliste");
            CU_ASSERT_EQUAL(getFactionName(f2),"Communiste");


            card djibril_aurelien_dembele_cabot = const_card("Djibril_Aurelien_Dembele_Cabot", "S'il y a plus de 3 cartes retournées sur la ligne de cette carte, la faction qui a posé cette carte gagne 5 points DDRS.", Djibril_Aurelien_Dembele_Cabot, 1);
            card laurent_prevel = const_card("Laurent_Prevel", "Si Laurent Prével est la dernière carte retournée du plateau, la faction qui a posé cette carte gagne la manche, quel que soit le nombre de points DDRS des deux factions.", Laurent_Prevel, 1);
            setCard_hand(f1->f_hand,fise,0);
            setCard_hand(f1->f_hand,fise,1);
            setCard_hand(f1->f_hand,djibril_aurelien_dembele_cabot,2);
            setCard_hand(f1->f_hand,laurent_prevel,3);

            
            card cafe = const_card("Cafe", "Supprimez toutes les cartes Thé et Alcool retournées sur le plateau. Si une carte Ecocup est retournée sur le plateau, la faction qui a posé cette carte gagne 1 point DDRS. Sinon elle perd 1 point DDRS.", Cafe, 3);
            card julien_forest = const_card("Julien_Forest", "La faction qui a posé cette carte gagne 6 points DDRS par carte FISE retournée sur le plateau si au moins une carte Café est retournée sur le plateau.", Julien_Forest, 1);
            card guillaume_burel = const_card("Guillaume_Burel", "Si la faction adverse de celle qui a posé cette carte a plus de points DDRS, la seconde lui vole 3 points DDRS.", Guillaume_Burel, 1);
            setCard_hand(f2->f_hand,cafe,0);
            setCard_hand(f2->f_hand,julien_forest,1);
            setCard_hand(f2->f_hand,ecocup,2);
            setCard_hand(f2->f_hand,guillaume_burel,3);

            putDownCard(b,getCard_hand(f1->f_hand,0),f1,0,0); // fise put
            putDownCard(b,getCard_hand(f2->f_hand,0),f2,1,0); // cafe put

            putDownCard(b,getCard_hand(f1->f_hand,1),f1,0,1); // fise put
            putDownCard(b,getCard_hand(f2->f_hand,1),f2,1,-1); // julien_forest put

            putDownCard(b,getCard_hand(f1->f_hand,2),f1,2,0); // djibril_aurelien_dembele_cabot put
            putDownCard(b,getCard_hand(f2->f_hand,2),f2,-1,1); // ecocup put

            putDownCard(b,getCard_hand(f1->f_hand,3),f1,1,-2); // laurent_prevel put
            putDownCard(b,getCard_hand(f2->f_hand,3),f2,2,-1); // guillaume_burel put


            //reveal round 3
            flipCard(b, &cardFlipped); // ecocup flipped
            CU_ASSERT_TRUE(f2->f_ddrsPoints==0);

            flipCard(b, &cardFlipped); // fise flipped
            CU_ASSERT_TRUE(f1->f_ddrsPoints==1);

            flipCard(b, &cardFlipped); // fise flipped
            CU_ASSERT_TRUE(f1->f_ddrsPoints==2);

            flipCard(b, &cardFlipped); // cafe flipped
            CU_ASSERT_TRUE(f2->f_ddrsPoints==0);

            flipCard(b, &cardFlipped); // djibril_aurelien_dembele_cabot flipped
            CU_ASSERT_TRUE(f1->f_ddrsPoints==7);

            flipCard(b, &cardFlipped); // julien_forest flipped
            CU_ASSERT_TRUE(f2->f_ddrsPoints==12);

            flipCard(b, &cardFlipped); // guillaume_burel flipped
            CU_ASSERT_TRUE(f2->f_ddrsPoints==12);

            flipCard(b, &cardFlipped); // laurent_prevel flipped
            CU_ASSERT_TRUE(f1->f_ddrsPoints>=999);

            break;
        default:
            ; // a declaration can't follow "default" in the c99 standart, only statement : ";" is an empty statement 
            faction tmp;
            tmp = f1;
            f1 = f2;
            f2 = tmp;


            CU_ASSERT_EQUAL(getNbRoundWin(f1),1); // Check that Capitaliste won round 1
            CU_ASSERT_EQUAL(getNbRoundWin(f2),0);


            //round 2: Capitaliste starting

            CU_ASSERT_EQUAL(getFactionName(f1),"Capitaliste");
            CU_ASSERT_EQUAL(getFactionName(f2),"Communiste");


            card vitera_y = const_card("Vitera_Y", "La faction qui a le moins de points DDRS gagne 3 points DDRS.", Vitera_Y, 1);
            card jonas_senizergues = const_card("Jonas_Senizergues", "Supprimez toutes les cartes Heures supplémentaires retournées du plateau.", Jonas_Senizergues, 1);
            setCard_hand(f1->f_hand,fise,0);
            setCard_hand(f1->f_hand,vitera_y,1);
            setCard_hand(f1->f_hand,jonas_senizergues,2);


            card heures_supplementaires = const_card("Heures_supplementaires", "La faction adverse de celle qui a posé cette carte perd 3 points DDRS par carte Heures supplémentaires retournée sur le plateau (y compris celle-ci).", Heures_supplementaires, 1);
            card christophe_mouilleron = const_card("Christophe_Mouilleron", "Si la carte Heures supplémentaires est retournée sur le plateau, supprimez toutes les cartes retournées du plateau sauf les cartes Christophe Mouilleron et Heures supplémentaires.", Christophe_Mouilleron, 1);
            setCard_hand(f2->f_hand,fisa,0);
            setCard_hand(f2->f_hand,heures_supplementaires,1);
            setCard_hand(f2->f_hand,christophe_mouilleron,2);


            
            putDownCard(b,getCard_hand(f1->f_hand,0),f1,0,0); // fise put
            putDownCard(b,getCard_hand(f2->f_hand,0),f2,0,1); // fisa put
            
            putDownCard(b,getCard_hand(f1->f_hand,1),f1,1,0); // vitera_y put
            putDownCard(b,getCard_hand(f2->f_hand,1),f2,0,-1); // heures_supplementaires put
            
            putDownCard(b,getCard_hand(f1->f_hand,2),f1,1,-1); // jonas_senizergues put
            putDownCard(b,getCard_hand(f2->f_hand,2),f2,0,-2); // christophe_mouilleron put
            

            //reveal round 2
            
            flipCard(b, &cardFlipped); // fisa flipped
            CU_ASSERT_TRUE(f2->f_ddrsPoints==0);

            flipCard(b, &cardFlipped); // fise flipped
            CU_ASSERT_TRUE(f1->f_ddrsPoints==1);
            
            flipCard(b, &cardFlipped); // vitera_y flipped
            CU_ASSERT_TRUE(f2->f_ddrsPoints==3);
            
            flipCard(b, &cardFlipped); // heures_supplementaires flipped
            CU_ASSERT_TRUE(f1->f_ddrsPoints==0);
            
            flipCard(b, &cardFlipped); // christophe_mouilleron flipped
            CU_ASSERT_PTR_NULL(getCard_board2D(b->b2D,0,0)); // check that fise is removed
            CU_ASSERT_PTR_NULL(getCard_board2D(b->b2D,0,-1)); // check that fisa is removed
            CU_ASSERT_PTR_NULL(getCard_board2D(b->b2D,1,-1)); // check that vitera_y is removed

            flipCard(b, &cardFlipped); // jonas_senizergues flipped
            CU_ASSERT_PTR_NULL(getCard_board2D(b->b2D,0,-2)); // check that heures_supplementaires is removed


            //end round 2: Communiste Win
            break;
        }
    }    

    CU_ASSERT_EQUAL(getNbRoundWin(f1),2); // Check that Capitaliste won round 3



        
}

int init_suite(){
    return 0;
}

int clean_suite(){
    return 0;
}


int main(){
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry()){
      return CU_get_error();
   }


    /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_Of_Initialisation",init_suite,clean_suite);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

    /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "test of card creation", cardCreationTest)) || 
        (NULL == CU_add_test(pSuite, "test of deck use", deckTest))  ||
        (NULL == CU_add_test(pSuite, "test of hand use", handTest))   ||
        (NULL == CU_add_test(pSuite, "test of faction", factionTest)) ||
        (NULL == CU_add_test(pSuite, "test of the game and cards' effects", gameTest))
        )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

    /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

















