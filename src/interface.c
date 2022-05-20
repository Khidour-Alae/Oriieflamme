#include "../headers/interface.h"
#include "../headers/faction.h"
#include "../headers/cards.h"
#include "../headers/board.h"
#include "../headers/structure.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *image = NULL;
SDL_Surface *tmp = NULL;
//image = SDL_LoadBMP("Cards/carteFC.bmp");
int statut = EXIT_FAILURE;
SDL_Color black = {0, 0, 0, 255};
SDL_Rect playButton = {725, 400, 400, 100};




// Chiffres //


// 0
void printZero(int x, int y){
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
    SDL_RenderDrawLine(renderer, x+4, y, x+13, y);
    SDL_RenderDrawLine(renderer, x+4, y+1, x+13, y+1);

    SDL_RenderDrawLine(renderer, x+14, y+2, x+14, y+17);
    SDL_RenderDrawLine(renderer, x+15, y+2, x+15, y+17);

    SDL_RenderDrawLine(renderer, x+4, y+18, x+13, y+18);
    SDL_RenderDrawLine(renderer, x+4, y+19, x+13, y+19);

    SDL_RenderDrawLine(renderer, x+2, y+2, x+2, y+17);
    SDL_RenderDrawLine(renderer, x+3, y+2, x+3, y+17);
}



// 1
void printUn(int x, int y){
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
    SDL_RenderDrawLine(renderer, x+6, y+5, x+14, y);
    SDL_RenderDrawLine(renderer, x+6, y+6, x+14, y+1);

    SDL_RenderDrawLine(renderer, x+14, y+2, x+14, y+19);
    SDL_RenderDrawLine(renderer, x+15, y+1, x+15, y+19);
}



// 2
void printDeux(int x, int y){
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
    SDL_RenderDrawLine(renderer, x+4, y, x+13, y);
    SDL_RenderDrawLine(renderer, x+4, y+1, x+13, y+1);

    SDL_RenderDrawLine(renderer, x+4, y+9, x+13, y+9);
    SDL_RenderDrawLine(renderer, x+4, y+10, x+13, y+10);

    SDL_RenderDrawLine(renderer, x+14, y+2, x+14, y+8);
    SDL_RenderDrawLine(renderer, x+15, y+2, x+15, y+8);

    SDL_RenderDrawLine(renderer, x+4, y+18, x+13, y+18);
    SDL_RenderDrawLine(renderer, x+4, y+19, x+13, y+19);

    SDL_RenderDrawLine(renderer, x+2, y+11, x+2, y+17);
    SDL_RenderDrawLine(renderer, x+3, y+11, x+3, y+17);
}



// 3
void printTrois(int x, int y){
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
    SDL_RenderDrawLine(renderer, x+4, y, x+13, y);
    SDL_RenderDrawLine(renderer, x+4, y+1, x+13, y+1);

    SDL_RenderDrawLine(renderer, x+4, y+9, x+13, y+9);
    SDL_RenderDrawLine(renderer, x+4, y+10, x+13, y+10);

    SDL_RenderDrawLine(renderer, x+14, y+2, x+14, y+8);
    SDL_RenderDrawLine(renderer, x+15, y+2, x+15, y+8);

    SDL_RenderDrawLine(renderer, x+4, y+18, x+13, y+18);
    SDL_RenderDrawLine(renderer, x+4, y+19, x+13, y+19);

    SDL_RenderDrawLine(renderer, x+14, y+11, x+14, y+17);
    SDL_RenderDrawLine(renderer, x+15, y+11, x+15, y+17);
}


// 4
void printQuatre(int x, int y){
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);

    SDL_RenderDrawLine(renderer, x+4, y+9, x+13, y+9);
    SDL_RenderDrawLine(renderer, x+4, y+10, x+13, y+10);

    SDL_RenderDrawLine(renderer, x+2, y+2, x+2, y+8);
    SDL_RenderDrawLine(renderer, x+3, y+2, x+3, y+8);

    SDL_RenderDrawLine(renderer, x+14, y+2, x+14, y+17);
    SDL_RenderDrawLine(renderer, x+15, y+2, x+15, y+17);
}



// 5 
void printCinq(int x, int y){
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
    SDL_RenderDrawLine(renderer, x+4, y, x+13, y);
    SDL_RenderDrawLine(renderer, x+4, y+1, x+13, y+1);

    SDL_RenderDrawLine(renderer, x+4, y+9, x+13, y+9);
    SDL_RenderDrawLine(renderer, x+4, y+10, x+13, y+10);

    SDL_RenderDrawLine(renderer, x+2, y+2, x+2, y+8);
    SDL_RenderDrawLine(renderer, x+3, y+2, x+3, y+8);

    SDL_RenderDrawLine(renderer, x+4, y+18, x+13, y+18);
    SDL_RenderDrawLine(renderer, x+4, y+19, x+13, y+19);

    SDL_RenderDrawLine(renderer, x+14, y+11, x+14, y+17);
    SDL_RenderDrawLine(renderer, x+15, y+11, x+15, y+17);
}



