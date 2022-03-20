/**
 * \file main.c
 * 
 * This file is our main file, it makes the game work as it is described in the subject : http://projet-info.pedago.ensiie.fr/subject
 * 
 * */

#include "board.h"
#include "cards.h"
#include "faction.h"
#include "interface.h"

#include <stdlib.h> //for rand()
#include <time.h> //for time()

int main()
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
                faction tmp;
                tmp = f1;
                f1 = f2;
                f2 = tmp;
            }
            shuffleDeck(&f1);
            shuffleDeck(&f2);
            break;
        case 3:
            if (rand() % 2)
            {
                faction tmp;
                tmp = f1;
                f1 = f2;
                f2 = tmp;
            }
            break;
        default:
            ; // a declaration can't follow "default" in the c99 standart, only statement : ";" is an empty statement 
            faction tmp;
            tmp = f1;
            f1 = f2;
            f2 = tmp;
            break;
        }

        //phase 1

        //drawphase
        drawCard(&f1);
        showHand(f1);
        if (!(hasTheDeckBeenShuffled(f1)) && (askReshuffle(f1)))
        {
            discardHand(&f1);
            reshuffleDeck(&f1);
            drawCard(&f1);
        }

        drawCard(&f2);
        showHand(f2);
        if (!(hasTheDeckBeenShuffled(f2)) && (askReshuffle(f2)))
        {
            discardHand(&f2);
            reshuffleDeck(&f2);
            drawCard(&f2);
        }
        
        //placing cards
        int pos; card cardToPlay;

        showHand(f1);
        cardToPlay = askCardWantToPlay(f1);
        pos = getCenterOfBoard(b);
        putDownCard(&b,cardToPlay,f1,pos); //the first card is placed in the center of board

        showHand(f2);
        cardToPlay = askCardWantToPlay(f2);
        showBoard(b);
        pos = askWhereWantToPlaceCard(cardToPlay);
        putDownCard(&b,cardToPlay,f2,pos);

        for (int i = 0; i < 7; i++)
        {
            showHand(f1);
            cardToPlay = askCardWantToPlay(f1);
            showBoard(b);
            pos = askWhereWantToPlaceCard(cardToPlay);
            putDownCard(&b,cardToPlay,f1,pos);

            showHand(f2);
            cardToPlay = askCardWantToPlay(f2);
            showBoard(b);
            pos = askWhereWantToPlaceCard(cardToPlay);
            putDownCard(&b,cardToPlay,f2,pos);
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

    freeBoard(&b);

    return 0;
}