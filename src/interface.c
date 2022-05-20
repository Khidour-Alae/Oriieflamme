#include "../headers/interface.h"
#include "../headers/faction.h"
#include "../headers/cards.h"
#include "../headers/board.h"
#include "../headers/structure.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>


void initializeSDL(){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *image = NULL;
    SDL_Surface *tmp = NULL;
    tmp = SDL_LoadBMP("Cards/carteFC.bmp");
    int statut = EXIT_FAILURE;
    SDL_Color black = {0, 0, 0, 255};

    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("Oriieflamme", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1850, 900, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }




    if (NULL == tmp){
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        goto Quit;
    }   
}


void showBoard(board b) {
    int xmin,ymin,xmax,ymax;
    getBoundingBoxOfTheBoardToPrint(b,&xmin,&ymin,&xmax,&ymax);

    printf("--- Voici le plateau ---\n\n");
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
    printf("\n\n");
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
    int f1_pts = getNbRoundWin(f1);
    int f2_pts = getNbRoundWin(f2);
    
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
    printf(" ---\n\n");
    printf("Voulez vous vider votre main, mélanger votre pioche et repiocher une main?\nVous ne pourrez effectuer cette action qu'une fois au cours de la partie.\n\n");


    printf("-Oui: [o]\n-Non: [n]\n\n");
    printf( "Veuillez saisir votre réponse : " );
    char buffer[150];
    fgets(buffer, 150, stdin);
    if (buffer[0] == 'o' || (buffer[0] == 'n'))
    {
        return (buffer[0] == 'o');
    }
    while (1)
    {
        printf("Mauvais input, veuillez réessayer\n\n");
        printf("-Oui: [o]\n-Non: [n]\n\n");
        printf( "Veuillez saisir votre réponse : " );
        char buffer[150];
        fgets(buffer, 150, stdin);
        if (buffer[0] == 'o' || (buffer[0] == 'n'))
        {
            return (buffer[0] == 'o');
        }
    }

}

void showWinner(faction f1,faction f2) {
    int f1_pts = getNbRoundWin(f1);
    int f2_pts = getNbRoundWin(f2);

    if (f1_pts > f2_pts) {
        printf("Félicitations à la faction ");
        print_nom_faction(f1);
        printf(" qui remporte la partie !\n\n");
        print_pts(f1, f2);
    }
    else 
    {
        printf("Félicitations à la faction ");
        print_nom_faction(f2);
        printf(" qui remporte la partie !\n\n");
        print_pts(f1, f2);
    }

}


void print_cardName(card c){
    if (c == NULL)
    {
        printf("Error, you'are trying to print the name of a NULL card");
        exit(1);
    }
    
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
    printf(" ---\n\n  Voici votre main\n\n");

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
    printf(" ||\n\n");
}

card askCardWantToPlay(faction f) {
    printf("Quelle carte souhaitez-vous jouer ? (le numéro) \n");
    int index; char buffer[150];
    while (1)
    {
        fgets(buffer, 150, stdin);
        sscanf(buffer,"%i", &index);
        printf("Vous avez choisi de jouer la carte %i\n\n",index);
        if (index >= 0 && index < NB_CARDS_IN_HAND && getCard_hand(getHand(f), index) != NULL)
        {
            card res = getCard_hand(getHand(f), index);
            discardCard_hand(getHand(f),index); //deletes the card from the hand
            return res;
        }
        else
        {
            printf("Vous avez entré un mauvais numéro, essayez de nouveau\n");
        }
    }
}

void askWhereWantToPlaceCard(board b, card c, int *x, int *y) {
    char buffer[150];
    printf("Où souhaitez-vous jouer la carte ");
    print_cardName(c);
    printf(" ? (Coordonnées x y)\n");
    printf("On rappelle qu'elle doit être jouée à côté d'une autre carte.\n");
    printf("x : ");
    fgets(buffer, 150, stdin);
    sscanf(buffer,"%i", x);
    printf("y : ");
    fgets(buffer, 150, stdin);
    sscanf(buffer,"%i", y);
    printf("\n");
    while (!isValidPlace(b,*x,*y))
    {
        printf("Vous ne pouvez pas placer une carte en (%i,%i). Veuillez réessayer.\n",*x,*y);
        printf("Où souhaitez-vous jouer la carte ");
        print_cardName(c);
        printf(" ? (Coordonnées x y)\n");
        printf("On rappelle qu'elle doit être jouée à côté d'une autre carte.\n");
        printf("x : ");
        fgets(buffer, 150, stdin);
        sscanf(buffer,"%i", x);
        printf("y : ");
        fgets(buffer, 150, stdin);
        sscanf(buffer,"%i", y);
        printf("\n");
    }   
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
    printf("\n\n");
}

void printRoundWinner(faction f, int round)
{
    printf("\nLa faction ");
    print_nom_faction(f);
    printf(" a gagné le round %i\n", round);
}

void printDdrsPts(faction f1, faction f2){
    int f1_pts = getFactionDdrsPoints(f1);
    int f2_pts = getFactionDdrsPoints(f2);
    
    printf("Faction : ");
    print_nom_faction(f1);
    printf("\n");
    printf("Score:");
    printf(": %i\n\n", f1_pts);

    printf("Faction : ");
    print_nom_faction(f2);
    printf("\n");
    printf("Score:");
    printf(": %i\n\n", f2_pts);
}