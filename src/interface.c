#include "../headers/interface.h"
#include "../headers/faction.h"
#include "../headers/cards.h"
#include "../headers/board.h"
#include "../headers/structure.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_nom_faction(faction f){
    int i = 0;
    char fname[150];
    fname = getFactionName(f);
    while (fname[i] != "/0"){
        printf("%f", fname[i]);
        i++;
    }
}

void print_pts(faction f1, faction f2){
    int f1_pts = getFactionDdrsPoints(f1);
    int f2_pts = getFactionDdrsPoints(f2);
    

    printf("Scores:\n\n");
    printf("Faction");
    print_nom_faction(f1);
    printf(": %i\n", f1_pts)

    printf("Faction");
    print_nom_faction(f2);
    printf(": %i", f2_pts)
}

int askResuffle(faction f){
    char buffer[2];

    print_nom_faction(f);
    system("cls");
    printf("\nVoulez vous vider votre main, mélanger votre pioche et repiocher une main?\nVous ne pourrez effectuez cette action qu'une fois au cours de la partie.\n\n");
    printf("-Oui: [o]\n-Non: [n]\n\n");

    
    printf( "Veuillez saisir votre réponse : " );
    scanf( "%[^\n]", buffer );

    return(buffer[0] == "o");
}

void showWinner(f1,f2){
    int f2_pts = getFactionDdrsPoints(f2);

    if (f1_pts > f2_pts) {
        prinf("Félicitations à la faction ")
        print_nom_faction(f1)
        printf(" qui remporte la partie !\n\n")
        print_pts(f1, f2);
    }

    if (f1_pts < f2_pts) {
        prinf("Félicitations à la faction ")
        print_nom_faction(f1)
        printf(" qui remporte la partie !\n\n")
        print_pts(f1, f2);
    }

    if (f1_pts == f2_pts) {
        prinf("Egalité, bravo aux deux factions pour cette palpitante partie !\n\n");
        print_pts(f1, f2);
    }
   
}
