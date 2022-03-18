// #include <stdio.h>
// #include "board.h"
// #include "cards.h"
// #include "faction.h"
// #include "interface.h"


//pas oublie les ifdef def endif dans les .h
//et code entierement en anglais

int main(int argc, char const *argv[])
{
    /* 
    plateau.h
    Fonction créer plateau createBoard
    Fonction libérer plateau freeBoard
    Fonction nouvelle manche newRound
    Fonction liste des factions listFactions
    Fonction poser carte putDownCard
    Fonction retourner carte flipCard
    
    carte.h et faction.h
    Fonction remélangeage utilisé hasTheDeckBeenShuffled
    Fonction remélanger reshuffleDeck
    Fonction vider sa main discardHand
    Fonction mélanger pioche shuffleDeck
    Fonction piocher drawCard

    interface.h
    Fonction afficher plateau showBoard
    Fonction afficher main showHand
    Fonction demander remélanger askReshuffle
    Fonction demander carte askCardWantToPlay
    Fonction demander position askWhereWantToPlaceCard
    Fonction afficher effet showCardEffect
    Fonction afficher vainqueur showWinner

     */

    //init
    Fonction créer plateau

    conterMancheNumber = 0;
    while (1)
    {
        if (!(Fonction nouvelle manche)) //le jeu est fini
        {
            break; //on sort du while
        }
        else conterMancheNumber++;

        switch (conterMancheNumber)
        {
        case 1:
            fonction de main.c pour choisir quel faction est la premiere avec un rand (on utilise Fonction liste des factions)
            Fonction mélanger pioche pour faction1
            Fonction mélanger pioche pour faction2
            break;
        case 3:
            fonction de main.c pour choisir quel faction est la premiere avec un rand (on utilise Fonction liste des factions)
            break;
        default:
            inverser premiere faction et deuxieme faction //attention quand on va dire qui sera le vainqueur a pas se tromper 
            break;
        }

        //phase 1

        //drawphase
        Fonction piocher premiere faction
        Fonction afficher main premiere faction
        if (!(Fonction remélangeage utilisé premiere faction) && (Fonction demander remélanger premiere faction))
        {
            Fonction vider sa main premiere faction
            Fonction remélanger
            Fonction piocher premiere faction
        }

        Fonction piocher deuxieme faction
        Fonction afficher main deuxieme faction
        if (!(Fonction remélangeage utilisé deuxieme faction) && (Fonction demander remélanger deuxieme faction))
        {
            Fonction vider sa main premiere faction
            Fonction remélanger deuxieme faction
            Fonction piocher deuxieme faction
        }
        
        //placing cards
        Fonction afficher main faction1
        Fonction demander carte faction1
        Fonction poser carte faction1 au centre
        //on affiche pas plateau pour faction1 car ca carte sera forcement au centre

        Fonction afficher main faction2
        Fonction demander carte faction2
        Fonction afficher plateau
        Fonction demander position
        Fonction poser carte faction2

        for (int i = 0; i < 7; i++)
        {
            Fonction afficher main faction1
            Fonction demander carte faction1
            Fonction afficher plateau
            Fonction demander position
            Fonction poser carte faction1

            Fonction afficher main faction2
            Fonction demander carte faction2
            Fonction afficher plateau
            Fonction demander position
            Fonction poser carte faction2
        }
        

        //reveal
        Fonction afficher plateau
        while (Fonction retourner carte(pointeurDescriptionEffect))
        {
            Fonction afficher plateau
            Fonction afficher effet pointeurDescriptionEffect
        }
        

        //qui a gagne la manche


    }
    //afficher vainqueur
    Fonction afficher vainqueur

    //end
    Fonction libérer plateau

    return 0;
}