// 6
void printSix(int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
    SDL_RenderDrawLine(renderer, x+4, y, x+13, y);
    SDL_RenderDrawLine(renderer, x+4, y+1, x+13, y+1);

    SDL_RenderDrawLine(renderer, x+4, y+9, x+13, y+9);
    SDL_RenderDrawLine(renderer, x+4, y+10, x+13, y+10);

    SDL_RenderDrawLine(renderer, x+2, y+2, x+2, y+8);
    SDL_RenderDrawLine(renderer, x+3, y+2, x+3, y+8);

    SDL_RenderDrawLine(renderer, x+4, y+18, x+13, y+18);
    SDL_RenderDrawLine(renderer, x+4, y+19, x+13, y+19);

    SDL_RenderDrawLine(renderer, x+14, y+11, x+14, y+17);
    SDL_RenderDrawLine(renderer, x+15, y+11, x+15, y+17);

    SDL_RenderDrawLine(renderer, x+2, y+11, x+2, y+17);
    SDL_RenderDrawLine(renderer, x+3, y+11, x+3, y+17);
}


// 7
void printSept(int x, int y){
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
    SDL_RenderDrawLine(renderer, x+4, y, x+18, y);
    SDL_RenderDrawLine(renderer, x+4, y+1, x+18, y+1);
    // SDL_RenderDrawLine(renderer, x, y+2, x+18, y+2);

    SDL_RenderDrawLine(renderer, x+18, y+2, x+8, y+20);
    SDL_RenderDrawLine(renderer, x+17, y+2, x+7, y+20);
    SDL_RenderDrawLine(renderer, x+18, y+2, x+6, y+20);
    
}


// 8
void printHuit(int x, int y){
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
    SDL_RenderDrawLine(renderer, x+4, y, x+13, y);
    SDL_RenderDrawLine(renderer, x+4, y+1, x+13, y+1);

    SDL_RenderDrawLine(renderer, x+4, y+9, x+13, y+9);
    SDL_RenderDrawLine(renderer, x+4, y+10, x+13, y+10);

    SDL_RenderDrawLine(renderer, x+2, y+2, x+2, y+8);
    SDL_RenderDrawLine(renderer, x+3, y+2, x+3, y+8);

    SDL_RenderDrawLine(renderer, x+4, y+18, x+13, y+18);
    SDL_RenderDrawLine(renderer, x+4, y+19, x+13, y+19);

    SDL_RenderDrawLine(renderer, x+14, y+11, x+14, y+17);
    SDL_RenderDrawLine(renderer, x+15, y+11, x+15, y+17);

    SDL_RenderDrawLine(renderer, x+2, y+11, x+2, y+17);
    SDL_RenderDrawLine(renderer, x+3, y+11, x+3, y+17);

    SDL_RenderDrawLine(renderer, x+14, y+2, x+14, y+8);
    SDL_RenderDrawLine(renderer, x+15, y+2, x+15, y+8);
}




// 9
void printNeuf(int x, int y){
    SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
    SDL_RenderDrawLine(renderer, x+4, y, x+13, y);
    SDL_RenderDrawLine(renderer, x+4, y+1, x+13, y+1);

    SDL_RenderDrawLine(renderer, x+4, y+9, x+13, y+9);
    SDL_RenderDrawLine(renderer, x+4, y+10, x+13, y+10);

    SDL_RenderDrawLine(renderer, x+2, y+2, x+2, y+8);
    SDL_RenderDrawLine(renderer, x+3, y+2, x+3, y+8);

    SDL_RenderDrawLine(renderer, x+4, y+18, x+13, y+18);
    SDL_RenderDrawLine(renderer, x+4, y+19, x+13, y+19);

    SDL_RenderDrawLine(renderer, x+14, y+11, x+14, y+17);
    SDL_RenderDrawLine(renderer, x+15, y+11, x+15, y+17);

    SDL_RenderDrawLine(renderer, x+14, y+2, x+14, y+8);
    SDL_RenderDrawLine(renderer, x+15, y+2, x+15, y+8);
}




void print_score(int n, int x, int y){
    int chiffre[9];
    int i=0;
    int mod = 0;
    while (n != 0){
        mod = n%10;
        chiffre[i] = mod;
        n -= mod;
        n = n/10;
        i++;
    }
    int j = 0;
    int l = 0;
    for (j=i-1; j>=0; j--){
        switch(chiffre[j]){
            case 1 : printUn(x + l, y); break;
            case 2 : printDeux(x + l, y); break;
            case 3 : printTrois(x + l, y); break;
            case 4 : printQuatre(x + l, y); break;
            case 5 : printCinq(x + l, y); break;
            case 6 : printSix(x + l, y); break;
            case 7 : printSept(x + l, y); break;
            case 8 : printHuit(x + l, y); break;
            case 9 : printNeuf(x + l, y); break;
        }
        l+=20;
    }
}









