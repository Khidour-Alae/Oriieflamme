#include "../headers/interface.h"
#include "../headers/faction.h"
#include "../headers/cards.h"
#include "../headers/board.h"
#include "../headers/structure.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void showBoard(board b) {
    int xmin,ymin,xmax,ymax;
    getBoundingBoxOfTheBoardToPrint(b,&xmin,&ymin,&xmax,&ymax);

    
    for (int y = ymax; y >= ymin; y--)
    {
        printf("%3i | ",y);
        for (int x = xmin; x < xmax+1; x++)
        {
            if (isCardPlacedAt(b,x,y))
            {
                if (isFlipped(b, x, y))
                {
                    printf(" O  | ");
                }
                else 
                {
                    printf(" X  | ");
                }
                
            }
            else
            {
                printf("    | ");
            }   
        }
        printf("\n");
    }
    printf("    | ");
    for (int x = xmin; x < xmax+1; x++)
    {
        printf("%3i | ",x);
    }
    printf("\n");
}

void print_nom_faction(faction f){
    int i = 0;
    char *fname;
    fname = getFactionName(f);
    while (fname[i] != '\0'){
        printf("%c", fname[i]);
        i++;
    }
}

void print_pts(faction f1, faction f2){
    int f1_pts = getFactionDdrsPoints(f1);
    int f2_pts = getFactionDdrsPoints(f2);
    
    printf("Faction : ");
    print_nom_faction(f1);
    printf("\n");
    printf("Scores:");
    printf(": %i\n\n", f1_pts);

    printf("Faction : ");
    print_nom_faction(f2);
    printf("\n");
    printf("Scores:");
    printf(": %i\n\n", f2_pts);
}

int askReshuffle(faction f){
    printf("--- Faction ");
    print_nom_faction(f);
    printf(" ---\n");
    printf("Voulez vous vider votre main, mélanger votre pioche et repiocher une main?\nVous ne pourrez effectuez cette action qu'une fois au cours de la partie.\n\n");
    printf("-Oui: [o]\n-Non: [n]\n\n");

    printf( "Veuillez saisir votre réponse : " );
    char buffer[150];
    fgets(buffer, 150, stdin);

    return (buffer[0] == 'o');
}

void showWinner(faction f1,faction f2) {
    int f1_pts = getFactionDdrsPoints(f1);
    int f2_pts = getFactionDdrsPoints(f2);

    system("clear");
    if (f1_pts > f2_pts) {
        printf("Félicitations à la faction ");
        print_nom_faction(f1);
        printf(" qui remporte la partie !\n\n");
        print_pts(f1, f2);
    }
    else if (f1_pts < f2_pts) {
        printf("Félicitations à la faction ");
        print_nom_faction(f2);
        printf(" qui remporte la partie !\n\n");
        print_pts(f1, f2);
    }
    else {
        printf("Egalité, bravo aux deux factions pour cette palpitante partie !\n\n");
        print_pts(f1, f2);
    }
}


void print_cardName(card c){
    int i = 0;
    char *cname;
    cname = getCardName(c);
    while (cname[i] != '\0'){
        printf("%c", cname[i]);
        i++;
    }
}

void showHand(faction f) {
    //system("clear");
    printf("--- Faction ");
    print_nom_faction(f);
    printf(" ---\nVoici votre main\n");

    hand h = getHand(f);
    card cardToShow;

    printf("|| ");
    for (int i = 0; i < NB_CARDS_IN_HAND - 1; i++)
    {
        cardToShow = getCard_hand(h,i);
        if (cardToShow != NULL)
        {
            print_cardName(cardToShow);
            printf(" (%i) | ",i);
        }
    }
    cardToShow = getCard_hand(h,NB_CARDS_IN_HAND - 1);
    if (cardToShow != NULL)
    {
        print_cardName(cardToShow);
        printf(" (%i)",NB_CARDS_IN_HAND - 1);
    }
    printf(" ||\n");
}

card askCardWantToPlay(faction f) {
    printf("Qu'elle carte souhaitez-vous jouer ? (le numéro) \n");
    int index;
    while (1)
    {
        scanf("%i", &index);
        printf("Vous avez choisie de jouer la carte %i\n",index);
        if (index >= 0 && index < NB_CARDS_IN_HAND)
        {
            card res = getCard_hand(getHand(f), index);
            discardCard_hand(getHand(f),index); //deletes the card from the hand
            return res;
        }
        else
        {
            printf("Vous avez entrer un mauvais numéro, essayez de nouveau\n");
        }
    }
}

void askWhereWantToPlaceCard(card c, int *x, int *y) {
    printf("Où souhaitez-vous jouer la carte ");
    print_cardName(c);
    printf(" ? (Coordonnées x y)\n");
    printf("On rapelle quel doit être jouer à côté d'une autre carte.\n");
    printf("x : ");
    scanf("%i", x);
    printf("\n");
    printf("y : ");
    scanf("%i", y);
    printf("\n");
}

void showCardEffect(card c) {
    printf("Voici l'effet de la carte ");
    print_cardName(c);
    printf(" :\n");

    int i = 0;
    char *cdes;
    cdes = getCardEffectDescription(c);
    while (cdes[i] != '\0'){
        printf("%c", cdes[i]);
        i++;
    }
    printf("\n");
}