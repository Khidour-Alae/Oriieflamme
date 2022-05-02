/**
 * \file main.c
 * 
 * This file is our main file, it makes the game work as it is described in the subject : http://projet-info.pedago.ensiie.fr/subject
 * 
 * */

#include "../headers/board.h"
#include "../headers/cards.h"
#include "../headers/faction.h"
#include "../headers/interface.h"

#include <stdlib.h> //for rand()
#include <time.h> //for time()
#include <stdio.h>
#include <unistd.h> //for sleep()

int main()
{
    //init
    system("clear");
    srand(time(NULL));

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
            if (rand() % 2)
            {
                faction tmp;
                tmp = f1;
                f1 = f2;
                f2 = tmp;
            }
            shuffleDeck(f1);
            shuffleDeck(f2);
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
        drawCards(f1);
        showHand(f1);
        if (!(hasTheDeckBeenShuffled(f1)) && (askReshuffle(f1)))
        {
            discardHand(f1);
            reshuffleDeck(f1);
            drawCards(f1);
            showHand(f1); sleep(1);
        }
        system("clear");

        drawCards(f2);
        showHand(f2);
        if (!(hasTheDeckBeenShuffled(f2)) && (askReshuffle(f2)))
        {
            discardHand(f2);
            reshuffleDeck(f2);
            drawCards(f2);
            showHand(f2); sleep(1);
        }
        system("clear");
        
        //placing cards
        int x,y; card cardToPlay;

        showHand(f1); sleep(1);
        cardToPlay = askCardWantToPlay(f1);
        putDownCard(b,cardToPlay,f1,0,0); //the first card is placed in the center of board
        printf("Votre carte a été placé au centre du plateau\n\n"); sleep(2);

        system("clear");
        showHand(f2); sleep(1);
        cardToPlay = askCardWantToPlay(f2);
        showBoard(b);
        askWhereWantToPlaceCard(cardToPlay,&x,&y);
        putDownCard(b,cardToPlay,f2,x,y);
        printf("Votre carte a été placé\n\n"); 
        showBoard(b); sleep(2);

        // for (int i = 0; i < 7; i++)
        // {
        //     system("clear");
        //     showHand(f1); sleep(1);
        //     cardToPlay = askCardWantToPlay(f1);
        //     showBoard(b);
        //     askWhereWantToPlaceCard(cardToPlay,&x,&y);
        //     putDownCard(b,cardToPlay,f1,x,y);
        //     printf("Votre carte a été placé\n\n"); 
        //     showBoard(b); sleep(2);

        //     system("clear");
        //     showHand(f2); sleep(1);
        //     cardToPlay = askCardWantToPlay(f2);
        //     showBoard(b);
        //     askWhereWantToPlaceCard(cardToPlay,&x,&y);
        //     putDownCard(b,cardToPlay,f2,x,y);
        //     printf("Votre carte a été placé\n\n"); 
        //     showBoard(b); sleep(2);
        // }


        //reveal
        system("clear");
        showBoard(b); sleep(2);
        card cardFlipped;
        while (flipCard(b, &cardFlipped))
        {
            showCardEffect(cardFlipped); sleep(4);
            showBoard(b); sleep(2);
        }
        sleep(3);
        system("clear");
    }
    
    system("clear");
    showWinner(f1,f2);

    freeBoard(b);
    return 0;
}