#include "board.h"
#include "cards.h"
#include "faction.h"
#include "interface.h"

#include <stdlib.h> //for rand()

int main(int argc, char const *argv[])
{
    //init
    srand(time(NULL));

    board b; faction* f;
    faction f1; faction f2;
    b = createBoard();
    f = listFactions(b);
    f1 = f[0]; f2 = f[1];

    int roundCounter = 0;
    while (1)
    {
        if (!(newRound(roundCounter,f1,f2))) //if game finished we leave the wile loop
        {
            break;
        }
        else roundCounter++;

        switch (roundCounter)
        {
        case 1:
            if (rand() % 2)
            {
                faction tmp = f1;
                f1 = f2;
                f2 = tmp;
            }
            shuffleDeck(f1);
            shuffleDeck(f2);
            break;
        case 3:
            if (rand() % 2)
            {
                faction tmp = f1;
                f1 = f2;
                f2 = tmp;
            }
            break;
        default:
            faction tmp = f1;
            f1 = f2;
            f2 = tmp;
            break;
        }

        //phase 1

        //drawphase
        drawCard(f1);
        showHand(f1);
        if (!(hasTheDeckBeenShuffled(f1)) && (askReshuffle(f1)))
        {
            discardHand(f1);
            reshuffleDeck(f1);
            drawCard(f1);
        }

        drawCard(f2);
        showHand(f2);
        if (!(hasTheDeckBeenShuffled(f1)) && (askReshuffle(f2)))
        {
            discardHand(f2);
            reshuffleDeck(f2);
            drawCard(f2);
        }
        
        //placing cards
        int pos; card cardToPlay;

        showHand(f1);
        cardToPlay = askCardWantToPlay(f1);
        pos = getCenterOfBoard(b);
        putDownCard(cardToPlay,pos,f1); //the first card is placed in the center of board

        showHand(f2);
        cardToPlay = askCardWantToPlay(f2);
        showBoard(b);
        pos = askWhereWantToPlaceCard();
        putDownCard(cardToPlay,pos,f2);

        for (int i = 0; i < 7; i++)
        {
            showHand(f1);
            askCardWantToPlay(f1);
            showBoard(b);
            pos = askWhereWantToPlaceCard();
            putDownCard(cardToPlay,pos,f1);

            showHand(f2);
            cardToPlay = askCardWantToPlay(f2);
            showBoard(b);
            pos = askWhereWantToPlaceCard();
            putDownCard(cardToPlay,pos,f2);
        }
        

        //reveal
        showBoard(b);
        card cardFlipped;
        while (flipCard(&cardFlipped))
        {
            showBoard(b);
            showCardEffect(cardFlipped);
        }
    }
    
    showWinner(f1,f2);

    freeBoard(b);

    return 0;
}