void quitSDL(){
    statut = EXIT_SUCCESS;
    //SDL_FreeSurface(tmp);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}

void initializeSDL(){
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        quitSDL();;
    }
    window = SDL_CreateWindow("Oriieflamme", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1850, 900, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        quitSDL();
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        quitSDL();
    }

    //  if (NULL == tmp){
    //     fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
    //     quitSDL();
    // }  
}


void afficheMenu(){
    if(0 != SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a))
        {
            fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            quitSDL();;
        }
        
        if(0 != SDL_RenderClear(renderer))
        {
            fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            quitSDL();
        }

        SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
        SDL_RenderFillRect(renderer, &playButton);
        mouseOver(renderer, playButton, 725, 400);
        SDL_RenderPresent(renderer);
}



void afficheJeu(){
    if(0 != SDL_SetRenderDrawColor(renderer, 46, 134, 193, 255))
        {
            fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            quitSDL();
        }
        
        if(0 != SDL_RenderClear(renderer))
        {
            fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            quitSDL();
        }
        

    // SDL_Surface *tmp = SDL_LoadBMP("Cards/oriieflamme3.bmp");
    // if (NULL == tmp)
    // {
    //     fprintf(stderr, "ERREUR LOADBMP");
    // }
    // SDL_Texture *image = SDL_CreateTextureFromSurface(renderer, tmp);
    // SDL_Rect dst = {775, -50, 600, 600};
    // SDL_RenderCopy(renderer, image, NULL, &dst); 
        
    SDL_RenderPresent(renderer);

}


int mouseOver(SDL_Renderer *renderer, SDL_Rect rect, int x, int y){
    int xp, yp;
    SDL_GetMouseState(&xp, &yp);
    if (xp < x + 100 && xp > x && yp > y && yp < y + 100) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect); 
        return 1;
    }
    return 0;
}


int clicked(SDL_Renderer *renderer, SDL_Rect rect, int x, int y, int *menu, int *jeu){
    if (mouseOver(renderer, rect, x, y)){
        *menu = 0;
        *jeu = 1;
    }
}

void afficheImage(const char *file, int x, int y, int size)
{
    tmp = SDL_LoadBMP(file);
    if (NULL == tmp)
    {
        fprintf(stderr, "ERREUR LOADBMP");
    }
    image = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_Rect dst = {x, y, size, size};
    SDL_RenderCopy(renderer, image, NULL, &dst);
    SDL_RenderPresent(renderer);
}


// void reshuffle(){
//     tmp = SDL_LoadBMP("Cards/askReshuffle.bmp");
//     SDL_Surface oui1 = 
//     SDL_Surface oui
//     if (NULL == tmp)
//     {
//         fprintf(stderr, "ERREUR LOADBMP");
//     }
//     image = SDL_CreateTextureFromSurface(renderer, tmp);
//     SDL_Rect dst = {15, 15, 200, 200};
//     SDL_RenderCopy(renderer, image, NULL, &dst);
//     SDL_RenderPresent(renderer);
// }
 

/* 
0 si rep = non
1 si rep = oui
2 sinon
*/
int askReshuffleV2(int xs, int ys, int click)
{
    int b1 = 0; // Curseur sur bouton 1
    int b2 = 0; // Curseur sur bouton 2
    SDL_RenderClear(renderer);
    afficheJeu();
    afficheImage("Cards/askReshuffle.bmp", 600, 100, 702, 419);
    if (xs > 100 && xs < 450 && ys > 500 && ys < 602) b1 = 1;
    if (b1) afficheImage("Cards/oui1.bmp", 100, 500, 350, 102);
    else afficheImage("Cards/oui0.bmp", 100, 500, 350, 102);
    if (xs > 1400 && xs < 1750 && ys > 500 && ys < 602) b2 = 0;
    if (b2) afficheImage("Cards/non1.bmp", 1400, 500, 350, 102);
    else afficheImage("Cards/non0.bmp", 1400, 500, 350, 102);
    if (b1&&click) return 1;
    if (b2&&click) return 0;
    return 2;
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


// void print_noms_factionV2(){
//     SDL_Surface *tmp = SDL_LoadBMP("Cards/oriieflamme3.bmp");
//     if (NULL == tmp)
//     {
//         fprintf(stderr, "ERREUR LOADBMP");
//     }
//     SDL_Texture *image = SDL_CreateTextureFromSurface(renderer, tmp);
//     SDL_Rect dst = {775, -50, 600, 600};
//     SDL_RenderCopy(renderer, image, NULL, &dst);
// }




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


void print_pointsV2(faction f1, faction f2){
    int f1_pts = getNbRoundWin(f1);
    // int f2_pts = getNbRoundWin(f2);
    SDL_Surface *tmp = SDL_LoadBMP("Cards/score.bmp");
    if (NULL == tmp)
    {
        fprintf(stderr, "ERREUR LOADBMP");
    }
    SDL_Texture *image = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_Rect dst = {-125, 50, 350, 200};
    SDL_RenderCopy(renderer, image, NULL, &dst);
    print_score(123, 100, 50);
    SDL_RenderPresent(renderer);
    
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